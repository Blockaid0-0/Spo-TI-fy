.ifdef TI83
 #include "ti83.inc"
 .binarymode TI83
 .define SafeRam saveSScreen
 .define bcall(xxxx) call xxxx
 ; there's no archive to delete from, so we only need _DelVar
 .define _DelVarArc _DelVar
 .org userMem
 .unsquish
.else
 #include "ti83plus.inc"
 .binarymode ti8x
 .define SafeRam saveSScreen
 .org userMem-2
 .db t2ByteTok,tAsmCmp
.endif

.variablename "SLNKDEMO" ; Brass: Program name as it appears in PRGM menu

; Brass: define variable(s) in our per-target SafeRam
.varloc SafeRam, 768
.var 64,TempString ; 64 bytes to hold up to 63 chars of null-terminated ASCII

Main:
 ld hl,L1name ; hl points to L1's name
 rst rMov9ToOP1 ; put L1's name in OP1
 rst rFindSym ; _CreateRList doesn't check if L1 already exists.
 jr c,NoDelete ; carry flag is set by FindSym if variable does not exist
 bcall(_DelVarArc) ; delete L1 if it exists, so _CreateRList can work safely
NoDelete: ; L1 surely does not exist by now, and OP1 still contains L1's name
 ld hl,2 ; create a 2-element list, will be {117,1}
 bcall(_CreateRList) ; L1name in OP4, de points to beginning of list data
 ; L1 has correct length bytes, but contains random number contents.
 inc de ; de was ptr to low byte of list size, now ptr to high byte of size
 inc de ; de was ptr to high byte of list size, now ptr to first byte of data
 ld hl,117 ; Command ID for short sensor name, which returns a string
 call WriteListElement ; get 117 as our first element
 ld hl,1 ; Channel ID for our short sensor name command
 call WriteListElement ; get 1 as our second element
 bcall(_OP4ToOP1) ; retrieve list name created by _CreateRList and put into OP1
 call LinkSend ; send it over the link port and check if it was sent
 jr nz,LinkFailure ; generate a message and exit if sending failed
 ld hl,Str1name ; we are requesting a string. the process is largely the same
 rst rMov9ToOP1 ; copy name of Str1 to OP1 so _AppGetCBL can see it
 rst rFindSym
 jr c,NoStrDelete1
 bcall(_DelVarArc)
NoStrDelete1:
 call LinkRecv ; request Str1 from CBL2/LabPro and check if we received it
 jr nz,LinkFailure ; generate a message and exit if Str1 wasn't received
 ; currently OP1 contains name of Str1, let's do something with it
 rst rFindSym ; find its data
 jr c,LinkFailure ; this should never happen, but bad things happen if it does
 ld bc,sizeof(TempString) ; Brass: sizeof() returns declared variable size (64)
 call SizeTokStr_FromStrng ; c flag if convtok will overflow, preserve de
 jr c,LinkFailure ; if we will overflow, abort
 ld hl,TempString ; we want ascii at (TempString)
 ex de,hl ; FindSym gave us tokens at (de), ConvTok expects them at (hl)
 call ConvTok_FromStrng ; convert tokens at (hl) to ascii at (de)
 bcall(_PutS)
 ret

LinkFailure: ; we end up here if we failed a link operation
 ld hl,Str1name ; let's leave a message if we didn't succeed
 rst rMov9ToOP1 ; and put that message into Str1
 rst rFindSym   ; Str1 might exist, we use FindSym to find out if it does
 jr c,LinkFail_NoDelete ; don't delete Str1 if it doesn't exist (C set)
 bcall(_DelVarArc) ; if Str1 does exist (C reset), delete it
LinkFail_NoDelete: ; Str1 does not exist, so we are free to make it
 ld hl,11 ; number of token bytes in our string at label LinkFailText
 push hl ; save the length for later, so we can copy using ldir
 bcall(_CreateStrng) ; create string of length 11, contents uninitialized
 ; String data is at de, which starts with two length bytes, low byte first.
 inc de ; de now points to high byte of length
 inc de ; de now points to the actual string data, where we can copy tokens to.
 ld hl,LinkFailText ; hl points to the message we want to copy into Str1
 pop bc ; bc contains our list length that was in hl earlier
 ldir ; copy bc bytes from LinkFailText to data section of Str1
 jr PrintString ; we jump to this so our next action is exiting the program
 
