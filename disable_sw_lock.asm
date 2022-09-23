; Nick Bild
; September 2022
; Transfer a block of memory from one location to another.

	.cr     z80

    ; push af

    xor a
    out ($ff),a

    ; pop af

	ret

; poke 14510,63
; poke 14412,63
; poke 14530,63

; 10 print 1
; run

; poke 15250,175
; poke 15251,211
; poke 15252,255
; poke 15253,201

; poke 14341,59
; poke 1434,146
; x=usr(0)

; poke 14510,191
; poke 14412,191
; poke 14530,191

; run
