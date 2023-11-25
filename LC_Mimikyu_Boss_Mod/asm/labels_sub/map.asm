.if _version == 1
    @LANS_ROOM_ORIGINAL_NPC equ 0xAA318
    @LANS_ROOM_FL_INIT_RESUME1 equ 0xAA748
    @LANS_ROOM_FL_INIT_RESUME2 equ 0xAA728
    @LANS_ROOM_FL_INIT_END equ 0xAA9CC
    @LANS_ROOM_FL_UPDATE_RESUME1 equ 0x0AB468
    @LANS_ROOM_FL_UPDATE_RESUME2 equ 0xAA9D8
    @LANS_ROOM_FL_UPDATE_END equ 0xAAA08
    @ENEMY_ID equ 0x0107
.else
    @LANS_ROOM_ORIGINAL_NPC equ 0xAAD80
    @LANS_ROOM_FL_INIT_RESUME1 equ 0xAB1A8
    @LANS_ROOM_FL_INIT_RESUME2 equ 0xAB188
    @LANS_ROOM_FL_INIT_END equ 0xAB42C
    @LANS_ROOM_FL_UPDATE_RESUME1 equ 0xAB468
    @LANS_ROOM_FL_UPDATE_RESUME2 equ 0xAB438
    @LANS_ROOM_FL_UPDATE_END equ 0xAB69C
    @ENEMY_ID equ 0x125
.endif

; Add Pikachu NPC to Lans Room
.align 4
lans_room_npc_list:
    .word @LANS_ROOM_ORIGINAL_NPC
    .word lans_room_pikachu_npc
    .word 0x000000FF

lans_room_pikachu_npc:
    NPC_MAKE_VISIBLE
    NPC_SET_SPRITE_CHAR 0x31
    NPC_SET_ANIMATION 0x00
    NPC_SET_DIALOG2 0x01
    NPC_SET_POSITION 32, -8, 0x0000
    NPC_NO_TURN_ON_INTERACT
    NPC_JUMP_LINKED @lans_room_pikachu_npc_loop
@lans_room_pikachu_npc_loop:
    NPC_WAIT 0x01
    NPC_JUMP @lans_room_pikachu_npc_loop

; The on init map script
lans_room_init_flag_listener_hook:
    FL_JUMP_IF_FLAG_NOT_ON 0x130F, @@no_pikachu_battle
    FL_JUMP_IF_PREVIOUS_BATTLE_OUTCOME_NOT_EQUAL 0x01, @@failedBattle
    FL_START_CUTSCENE lans_room_after_battle_pikachu_cutscene, 0x00000003
    FL_JUMP @LANS_ROOM_FL_INIT_END
@@failedBattle:
    FL_START_CUTSCENE lans_room_after_battle_pikachu_cutscene, 0x00000004
    FL_JUMP @LANS_ROOM_FL_INIT_END
@@no_pikachu_battle:
;return to regular code
    FL_JUMP_IF_PROGRESS_BETWEEN 0x00, 0x0F, @LANS_ROOM_FL_INIT_RESUME1
    FL_JUMP @LANS_ROOM_FL_INIT_RESUME2


lans_room_after_battle_pikachu_cutscene:
    CS_LOCK_PLAYER
    CS_DECOMPRESS_TEXT 0x02030004
    CS_WAIT 0x1E
    CS_SET_SCREEN_TRANSITION 0x08, 0x08
    CS_WAIT_FOR_SCREEN_TRANSITION
    CS_WAIT 0x1E
    ;Start message based on the first parameter
    CS_START_MESSAGE_VAR 0x04
    ;Wait for dialog to finish
    CS_WAIT_FOR_MESSAGE_PARAMETER 0x80
    CS_UNLOCK_PLAYER
    CS_END

; The constant map script
lans_room_flag_listener_hook:
    FL_JUMP_IF_FLAG_NOT_ON 0x130F, @@no_pikachu_battle
    FL_START_CUTSCENE lans_room_pikachu_cutscene, 0x00000000
    FL_JUMP @LANS_ROOM_FL_UPDATE_END
@@no_pikachu_battle:
;return to regular code
    FL_JUMP_IF_PROGRESS_BETWEEN 0x00, 0x0F, @LANS_ROOM_FL_UPDATE_RESUME1
    FL_JUMP @LANS_ROOM_FL_UPDATE_RESUME2

; Needed for CS_CALL_ASM
.align 8
lans_room_start_pikachu_battle_code_ptr:
    .fill 0x08, 0x00

lans_room_pikachu_cutscene:
    CS_LOCK_PLAYER
    CS_WAIT_FOR_UNPAUSE
    CS_WAIT_FOR_SCREEN_TRANSITION
    CS_WAIT 0x1E
    ;All the real processing is handled by code
    CS_CALL_ASM lans_room_start_pikachu_battle_code_ptr
    CS_UNLOCK_PLAYER
    CS_END

; Battle Structs
.align 4
lans_room_pikachu_battle:
    .byte 0x00 ;Battlefield
    .byte 0x00 ;unknown?
    .byte 0x16 ;Music
    .byte 0x00 ;Battle Mode
    .byte 0x14 ;Background (Graveyard Area)
    .byte 0x00 ;Battle Count
    .byte 0x38 ;Panel pattern
    .byte 0x00 ;unknown?
    .word 0x004198D7
    .word lans_room_pikachu_battle_layout

.align 4
lans_room_pikachu_battle_layout:
    .byte 0x00, 0x22 :: .halfword 0x0000
    .byte 0x11, 0x25 :: .halfword @ENEMY_ID
    .byte 0xF0
;eof