PrintString:
 ; Print the contents of Str1 on the screen.
 ; Inputs: (curRow) contains cursor position
 ; Outputs: Str1 on the display
 ; Destroys: all
 ld hl,Str1name ; we want to display Str1's contents,
 rst rMov9ToOP1 ; so put Str1's name in OP1
 bcall(_FormDisp) ; essentially Disp command on variable name in hl
 ret

ConvTok:
 ; Converts a block of tokens in one memory region to printable text in
 ;   another, zero-terminated
 ; Inputs: hl (start of tokens), de (location to write text)
 ; Outputs: de points to memory after end of tokens, hl points to
 ;   start of ASCII text
 ; Destroys: all
 push de ; save the pointer to text
-:
 push hl ; save current token pointer, as _Get_Tok_Strng will destroy it
 push de ; save location we will copy OP3 to, as _Get_Tok_Strng will destroy it
 bcall(_Get_Tok_Strng) ; OP3 = hl = string; a = bc = length of string;
 ;                        de destroyed
 pop de ; retrieve our write destination
 ldir ; ld (de),(hl) \ inc de \ inc hl \ dec bc until BC=0.
 ;        after this, hl=OP3+bc, de=de+bc
 pop hl ; get our token pointer back. next we need to figure out how far to
 ;          advance this pointer
 ld a,(hl) ; check if we just converted a 2-byte token
 bcall(_isA2ByteTok) ; Z flag if 2-byte token, no register affection
 jr nz,ConvTok_OneByte ; only increment once if not a 2-byte token
 inc hl ; only called if a 2-byte token
ConvTok_OneByte:
 inc hl ; go to the next token
 ld a,(hl) ; peek at our current token...
 or a ; ... and use it to define the Z flag
 jr nz,{-} ; if non-zero, we still have tokens to parse
 xor a ; create a zero in A...
 ld (de),a ; ... so we can zero-terminate the text in de
 ex de,hl ; de now points to end of converted tokens
 pop hl ; hl now points to the start of converted text
 ret ; everything is converted

SizeTokStr:
 ; Checks if we'll run out of memory when running ConvTok.
 ; Inputs: hl points to series of tokens
 ;         bc contains size to compare against
 ; Outputs: NC if tokens fit in memory.
 ;          bc = length of tokens
 ;          hl = size to compare against
 ; Destroys: all except de
 push de ; points to tokens, save a second FindSym call by saving
 push bc ; save our memory size for the end, we need BC for other things
 ex de,hl ; _GetTokLen works off of (de)
 ld bc,0 ; count our bytes in this 16-bit register
SizeTokStr_Loop:
 ld a,(de) ; put byte we're pointing to in accumulator
 or a ; set flags, we look for zero flag
 jr z,SizeTokStr_SizeCompare ; if we find (de)==0, we're done
 call SizeTokStr_Count ; bc += len((de)), de += len(token)
 jr SizeTokStr_Loop ; move on to the next token
SizeTokStr_SizeCompare:
 ; grab original memory size in hl, bc to de, return CpHLDE
 pop hl ; we need our memory size we pushed to stack way back at the start
 dec hl ; set aside a byte for the null terminator
 ld e,c ; we don't need de as a pointer anymore, and we can only compare
 ld d,b ; hl to de and not hl to bc (via bcall at least)
 bcall(_CpHLDE) ; NC if de < hl
 pop de ; restore token pointer
 ret

ConvTok_FromStrng:
 ; Converts a length-prefixed block of tokens in one memory region to  
 ;   printable text in another, zero-terminated
 ; Inputs: hl (start of tokens), de (location to write text)
 ; Outputs: de points to memory after end of tokens, hl points to
 ;   start of ASCII text
 ; Destroys: all
 push de ; save the pointer to text
 ld c,(hl) ; hl points to low byte of length, copy to low byte of bc
 inc hl
 ld b,(hl) ; hl points to high byte of length, copy to high byte of bc
 inc hl ; hl now points at token series
ConvTok_FromStrng_Loop:
 ld a,c ; copy c to accumulator
 or b ; superimpose b over c and set flags
 jr z,ConvTok_FromStrng_Exit ; if both b and c are zero, exit
 push bc ; save counter as _Get_Tok_Strng will destroy it
 push hl ; save current token pointer, as _Get_Tok_Strng will destroy it
 push de ; save location we will copy OP3 to, as _Get_Tok_Strng will destroy it
 bcall(_Get_Tok_Strng) ; OP3 = hl = string; a = bc = length of string;
 ;                        de destroyed
 ld hl,OP3 ; TI's documentation says hl is OP3 by now, but it isn't.
 pop de ; retrieve our write destination
 ldir ; ld (de),(hl) \ inc de \ inc hl \ dec bc until BC=0.
 ;        after this, hl=OP3+bc, de=de+bc
 pop hl ; get our token pointer back. next we need to figure out how far to
 ;          advance this pointer
 ld a,(hl) ; check if we just converted a 2-byte token
 bcall(_isA2ByteTok) ; Z flag if 2-byte token, no register affection
 pop bc ; get our counter back from the stack, dec. by 2 if 2-byte token found
 jr nz,ConvTok_FromStrng_OneByte ; only increment once if not a 2-byte token
 dec bc ; decrement counter by 2 if we have a 2-byte token
 inc hl ; only called if a 2-byte token
