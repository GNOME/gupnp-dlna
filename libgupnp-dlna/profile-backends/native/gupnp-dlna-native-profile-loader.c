/*
 * Copyright (C) 2010 Nokia Corporation.
 * Copyright (C) 2012 Intel Corporation.
 *
 * Authors: Arun Raghavan <arun.raghavan@collabora.co.uk>
 *          Krzesimir Nowak <krnowak@openismus.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H_ */
#include "gupnp-dlna-native-profile-loader.h"
#include "gupnp-dlna-native-profile.h"
#include "gupnp-dlna-native-sets.h"
#include "gupnp-dlna-native-utils.h"

G_DEFINE_TYPE (GUPnPDLNANativeProfileLoader,
               gupnp_dlna_native_profile_loader,
               GUPNP_TYPE_DLNA_PROFILE_LOADER)

typedef enum {
        GUPNP_DLNA_NATIVE_PARSED_ELEMENT_RESTRICTIONS,
        GUPNP_DLNA_NATIVE_PARSED_ELEMENT_RESTRICTION,
        GUPNP_DLNA_NATIVE_PARSED_ELEMENT_FIELD,
        GUPNP_DLNA_NATIVE_PARSED_ELEMENT_PARENT,
        GUPNP_DLNA_NATIVE_PARSED_ELEMENT_DLNA_PROFILE,
        GUPNP_DLNA_NATIVE_PARSED_ELEMENT_INVALID
} GUPnPDLNANativeParsedElement;

typedef enum {
        GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_AUDIO,
        GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_CONTAINER,
        GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_IMAGE,
        GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_VIDEO,
        GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_INVALID
} GUPnPDLNANativeRestrictionType;

typedef struct {
        GList *audios;
        GList *containers;
        GList *images;
        GList *videos;
} GUPnPDLNANativeProfileData;

typedef struct {
        gchar                    *name;
        GUPnPDLNANativeValueList *list;
} GUPnPDLNANativeNameValueListPair;

typedef struct {
        GList *name_list_pairs;
        GList *parents;
} GUPnPDLNANativeRestrictionData;

typedef struct {
        GUPnPDLNANativeRestriction     *restriction;
        GUPnPDLNANativeRestrictionType  type;
} GUPnPDLNANativeDescription;

struct _GUPnPDLNANativeProfileLoaderPrivate {
        GHashTable *descriptions;
        GList      *tags_stack;
        GList      *dlna_profile_data_stack;
        GList      *restriction_data_stack;
};

static GUPnPDLNANativeNameValueListPair *
gupnp_dlna_native_name_value_list_pair_new (const gchar              *name,
                                            GUPnPDLNANativeValueList *list)
{
        GUPnPDLNANativeNameValueListPair *pair =
                                 g_slice_new (GUPnPDLNANativeNameValueListPair);

        pair->name = g_strdup (name);
        pair->list = list;

        return pair;
}

static void
gupnp_dlna_native_name_value_list_pair_free
                                        (GUPnPDLNANativeNameValueListPair *pair)
{
        if (pair == NULL)
                return;
        g_free (pair->name);
        gupnp_dlna_native_value_list_free (pair->list);
        g_slice_free (GUPnPDLNANativeNameValueListPair, pair);
}

static GUPnPDLNANativeDescription *
gupnp_dlna_native_description_new (GUPnPDLNANativeRestriction     *restriction,
                                   GUPnPDLNANativeRestrictionType  type)
{
        GUPnPDLNANativeDescription *description =
                                       g_slice_new (GUPnPDLNANativeDescription);

        description->restriction = restriction;
        description->type = type;

        return description;
}

static void
gupnp_dlna_native_description_free (GUPnPDLNANativeDescription *description)
{
        if (description == NULL)
                return;

        gupnp_dlna_native_restriction_free (description->restriction);
        g_slice_free (GUPnPDLNANativeDescription, description);
}

