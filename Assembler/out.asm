.586
.model flat, stdcall
option casemap:none
includelib msvcrt.lib
includelib legacy_stdio_definitions.lib
includelib vcruntimed.lib
includelib ucrtd.lib
includelib kernel32.lib
includelib "../HKV-2025/Debug/StaticLibrary.lib"
ExitProcess PROTO :DWORD
.stack 4096


 outrad PROTO STDCALL :DWORD

 consolidate PROTO STDCALL :DWORD, :DWORD, :DWORD

 mightiness PROTO STDCALL :DWORD, :DWORD, :DWORD

 compare_scrolls PROTO STDCALL :DWORD, :DWORD, :DWORD

 filament PROTO STDCALL :DWORD, :DWORD

 confession PROTO STDCALL :DWORD, :DWORD

.const
		newline byte 13, 10, 0
		LTRL1 byte 'Greetings, ', 0
		LTRL2 sdword 10
		LTRL3 sdword 25
		LTRL4 sdword 5
		LTRL5 byte 'Sir Arthur', 0
		LTRL6 byte ' the Brave', 0
		LTRL7 byte 'a', 0
		LTRL8 sdword 0
		LTRL9 sdword 1
		LTRL10 byte 'Titles differ.', 0
		LTRL11 byte 'Titles are equal.', 0
		LTRL12 sdword 12
		LTRL13 sdword 44
		LTRL14 sdword 33
		LTRL15 sdword 42
		LTRL16 sdword 1
		LTRL17 byte 'Path one taken.', 0
		LTRL18 sdword 2
		LTRL19 byte 'Path two taken.', 0
		LTRL20 byte 'No matching path -- tiresome.', 0
		LTRL21 byte 'End of demonstration.', 0
.data
		temp dword ?
		buffer byte 256 dup(0)
		a dword 0
		b dword 0
		c1 dword 0
		maxthree_temp dword 0
		s1 dword ?
		s2 dword ?
		unitescrolls_result dword ?
		unitescrolls_len1 dword 0
		name1 dword ?
		greet_hello dword ?
		temple_x dword 0
		temple_y dword 0
		temple_z dword 0
		temple_maxval dword 0
		temple_name1 dword ?
		temple_title1 dword ?
		temple_fulltitle dword ?
		temple_sym dword ?
		temple_equalcheck dword 0
		temple_bitor dword 0
		temple_bitand dword 0
		temple_bitnot dword 0
		temple_t dword 0
.code

;----------- maxthree ------------
maxthree PROC
push ebp
mov ebp, esp
push ebx
push esi
push edi
mov eax, [ebp+8]
mov a, eax
mov eax, [ebp+12]
mov b, eax
mov eax, [ebp+16]
mov c1, eax

push a
push b
push eax
pop eax
mov maxthree_temp, eax

push maxthree_temp
push c1
push eax
pop eax
mov maxthree_temp, eax

; --- restore registers ---
pop edi
pop esi
pop ebx
mov esp, ebp
pop ebp
; -------------------------
mov eax, maxthree_temp
ret 12
maxthree ENDP
;------------------------------


;----------- unitescrolls ------------
unitescrolls PROC
push ebp
mov ebp, esp
push ebx
push esi
push edi
mov eax, [ebp+8]
mov s1, eax
mov eax, [ebp+12]
mov s2, eax

push s1
push eax
pop eax
mov unitescrolls_len1, eax


push 1
push unitescrolls_len1
call confession

push offset newline
call outrad

mov eax, s1
mov unitescrolls_result, eax

; --- restore registers ---
pop edi
pop esi
pop ebx
mov esp, ebp
pop ebp
; -------------------------
mov eax, unitescrolls_result
ret 8
unitescrolls ENDP
;------------------------------


;----------- greet ------------
greet PROC
push ebp
mov ebp, esp
push ebx
push esi
push edi
mov eax, [ebp+8]
mov name1, eax

mov greet_hello, offset LTRL1

mov eax, greet_hello
mov greet_hello, eax


push 2
push greet_hello
call confession

push offset newline
call outrad

; --- restore registers ---
pop edi
pop esi
pop ebx
mov esp, ebp
pop ebp
; -------------------------
mov eax, 0
ret 4
greet ENDP
;------------------------------


;----------- MAIN ------------
main PROC
push LTRL2
pop eax
mov temple_x, eax

push LTRL3
pop eax
mov temple_y, eax

push LTRL4
pop eax
mov temple_z, eax

mov temple_name1, offset LTRL5

mov temple_title1, offset LTRL6

mov temple_sym, offset LTRL7


push 4
push temple_sym
call confession

push offset newline
call outrad

push LTRL8
pop eax
mov temple_equalcheck, eax


pop eax
push eax
call greet

mov eax, temple_name1
mov temple_fulltitle, eax


push 2
push temple_fulltitle
call confession

push offset newline
call outrad

push temple_name1
push temple_title1
push eax
pop eax
mov temple_equalcheck, eax


push 2
push offset LTRL10
call confession

push offset newline
call outrad


push 2
push offset LTRL11
call confession

push offset newline
call outrad

push temple_x
push LTRL4
pop ebx
pop eax
add eax, ebx
push eax
pop eax
mov temple_x, eax


push 1
push temple_x
call confession

push offset newline
call outrad

push LTRL12
push LTRL13
push LTRL14

pop eax
pop ebx
pop ecx
push ecx
push ebx
push eax
call maxthree
push eax
pop eax
mov temple_maxval, eax


push 1
push temple_maxval
call confession

push offset newline
call outrad

push temple_x
push temple_z
pop ebx
pop eax
or eax, ebx
push eax
pop eax
mov temple_bitor, eax


push 1
push temple_bitor
call confession

push offset newline
call outrad

push temple_x
push temple_z
pop ebx
pop eax
and eax, ebx
push eax
pop eax
mov temple_bitand, eax


push 1
push temple_bitand
call confession

push offset newline
call outrad

push temple_x
pop eax
not eax
push eax
pop eax
mov temple_bitnot, eax


push 1
push temple_bitnot
call confession

push offset newline
call outrad

push temple_x
push temple_y
pop ebx
pop eax
add eax, ebx
push eax
push LTRL15
pop ebx
pop eax
imul eax, ebx
push eax
push temple_x
push temple_y
push eax
pop eax
mov temple_t, eax

mov edx, temple_x

cmp edx, LTRL16
je path3_297

path3_297:


push 2
push offset LTRL17
call confession

push offset newline
call outrad

cmp edx, LTRL18
je path3_306

path3_306:


push 2
push offset LTRL19
call confession

push offset newline
call outrad

jmp council_end3


push 2
push offset LTRL20
call confession

push offset newline
call outrad

jmp council_end3
council_end3:


push 2
push offset LTRL21
call confession

push offset newline
call outrad

push 0
call ExitProcess
main ENDP
end main
