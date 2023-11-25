.align 4
mimikyu_ai_struct1:
    .byte 0x08;sprite_category
    .byte 0x11;sprite_index
    .byte 0x01;palettes_per_version
    .byte 0x01;enemy_type
    .byte 0x00;collision_damage
    .byte 0x00;element
    .byte 0x00;secondary_element
    .byte 0x01;has_shadow

.align 4
mimikyu_ai_struct2:
    .halfword 0x0000 | 2242;hp + element
    .byte 0x00;version
    .byte 0x00;collision_flags
    .halfword 0x000A;collision_damage

.align 4
mimikyu_chip_pool:
    .fill 0xC0, 0xFF

; Attack pointers for reference in attack list
.align 8
mimikyu_update_ai_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_move_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_shadowball_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_confuseray_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_shadow_claw_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_shadow_sneak_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_playrough_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_proc_disguise_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_disguise_ptr:
    .fill 0x08, 0x00
.align 8
mimikyu_on_damage_ptr:
    .fill 0x08, 0x00


mimikyu_attack_pool:
;first 8 pointers are usually the same for every navi
    .word emcomm_EmCommonEntry;0
    .word navicomm_NaviCommonMoveInit;1
    .word emcomm_BossCommonOut;2
    .word navicomm_NaviCommonStagger;3
    .word navicomm_NaviCommonPalsy;4
    .word navicomm_NaviCommonBlow;5
    .word navicomm_NaviCommonFreeze;
    .word navicomm_NaviCommonBubble;7
    .word mimikyu_update_ai_ptr;8
    .word mimikyu_move_ptr;9
    .word mimikyu_shadowball_ptr;A
    .word mimikyu_confuseray_ptr;B
    .word mimikyu_shadow_claw_ptr;C
    .word mimikyu_shadow_sneak_ptr;D
    .word mimikyu_playrough_ptr;E
    .word mimikyu_disguise_ptr;F
;eof