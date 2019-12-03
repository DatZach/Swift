'''
	UpdateEngineBindings.py
	Called by Compiler's pre-build event to update the JSON
	that specifies the Engine bindings (classes, methods, enums, etc).
	
	Required Package:
		https://pypi.python.org/pypi/CppHeaderParser/
		
	Example Comment:
		MUST BE BEFORE void Install()
		/*!
			class Debug

			fields
			ScaleX		public int
			Something	public object:Color

			properties
			Foo			static get set int

			methods
			WriteLine	1 static
			Write		1 static
			Log			1 static
			Trace		1 static
			Warn		1 static
			Error		1 static

			constants
			Test		3
			Test2		4.0
			Lol			"HelloWorld"

			enum Key
			A			1
			B			2
			C			3
			endenum
		 */
'''

from os import listdir, walk
from os.path import isfile, join, splitext, basename
import sys
import json
import CppHeaderParser
import re
import json
import datetime

MODE_NONE		= 0
MODE_FIELDS		= 1
MODE_PROPERTIES	= 2
MODE_METHODS	= 3
MODE_CONSTANTS	= 4
MODE_ENUMS		= 5

def parse_variant(arg):
	variant = {}
	
	if 'int' in arg:
		variant['Type'] = 'Int64'
		variant['IntValue'] = 0
	elif 'double' in arg:
		variant['Type'] = 'Double'
		variant['DoubleValue'] = 0.0
	elif 'string' in arg:
		variant['Type'] = 'String'
		variant['StringValue'] = ''
	elif 'nil' in arg:
		variant['Type'] = 'Nil'
	elif 'list' in arg:
		variant['Type'] = 'List'
		variant['ArrayValue'] = []
	elif arg.startswith('object'):
		value = arg.split(':')
		if len(value) > 1:
			variant['ObjectName'] = value[1]
		
		variant['Type'] = 'Object'
	elif arg.startswith('"') and arg.endswith('"'):
		variant['StringValue'] = arg.replace('"', '')
		variant['Type'] = 'String'
	elif arg.isdigit():
		variant['IntValue'] = int(arg)
		variant['Type'] = 'Int64'
	elif '.' in arg:
		variant['DoubleValue'] = float(arg)
		variant['Type'] = 'Double'
	else:
		print 'Warn: Unable to resolve variant type of arg - ' + arg
		
	return variant