static GUPnPDLNANativeProfileData*
gupnp_dlna_native_profile_data_new (void)
{
        GUPnPDLNANativeProfileData* data =
                                       g_slice_new (GUPnPDLNANativeProfileData);

        data->audios = NULL;
        data->containers = NULL;
        data->images = NULL;
        data->videos = NULL;

        return data;
}

static void
gupnp_dlna_native_profile_data_free (GUPnPDLNANativeProfileData *data)
{
        if (data == NULL)
                return;

        gupnp_dlna_native_utils_free_restrictions (data->audios);
        gupnp_dlna_native_utils_free_restrictions (data->containers);
        gupnp_dlna_native_utils_free_restrictions (data->images);
        gupnp_dlna_native_utils_free_restrictions (data->videos);
        g_slice_free (GUPnPDLNANativeProfileData, data);
}

static GUPnPDLNANativeRestrictionData *
gupnp_dlna_native_restriction_data_new (void)
{
        GUPnPDLNANativeRestrictionData* data =
                                   g_slice_new (GUPnPDLNANativeRestrictionData);

        data->name_list_pairs = NULL;
        data->parents = NULL;

        return data;
}

static void
gupnp_dlna_native_restriction_data_free (GUPnPDLNANativeRestrictionData *data)
{
        if (data == NULL)
                return;

        if (data->name_list_pairs != NULL)
                g_list_free_full
                 (data->name_list_pairs,
                  (GDestroyNotify) gupnp_dlna_native_name_value_list_pair_free);
        if (data->parents != NULL)
                g_list_free_full
                          (data->parents,
                           (GDestroyNotify) gupnp_dlna_native_restriction_free);
        g_slice_free (GUPnPDLNANativeRestrictionData, data);
}

static void
gupnp_dlna_native_profile_data_stack_free (GList* stack)
{
        g_list_free_full (stack,
                          (GDestroyNotify) gupnp_dlna_native_profile_data_free);
}

static void
gupnp_dlna_native_restriction_data_stack_free (GList* stack)
{
        g_list_free_full
                     (stack,
                      (GDestroyNotify) gupnp_dlna_native_restriction_data_free);
}

static void
push_tag (GUPnPDLNANativeProfileLoader *native_loader,
          GUPnPDLNANativeParsedElement  element)
{
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        gint raw_element = (gint) element;

        priv->tags_stack = g_list_prepend (priv->tags_stack,
                                           GINT_TO_POINTER (raw_element));
}

static void
pop_tag (GUPnPDLNANativeProfileLoader *native_loader)
{
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;

        priv->tags_stack = g_list_delete_link (priv->tags_stack,
                                               priv->tags_stack);
}

static GUPnPDLNANativeParsedElement
top_tag (GUPnPDLNANativeProfileLoader *native_loader)
{
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;

        if (priv->tags_stack != NULL) {
                gint top_raw = GPOINTER_TO_INT (priv->tags_stack->data);

                return (GUPnPDLNANativeParsedElement) top_raw;
        }

        return GUPNP_DLNA_NATIVE_PARSED_ELEMENT_INVALID;
}

static void
backend_pre_field (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);

        push_tag (native_loader, GUPNP_DLNA_NATIVE_PARSED_ELEMENT_FIELD);
}

static GUPnPDLNANativeValueType *
value_type_from_string (const gchar *type)
{
        if (!g_strcmp0 (type, "boolean"))
                return gupnp_dlna_native_value_type_bool ();
        else if (!g_strcmp0 (type, "float")) {
                g_warning ("'float' data type is not yet supported.");

                return NULL;
        } else if (!g_strcmp0 (type, "fourcc")) {
                g_warning ("'fourcc' data type is not yet supported.");

                return NULL;
        } else if (!g_strcmp0 (type, "fraction"))
                return gupnp_dlna_native_value_type_fraction ();
        else if (!g_strcmp0 (type, "int"))
                return gupnp_dlna_native_value_type_int ();
        else if (!g_strcmp0 (type, "string"))
                return gupnp_dlna_native_value_type_string ();
        g_critical ("Unknown value type: %s", type);

        return NULL;
}

