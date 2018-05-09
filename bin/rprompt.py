#!/usr/bin/env python3

import sys, os, socket, getpass

sys.stdout.write(socket.gethostname() + ":" + os.getcwd() + "#> ")
