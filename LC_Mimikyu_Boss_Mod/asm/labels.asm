.gba
.relativeinclude on

.if _version == 1
    @OUTPUT_FILE equ "out/labels_f.bin"
    @FREESPACE_START equ 0xF2B00
.else
    @OUTPUT_FILE equ "out/labels.bin"
    @FREESPACE_START equ 0xF2B00
.endif

.include "labels_macros/version_macros.asm"
.include "labels_macros/npc_macros.asm"
.include "labels_macros/map_flag_listener_macros.asm"
.include "labels_macros/map_cutscene_macros.asm"

.include "labels_macros/defines.asm"

.create @OUTPUT_FILE, @FREESPACE_START
    .include "labels_sub/map.asm"
    .include "labels_sub/mimikyu_boss.asm"
    .include "labels_sub/objects.asm"
.close
;eof