ConvTok_FromStrng_OneByte:
 inc hl ; go to the next token
 dec bc ; take one off from the counter
 jr ConvTok_FromStrng_Loop ; at start of loop, we check if counter reached zero
ConvTok_FromStrng_Exit:
 xor a ; create a zero in A...
 ld (de),a ; ... so we can zero-terminate the text in de
 ex de,hl ; de now points to end of converted tokens
 pop hl ; hl now points to the start of converted text
 ret ; everything is converted

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
 jr z,SizeTokStr_SizeCompare ; length has reached zero, exit
 ; otherwise, Str1 contains unconverted tokens
 call SizeTokStr_Count ; bc += len((de)), de += len(token)
 dec hl
 jr SizeTokStr_FromStrng_Loop ; move on to the next token
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
 jr nz,SizeTokStr_Count_OneByte
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

WriteListElement:
 ; Write a value in hl to 9-byte real in (de)
 ; Inputs: hl (value to write), de (where to write 9 bytes)
 ; Outputs: de += 9
 ; Destroys: all except de
 push de ; we needs to preserve de for MovFrOP1's destination
 bcall(_SetXXXXOP2) ; convert value in hl to a 9-byte real, destroys all regs
 bcall(_OP2ToOP1) ; the result is in OP2 and we need it in OP1
 pop de ; get our pointer back
 bcall(_MovFrOP1) ; OP1 -> (de), de += 9
 ret

; TI-83-specific routines -- they have different behavior
.ifdef TI83
 LinkSend:
  bcall(_SendVarCmd)
  xor a ; we cannot detect errors here as there is no ComFailed flag,
  ret   ; so we always return success (Z flag set)
 
 LinkRecv:
 ; Inputs: OP1 contains variable name to get
 ; Outputs: Z flag is set if we received data
 ; Destroys: all
  ; we don't have _AppGetCBL, we have to re-create it from scratch
  set IDis95h,(iy+linkFlags) ; set up the calculator ID to use for the request
  res IDis82h,(iy+linkFlags)
  ld a,(OP1+1) ; check data type
  cp tVarLst ; use $95 for lists to surpass 99 elements; use $82 otherwise
  jr z,{+}
  res IDis95h,(iy+linkFlags) ; use this for lists
  set IDis82h,(iy+linkFlags)
 +:
  di ; don't do this on 82
  res secondTimerEnabled,(iy+interruptFlags) ; don't do this on 82
  ld a,$15
  ld (sendRecState),a ; nobody knows what this does, but it has to be $15
  bcall(_GetVarCmd) ; send a silent link request to CBL-alike device
  ; if this fails, Str1 remains deleted
  ; OP1 should still contain Str1name -- use FindSym to see if Str1 was created
  rst rFindSym ; check if we actually got anything from the remote device
  jr c,LinkRecv_fail ; c flag set if variable does not exist
  xor a ; set Z flag
  ret
  LinkRecv_Fail:
  ld a,1
  or a ; reset Z flag
  ret
.else
 LinkSend:
  bcall(_SendVarCmd) ; try to send list from OP1
  bit ComFailed, (iy+getSendFlg) ; getSendFlg.ComFailed set if failed
  ret
 
 LinkRecv:
  bcall(_AppGetCBL) ; request Str1 (from OP1) from the attached CBL2/LabPro
  bit ComFailed, (iy+getSendFlg) ; getSendFlg.ComFailed set if failed
  ret
.endif

L1name:
 .db ListObj, tVarLst, tL1, 0, 0
Str1name:
 .db StrngObj, tVarStrng, tStr1, 0, 0
LinkFailText:
 .db "LINK",tSpace,"FAILED" ; this is actually a string of tokens, not ASCII

.ifdef TI83
 .squish
 .db tEnd,tEnter,t0,t0,t0,t0,tEnter,tEnd
.endif