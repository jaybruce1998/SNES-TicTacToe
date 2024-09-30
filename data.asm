.include "hdr.asm"

.section ".rodata1" superfree

tilefont:
.incbin "pvsneslibfont.pic"

palfont:
.incbin "pvsneslibfont.pal"

character_tiles: .incbin "character.pic"
character_tiles_end:

character_pal: .incbin "character.pal"
character_pal_end:

.ends
