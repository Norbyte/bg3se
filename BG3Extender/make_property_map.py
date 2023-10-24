import re

ns_start_re = r'^BEGIN_NS\(\s*(.*)\s*\)$'
ns_end_re = r'^END_NS\(\s*\)$'
struct_re = r'^(struct|class)\s+(?P<attributes>(\[\[[a-zA-Z0-9:_]+\]\]\s*)*)?(?P<class>[a-zA-Z0-9_]+)(\s*:(\s*public)?\s+(?P<baseclass>[a-zA-Z0-9_<>:]+))?$'
struct_start_re = r'^{$'
struct_end_re = r'^(};|}\))$'
static_property_re = r'^static\s+constexpr\s+(?P<type>.+)\s+(?P<name>.+)\s*=\s*(?P<value>.+)\s*;?$'
typedef_re = r'^using\s+.*=.*;?$'
attributes_re = r'^((\[\[[a-zA-Z0-9:_]+\]\]\s*)+)$'
attribute_re = r'\[\[([a-zA-Z0-9:_]+)\]\]'
property_re = r'^(?P<attributes>(\[\[[a-zA-Z0-9:_]+\]\]\s*)*)?(?P<type>[a-zA-Z0-9_<>*:, ]+)\s+(?P<name>[a-zA-Z0-9_]+)\s*(?P<initval>{.*})?\s*;\s*(?P<comment>//.*)?$'
tag_component_re = r'^DEFINE_TAG_COMPONENT\((?P<ns>[^,]+), (?P<name>[^,]+), (?P<type>[^,]+)\)$'
boost_re = r'^DEFN_BOOST\(\s*(?P<name>[^,]+),\s*(?P<boostType>[^,]+),\s*{$'
ignore_re = r'^(BEGIN_SE|END_SE).*$'

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

    def load_file(self, path):
        with open(path, 'r') as header:
            for line in header.readlines():
                line = line.strip()
                loader.parse_line(line)
        if len(self.ns_stack) > 0 or len(self.struct_stack) > 0 or len(self.next_attributes) > 0 or self.cur_struct is not None:
            raise Exception("Partially parsed namespace or struct after EOF")

    def parse_attributes(self, line):
        for match in re.finditer(attribute_re, line):
            self.next_attributes.append(match.group(1))

    def enter_ns(self, ns):
        self.ns_stack.append(self.cur_ns)
        if self.cur_ns != '':
            self.cur_ns = self.cur_ns + '::' + ns
        else:
            self.cur_ns = ns

    def exit_ns(self):
        self.cur_ns = self.ns_stack.pop()

    def enter_struct(self, name, base, attrs):
        self.enter_ns(name)
        self.struct_stack.append(self.cur_struct)
        struct = {
            'name': self.cur_ns,
            'base': base,
            'attributes': attrs,
            'members': {}
        }
        self.cur_struct = struct
        self.structs[self.cur_ns] = struct
        self.next_struct = None
        self.next_attributes = []

    def exit_struct(self):
        self.cur_struct = self.struct_stack.pop()
        self.exit_ns()

    def parse_line(self, line):
        if line == '' or line[0] == '/' or line[0] == '#' or re.match(ignore_re, line) is not None:
            return
        
        match = re.match(ns_start_re, line)
        if match is not None:
            self.enter_ns(match.group(1))
            return
        
        if re.match(ns_end_re, line) is not None:
            self.exit_ns()
            return
        
        match = re.match(struct_re, line)
        if match is not None:
            self.parse_attributes(match.group('attributes'))
            self.next_struct = match.group('class')
            self.next_struct_base = match.group('baseclass')
            return
        
        if self.next_struct is not None and re.match(struct_start_re, line) is not None:
            self.enter_struct(self.next_struct, self.next_struct_base, self.next_attributes)
            return
        
        if re.match(struct_end_re, line) is not None:
            self.exit_struct()
            return
        
        match = re.match(tag_component_re, line)
        if match is not None:
            self.enter_struct(match.group('name'), 'BaseComponent', [])
            self.exit_struct()
            return
        
        match = re.match(boost_re, line)
        if match is not None:
            self.enter_struct(match.group('name') + 'BoostComponent', 'BaseComponent', [])
            return
        
        if re.match(static_property_re, line) is not None:
            return
        
        if re.match(typedef_re, line) is not None:
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
    if struct['base'] and not struct['base'].startswith('ProtectedGameObject<'):
        pm += 'INHERIT(' + struct['base'] + ')\n'
    for name,member in struct['members'].items():
        pm += generate_member(name, member)
    pm += 'END_CLS()\n'
    return pm


sources = [
    'GameDefinitions/Base/ExposedTypes.h',
    'GameDefinitions/GuidResources.h',
    'GameDefinitions/Module.h',
    'GameDefinitions/Hit.h',
    'GameDefinitions/Interrupt.h',
    'GameDefinitions/RootTemplates.h',
    'GameDefinitions/Components/Boosts.h',
    'GameDefinitions/Components/Combat.h',
    'GameDefinitions/Components/Components.h',
    'GameDefinitions/Components/Spell.h',
    'GameDefinitions/Components/Data.h',
    'GameDefinitions/Components/Inventory.h',
    'GameDefinitions/Components/Visual.h',
    'GameDefinitions/Components/Passives.h',
    'GameDefinitions/Components/Runtime.h',
    'GameDefinitions/Resources.h'
]

structs = {}

for source in sources:
    loader = DefinitionLoader(structs)
    loader.load_file(source)

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
