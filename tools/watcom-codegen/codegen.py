#!/usr/bin/env python3 -u

#######################################################
# Parses Watcom "exedump" output into c skeleton coode
# exedump: https://github.com/jeff-1amstudios/open-watcom-v2/tree/master/bld/exedump
#
# Usage: codegen.py <path to dump file> <path to write generated code>
#######################################################
import sys
import re
import os
import errno
import shutil

#BASE_OUTPUT_DIR = '_generated/'

module_start_regex = '\d+\\) Name:\s+(\S+)'
LOCAL_HEADER_REGEX = '(\S+):\s+(\S+)'
GLOBAL_VAR_REGEX = '\"(.*)\" addr = (.*),\s+type = (\d+)'
FUNCTION_OFFSET_REGEX = 'start off = (\S+), code size = (\S+), parent off = (\S+)'
LOCAL_VAR_REGEX = 'name = "(\S+)",  type = (\d+)'
LOCAL_VAR_ADDR_REGEX = 'address: (\S+)\( (\S+) \)'
LOCALS_SECTION_HEADER = '*** Locals ***'

TYPES_SECTION_HEADER = '*** Types ***'
TYPE_HEADER_REGEX = '(\S+): (.+)\((\d+)\)'
ARRAY_TYPE_REGEX = 'high bound = (\S+)   base type idx = (\d+)'
NAME_TYPE_REGEX = '"(\S+)"  type idx = (\d+)  scope idx = (\d+)'
FIELD_LIST_REGEX = 'number of fields = (\S+)   size = (\S+)'

FIELD_HEADER_REGEX = '(\S+):\s+(\S+)'
FIELD_ITEM_REGEX = '"(\S+)"  offset = (\S+)  type idx = (\d+)'
FIELD_ITEM_BIT_REGEX = 'start bit = (\S+)  bit size = (\S+)'

ENUM_LIST_REGEX = 'number of consts = (\S+)   scalar type = (\S+), (\S+)'
ENUM_ITEM_REGEX = '"(\S+)"   value = (\S+)'

SECTION_UNDERLINE = '^(=*)$' # ignore these lines

fp = open(sys.argv[1], 'r')
BASE_OUTPUT_DIR = sys.argv[2]

STATE_NONE = 0
STATE_MODULE = 1
STATE_LOCALS = 2
STATE_TYPES = 3

INDENT_SPACES = 4

modules = []
scopes = []
last_field_list = {}
last_fn = None
last_local_type = ''
line_number = 0
last_read_offset = 0
eof = 0
generated_type_names = {}
indent = 0

dr_types_file = {}
br_types_file = {}

def read_line():
  global line_number
  global last_read_offset
  global eof

  last_read_offset = fp.tell()
  line = fp.readline()
  if line == '':
    eof = 1
  line_number = line_number + 1
  return line.strip()

def unread_line():
  global last_read_offset
  global line_number

  line_number = line_number - 1
  fp.seek(last_read_offset, os.SEEK_SET)


