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

   .dw $0000         ;or .dw $0000 if you don't have a description
   .db $07,$00         ;always this string
   .dw $0000            ;or .dw $0000 if you don't have an icon
   .dw $0000             ;usually .dw $0000 if you don't have or know what an ALE is
Start:                             ;main routines
   call OpenGUIStack
   ld hl, mainWindow
   ld de, mainWindowE-mainWindow
   ld a, GUIRLargeWin
   call PushGUIStack
   call RenderGUI
   call PopGUIStacks
   bcall(_GetKey***REMOVED***
   cp $09
   jp z, exit
   jp Start
mainWindow:
	.db %01111100
	.db %01000100
	.db %01000100
	.db %01000100
	.db %11001100
	.db " Spo(TI***REMOVED***fy",0
mainWindowE:
exit:
    ret
    call CloseGUIStack