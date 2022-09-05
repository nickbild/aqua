import re


asm_file = "out.txt"
mc = []


for line in open(asm_file):
    line = line.strip()

    # Filter for lines starting with memory addresses.
    if re.match('^.{4}-', line):
        # Remove the memory location.
        line = re.sub('^.{4}-', '', line)
        # Remove everything after the machine code.
        line = re.sub('\s{2,9999}.*$', '', line)

        # Append the machine code to a list, in decimal format.
        for val in line.split():
            val = int(val, 16)  
            mc.append(val)
        
print(mc)