def read_file():
  state = STATE_NONE
  current_module = None

  while eof == 0:
    line = read_line()

    if line == '':
      continue
    if line.startswith('Data'):
      continue

    match = re.match(module_start_regex, line)
    if match:

      if current_module is not None:
        cleanup_function_args(current_module)

      current_module = {
        'name': match.group(1),
        'functions': [],
        'global_vars': [],
        'types': {},
        'number': len(modules)+1
      }
      print ("Module", len(modules), current_module['name'])
      modules.append(current_module)
      last_fn = None
      last_local_type = ''
      # if len(modules) == 2:
      #   break

    elif line == LOCALS_SECTION_HEADER:
      state = STATE_LOCALS
      line = read_line()  #skip underline
    elif line == TYPES_SECTION_HEADER:
      state = STATE_TYPES
    elif state == STATE_LOCALS:
      match = re.match(LOCAL_HEADER_REGEX, line)
      if match:
        local_type = match.group(2)

        if local_type == 'MODULE_386':
          glob = process_global_var()
          current_module['global_vars'].append(glob)
          last_fn = None

        elif local_type == 'NEAR_RTN_386' or local_type == 'FAR_RTN_386':
          fn = process_function(current_module)
          last_fn = fn
          current_module['functions'].append(fn)

        elif local_type == 'LOCAL':
          if last_fn is not None:
            local_var = process_function_var()
            last_fn['local_vars'].append(local_var)
          else:
            # TODO: ignore block_386 scoped local variables for now
            # Maybe indicates asm { ... } usage?
            local_var = process_function_var()

        elif local_type == 'SET_BASE_386':
          last_local_type = local_type
          last_fn = None
          read_line()

        elif local_type == 'BLOCK_386':
          read_line()
          last_fn = None

        else:
          print('Line ', line_number, 'Unknown local type', local_type)
          last_fn = None
        last_local_type = local_type
      else:
        print('Line ', line_number, 'WARN: no match:', line)
    elif state == STATE_TYPES:
      match = re.match(TYPE_HEADER_REGEX, line)
      if match:
        t = match.group(2)
        idx = match.group(3)
        typedef = process_type(current_module, t)
        if typedef is not None:
          typedef['type'] = t
          typedef['id'] = idx
          current_module['types'][idx] = typedef
          #print line_number, idx, typedef
      else:
        pass
        # modules often end with an empty type
        #print 'Line ', line_number, 'WARN: STATE_NONE:', line, 'state was', state
        #state = STATE_NONE

        #os.exit(1)

def process_global_var():
  glob = {}
  line = read_line()
  match = re.match(GLOBAL_VAR_REGEX, line)
  glob['name'] = match.group(1)
  glob['addr'] = match.group(2)
  glob['type'] = match.group(3)
  return glob

def process_function(module):
  fn = { 'args': [], 'local_vars': [] }

  #start off = 00000000, code size = 000000C9, parent off = 0000
  line = read_line()
  match = re.match(FUNCTION_OFFSET_REGEX, line)
  fn['offset'] = int(match.group(1), 16)
  fn['size'] = int(match.group(2), 16)
  fn['parent_offset'] = int(match.group(3), 16)

  # prologue size = 23,  epilogue size = 9
  read_line()

  # return address offset (from bp) = 00000004
  read_line()

  # return type:  1322
  line = read_line()
  parts = line.split(':')
  fn['type'] = parts[1].strip()

  # return value: <none>
  line = read_line()
  parts = line.split(':')
  val = parts[1].strip()
  if val == 'MULTI_REG(1)' or val == 'REG':
    fn['return_value'] = parts[2].strip()
  elif val == 'IND_REG_RALLOC_NEAR( EAX )':
    fn['return_value'] = 'EAX'
  elif val == '<none>':
    pass
  else:
    print('Line:', line_number, 'Unhandled return value', parts)

  # Parm 0: MULTI_REG(1): EAX
  line = read_line()
  while line.startswith('Parm'):
    parts = line.split(':')
    fn['args'].append(parts[2].strip())
    line = read_line()

  # Name = "AllocateActorMatrix"
  parts = line.split('=')
  fn['name'] = parts[1].replace('\"', '').strip()

  return fn

def process_function_var():
  local_var = {}

  #address: BP_OFFSET_BYTE( E4 )
  line = read_line()
  match = re.match(LOCAL_VAR_ADDR_REGEX, line)
  local_var['addr_type'] = match.group(1)
  local_var['addr'] = match.group(2)

  #name = "pTrack_spec",  type = 1320
  line = read_line()
  match = re.match(LOCAL_VAR_REGEX, line)
  local_var['name'] = match.group(1).strip()
  local_var['type'] = match.group(2).strip()

  return local_var

