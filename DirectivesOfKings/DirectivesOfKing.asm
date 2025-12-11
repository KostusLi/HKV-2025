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
		LTRL7 byte 'Tax collected: ', 0
		LTRL8 sdword 50
		LTRL9 byte 'Treasury after expenses: ', 0
		LTRL10 byte 'ARMY TRAINING', 0
		LTRL11 sdword 0
		LTRL12 sdword 5
		LTRL13 byte 'Training knight number: ', 0
		LTRL14 sdword 1
		LTRL15 byte 'Army is ready!', 0
		LTRL16 byte 'WEAPONRY SELECTION', 0
		LTRL17 sdword 4
		LTRL18 byte 'Blacksmith forged weapon ID: ', 0
		LTRL19 byte 'We have a Sword!', 0
		LTRL20 sdword 2
		LTRL21 byte 'We have a Bow!', 0
		LTRL22 sdword 3
		LTRL23 byte 'We have an Axe!', 0
		LTRL24 byte 'Peasants will fight with forks...', 0
		LTRL25 byte 'MAGIC AND RUNES', 0
		LTRL26 sdword 10
		LTRL27 sdword 8
		LTRL28 sdword 7
		LTRL29 sdword 14
		LTRL30 sdword 20
		LTRL31 sdword 6
		LTRL32 byte 'Mana (octal o12 -> dec): ', 0
		LTRL33 byte 'Bitwise AND (5 & 3): ', 0
		LTRL34 byte 'Bitwise OR (5 | 3): ', 0
		LTRL35 byte 'Bitwise NOT (~5): ', 0
		LTRL36 byte 'SCROLL ARCHIVE', 0
		LTRL37 byte 'Royal ', 0
		LTRL38 byte 'Decree', 0
		LTRL39 byte 'Length of decree: ', 0
		LTRL40 byte '123', 0
		LTRL41 byte 'String 123 to Squire: ', 0
		LTRL42 byte 'Ancient Secret', 0
		LTRL43 byte 'Copy of scroll: ', 0
		LTRL44 byte 'Copy is perfect', 0
		LTRL45 byte 'Copy failed', 0
		LTRL46 byte 'FINAL BATTLE CALCULATION', 0
		LTRL47 byte 'Hero wins with critical hit!', 0
		LTRL48 byte 'Hero needs to run!', 0
		LTRL49 byte 'COMPLEX PRIORITY TEST', 0
		LTRL50 sdword 12
		LTRL51 byte 'Calc: 5 + 3 * 2 bitand 12 bitor 1', 0
		LTRL52 byte 'Expected: 9', 0
		LTRL53 byte 'Result: ', 0
		LTRL54 byte 'Calc: bitnot 0 + 5 (Expect 4): ', 0
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
		calctaxpartone dword ?
		calctaxparttwo dword ?
		calctaxfulldoc dword ?
		calctaxlen dword 0
		calctaxnumstr dword ?
		calctaxnumval dword 0
		calctaxoriginal dword ?
		calctaxcopy dword ?
		calctaxissame dword 0
		calctaxdamage dword 0
		calctaxhp dword 0
		calctaxvala dword 0
		calctaxvalb dword 0
		calctaxcomplexres dword 0
		calctaxnottest dword 0
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
call confessionscroll


push templetax
call confessionsquire

push offset newline
call confessionscroll

push templegold
push templetax
pop ebx
pop eax
add eax, ebx
push eax
push LTRL8
pop ebx
pop eax
sub eax, ebx
jnc bk
neg eax
bk: 
push eax

pop ebx
mov templegold, ebx


push offset LTRL9
call confessionscroll


push templegold
call confessionsquire

push offset newline
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL10
call proclaim

push LTRL11

pop ebx
mov templeknights, ebx

push LTRL12

pop ebx
mov templemaxkni, ebx

mov edx, templeknights
cmp edx, templemaxkni

jle cycle1
jmp cyclenext1
cycle1:

push offset LTRL13
call confessionscroll


push templeknights
call confessionsquire

push offset newline
call confessionscroll

push templeknights
push LTRL14
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

push offset LTRL15
call confessionscroll

push offset newline
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL16
call proclaim


push LTRL17
push LTRL14
push offset buffer
call fortune
push eax

pop ebx
mov templeweap, ebx


push offset LTRL18
call confessionscroll


push templeweap
call confessionsquire

push offset newline
call confessionscroll

