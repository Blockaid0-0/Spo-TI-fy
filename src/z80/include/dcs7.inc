;+----------------------------------------+
;|      Doors CS v7.0 Include File        |
;|         TI-83+/TI-84+ Version          |
;|          Revised July 6, 2010          |
;|           By Kerm Martian              |
;|                                        |
;| Please see http://dcs.cemetech.net for |
;| information and updates including a    |
;| full Developers' SDK.                  |
;+----------------------------------------+

;----> Safe Memory Areas
; saferam1 equ  768 bytes (APDRAM)
; saferam2 equ  531 bytes (statRAM)
; saferam3 equ  128 bytes (textMem)
; saferam4 equ  66 bytes (OpXs)
; saferam5 equ  10 bytes (iMathPtrs)

#DEFINE equ .equ
#DEFINE EQU .equ
#DEFINE end .end
#DEFINE END .end

#define bcallz(xxxx) b_callz(xxxx)
#define bcallnz(xxxx) b_callnz(xxxx)
#define bcallc(xxxx) b_callc(xxxx)
#define bcallnc(xxxx) b_callnc(xxxx)
#define bjump(xxxx) b_jump(xxxx)

SEram		equ $86EC
saferam1	equ 9872h
saferam2	equ 8A3Ah
saferam3	equ 8508h
saferam4	equ 8478h
saferamp	equ 9872h
saferamp2	equ 8251h
;+7: cn2_int_recbuf
Cn2_Int_RecBuf equ SavesScreen+5+2						;86F3h
;+270: cn2_int_sendbuf
Cn2_Int_SendBuf equ SavesScreen+256+5+2+7				;87FAh


iversion              equ  $4083
irandom               equ  $4086
isprite               equ  $4089
ilsprite              equ  $408C
igetpix               equ  $408F
ifastcopy             equ  $4092
idetect               equ  $4095
idecomp               equ  $4098

ionversion              equ  $4083
ionrandom               equ  $4086
ionputsprite            equ  $4089
ionlargesprite        equ  $408C
iongetpixel           equ  $408F
ionfastcopy           equ  $4092
iondetect             equ  $4095
iondecompress         equ  $4098

iputsprite            equ  $4089
ilargesprite        equ  $408C
igetpixel           equ  $408F
idecompress         equ  $4098

;========================================================================== ;
;==========================  MirageOS Subroutines ========================= ;
;========================================================================== ;

directin              equ  $409B
sendbytetios          equ  $409E
getbytetios           equ  $40A1
version               equ  $40A4
setvputs              equ  $40A7
setpixel              equ  $40AA
fastcopys             equ  $40AD
delayb                equ  $40B0
multhe                equ  $40B3
multhl                equ  $40B6
quittoshell           equ  $40B9
fastline              equ  $40BC
pixelonhl             equ  $40BF
pixeloff              equ  $40C2
pixelxor              equ  $40C5
pixeltest             equ  $40C8
pixeloffhl            equ  $40CB
pixelxorhl            equ  $40DE
pixeltesthl           equ  $40D1
fastlineb             equ  $40D4
fastlinew             equ  $40D7
fastlinex             equ  $40DA
pointonc              equ  $40DD
pointoffc             equ  $40E0
pointxorc             equ  $40E3
centertext            equ  $40E6
cphlbc                equ  $40E9
putsprite8            equ  $40EC
fastcopyb             equ  $40EF
vputsc                equ  $40F2
scrolld7              equ  $40F5
vnewline              equ  $40F8
rand127               equ  $40FB
disprle               equ  $40FE
cphlde                equ  $4101
screentopic           equ  $4104
fastlined             equ  $4107
disprlel              equ  $410A
getnextgoodprog       equ  $410D
getprevgoodprog       equ  $4110
getnext               equ  $4113
getprev               equ  $4116
compstrs              equ  $4119
nextstr               equ  $411C
getinfo               equ  $411F
fastrectangle         equ  $4122
gettext               equ  $4125
gettextv              equ  $4128
FastRectangle_Save    equ  $412B
vputa                 equ  $412E
runprog               equ  $4131
isgoodprog            equ  $4134
existfold             equ  $4137
delprog               equ  $413A
filledrectangle       equ  $413D
nextfolder            equ  $4140
delfolder             equ  $4143
moveall               equ  $4149
curfoldname           equ  $4149
curfoldnamea          equ  $414C
createfolder          equ  $414F
compstrsn             equ  $4152
folder_menu_start     equ  $4155
options_screen        equ  $4158
put_folder_name_top   equ  $415B
general_key_routine   equ  $415E
find_num_good_progs   equ  $4161
put_scrollbar         equ  $4164
invert_lines          equ  $4167
invert_1_line         equ  $416A
right_align_value     equ  $416D
put_mirageos_header   equ  $4170
put_size_graphic      equ  $4173
sendprog              equ  $4176
hideprog              equ  $4179
arcprog               equ  $417C
filledrectangle_save  equ  $417F
getbytetiosw          equ  $4182
vatswap               equ  $4185
renameprog            equ  $4188
renamefolder          equ  $418B
sysmain               equ  $418E
setupint              equ  $4191
move_gui_prog         equ  $4194
largespritehl         equ  $4197
Update_Scrollbar      equ  $419A
Initial_Scrollbar     equ  $419D
sortallfolds          equ  $41A0
dofoldsort            equ  $41A3
getfoldsort           equ  $41A6
setfoldsort           equ  $41A9
Increase_Cur_Element  equ  $41AC
Decrease_Cur_Element  equ  $41AF
Increase_Max_Elements equ  $41B2
Decrease_Max_Elements equ  $41B5
Add_A_To_Cur_Element  equ  $41B8
Sub_A_From_Cur_Element equ  $41BB
Add_A_To_Max_Elements equ  $41BE
Sub_A_From_Max_Elements equ  $41C1
Skip_Forward_B_From_Top equ  $41C4
Get_Curgoodprog_Ptr   equ  $41C7
getchecksum           equ  $41CA
freearc               equ  $41CD
swapram               equ  $41D0
hideall               equ  $41D3
Small_Window 	      equ  $41D6
ClrDialogFull 	      equ  $41D9
LargeWindow 	      equ  $41DC
ClrWinFull 	      equ  $41DF
PlaySound 	      equ  $41E2
VDispHL 	      equ  $41E5
Pause 	              equ  $41E8
hDetect 	      equ  $41EB
OpenGUIStack 	      equ  $41EE
CloseGUIStack	      equ  $41F1
PushGUIStack 	      equ  $41F4
PopGUIStack 	      equ  $41F7
RenderGUI 	      equ  $41FA
PopGUIStacks	      equ  $41FD
GUIMouse 	      equ  $4200
GUIFindFirst 	      equ  $4203
GUIFindNext 	      equ  $4206
ResetAppPage 	      equ  $4000
Cn2_Setup	      equ  $4209
Cn2_Clear_SendBuf     equ  $420C
Cn2_Clear_RecBuf      equ  $420F
Cn2_Setdown	      equ  $4212
FileOpen 	      equ  $4215
FileSave 	      equ  $4218
FileSaveAs 	      equ  $421B
DispLongInt	      equ  $421E
Cn2_GetK		  equ  $4221
DPutMap			  equ  $4224
APGui_gui7ToTop   equ  $4227
PushGUIStacks	  equ  $422A
GUIFindThis		  equ  $422D

