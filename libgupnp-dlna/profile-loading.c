/*
 * Copyright (C) 2010 Nokia Corporation.
 *
 * Authors: Arun Raghavan <arun.raghavan@collabora.co.uk>
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
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <glib.h>
#include <glib/gstdio.h>
#include <glib-object.h>
#include <libxml/xmlreader.h>
#include <libxml/relaxng.h>
#include <gst/pbutils/pbutils.h>
#include "profile-loading.h"
#include "gupnp-dlna-profile.h"
#include "gupnp-dlna-profile-private.h"

#define GST_CAPS_NULL_NAME "NULL"
#define DLNA_DATA_DIR DATA_DIR                              \
        G_DIR_SEPARATOR_S "dlna-profiles" G_DIR_SEPARATOR_S

static gboolean
copy_func (GQuark field_id, const GValue *value, gpointer data)
{
        GstStructure *st2 = (GstStructure *)data;

        if (!gst_structure_has_field (st2, g_quark_to_string (field_id)))
                gst_structure_id_set_value (st2, field_id, value);

        return TRUE;
}

/* Note: It is assumed that caps1 and caps2 have only 1 structure each */
static GstCaps *
merge_caps (GstCaps *caps1, GstCaps *caps2)
{
        GstStructure *st1, *st2;
        GstCaps *ret;
        gboolean any = FALSE;

        /* If one of the caps GST_CAPS_ANY, gst_caps_merge will result in a
         * GST_CAPS_ANY, which might not be correct for us */
        if (!gst_caps_is_any (caps1) && !gst_caps_is_any (caps2)) {
                any = TRUE;
                gst_caps_merge (caps1, gst_caps_copy (caps2));
                gst_caps_do_simplify (caps1);
        }

        ret = gst_caps_make_writable (caps1);
        st1 = gst_caps_get_structure (ret, 0);
        if (gst_caps_get_size (caps1) == 2)
                /* Non-merged fields were copied to a second structure in caps
                 * at gst_merge_caps() time */
                st2 = gst_caps_get_structure (ret, 1);
        else
                /* Either one of the caps was GST_CAPS_ANY, or there were no
                 * unmerged fields */
                st2 = gst_caps_get_structure (caps2, 0);

        /* If caps1 has a name, we retain it. If not, and caps2 does, caps1
         * gets caps2's name. */
        if ((g_strcmp0 (GST_CAPS_NULL_NAME,
                        gst_structure_get_name (st1)) == 0) &&
            (g_strcmp0 (GST_CAPS_NULL_NAME,
                        gst_structure_get_name (st2)) != 0)) {
                gst_structure_set_name (st1, gst_structure_get_name (st2));
        }

        /* We now walk over the structures and append any fields that are in
         * caps2 but not in caps1. */
        if (any || gst_caps_get_size (caps1) == 2)
                gst_structure_foreach (st2, copy_func, st1);

        if (gst_caps_get_size (caps1) == 2)
                gst_caps_remove_structure (ret, 1);

        return ret;
}

static xmlChar *
get_value (xmlTextReaderPtr reader)
{
        xmlChar *value = NULL, *curr;
        int ret = 1;

        curr = xmlTextReaderName (reader);

        /* This function may be called with reader pointing to a <field> or
         * the element just below a <field>. In the former case, we move the
         * cursor forward and then continue processing. */
        if (xmlStrEqual (curr, BAD_CAST ("field")))
                ret = xmlTextReaderRead (reader);
        xmlFree (curr);

        while (ret == 1) {
                xmlChar *tag;

                tag = xmlTextReaderName (reader);

                if (xmlTextReaderNodeType (reader) == 1 &&
                    xmlStrEqual (tag, BAD_CAST ("value"))) {
                        /* <value> */

                        /* Note: This assumes you won't have a comment in the
                         *       middle of your text */
                        do {
                                ret = xmlTextReaderRead (reader);
                        } while (ret == 1 &&
                                 xmlTextReaderNodeType (reader) != 3 &&
                                 xmlTextReaderNodeType (reader) != 15);

                        /* We're now at the real text between a <value> and a
                         * </value> */

                        if (xmlTextReaderNodeType (reader) == 3)
                                value = xmlTextReaderValue (reader);
                }

                if (xmlTextReaderNodeType (reader) == 15 &&
                    xmlStrEqual (tag, BAD_CAST ("value"))) {
                        /* </value> */
                        xmlFree (tag);

                        break;
                }

                xmlFree (tag);
                ret = xmlTextReaderRead (reader);
        }

        if (!value)
                g_warning ("Empty <value>s are illegal");

        return value;
}

