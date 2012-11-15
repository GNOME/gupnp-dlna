/*
 * Copyright (C) 2012 Intel Corporation.
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

#ifndef __GUPNP_DLNA_GST_INFO_UTILS_H__
#define __GUPNP_DLNA_GST_INFO_UTILS_H__

#include <glib.h>
#include <gst/pbutils/pbutils.h>
#include "gupnp-dlna-values.h"

G_BEGIN_DECLS

/* some unref functions are macros, so we wrap them into functions,
   because we want to pass them to g_clear_pointer. */
void
gupnp_dlna_gst_discoverer_info_unref (gpointer info);

void
gupnp_dlna_gst_discoverer_stream_info_unref (gpointer stream);

GUPnPDLNAIntValue
gupnp_dlna_gst_get_int_value (GstCaps* caps,
                              GstDiscovererStreamInfo *stream,
                              GstDiscovererInfo *info,
                              const gchar *name);

GUPnPDLNAStringValue
gupnp_dlna_gst_get_string_value (GstCaps* caps,
                                 GstDiscovererStreamInfo *stream,
                                 GstDiscovererInfo *info,
                                 const gchar *name);

GUPnPDLNABoolValue
gupnp_dlna_gst_get_bool_value (GstCaps* caps,
                               GstDiscovererStreamInfo *stream,
                               GstDiscovererInfo *info,
                               const gchar *name);

GUPnPDLNAFractionValue
gupnp_dlna_gst_get_fraction_value (GstCaps* caps,
                                   GstDiscovererStreamInfo *stream,
                                   GstDiscovererInfo *info,
                                   const gchar *name);

GUPnPDLNAStringValue
gupnp_dlna_gst_get_mime (GstCaps* caps);

G_END_DECLS

#endif /* __GUPNP_DLNA_GST_INFO_UTILS_H__ */
