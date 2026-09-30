.nolist
#include "include/ti83plus.inc"
#include "include/dcs7.inc"
.list
   .org progstart
   .db $BB,$6D

Init:
   xor d
   ret
   jp Start

   .dw $0000         
   .db $07,$00       
   .dw $0000         
   .dw $0000         
Start:
   bcall(_RunIndicOff)
   jp TestStr1

TestStr1:
   ld hl, stringOne
   rst rMOV9TOOP1
   bcall(_ChkFindSym)
   jp C, MakeStr1
   bcall(_DelVarArc)
   jp TestStr1

TestStr2:
   ld hl, stringTwo
   rst rMOV9TOOP1
   bcall(_ChkFindSym)
   jp C, MakeStr2
   bcall(_DelVarArc)
   jp TestStr2

TestStr3:
   ld hl, stringThree
   rst rMOV9TOOP1
   bcall(_ChkFindSym)
   jp C, MakeStr3
   bcall(_DelVarArc)
   jp TestStr3
   
TestA:
   ld hl, RealA
   rst rMOV9TOOP1
   bcall(_ChkFindSym)
   jp C, MakeA
   bcall(_DelVarArc)
   jp TestA
TestB:
   ld hl, TestB
   rst rMOV9TOOP1
   bcall(_ChkFindSym)
   jp C, MakeB
   bcall(_DelVarArc)
   jp TestB

MakeStr1:
   ld hl, stringOne
   rst rMOV9TOOP1
   ld hl, stringDataOneE-stringDataOne
   bcall(_CreateStrng)
   inc de
   inc de
   ld hl, stringDataOne
   ld bc, stringDataOneE-stringDataOne
   ldir
   jp TestStr2
MakeStr2:
   ld hl, stringTwo
   rst rMOV9TOOP1
   ld hl, stringDataTwoE-stringDataTwo
   bcall(_CreateStrng)
   inc de
   inc de
   ld hl, stringDataTwo
   ld bc, stringDataTwoE-stringDataTwo
   ldir
   jp TestStr3
MakeStr3:
   ld hl, stringThree
   rst rMOV9TOOP1
   ld hl, stringDataThreeE-stringDataThree
   bcall(_CreateStrng)
   inc de
   inc de
   ld hl, stringDataThree
   ld bc, stringDataThreeE-stringDataThree
   ldir
   jp TestA
MakeA:
   ld hl, RealA
   rst rMOV9TOOP1
   ld hl, RealADataE-RealAData
   bcall(_CreateReal)
   inc de
   inc de
   ld hl, RealAData
   ld bc, RealADataE-RealAData
   ldir
   jp TestB
MakeB:
   ld hl, RealB
   rst rMOV9TOOP1
   ld hl, RealBDataE-RealBData
   bcall(_CreateReal)
   inc de
   inc de
   ld hl, RealBData
   ld bc, RealBDataE-RealBData
   ldir
   jp RenderLoop

