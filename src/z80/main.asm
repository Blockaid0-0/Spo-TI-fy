.nolist
#include "include/ti83plus.inc"
#include "include/dcs7.inc"
.list
   .org progstart
   .db $BB,$6D
Init:
   xor d
   ret
   jr Start

   .dw $0000
   .db $07,$00
   .dw $0000
   .dw $0000
Start:
   ;ld hl, stringName
   ;bcall(_Mov9ToOP1)
   ;bcall(_ChkFindSym)
   ;JR     C, String
   ;bcall(_DelVarArc)

   call OpenGUIStack
   ld hl, mainWindow
   ld de, mainWindowE-mainWindow
   ld a, GUIRLargeWin
   call PushGUIStack

   ld hl, firstText
   ld de, firstTextE-firstText
   ld a, GUIRText
   call PushGUIStack

   ld hl, secondText
   ld de, secondTextE-secondText
   ld a, GUIRText
   call PushGUIStack

   ld hl, thirdText
   ld de, thirdTextE-thirdText
   ld a, GUIRText
   call PushGUIStack

   call RenderGUI
   call PopGUIStacks
   bcall(_GetKey)
   cp $09
   jp z, exit
   jp Start

mainWindow:
	.db %01111000
	.db %01001000
	.db %01001000
	.db %01001000
	.db %11011000
	.db " Spo(TI)fy",0
mainWindowE:
firstText:
	.db 4
	.db 2
	.db 0
	.db "Song",0
firstTextE:
secondText:
	.db 4
	.db 10
	.db 0
	.db "Artist",0
secondTextE:
thirdText:
	.db 4
	.db 18
	.db 0
	.db "Time",0
thirdTextE:
;String:
;   ld hl, stringDataE-stringData
;   bcall(_CreateStrng)
;   inc de
;   inc de
;   ld hl, stringData
;   ld bc, stringDataE-stringData
;   ldir
;   ret
;stringName:
;   .db StrngObj,tVarStrng,tStr1,0
;stringData:
;   .dw "FINALLY",0
;stringDataE:
exit:
    ret
    call CloseGUIStack