mov eax, templeweap
cmp eax, LTRL14
je case1_1
cmp eax, LTRL20
je case1_2
cmp eax, LTRL22
je case1_3
jmp default1

case1_1:

push offset LTRL19
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

case1_2:

push offset LTRL21
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

case1_3:

push offset LTRL23
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

default1:

push offset LTRL24
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end1

switch_end1:
push offset newline
call confessionscroll


push offset LTRL25
call proclaim

push LTRL26

pop ebx
mov calctaxmana, ebx

push LTRL27

pop ebx
mov calctaxcost, ebx

push LTRL28

pop ebx
mov calctaxitemi, ebx

push LTRL29

pop ebx
mov calctaxitemik, ebx

push LTRL30

pop ebx
mov calctaxitemif, ebx

push LTRL12
push LTRL31
pop ebx
pop eax
add eax, ebx
push eax
push LTRL17
push LTRL20
pop ebx
pop eax
sub eax, ebx
jnc bkm
neg eax
bkm: 
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


push offset LTRL32
call confessionscroll


push calctaxmana
call confessionsquire

push offset newline
call confessionscroll

push LTRL12

pop ebx
mov calctaxpower, ebx

push LTRL22

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


push offset LTRL33
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


push offset LTRL34
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


push offset LTRL35
call confessionscroll


push calctaxcursed
call confessionsquire

push offset newline
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL36
call proclaim

mov calctaxpartone, offset LTRL37
mov calctaxparttwo, offset LTRL38

push calctaxparttwo
push calctaxpartone
push offset buffer
call consolidate
mov calctaxfulldoc, eax

push calctaxfulldoc
call confessionscroll

push offset newline
call confessionscroll


push calctaxfulldoc
push offset buffer
call filament
push eax

pop ebx
mov calctaxlen, ebx


push offset LTRL39
call confessionscroll


push calctaxlen
call confessionsquire

push offset newline
call confessionscroll

mov calctaxnumstr, offset LTRL40

push calctaxnumstr
push offset buffer
call transmute
push eax

pop ebx
mov calctaxnumval, ebx


push offset LTRL41
call confessionscroll


push calctaxnumval
call confessionsquire

push offset newline
call confessionscroll

mov calctaxoriginal, offset LTRL42

push calctaxoriginal
push offset buffer
call oblivion
mov calctaxcopy, eax

push offset LTRL43
call confessionscroll


push calctaxcopy
call confessionscroll

push offset newline
call confessionscroll


push calctaxcopy
push calctaxoriginal
push offset buffer
call comparescrolls
push eax

pop ebx
mov calctaxissame, ebx

mov edx, calctaxissame
cmp edx, LTRL14

je right2
jne wrong2
right2:

push offset LTRL44
call confessionscroll

push offset newline
call confessionscroll

jmp next2
wrong2:

push offset LTRL45
call confessionscroll

push offset newline
call confessionscroll

next2:
push offset newline
call confessionscroll


push offset LTRL46
call proclaim

push LTRL8

pop ebx
mov calctaxdamage, ebx

push LTRL3

pop ebx
mov calctaxhp, ebx

push calctaxdamage
push LTRL20
pop ebx
pop eax
imul eax, ebx
push eax

pop ebx
mov calctaxdamage, ebx

mov edx, calctaxdamage
cmp edx, calctaxhp

jg right3
jle wrong3
right3:

push offset LTRL47
call confessionscroll

jmp next3
wrong3:

push offset LTRL48
call confessionscroll

next3:
push offset newline
call confessionscroll


push offset LTRL49
call proclaim

push LTRL12

pop ebx
mov calctaxvala, ebx

push LTRL22

pop ebx
mov calctaxvalb, ebx

push calctaxvala
push calctaxvalb
push LTRL20
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push LTRL50
pop ebx
pop eax
and eax, ebx
push eax
push LTRL14
pop ebx
pop eax
or eax, ebx
push eax

pop ebx
mov calctaxcomplexres, ebx


push offset LTRL51
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL52
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL53
call confessionscroll


push calctaxcomplexres
call confessionsquire

push offset newline
call confessionscroll

push LTRL11
pop eax
not eax
push eax
push LTRL12
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov calctaxnottest, ebx


push offset LTRL54
call confessionscroll


push calctaxnottest
call confessionsquire

push offset newline
call confessionscroll

push 0
call ExitProcess
main ENDP
end main
