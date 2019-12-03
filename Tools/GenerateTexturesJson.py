'''
	GenerateTexturesJson.py
	Used for testing the texture sheet packing algorithm
	Will generate a valid textures.json all the images
		in a directory.
'''

from os import listdir
from os.path import isfile, join, splitext
import sys
import json

EXTENSIONS = ['.png', '.jpg', '.bmp', '.ico', '.gif', '.tga']

def main():
    path = sys.argv[1]
    files = [f for f in listdir(path) if isfile(join(path, f)) and splitext(f)[1].lower() in EXTENSIONS]

    print 'Serializing ' + str(len(files)) + ' files.'

    textures = {}

    for i, f in enumerate(files):
        textures[str(i)] = {'filename': f}

    serialized_json = json.dumps({'Textures': textures}, sort_keys=True,
                                 indent=4, separators=(',', ': '))

    with open(sys.argv[2], 'w') as f:
        f.write(serialized_json)

if __name__ == '__main__':
    main()
