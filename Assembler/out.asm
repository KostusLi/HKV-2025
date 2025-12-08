.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib "../HKV-2025/Debug/StaticLibrary.lib"
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
		LTRL1 byte 'Result: ', 0
		LTRL2 sdword 2
		LTRL3 sdword 4
		LTRL4 sdword -4
		LTRL5 byte 'result of division: ', 0
		LTRL6 byte '----------------------', 0
		LTRL7 byte '(', 0
		LTRL8 byte '>=', 0
		LTRL9 byte ') :', 0
		LTRL10 byte 'True', 0
		LTRL11 byte 'False', 0
		LTRL12 byte 'Copited string: ', 0
		LTRL13 byte 'lipupu', 0
		LTRL14 byte 'H', 0
		LTRL15 byte 'i', 0
		LTRL16 byte '!', 0
		LTRL17 byte 'Compare: ', 0
		LTRL18 sdword 1
		LTRL19 byte 'The lines are the same', 0
		LTRL20 byte 'The lines are not the same', 0
		LTRL21 byte 'Random number: ', 0
		LTRL22 sdword 10
		LTRL23 sdword 5
		LTRL24 sdword 7
		LTRL25 byte 'Arithmetic expression:  ', 0
		LTRL26 sdword 8
		LTRL27 byte '123', 0
		LTRL28 byte 'Converting a string to a number: ', 0
		LTRL29 sdword 23
		LTRL30 byte 'Domain expension', 0
		LTRL31 sdword 6
		LTRL32 byte 'shut up', 0
		LTRL33 byte 'g', 0
		LTRL34 byte 'I love SE', 0
		LTRL35 byte 'Line length: ', 0
		LTRL36 sdword 127
		LTRL37 sdword 3
		LTRL38 byte 'Remainder after division 54 by 3: ', 0
		LTRL39 byte 'Number after shifted to the left : ', 0
		LTRL40 byte 'KostusLi - ', 0
		LTRL41 byte 'David goggins', 0
		LTRL42 sdword 37
		LTRL43 byte 'Sum of octal(o45) and decimal(123): ', 0
		LTRL44 sdword 83
		LTRL45 sdword 45
		LTRL46 byte 'Error', 0
		LTRL47 byte 'Cycle from 2 to 10: ', 0
		LTRL48 byte ' ', 0
		LTRL49 sdword 20
		LTRL50 byte 'penis', 0
		LTRL51 byte 'Choose 3', 0
		LTRL52 byte 'Choose 1', 0
		LTRL53 byte 'have not path', 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		minres dword 0
		standstr dword ?
		templestr dword ?
		templeabc dword 0
		templeabcd dword 0
		templef dword 0
		temples dword 0
		templefinish dword 0
		standstroka dword ?
		standstrochka dword ?
		standmh dword ?
		standmi dword ?
		standmiv dword ?
		standdsa dword ?
		standytr dword ?
		standasd dword 0
		standx dword 0
		standu dword 0
		standv dword 0
		standr dword 0
		standc dword ?
		stande dword 0
		standk dword 0
		standlen dword ?
		standnumb dword 0
		standremainder dword 0
		standresult dword 0
		standsdv dword 0
		standname dword ?
		standsurname dword ?
		standim dword 0
		standfrigh dword 0
		standbip dword 0
		standcup dword 0
		standpoi dword 0
		standisi dword 0
		standab dword 0
.code

;----------- min ------------
min PROC,
	minx : sdword, miny : sdword  
; --- save registers ---
push ebx
push edx
; ----------------------
mov edx, minx
cmp edx, miny

jg right1
jl wrong1
right1:
push minx

pop ebx
mov minres, ebx

jmp next1
wrong1:
push miny

pop ebx
mov minres, ebx

next1:
; --- restore registers ---
pop edx
pop ebx
; -------------------------
mov eax, minres
ret
min ENDP
;------------------------------


;----------- stand ------------
stand PROC,
	standa : dword, standb : dword  
; --- save registers ---
push ebx
push edx
; ----------------------

push standb
push standa
push offset buffer
call consolidate
mov standstr, eax

push offset LTRL1
call confessionscroll


push standstr
call confessionscroll

push offset newline
call confessionscroll

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
stand ENDP
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

push LTRL2

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


push offset LTRL5
call confessionscroll


push templefinish
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

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
mov standstr, offset LTRL11
next2:

push standstr
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL12
call confessionscroll

mov standstroka, offset LTRL13

push standstroka
call confessionscroll

push offset newline
call confessionscroll


push standstroka
push offset buffer
call oblivion
mov standstrochka, eax

push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

mov standmh, offset LTRL14
mov standmi, offset LTRL15
mov standmiv, offset LTRL16

push standmh
call confessionscroll


push standmi
call confessionscroll


push standmiv
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

mov standdsa, offset LTRL11
mov standytr, offset LTRL11

push offset LTRL17
call confessionscroll


