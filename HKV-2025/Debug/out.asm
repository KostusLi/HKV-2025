.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "Arsenal.lib"
ExitProcess PROTO:DWORD 
.stack 4096


 confessionsquire PROTO : DWORD

 confessionscroll PROTO : DWORD

 consolidate PROTO : DWORD, : DWORD, : DWORD

 comparescrolls PROTO : DWORD, : DWORD, : DWORD

 fortune PROTO : DWORD, : DWORD, : DWORD

 filament PROTO : DWORD, : DWORD

 transmute  PROTO : DWORD,  : DWORD

 oblivion PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 byte '--- ', 0
		LTRL2 byte ' ---', 0
		LTRL3 sdword 100
		LTRL4 byte 'KINGDOM STATUS REPORT', 0
		LTRL5 sdword 15
		LTRL6 byte 'Base Gold: ', 0
		LTRL7 byte 'ARMY TRAINING', 0
		LTRL8 sdword 0
		LTRL9 sdword 5
		LTRL10 byte 'Training knight number: ', 0
		LTRL11 sdword 1
		LTRL12 sdword 10
		LTRL13 byte 'Knight are so Rich!!)))', 0
		LTRL14 sdword 120
		LTRL15 byte 'Knight are so boring(((', 0
		LTRL16 byte 'Army is ready!', 0
		LTRL17 byte 'WEAPONRY SELECTION', 0
		LTRL18 sdword 3
		LTRL19 byte 'Dragonborn', 0
		LTRL20 byte 'Blacksmith forged weapon ID: ', 0
		LTRL21 byte 'We have a Sword!', 0
		LTRL22 byte 'Scout of the Earth', 0
		LTRL23 sdword 2
		LTRL24 byte 'We have a Bow!', 0
		LTRL25 sdword 8
		LTRL26 byte 'Peasants will fight with forks...', 0
		LTRL27 byte 'We have an Axe!', 0
		LTRL28 byte 'MAGIC AND RUNES', 0
		LTRL29 sdword 7
		LTRL30 sdword 14
		LTRL31 sdword 20
		LTRL32 sdword 6
		LTRL33 sdword 4
		LTRL34 sdword -6
		LTRL35 byte 'Mana (octal o12 -> dec): ', 0
		LTRL36 byte 'Bitwise AND (5 & 3): ', 0
		LTRL37 byte 'Bitwise OR (5 | 3): ', 0
		LTRL38 byte 'Bitwise NOT (~5): ', 0
		LTRL39 byte 'A', 0
		LTRL40 byte 'u', 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		calctaxresult dword 0
		templegold dword 0
		templetaxrate dword 0
		templetax dword 0
		templeknights dword 0
		templemaxkni dword 0
		templeweap dword 0
		templepiligrim dword ?
		templemana dword 0
		templecost dword 0
		templeitemi dword 0
		templeitemik dword 0
		templeitemif dword 0
		templeresik dword 0
		templeresultichek dword 0
		templepower dword 0
		templeartifact dword 0
		templecombined dword 0
		templecursed dword 0
		templerunic dword ?
		templeranik dword ?
.code

;----------- proclaim ------------
proclaim PROC,
	proclaimtitle : dword  
; --- save registers ---
push ebx
push edx
; ----------------------

push offset LTRL1
call confessionscroll


push proclaimtitle
call confessionscroll


push offset LTRL2
call confessionscroll

push offset newline
call confessionscroll

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
proclaim ENDP
;------------------------------


;----------- calctax ------------
calctax PROC,
	calctaxincome : sdword, calctaxrate : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
push calctaxincome
push calctaxrate
pop ebx
pop eax
imul eax, ebx
push eax
push LTRL3
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov calctaxresult, ebx

; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, calctaxresult
ret
calctax ENDP
;------------------------------


;----------- MAIN ------------
main PROC

push offset LTRL4
call proclaim

push LTRL3

pop ebx
mov templegold, ebx

push LTRL5

pop ebx
mov templetaxrate, ebx


push templetaxrate
push templegold
call calctax
push eax

pop ebx
mov templetax, ebx


push offset LTRL6
call confessionscroll


push templegold
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL7
call proclaim

push LTRL8

pop ebx
mov templeknights, ebx

push LTRL9

pop ebx
mov templemaxkni, ebx