static void
xml_str_free (xmlChar *str, gpointer unused)
{
        xmlFree (str);
}

static void
free_restrictions_struct (gpointer data, gpointer user_data)
{
        GUPnPDLNARestrictions *restr = (GUPnPDLNARestrictions *)data;
        if (restr) {
                if (restr->caps)
                        gst_caps_unref (restr->caps);

                g_free (restr);
        }
}

static void
process_range (xmlTextReaderPtr reader, GString *caps_str)
{
        xmlChar *min, *max;

        min = xmlTextReaderGetAttribute (reader, BAD_CAST ("min"));
        max = xmlTextReaderGetAttribute (reader, BAD_CAST ("max"));

        g_string_append_printf (caps_str, "[ %s, %s ]", min, max);

        xmlFree (min);
        xmlFree (max);
}

static int
process_field (xmlTextReaderPtr reader,
               GString          *caps_str,
               gboolean         relaxed_mode,
               gboolean         extended_mode)
{
        int ret;
        xmlChar *name;
        xmlChar *type;
        xmlChar *used;
        GList *values = NULL;
        gboolean done = FALSE, skip = FALSE;

        /*
         * Parse the 'used' attribute and figure out the mode we
         * need to follow.
         */
        used = xmlTextReaderGetAttribute (reader, BAD_CAST ("used"));
        if (used) {
                if ((relaxed_mode == FALSE) &&
                    xmlStrEqual (used, BAD_CAST ("in-relaxed"))) {
                        skip = TRUE;
                } else if ((relaxed_mode == TRUE) &&
                           (xmlStrEqual (used, BAD_CAST ("in-strict")))) {
                        skip = TRUE;
                }

                xmlFree (used);
        }

        name = xmlTextReaderGetAttribute (reader, BAD_CAST ("name"));
        type = xmlTextReaderGetAttribute (reader, BAD_CAST ("type"));

        /*
         * This function reads a <field> and appends it to caps_str in the
         * GstCaps-as-a-string format:
         *
         *   Single value: field = (type) value
         *   Multiple values: field = (type) { value1, value2, value3 }
         *   Range: field = (type) [ min, max ]
         */

        /* Fields are comma-separeted. The leading comma is okay for the first
         * field - we will be prepending the restriction name to this string */
        if (!skip)
                g_string_append_printf (caps_str, ", %s = (%s) ", name, type);

        xmlFree (name);
        xmlFree (type);

        ret = xmlTextReaderRead (reader);
        while (ret == 1 && !done) {
                xmlChar *tag;

                tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case 1:
                        if (skip)
                                break;

                        if (xmlStrEqual (tag, BAD_CAST ("range"))) {
                                /* <range> */
                                process_range (reader, caps_str);
                        } else if (xmlStrEqual (tag, BAD_CAST ("value"))) {
                                /* <value> */
                                xmlChar *value;

                                value = get_value (reader);

                                if (value)
                                        values = g_list_append (values, value);
                        }

                        break;

                case 15:
                        if (xmlStrEqual (tag, BAD_CAST ("field")))
                                /* </field> */
                                done = TRUE;

                        break;

                default:
                        break;
                }

                xmlFree (tag);
                ret = xmlTextReaderRead (reader);
        }

        if (skip)
                return ret;

        if (g_list_length (values) == 1)
                /* Single value */
                g_string_append_printf (caps_str,
                                        "%s",
                                        (xmlChar *) values->data);
        else if (g_list_length (values) > 1) {
                /* Multiple values */
                GList *tmp = values->next;
                g_string_append_printf (caps_str,
                                        "{ %s",
                                        (xmlChar *) values->data);

                do {
                        g_string_append_printf (caps_str,
                                                ", %s",
                                                (xmlChar *) tmp->data);
                } while ((tmp = tmp->next) != NULL);

                g_string_append_printf (caps_str, " }");
        }

        if (values) {
                g_list_foreach (values, (GFunc) xml_str_free, NULL);
                g_list_free (values);
        }

        return ret;
}