RenderLoop:
   call OpenGUIStack
   ld hl, mainWindow
   ld de, mainWindowE-mainWindow
   ld a, GUIRLargeWin
   call PushGUIStack

   ld hl, stringOne
   rst rMOV9TOOP1
   call LinkSend
   jp nz,LinkFailureOne

   ld hl, firstText
   ld de, firstTextE-firstText
   ld a, GUIRText
   call PushGUIStack

   ld hl, stringOne
   rst rMOV9TOOP1
   call LinkRecv
   jp nz,LinkFailureOne
   rst rFindSym
   jp c,LinkFailureOne
   ld bc,TempString-TempStringE
   call SizeTokStr_FromStrng
   ld hl,TempString
   ex de,hl
   call ConvTok_FromStrng
   ld de, StringSto1
   bcall(_strcopy)

   ld hl, stringTwo
   rst rMOV9TOOP1
   call LinkSend
   jp nz,LinkFailureTwo

   ld hl, secondText
   ld de, secondTextE-secondText
   ld a, GUIRText
   call PushGUIStack

   ld hl, stringTwo
   rst rMOV9TOOP1
   call LinkRecv
   jp nz,LinkFailureTwo
   rst rFindSym
   jp c,LinkFailureTwo
   ld bc,TempString-TempStringE
   call SizeTokStr_FromStrng
   ld hl,TempString
   ex de,hl
   call ConvTok_FromStrng
   ld de, StringSto2
   bcall(_strcopy)

   ld hl, stringThree
   rst rMOV9TOOP1
   call LinkSend
   jp nz,LinkFailureThree

   ld hl, thirdText
   ld de, thirdTextE-thirdText
   ld a, GUIRText
   call PushGUIStack

   ld hl, stringThree
   rst rMOV9TOOP1
   call LinkRecv
   jp nz,LinkFailureThree
   rst rFindSym
   jp c,LinkFailureThree
   ld bc,TempString-TempStringE
   call SizeTokStr_FromStrng
   ld hl,TempString
   ex de,hl

   call ConvTok_FromStrng
   ld de, StringSto3
   bcall(_strcopy)

   ld hl, barBorder
   ld de, barBorderE-barBorder
   ld a, GUIRBorder
   call PushGUIStack

   call barMath
   ld a, e
   cp 0
   call z, fixBar
   ld (barCompleted), a 

   ld hl, barLine
   ld de, barLineE-barLine
   ld a, GUIRRect
   call PushGUIStack

   ld hl, RealB
   rst rMOV9TOOP1
   call LinkSend
   jp nz,LinkFailureB

   ld hl, RealB
   rst rMOV9TOOP1
   call LinkRecv
   jp nz,LinkFailureB

   call detectPlay

   ld hl, playingInd
   ld de, playingIndE-playingInd
   ld a, GUIRSprite
   call PushGUIStack

   call RenderGUI
   ld b, 6
   call PopGUIStacks

   bcall(_GetCSC)
   cp skClear
   jp z, exit

   jp RenderLoop

LinkFailureOne:
   ld hl,stringOne
   rst rMov9ToOP1
   rst rFindSym
   jp c,LinkFail_NoDeleteStr
   bcall(_DelVarArc)
LinkFailureTwo:
   ld hl,stringTwo
   rst rMov9ToOP1
   rst rFindSym
   jp c,LinkFail_NoDeleteStr
   bcall(_DelVarArc)
LinkFailureThree:
   ld hl,stringThree
   rst rMov9ToOP1
   rst rFindSym
   jp c,LinkFail_NoDeleteStr
   bcall(_DelVarArc)
LinkFailureA:
   ld hl,RealA
   rst rMov9ToOP1
   rst rFindSym
   jp c,LinkFail_NoDeleteReal
   bcall(_DelVarArc)
LinkFailureB:
   ld hl,RealB
   rst rMov9ToOP1
   rst rFindSym
   jp c,LinkFail_NoDeleteReal
   bcall(_DelVarArc)

TempString:
   .block 64
TempStringE:

LinkFail_NoDeleteStr:
   ld hl,11
   push hl
   bcall(_CreateStrng)
   inc de
   inc de
   ld hl,exitFailed
   pop bc
   ldir
LinkFail_NoDeleteReal:
   ld hl,11
   push hl
   bcall(_CreateReal)
   inc de
   inc de
   ld hl,exitFailed
   pop bc
   ldir
barMath:
   ld hl, RealA
   rst rMOV9TOOP1
   call LinkSend
   jp nz,LinkFailureA
   
   ld hl, RealA
   rst rMOV9TOOP1
   call LinkRecv
   jp nz,LinkFailureA

   ld hl, RealA
   rst rMOV9TOOP1
   bcall(_RclVarSym)

   ld a, 100
   bcall(_OP2SetA)
   bcall(_fpdiv)

   ld a, 84
   bcall(_OP2SetA)
   bcall(_FPMult)

   bcall(_int)

   bcall(_ConvOP1)
   ret

fixBar:
   ld a, 1
   ret
detectPlay:
   ld hl, RealB
   rst rMOV9TOOP1
   bcall(_RclVarSym)
   bcall(_ConvOP1)
   ld a, e
   or a
   jp z, loadPause
   jp loadPlay
   ret
