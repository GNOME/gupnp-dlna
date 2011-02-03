/* GUPnPDLNA
 * gupnp-dlna-ls-profiles.c
 *
 * Copyright (C) 2010 Nokia Corporation
 * Copyright (C) 2010 Collabora Multimedia
 *
 * Authors: Parthasarathi Susarla <partha.susarla@collabora.co.uk>
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <glib-object.h>
#include <gio/gio.h>

#include <libgupnp-dlna/gupnp-dlna-profile.h>
#include <libgupnp-dlna/gupnp-dlna-discoverer.h>

#include <gst/pbutils/pbutils.h>

static gboolean verbose = FALSE, relaxed = FALSE;

static void print_caps (const GstCaps *caps)
{
        int i;

        for (i = 0; i < gst_caps_get_size (caps); i++) {
                GstStructure *structure = gst_caps_get_structure (caps, i);
                gchar *tmp = gst_structure_to_string (structure);

                g_print ("%s`- %s\n", i ? "    " : "", tmp);

                g_free (tmp);
        }
}

static void
print_profile (GUPnPDLNAProfile *profile, gpointer unused)
{
        GstEncodingProfile *enc_profile;
        const GList *tmp;
        gchar *caps_str;

        enc_profile = gupnp_dlna_profile_get_encoding_profile (profile);
        tmp = gst_encoding_container_profile_get_profiles
                                        (GST_ENCODING_CONTAINER_PROFILE (enc_profile));

        g_print ("%s %-30s%-35s",
                 gupnp_dlna_profile_get_extended (profile) ? "*" : " ",
                 gupnp_dlna_profile_get_name (profile),
                 gupnp_dlna_profile_get_mime (profile));

        if (verbose) {
                caps_str = gst_caps_to_string
                        (gst_encoding_profile_get_format (enc_profile));
                g_print ("\n`- container: %s\n", caps_str);
                g_free (caps_str);

                while (tmp) {
                        print_caps (gst_encoding_profile_get_format
                                        (GST_ENCODING_PROFILE (tmp->data)));
                        tmp = tmp->next;
                }
        }

        g_print ("\n");
        gst_encoding_profile_unref (enc_profile);
}

int
main (int argc, char **argv)
{
        GError *err = NULL;
        GList *profiles = NULL;
        GUPnPDLNADiscoverer *discover;

        GOptionEntry options[] = {
                {"verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
                 "Print (very) verbose output", NULL},
                {"relaxed", 'r', 0, G_OPTION_ARG_NONE, &relaxed,
                 "Read profiles in relaxed mode (only useful with -v)", NULL},
                {NULL}
        };

        GOptionContext *ctx;

        if (!g_thread_supported ())
                g_thread_init(NULL);

        ctx = g_option_context_new (" - program to list all the DLNA profiles supported by gupnp-dlna");
        g_option_context_add_main_entries (ctx, options, NULL);
        g_option_context_add_group (ctx, gst_init_get_option_group ());

        if (!g_option_context_parse (ctx, &argc, &argv, &err)) {

                g_print ("Error initializing: %s\n", err->message);
                exit (1);
        }

        g_option_context_free (ctx);

        gst_init (&argc, &argv);

        discover = gupnp_dlna_discoverer_new ((GstClockTime) GST_SECOND,
                                              relaxed,
                                              TRUE);

        profiles = (GList *) gupnp_dlna_discoverer_list_profiles (discover);

        if (!verbose) {
                g_print ("  %-30s%s\n", "Name", "MIME type");
                g_print ("---------------------------------------------------"
                         "---------------------\n");
        }
        g_list_foreach (profiles, (GFunc) print_profile, NULL);
        g_print ("\nProfiles with a '*' against their name are extended "
                 "(non-standard) profiles.\n\n");

        g_object_unref (discover);

        return 0;
}