push standytr
push standdsa
push offset buffer
call comparescrolls
push eax

pop ebx
mov standasd, ebx

mov edx, standasd
cmp edx, LTRL18

jz right3
jnz wrong3
right3:

push offset LTRL19
call confessionscroll

jmp next3
wrong3:

push offset LTRL20
call confessionscroll

next3:
push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll


push offset LTRL21
call confessionscroll


push LTRL22
push LTRL18
push offset buffer
call fortune
push eax

pop ebx
mov standx, ebx


push standx
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

push LTRL23

pop ebx
mov standu, ebx

push LTRL24

pop ebx
mov standv, ebx


push offset LTRL25
call confessionscroll

push standu
push standv
push LTRL2
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push LTRL3
push LTRL3
pop ebx
pop eax
add eax, ebx
push eax
push LTRL2
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

push LTRL26
push LTRL23
call min
push eax
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standr, ebx


push standr
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

mov standc, offset LTRL27

push offset LTRL28
call confessionscroll


push standc
push offset buffer
call transmute
push eax

pop ebx
mov stande, ebx


push stande
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

mov eax, LTRL29
cmp eax, LTRL29
je case1_1
cmp eax, LTRL31
je case1_2
cmp eax, stande
je case1_3
jmp switch_end1

case1_1:

push offset LTRL30
call confessionscroll

push offset newline
call confessionscroll

case1_2:

push offset LTRL32
call confessionscroll

push offset newline
call confessionscroll

case1_3:

push offset LTRL33
call confessionscroll

push offset newline
call confessionscroll

switch_end1:
mov standlen, offset LTRL34

push standlen
push offset buffer
call filament
push eax

pop ebx
mov standk, ebx


push offset LTRL35
call confessionscroll


push standk
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

push LTRL36

pop ebx
mov standnumb, ebx

push standnumb
push LTRL37
pop ebx
pop eax
cdq
mov edx,0
idiv ebx
push edx

pop ebx
mov standremainder, ebx


push offset LTRL38
call confessionscroll


push standremainder
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

push LTRL37

pop ebx
mov standsdv, ebx


push offset LTRL39
call confessionscroll


push standresult
call confessionsquire

push offset newline
call confessionscroll


push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

mov standname, offset LTRL40
mov standsurname, offset LTRL41
push LTRL42

pop ebx
mov standim, ebx


push standim
call confessionsquire

push offset newline
call confessionscroll

push standim
push stande
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov stande, ebx


push offset LTRL43
call confessionscroll


push stande
call confessionsquire

push offset newline
call confessionscroll

push LTRL44
push LTRL45
push LTRL23
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
mov stande, ebx


push stande
call confessionsquire

push offset newline
call confessionscroll

push LTRL23
push LTRL37
pop ebx
pop eax
and eax, ebx
push eax
push LTRL2
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standfrigh, ebx


push standfrigh
call confessionsquire

push offset newline
call confessionscroll

push standfrigh
pop eax
not eax
push eax

pop ebx
mov standbip, ebx


push standbip
call confessionsquire

push offset newline
call confessionscroll

push standfrigh
push standbip
pop ebx
pop eax
or eax, ebx
push eax

pop ebx
mov standcup, ebx


push standcup
call confessionsquire

push offset newline
call confessionscroll

push LTRL3

pop ebx
mov standpoi, ebx

push LTRL3

pop ebx
mov standisi, ebx

mov edx, standpoi
cmp edx, standisi

jz right4
jl right4
jnz wrong4
right4:

push standsurname
push standname
call stand

jmp next4
wrong4:

push offset LTRL46
call confessionscroll

next4:

push offset LTRL6
call confessionscroll

push offset newline
call confessionscroll

push LTRL2

pop ebx
mov standab, ebx


push offset LTRL47
call confessionscroll

mov edx, standab
cmp edx, LTRL22

jnz cycle5
jmp cyclenext5
cycle5:

push standab
call confessionsquire


push offset LTRL48
call confessionscroll

push standab
push LTRL2
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standab, ebx

mov edx, standab
cmp edx, LTRL22

jnz cycle5
cyclenext5:

push standab
call confessionsquire

mov edx, standab
cmp edx, LTRL49

jnz cycle6
jmp cyclenext6
cycle6:

push offset LTRL50
call confessionscroll

push standab
push LTRL18
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standab, ebx

mov edx, standab
cmp edx, LTRL49

jnz cycle6
cyclenext6:
push offset newline
call confessionscroll

mov eax, LTRL37
cmp eax, LTRL37
je case2_1
cmp eax, LTRL18
je case2_2
jmp default2

case2_1:

push offset LTRL51
call confessionscroll

push offset newline
call confessionscroll

case2_2:

push offset LTRL52
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end2

default2:

push offset LTRL53
call confessionscroll

push offset newline
call confessionscroll

jmp switch_end2

switch_end2:
push 0
call ExitProcess
main ENDP
end main
