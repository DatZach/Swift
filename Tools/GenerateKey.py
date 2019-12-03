'''
	GenerateKey.py
	Generate a key for the crackable
'''

from Tkinter import Tk
import string
import random

def clipboard_set(value):
	tk = Tk()
	tk.withdraw()
	tk.clipboard_clear()
	tk.clipboard_append(value)
	tk.destroy()

def random_string(length):
	return ''.join(random.choice(string.ascii_uppercase) for x in range(length))

def create_key():
	sextet_a = random_string(6)
	sextet_b = sextet_a.encode('rot13')
	checksum = 0
	
	for c in sextet_a + sextet_b:
		checksum += ord(c) ^ 0x2D
	
	checksum <<= 2
	checksum ^= 0x1D234
	checksum %= 999999
	
	return '%s-%s-%s' % (sextet_a, sextet_b, checksum)

def main():
	key = create_key()
	print 'Generated key: ' + key
	clipboard_set(key);
	print 'Copied to your clipboard.'

if __name__ == '__main__':
	main()
