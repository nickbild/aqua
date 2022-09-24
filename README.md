# Aqua

Aqua turns the Mattel Aquarius into a respectable computer by adding 32 KB of RAM and SD card mass storage.

The Mattel Aquarius home computer was released in June of 1983 and had already been discontinued by October of that same year.  The short lifespan of this machine is no big surprise — it is equipped with 4 KB of RAM, which is reduced to 1.7 KB of free RAM after factoring in the reserved video memory and system variables.  It also has a terrible rubber chicklet keyboard that is not quite large enough for adult hands.  But despite these (and many more!) flaws, beyond all reason, I still love this computer so I wanted to give mine a little help.  By adding 32 KB of additional memory it is much more usable, and the SD card reader allows me to write software on a modern machine, then easily load it onto the Aquarius, which means I don't have to try to type in long programs on its awful keyboard (or use the cassette port, which is slow and fails far more than it succeeds).

![](https://raw.githubusercontent.com/nickbild/aqua/main/media/testing_write_sm.jpg)

## How It Works

The Aqua PCB ([KiCad files here](https://github.com/nickbild/aqua/tree/main/aqua_pcb)) is designed to plug into the cartridge port of the Aquarius.  It has a 32 KB SRAM chip and logic chips to map it into the 32K addresses above onboard memory ($4000-$BFFF).  It also has a pin header that gives easy access to the cartridge port's connectors.  These are quite useful since the computer offers hardly any other options for extension, and the cartridge port offers direct, unbuffered access to the address and data bus, and virtually all other signals on the Z80.

Aqua brings the Aquarius closer to being in the same league as other contemporary computers, but there is still no decent way to load programs onto it — and using the computer's keyboard to type them is a terrible experience.  So I built AquaWrite, which connects to Aqua's pin header, to load programs/data into memory on the Aquarius from an SD card.  Using AquaWrite, it is possible to write and assemble programs on a modern computer, then write the machine code, as a sequence of newline-delimited bytes, to a text file.  That card can be inserted into AquaWrite and loaded into memory in a few seconds.

AquaWrite ([KiCad files here](https://github.com/nickbild/aqua/tree/main/aqua_write_pcb)) uses an Arduino Mega 2560 ([Arduino code here](https://github.com/nickbild/aqua/tree/main/aqua_write)) to handle SD card access and writing that data to a 1 KB dual-port SRAM chip.  That SRAM chip is mapped to from addresses $FC00-$FFFF, and is used to transfer programs of any size into lower memory addresses in 1 KB pages.  The Arduino first writes a small (20 byte) program in machine code to the SRAM, then the next 1,004 bytes to be transferred.  When ready, the user is prompted to run a `USR` call that executes the machine code and transfers the bytes to a lower memory address.  This is done because the Aquarius prevents access to addresses in the range of this SRAM from BASIC.  If the data to transfer exceeds 1004 bytes, the user presses a button on AquaWrite to load up each additional segment, then again runs `USR` to move the data to the next set of memory addresses.  Any size program (up to the limits of the Aquarius' memory) can be loaded in this way.

This is a bit more complicated because the Aquarius uses an early form of copy protection for external devices (anything that is not onboard the stock computer).  A PLA sits between the Z80 and address bus that XORs addresses with a software lock code (initialized to a random value on startup), which would scramble any values loaded via SD.  So before using AquaWrite for the first time after start up, a sequence of commands need to run to disable this software lock:

```
poke 14510,63
poke 14412,63
poke 14530,63

10 print 1
run

poke 15250,175
poke 15251,211
poke 15252,255
poke 15253,201

poke 14341,59
poke 14340,146
x=usr(0)

poke 14510,191
poke 14412,191
poke 14530,191

run
```

This relocates the stack and other system variables to onboard memory (before the lock code is changed and they get scrambled), then runs a simple program that writes a `0` to the I/O port at $FF that stores the software lock code.  Since data is encoded/decoded by XORing it with the lock code, setting that code to `0` effectively turns it off — the result is always the same as the initial value.

Aqua:
![](https://raw.githubusercontent.com/nickbild/aqua/main/aqua_pcb/breakout_board.svg)

AquaWrite:
![](https://raw.githubusercontent.com/nickbild/aqua/main/aqua_write_pcb/aqua_write_pcb.svg)

## Media

The Aqua board, which adds 32 KB of SRAM and breaks the cartridge port connectors out into a pin header for easy access:

![](https://raw.githubusercontent.com/nickbild/aqua/main/media/aqua_pcb_sm.jpg)

![](https://raw.githubusercontent.com/nickbild/aqua/main/media/aqua_pcb_inserted_sm.jpg)

![](https://raw.githubusercontent.com/nickbild/aqua/main/media/aqua_pcb_inserted_zoom_out_sm.jpg)

AquaWrite, which enables loading data from an SD card to Aquarius RAM:

![](https://raw.githubusercontent.com/nickbild/aqua/main/media/aqua_write_sm.jpg)

![](https://raw.githubusercontent.com/nickbild/aqua/main/media/aqua_write_close_sm.jpg)

## Bill of Materials

Aqua:
- Aqua PCB ([KiCad files here](https://github.com/nickbild/aqua/tree/main/aqua_pcb))
- 1 x 32 KB SRAM chip
- 1 x 74LS00
- 1 x 74LS86
- 1 x 74LS04
- 4 x 0.1 uF ceramic capacitors
- 1 x 1000 uF electrolytic capacitor
- 2 x 10mm M3 standoffs
- 2 x 15mm M3 standoffs
- 44 x male pin headers

AquaWrite:
- 1 x CY7C130-55PC 1 KB dual-port SRAM
- 1 x 74LS682
- 1 x 74LS32
- 1 x Arduino Mega 2560
- 1 x SD card breakout board
- 1 x 0.1 uF ceramic capacitors
- 1 x 47 uF electrolytic capacitor
- 1 x 10K ohm resistor
- 1 x pushbutton

## About the Author

[Nick A. Bild, MS](https://nickbild79.firebaseapp.com/#!/)
