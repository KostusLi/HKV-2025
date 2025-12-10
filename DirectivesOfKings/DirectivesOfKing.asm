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
		LTRL1 byte 'Resultiik: ', 0
		LTRL2 sdword 3
		LTRL3 sdword 6
		LTRL4 sdword -8
		LTRL5 sdword 4
		LTRL6 byte 'Finishing result of tournoment: ', 0
		LTRL7 byte '(', 0
		LTRL8 byte '>=', 0
		LTRL9 byte ') :', 0
		LTRL10 byte 'Faith', 0
		LTRL11 byte 'Exile', 0
		LTRL12 byte 'scroll copied: ', 0
		LTRL13 byte 'lipupu', 0
		LTRL14 byte 'B', 0
		LTRL15 byte 'r', 0
		LTRL16 byte 'o', 0
		LTRL17 byte 'F', 0
		LTRL18 byte 'Compare scrolls: ', 0
		LTRL19 sdword 1
		LTRL20 byte 'Scrolls faith', 0
		LTRL21 byte 'Scrolls exile', 0
		LTRL22 byte 'Fortune of squires: ', 0
		LTRL23 sdword 10
		LTRL24 sdword 5
		LTRL25 sdword 7
		LTRL26 byte 'Wisdomers are learned expressions:  ', 0
		LTRL27 sdword 2
		LTRL28 sdword 8
		LTRL29 byte '34', 0
		LTRL30 byte 'Transmutation from scroll to squire: ', 0
		LTRL31 sdword 23
		LTRL32 byte 'Domain expension', 0
		LTRL33 byte 'shut up', 0
		LTRL34 byte 'g', 0
		LTRL35 byte 'Charge, my battle brothers!', 0
		LTRL36 byte 'The filament of me scroll: ', 0
		LTRL37 sdword 127
		LTRL38 byte 'Remainder after division 54 by 3: ', 0
		LTRL39 byte 'KostusLi - ', 0
		LTRL40 byte 'David goggins', 0
		LTRL41 sdword 37
		LTRL42 byte 'Commutation of octal(o45) and decimal(123): ', 0
		LTRL43 sdword 83
		LTRL44 sdword 45
		LTRL45 byte 'Error', 0
		LTRL46 byte 'Patrol from 2 to 10: ', 0
		LTRL47 byte ' ', 0
		LTRL48 sdword 20
		LTRL49 byte 'hi', 0
		LTRL50 byte 'a', 0
		LTRL51 byte 'Choose 3', 0
		LTRL52 byte 'g', 0
		LTRL53 byte 'Choose 1', 0
		LTRL54 byte 'have not path', 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		bastardres dword 0
		castlestr dword ?
		templestr dword ?
		templeabc dword 0
		templeabcd dword 0
		templef dword 0
		temples dword 0
		templefinish dword 0
		castlestroka dword ?
		castlestrochka dword ?
		castlemh dword ?
		castlemi dword ?
		castlemiv dword ?
		castledsa dword ?
		castleytr dword ?
		castleasd dword 0
		castlex dword 0
		castleu dword 0
		castlev dword 0
		castler dword 0
		castlec dword ?
		castlee dword 0
		castlek dword 0
		castlelen dword ?
		castlenumb dword 0
		castleremainder dword 0
		castlename dword ?
		castlesurname dword ?
		castleim dword 0
		castlefrigh dword 0
		castlebip dword 0
		castlecup dword 0
		castlepoi dword 0
		castleisi dword 0
		castlekni dword 0
.code

;----------- bastard ------------
bastard PROC,
	bastardx : sdword, bastardy : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
mov edx, bastardx
cmp edx, bastardy

jg right1
jl wrong1
right1:
push bastardx

pop ebx
mov bastardres, ebx

jmp next1
wrong1:
push bastardy

pop ebx
mov bastardres, ebx

next1:
; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, bastardres
ret
bastard ENDP
;------------------------------


;----------- castle ------------
castle PROC,
	castlea : dword, castleb : dword  
; --- save registers ---
push ebx
push edx
; ----------------------

push castleb
push castlea
push offset buffer
call consolidate
mov castlestr, eax

push offset LTRL1
call confessionscroll


push castlestr
call confessionscroll

push offset newline
call confessionscroll

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
castle ENDP
;------------------------------


;----------- MAIN ------------
main PROC
push LTRL2

pop ebx
mov templeabc, ebx

push LTRL3

pop ebx
mov templeabcd, ebx

push LTRL4

pop ebx
mov templef, ebx

push LTRL5

pop ebx
mov temples, ebx

push templef
push temples
pop ebx
pop eax
cdq
idiv ebx
push eax

pop ebx
mov templefinish, ebx


push offset LTRL6
call confessionscroll


push templefinish
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL7
call confessionscroll


push templeabc
call confessionsquire


push offset LTRL8
call confessionscroll


push templeabcd
call confessionsquire


push offset LTRL9
call confessionscroll

mov edx, templeabc
cmp edx, templeabcd

jz right2
jg right2
jnz wrong2
right2:
mov templestr, offset LTRL10
jmp next2
wrong2:
mov castlestr, offset LTRL11
next2:

push castlestr
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL12
call confessionscroll

mov castlestroka, offset LTRL13

push castlestroka
call confessionscroll

push offset newline
call confessionscroll


push castlestroka
push offset buffer
call oblivion
mov castlestrochka, eax
mov castlemh, offset LTRL14
mov castlemi, offset LTRL15
mov castlemiv, offset LTRL16

push castlemh
call confessionscroll


push castlemi
call confessionscroll


