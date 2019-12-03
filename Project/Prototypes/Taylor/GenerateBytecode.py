'''
	GenerateBytecode.py
	Convert dumped bytecode into C++ code
'''

from Tkinter import Tk
import re

def clipboard_get():
	tk = Tk()
	tk.withdraw()
	value = tk.clipboard_get()
	tk.destroy()
	
	return value

def clipboard_set(value):
	tk = Tk()
	tk.withdraw()
	tk.clipboard_clear()
	tk.clipboard_append(value)
	tk.destroy()

def main():
	cpp_code = ''
	
	for line in clipboard_get().split('\n'):
		tokens = re.sub(r'\s+', ' ', line).split(' ')
		
		if len(tokens) == 3:
			cpp_code += '\tinstructions.push_back(Vm::Instruction(Vm::Instruction::%s, %s));\n' % (tokens[1], tokens[2])
		elif len(tokens) == 2:
			cpp_code += '\tinstructions.push_back(Vm::Instruction(Vm::Instruction::%s));\n' % tokens[1]
		
	clipboard_set(cpp_code)

if __name__ == '__main__':
	main()
