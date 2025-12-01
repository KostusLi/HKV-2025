#include "stdafx.h"
#include <stack>
#include <cstring>
#include <iostream>
#include "PolishNotation.h"
#include "LT.h"
#include "IT.h"
#include "LexAnalysis.h"
#include "Error.h"
#include "Log.h"

using namespace std;
namespace Polish
{
    // Определение приоритетов операторов
    // Приоритет 0: скобки (не используются в стеке)
    // Приоритет 1: сложение и вычитание (+, -)
    // Приоритет 2: умножение и деление (*, /)
    // Приоритет 3: операторы сравнения (<, >, !, ?)
    // Приоритет 4: побитовые И и ИЛИ (&, |)
    // Приоритет 5: унарный оператор побитового НЕ (~)
    int getPriority(LT::Entry& e)
    {
        switch (e.lexema)
        {
        case LEX_LEFTBRAC: case LEX_RIGHTBRAC: return 0;  // Скобки для вызовов функций
        case LEX_PLUS: case LEX_MINUS: return 1;          // Арифметические: +, -
        case LEX_STAR: case LEX_DIRSLASH: return 2;       // Арифметические: *, /
        case LEX_MORE: case LEX_LESS: case LEX_EQUALS: case LEX_NONEQUALS: return 3; // Сравнение: <, >, !, ?
        case LEX_ALLY: case LEX_BIND: return 4;           // Побитовые: |, &
        case LEX_BANISH: return 5;                        // Унарный побитовый НЕ: ~
        default: return -1;
        }
    }

    // Проверка, является ли оператор унарным
    bool isUnaryOperator(char lexema)
    {
        return lexema == LEX_BANISH;
    }

    // Проверка, является ли оператор бинарным
    bool isBinaryOperator(char lexema)
    {
        return lexema == LEX_PLUS || lexema == LEX_MINUS || 
               lexema == LEX_STAR || lexema == LEX_DIRSLASH ||
               lexema == LEX_MORE || lexema == LEX_LESS || 
               lexema == LEX_EQUALS || lexema == LEX_NONEQUALS ||
               lexema == LEX_ALLY || lexema == LEX_BIND;
    }

    bool PolishNotation(Lexer::LEX& tbls, Log::LOG& log)
    {
        unsigned curExprBegin = 0;
        ltvec v; // Вектор для хранения выражения
        LT::LexTable new_table = LT::Create(tbls.lextable.maxsize);
        intvec vpositions = getExprPositions(tbls); // Позиции начала выражений

        for (int i = 0; i < tbls.lextable.size; i++)
        {
            if (curExprBegin < vpositions.size() && i == vpositions[curExprBegin]) // Если мы на начале выражения
            {
                int lexcount = fillVector(vpositions[curExprBegin], tbls.lextable, v); // Заполняем вектор выражения из лексем
                if (lexcount > 1)
                {
                    bool rc = setPolishNotation(tbls.idtable, log, vpositions[curExprBegin], v); // Преобразуем выражение в польскую нотацию
                    if (!rc)
                        return false;

                    // Вывод преобразованного выражения (для отладки)
                    for (unsigned j = 0; j < v.size(); j++)
                    {
                        cout << v[j].lexema;
                    }
                    cout << endl;
                }

                addToTable(new_table, tbls.idtable, v); // Добавляем преобразованное выражение в таблицу + обновляем индексы
                i += lexcount - 1;
                curExprBegin++;
                continue;
            }
            if (tbls.lextable.table[i].lexema == LEX_ID || tbls.lextable.table[i].lexema == LEX_LITERAL)
            {
                int firstind = Lexer::getIndexInLT(new_table, tbls.lextable.table[i].idxTI);
                if (firstind == -1)
                    firstind = new_table.size;
                tbls.idtable.table[tbls.lextable.table[i].idxTI].idxfirstLE = firstind;
            }
            LT::Add(new_table, tbls.lextable.table[i]);
        }

        tbls.lextable = new_table;
        return true;
    }

    int __cdecl fillVector(int posExprBegin, LT::LexTable& lextable, ltvec& v)
    {
        v.clear();
        for (int i = posExprBegin; i < lextable.size; i++)
        {
            // Выражение заканчивается на точку с запятой
            if (lextable.table[i].lexema == LEX_SEMICOLON)
                break;
            // Не включаем скобки {} (блоки кода)
            if (lextable.table[i].lexema == LEX_LEFTBRACE || lextable.table[i].lexema == LEX_BRACELET)
                break;
            // Скобки [] используются для условий циклов и параметров функций
            // В выражениях присваивания они не должны встречаться
            // Если встречаем [, это может быть условие цикла - пропускаем
            if (lextable.table[i].lexema == LEX_LEFTHESIS)
            {
                // Пропускаем всё до закрывающей скобки ]
                int j = i + 1;
                while (j < lextable.size && lextable.table[j].lexema != LEX_RIGHTTHESIS)
                    j++;
                if (j < lextable.size)
                    i = j; // Пропускаем всю конструкцию [ ... ]
                else
                    break; // Незакрытая скобка - ошибка
                continue;
            }
            if (lextable.table[i].lexema == LEX_RIGHTTHESIS)
            {
                // Неожиданная закрывающая скобка - пропускаем
                break;
            }
            v.push_back(LT::Entry(lextable.table[i]));
        }
        return v.size();
    }

    void addToTable(LT::LexTable& new_table, IT::IdTable& idtable, ltvec& v)
    {
        for (unsigned i = 0; i < v.size(); i++)
        {
            LT::Add(new_table, v[i]);
            // Обновляем индексы первого вхождения в ТЛ и ТИ
            if (v[i].lexema == LEX_ID || v[i].lexema == LEX_LITERAL)
            {
                int firstind = Lexer::getIndexInLT(new_table, v[i].idxTI);
                idtable.table[v[i].idxTI].idxfirstLE = firstind;
            }
        }
    }

    intvec getExprPositions(Lexer::LEX& tbls)
    {
        intvec v;
        bool f_begin = false; // Флаг начала выражения
        bool f_end = false;  // Флаг конца выражения
        int begin = 0;  int end = 0;

        for (int i = 0; i < tbls.lextable.size; i++)
        {
            if (tbls.lextable.table[i].lexema == LEX_ASSIGN) // Начало выражения (после =)
            {
                begin = i + 1;
                f_begin = true;
                continue;
            }
            if (f_begin && tbls.lextable.table[i].lexema == LEX_SEMICOLON) // Конец выражения (до ;)
            {
                end = i;
                f_end = true;
                continue;
            }
            if (f_begin && f_end) // Сохраняем начало и конец выражения в вектор
            {
                v.push_back(begin);
                f_begin = f_end = false;
            }
        }
        return v;
    }

    bool __cdecl setPolishNotation(IT::IdTable& idtable, Log::LOG& log, int lextable_pos, ltvec& v)
    {
        // Результирующий вектор
        vector<LT::Entry> result;
        // Стек для операторов
        stack<LT::Entry> s;
        // Стек для имен функций (для правильного порядка в постфиксной записи)
        stack<LT::Entry> funcStack;
        // Счетчик вложенности скобок функций
        int funcCallDepth = 0;

        for (unsigned i = 0; i < v.size(); i++)
        {
            // Обработка скобок для вызовов функций () - обрабатываем ПЕРЕД запятыми
            if (v[i].lexema == LEX_LEFTBRAC)
            {
                // Проверяем, является ли это вызовом функции
                // Имя функции уже сохранено в funcStack при обработке идентификатора
                bool isFunctionCall = false;
                if (i > 0)
                {
                    // Проверка на стандартные функции библиотеки
                    if (v[i-1].lexema == LEX_COMPSCROLLS || 
                        v[i-1].lexema == LEX_CONSOLIDATE || 
                        v[i-1].lexema == LEX_MIGHTINESS || 
                        v[i-1].lexema == LEX_FILAMENT)
                    {
                        isFunctionCall = true;
                    }
                    // Проверка на обычную функцию (LEX_ID)
                    else if (v[i-1].lexema == LEX_ID && v[i-1].idxTI != TI_NULLIDX)
                    {
                        IT::IDTYPE idtype = idtable.table[v[i-1].idxTI].idtype;
                        if (idtype == IT::IDTYPE::F || idtype == IT::IDTYPE::S)
                        {
                            isFunctionCall = true;
                        }
                    }
                }
                
                if (isFunctionCall)
                {
                    funcCallDepth++;
                    // Открывающая скобка функции - НЕ записываем в результат (скобки не нужны в польской нотации)
                }
                else
                {
                    // Это арифметическая скобка - добавляем в стек
                    s.push(v[i]);
                }
                continue;
            }

            if (v[i].lexema == LEX_RIGHTBRAC)
            {
                if (funcCallDepth > 0)
                {
                    // Закрывающая скобка вызова функции
                    funcCallDepth--;
                    // Записываем имя функции в результат (в постфиксной нотации функция идет после аргументов)
                    if (!funcStack.empty())
                    {
                        result.push_back(funcStack.top());
                        funcStack.pop();
                    }
                    // Закрывающую скобку НЕ записываем в результат
                    continue;
                }
                else
                {
                    // Это скобка в арифметическом выражении - обрабатываем как обычную скобку
                    // Выталкиваем операторы из стека до открывающей скобки
                    while (!s.empty() && s.top().lexema != LEX_LEFTBRAC)
                    {
                        result.push_back(s.top());
                        s.pop();
                    }
                    if (!s.empty() && s.top().lexema == LEX_LEFTBRAC)
                        s.pop();
                    continue;
                }
            }

            // Обработка запятых - они разделяют аргументы функции, не преобразуются в польскую нотацию
            // Обрабатываем запятую ПОСЛЕ скобок, чтобы funcCallDepth был уже установлен
            if (v[i].lexema == LEX_COMMA)
            {
                // Запятая допустима только внутри вызова функции
                if (funcCallDepth > 0)
                {
                    // Просто пропускаем запятую
                    continue;
                }
                else
                {
                    // Запятая вне вызова функции - ошибка
                    Log::writeError(log.stream, Error::GetError(613, v[i].sn, 0));
                    return false;
                }
            }

            // Обработка стандартных функций библиотеки
            if (v[i].lexema == LEX_COMPSCROLLS || 
                v[i].lexema == LEX_CONSOLIDATE || 
                v[i].lexema == LEX_MIGHTINESS || 
                v[i].lexema == LEX_FILAMENT)
            {
                // Проверяем, является ли это вызовом функции (следующая лексема - открывающая скобка)
                if (i + 1 < v.size() && v[i + 1].lexema == LEX_LEFTBRAC)
                {
                    // Это вызов функции - сохраняем имя функции в стек, добавим в результат после аргументов
                    funcStack.push(v[i]);
                }
                else
                {
                    // Не вызов функции (хотя для стандартных функций это маловероятно)
                    result.push_back(v[i]);
                }
                continue;
            }

            // Обработка идентификаторов и литералов
            if (v[i].lexema == LEX_LITERAL || v[i].lexema == LEX_ID)
            {
                // Проверяем, является ли это вызовом функции
                if (v[i].lexema == LEX_ID && v[i].idxTI != TI_NULLIDX)
                {
                    IT::IDTYPE idtype = idtable.table[v[i].idxTI].idtype;
                    if ((idtype == IT::IDTYPE::F || idtype == IT::IDTYPE::S) &&
                        i + 1 < v.size() && v[i + 1].lexema == LEX_LEFTBRAC)
                    {
                        // Это вызов функции - сохраняем имя функции в стек, добавим в результат после аргументов
                        funcStack.push(v[i]);
                        continue;
                    }
                }
                result.push_back(v[i]); // Добавляем операнд в результирующий вектор
                continue;
            }

            // Обработка операторов
            int priority = getPriority(v[i]);

            // Обработка операторов
            if (isBinaryOperator(v[i].lexema) || isUnaryOperator(v[i].lexema))
            {
                // Для унарных операторов (banish) - особая обработка
                if (isUnaryOperator(v[i].lexema))
                {
                    // Унарный оператор имеет высокий приоритет и обрабатывается сразу
                    s.push(v[i]);
                    continue;
                }

                // Для бинарных операторов: выталкиваем операторы с большим или равным приоритетом
                while (!s.empty() && 
                       s.top().lexema != LEX_LEFTBRAC && 
                       getPriority(s.top()) >= priority)
                {
                    result.push_back(s.top());
                    s.pop();
                }
                s.push(v[i]);
                continue;
            }

            // Проверка на недопустимые символы в выражении
            // Запятая и стандартные функции уже обработаны выше, поэтому их здесь не проверяем
            if (v[i].lexema != LEX_LEFTBRAC && v[i].lexema != LEX_RIGHTBRAC && 
                v[i].lexema != LEX_PLUS && v[i].lexema != LEX_MINUS && 
                v[i].lexema != LEX_STAR && v[i].lexema != LEX_DIRSLASH && 
                v[i].lexema != LEX_ID && v[i].lexema != LEX_LITERAL && 
                v[i].lexema != LEX_MORE && v[i].lexema != LEX_LESS && 
                v[i].lexema != LEX_EQUALS && v[i].lexema != LEX_NONEQUALS &&
                v[i].lexema != LEX_ALLY && v[i].lexema != LEX_BIND && 
                v[i].lexema != LEX_BANISH && v[i].lexema != LEX_COMMA &&
                v[i].lexema != LEX_COMPSCROLLS && v[i].lexema != LEX_CONSOLIDATE &&
                v[i].lexema != LEX_MIGHTINESS && v[i].lexema != LEX_FILAMENT)
            {
                Log::writeError(log.stream, Error::GetError(613, v[i].sn, 0));
                return false;
            }
        }

        // Выталкиваем оставшиеся операторы из стека
        while (!s.empty()) 
        { 
            if (s.top().lexema == LEX_LEFTBRAC)
            {
                // Незакрытая скобка - ошибка
                Log::writeError(log.stream, Error::GetError(605, s.top().sn, 0));
                return false;
            }
            result.push_back(s.top()); 
            s.pop(); 
        }
        v = result;
        return true;
    }
}
