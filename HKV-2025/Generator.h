#pragma once

#include "IT.h"
#include "LexAnalysis.h"
#include "LT.h"

#define SEPSTREMP "\n;------------------------------\n"
#define SEPSTR(x) "\n;----------- " + string(x) + " ------------\n"

#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"option casemap:none\n"\
"includelib msvcrt.lib\n"\
"includelib legacy_stdio_definitions.lib\n"\
"includelib vcruntimed.lib\n"\
"includelib ucrtd.lib\n"\
"includelib kernel32.lib\n"\
"includelib \"../HKV-2025/Debug/StaticLibrary.lib\"\n"\
"ExitProcess PROTO :DWORD\n"\
".stack 4096\n"

#define END "push 0\ncall ExitProcess\nmain ENDP\nend main"

#define EXTERN "\n outrad PROTO STDCALL :DWORD\n"\
"\n consolidate PROTO STDCALL :DWORD, :DWORD, :DWORD\n"\
"\n mightiness PROTO STDCALL :DWORD, :DWORD, :DWORD\n"\
"\n compare_scrolls PROTO STDCALL :DWORD, :DWORD, :DWORD\n"\
"\n filament PROTO STDCALL :DWORD, :DWORD\n"\
"\n confession PROTO STDCALL :DWORD, :DWORD\n"

#define ITENTRY(x) tables.idtable.table[tables.lextable.table[x].idxTI]
#define LEXEMA(x) tables.lextable.table[x].lexema

#define CONST ".const\n\t\tnewline byte 13, 10, 0"
#define DATA ".data\n\t\ttemp dword ?\n\t\tbuffer byte 256 dup(0)"
#define CODE ".code"

namespace Gener {
    void CodeGeneration(Lexer::LEX& tables, Parm::PARM& parm, Log::LOG& log);
};

