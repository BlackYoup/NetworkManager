/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* NetworkManager -- Network link manager
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Copyright (C) 2007 - 2008 Novell, Inc.
 * Copyright (C) 2007 - 2010 Red Hat, Inc.
 */

#ifndef NM_MANAGER_H
#define NM_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <dbus/dbus-glib.h>
#include "nm-device.h"
#include "nm-device-interface.h"
#include "nm-settings.h"

#define NM_TYPE_MANAGER            (nm_manager_get_type ())
#define NM_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), NM_TYPE_MANAGER, NMManager))
#define NM_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), NM_TYPE_MANAGER, NMManagerClass))
#define NM_IS_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NM_TYPE_MANAGER))
#define NM_IS_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), NM_TYPE_MANAGER))
#define NM_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), NM_TYPE_MANAGER, NMManagerClass))

#define NM_MANAGER_VERSION "version"
#define NM_MANAGER_STATE "state"
#define NM_MANAGER_NETWORKING_ENABLED "networking-enabled"
#define NM_MANAGER_WIRELESS_ENABLED "wireless-enabled"
#define NM_MANAGER_WIRELESS_HARDWARE_ENABLED "wireless-hardware-enabled"
#define NM_MANAGER_WWAN_ENABLED "wwan-enabled"
#define NM_MANAGER_WWAN_HARDWARE_ENABLED "wwan-hardware-enabled"
#define NM_MANAGER_WIMAX_ENABLED "wimax-enabled"
#define NM_MANAGER_WIMAX_HARDWARE_ENABLED "wimax-hardware-enabled"
#define NM_MANAGER_ACTIVE_CONNECTIONS "active-connections"

/* Not exported */
#define NM_MANAGER_HOSTNAME "hostname"
#define NM_MANAGER_SLEEPING "sleeping"

typedef struct {
	GObject parent;
} NMManager;

typedef struct {
	GObjectClass parent;

	/* Signals */
	void (*device_added) (NMManager *manager, NMDevice *device);
	void (*device_removed) (NMManager *manager, NMDevice *device);
	void (*state_changed) (NMManager *manager, guint state);
	void (*properties_changed) (NMManager *manager, GHashTable *properties);
} NMManagerClass;

GType nm_manager_get_type (void);

NMManager *nm_manager_get (NMSettings *settings,
                           const char *config_file,
                           const char *plugins,
                           const char *state_file,
                           gboolean initial_net_enabled,
                           gboolean initial_wifi_enabled,
                           gboolean initial_wwan_enabled,
						   gboolean initial_wimax_enabled,
                           GError **error);

void nm_manager_start (NMManager *manager);

/* Device handling */

GSList *nm_manager_get_devices (NMManager *manager);

const char * nm_manager_activate_connection (NMManager *manager,
                                             NMConnection *connection,
                                             const char *specific_object,
                                             const char *device_path,
                                             const char *dbus_sender, /* NULL if automatic */
                                             GError **error);

gboolean nm_manager_deactivate_connection (NMManager *manager,
                                           const char *connection_path,
                                           NMDeviceStateReason reason,
                                           GError **error);

/* State handling */

NMState nm_manager_get_state (NMManager *manager);

GPtrArray * nm_manager_get_active_connections_by_connection (NMManager *manager,
                                                             NMConnection *connection);

/* Compat manager accessors */

GSList *nm_manager_compat_get_active_connections (NMManager *self);

void nm_manager_compat_enable (NMManager *self,
                               gboolean enable,
                               DBusGMethodInvocation *context);

void nm_manager_compat_sleep (NMManager *self,
                              gboolean do_sleep,
                              DBusGMethodInvocation *context);

void nm_manager_compat_deactivate_connection (NMManager *self,
                                              const char *active_path,
                                              DBusGMethodInvocation *context);

GSList *nm_manager_compat_get_user_connections (NMManager *self);

gboolean nm_manager_compat_auto_user_connections_allowed (NMManager *self);

void nm_manager_compat_activate_connection (NMManager *self,
                                            gboolean user_settings,
                                            const char *connection_path,
                                            const char *device_path,
                                            const char *specific_object_path,
                                            DBusGMethodInvocation *context);

#endif /* NM_MANAGER_H */