;null				equ $00
;LargeWin				equ $01
;SmallWin				equ $02
;FullScreenImg			equ $03
;Text					equ $04
;WinButtons				equ $05
;WrappedText				equ $06
;ButtonText				equ $07
;ButtonImg				equ $08
;TextLineIn				equ $09
;Radio				equ $0A
;Checkbox				equ $0B
;ByteInt				equ $0C
;WordInt				equ $0D
;Hotspot				equ $0E
;TextMultiline			equ $0F
;Sprite				equ $10
;LargeSprite				equ $11
;PassIn				equ $12
;ScrollVert				equ $13
;ScrollHoriz				equ $14
;Border				equ $15
;Rect				equ $16
;Custom				equ $17
;MouseCursor				equ $18
GUIRnull					equ $00			;COMPLETE	XY	[none]
GUIRLargeWin				equ $01			;COMPLETE	--	[5 byte icon, header text]
GUIRSmallWin				equ $02			;COMPLETE	XY	[x,y, 5 byte icon, header text]
GUIRFullScreenImg			equ $03			;COMPLETE	XY	[768 bytes]
GUIRText					equ $04			;COMPLETE	XY	[x,y,font,z.t. text]
GUIRWinButtons				equ $05			;COMPLETE	--	[which {-,[],X,00000}, onclick[-] {word}, onclick[] {word}, onclick[X] {word}]
GUIRWrappedText				equ $06			;COMPLETE	XY	[x,y,width,font,z.t. text]
GUIRButtonText				equ $07			;COMPLETE	XY	[x,y,onclick {word},zt text]
GUIRButtonImg				equ $08			;COMPLETE	XY	[x,y,onclick {word},img width [BYTES],button width [PX], img data (5 rows high)]
GUIRTextLineIn				equ $09			;COMPLETE	XY	[x,y,width,maxchar [word],curdatapos (0 to datalength) [word],data (z.t.)]
GUIRRadio					equ $0A			;COMPLETE	XY	[x,y,group,state {bool},zttext]  1 per group
GUIRCheckbox				equ $0B			;COMPLETE	XY	[x,y,group,state {bool},zttext] ++ per group
GUIRByteInt					equ $0C			;COMPLETE	XY	[x,y,value,min,max]
GUIRWordInt					equ $0D			;COMPLETE	XY	[x,y,value,min,max]
GUIRHotspot					equ $0E			;COMPLETE	XY	[x,y,width,height,ptr]
GUIRTextMultiline			equ $0F			;COMPLETE	XY	[x,y,rows,width,curdatapos (0 to datalength) [word],data]
GUIRSprite					equ $10			;COMPLETE	XY	[x,y,b,data]
GUIRLargeSprite				equ $11			;COMPLETE	XY	[x,y,a,b,data]
GUIRPassIn					equ $12			;COMPLETE	XY	[x,y,width,maxchar [word],curdatapos (0 to datalength) [word],data (z.t.)]
GUIRScrollVert				equ $13			;COMPLETE	XY	[x,y,height,ID,per,min [word],max [word],cur [word],onScrollUp,onScrollDown]
GUIRScrollHoriz				equ $14			;COMPLETE	XY	[x,y,width,ID,per,min [word],max [word],cur [word],onScrollUp,onScrollDown]
GUIRBorder					equ $15			;COMPLETE	XY	[x,y,width,height,color]
GUIRRect					equ $16			;COMPLETE	XY	[x,y,width,height,fill]
GUIRCustom					equ $17			;COMPLETE	--	[ptr_to_routine]
GUIRMouseCursor				equ $18			;COMPLETE	--  [xy,xy,8-byte mask,8-byte sprite]
GUIRMouseClick				equ $19		

;other compatibility equates
gbuf 			.equ	plotsscreen
progstart		.equ 	$9d95-2
_deletemem		.equ	_delmem

;End of file

;end