static void
append_value_to_list (GUPnPDLNAFieldValue      *value,
                      GUPnPDLNANativeValueList *list)
{
        if (value == NULL)
                return;

        switch (value->type) {
        case GUPNP_DLNA_FIELD_VALUE_TYPE_RANGE:
                if (!gupnp_dlna_native_value_list_add_range
                                        (list,
                                         value->value.range.min,
                                         value->value.range.max))
                        g_warning ("Failed to add range value (%s, %s).",
                                   value->value.range.min,
                                   value->value.range.max);
                break;
        case GUPNP_DLNA_FIELD_VALUE_TYPE_SINGLE:
                if (!gupnp_dlna_native_value_list_add_single
                                        (list,
                                         value->value.single))
                        g_warning ("Failed to add single value (%s).",
                                   value->value.single);

                break;
        default:
                g_critical ("Unknown field value type: %d", (gint) value->type);
        }
}

static void
backend_post_field (GUPnPDLNAProfileLoader *loader,
                    const gchar            *name,
                    const gchar            *type,
                    GList                  *values)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv;
        GUPnPDLNANativeRestrictionData *restriction_data;
        GUPnPDLNANativeNameValueListPair *pair;
        GUPnPDLNANativeValueList *value_list;
        GUPnPDLNANativeValueType* value_type;
        GList *iter;

        pop_tag (native_loader);

        if (name == NULL || type == NULL)
                return;

        value_type = value_type_from_string (type);

        if (value_type == NULL)
                return;

        priv = native_loader->priv;
        restriction_data =
          (GUPnPDLNANativeRestrictionData *) priv->restriction_data_stack->data;
        value_list = gupnp_dlna_native_value_list_new (value_type);

        for (iter = values; iter != NULL; iter = iter->next) {
                GUPnPDLNAFieldValue *field_value =
                                        (GUPnPDLNAFieldValue *) iter->data;

                append_value_to_list (field_value, value_list);
        }

        pair = gupnp_dlna_native_name_value_list_pair_new (name, value_list);
        restriction_data->name_list_pairs = g_list_prepend
                                        (restriction_data->name_list_pairs,
                                         pair);
}

static void
backend_merge_restrictions (GUPnPDLNANativeProfileLoader *native_loader,
                            GUPnPDLNANativeDescription   *description)
{
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeProfileData* data =
             (GUPnPDLNANativeProfileData *) priv->dlna_profile_data_stack->data;
        GList **target_list;
        GUPnPDLNANativeRestriction *copy;

        if (description == NULL || description->restriction == NULL)
                return;

        switch (description->type) {
        case GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_AUDIO:
                target_list = &data->audios;

                break;
        case GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_CONTAINER:
                target_list = &data->containers;

                break;
        case GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_IMAGE:
                target_list = &data->images;

                break;
        case GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_VIDEO:
                target_list = &data->videos;

                break;
        default:
                g_assert_not_reached ();
        }

        copy = gupnp_dlna_native_restriction_copy (description->restriction);
        *target_list = g_list_prepend (*target_list, copy);
}

static void
backend_merge_restrictions_if_in_dlna_profile
                                   (GUPnPDLNANativeProfileLoader *native_loader,
                                    GUPnPDLNANativeDescription   *description)
{
        GUPnPDLNANativeParsedElement element = top_tag (native_loader);

        if (element == GUPNP_DLNA_NATIVE_PARSED_ELEMENT_DLNA_PROFILE)
                backend_merge_restrictions (native_loader, description);
}