def process_type(current_module, type_name):
  global last_field_list
  line = read_line()
  if type_name == 'SCOPE' or type_name == 'SCALAR':
    name = line.split('"')[1].strip()
    return { 'value': name }
  elif type_name == 'NEAR386 PTR' or type_name == 'FAR386 PTR':
    base_type = line.split('=')[1].strip()
    return { 'base_type': base_type }
  elif type_name == 'BYTE_INDEX ARRAY' or type_name == 'WORD_INDEX ARRAY':
    # high bound = 00   base type idx = 11
    match = re.match(ARRAY_TYPE_REGEX, line)
    return { 'upper_bound': int(match.group(1), 16), 'base_type': match.group(2) }
  elif type_name == 'NAME':
    #"va_list"  type idx = 12  scope idx = 0
    match = re.match(NAME_TYPE_REGEX, line)
    return { 'value': match.group(1), 'type_idx': match.group(2), 'scope_idx': match.group(3) }

  elif type_name == 'NEAR386 PROC' or type_name == 'FAR386 PROC':
    # return type = 30
    return_type = line.split('=')[1].strip()
    args = []
    while True:
      # param 1:  type idx = 52
      line = read_line()
      if line.startswith("param"):
        args.append(line.split('=')[1].strip())
      else:
        # this is nasty, we have to keep reading until we get a line
        # that is not a 'param', then unget the line
        unread_line()
        break
    return { 'return_type': return_type, 'args': args }

  elif type_name == 'FIELD_LIST':
    # number of fields = 000D   size = 00000034
    match = re.match(FIELD_LIST_REGEX, line)
    nbr_fields = int(match.group(1), 16)
    struct_size = int(match.group(2), 16)
    fields = []
    for i in range(nbr_fields):
      line = read_line()
      match = re.match(FIELD_HEADER_REGEX, line)
      type_name = match.group(2)
      line = read_line()
      match = re.match(FIELD_ITEM_REGEX, line)
      field = { 'type': type_name, 'name': match.group(1), 'offset': int(match.group(2), 16), 'type_idx': match.group(3) }
      if type_name == 'BIT_BYTE' or type_name == 'BIT_WORD':
        # start bit = 1B  bit size = 01
        line = read_line()
        match = re.match(FIELD_ITEM_BIT_REGEX, line)
        field['start_bit'] = match.group(1)
        field['bit_size'] = int(match.group(2))
        #print field
      fields.append(field)
    return { 'size': struct_size, 'fields': fields }
  elif type_name == 'ENUM_LIST':
    match = re.match(ENUM_LIST_REGEX, line)
    nbr_fields = int(match.group(1), 16)
    type_idx = int(match.group(2), 16)
    elements = {}
    for i in range(nbr_fields):
      element = { ''}
      line = read_line()
      line = read_line()
      match = re.match(ENUM_ITEM_REGEX, line)
      elements[match.group(1)] = int(match.group(2), 16)
    return { 'type_idx': type_idx, 'elements': elements }
  else:
    print('*** UNEXPECTED', type_name, line)
    return None


def resolve_type_str(module, type_idx, var_name, decl=True):
  global indent
  original_type = module['types'][type_idx]
  t = module['types'][type_idx]
  bounds = ''
  indirections = ''

  while 'base_type' in t:
    if t['type'] == 'NEAR386 PTR' or t['type'] == 'FAR386 PTR':
      indirections = indirections + '*'
    elif t['type'] == 'WORD_INDEX ARRAY' or t['type'] == 'BYTE_INDEX ARRAY':
      bounds = bounds + '[{}]'.format(t['upper_bound']+1)
    t = module['types'][t['base_type']]

  if t['type'] == 'NEAR386 PROC' or t['type'] == 'FAR386 PROC':
    print (var_name, 'IS PROC')
    return_type = resolve_type_str(module, t['return_type'], '')
    #print 'rt', return_type
    a = describe_args(module, t, False)
    print(var_name, 'a', a)
    # print 'i', indirections
    # print 'b', bounds
    s = return_type + ' (' + indirections + var_name + bounds + ')(' + a + ')'
    return s
  if decl == True and t['type'] == 'FIELD_LIST':
    indent += 1
    print ('decl', var_name)
    s = get_type_declaration(module, t, var_name) + bounds
    indent -= 1
    return s
  if 'value' in t:
    if var_name == "":
      return t['value'] + indirections
    else:
      if indirections != '':
        indirections = ' ' + indirections
      else:
        indirections = ' '
      return t['value'] + indirections + var_name + bounds
  else:
    t2 = get_child_reference(module, type_idx)
    if t2 is not None:
      if var_name == "":
        return t2['value'] + indirections
      else:
        if indirections != '':
          indirections = ' ' + indirections
        return t2['value'] + indirections + var_name + bounds
      #return t2['value'] + ' ' + indirections + var_name + bounds

    print (t)
    print ('Warning: type name not found for', type_idx, module['name'])
    return ""
    #print 'resolved to', type_idx, t
    #return resolve_name_for_type(module, type_idx) + ' ' + indirections + '{}' + bounds

