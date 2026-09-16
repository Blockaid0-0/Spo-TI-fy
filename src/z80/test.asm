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
    ld hl, stringName
   bcall(_Mov9ToOP1)
   bcall(_ChkFindSym)
   JR     C, next
   bcall(_DelVarArc)
   
next:
   ld hl, stringDataE-stringData
   bcall(_CreateStrng)
   inc de
   inc de
   ld hl, stringData
   ld bc, stringDataE-stringData
   ldir
   bcall(_DispHL)
   ret
stringName:
   .db StrngObj,tVarStrng,tStr1,0
stringData:
   .db "FINALLY"
stringDataE: