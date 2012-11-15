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

#include <glib/gstdio.h>
#include <libxml/xmlreader.h>
#include <libxml/relaxng.h>
#include "gupnp-dlna-profile-loader.h"
#include "gupnp-dlna-profile-backend.h"

G_DEFINE_ABSTRACT_TYPE (GUPnPDLNAProfileLoader,
                        gupnp_dlna_profile_loader,
                        G_TYPE_OBJECT)

#define DLNA_DATA_DIR DATA_DIR G_DIR_SEPARATOR_S "dlna-profiles"
#define NODE_TYPE_ELEMENT_START 1
#define NODE_TYPE_TEXT 3
#define NODE_TYPE_ELEMENT_END 15

enum {
        PROP_0,

        PROP_RELAXED_MODE,
        PROP_EXTENDED_MODE
};

struct _GUPnPDLNAProfileLoaderPrivate {
        GHashTable *restrictions;
        GHashTable *profile_ids;
        GHashTable *files_hash;
        gboolean    relaxed_mode;
        gboolean    extended_mode;
};

static void
gupnp_dlna_profile_loader_run_pre_field (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->pre_field != NULL);

        loader_class->pre_field (loader);
}

static void
gupnp_dlna_profile_loader_run_post_field (GUPnPDLNAProfileLoader *loader,
                                          gchar                  *name,
                                          gchar                  *type,
                                          GList                  *values)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->post_field != NULL);

        loader_class->post_field (loader,
                                  name,
                                  type,
                                  values);
}

static void
gupnp_dlna_profile_loader_run_pre_parent (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->pre_parent != NULL);

        loader_class->pre_parent (loader);
}

static void
gupnp_dlna_profile_loader_run_post_parent (GUPnPDLNAProfileLoader *loader,
                                           gchar *parent)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->post_parent != NULL);

        loader_class->post_parent (loader, parent);
}

static void
gupnp_dlna_profile_loader_run_pre_restriction (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->pre_restriction != NULL);

        loader_class->pre_restriction (loader);
}

static void
gupnp_dlna_profile_loader_run_post_restriction
                                      (GUPnPDLNAProfileLoader *loader,
                                       gchar                  *restriction_type,
                                       gchar                  *id,
                                       gchar                  *name)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->post_restriction != NULL);

        loader_class->post_restriction (loader,
                                        restriction_type,
                                        id,
                                        name);
}

static void
gupnp_dlna_profile_loader_run_pre_restrictions (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->pre_restrictions != NULL);

        loader_class->pre_restrictions (loader);
}

static void
gupnp_dlna_profile_loader_run_post_restrictions (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->post_restrictions != NULL);

        loader_class->post_restrictions (loader);
}

static void
gupnp_dlna_profile_loader_run_pre_dlna_profile (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->pre_dlna_profile != NULL);

        loader_class->pre_dlna_profile (loader);
}

static GUPnPDLNAProfile *
gupnp_dlna_profile_loader_run_create_profile (GUPnPDLNAProfileLoader *loader,
                                              GUPnPDLNAProfile       *base,
                                              gchar                  *name,
                                              gchar                  *mime,
                                              gboolean                extended)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader), NULL);

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class),
                              NULL);
        g_return_val_if_fail (loader_class->create_profile != NULL, NULL);

        return loader_class->create_profile (loader,
                                             base,
                                             name,
                                             mime,
                                             extended);
}

static void
gupnp_dlna_profile_loader_run_post_dlna_profile (GUPnPDLNAProfileLoader *loader)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader));

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class));
        g_return_if_fail (loader_class->post_dlna_profile != NULL);

        loader_class->post_dlna_profile (loader);
}

static GList *
gupnp_dlna_profile_loader_run_cleanup (GUPnPDLNAProfileLoader *loader,
                                       GList *profiles)
{
        GUPnPDLNAProfileLoaderClass *loader_class;

        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader), NULL);

        loader_class = GUPNP_DLNA_PROFILE_LOADER_GET_CLASS (loader);

        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER_CLASS (loader_class),
                              NULL);
        g_return_val_if_fail (loader_class->cleanup != NULL, NULL);

        return loader_class->cleanup (loader,
                                      profiles);
}