def resolve_function_header(module, fn):
  text = ''
  text += '// Offset: ' + str(fn['offset']) + '\n// Size: ' + hex(fn['size'])
  text += '\n//IDA: ' + resolve_function_ida_signature(module, fn)
  return text

def cleanup_function_args(module):
  # if theres a single "void" argument for a function, we just remove it
  for fn in module['functions']:
    type_idx = fn['type']
    fn_type = module['types'][type_idx]
    if len(fn_type['args']) == 1:
      arg_type = resolve_type_str(module, fn_type['args'][0], '')
      if arg_type == 'void':
        fn_type['args'] = []


def get_function_arg_count(module, fn):
  type_idx = fn['type']
  fn_type = module['types'][type_idx]
  return len(fn_type['args'])

def is_function_vararg(module, fn):
  for arg in fn['local_vars']:
    str = resolve_type_str(module, arg['type'], 'a')
    # if fn['name'] == 'DrawColumns':
    #   print '>x', str
    if 'va_list' in str:
      return True
  return False

def resolve_function_signature(module, fn):
  type_idx = fn['type']
  fn_type = module['types'][type_idx]
  return_type = resolve_type_str(module, fn_type['return_type'], "")
  args = ''
  for i in range(len(fn_type['args'])):
    arg = fn_type['args'][i]

    if arg not in module['types']:
      continue
    else:
      arg_type = module['types'][arg]

    name = fn['local_vars'][i]['name']
    arg_type = resolve_type_str(module, arg, name, False)
    if len(args) != 0:
      args += ', '
    args += arg_type

  if is_function_vararg(module, fn):
    args += ', ...'

  return return_type + ' ' + fn['name'] + '(' + args + ')'

def resolve_function_ida_signature(module, fn):
  type_idx = fn['type']
  fn_type = module['types'][type_idx]
  return_type = resolve_type_str(module, fn_type['return_type'], "")
  args = ''
  for i in range(len(fn_type['args'])):
    arg = fn_type['args'][i]

    if arg not in module['types']:
      continue
    else:
      arg_type = module['types'][arg]

    name = fn['local_vars'][i]['name']
    arg_type = resolve_type_str(module, arg, name, False)
    if len(args) != 0:
      args += ', '
    args += arg_type
    if i < len(fn['args']):
      args += '@<' + fn['args'][i] + '>'

  if is_function_vararg(module, fn):
    args += ', ...'

  # void __usercall PathCat(char *pDestn_str@<eax>, char *pStr_1@<edx>, char *pStr_2@<ebx>)
  # float __usercall GetAFloat@<st0>(FILE *pF@<eax>)
  return_reg = ''
  if len(fn['args']) > 0 and 'return_value' in fn:
    return_reg = '@<' + fn['return_value'] + '>'
  call_type = '__cdecl'
  if len(fn['args']) > 0:
    call_type = '__usercall'
  return return_type + ' ' + call_type + ' ' + fn['name'] + return_reg + '(' + args + ')'