loadPause:
   ld hl, Paused
   ld de, playingMode
   ld bc, 5
   ldir
   ret
loadPlay:
   ld hl, Playing
   ld de, playingMode
   ld bc, 5
   ldir
   ret
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
StringSto1:
   .block 64
firstTextE:
secondText:
	.db 4
	.db 10
	.db 0
StringSto2:
   .block 64
secondTextE:
thirdText:
	.db 4
	.db 18
	.db 0
StringSto3:
   .block 64
thirdTextE:
barBorder:
  .db 4
  .db 38
  .db 85
  .db 4
  .db $FF
barBorderE:
barLine:
  .db 5
  .db 39
barCompleted:
  .block 1
  .db 3
  .db $FE
barLineE:
playingInd:
   .db 42
   .db 46
   .db 5
playingMode:
   .block 5
playingIndE:

Paused:
   .db %00110000
	.db %00111000
	.db %00111100
	.db %00111000
	.db %00110000
Playing:
   .db %01100110
	.db %01100110
	.db %01100110
	.db %01100110
	.db %01100110

ConvTok_FromStrng:
   push de
   ld c,(hl)
   inc hl
   ld b,(hl)
   inc hl
ConvTok_FromStrng_Loop:
   ld a,c
   or b
   jp z,ConvTok_FromStrng_Exit
   push bc
   push hl
   push de
   bcall(_Get_Tok_Strng)
   ld hl,OP3
   pop de
   ldir
   pop hl
   ld a,(hl)
   bcall(_isA2ByteTok)
   pop bc
   jp nz,ConvTok_FromStrng_OneByte
   dec bc
   inc hl
ConvTok_FromStrng_OneByte:
   inc hl
   dec bc
   jp ConvTok_FromStrng_Loop
ConvTok_FromStrng_Exit:
   xor a
   ld (de),a
   ex de,hl
   pop hl
   ret
ConvTok:
   push de
Minus:
   push hl
   push de
   bcall(_Get_Tok_Strng)
   pop de
   ldir
   pop hl
   ld a,(hl)
   bcall(_isA2ByteTok)
   jp nz,ConvTok_OneByte
   inc hl
ConvTok_OneByte:
   inc hl
   ld a,(hl)
   or a
   jp nz,Minus
   xor a
   ld (de),a
   ex de,hl
   pop hl
   ret

SizeTokStr:
   push de ; points to tokens, save a second FindSym call by saving
   push bc ; save our memory size for the end, we need BC for other things
   ex de,hl ; _GetTokLen works off of (de)
   ld bc,0 ; count our bytes in this 16-bit register
SizeTokStr_Loop:
   ld a,(de) ; put byte we're pointing to in accumulator
   or a ; set flags, we look for zero flag
   jp z,SizeTokStr_SizeCompare ; if we find (de)==0, we're done
   call SizeTokStr_Count ; bc += len((de)), de += len(token)
   jp SizeTokStr_Loop ; move on to the next token
SizeTokStr_SizeCompare:
   ; grab original memory size in hl, bc to de, return CpHLDE
   pop hl ; we need our memory size we pushed to stack way back at the start
   dec hl ; set aside a byte for the null terminator
   ld e,c ; we don't need de as a pointer anymore, and we can only compare
   ld d,b ; hl to de and not hl to bc (via bcall at least)
   bcall(_CpHLDE) ; NC if de < hl
   pop de ; restore token pointer
   ret


SizeTokStr_FromStrng:
   ; Checks if we'll run out of memory when running ConvTok.
   ; This version accepts FindSym's output instead of zero-terminated tokens.
   ; Inputs: de points to string data, beginning with length bytes
   ;         bc contains size to compare against
   ; Outputs: NC if tokens fit in memory.
   ;          bc = length of tokens
   ;          hl = size to compare against
   ; Destroys: all except de
   push de ; points to tokens, save a second FindSym call by saving
   push bc ; save our memory size for the end, we will clobber BC
   ld a,(de) ; low byte of size...
   ld l,a ; ... goes into low byte of hl
   inc de ; advance the pointer
   ld a,(de) ; high byte of size...
   ld h,a ; ... goes into high byte of hl
   inc de ; de now points to series of tokens
   ld bc,0 ; reset the string length counter
