#!/usr/bin/env python3.6

import sys, os, socket, getpass

sys.stdout.write(socket.gethostname() + ":" + os.getcwd() + "#> ")
