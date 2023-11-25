.gba
.relativeinclude on

.create "out/rom.bin", 0x087FF500
    .include "rom_sub/sprite.asm"
.close
;eof