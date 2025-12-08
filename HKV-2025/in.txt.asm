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

 power PROTO : DWORD, : DWORD, : DWORD

 comparescrolls PROTO : DWORD, : DWORD, : DWORD

 fortune PROTO : DWORD, : DWORD, : DWORD

 filament PROTO : DWORD, : DWORD

 transmute  PROTO : DWORD,  : DWORD

 oblivion PROTO : DWORD, : DWORD

.const
		newline byte 13, 10, 0
		LTRL1 byte 'Result: ', 0
		LTRL2 byte 'Number at sixteen format: ', 0
		LTRL3 sdword 74
		LTRL4 byte '----------------------', 0
		LTRL5 sdword 2
		LTRL6 sdword 4
		LTRL7 sdword -4
		LTRL8 byte 'result of division: ', 0
		LTRL9 byte '(', 0
		LTRL10 byte '>=', 0
		LTRL11 byte ') :', 0
		LTRL12 byte 'True', 0
		LTRL13 byte 'False', 0
		LTRL14 byte 'Copited string: ', 0
		LTRL15 byte 'lipupu', 0
		LTRL16 byte 'H', 0
		LTRL17 byte 'i', 0
		LTRL18 byte '!', 0
		LTRL19 byte 'Compare: ', 0
		LTRL20 sdword 1
		LTRL21 byte 'The lines are the same', 0
		LTRL22 byte 'The lines are not the same', 0
		LTRL23 byte 'Exponentiation of a number: ', 0
		LTRL24 sdword 6
		LTRL25 byte 'Random number: ', 0
		LTRL26 sdword 10
		LTRL27 sdword 5
		LTRL28 sdword 7
		LTRL29 byte 'Arithmetic expression:  ', 0
		LTRL30 sdword 8
		LTRL31 byte '123', 0
		LTRL32 byte 'Converting a string to a number: ', 0
		LTRL33 sdword 23
		LTRL34 byte 'Domain expension', 0
		LTRL35 byte 'shut up', 0
		LTRL36 byte 'g', 0
		LTRL37 byte 'I love SE', 0
		LTRL38 byte 'Line length: ', 0
		LTRL39 sdword 127
		LTRL40 sdword 3
		LTRL41 byte 'Remainder after division 54 by 3: ', 0
		LTRL42 byte 'Number after shifted to the left : ', 0
		LTRL43 byte 'KostusLi - ', 0
		LTRL44 byte 'David goggins', 0
		LTRL45 sdword 37
		LTRL46 byte 'Sum of octal(o45) and decimal(123): ', 0
		LTRL47 sdword 83
		LTRL48 sdword 45
		LTRL49 byte 'Error', 0
		LTRL50 byte 'Cycle from 2 to 10: ', 0
		LTRL51 byte ' ', 0
		LTRL52 sdword 20
		LTRL53 byte 'penis', 0
		LTRL54 byte 'Choose 3', 0
		LTRL55 byte 'Choose 1', 0
		LTRL56 byte 'have not path', 0
.data
		temp sdword ?
		buffer byte 256 dup(0)
		minres dword 0
		standstr dword ?
		templesix dword 0
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
		standp dword 0
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
call outrad


push standstr
call outrad

push offset newline
call outrad

; --- restore registers ---
pop edx
pop ebx
; -------------------------
ret
stand ENDP
;------------------------------


;----------- MAIN ------------
main PROC

push offset LTRL2
call outrad

push LTRL3

pop ebx
mov templesix, ebx


push templesix
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

push LTRL5

pop ebx
mov templeabc, ebx

push LTRL6

pop ebx
mov templeabcd, ebx

push LTRL7

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


push offset LTRL8
call outrad


push templefinish
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad


push offset LTRL9
call outrad


push templeabc
call outlich


push offset LTRL10
call outrad


push templeabcd
call outlich


push offset LTRL11
call outrad

mov edx, templeabc
cmp edx, templeabcd

jz right2
jg right2
jnz wrong2
right2:
mov templestr, offset LTRL12
jmp next2
wrong2:
mov standstr, offset LTRL13
next2:

push standstr
call outrad

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad


push offset LTRL14
call outrad

mov standstroka, offset LTRL15

push standstroka
call outrad

push offset newline
call outrad


push standstroka
push offset buffer
call oblivion
mov standstrochka, eax

push offset LTRL4
call outrad

push offset newline
call outrad

mov standmh, offset LTRL16
mov standmi, offset LTRL17
mov standmiv, offset LTRL18

push standmh
call outrad


push standmi
call outrad


push standmiv
call outrad

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

mov standdsa, offset LTRL13
mov standytr, offset LTRL13

push offset LTRL19
call outrad


push standytr
push standdsa
push offset buffer
call comparescrolls
push eax

pop ebx
mov standasd, ebx

mov edx, standasd
cmp edx, LTRL20

