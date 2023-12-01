import re

ns_start_re = r'^BEGIN(_BARE)?_NS\(\s*(.*)\s*\)$'
ns_end_re = r'^END(_BARE)?_NS\(\s*\)$'
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

template_re = r'^template\s*<.*>$'
explicit_instantiation_re = r'template\s+(struct|class)\s+(?P<name>[a-zA-Z0-9_:<>*:, ]+)\s*(?P<template_args><[a-zA-Z0-9_<>*:, ]+>);$'

member_function_re = r'^(?P<attributes>(\[\[[a-zA-Z0-9:_]+\]\]\s*)*)\s*(inline\s+)?(?P<retval>[a-zA-Z0-9_:<>*, ]+)\s+(?P<name>[a-zA-Z0-9_]+)\s*\(.*\)\s*(const)?$'

class Structure:
    def __init__(self, ns_stack : list[str], base : str, attributes : list):
        self.name_ns : list[str] = ns_stack.copy()
        self.ns_stack : list[str] = []
        self.base : str = base
        self.attributes : list[str] = attributes.copy()
        self.members = {}
        self.raw_lines : list[str] = []
        self.getters : list[str] = []
        self.setters : list[str] = []

    def name(self) -> str:
        if len(self.name_ns) == 0:
            return ''
        else:
            nsname = self.name_ns[0]
            for ns in self.name_ns[1:]:
                nsname = nsname + '::' + ns
            return nsname
        
    def generate_property_map(self) -> str:
        pm = 'BEGIN_CLS(' + self.name() + ')\n'
        if self.base and not self.base.startswith('ProtectedGameObject<'):
            pm += 'INHERIT(' + self.base + ')\n'
        for name,member in self.members.items():
            pm += generate_member(name, member)
        for line in self.raw_lines:
            pm += line + '\n'
        for name in self.getters:
            if name in self.setters:
                pm += 'P_GETTER_SETTER(' + name + "," + name + "," + name + ")\n"
            else:
                pm += 'P_GETTER(' + name + "," + name + ")\n"
        for name in self.setters:
            if name not in self.getters:
                print("Cannot have setter without corresponding getter for name", name)
        pm += 'END_CLS()\n'
        return pm
    
    def clone(self):
        ret = Structure(self.name_ns, self.base, self.attributes)
        ret.members = self.members.copy()
        ret.raw_lines = self.raw_lines.copy()
        ret.getters = self.getters.copy()
        ret.setters = self.setters.copy()
        return ret
        
class Template(Structure):
    def __init__(self, ns_stack : list[str], base : str, attributes : list):
        Structure.__init__(self, ns_stack, base, attributes)
        self.children : dict[str, Structure] = {}
        self.templates : dict[str, Template] = {}
        self.template_list : list[Template] = []

    def clone(self):
        ret = Template(self.name_ns, self.base, self.attributes)
        ret.members = self.members.copy()
        ret.raw_lines = self.raw_lines.copy()
        ret.getters = self.getters.copy()
        ret.setters = self.setters.copy()
        for k,v in self.children.items():
            ret.children[k] = v.clone()
        for v in self.template_list:
            ret.template_list.append(v.clone())
        for v in ret.template_list:
            nsname = v.name_ns[-1]
            ret.templates[nsname] = v
            for ns in reversed(v.name_ns[:-1]):
                nsname = ns + '::' + nsname
                ret.templates[nsname] = v

        return ret

class DefinitionLoader:
    def __init__(self, structs):
        self.ns_stack : list[str] = []
        self.cur_struct : Structure = None
        self.struct_stack : list[Structure] = []
        self.next_struct : str = None
        self.next_struct_base : str = None
        self.next_attributes : list[str] = []
        self.structs : dict[str, Structure] = structs
        self.template_stack : list[Template] = []
        self.cur_template : Template = None
        self.templates : dict[str, Template] = {}
        self.next_template : bool = False

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
        if self.cur_template is None:
            self.ns_stack.append(ns)
        else:
            self.cur_template.ns_stack.append(ns)

    def exit_ns(self):
        if self.cur_template is None:
            self.ns_stack.pop()
        else:
            self.cur_template.ns_stack.pop()

    def enter_struct_normal(self, name, base, attrs):
        self.enter_ns(name)
        self.struct_stack.append(self.cur_struct)

        struct = None

        if self.next_template:
            struct = Template(self.ns_stack, base, attrs)
            nsname = self.ns_stack[-1]
            self.templates[nsname] = struct
            for ns in reversed(self.ns_stack[:-1]):
                nsname = ns + '::' + nsname
                self.templates[nsname] = struct
            self.cur_template = struct
        else:
            struct = Structure(self.ns_stack, base, attrs)
            self.structs[struct.name()] = struct
        self.cur_struct = struct
        self.next_struct = None
        self.next_attributes = []
        self.next_template = False

    def enter_struct(self, name, base, attrs):
        if self.cur_template is None:
            self.enter_struct_normal(name, base, attrs)
            return
        
        self.enter_ns(name)
        self.struct_stack.append(self.cur_struct)
        
        struct = None
        
        if self.next_template:
            self.template_stack.append(self.cur_template)
            struct = Template(self.cur_template.ns_stack, base, attrs)
            self.cur_template.template_list.append(struct)
            nsname = self.cur_template.ns_stack[-1]
            self.cur_template.templates[nsname] = struct
            for ns in reversed(self.cur_template.ns_stack[:-1]):
                nsname = ns + '::' + nsname
                self.cur_template.templates[nsname] = struct
            self.cur_template = struct
        else:
            struct = Structure(self.cur_template.ns_stack, base, attrs)
            self.cur_template.children[struct.name()] = struct
        self.cur_struct = struct
        self.next_struct = None
        self.next_attributes = []
        self.next_template = False

    def exit_struct(self):
        if self.cur_struct is self.cur_template:
            if len(self.template_stack) != 0:
                self.cur_template = self.template_stack.pop()
            else:
                self.cur_template = None
        self.cur_struct = self.struct_stack.pop()
        self.exit_ns()

    def explicit_instantiate(self, name : str, template_args : str):
        othername = None
        othername = '::' + name

        template = None

        if name in self.templates:
            template = self.templates[name]
        elif othername in self.templates:
            template = self.templates[othername]
        else:
            return False
        
        templclone = template.clone()
        templclone.name_ns[-1] += template_args
        
        structs[templclone.name()] = templclone

        for subtemplate in template.template_list:
            newtemplate = subtemplate.clone()
            for ns in reversed(templclone.name_ns):
                newtemplate.name_ns.insert(0, ns)
            
            nsname = newtemplate.name_ns[-1]
            self.templates[nsname] = newtemplate
            for ns in reversed(newtemplate.name_ns[:-1]):
                nsname = ns + '::' + nsname
                self.templates[nsname] = newtemplate
        
        return True

    def parse_line(self, line):
        if len(line) > 3 and line[0] == '/' and line[1] == '/' and line[2] == '#':
            self.cur_struct.raw_lines.append(line[3:])
            return
        
        if line == '' or line[0] == '/' or line[0] == '#' or re.match(ignore_re, line) is not None:
            return
        
        match = re.match(ns_start_re, line)
        if match is not None:
            self.enter_ns(match.group(2))
            return
        
        match = re.match(template_re, line)
        if match is not None:
            self.next_template = True
            return
        
        match = re.match(explicit_instantiation_re, line)
        if match is not None:
            if self.explicit_instantiate(match.group('name'), match.group('template_args')):
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
            self.cur_struct.members[match.group('name')] = {
                'type': match.group('type'),
                'attributes': self.next_attributes
            }
            self.next_attributes = []
            return

        match = re.match(member_function_re, line)
        if match is not None:
            self.parse_attributes(match.group('attributes'))
            if 'bg3::getter' in self.next_attributes:
                self.cur_struct.getters.append(match.group('name'))
            if 'bg3::setter' in self.next_attributes:
                self.cur_struct.setters.append(match.group('name'))
            return
        
        print('UNKNOWN: ', line)
        self.next_template = False

def generate_member(name, member):
    if 'bg3::hidden' in member['attributes']:
        return ''

    if 'bg3::readonly' in member['attributes']:
        return 'P_RO(' + name + ')\n'
    
    return 'P(' + name + ')\n'


sources = [
    'GameDefinitions/Base/ExposedTypes.h',
    'GameDefinitions/GuidResources.h',
    'GameDefinitions/Module.h',
    'GameDefinitions/Hit.h',
    'GameDefinitions/Interrupt.h',
    'GameDefinitions/RootTemplates.h',
    'GameDefinitions/Components/Boosts.h',
    'GameDefinitions/Components/CharacterCreation.h',
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

structs : dict[str, Structure] = {}

for source in sources:
    loader = DefinitionLoader(structs)
    loader.load_file(source)

propmap = ''
component_names = ''
for n,struct in structs.items():
    if 'bg3::hidden' not in struct.attributes:
        propmap += struct.generate_property_map() + '\n'
        if struct.base == 'BaseComponent':
            component_names += 'T(' + n + ')\n'


with open('GameDefinitions/PropertyMaps/Generated.inl', 'w') as f:
    f.write(propmap)

with open('GameDefinitions/Components/GeneratedComponentTypes.inl', 'w') as f:
    f.write(component_names)