static void
backend_collect_parents (GUPnPDLNANativeProfileLoader *native_loader,
                         GUPnPDLNANativeDescription   *description)
{
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeRestrictionData *data =
          (GUPnPDLNANativeRestrictionData *) priv->restriction_data_stack->data;

        if (description != NULL && description->restriction != NULL) {
                /* Collect parents in a list - we'll
                 * coalesce them later */
                GUPnPDLNANativeRestriction *copy =
                  gupnp_dlna_native_restriction_copy (description->restriction);

                data->parents = g_list_prepend (data->parents, copy);
        }
}

static void
backend_collect_parents_if_in_restriction
                                   (GUPnPDLNANativeProfileLoader *native_loader,
                                    GUPnPDLNANativeDescription   *description)
{
        GUPnPDLNANativeParsedElement element = top_tag (native_loader);

        if (element == GUPNP_DLNA_NATIVE_PARSED_ELEMENT_RESTRICTION)
                backend_collect_parents (native_loader, description);
}

static void
backend_pre_parent (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);

        push_tag (native_loader, GUPNP_DLNA_NATIVE_PARSED_ELEMENT_PARENT);
}

static void
backend_post_parent (GUPnPDLNAProfileLoader *loader,
                     const gchar            *parent)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;

        pop_tag (native_loader);

        if (parent != NULL) {
                GUPnPDLNANativeDescription *description = g_hash_table_lookup
                                        (priv->descriptions,
                                         parent);

                backend_merge_restrictions_if_in_dlna_profile (native_loader,
                                                               description);
                backend_collect_parents_if_in_restriction (native_loader,
                                                           description);
        }
}

static void
backend_pre_restriction (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeRestrictionData *data =
                                      gupnp_dlna_native_restriction_data_new ();

        push_tag (native_loader, GUPNP_DLNA_NATIVE_PARSED_ELEMENT_RESTRICTION);

        priv->restriction_data_stack = g_list_prepend
                                        (priv->restriction_data_stack,
                                         data);
}

static GUPnPDLNANativeRestrictionType
restriction_type_from_string (const gchar *type)
{
        if (!g_strcmp0 (type, "audio"))
                return GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_AUDIO;
        else if (!g_strcmp0 (type, "container"))
                return GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_CONTAINER;
        else if (!g_strcmp0 (type, "image"))
                return GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_IMAGE;
        else if (!g_strcmp0 (type, "video"))
                return GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_VIDEO;

        return GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_INVALID;
}

static void
backend_post_restriction (GUPnPDLNAProfileLoader *loader,
                          const gchar            *restriction_type,
                          const gchar            *id,
                          const gchar            *name)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeRestrictionData *data =
          (GUPnPDLNANativeRestrictionData *) priv->restriction_data_stack->data;
        GUPnPDLNANativeRestriction *restriction;
        GUPnPDLNANativeDescription *description;
        GUPnPDLNANativeRestrictionType type;
        GList *iter;

        pop_tag (native_loader);

        /* If this is NULL then it means that 'used' attribute was
           different from relaxed_mode setting. In this case we just
           ignore it.
         */
        if (restriction_type == NULL)
                goto out;

        restriction = gupnp_dlna_native_restriction_new (name);

        for (iter = data->name_list_pairs; iter != NULL; iter = iter->next) {
                GUPnPDLNANativeNameValueListPair *pair =
                                (GUPnPDLNANativeNameValueListPair *) iter->data;

                if (gupnp_dlna_native_restriction_add_value_list (restriction,
                                                                  pair->name,
                                                                  pair->list))
                        pair->list = NULL;
        }

        type = restriction_type_from_string (restriction_type);

        if (type == GUPNP_DLNA_NATIVE_RESTRICTION_TYPE_INVALID) {
                g_warning ("Support for '%s' restrictions not yet implemented.",
                           restriction_type);
                goto out;
        }

        iter = data->parents = g_list_reverse (data->parents);
        for (iter = data->parents; iter != NULL; iter = iter->next) {
                /* Merge all the parent caps. The child overrides parent
                 * attributes */
                GUPnPDLNANativeRestriction *parent =
                                      (GUPnPDLNANativeRestriction *) iter->data;

                gupnp_dlna_native_restriction_merge (restriction,
                                                     parent);
                iter->data = NULL;
        }

        description = gupnp_dlna_native_description_new (restriction, type);
        backend_merge_restrictions_if_in_dlna_profile (native_loader,
                                                       description);
        if (id != NULL)
                g_hash_table_replace (priv->descriptions,
                                      g_strdup (id),
                                      description);
        else
                gupnp_dlna_native_description_free (description);

 out:
        gupnp_dlna_native_restriction_data_free (data);
        priv->restriction_data_stack = g_list_delete_link
                                        (priv->restriction_data_stack,
                                         priv->restriction_data_stack);
}