static GUPnPDLNARestrictions *
process_parent (xmlTextReaderPtr reader, GUPnPDLNALoadState *data)
{
        xmlChar *parent;
        xmlChar *used;
        GUPnPDLNARestrictions *restr = NULL;

        /*
         * Check to see if we need to follow any relaxed/strict mode
         * restrictions.
         */
        used = xmlTextReaderGetAttribute (reader, BAD_CAST ("used"));
        if (used) {
                if ((data->relaxed_mode == FALSE) &&
                    xmlStrEqual (used, BAD_CAST ("in-relaxed"))) {
                        xmlFree (used);
                        return NULL;
                } else if ((data->relaxed_mode == TRUE) &&
                           (xmlStrEqual (used, BAD_CAST ("in-strict")))) {
                        xmlFree (used);
                        return NULL;
                }
        }

        parent = xmlTextReaderGetAttribute (reader, BAD_CAST ("name"));
        restr = g_hash_table_lookup (data->restrictions, parent);

        if (!restr) {
                g_warning ("Could not find parent restriction: %s", parent);
                return NULL;
        }

        xmlFree (parent);
        xmlFree (used);

        return restr;
}

static GUPnPDLNARestrictions *
process_restriction (xmlTextReaderPtr reader, GUPnPDLNALoadState *data)
{
        GUPnPDLNARestrictions *restr = NULL;
        GType type;
        GstCaps *caps = NULL;
        GString *caps_str = g_string_sized_new (100);
        GList *parents = NULL, *tmp;
        xmlChar *id, *name = NULL, *restr_type, *used;
        int ret;
        gboolean done = FALSE, skip = FALSE;

        /*
         * First we parse the 'used' attribute and figure out
         * the mode we need to comply to.
         */
        used = xmlTextReaderGetAttribute (reader, BAD_CAST ("used"));
        if (used) {
                if ((data->relaxed_mode == FALSE) &&
                    xmlStrEqual (used, BAD_CAST ("in-relaxed"))) {
                        skip = TRUE;
                } else if ((data->relaxed_mode == TRUE) &&
                           (xmlStrEqual (used, BAD_CAST ("in-strict")))) {
                        skip = TRUE;
                }
        }

        /* We then walk through the fields in this restriction, and make a
         * string that can be parsed by gst_caps_from_string (). We then make
         * a GstCaps from this string */

        id = xmlTextReaderGetAttribute (reader, BAD_CAST ("id"));
        restr_type = xmlTextReaderGetAttribute (reader, BAD_CAST ("type"));

        ret = xmlTextReaderRead (reader);
        while (ret == 1 && !done) {
                xmlChar *tag;

                tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case 1:
                        if (skip)
                                break;

                        if (xmlStrEqual (tag, BAD_CAST ("field"))) {
                                /* <field> */
                                xmlChar *field;

                                field = xmlTextReaderGetAttribute
                                        (reader, BAD_CAST ("name"));

                                /* We handle the "name" field specially - if
                                 * present, it is the caps name */
                                if (xmlStrEqual (field, BAD_CAST ("name")))
                                        name = get_value (reader);
                                else
                                        process_field (reader,
                                                       caps_str,
                                                       data->relaxed_mode,
                                                       data->extended_mode);

                                xmlFree (field);
                        } else if (xmlStrEqual (tag, BAD_CAST ("parent"))) {
                                /* <parent> */
                                GUPnPDLNARestrictions *restr =
                                        process_parent (reader, data);

                                if (restr && restr->caps)
                                        /* Collect parents in a list - we'll
                                         * coalesce them later */
                                        parents = g_list_append (parents,
                                                                 gst_caps_copy
                                                                 (restr->caps));
                        }

                        break;

                case 15:
                        if (xmlStrEqual (tag, BAD_CAST ("restriction")))
                                /* </restriction> */
                                done = TRUE;

                        break;

                default:
                        break;
                }

                xmlFree (tag);
                ret = xmlTextReaderRead (reader);
        }

        if (skip)
                goto out;

        /* If the restriction doesn't have a name, we make it up */
        if (!name)
                name = BAD_CAST (g_strdup (GST_CAPS_NULL_NAME));
        g_string_prepend (caps_str, (gchar *) name);
        xmlFree (name);

        if (xmlStrEqual (restr_type, BAD_CAST ("container")))
                type = GST_TYPE_ENCODING_CONTAINER_PROFILE;
        else if (xmlStrEqual (restr_type, BAD_CAST ("audio")))
                type = GST_TYPE_ENCODING_AUDIO_PROFILE;
        else if (xmlStrEqual (restr_type, BAD_CAST ("video")))
                type = GST_TYPE_ENCODING_VIDEO_PROFILE;
        else if (xmlStrEqual (restr_type, BAD_CAST ("image")))
                type = GST_TYPE_ENCODING_VIDEO_PROFILE;
        else {
                g_warning ("Support for '%s' restrictions not yet implemented",
                           restr_type);
                goto out;
        }

        caps = gst_caps_from_string (caps_str->str);
        g_string_free (caps_str, TRUE);
        g_return_val_if_fail (caps != NULL, NULL);

        tmp = parents;
        while (tmp) {
                /* Merge all the parent caps. The child overrides parent
                 * attributes */
                GstCaps *tmp_caps = (GstCaps *)tmp->data;
                caps = merge_caps (caps, tmp_caps);
                gst_caps_unref (tmp_caps);
                tmp = tmp->next;
        }

        restr = g_new0 (GUPnPDLNARestrictions, 1);

        restr->caps = gst_caps_copy (caps);
        restr->type = type;

        if (id)
                g_hash_table_insert (data->restrictions, id, restr);

