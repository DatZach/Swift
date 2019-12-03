'''
	EnginePostBuild.py
	Handle post-build for Swift Engine
'''

import subprocess
import os
import sys

def get_git_revision():
    pipe = subprocess.Popen(['git', 'rev-parse', '--short', 'HEAD'],
                            stdout=subprocess.PIPE, shell=os.name == 'nt')

    return pipe.stdout.read().replace('\n', '')

def script_dir():
	return os.path.dirname(os.path.realpath(__file__))

def main():
	source_path = os.path.join(sys.argv[1], 'include', 'version.hpp')
	
	with open(os.path.join(script_dir(), 'push', 'minor.txt')) as f:
		minor = f.readlines()[0]
	
	with open(source_path) as f:
		source = f.readlines()

	for i, l in enumerate(source):
		line = l.split(' ')
		
		if 'MINOR' in l:
			source[i] = '%s %s %s\n' % (line[0], line[1], minor)
			print source[i]
		elif 'BUILD' in l:
			version = line[2]
			source[i] = '%s %s %i\n' % (line[0], line[1], int(version) + 1)
			print source[i]
		elif 'REV_TAG' in l:
			version = line[2]
			source[i] = '%s %s "%s"\n' % (line[0], line[1], get_git_revision())
			print source[i]
			break
			
	with open(source_path, 'w') as f:
		f.writelines(source)

if __name__ == '__main__':
	main()