static void
backend_pre_restrictions (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);

        push_tag (native_loader, GUPNP_DLNA_NATIVE_PARSED_ELEMENT_RESTRICTIONS);
}

static void
backend_post_restrictions (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);

        pop_tag (native_loader);
}

static void
backend_pre_dlna_profile (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeProfileData* data =
                                        gupnp_dlna_native_profile_data_new ();

        push_tag (native_loader, GUPNP_DLNA_NATIVE_PARSED_ELEMENT_DLNA_PROFILE);
        priv->dlna_profile_data_stack = g_list_prepend
                                        (priv->dlna_profile_data_stack,
                                         data);
}

static GList *
copy_restrictions_list (GList *list)
{
        GList *dup = NULL;
        GList *iter;

        for (iter = list; iter != NULL; iter = iter->next) {
                GUPnPDLNANativeRestriction *restriction =
                                     GUPNP_DLNA_NATIVE_RESTRICTION (iter->data);
                GUPnPDLNANativeRestriction *copy =
                               gupnp_dlna_native_restriction_copy (restriction);

                if (copy)
                        dup = g_list_prepend (dup, copy);
        }

        return dup;
}

static void
merge_base_restrictions (GUPnPDLNANativeProfileData *data,
                         GUPnPDLNANativeProfile     *profile)
{
        GList *audio_restrictions =
                     gupnp_dlna_native_profile_get_audio_restrictions (profile);
        GList *container_restrictions =
                 gupnp_dlna_native_profile_get_container_restrictions (profile);
        GList *image_restrictions =
                     gupnp_dlna_native_profile_get_image_restrictions (profile);
        GList *video_restrictions =
                     gupnp_dlna_native_profile_get_video_restrictions (profile);

        if (audio_restrictions != NULL) {
                GList *copy = copy_restrictions_list (audio_restrictions);

                data->audios = g_list_concat (copy, data->audios);
        }
        if (container_restrictions != NULL) {
                GList *copy = copy_restrictions_list (container_restrictions);

                data->containers = g_list_concat (copy, data->containers);
        }
        if (image_restrictions != NULL) {
                GList *copy = copy_restrictions_list (image_restrictions);

                data->images = g_list_concat (copy, data->images);
        }
        if (video_restrictions != NULL) {
                GList *copy = copy_restrictions_list (video_restrictions);

                data->videos = g_list_concat (copy, data->videos);
        }
}

static gboolean
restrictions_list_is_empty (GList *list)
{
        GList *iter;

        for (iter = list; iter != NULL; iter = iter->next) {
                GUPnPDLNANativeRestriction *restriction =
                                     GUPNP_DLNA_NATIVE_RESTRICTION (iter->data);

                if (restriction != NULL &&
                    !gupnp_dlna_native_restriction_is_empty (restriction))
                        return FALSE;
        }

        return TRUE;
}

