import os, re

event_re = r'^EVENT(?P<type>[A-Z_]*)\((?P<args>[^)]+)\)$'

class EventsPreprocessor:
    def __init__(self):
        self.lines = ''
        self.next_event_id = 0

    def load_file(self, path):
        with open(path, 'r') as header:
            for line in header.readlines():
                line = line.strip()
                self.lines += self.preprocess_line(line) + "\n"

    def preprocess_line(self, line):
        match = re.match(event_re, line)
        if match is not None:
            event_id = self.next_event_id
            self.next_event_id += 1
            return 'DECLARE_EVENT' + match.group('type') + '(' + str(event_id) + ', ' + match.group('args')+ ')'
        
        if line:
            print("Unrecognized event definition: " + line)
        return ""


def get_most_recent_change():
    return os.path.getmtime('GameDefinitions/Events.inl')


def get_definitions_generation_date():
    enums_path = 'GameDefinitions/Generated/Events.inl'
    if os.path.exists(enums_path):
        return os.path.getmtime(enums_path)
    else:
        return 0

if get_most_recent_change() < get_definitions_generation_date():
    print("No event definition changes detected (based on file modification time); skipping generation step")
    exit(0)


preprocessor = EventsPreprocessor()
preprocessor.load_file('GameDefinitions/Events.inl')

with open('GameDefinitions/Generated/Events.inl', 'w') as f:
    f.write(preprocessor.lines)

events_meta = '#pragma once\n'
events_meta += 'BEGIN_SE()\n'
events_meta += 'static constexpr int EventRegistrySize = ' + str(preprocessor.next_event_id) + ';\n'
events_meta += 'END_SE()\n'

with open('GameDefinitions/Generated/EventsMeta.h', 'w') as f:
    f.write(events_meta)