out:
        xmlFree (restr_type);
        if (used)
                xmlFree (used);
        if (caps)
                gst_caps_unref (caps);
        if (parents)
                g_list_free (parents);

        return restr;
}

static void
process_restrictions (xmlTextReaderPtr reader, GUPnPDLNALoadState *data)
{
        int ret = xmlTextReaderRead (reader);

        while (ret == 1) {
                xmlChar *tag;

                tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case 1:
                        if (xmlStrEqual (tag, BAD_CAST ("restriction"))) {
                                /* <restriction> */
                                process_restriction (reader, data);
                        }

                        break;

                case 15:
                        if (xmlStrEqual (tag, BAD_CAST ("restrictions"))) {
                                /* </restrictions> */
                                xmlFree (tag);
                                return;
                        }

                default:
                        break;
                }

                xmlFree (tag);
                ret = xmlTextReaderRead (reader);
        }
}

static void
process_dlna_profile (xmlTextReaderPtr   reader,
                      GList              **profiles,
                      GUPnPDLNALoadState *data)
{
        guint ret;
        GUPnPDLNAProfile *profile = NULL;
        GUPnPDLNAProfile  *base = NULL;
        GUPnPDLNARestrictions *restr = NULL;
        GstCaps *temp_audio = NULL, *temp_video = NULL, *temp_container = NULL;
        xmlChar *name, *mime, *id, *base_profile, *extended;
        gboolean done = FALSE, is_extended = FALSE;

        name = xmlTextReaderGetAttribute (reader, BAD_CAST ("name"));
        mime = xmlTextReaderGetAttribute (reader, BAD_CAST ("mime"));
        extended = xmlTextReaderGetAttribute (reader, BAD_CAST ("extended"));
        id = xmlTextReaderGetAttribute (reader, BAD_CAST ("id"));
        base_profile = xmlTextReaderGetAttribute (reader,
                                                  BAD_CAST ("base-profile"));

        /* Create temporary place-holders for caps */
        temp_container = gst_caps_new_empty ();
        temp_video = gst_caps_new_empty ();
        temp_audio = gst_caps_new_empty ();

        if (!name) {
                g_assert (mime == NULL);

                /* We need a non-NULL string to not trigger asserts in the
                 * places these are used. Profiles without names are used
                 * only for inheritance, not for actual matching. */
                name = xmlStrdup (BAD_CAST (""));
                mime = xmlStrdup (BAD_CAST (""));
        }

        if (extended && xmlStrEqual (extended, BAD_CAST ("true"))) {
                /* If we're not in extended mode, skip this profile */
                if (!data->extended_mode)
                        goto out;

                is_extended = TRUE;
        }

        ret = xmlTextReaderRead (reader);
        while (ret == 1 && !done) {
                xmlChar *tag;

                tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case 1:
                        if (xmlStrEqual (tag, BAD_CAST ("restriction")))
                                restr = process_restriction (reader, data);
                        else if (xmlStrEqual (tag, BAD_CAST ("parent")))
                                restr = process_parent (reader, data);

                        if (!restr)
                                break;

                        if (restr->type == GST_TYPE_ENCODING_CONTAINER_PROFILE)
                                gst_caps_merge (temp_container,
                                                gst_caps_copy (restr->caps));
                        else if (restr->type == GST_TYPE_ENCODING_VIDEO_PROFILE)
                                gst_caps_merge (temp_video,
                                                gst_caps_copy (restr->caps));
                        else if (restr->type == GST_TYPE_ENCODING_AUDIO_PROFILE)
                                gst_caps_merge (temp_audio,
                                                gst_caps_copy (restr->caps));
                        else
                                g_assert_not_reached ();

                        break;

                case 15:
                        if (xmlStrEqual (tag, BAD_CAST ("dlna-profile")))
                                done = TRUE;

                default:
                        break;
                }

                xmlFree (tag);
                ret = xmlTextReaderRead (reader);
        }

        if (base_profile) {
                base = g_hash_table_lookup (data->profile_ids, base_profile);
                if (!base)
                        g_warning ("Invalid base-profile reference");
        }


        /* create a new GUPnPDLNAProfile */
        profile = gupnp_dlna_profile_new ((gchar *)name,
                                          (gchar *)mime,
                                          GST_CAPS_NONE,
                                          GST_CAPS_NONE,
                                          GST_CAPS_NONE,
                                          is_extended);

        /* Inherit from base profile, if it exists*/
        if (base) {
                const GstCaps *video_caps =
                                       gupnp_dlna_profile_get_video_caps (base);
                const GstCaps *audio_caps =
                                       gupnp_dlna_profile_get_audio_caps (base);
                const GstCaps *container_caps =
                                       gupnp_dlna_profile_get_container_caps (base);

                if (GST_IS_CAPS (video_caps))
                        gst_caps_merge (temp_video,
                                        gst_caps_copy (video_caps));
                if (GST_IS_CAPS (audio_caps))
                        gst_caps_merge (temp_audio,
                                        gst_caps_copy (audio_caps));
                if (GST_IS_CAPS (container_caps))
                        gst_caps_merge (temp_container,
                                        gst_caps_copy (container_caps));

        }


        /* The merged caps will be our new GUPnPDLNAProfile */

        if (GST_IS_CAPS (temp_container) && !gst_caps_is_empty (temp_container))
                gupnp_dlna_profile_set_container_caps (profile, temp_container);
        if (GST_IS_CAPS (temp_video) && !gst_caps_is_empty (temp_video))
                gupnp_dlna_profile_set_video_caps (profile, temp_video);
        if (GST_IS_CAPS (temp_audio) && !gst_caps_is_empty (temp_audio))
                gupnp_dlna_profile_set_audio_caps (profile, temp_audio);

        *profiles = g_list_append (*profiles, profile);

        if (id) {
                /* id is freed when the hash table is destroyed */
                g_object_ref (profile);
                g_hash_table_insert (data->profile_ids, id, profile);
        }