static GUPnPDLNAProfile *
backend_create_profile (GUPnPDLNAProfileLoader *loader,
                        GUPnPDLNAProfile       *base,
                        const gchar            *name,
                        const gchar            *mime,
                        gboolean                extended)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeProfileData *data =
             (GUPnPDLNANativeProfileData *) priv->dlna_profile_data_stack->data;
        GUPnPDLNANativeProfile *profile;
        GList *audio_restrictions = NULL;
        GList *container_restrictions = NULL;
        GList *image_restrictions = NULL;
        GList *video_restrictions = NULL;

        /* Inherit from base profile, if it exists */
        if (GUPNP_IS_DLNA_NATIVE_PROFILE (base))
                merge_base_restrictions (data,
                                         GUPNP_DLNA_NATIVE_PROFILE (base));

        /* The merged caps will be our new GUPnPDLNAProfile */
        if (!restrictions_list_is_empty (data->audios)) {
                audio_restrictions = g_list_reverse (data->audios);
                data->audios = NULL;
        }
        if (!restrictions_list_is_empty (data->containers)) {
                container_restrictions = g_list_reverse (data->containers);
                data->containers = NULL;
        }
        if (!restrictions_list_is_empty (data->images)) {
                image_restrictions = g_list_reverse (data->images);
                data->images = NULL;
        }
        if (!restrictions_list_is_empty (data->videos)) {
                video_restrictions = g_list_reverse (data->videos);
                data->videos = NULL;
        }

        profile = gupnp_dlna_native_profile_new (name,
                                                 mime,
                                                 audio_restrictions,
                                                 container_restrictions,
                                                 image_restrictions,
                                                 video_restrictions,
                                                 extended);

        return GUPNP_DLNA_PROFILE (profile);
}

static void
backend_post_dlna_profile (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (loader);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GUPnPDLNANativeProfileData *data =
             (GUPnPDLNANativeProfileData *) priv->dlna_profile_data_stack->data;

        pop_tag (native_loader);
        gupnp_dlna_native_profile_data_free (data);
        priv->dlna_profile_data_stack = g_list_delete_link
                                        (priv->dlna_profile_data_stack,
                                         priv->dlna_profile_data_stack);
}

static GList *
backend_cleanup (GUPnPDLNAProfileLoader *loader G_GNUC_UNUSED,
                 GList *profiles)
{
        /* Now that we're done loading profiles, remove all profiles
         * with no name which are only used for inheritance and not
         * matching. */
        GList *iter = profiles;

        while (iter != NULL) {
                GList *next = iter->next;

                if (GUPNP_IS_DLNA_NATIVE_PROFILE (iter->data)) {
                        GUPnPDLNANativeProfile *profile =
                                        GUPNP_DLNA_NATIVE_PROFILE (iter->data);
                        const gchar *name = gupnp_dlna_profile_get_name
                                        (GUPNP_DLNA_PROFILE (profile));

                        if (name == NULL || name[0] == '\0') {
                                profiles = g_list_delete_link (profiles, iter);
                                g_object_unref (profile);
                        } else {
                                /* TODO: simplify restrictions in
                                 * profile if possible.
                                 */
                        }
                } else {
                        g_critical
                                  ("Profile in list is not from this backend.");
                }

                iter = next;
        }

        for (iter = profiles; iter != NULL; iter = iter->next) {
                GUPnPDLNANativeProfile *profile = GUPNP_DLNA_NATIVE_PROFILE
                                        (iter->data);
                GUPnPDLNAProfile *dlna_profile = GUPNP_DLNA_PROFILE (profile);
                gchar *acaps =
                             gupnp_dlna_native_utils_restrictions_list_to_string
                   (gupnp_dlna_native_profile_get_audio_restrictions (profile));
                gchar *ccaps =
                             gupnp_dlna_native_utils_restrictions_list_to_string
                           (gupnp_dlna_native_profile_get_container_restrictions
                                        (profile));
                gchar *icaps =
                             gupnp_dlna_native_utils_restrictions_list_to_string
                   (gupnp_dlna_native_profile_get_image_restrictions (profile));
                gchar *vcaps =
                             gupnp_dlna_native_utils_restrictions_list_to_string
                   (gupnp_dlna_native_profile_get_video_restrictions (profile));

                g_debug ("Loaded profile: %s\nMIME: %s\naudio caps: %s\n"
                         "container caps: %s\nimage caps: %s\nvideo caps: %s\n",
                         gupnp_dlna_profile_get_name (dlna_profile),
                         gupnp_dlna_profile_get_mime (dlna_profile),
                         acaps,
                         ccaps,
                         icaps,
                         vcaps);
                g_free (acaps);
                g_free (ccaps);
                g_free (icaps);
                g_free (vcaps);
        }

        return profiles;
}

