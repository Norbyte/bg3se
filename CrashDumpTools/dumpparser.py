import re
from operator import itemgetter

files = {}
stacks = {}

dumpRe = re.compile("^Loading Dump File \\[(.*)\\]$")
frameRe = re.compile("^[0-9a-f]+`[0-9a-f]+ [0-9a-f-]+`[0-9a-f-]+     (.*)$")
ptrRe = re.compile("^0x[0-9a-f]+`[0-9a-f]+$")

with open('dump.txt', encoding="latin-1") as f:
    dump = f.read()

curFile = None

total = 0
for line in dump.split("\n"):
    fn = dumpRe.match(line)
    if fn is not None:
        curFile = []
        files[fn.group(1)] = curFile
        total = total + 1

    frame = frameRe.match(line)
    if frame is not None and curFile is not None:
        if ptrRe.match(frame.group(1)) is not None:
            curFile.append("(unknown ptr)")
        else:
            curFile.append(frame.group(1))

total_stks = 0
for fn,stack in files.items():
    stkText = ";".join(stack[0:5])
    stk = stacks.get(stkText)
    total_stks = total_stks + 1
    if stk is None:
        stk = {
            "refs": 1,
            "stack": stack[0:5],
            "dmps": [fn]
        }
        stacks[stkText] = stk
    else:
        stk["refs"] = stk["refs"] + 1
        stk["dmps"].append(fn)

sortedStks = sorted(stacks.values(), key=itemgetter("refs"), reverse=True)

total_crashes = 0
extender_crashes = 0
for stk in sortedStks:
    is_extender = False
    for line in stk["stack"]:
        if "BG3ScriptExtender" in line and "WrappableFunction" not in line:
            is_extender = True
            
    total_crashes += stk["refs"]
    if is_extender:
        extender_crashes += stk["refs"]
    
print(str(total_crashes) + " crashes, " + str(extender_crashes) + " may be extender crash")

for stk in sortedStks:
    print("Occurrences: " + str(stk["refs"]))
    print("Dump: " + stk["dmps"][0])
    print("\n".join(stk["stack"]))
    print("")