static void
gupnp_dlna_profile_loader_get_property (GObject    *object,
                                        guint       prop_id,
                                        GValue     *value,
                                        GParamSpec *pspec)
{
        GUPnPDLNAProfileLoader *loader = GUPNP_DLNA_PROFILE_LOADER (object);
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        switch (prop_id) {
        case PROP_RELAXED_MODE:
                g_value_set_boolean (value, priv->relaxed_mode);
                break;

        case PROP_EXTENDED_MODE:
                g_value_set_boolean (value, priv->extended_mode);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gupnp_dlna_profile_loader_set_property (GObject      *object,
                                        guint         prop_id,
                                        const GValue *value,
                                        GParamSpec   *pspec)
{
        GUPnPDLNAProfileLoader *loader = GUPNP_DLNA_PROFILE_LOADER (object);
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        switch (prop_id) {
        case PROP_RELAXED_MODE:
                priv->relaxed_mode = g_value_get_boolean (value);
                break;

        case PROP_EXTENDED_MODE:
                priv->extended_mode = g_value_get_boolean (value);
                break;

        default:
                G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gupnp_dlna_profile_loader_dispose (GObject *object)
{
        GUPnPDLNAProfileLoader *loader = GUPNP_DLNA_PROFILE_LOADER (object);
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        g_clear_pointer (&priv->restrictions, g_hash_table_unref);
        g_clear_pointer (&priv->profile_ids, g_hash_table_unref);
        g_clear_pointer (&priv->files_hash, g_hash_table_unref);

        G_OBJECT_CLASS (gupnp_dlna_profile_loader_parent_class)->dispose
                                        (object);
}

static void
gupnp_dlna_profile_loader_class_init
                              (GUPnPDLNAProfileLoaderClass *loader_class)
{
        GObjectClass *object_class = G_OBJECT_CLASS (loader_class);
        GParamSpec *spec;

        object_class->get_property = gupnp_dlna_profile_loader_get_property;
        object_class->set_property = gupnp_dlna_profile_loader_set_property;
        object_class->dispose = gupnp_dlna_profile_loader_dispose;
        loader_class->pre_field = NULL;
        loader_class->post_field = NULL;
        loader_class->pre_parent = NULL;
        loader_class->post_parent = NULL;
        loader_class->pre_restriction = NULL;
        loader_class->post_restriction = NULL;
        loader_class->pre_restrictions = NULL;
        loader_class->post_restrictions = NULL;
        loader_class->pre_dlna_profile = NULL;
        loader_class->create_profile = NULL;
        loader_class->post_dlna_profile = NULL;
        loader_class->cleanup = NULL;

        spec = g_param_spec_boolean ("relaxed-mode",
                                     "Relaxed mode",
                                     "Whether loader loads relaxed profiles",
                                     TRUE,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY |
                                     G_PARAM_STATIC_STRINGS);

        g_object_class_install_property (object_class,
                                         PROP_RELAXED_MODE,
                                         spec);

        spec = g_param_spec_boolean ("extended-mode",
                                     "Extended mode",
                                     "Whether loader loads extended profiles",
                                     TRUE,
                                     G_PARAM_READWRITE |
                                     G_PARAM_CONSTRUCT_ONLY |
                                     G_PARAM_STATIC_STRINGS);

        g_object_class_install_property (object_class,
                                         PROP_EXTENDED_MODE,
                                         spec);

        g_type_class_add_private (loader_class,
                                  sizeof (GUPnPDLNAProfileLoaderPrivate));
}

static void
gupnp_dlna_profile_loader_init (GUPnPDLNAProfileLoader *self)
{
        GUPnPDLNAProfileLoaderPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE
                                        (self,
                                         GUPNP_TYPE_DLNA_PROFILE_LOADER,
                                         GUPnPDLNAProfileLoaderPrivate);

        priv->restrictions = g_hash_table_new_full (g_str_hash,
                                                    g_str_equal,
                                                    g_free,
                                                    NULL);
        priv->profile_ids = g_hash_table_new_full (g_str_hash,
                                                   g_str_equal,
                                                   g_free,
                                                   g_object_unref);
        priv->files_hash = g_hash_table_new_full (g_str_hash,
                                                  g_str_equal,
                                                  g_free,
                                                  NULL);
        self->priv = priv;
}

static GUPnPDLNAFieldValue *
get_value (xmlTextReaderPtr reader)
{
        GUPnPDLNAFieldValue *value = NULL;
        xmlChar *curr = xmlTextReaderName (reader);
        int ret = 1;

        /* This function may be called with reader pointing to a <field> or
         * the element just below a <field>. In the former case, we move the
         * cursor forward and then continue processing. */
        if (xmlStrEqual (curr, BAD_CAST ("field")))
                ret = xmlTextReaderRead (reader);
        xmlFree (curr);

        while (ret == 1) {
                xmlChar *tag = xmlTextReaderName (reader);

                if (xmlTextReaderNodeType (reader) == NODE_TYPE_ELEMENT_START &&
                    xmlStrEqual (tag, BAD_CAST ("value"))) {
                        /* <value> */
                        gint node_type;

                        /* Note: This assumes you won't have a comment in the
                         *       middle of your text */
                        do {
                                ret = xmlTextReaderRead (reader);
                                node_type = xmlTextReaderNodeType (reader);
                        } while (ret == 1 &&
                                 node_type != NODE_TYPE_TEXT &&
                                 node_type != NODE_TYPE_ELEMENT_END);

                        /* We're now at the real text between a <value> and a
                         * </value> */

                        if (node_type == NODE_TYPE_TEXT) {
                                xmlChar *string = xmlTextReaderValue (reader);

                                value = gupnp_dlna_field_value_new_single
                                                            ((gchar *) string);
                                if (string)
                                        xmlFree (string);
                        }
                }

                if (xmlTextReaderNodeType (reader) == NODE_TYPE_ELEMENT_END &&
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

static GUPnPDLNAFieldValue *
get_range (xmlTextReaderPtr reader)
{
        xmlChar *min = xmlTextReaderGetAttribute (reader, BAD_CAST ("min"));
        xmlChar *max = xmlTextReaderGetAttribute (reader, BAD_CAST ("max"));
        GUPnPDLNAFieldValue *value = gupnp_dlna_field_value_new_range
                                        ((gchar *) min,
                                         (gchar *) max);

        if (min)
                xmlFree (min);
        if (max)
                xmlFree (max);

        return value;
}

static void
process_field (GUPnPDLNAProfileLoader *loader,
               xmlTextReaderPtr        reader)
{
        xmlChar *name = NULL;
        xmlChar *type = NULL;
        xmlChar *used;
        GList *values = NULL;
        gboolean done = FALSE;
        gboolean skip = FALSE;
        GUPnPDLNAFieldValue *value = NULL;
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        gupnp_dlna_profile_loader_run_pre_field (loader);

        /*
         * Parse the 'used' attribute and figure out the mode we
         * need to follow.
         */
        used = xmlTextReaderGetAttribute (reader, BAD_CAST ("used"));
        if (used) {
                if ((priv->relaxed_mode == FALSE) &&
                    xmlStrEqual (used, BAD_CAST ("in-relaxed"))) {
                        skip = TRUE;
                } else if ((priv->relaxed_mode == TRUE) &&
                           (xmlStrEqual (used, BAD_CAST ("in-strict")))) {
                        skip = TRUE;
                }

                xmlFree (used);
        }

        if (!skip) {
                name = xmlTextReaderGetAttribute (reader, BAD_CAST ("name"));
                type = xmlTextReaderGetAttribute (reader, BAD_CAST ("type"));
        }

        /* I don't like it - we should check done first, then try to
         * read next tag. But this is how it was done in original
         * code, so I am not touching it for now - krnowak.
         */
        while ((xmlTextReaderRead (reader) == 1) && !done) {
                xmlChar *tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case NODE_TYPE_ELEMENT_START:
                        if (skip)
                                break;

                        if (xmlStrEqual (tag, BAD_CAST ("range"))) {
                                /* <range> */
                                value = get_range (reader);
                        } else if (xmlStrEqual (tag, BAD_CAST ("value"))) {
                                /* <value> */
                                value = get_value (reader);
                        }

                        if (value)
                                values = g_list_prepend (values, value);
                        break;

                case NODE_TYPE_ELEMENT_END:
                        if (xmlStrEqual (tag, BAD_CAST ("field")))
                                /* </field> */
                                done = TRUE;

                        break;

                default:
                        break;
                }

                xmlFree (tag);
        }

        if (values)
                values = g_list_reverse (values);

        gupnp_dlna_profile_loader_run_post_field (loader,
                                                  (gchar *)name,
                                                  (gchar *)type,
                                                  values);

        if (name)
                xmlFree (name);
        if (type)
                xmlFree (type);
        if (values) {
                g_list_free_full (values,
                                  (GDestroyNotify) gupnp_dlna_field_value_free);
        }
}

static void
process_parent (GUPnPDLNAProfileLoader *loader,
                xmlTextReaderPtr        reader)
{
        xmlChar *parent = NULL;
        xmlChar *used = NULL;
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        gupnp_dlna_profile_loader_run_pre_parent (loader);

        /*
         * Check to see if we need to follow any relaxed/strict mode
         * restrictions.
         */
        used = xmlTextReaderGetAttribute (reader, BAD_CAST ("used"));
        if (used) {
                gboolean go_out = FALSE;

                if ((priv->relaxed_mode == FALSE) &&
                    xmlStrEqual (used, BAD_CAST ("in-relaxed"))) {
                        go_out = TRUE;
                } else if ((priv->relaxed_mode == TRUE) &&
                           (xmlStrEqual (used, BAD_CAST ("in-strict")))) {
                        go_out = TRUE;
                }
                xmlFree (used);
                if (go_out)
                        goto out;
        }

        parent = xmlTextReaderGetAttribute (reader, BAD_CAST ("name"));

        if (!g_hash_table_contains (priv->restrictions, parent))
                g_warning ("Could not find parent restriction: %s", parent);

 out:
        gupnp_dlna_profile_loader_run_post_parent (loader, (gchar*) parent);

        if (parent)
                xmlFree (parent);
}

static void
process_restriction (GUPnPDLNAProfileLoader *loader,
                     xmlTextReaderPtr        reader)
{
        xmlChar *used;
        gboolean done = FALSE;
        gboolean skip = FALSE;
        xmlChar *id = NULL;
        xmlChar *type = NULL;
        gchar *name = NULL;
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        gupnp_dlna_profile_loader_run_pre_restriction (loader);
        /*
         * First we parse the 'used' attribute and figure out
         * the mode we need to comply to.
         */
        used = xmlTextReaderGetAttribute (reader, BAD_CAST ("used"));
        if (used) {
                if ((priv->relaxed_mode == FALSE) &&
                    xmlStrEqual (used, BAD_CAST ("in-relaxed"))) {
                        skip = TRUE;
                } else if ((priv->relaxed_mode == TRUE) &&
                           (xmlStrEqual (used, BAD_CAST ("in-strict")))) {
                        skip = TRUE;
                }
                xmlFree (used);
        }

        if (!skip) {
                id = xmlTextReaderGetAttribute (reader, BAD_CAST ("id"));
                type = xmlTextReaderGetAttribute (reader, BAD_CAST ("type"));
        }

        /* I don't like it - we should check done first, then try to
         * read next tag. But this is how it was done in original
         * code, so I am not touching it for now - krnowak.
         */
        while ((xmlTextReaderRead (reader) == 1) && !done) {
                xmlChar *tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case NODE_TYPE_ELEMENT_START:
                        if (skip)
                                break;

                        if (xmlStrEqual (tag, BAD_CAST ("field"))) {
                                /* <field> */
                                xmlChar *field;

                                field = xmlTextReaderGetAttribute
                                        (reader,
                                         BAD_CAST ("name"));

                                if (xmlStrEqual (field, BAD_CAST ("name"))) {
                                        GUPnPDLNAFieldValue *value = get_value
                                                (reader);

                                        if (value) {
                                                /* get_value returns
                                                   single type
                                                   value. */
                                                name =
                                                 g_strdup (value->value.single);
                                                gupnp_dlna_field_value_free
                                                        (value);
                                        }
                                } else
                                        process_field (loader, reader);

                                xmlFree (field);
                        } else if (xmlStrEqual (tag, BAD_CAST ("parent"))) {
                                /* <parent> */
                                process_parent (loader, reader);
                        }

                        break;

                case NODE_TYPE_ELEMENT_END:
                        if (xmlStrEqual (tag, BAD_CAST ("restriction")))
                                /* </restriction> */
                                done = TRUE;

                        break;

                default:
                        break;
                }

                xmlFree (tag);
        }

        if (skip)
                goto out;

        if (id)
                g_hash_table_add (priv->restrictions, g_strdup ((gchar *) id));

out:
        gupnp_dlna_profile_loader_run_post_restriction (loader,
                                                        (gchar *) type,
                                                        (gchar *) id,
                                                        name);
        if (type)
                xmlFree (type);
        if (id)
                xmlFree (id);
        g_free (name);
}

static void
process_restrictions (GUPnPDLNAProfileLoader *loader,
                      xmlTextReaderPtr        reader)
{
        gboolean done = FALSE;

        gupnp_dlna_profile_loader_run_pre_restrictions (loader);

        while (!done && xmlTextReaderRead (reader) == 1) {
                xmlChar *tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case NODE_TYPE_ELEMENT_START:
                        if (xmlStrEqual (tag, BAD_CAST ("restriction"))) {
                                /* <restriction> */
                                process_restriction (loader, reader);
                        }

                        break;

                case NODE_TYPE_ELEMENT_END:
                        if (xmlStrEqual (tag, BAD_CAST ("restrictions")))
                                /* </restrictions> */
                                done = TRUE;

                default:
                        break;
                }

                xmlFree (tag);
        }

        gupnp_dlna_profile_loader_run_post_restrictions (loader);
}

static void
process_dlna_profile (GUPnPDLNAProfileLoader  *loader,
                      xmlTextReaderPtr         reader,
                      GList                  **profiles)
{
        GUPnPDLNAProfile *profile = NULL;
        GUPnPDLNAProfile *base = NULL;
        xmlChar *name;
        xmlChar *mime;
        xmlChar *id;
        xmlChar *base_profile;
        xmlChar *extended;
        gboolean done = FALSE;
        gboolean is_extended = FALSE;
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        gupnp_dlna_profile_loader_run_pre_dlna_profile (loader);

        name = xmlTextReaderGetAttribute (reader, BAD_CAST ("name"));
        mime = xmlTextReaderGetAttribute (reader, BAD_CAST ("mime"));
        extended = xmlTextReaderGetAttribute (reader, BAD_CAST ("extended"));
        id = xmlTextReaderGetAttribute (reader, BAD_CAST ("id"));
        base_profile = xmlTextReaderGetAttribute (reader,
                                                  BAD_CAST ("base-profile"));

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
                if (!priv->extended_mode)
                        goto out;

                is_extended = TRUE;
        }

        /* I don't like it - we should check done first, then try to
         * read next tag. But this is how it was done in original
         * code, so I am not touching it for now - krnowak.
         */
        while ((xmlTextReaderRead (reader) == 1) && !done) {
                xmlChar *tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                case NODE_TYPE_ELEMENT_START:
                        if (xmlStrEqual (tag, BAD_CAST ("restriction")))
                                process_restriction (loader, reader);
                        else if (xmlStrEqual (tag, BAD_CAST ("parent")))
                                process_parent (loader, reader);
                        break;

                case NODE_TYPE_ELEMENT_END:
                        if (xmlStrEqual (tag, BAD_CAST ("dlna-profile")))
                                done = TRUE;

                default:
                        break;
                }

                xmlFree (tag);
        }

        if (base_profile) {
                base = g_hash_table_lookup (priv->profile_ids, base_profile);
                if (!base)
                        g_warning ("Invalid base-profile reference");
        }


        profile = gupnp_dlna_profile_loader_run_create_profile (loader,
                                                                base,
                                                                (gchar *) name,
                                                                (gchar *) mime,
                                                                is_extended);

        *profiles = g_list_prepend (*profiles, profile);

        if (id) {
                g_hash_table_replace (priv->profile_ids,
                                      g_strdup ((gchar *) id),
                                      g_object_ref (profile));
        }

out:
        gupnp_dlna_profile_loader_run_post_dlna_profile (loader);

        if (id)
                xmlFree (id);
        if (mime)
                xmlFree (mime);
        if (name)
                xmlFree (name);
        if (extended)
                xmlFree (extended);
        if (base_profile)
                xmlFree (base_profile);
}

static void
gupnp_dlna_profile_loader_get_from_file (GUPnPDLNAProfileLoader  *loader,
                                         const char              *file_name,
                                         GList                  **profiles);

static void
process_include (GUPnPDLNAProfileLoader  *loader,
                 xmlTextReaderPtr         reader,
                 GList                  **profiles)
{
        xmlChar *path;
        gchar *g_path;

        path = xmlTextReaderGetAttribute (reader, BAD_CAST ("ref"));
        g_path = g_strdup ((gchar *) path);
        if (path)
                xmlFree (path);

        if (!g_path_is_absolute (g_path)) {
                gchar *tmp = g_build_filename (DLNA_DATA_DIR,
                                               g_path,
                                               NULL);
                g_free (g_path);
                g_path = tmp;
        }

        gupnp_dlna_profile_loader_get_from_file (loader, g_path, profiles);
        g_free (g_path);
}

GUPnPDLNAProfileLoader *
gupnp_dlna_profile_loader_get_default (gboolean relaxed_mode,
                                       gboolean extended_mode)
{
        return gupnp_dlna_profile_backend_get_loader (relaxed_mode,
                                                      extended_mode);
}

/* This can go away once we have a glib function to canonicalize paths (see
 * https://bugzilla.gnome.org/show_bug.cgi?id=111848
 *
 * The implementation is not generic enough, but sufficient for our use. The
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

static void
gupnp_dlna_profile_loader_get_from_file (GUPnPDLNAProfileLoader  *loader,
                                         const char              *file_name,
                                         GList                  **profiles)
{
        gchar *path = canonicalize_path_name (file_name);
        xmlTextReaderPtr reader = NULL;
        xmlRelaxNGParserCtxtPtr rngp = NULL;
        xmlRelaxNGPtr rngs = NULL;
        GUPnPDLNAProfileLoaderPrivate *priv = loader->priv;

        if (g_hash_table_contains (priv->files_hash, path))
                goto out;
        else
                g_hash_table_add (priv->files_hash, g_strdup (path));

        reader = xmlNewTextReaderFilename (path);
        if (!reader)
                goto out;

        /* Load the schema for validation */
        rngp = xmlRelaxNGNewParserCtxt (DLNA_DATA_DIR
                                        G_DIR_SEPARATOR_S
                                        "dlna-profiles.rng");
        if (!rngp)
                goto out;
        rngs = xmlRelaxNGParse (rngp);
        if (!rngs)
                goto out;
        if (xmlTextReaderRelaxNGSetSchema (reader, rngs) < 0)
                goto out;

        while (xmlTextReaderRead (reader) == 1) {
                xmlChar *tag = xmlTextReaderName (reader);

                switch (xmlTextReaderNodeType (reader)) {
                        case NODE_TYPE_ELEMENT_START:
                                if (xmlStrEqual (tag, BAD_CAST ("include"))) {
                                        /* <include> */
                                        process_include (loader,
                                                         reader,
                                                         profiles);
                                } else if (xmlStrEqual (tag,
                                        BAD_CAST ("restrictions"))) {
                                        /* <restrictions> */
                                        process_restrictions (loader,
                                                              reader);
                                } else if (xmlStrEqual (tag,
                                        BAD_CAST ("dlna-profile"))) {
                                        /* <dlna-profile> */
                                        process_dlna_profile (loader,
                                                              reader,
                                                              profiles);

                                }

                                break;

                        default:
                                break;
                }

                xmlFree (tag);
        }


out:
        g_free (path);
        if (reader)
                xmlFreeTextReader (reader);
        if (rngs)
                xmlRelaxNGFree (rngs);
        if (rngp)
                xmlRelaxNGFreeParserCtxt (rngp);
}

static GList *
gupnp_dlna_profile_loader_get_from_dir (GUPnPDLNAProfileLoader *loader,
                                        gchar                  *profile_dir)
{
        GDir *dir;
        GList *profiles = NULL;

        if ((dir = g_dir_open (profile_dir, 0, NULL))) {
                const gchar *entry;

                while ((entry = g_dir_read_name (dir))) {
                        gchar *path = g_build_filename (profile_dir,
                                                        entry,
                                                        NULL);

                        if (g_str_has_suffix (entry, ".xml") &&
                            g_file_test (path, G_FILE_TEST_IS_REGULAR)) {
                                gupnp_dlna_profile_loader_get_from_file
                                        (loader,
                                         path,
                                         &profiles);
                        }

                        g_free (path);
                }

                g_dir_close (dir);
        }

        return profiles;
}

GList *
gupnp_dlna_profile_loader_get_from_disk (GUPnPDLNAProfileLoader *loader)
{
        GList *profiles;

        g_return_val_if_fail (GUPNP_IS_DLNA_PROFILE_LOADER (loader), NULL);

        profiles = gupnp_dlna_profile_loader_get_from_dir (loader,
                                                           DLNA_DATA_DIR);

        profiles = g_list_reverse (profiles);

        return gupnp_dlna_profile_loader_run_cleanup (loader, profiles);
}
