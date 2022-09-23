; Nick Bild
; September 2022
; Transfer a block of memory from one location to another.

	.cr     z80

    ; Starting address of this loader.
    .org 64512

    ; Load initial from and to addresses.
    ; from = 64512 + 20 (size of this loader)
	ld hl,64532
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
    ld a,d
    or e
    jp nz,LOOP1

	ret
