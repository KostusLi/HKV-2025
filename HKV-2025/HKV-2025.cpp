#include <iostream>
#include "stdafx.h"


int wmain(int argc, wchar_t* argv[])
{
	char SCRIBE_KRASH[] = "Поражение Скаутов (Лексический анализ завершен с ошибками)";
	char TEMPLE_KRASH[] = "Падение Храма (Синтаксический анализ завершен с ошибками)";
	char SOVET_KRASH[] = "Крах Совета (Семантические ошибки)";
	char BRAND_KRASH[] = "Крах Клеймения (Ошибка при преобразовании выражения)";

	char SCRIBE_TRIUMPH[] = "Победа Скаутов (Лексический анализ завершен без ошибок)";
	char TEMPLE_TRIUMPH[] = "Триумф в Храме (Синтаксический анализ завершен без ошибок)";
	char SOVET_TRIUMPH[] = "Триумф в Совете (Семантический анализ завершен без ошибок)";
	char BRAND_TRIUMPH[] = "Триумф после Клеймения (Преобразование выражений завершено без ошибок)";

	char SVOD[] = "--------------------СВОД КОНЕЧНЫХ СВИКОВ И ТАБЛИЦ РОДОВ-------------------";
	char STOP_ORDER[] = "\nПриказ: Выполнение программы остановлено";
	char GLORY_ORDER[] = "ПРОГРАММА УСПЕШНО ЗАВЕРШЕНА! Слава Империи!";

	setlocale(LC_ALL, "Russian");
	Log::LOG log;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getstream(parm.log);
		Log::writeLog(log);														//написать заголовок журнала
		Log::writeParm(log, parm);												//записать в журнал параметры
		In::IN in = In::getin(parm.in, log.stream);
		Log::writeIn(log.stream, in);

		in.words = In::getWordsTable(log.stream, in.text, in.code, in.size);	//разобрать на токены
		Lexer::LEX tables;
		bool lex_ok = Lexer::analyze(tables, in, log, parm);					//выполнить лексический анализ
		LT::writeLexTable(log.stream, tables.lextable);							//записать в журнал таблицы лексем и идентификаторов 
		IT::writeIdTable(log.stream, tables.idtable);							//а также соответствие токенов и лексем
		LT::writeLexemsOnLines(log.stream, tables.lextable);
		if (!lex_ok)
		{
			Log::writeLine(log.stream, SCRIBE_KRASH, "");
			Log::writeLine(&std::cout, SCRIBE_KRASH, STOP_ORDER, "");
			return 0;
		}
		else
		{
			Log::writeLine(&std::cout, SCRIBE_TRIUMPH, "");
		}
		MFST_TRACE_START(log.stream);
		MFST::Mfst mfst(tables, GRB::getGreibach());							//выполнить синтаксический анализ
		bool synt_ok = mfst.start(log);
		mfst.savededucation();
		mfst.printrules(log);													//вывести дерево разбора
		if (!synt_ok)
		{
			Log::writeLine(log.stream, TEMPLE_KRASH, "");
			Log::writeLine(&std::cout, TEMPLE_KRASH, STOP_ORDER, "");
			return 0;
		}
		else Log::writeLine(&std::cout, TEMPLE_TRIUMPH, "");
		bool sem_ok = Semantic::semanticsCheck(tables, log);					//выполнить семантический анализ
		if (!sem_ok)
		{
			Log::writeLine(log.stream, SOVET_KRASH, "");
			Log::writeLine(&std::cout, SOVET_KRASH, STOP_ORDER, "");
			return 0;
		}
		else Log::writeLine(&std::cout, SOVET_TRIUMPH, "");
		bool polish_ok = Polish::PolishNotation(tables, log);					//выполнить преобразование выражений в ПОЛИЗ
		if (!polish_ok)
		{
			Log::writeLine(log.stream, BRAND_KRASH, "");
			Log::writeLine(&std::cout, BRAND_KRASH, STOP_ORDER, "");
			return 0;
		}
		else Log::writeLine(&std::cout, BRAND_TRIUMPH, "");
		Log::writeLine(log.stream, SVOD, "");
		LT::writeLexTable(log.stream, tables.lextable);							//записать в журнал новые таблицы лексем и идентификаторов
		IT::writeIdTable(log.stream, tables.idtable);
		LT::writeLexemsOnLines(log.stream, tables.lextable);					//а также соответствие токенов и лексем
		//Log::writeLine(&std::cout, MESSAGE, "");
		//IT::writeIdTable(&std::cout, tables.idtable);							//записать в командную строку новые таблицы лексем и идентификаторов 
		//LT::writeLexTable(&std::cout, tables.lextable);							//а также соответствие токенов и лексем
		//LT::writeLexemsOnLines(&std::cout, tables.lextable);

		Gener::CodeGeneration(tables, parm, log);								//выполнить генерацию кода
		Log::writeLine(log.stream, GLORY_ORDER, "");									//итог работы программы
		Log::writeLine(&std::cout, GLORY_ORDER, "");
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message << ", строка " << e.position.line << ", позиция " << e.position.col << std::endl;
		system("pause");
	}
	return 0;
}