mov edx, templeknights
cmp edx, templemaxkni

jle cycle1
jmp cyclenext1
cycle1:

push offset LTRL10
call confessionscroll


push templeknights
call confessionsquire

push offset newline
call confessionscroll

push templeknights
push LTRL11
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov templeknights, ebx

push templegold
push LTRL12
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov templegold, ebx

mov edx, templeknights
cmp edx, templemaxkni

jle cycle1
cyclenext1:

push templegold
call confessionsquire

push offset newline
call confessionscroll

mov edx, templegold
cmp edx, LTRL3

jge right2
jl wrong2
right2:

push offset LTRL13
call confessionscroll

push LTRL14

pop ebx
mov templegold, ebx

push offset newline
call confessionscroll

jmp next2
wrong2:

push offset LTRL15
call confessionscroll

next2:

push offset LTRL16
call confessionscroll

push offset newline
call confessionscroll


push templegold
call confessionsquire

push offset newline
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL17
call proclaim


push LTRL18
push LTRL11
push offset buffer
call fortune
push eax

pop ebx
mov templeweap, ebx

mov templepiligrim, offset LTRL19

push offset LTRL20
call confessionscroll


push templeweap
call confessionsquire

push offset newline
call confessionscroll

mov eax, templeweap
cmp eax, LTRL11
je case1_1
cmp eax, LTRL23
je case1_2
cmp eax, LTRL18
je case1_3
jmp default1

case1_1:

push offset LTRL21
call confessionscroll

push offset newline
call confessionscroll

mov templepiligrim, offset LTRL22
jmp switch_end1

case1_2:

push offset LTRL24
call confessionscroll

push offset newline
call confessionscroll

push LTRL25

pop ebx
mov templeweap, ebx

jmp switch_end1

default1:

push offset LTRL26
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

case1_3:

push offset LTRL27
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

switch_end1:
push offset newline
call confessionscroll


push templepiligrim
call confessionscroll

push offset newline
call confessionscroll


push templeweap
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL28
call proclaim

push LTRL12

pop ebx
mov templemana, ebx

push LTRL25

pop ebx
mov templecost, ebx

push LTRL29

pop ebx
mov templeitemi, ebx

push LTRL30

pop ebx
mov templeitemik, ebx

push LTRL31

pop ebx
mov templeitemif, ebx

push LTRL9
push LTRL32
pop ebx
pop eax
add eax, ebx
push eax
push LTRL33
push LTRL23
pop ebx
pop eax
sub eax, ebx
jnc bk
neg eax
bk: 
push eax
pop ebx
pop eax
imul eax, ebx
push eax
push templeitemik
push templeitemi
pop ebx
pop eax
cdq
idiv ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push templeitemif
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov templeresik, ebx


push templeresik
call confessionsquire

push offset newline
call confessionscroll

push LTRL12
push LTRL25
pop ebx
pop eax
and eax, ebx
push eax
push LTRL32
push LTRL23
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push LTRL34
pop eax
not eax
push eax
pop ebx
pop eax
sub eax, ebx
jnc bkm
neg eax
bkm: 
push eax

pop ebx
mov templeresultichek, ebx


push templeresultichek
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL35
call confessionscroll


push templemana
call confessionsquire

push offset newline
call confessionscroll

push LTRL9

pop ebx
mov templepower, ebx

push LTRL18

pop ebx
mov templeartifact, ebx

push templepower
push templeartifact
pop ebx
pop eax
and eax, ebx
push eax

pop ebx
mov templecombined, ebx


push offset LTRL36
call confessionscroll


push templecombined
call confessionsquire

push offset newline
call confessionscroll

push templepower
push templeartifact
pop ebx
pop eax
or eax, ebx
push eax

pop ebx
mov templecombined, ebx


push offset LTRL37
call confessionscroll


push templecombined
call confessionsquire

push offset newline
call confessionscroll

push templepower
pop eax
not eax
push eax

pop ebx
mov templecursed, ebx


push offset LTRL38
call confessionscroll


push templecursed
call confessionsquire

push offset newline
call confessionscroll

mov templerunic, offset LTRL39
mov templeranik, offset LTRL40

push templerunic
call confessionscroll


push templeranik
call confessionscroll

push offset newline
call confessionscroll

push 0
call ExitProcess
main ENDP
end main
