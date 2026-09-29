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
   ld hl, RealB
   bcall(_Mov9ToOP1)
   bcall(_ChkFindSym)
   JR     C, next
   bcall(_DelVarArc)
   jp Start   
next:
   bcall(_CreateReal)
   bcall(_OP1Set1)
   bcall(_PushRealO1)
   bcall(_ZeroOP1)
   ld a,'B'
   ld (OP1+1), a
   bcall(_StoOther)
   ret
RealB:
   .db RealObj,"B",0,0
;RealBData:
;   .db $23,$45,$00,$00,$00,$00,$00,$82,$80
;RealBDataE: