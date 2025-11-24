#pragma once
#include "LT.h"
#include "IT.h"
#include "In.h"
#include "Log.h"
#include "FST.h"
#include <iostream>

#define TYPE_SQUIRE     "squire"
#define TYPE_SCROLL     "scroll"
#define TYPE_RUNE       "rune"
#define TYPE_HONOR      "honor"
#define TYPE_HOLLOW     "hollow"

#define FN_COMPARE_SCROLLS  "compare_scrolls"
#define FN_CONSOLIDATE      "consolidate"
#define FN_MIGHTINESS       "mightiness"
#define FN_FILAMENT         "filament"
#define FN_CONF_SCROLL      "confessionScroll"
#define FN_CONF_SQUIRE      "confessionSquire"
#define FN_CONF_RUNE        "confessionRune"
#define KW_TEMPLE           "temple"
#define LITERAL_EXILE       "exile"
#define LITERAL_FAITH       "faith"

#define ISTYPE(str) ( \
    !strcmp(str, TYPE_SQUIRE) || \
    !strcmp(str, TYPE_SCROLL) || \
    !strcmp(str, TYPE_RUNE)   || \
    !strcmp(str, TYPE_HONOR)  || \
    !strcmp(str, TYPE_HOLLOW) \
)

namespace Lexer
{
    struct LEX
    {
        LT::LexTable lextable;
        IT::IdTable idtable;
        LEX() {}
    };

    struct Graph
    {
        char lexema;
        FST::FST graph;
    };

    IT::Entry* getEntry(
        Lexer::LEX& tables,
        char lex,
        char* id,
        char* idtype,
        bool isParam,
        bool isFunc,
        Log::LOG log,
        int line,
        bool& rc_err
    );

    bool analyze(LEX& tables, In::IN& in, Log::LOG& log, Parm::PARM& parm);
    int getIndexInLT(LT::LexTable& lextable, int itTableIndex);
};