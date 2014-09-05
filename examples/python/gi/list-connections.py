#!/usr/bin/env python
# -*- Mode: Python; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
# vim: ft=python ts=4 sts=4 sw=4 et ai

#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Copyright 2012 - 2014 Red Hat, Inc.
#

from gi.repository import NM

# This example asks settings service for all configured connections.
# Unfortunately, at this time since libnm still makes heavy use of
# GValue and GHashTable (rather than GVariant), libnm isn't fully
# usable from GObject Introspection-ready languages. Most functions will
# work fine, but e.g. nm_connection_to_dbus() causes failures.


def print_values(setting, key, value, flags, data):
    print "  %s.%s: %s" % (setting.get_name(), key, value)

if __name__ == "__main__":
    # create RemoteSettings object
    settings = NM.RemoteSettings.new(None)

    # get all connections
    connections = settings.list_connections()

    # print the connections' details
    for c in connections:
        print "=== %s : %s ===" % (c.get_id(), c.get_path())
        c.for_each_setting_value(print_values, None)
        print "\n"