def describe_args(module, fn_type, resolve_names):
  args = ''
  for i in range(len(fn_type['args'])):
    arg = fn_type['args'][i]
    arg_type = resolve_type_str(module, arg, '')
    if resolve_names:
      name = fn['local_vars'][i]['name']

    if len(args) != 0:
      args += ', '
    args += arg_type
    if resolve_names:
      args += ' ' + name
  return args

def get_function_by_name(module, name):
  for fn in module['functions']:
    print (fn)
    if fn['type'] == name:
      return fn
  return None


def get_struct_name(module, t):
  target_id = t['id']
  found_id = None
  found_name = None
  for i in module['types']:
    t2 = module['types'][i]
    if 'type_idx' in t2 and t2['type_idx'] == target_id:
      print ('t2', t2)
      if t2['scope_idx'] == '0':
        return t2['value']
      found_id = t2['id']
      found_name = t2['value']
      print ('found1', t2['value'])
  if found_id is None:
    print ('ERROR1: not found', t)
    return None
  for i in module['types']:
    t2 = module['types'][i]
    if 'type_idx' in t2 and t2['type_idx'] == found_id:
      #print ('found2', t2['value'])
      return t2['value']

  print ('Warning: did not find struct_name for', target_id, 'using', found_name)
  return found_name

def get_struct_tag_name(module, t):
  target_id = t['id']
  for i in module['types']:
    t2 = module['types'][i]
    if 'type_idx' in t2 and t2['type_idx'] == target_id:
      return t2['value']
  return None

def get_type_declaration(module, t, name):
  global indent
  indirections = ''
  bounds = ''
  while 'base_type' in t:
    if t['type'] == 'NEAR386 PTR' or t['type'] == 'FAR386 PTR':
      indirections = indirections + '*'
    elif t['type'] == 'WORD_INDEX ARRAY' or t['type'] == 'BYTE_INDEX ARRAY':
      bounds = bounds + '[{}]'.format(t['upper_bound']+1)
    t = module['types'][t['base_type']]

  #print ('type decl', t)
  if t['type'] == 'FIELD_LIST':
    s = 'struct'
    tag_name = get_struct_tag_name(module, t)
    if tag_name is not None:
      s += ' ' + tag_name
    s += ' {'
    s += '\t\t// size: ' + hex(t['size'])
    first_elem = True
    for e in reversed(t['fields']):
      s += '\n'
      s += ' ' * (indent * INDENT_SPACES)
      s += (' ' * INDENT_SPACES) + resolve_type_str(module, e['type_idx'], e['name']);
      if e['type'] == 'BIT_BYTE' or e['type'] == 'BIT_WORD':
        s += ': ' + str(e['bit_size'])
      s += ';'
      s += '\t\t// @' + hex(e['offset'])
      first_elem = False
    s += '\n' + ' ' * (indent * INDENT_SPACES) + '}'
    if get_struct_name(module, t) is None:
      s += ' ' + name
    return s
  elif t['type'] == 'NEAR386 PROC':
    return_type = resolve_type_str(module, t['return_type'], '')
    #print 'rt', return_type
    a = describe_args(module, t, False)
    print(name, 'a', a)
    # print 'i', indirections
    # print 'b', bounds
    if indirections != '':
      indirections = ' ' + indirections
    return return_type + indirections + ' ' + name + '(' + a + ')'
  elif t['type'] == 'SCALAR' or t['type'] == 'NAME':
    if indirections != '':
      indirections = ' ' + indirections
    return t['value'] + indirections + ' ' + name + bounds
  else:
    return 'WARN: No decl for ' + t['type']