jz right3
jnz wrong3
right3:

push offset LTRL21
call outrad

jmp next3
wrong3:

push offset LTRL22
call outrad

next3:
push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad


push offset LTRL23
call outrad

push LTRL24

pop ebx
mov standp, ebx


push standp
push LTRL5
push offset buffer
call power
push eax

pop ebx
mov standp, ebx


push standp
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad


push offset LTRL25
call outrad


push LTRL26
push LTRL20
push offset buffer
call fortune
push eax

pop ebx
mov standx, ebx


push standx
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

push LTRL27

pop ebx
mov standu, ebx

push LTRL28

pop ebx
mov standv, ebx


push offset LTRL29
call outrad

push standu
push standv
push LTRL5
pop ebx
pop eax
imul eax, ebx
push eax
pop ebx
pop eax
add eax, ebx
push eax
push LTRL6
push LTRL6
pop ebx
pop eax
add eax, ebx
push eax
push LTRL5
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

push LTRL30
push LTRL27
call min
push eax
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standr, ebx


push standr
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

mov standc, offset LTRL31

push offset LTRL32
call outrad


push standc
push offset buffer
call transmute
push eax

pop ebx
mov stande, ebx


push stande
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

mov eax, LTRL33
cmp eax, LTRL33
je case1_1
cmp eax, LTRL24
je case1_2
cmp eax, stande
je case1_3
jmp switch_end1

case1_1:

push offset LTRL34
call outrad

push offset newline
call outrad

case1_2:

push offset LTRL35
call outrad

push offset newline
call outrad

case1_3:

push offset LTRL36
call outrad

push offset newline
call outrad

switch_end1:
mov standlen, offset LTRL37

push standlen
push offset buffer
call filament
push eax

pop ebx
mov standk, ebx


push offset LTRL38
call outrad


push standk
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

push LTRL39

pop ebx
mov standnumb, ebx

push standnumb
push LTRL40
pop ebx
pop eax
cdq
mov edx,0
idiv ebx
push edx

pop ebx
mov standremainder, ebx


push offset LTRL41
call outrad


push standremainder
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

push LTRL40

pop ebx
mov standsdv, ebx

push standsdv
push LTRL5
pop ebx 
pop eax 
mov cl, bl 
shl eax, cl
push eax

pop ebx
mov standresult, ebx


push offset LTRL42
call outrad


push standresult
call outlich

push offset newline
call outrad


push offset LTRL4
call outrad

push offset newline
call outrad

mov standname, offset LTRL43
mov standsurname, offset LTRL44
push LTRL45

pop ebx
mov standim, ebx


push standim
call outlich

push offset newline
call outrad

push standim
push stande
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov stande, ebx


push offset LTRL46
call outrad


push stande
call outlich

push offset newline
call outrad

push LTRL47
push LTRL48
push LTRL27
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
call outlich

push offset newline
call outrad

push LTRL27
push LTRL40
pop ebx
pop eax
and eax, ebx
push eax
push LTRL5
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standfrigh, ebx


push standfrigh
call outlich

push offset newline
call outrad

push standfrigh
pop eax
not eax
push eax

pop ebx
mov standbip, ebx


push standbip
call outlich

push offset newline
call outrad

push standfrigh
push standbip
pop ebx
pop eax
or eax, ebx
push eax

pop ebx
mov standcup, ebx


push standcup
call outlich

push offset newline
call outrad

push LTRL6

pop ebx
mov standpoi, ebx

push LTRL6

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

push offset LTRL49
call outrad

next4:

push offset LTRL4
call outrad

push offset newline
call outrad

push LTRL5

pop ebx
mov standab, ebx


push offset LTRL50
call outrad

mov edx, standab
cmp edx, LTRL26

jnz cycle5
jmp cyclenext5
cycle5:

push standab
call outlich


push offset LTRL51
call outrad

push standab
push LTRL5
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standab, ebx

mov edx, standab
cmp edx, LTRL26

jnz cycle5
cyclenext5:

push standab
call outlich

mov edx, standab
cmp edx, LTRL52

jnz cycle6
jmp cyclenext6
cycle6:

push offset LTRL53
call outrad

push standab
push LTRL20
pop ebx
pop eax
add eax, ebx
push eax

pop ebx
mov standab, ebx

mov edx, standab
cmp edx, LTRL52

jnz cycle6
cyclenext6:
push offset newline
call outrad

mov eax, LTRL40
cmp eax, LTRL40
je case2_1
cmp eax, LTRL20
je case2_2
jmp default2

case2_1:

push offset LTRL54
call outrad

push offset newline
call outrad

case2_2:

push offset LTRL55
call outrad

push offset newline
call outrad

jmp switch_end2

default2:

push offset LTRL56
call outrad

push offset newline
call outrad

jmp switch_end2

switch_end2:
push 0
call ExitProcess
main ENDP
end main