push castlemiv
call confessionscroll

push offset newline
call confessionscroll

mov castledsa, offset LTRL17
mov castleytr, offset LTRL10

push offset LTRL18
call confessionscroll


push castleytr
push castledsa
push offset buffer
call comparescrolls
push eax

pop ebx
mov castleasd, ebx

mov edx, castleasd
cmp edx, LTRL19

jz right3
jnz wrong3
right3:

push offset LTRL20
call confessionscroll

jmp next3
wrong3:

push offset LTRL21
call confessionscroll

next3:
push offset newline
call confessionscroll

mov ecx, castleytr
mov castledsa, ecx

push castledsa
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL22
call confessionscroll


push LTRL23
push LTRL19
push offset buffer
call fortune
push eax

pop ebx
mov castlex, ebx


push castlex
call confessionsquire

push offset newline
call confessionscroll

push LTRL24

pop ebx
mov castleu, ebx

push LTRL25

pop ebx
mov castlev, ebx


push offset LTRL26
call confessionscroll

push castleu
push castlev
push LTRL27
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push LTRL5
push LTRL5
pop ebx
pop eax
add eax, ebx
push eax
push LTRL27
pop ebx
pop eax
cdq
idiv ebx
push eax
pop ebx
pop eax
sub eax, ebx
jnc bk
neg eax
bk: 
push eax

push LTRL28
push LTRL24
call bastard
push eax
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov castler, ebx


push castler
call confessionsquire

push offset newline
call confessionscroll

mov castlec, offset LTRL29

push offset LTRL30
call confessionscroll


push castlec
push offset buffer
call transmute
push eax

pop ebx
mov castlee, ebx


push castlee
call confessionsquire

push offset newline
call confessionscroll

mov eax, LTRL31
cmp eax, LTRL31
je case1_1
cmp eax, LTRL3
je case1_2
cmp eax, castlee
je case1_3
jmp switch_end1

case1_1:

push offset LTRL32
call confessionscroll

push offset newline
call confessionscroll

case1_2:

push offset LTRL33
call confessionscroll

push offset newline
call confessionscroll

case1_3:

push offset LTRL34
call confessionscroll

push offset newline
call confessionscroll

switch_end1:
mov castlelen, offset LTRL35

push castlelen
push offset buffer
call filament
push eax

pop ebx
mov castlek, ebx


push offset LTRL36
call confessionscroll


push castlek
call confessionsquire

push offset newline
call confessionscroll

push LTRL37

pop ebx
mov castlenumb, ebx

push castlenumb
push LTRL2
pop ebx
pop eax
cdq
mov edx,0
idiv ebx
push edx

pop ebx
mov castleremainder, ebx


push offset LTRL38
call confessionscroll


push castleremainder
call confessionsquire

push offset newline
call confessionscroll

mov castlename, offset LTRL39
mov castlesurname, offset LTRL40
push LTRL41

pop ebx
mov castleim, ebx


push castleim
call confessionsquire

push offset newline
call confessionscroll

push castleim
push castlee
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov castlee, ebx


push offset LTRL42
call confessionscroll


push castlee
call confessionsquire

push offset newline
call confessionscroll

push LTRL43
push LTRL44
push LTRL24
pop ebx
pop eax
sub eax, ebx
jnc bkm
neg eax
bkm: 
push eax
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov castlee, ebx


push castlee
call confessionsquire

push offset newline
call confessionscroll

push LTRL24
push LTRL2
pop ebx
pop eax
and eax, ebx
push eax
push LTRL27
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov castlefrigh, ebx


push castlefrigh
call confessionsquire

push offset newline
call confessionscroll

push castlefrigh
pop eax
not eax
push eax

pop ebx
mov castlebip, ebx


push castlebip
call confessionsquire

push offset newline
call confessionscroll

push castlefrigh
push castlebip
pop ebx
pop eax
or eax, ebx
push eax

pop ebx
mov castlecup, ebx


push castlecup
call confessionsquire

push offset newline
call confessionscroll

push LTRL5

pop ebx
mov castlepoi, ebx

push LTRL5

pop ebx
mov castleisi, ebx

mov edx, castlepoi
cmp edx, castleisi

jz right4
jl right4
jnz wrong4
right4:

push castlesurname
push castlename
call castle

jmp next4
wrong4:

push offset LTRL45
call confessionscroll

next4:
push LTRL27

pop ebx
mov castlekni, ebx


push offset LTRL46
call confessionscroll

mov edx, castlekni
cmp edx, LTRL23

jnz cycle5
jmp cyclenext5
cycle5:

push castlekni
call confessionsquire


push offset LTRL47
call confessionscroll

push castlekni
push LTRL27
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov castlekni, ebx

mov edx, castlekni
cmp edx, LTRL23

jnz cycle5
cyclenext5:

push castlekni
call confessionsquire

mov edx, castlekni
cmp edx, LTRL48

jnz cycle6
jmp cyclenext6
cycle6:

push offset LTRL49
call confessionscroll

push castlekni
push LTRL19
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov castlekni, ebx

mov edx, castlekni
cmp edx, LTRL48

jnz cycle6
cyclenext6:
push offset newline
call confessionscroll

mov eax, offset LTRL50
cmp eax, offset LTRL50
je case2_1
cmp eax, offset LTRL52
je case2_2
jmp default2

case2_1:

push offset LTRL51
call confessionscroll

push offset newline
call confessionscroll

case2_2:

push offset LTRL53
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end2

default2:

push offset LTRL54
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end2

switch_end2:
push 0
call ExitProcess
main ENDP
end main
