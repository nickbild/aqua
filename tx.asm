; Nick Bild
; September 2022
; Transfer a block of memory from one location to another.

	.cr     z80

    ; Starting address of this loader.
    .org 21000

    ; Load initial from and to addresses.
	ld hl,20000
	ld bc,16384

    ; Initialize loop.
    ; This is the number of sequential bytes to copy.
    ld e,#10
LOOP1:

    ; Copy data at from address to destination address, via A.
    ld a,(hl)
    ld (bc),a

    ; Move to the next address pair.
    inc hl
    inc bc

    dec e
    jp nz,LOOP1

	ret
