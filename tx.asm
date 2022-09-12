; Nick Bild
; September 2022
; Transfer a block of memory from one location to another.

	.cr     z80

    ; Starting address of this loader.
    .org 49152

    ; Load initial from and to addresses.
    ; from = 49152 + 18 (size of this loader)
	ld hl,49170
	ld bc,16384

    ; Initialize loop.
    ; This is the number of sequential bytes to copy.
    ; 1024 (DP RAM size) - 18 (size of this loader)
    ld de,1006
LOOP1:

    ; Copy data at from address to destination address, via A.
    ld a,(hl)
    ld (bc),a

    ; Move to the next address pair.
    inc hl
    inc bc

    dec de
    jp nz,LOOP1

	ret
