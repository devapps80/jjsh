#!/usr/bin/env python3.6

import shutil
import os, sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-strings', type=str, nargs='*', help='download files', required=False)
args = parser.parse_args()

basepath = os.getcwd()
destination = "/tmp/files1/"

if len(sys.argv) == 1:
    for fname in os.listdir(basepath):
        path = os.path.join(basepath, fname)
        if os.path.isdir(path):
            print('{0} {1}'.format('Skipping...', path))
            continue
        print('{0} {1}'.format('Copying...', fname))
        shutil.copy(fname, destination)
else:
    for x in args.strings:
        try:
            source =  x
            print('{0} {1}'.format('Copying...', x))
            shutil.copy(source, destination)
        except (FileNotFoundError, IOError):
            print("Wrong file or file path")
