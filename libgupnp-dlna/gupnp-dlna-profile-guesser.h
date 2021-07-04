/*
 * Copyright (C) 2010 Nokia Corporation.
 * Copyright (C) 2012, 2013 Intel Corporation
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

#ifndef __GUPNP_DLNA_PROFILE_GUESSER_H__
#define __GUPNP_DLNA_PROFILE_GUESSER_H__

#include <glib-object.h>
#include <libgupnp-dlna/gupnp-dlna-profile.h>
#include <libgupnp-dlna/gupnp-dlna-information.h>

G_BEGIN_DECLS

#define GUPNP_TYPE_DLNA_PROFILE_GUESSER (gupnp_dlna_profile_guesser_get_type())

G_DECLARE_DERIVABLE_TYPE (GUPnPDLNAProfileGuesser,
                          gupnp_dlna_profile_guesser,
                          GUPNP_DLNA,
                          PROFILE_GUESSER,
                          GObject)

// Backwards-compatible defines
/**
 * GUPNP_IS_DLNA_PROFILE_GUESSER: (skip)
 */
#define GUPNP_IS_DLNA_PROFILE_GUESSER GUPNP_DLNA_IS_PROFILE_GUESSER
/**
 * GUPNP_IS_DLNA_PROFILE_GUESSER_CLASS: (skip)
 */
#define GUPNP_IS_DLNA_PROFILE_GUESSER_CLASS GUPNP_DLNA_IS_PROFILE_GUESSER_CLASS

/**
 * GUPnPDLNAProfileGuesserClass:
 * @parent_class: A #GObjectClass - parent of this class.
 */
struct _GUPnPDLNAProfileGuesserClass {
        GObjectClass parent_class;
};

GUPnPDLNAProfileGuesser *
gupnp_dlna_profile_guesser_new (gboolean relaxed_mode,
                                gboolean extended_mode);

/* Asynchronous API */
gboolean
gupnp_dlna_profile_guesser_guess_profile_async
                                       (GUPnPDLNAProfileGuesser  *guesser,
                                        const gchar              *uri,
                                        guint                     timeout_in_ms,
                                        GError                  **error);

/* Synchronous API */
GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_guess_profile_sync
                                       (GUPnPDLNAProfileGuesser  *guesser,
                                        const gchar              *uri,
                                        guint                     timeout_in_ms,
                                        GUPnPDLNAInformation    **dlna_info,
                                        GError                  **error);

GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_guess_profile_from_info
                                        (GUPnPDLNAProfileGuesser *guesser,
                                         GUPnPDLNAInformation    *info);

/* Get a GUPnPDLNAProfile by name */
GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_get_profile (GUPnPDLNAProfileGuesser *guesser,
                                        const gchar             *name);

/* API to list all available profiles */
GList *
gupnp_dlna_profile_guesser_list_profiles (GUPnPDLNAProfileGuesser *guesser);

gboolean
gupnp_dlna_profile_guesser_get_relaxed_mode (GUPnPDLNAProfileGuesser *guesser);

gboolean
gupnp_dlna_profile_guesser_get_extended_mode (GUPnPDLNAProfileGuesser *guesser);

void
gupnp_dlna_profile_guesser_cleanup (void);

G_END_DECLS

#endif /* __GUPNP_DLNA_PROFILE_GUESSER_H__ */