def generate_c_skeleton():
  global dr_types_file
  global br_types_file

  try:
    shutil.rmtree(BASE_OUTPUT_DIR + '/*')
  except:
    pass
  mkdir_p(BASE_OUTPUT_DIR + '/types')

  dr_types_file = open(BASE_OUTPUT_DIR + '/types/dr_types.h', 'w')
  br_types_file = open(BASE_OUTPUT_DIR + '/types/br_types.h', 'w')

  dr_types_file.write("#ifndef DR_TYPES_H\n")
  dr_types_file.write("#define DR_TYPES_H\n\n")
  dr_types_file.write('#include \"br_types.h\"\n\n')

  br_types_file.write("#ifndef BR_TYPES_H\n")
  br_types_file.write("#define BR_TYPES_H\n\n")

  for m in modules:
    # ignore lib modules
    if 'DETHRACE' not in m['name'] and 'BRSRC13' not in m['name']:
      print("ignoring", m['name'])
      continue
    print(m['name'])
    print('-----------------------')
    generate_h_file(m)
    generate_c_file(m)
    generate_types_header(m)

  br_types_file.write("\n#endif")
  dr_types_file.write("\n#endif")

def generate_h_file(module):
  filename = BASE_OUTPUT_DIR + module['name'][3:].replace('\\', '/').replace('.c', '.h')

  dir = os.path.dirname(filename)
  name = os.path.basename(filename)

  mkdir_p(dir)
  h_file = open(filename, 'w')

  def_name = '_' + name.upper().replace('.', '_') + '_'
  h_file.write('#ifndef ' + def_name + '\n')
  h_file.write('#define ' + def_name + '\n\n')

  h_file.write('#include \"dr_types.h\"\n')
  h_file.write('#include \"br_types.h\"\n\n')
  for fn in module['functions']:
    h_file.write(resolve_function_header(module, fn))
    h_file.write('\n')
    h_file.write(resolve_function_signature(module, fn) + ';')
    h_file.write('\n\n')
  h_file.write('#endif\n')

def generate_c_file(module):
  filename = BASE_OUTPUT_DIR + module['name'][3:].replace('\\', '/')

  c_file = open(filename, 'w')
  c_file.write('#include \"')
  c_file.write(os.path.basename(filename).replace('.c', '.h'))
  c_file.write('\"\n\n')

  # global variables
  for gv in module['global_vars']:
    # print('gv:', gv)
    str = resolve_type_str(module, gv['type'], gv['name'])
    c_file.write(str)
    c_file.write(';\n')
  c_file.write('\n')

  # functions
  for fn in module['functions']:
    c_file.write(resolve_function_header(module, fn))
    c_file.write('\n')
    c_file.write(resolve_function_signature(module, fn))
    c_file.write(' {\n')

    # skip local variables that were passed in as arguments
    arg_count = get_function_arg_count(module, fn)
    
    for v in fn['local_vars'][arg_count:]:
      c_file.write(' ' * INDENT_SPACES)
      if 'CONST' in v['addr_type']:
        c_file.write('static ')
      c_file.write(resolve_type_str(module, v['type'], v['name']))
      c_file.write(';\n')

    
    c_file.write(' ' * INDENT_SPACES)
    c_file.write('LOG_TRACE("(')
    do_comma = False
    for v in fn['local_vars'][:arg_count]:
      if do_comma:
        c_file.write(', ')
      type_str = resolve_type_str(module, v['type'], '')
      fmt = '%d'
      if type_str == 'char*':
        fmt = '\\"%s\\"'
      elif '*' in type_str:
        fmt = '%p'
      elif 'int' in type_str:
        fmt = '%d'
      elif 'scalar' in type_str or 'float' in type_str:
        fmt = '%f'
      elif 'tU32' in type_str or 'tS32' in type_str or 'tU16' in type_str or 'tS16' in type_str:
        fmt = '%d'
      elif 'tU8' in type_str:
        fmt = '%d'
      elif 'br_angle' in type_str:
        fmt = '%d'
      elif 'size_t' in type_str:
        fmt = '%d'
      else:
        print('Unknown type for format', type_str)
      c_file.write(fmt)
      do_comma = True
    c_file.write(')"')

    for v in fn['local_vars'][:arg_count]:
      c_file.write(', ')
      c_file.write(v['name'])
      do_comma = True
      
    c_file.write(');\n')
    c_file.write('NOT_IMPLEMENTED();\n')
    c_file.write('}\n\n')

