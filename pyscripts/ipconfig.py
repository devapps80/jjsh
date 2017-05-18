#!/usr/bin/env python3.6

from subprocess import call
import socket, struct, os

route = "/proc/net/route"

def get_default_gateway_linux():
    """Read the default gateway directly from /proc."""
    with open(route) as fh:
        for line in fh:
            fields = line.strip().split()
            if fields[1] != '00000000' or not int(fields[3], 16) & 2:
                continue

            return socket.inet_ntoa(struct.pack("<L", int(fields[2], 16)))

def get_default_iface_name_linux():
    """Read the default gateway directly from /proc."""
    with open(route) as fh:
        for line in fh.readlines():
            try:
                iface, dest, _, flags, _, _, _, _, _, _, _, = line.strip().split()
                if dest != '00000000' or not int(flags, 16) & 2:
                    continue
                return iface
            except:
                continue

def get_resolv():
    rconf = open("/etc/resolv.conf").read().split()
    for item in rconf:
        if len(item.split(".")) == 4:
            return item

call(['ifconfig', '-a'])
print ('{0}                 {1}'.format('Gateway', 'Interface'))
gateway = get_default_gateway_linux()
interface = get_default_iface_name_linux()
print ('{0}              {1}'.format(gateway, str(interface)))
print ("")
print ('{0}'.format("DNS Servers"))
server = get_resolv()
print ('{0}'.format(server))