static void
gupnp_dlna_native_profile_loader_dispose (GObject *object)
{
        GUPnPDLNANativeProfileLoader *native_loader =
                                      GUPNP_DLNA_NATIVE_PROFILE_LOADER (object);
        GUPnPDLNANativeProfileLoaderPrivate *priv = native_loader->priv;
        GObjectClass *parent =
                 G_OBJECT_CLASS (gupnp_dlna_native_profile_loader_parent_class);

        g_clear_pointer (&priv->descriptions, g_hash_table_unref);
        g_clear_pointer (&priv->tags_stack, g_list_free);
        g_clear_pointer (&priv->dlna_profile_data_stack,
                         gupnp_dlna_native_profile_data_stack_free);
        g_clear_pointer (&priv->restriction_data_stack,
                         gupnp_dlna_native_restriction_data_stack_free);

        parent->dispose (object);
}

static void
gupnp_dlna_native_profile_loader_class_init
                        (GUPnPDLNANativeProfileLoaderClass *native_loader_class)
{
        GUPnPDLNAProfileLoaderClass *loader_class =
                          GUPNP_DLNA_PROFILE_LOADER_CLASS (native_loader_class);
        GObjectClass *object_class = G_OBJECT_CLASS (native_loader_class);

        object_class->dispose = gupnp_dlna_native_profile_loader_dispose;
        loader_class->pre_field = backend_pre_field;
        loader_class->post_field = backend_post_field;
        loader_class->pre_parent = backend_pre_parent;
        loader_class->post_parent = backend_post_parent;
        loader_class->pre_restriction = backend_pre_restriction;
        loader_class->post_restriction = backend_post_restriction;
        loader_class->pre_restrictions = backend_pre_restrictions;
        loader_class->post_restrictions = backend_post_restrictions;
        loader_class->pre_dlna_profile = backend_pre_dlna_profile;
        loader_class->create_profile = backend_create_profile;
        loader_class->post_dlna_profile = backend_post_dlna_profile;
        loader_class->cleanup = backend_cleanup;

        g_type_class_add_private (native_loader_class,
                                  sizeof (GUPnPDLNANativeProfileLoaderPrivate));
}

static void
gupnp_dlna_native_profile_loader_init (GUPnPDLNANativeProfileLoader *self)
{
        GUPnPDLNANativeProfileLoaderPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (self,
                                         GUPNP_TYPE_DLNA_NATIVE_PROFILE_LOADER,
                                         GUPnPDLNANativeProfileLoaderPrivate);

        priv->descriptions = g_hash_table_new_full
                          (g_str_hash,
                           g_str_equal,
                           g_free,
                           (GDestroyNotify) gupnp_dlna_native_description_free);
        priv->tags_stack = NULL;
        priv->dlna_profile_data_stack = NULL;
        priv->restriction_data_stack = NULL;
        self->priv = priv;
}

GUPnPDLNANativeProfileLoader *
gupnp_dlna_native_profile_loader_new (gboolean relaxed_mode,
                                      gboolean extended_mode)
{
        return GUPNP_DLNA_NATIVE_PROFILE_LOADER (g_object_new
                                        (GUPNP_TYPE_DLNA_NATIVE_PROFILE_LOADER,
                                         "relaxed-mode", relaxed_mode,
                                         "extended-mode", extended_mode,
                                         NULL));
}
