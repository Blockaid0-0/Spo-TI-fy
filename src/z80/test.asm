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
   bcall(_RunIndicOff)
   ld hl, stringName
   bcall(_Mov9ToOP1)
   bcall(_AppGetCbl)
   bit comFailed, (iy+getSendFlg)
   JR nz, exitB
   jr exitG
;next:
;   ld hl, stringDataE-stringData
;   bcall(_CreateStrng)
;   inc de
;   inc de
;   ld hl, stringData
;   ld bc, stringDataE-stringData
;   ldir
;   ret
stringName:
   .db StrngObj,tVarStrng,tStr1,0
;stringData:
;   .db "FINALLY"
;stringDataE:
exitB:
   bcall(_ClrLCDFull)
   ld a, 0
   ld (CurCol), a
   ld (CurCol), a
   ld hl, failed
   bcall(_PutS)
   bcall(_GetKey)
   ret
failed:
   .db "FAILED, Press any key to exit",0
exitG:
   
   bcall(_ClrLCDFull)
   ld a, 0
   ld (CurCol), a
   ld (CurRow), a
   ld hl, success
   bcall(_PutS)
   ld a, 3
   ld (CurRow), a
   ld a, 0
   ld (CurCol), a
   ld hl, stringName
   bcall(_Mov9ToOP1)
   bcall(_FindSym)
   bcall(_PutS)
   bcall(_GetKey)
   ret
success:
   .db "SUCCESS, Press any key to exit",0