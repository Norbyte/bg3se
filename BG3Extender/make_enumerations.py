import os, re

enum_start_re = r'^BEGIN_ENUM(?P<type>[^\s(]*)\((?P<args>[^)]+)\).*$'
bitmask_start_re = r'^BEGIN_BITMASK(?P<type>[^\s(]*)\((?P<args>[^)]+)\).*$'
enum_entry_re = r'^E\(\s*(?P<label>[^\s]+)\s*\).*$'
enum_entry_value_re = r'^\s*EV\(\s*(?P<label>[^\s]+)\s*,\s*(?P<value>[^\s]+)\s*\).*$'

class EnumerationsSharedData:
    def __init__(self):
        self.next_enum_id = 0
        self.next_bitmask_id = 0

class EnumerationsPreprocessor:
    def __init__(self, sd):
        self.lines = ''
        self.next_value = 0
        self.shared = sd

    def load_file(self, path):
        with open(path, 'r') as header:
            for line in header.readlines():
                line = line.strip()
                self.lines += self.preprocess_line(line) + "\n"

    def preprocess_line(self, line):
        match = re.match(bitmask_start_re, line)
        if match is not None:
            self.next_value = 0
            bitmask_id = self.shared.next_bitmask_id
            self.shared.next_bitmask_id += 1
            return 'BEGIN_BITMASK' + match.group('type') + '(' + match.group('args') + ', ' + str(bitmask_id) + ')'
        
        match = re.match(enum_start_re, line)
        if match is not None:
            self.next_value = 0
            enum_id = self.shared.next_enum_id
            self.shared.next_enum_id += 1
            return 'BEGIN_ENUM' + match.group('type') + '(' + match.group('args') + ', ' + str(enum_id) + ')'
        
        match = re.match(enum_entry_value_re, line)
        if match is not None:
            val = match.group('value')
            if len(val) > 2 and val[0] == '0' and val[1] == 'x':
                value = int(val[2:], 16) + 1
            elif val.isnumeric():
                value = int(val) + 1
            else:
                value = None
            self.next_value = value
            return "\t" + line
        
        match = re.match(enum_entry_re, line)
        if match is not None:
            val = self.next_value
            self.next_value += 1
            return '\tEV(' + match.group('label') + ', ' + str(val) + ')'
        
        return line


shared = EnumerationsSharedData()
preprocessor = EnumerationsPreprocessor(shared)
external_preprocessor = EnumerationsPreprocessor(shared)

for file in os.listdir('GameDefinitions/Enumerations'):
    if file.endswith('.inl'):
        preprocessor.load_file('GameDefinitions/Enumerations/' + file)

for file in os.listdir('GameDefinitions/ExternalEnumerations'):
    if file.endswith('.inl'):
        external_preprocessor.load_file('GameDefinitions/ExternalEnumerations/' + file)

cur_enumerations = ''
try:
    if not os.path.exists('GameDefinitions/Generated'):
        os.mkdir('GameDefinitions/Generated')
    with open('GameDefinitions/Generated/Enumerations.inl', 'r') as f:
        cur_enumerations = f.read()
except FileNotFoundError:
    pass
    
cur_extenumerations = ''
try:
    with open('GameDefinitions/Generated/ExternalEnumerations.inl', 'r') as f:
        cur_extenumerations = f.read()
except FileNotFoundError:
    pass

if cur_enumerations != preprocessor.lines:
    with open('GameDefinitions/Generated/Enumerations.inl', 'w') as f:
        f.write(preprocessor.lines)
else:
    print("No enumeration changes detected")

if cur_extenumerations != external_preprocessor.lines:
    with open('GameDefinitions/Generated/ExternalEnumerations.inl', 'w') as f:
        f.write(external_preprocessor.lines)
else:
    print("No external enumeration changes detected")
