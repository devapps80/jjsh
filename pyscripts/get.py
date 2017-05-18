#!/usr/bin/env python3.6

import shutil
import os, sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-strings', type=str, nargs='*', help='download files', required=False)
args = parser.parse_args()

source2 = os.listdir(os.getcwd())
destination = "/tmp/files1/"

if len(sys.argv) == 1:
    for files in source2:
        print('{0} {1}'.format('Copying...', files))
        shutil.copy(files, destination)
else:
    for x in args.strings:
        try:
            source =  x
            print('{0} {1}'.format('Copying...', x))
            shutil.copy(source, destination)
        except (FileNotFoundError, IOError):
            print("Wrong file or file path")