out:

        if (temp_container)
                gst_caps_unref (temp_container);
        if (temp_audio)
                gst_caps_unref (temp_audio);
        if (temp_video)
                gst_caps_unref (temp_video);

        xmlFree (mime);
        xmlFree (name);
        if (extended)
                xmlFree (extended);
        if (base_profile)
                xmlFree (base_profile);
}

static GList *
process_include (xmlTextReaderPtr reader, GUPnPDLNALoadState *data)
{
        xmlChar *path;
        GList *ret;

        path = xmlTextReaderGetAttribute (reader, BAD_CAST ("ref"));

        if (!g_path_is_absolute ((gchar *) path)) {
                gchar *tmp = g_strconcat (DLNA_DATA_DIR,
                                          G_DIR_SEPARATOR_S,
                                          path,
                                          NULL);
                xmlFree (path);
                path = BAD_CAST (tmp);
        }

        ret = gupnp_dlna_load_profiles_from_file ((gchar *) path,
                                                  data);
        xmlFree (path);

        return ret;
}

/* This can go away once we have a glib function to canonicalize paths (see
 * https://bugzilla.gnome.org/show_bug.cgi?id=111848
 *
 * The implementationis not generic enough, but sufficient for our use. The
 * idea is taken from Tristan Van Berkom's comment in the bug mentioned above:
 *
 *   1. cd dirname(path)
 *   2. absdir = $CWD
 *   3. cd $OLDPWD
 *   4. abspath = absdir + basename(path)
 */
