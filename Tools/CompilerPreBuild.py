'''
	CompilerPostBuild.py
	Handle post-build for the Swift Compiler
'''

import subprocess
import os
import sys

ATTRIBUTE = 'AssemblyInformationalVersion'

def get_git_revision():
    pipe = subprocess.Popen(['git', 'rev-parse', '--short', 'HEAD'],
                            stdout=subprocess.PIPE, shell=os.name == 'nt')

    return pipe.stdout.read().replace('\n', '')
	
def script_dir():
    return os.path.dirname(os.path.realpath(__file__))

def main():
    source_path = os.path.join(sys.argv[1], 'Properties', 'AssemblyInfo.cs')
	
    with open(os.path.join(script_dir(), 'push', 'minor.txt')) as f:
        minor = f.readlines()[0]

    with open(source_path) as f:
        source = f.readlines()

    for i, l in enumerate(source):
        if ATTRIBUTE in l:
            version = l.split('("')[1].split('")')[0]

            version = version.split('-')[0]
            major = version.split('.')[0]
            #minor = version.split('.')[1]
            build = int(version.split('.')[2])

            version = "%s.%s.%i-%s" % (major, minor, build + 1, get_git_revision())

            source[i] = '[assembly: %s("%s")]\n' % (ATTRIBUTE, version)

    with open(source_path, 'w') as f:
        f.writelines(source)

if __name__ == '__main__':
    main()