def get_child_reference(module, type_idx):
  for i in module['types']:
    t = module['types'][i]
    if 'type_idx' in t and t['type_idx'] == type_idx:
      return t
  return None

def generate_types_header(module):
  global generated_type_names
  global indent
  global dr_types_file
  global br_types_file

  typedefs = {}
  structs = {}
  enums = {}
  fns = {}

  #h_file = open(filename, 'w')
  for type_idx in module['types']:
    t = module['types'][type_idx]
    if t['type'] != 'NAME':
      continue
    type_name = t['value']

    cache_type_name = type_name 
    
    # get type that this name references
    t = module['types'][t['type_idx']]
    if t is None:
      continue

    if t['type'] == 'FIELD_LIST':
      if len(t['fields']) == 0:
        print("skipping", type_name, "it has no fields")
        continue
      cache_type_name = '+' + cache_type_name + '|' + str(len(t['fields']))
      found_prefix = False
      found = ''
      for x in generated_type_names:
        if '+' + type_name + '|' in x:
          found_prefix=True
          found = x
      if found_prefix and cache_type_name not in generated_type_names:
        print('CACHEFOUND:', found, cache_type_name)
      
    if cache_type_name in generated_type_names:
      # skip duplicates
      continue

    generated_type_names[cache_type_name] = 1

    if t['type'] == 'ENUM_LIST':
      s = '\ntypedef enum {} {{'.format(type_name)
      first_elem = True
      for k, v in sorted(t['elements'].items(), key=lambda x: x[1]):
        if not first_elem:
          s += ','
        s += '\n{}{} = {}'.format((' ' * INDENT_SPACES), k, v)
        first_elem = False
      s += '\n}} {};\n'.format(type_name)
      enums[type_name] = s
    elif t['type'] == 'FIELD_LIST':
      type_name = get_struct_name(module, t)
      tag_name = get_struct_tag_name(module, t)
      s = 'typedef struct ' + tag_name + ' ' + type_name + ';\n'
      #typedefs[type_name] = s

      s = 'typedef ' + get_type_declaration(module, t, type_name) + ' ' + type_name + ';\n\n'
      structs[type_name] = s
    elif t['type'] == 'SCALAR' or t['type'] == 'NAME':
      s = 'typedef ' + get_type_declaration(module, t, type_name) + ';\n'
      typedefs[type_name] = s
    elif 'PROC' in t['type']:
      s = 'typedef ' + get_type_declaration(module, t, type_name) + ';\n'
      fns[type_name] = s
    elif 'NEAR386' in t['type'] or 'FAR386' in t['type']:
      s = 'typedef ' + get_type_declaration(module, t, type_name) + ';\n'
      typedefs[type_name] = s
    elif 'ARRAY' in t['type']:
      s = 'typedef ' + get_type_declaration(module, t, type_name) + ';\n'
      typedefs[type_name] = s
    else:
      print('ignoring type', type_name, t)

  for type_name, s in typedefs.items():
    if 'br' in type_name:
      br_types_file.write(s)
    else:
      dr_types_file.write(s)
  for type_name, s in enums.items():
    if 'br' in type_name:
      br_types_file.write(s)
    else:
      dr_types_file.write(s)
  for type_name, s in fns.items():
    if 'br' in type_name:
      br_types_file.write(s)
    else:
      dr_types_file.write(s)
  for type_name, s in structs.items():
    if 'br' in type_name:
      br_types_file.write(s)
    else:
      dr_types_file.write(s)


  br_types_file.flush()
  dr_types_file.flush()


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:  # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


read_file()
print('last line:', line_number)
print(len(modules))
# print(len(modules[0]['types']))
# print(len(modules[0]['functions']))
generate_c_skeleton()
#print json.dumps(modules)