static gchar *
canonicalize_path_name (const char *path)
{
        gchar *dir_name = NULL, *file_name = NULL, *abs_dir = NULL,
              *old_dir = NULL, *ret = NULL;

        if (g_path_is_absolute (path))
                return g_strdup (path);

        old_dir = g_get_current_dir ();
        dir_name = g_path_get_dirname (path);

        if (g_chdir (dir_name) < 0) {
                ret = g_strdup (path);
                goto out;
        }

        abs_dir = g_get_current_dir ();
        g_chdir (old_dir);

        file_name = g_path_get_basename (path);
        ret = g_build_filename (abs_dir, file_name, NULL);

out:
        g_free (dir_name);
        g_free (file_name);
        g_free (abs_dir);
        g_free (old_dir);

        return ret;
}

GList *
gupnp_dlna_load_profiles_from_file (const char         *file_name,
                                    GUPnPDLNALoadState *data)
{
        GList *profiles = NULL;
        gchar *path = NULL;
        xmlTextReaderPtr reader;
        xmlRelaxNGParserCtxtPtr rngp;
        xmlRelaxNGPtr rngs;
        int ret;

        path = canonicalize_path_name (file_name);
        if (g_hash_table_lookup_extended (data->files_hash, path, NULL, NULL))
                goto out;
        else
                g_hash_table_insert (data->files_hash, g_strdup (path), NULL);

        reader = xmlNewTextReaderFilename (path);
        if (!reader)
                goto out;

        /* Load the schema for validation */
        rngp = xmlRelaxNGNewParserCtxt (DLNA_DATA_DIR "dlna-profiles.rng");
        rngs = xmlRelaxNGParse (rngp);
        xmlTextReaderRelaxNGSetSchema (reader, rngs);

        ret = xmlTextReaderRead (reader);
        while (ret == 1) {
                xmlChar *tag;

                tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                        /* Start tag */
                        case 1:
                                if (xmlStrEqual (tag, BAD_CAST ("include"))) {
                                        /* <include> */
                                        GList *include =
                                                process_include (reader,
                                                                 data);
                                        profiles = g_list_concat (profiles,
                                                                  include);
                                } else if (xmlStrEqual (tag,
                                        BAD_CAST ("restrictions"))) {
                                        /* <restrictions> */
                                        process_restrictions (reader,
                                                              data);
                                } else if (xmlStrEqual (tag,
                                        BAD_CAST ("dlna-profile"))) {
                                        /* <dlna-profile> */
                                        process_dlna_profile (reader,
                                                              &profiles,
                                                              data);

                                }

                                break;

                        default:
                                break;
                }

                xmlFree (tag);
                ret = xmlTextReaderRead (reader);
        }

        xmlFreeTextReader (reader);
        xmlRelaxNGFree (rngs);
        xmlRelaxNGFreeParserCtxt (rngp);

