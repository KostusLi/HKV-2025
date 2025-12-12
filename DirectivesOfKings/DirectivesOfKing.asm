.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "../HKV-2025/Debug/Arsenal.lib"
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
		LTRL12 byte 'Army is ready!', 0
		LTRL13 byte 'WEAPONRY SELECTION', 0
		LTRL14 sdword 3
		LTRL15 byte 'Blacksmith forged weapon ID: ', 0
		LTRL16 byte 'We have a Sword!', 0
		LTRL17 sdword 2
		LTRL18 byte 'We have a Bow!', 0
		LTRL19 byte 'We have an Axe!', 0
		LTRL20 byte 'Peasants will fight with forks...', 0
		LTRL21 byte 'MAGIC AND RUNES', 0
		LTRL22 byte 'Dragonborn', 0
		LTRL23 sdword 10
		LTRL24 sdword 8
		LTRL25 sdword 7
		LTRL26 sdword 14
		LTRL27 sdword 20
		LTRL28 sdword 6
		LTRL29 sdword 4
		LTRL30 byte 'Mana (octal o12 -> dec): ', 0
		LTRL31 byte 'Bitwise AND (5 & 3): ', 0
		LTRL32 byte 'Bitwise OR (5 | 3): ', 0
		LTRL33 byte 'Bitwise NOT (~5): ', 0
		LTRL34 byte 'A', 0
		LTRL35 byte 'u', 0
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
		calctaxpiligrim dword ?
		calctaxmana dword 0
		calctaxcost dword 0
		calctaxitemi dword 0
		calctaxitemik dword 0
		calctaxitemif dword 0
		calctaxresik dword 0
		calctaxpower dword 0
		calctaxartifact dword 0
		calctaxcombined dword 0
		calctaxcursed dword 0
		calctaxrunic dword ?
		calctaxranik dword ?
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

mov edx, templeknights
cmp edx, templemaxkni

jle cycle1
cyclenext1:

push offset LTRL12
call confessionscroll

push offset newline
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL13
call proclaim


push LTRL14
push LTRL11
push offset buffer
call fortune
push eax

pop ebx
mov templeweap, ebx


push offset LTRL15
call confessionscroll


push templeweap
call confessionsquire

push offset newline
call confessionscroll

mov eax, templeweap
cmp eax, LTRL11
je case1_1
cmp eax, LTRL17
je case1_2
cmp eax, LTRL14
je case1_3
jmp default1

case1_1:

push offset LTRL16
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

case1_2:

push offset LTRL18
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

case1_3:

push offset LTRL19
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

default1:

push offset LTRL20
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

switch_end1:
push offset newline
call confessionscroll


push offset LTRL21
call proclaim

mov calctaxpiligrim, offset LTRL22
push LTRL23

pop ebx
mov calctaxmana, ebx

push LTRL24

pop ebx
mov calctaxcost, ebx

push LTRL25

pop ebx
mov calctaxitemi, ebx

push LTRL26

pop ebx
mov calctaxitemik, ebx

push LTRL27

pop ebx
mov calctaxitemif, ebx

push LTRL9
push LTRL28
pop ebx
pop eax
add eax, ebx
push eax
push LTRL29
push LTRL17
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
push calctaxitemik
push calctaxitemi
pop ebx
pop eax
cdq
idiv ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push calctaxitemif
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov calctaxresik, ebx


push calctaxresik
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL30
call confessionscroll


push calctaxmana
call confessionsquire

push offset newline
call confessionscroll

push LTRL9

pop ebx
mov calctaxpower, ebx

push LTRL14

pop ebx
mov calctaxartifact, ebx

push calctaxpower
push calctaxartifact
pop ebx
pop eax
and eax, ebx
push eax

pop ebx
mov calctaxcombined, ebx


push offset LTRL31
call confessionscroll


push calctaxcombined
call confessionsquire

push offset newline
call confessionscroll

push calctaxpower
push calctaxartifact
pop ebx
pop eax
or eax, ebx
push eax

pop ebx
mov calctaxcombined, ebx


push offset LTRL32
call confessionscroll


push calctaxcombined
call confessionsquire

push offset newline
call confessionscroll

push calctaxpower
pop eax
not eax
push eax

pop ebx
mov calctaxcursed, ebx


push offset LTRL33
call confessionscroll


push calctaxcursed
call confessionsquire

push offset newline
call confessionscroll

mov calctaxrunic, offset LTRL34
mov calctaxranik, offset LTRL35

push calctaxrunic
call confessionscroll


push calctaxranik
call confessionscroll

push offset newline
call confessionscroll

push 0
call ExitProcess
main ENDP
end main
