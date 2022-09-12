#!/bin/bash
# Usage:
# ./assemble.sh [file.asm]

# Assemble code.
~/software/sbasm/sbasm > out.txt <&1 $1
# Parse output.
python3 parse_z80.py
