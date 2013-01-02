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

#ifndef __GUPNP_DLNA_PROFILE_GUESSER_IMPL_H__
#define __GUPNP_DLNA_PROFILE_GUESSER_IMPL_H__

#include <glib.h>
#include <glib-object.h>

#include "gupnp-dlna-profile.h"
#include "gupnp-dlna-information.h"

G_BEGIN_DECLS

GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_impl_guess_image_profile
                                        (GUPnPDLNAInformation *info,
                                         GList                *profiles);

GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_impl_guess_video_profile
                                        (GUPnPDLNAInformation *info,
                                         GList                *profiles);
GUPnPDLNAProfile *
gupnp_dlna_profile_guesser_impl_guess_audio_profile
                                        (GUPnPDLNAInformation *info,
                                         GList                *profiles);

G_END_DECLS

#endif /* __GUPNP_DLNA_PROFILE_GUESSER_IMPL_H__ */