def parse_binding_header(header):
	mode = MODE_NONE
	class_json = { 'Fields': [], 'Methods': [], 'Enums': [], 'Constants': [] }
	class_enum = { 'Name': '', 'Visibility': 'Public', 'Values': {} }
	
	for line in header[0].splitlines(False):
		line = line.strip()
		
		# Mode switching
		if line.startswith('ignore'):
			return None
		elif line.startswith('class'):
			class_json['Name'] = line.split(' ')[1]
			continue
		elif line.startswith('fields'):
			mode = MODE_FIELDS
			continue
		elif line.startswith('properties'):
			mode = MODE_PROPERTIES
			continue
		elif line.startswith('methods'):
			mode = MODE_METHODS
			continue
		elif line.startswith('constants'):
			mode = MODE_CONSTANTS
			continue
		elif line.startswith('enum'):
			mode = MODE_ENUMS
			class_enum = { 'Name': '', 'Visibility': 'Public', 'Values': {} }
			class_enum['Name'] = re.sub('\t+', ' ', line).split(' ')[1]
			continue
		elif line.startswith('endenum'):
			mode = MODE_NONE
			class_json['Enums'].append(class_enum)
			continue
		elif line.startswith('*/'):
			break

		# Mode specific parsing
		if mode == MODE_FIELDS:
			class_field = { 'Name': '', 'Visibility': 'Public', 'Static': False }
			args = re.sub('\t+', ' ', line).split(' ')
			if len(args) == 0:
				print 'Malformed field - ' + line
				continue
			
			class_field['Name'] = args[0]
			
			if 'public' in args:
				class_field['Visibility'] = 'Public'
			
			if 'private' in args:
				class_field['Visibility'] = 'Private'

			if 'static' in args:
				class_field['Static'] = True
				
			if  args[-1] in [ 'int', 'double', 'string', 'list', 'object' ] or args[-1].startswith('object'):
				class_field['Value'] = parse_variant(args[-1])
			
			class_json['Fields'].append(class_field)
		elif mode == MODE_PROPERTIES:
			# Properties
			class_field = { 'Name': '', 'Visibility': 'Public', 'Static': False }
			property_name = ''
			
			args = re.sub('\t+', ' ', line).split(' ')
			if len(args) == 0:
				print 'Malformed property: ' + line
				continue
				
			property_name = args[0]
			class_field['Name'] = '__property_' + property_name
		
			if 'static' in args:
				class_field['Static'] = True
				
			if 'public' in args:
				class_field['Visibility'] = 'Public'
			
			if 'private' in args:
				class_field['Visibility'] = 'Private'

			if 'get' in args:
				class_json['Methods'].append(
					{ 'Name': '__get_' + property_name,
					  'Visibility': 'Public',
					  'Static': class_field['Static'],
					  'Arguments': [ 'x' ] })
			
			if 'set' in args:
				class_json['Methods'].append(
					{ 'Name': '__set_' + property_name,
					  'Visibility': 'Public',
					  'Static': class_field['Static'],
					  'Arguments': [ 'x' ] })
					  
			if  args[-1] in [ 'int', 'double', 'string', 'list', 'object' ] or args[-1].startswith('object'):
				class_field['Value'] = parse_variant(args[-1])
			
			class_json['Fields'].append(class_field)
		elif mode == MODE_METHODS:
			# Methods
			class_method = { 'Name': '', 'Visibility': 'Public', 'Static': False, 'Arguments': [] }
			
			args = re.sub('\t+', ' ', line).split(' ')
			if len(args) < 2:
				print 'Warn: Malformed method definition - ' + line
				continue
			
			class_method['Name'] = args[0]
			for x in xrange(int(args[1])):
				class_method['Arguments'].append('arg' + str(x))
			
			if 'public' in args:
				class_method['Visibility'] = 'Public'
			
			if 'private' in args:
				class_method['Visibility'] = 'Private'
				
			if 'static' in args:
				class_method['Static'] = True
			
			class_json['Methods'].append(class_method)
		elif mode == MODE_CONSTANTS:
			# Constants
			class_constant = { 'Name': '', 'Visibility': 'Public', 'Value': {} }
			
			args = re.sub('\t+', ' ', line).split(' ')
			if len(args) < 2:
				print 'Warn: Malformed constant definition - ' + line
				continue
			
			class_constant['Name'] = args[0]
			class_constant['Value'] = parse_variant(args[1])
			
			class_json['Constants'].append(class_constant)
		elif mode == MODE_ENUMS:
			# Enumerations
			args = re.sub('\t+', ' ', line).split(' ')
			if len(args) < 2:
				print 'Malformed enum entry - ' + line
				mode = MODE_NONE
				continue
			
			class_enum['Values'][args[0]] = int(args[1])
	
	return class_json

def parse_stdlib_file(fname):
	print basename(fname)
	
	try:
		cpp = CppHeaderParser.CppHeader(fname)
	except CppHeaderParser.CppParseError, e:
		print e
		sys.exit(1)
	
	header = \
		[func['doxygen']
			for func in cpp.functions
			if func['name'] == 'Install'
			if 'doxygen' in func
		]
	
	if len(header) == 0:
		print 'Warning: No binding header found!'
		return
	
	return parse_binding_header(header)

def main():
	engine_json = []
	
	if len(sys.argv) < 3:
		print 'UpdateEngineBindings.py <Source Directory> <Output File>'
		return
		
	start_time = datetime.datetime.now()
	
	path = sys.argv[1]
	files = \
		[join(path, sub, f)
			for _, subs, _ in walk(path)
			for sub in subs
			for f in listdir(join(path, sub))
		]
	
	for f in files:
		x = parse_stdlib_file(f)
		if x is not None:
			engine_json.append(x)
	
	serialized_json = json.dumps(engine_json, sort_keys=True,
								 indent=4, separators=(',', ': '))
	
	with open(sys.argv[2], 'w') as f:
		f.write(serialized_json)
	
	print 'Generated engine bindings in ' + str(datetime.datetime.now() - start_time)

if __name__ == '__main__':
	main()
