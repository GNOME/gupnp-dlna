/*
 * Copyright (C) 2012, 2013 Intel Corporation.
 *
 * Authors: Krzesimir Nowak <krnowak@openismus.com>
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

#include "gupnp-dlna-native-profile-loader.h"
#include "gupnp-dlna-native-profile-backend.h"

GUPnPDLNAProfileLoader *
gupnp_dlna_native_profile_backend_get_loader (gboolean relaxed_mode,
                                              gboolean extended_mode)
{
        return GUPNP_DLNA_PROFILE_LOADER
                (gupnp_dlna_native_profile_loader_new (relaxed_mode,
                                                       extended_mode));
}
