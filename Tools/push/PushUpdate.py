'''
    PushUpdate.py
    Used to recompile all Swift components and push the revision out.
'''

import subprocess
import os
import sys
import zipfile
import datetime
from poster.encode import multipart_encode
from poster.streaminghttp import register_openers
import urllib2
import time

REMOTE_PATH = 'http://www.x2048.com/swift/update/push.php'
PASSWORD = '__REDACTED__'

def get_git_revision():
    pipe = subprocess.Popen(['git', 'rev-parse', '--short', 'HEAD'],
                            stdout=subprocess.PIPE, shell=os.name == 'nt')
    
    return pipe.stdout.read().replace('\n', '')

def script_dir():
    return os.path.dirname(os.path.realpath(__file__))

def increment_minor_version():
    print 'Minor version file at ' + os.path.join(script_dir(), 'minor.txt')
    
    with open(os.path.join(script_dir(), 'minor.txt')) as f:
        minor = f.readlines()[0]
    
    with open(os.path.join(script_dir(), 'minor.txt'), 'w') as f:
        f.write(str(int(minor) + 1))

def batch_execute(path):
    p = subprocess.Popen(path, cwd=r".")
    stdout, stderr = p.communicate()

def zipdir(path, zip):
    for root, dirs, files in os.walk(path):
        for file in files:
            zip.write(os.path.join(root, file))

def upload_file(remote_path, path):
    datagen, headers = multipart_encode({ 'file': open(path, 'rb') })
    request = urllib2.Request(remote_path, datagen, headers)
    print urllib2.urlopen(request).read()

def main():
    pwd = ''
    while pwd != 'push':
        pwd = raw_input('Password: ')
    
    if pwd != 'push':
        return
    
    increment_minor_version()
    
    with open(os.path.join(script_dir(), 'minor.txt')) as f:
        print 'Pushing minor version ' + f.readlines()[0]
    
    rev_tag = get_git_revision()
    start_time = datetime.datetime.now()
    batch_execute("winbuild.bat")
    print 'Compiled in ' + str(datetime.datetime.now() - start_time)
    
    print 'Zipping...'
    zip_filename = rev_tag + '.zip'
    with zipfile.ZipFile(zip_filename, 'w') as zipf:
        os.chdir('bin')
        zipdir('.', zipf)
        zipf.close()
        os.chdir('..')
    print 'Done!'
    
    print 'Uploading...'
    remote_path = REMOTE_PATH + '?pwd=' + PASSWORD + '&revtag=' + rev_tag
    upload_file(remote_path, zip_filename)
    
    print 'Cleaning up...'
    os.remove(zip_filename)
    
    raw_input('Press Enter to continue...')

if __name__ == '__main__':
    register_openers()
    main()
