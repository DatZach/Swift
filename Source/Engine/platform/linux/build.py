"""
	build.py
	Swift Engine build script for Linux platform
"""

import fnmatch
import subprocess
import os

EXTENSION		= '*.cpp'
SOURCE_PATH		= './../../src'
LIBS_PATH		= 'libs'
OBJECTS_PATH	= 'objs'
INCLUDE_PATHS	= [ SOURCE_PATH + '/include', LIBS_PATH + '/include' ]

def main():
	sources = []
	objects = []

	# Grab all sources
	for root, dirs, files in os.walk(SOURCE_PATH):
		for filename in fnmatch.filter(files, EXTENSION):
			sources.append(os.path.join(root, filename))

	# Build commands
	for source in sources:
		filename, _ = os.path.splitext(source)
		filename = filename.split(SOURCE_PATH)[1]
		objpath = OBJECTS_PATH + filename + '.o'
		objects.append(objpath)
		
		cmd = [ 'g++' ]
		for p in INCLUDE_PATHS:
			cmd.append('-I' + p)
		
		cmd.extend([ '-c', source, '-o', objpath ])

		subprocess.call(cmd)

if __name__ == '__main__':
	main()

