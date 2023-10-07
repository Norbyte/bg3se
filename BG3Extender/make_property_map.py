import re

ns_start_re = r'^BEGIN_NS\(\s*(.*)\s*\)$'
ns_end_re = r'^END_NS\(\s*\)$'
struct_re = r'^(struct|class)\s+(?P<class>[a-zA-Z0-9_]+)(\s*:\s*public\s+(?P<baseclass>[a-zA-Z0-9_]+))?$'
struct_start_re = r'^{$'
struct_end_re = r'^};$'
static_property_re = r'^static\s+constexpr\s+(?P<type>.+)\s+(?P<name>.+)\s*=\s*(?P<value>.+)\s*;?$'
attributes_re = r'^((\[\[[a-zA-Z0-9:_]+\]\]\s*)+)$'
attribute_re = r'\[\[([a-zA-Z0-9:_]+)\]\]'
property_re = r'^(?P<attributes>(\[\[[a-zA-Z0-9:_]+\]\]\s*)*)?(?P<type>.+)\s+(?P<name>[a-zA-Z0-9_]+)\s*;$'

class DefinitionLoader:
    def __init__(self, structs):
        self.cur_ns = ''
        self.ns_stack = []
        self.cur_struct = None
        self.struct_stack = []
        self.next_struct = None
        self.next_struct_base = None
        self.next_attributes = []
        self.structs = structs

    def parse_attributes(self, line):
        for match in re.finditer(attribute_re, line):
            self.next_attributes.append(match.group(1))

    def parse_line(self, line):
        if line == '' or line[0] == '/' or line[0] == '#':
            return
        
        if re.match(ns_end_re, line) is not None:
            self.cur_ns = self.ns_stack.pop()
            return
        
        match = re.match(ns_start_re, line)
        if match is not None:
            self.ns_stack.append(self.cur_ns)
            if self.cur_ns != '':
                self.cur_ns = self.cur_ns + '::' + match.group(1)
            else:
                self.cur_ns = match.group(1)
            return
        
        match = re.match(struct_re, line)
        if match is not None:
            self.next_struct = match.group('class')
            self.next_struct_base = match.group('baseclass')
            return
        
        if self.next_struct is not None and re.match(struct_start_re, line) is not None:
            self.struct_stack.append(self.cur_struct)
            self.ns_stack.append(self.cur_ns)
            if self.cur_ns != '':
                self.cur_ns = self.cur_ns + '::' + self.next_struct
            else:
                self.cur_ns = self.next_struct
            struct = {
                'name': self.cur_ns,
                'base': self.next_struct_base,
                'attributes': self.next_attributes,
                'members': {}
            }
            self.cur_struct = struct
            self.structs[self.cur_ns] = struct
            self.next_struct = None
            self.next_attributes = []
            return
        
        if re.match(struct_end_re, line) is not None:
            self.cur_struct = self.struct_stack.pop()
            self.cur_ns = self.ns_stack.pop()
            return
        
        if re.match(static_property_re, line) is not None:
            return
        
        match = re.match(attributes_re, line)
        if match is not None:
            self.parse_attributes(match.group(1))
            return
        
        match = re.match(property_re, line)
        if match is not None:
            self.parse_attributes(match.group('attributes'))
            self.cur_struct['members'][match.group('name')] = {
                'type': match.group('type'),
                'attributes': self.next_attributes
            }
            self.next_attributes = []
            return
        
        print('UNKNOWN: ', line)

def generate_member(name, member):
    if 'bg3::hidden' in member['attributes']:
        return ''

    if 'bg3::readonly' in member['attributes']:
        return 'P_RO(' + name + ')\n'
    
    return 'P(' + name + ')\n'

def generate_property_map(struct):
    pm = 'BEGIN_CLS(' + struct['name'] + ')\n'
    if struct['base']:
        pm += 'INHERIT(' + struct['base'] + ')\n'
    for name,member in struct['members'].items():
        pm += generate_member(name, member)
    pm += 'END_CLS()\n'
    return pm


sources = [
    'GameDefinitions/Components/Spell.h'
]

structs = {}

for source in sources:
    loader = DefinitionLoader(structs)
    with open(source, 'r') as header:
        for line in header.readlines():
            line = line.strip()
            loader.parse_line(line)

propmap = ''
component_names = ''
for n,struct in structs.items():
    if 'bg3::hidden' not in struct['attributes']:
        propmap += generate_property_map(struct) + '\n'
        if struct['base'] == 'BaseComponent':
            component_names += 'T(' + n + ')\n'


with open('GameDefinitions/PropertyMaps/Generated.inl', 'w') as f:
    f.write(propmap)

with open('GameDefinitions/Components/GeneratedComponentTypes.inl', 'w') as f:
    f.write(component_names)