SizeTokStr_FromStrng_Loop:
   xor a ; zero out accumulator
   ld a,l ; get l into accumulator
   or h ; superimpose h onto accumulator and set flags
   jp z,SizeTokStr_SizeCompare ; length has reached zero, exit
   ; otherwise, Str1 contains unconverted tokens
   call SizeTokStr_Count ; bc += len((de)), de += len(token)
   dec hl
   jp SizeTokStr_FromStrng_Loop ; move on to the next token
   ; when loop completes, hl == 0, (de) == end of string, bc == length of tokens,
   ; and (sp) == memory size pushed to stack at beginning
   ; this is the same exit condition as ConvTok_SizeCheck, so we reuse its code

SizeTokStr_Count:
   ; Takes the length of a token in (de) and adds it to bc.
   ; Inputs: de contains pointer to a token or 2-byte token
   ; Outputs: de points to next token
   ;          bc is updated with new string length
   ;          a contains length of token
   ; Destroys: all except hl
   push hl ; contains remaining token count for StrngObj format
   push bc ; current count, clobbered by _GetTokLen. must be last on stack
   push de ; we need de for _GetTokLen, so we need the pointer somewhere else
   pop hl  ; hl will contain our working pointer to token
   ld a,(hl) ; peek at current token
   bcall(_IsA2ByteTok) ; preserve registers, Z if token is 2-byte prefix
   ; if it's a 2-byte token, the prefix goes in d and the suffix in e
   ; if it's a 1-byte token, d must be 0 and e contains the token
   ld d,0
   ld e,a
   jp nz,SizeTokStr_Count_OneByte
   ld d,e
   inc hl
   ld e,(hl)
SizeTokStr_Count_OneByte:
   inc hl ; hl now points to the next token, which is de's return value
   push hl ; keep hl from being clobbered
   bcall(_GetTokLen) ; token in de -> length in accumulator, regs destroyed
   pop hl ; hl still contains next token pointer
   pop bc ; bc now contains our string length count. add a to it for result.
   ld e,a ; might be worth saving the length so we can return it
   ; 8-to-16 addition: bc += a
   add a,c ; simple 8-bit addition setting carry
   ld c,a ; copy result to c preserving flags
   adc a,b ; a contains sum of b, c, and carry
   sub c ; remove c from sum of b, c, and carry
   ld b,a ; set high to b + carry
   ex de,hl ; hl contains de return value, move it to de. l contains token length
   ld a,l
   pop hl ; restore original hl content
   ret
LinkRecv:
   bcall(_AppGetCBL) ; request Str1 (from OP1) from the attached CBL2/LabPro
   bit ComFailed, (iy+getSendFlg) ; getSendFlg.ComFailed set if failed
   ret
LinkSend:
   bcall(_SendVarCmd) ; try to send list from OP1
   bit ComFailed, (iy+getSendFlg) ; getSendFlg.ComFailed set if failed
   ret

stringOne:
   .db StrngObj,tVarStrng,tStr1,0
stringDataOne:
   .db "STR1"
stringDataOneE:
stringTwo:
   .db StrngObj,tVarStrng,tStr2,0
stringDataTwo:
   .db "STR2"
stringDataTwoE:
stringThree:
   .db StrngObj,tVarStrng,tStr3,0
stringDataThree:
   .db "STR3"
stringDataThreeE:
RealA:
   .db RealObj,"A",0
RealAData:
   .db 0
RealADataE:
RealB:
   .db RealObj,"B",0
RealBData:
   .db 1
RealBDataE:

LinkFailText:
 .db "LINK FAILED",0 ; this is actually a string of tokens, not ASCII
exitFailed:
   call CloseGUIStack
   bcall(_ClrLCDFull)
   ld a, 0
   ld (CurRow), a
   ld (CurCol), a
   ld hl, LinkFailText
   bcall(_PutS)
   ret
exit:
   call CloseGUIStack
   ret
    