out:
        g_free (path);

        return profiles;
}

GList *
gupnp_dlna_load_profiles_from_dir (gchar *profile_dir, GUPnPDLNALoadState *data)
{
        GDir *dir;

        data->restrictions =
                g_hash_table_new_full (g_str_hash,
                                       g_str_equal,
                                       (GDestroyNotify) xmlFree,
                                       (GDestroyNotify)
                                       free_restrictions_struct);
        data->profile_ids =
                g_hash_table_new_full (g_str_hash,
                                       g_str_equal,
                                       (GDestroyNotify) xmlFree,
                                       (GDestroyNotify)
                                       g_object_unref);

        GList *profiles = NULL;

        if ((dir = g_dir_open (profile_dir, 0, NULL))) {
                const gchar *entry;

                while ((entry = g_dir_read_name (dir))) {
                        gchar *path = g_strconcat (profile_dir,
                                                   G_DIR_SEPARATOR_S,
                                                   entry,
                                                   NULL);

                        if (g_str_has_suffix (entry, ".xml") &&
                            g_file_test (path, G_FILE_TEST_IS_REGULAR)) {
                                profiles = g_list_concat (profiles,
                                        gupnp_dlna_load_profiles_from_file (
                                                path,
                                                data));
                        }

                        g_free (path);
                }

                g_dir_close (dir);
        }

        g_hash_table_unref (data->restrictions);
        g_hash_table_unref (data->profile_ids);

        return profiles;
}

GList *
gupnp_dlna_load_profiles_from_disk (gboolean relaxed_mode,
                                    gboolean extended_mode)
{
        GUPnPDLNALoadState *load_data;
        GList *ret, *i;

        load_data = g_new0 (GUPnPDLNALoadState, 1);

        load_data->files_hash = g_hash_table_new_full (g_str_hash,
                                                       g_str_equal,
                                                       g_free,
                                                       NULL);
        load_data->relaxed_mode = relaxed_mode;
        load_data->extended_mode = extended_mode;

        ret = gupnp_dlna_load_profiles_from_dir (DLNA_DATA_DIR,
                                                 load_data);

        /* Now that we're done loading profiles, remove all profiles with no
         * name which are only used for inheritance and not matching. */
        i = ret;
        while (i) {
                const gchar *name;
                GUPnPDLNAProfile *profile = i->data;
                GstEncodingProfile *enc_profile =
                                        gupnp_dlna_profile_get_encoding_profile
                                                  (profile);
                GList *tmp = g_list_next (i);

                name = gst_encoding_profile_get_name (enc_profile);
                if (name[0] == '\0') {
                        ret = g_list_delete_link (ret, i);
                        g_object_unref (profile);
                }

                i = tmp;
        }

        g_hash_table_unref (load_data->files_hash);
        g_free (load_data);
        load_data = NULL;

        return ret;
}
