#include <iostream>
#include "stdafx.h"


int wmain(int argc, wchar_t* argv[])
{
	char SCRIBE_KRASH[] = "\033[31mПоражение Скаутов (Лексический анализ завершен с ошибками)\033[0m";
	char TEMPLE_KRASH[] = "\033[31mПадение Храма (Синтаксический анализ завершен с ошибками)\033[0m";
	char SOVET_KRASH[] = "\033[31mКрах Совета (Семантические ошибки)\033[0m";
	char BRAND_KRASH[] = "\033[31mКрах Клеймения (Ошибка при преобразовании выражения)\033[0m";

	char SCRIBE_TRIUMPH[] = "\033[32mПобеда Скаутов (Лексический анализ завершен без ошибок)\033[0m";
	char TEMPLE_TRIUMPH[] = "\033[32mТриумф в Храме (Синтаксический анализ завершен без ошибок)\033[0m";
	char SOVET_TRIUMPH[] = "\033[32mТриумф в Совете (Семантический анализ завершен без ошибок)\033[0m";
	char BRAND_TRIUMPH[] = "\033[32mТриумф после Клеймения (Преобразование выражений завершено без ошибок)\033[0m";

	char SVOD[] = "--------------------СВОД КОНЕЧНЫХ СВИТКОВ И ТАБЛИЦ РОДОВ-------------------";
	char STOP_ORDER[] = "\n\033[34mПриказ: Выполнение программы остановлено\033[0m";
	char GLORY_ORDER[] = "\033[32mПРОГРАММА УСПЕШНО ЗАВЕРШЕНА! Слава Империи!\033[0m";
	char DOK_CREATE[] = "\033[33m	Out.asm был призван! \033[0m";
	char LOG_CREATE[] = "\033[33m	Полный log-файл был призван! \033[0m";

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
		else { 
		Log::writeLine(&std::cout, TEMPLE_TRIUMPH, ""); 
		Visualizer::TreeBuilder treeBuilder;

		// Проходим по сохраненным шагам вывода
		for (int i = 0; i < mfst.deducation.size; i++)
		{
			// Получаем правило по индексу
			short nrule = mfst.deducation.nrules[i];
			short nchain = mfst.deducation.nrulechains[i];

			// Получаем строковое представление правила (например, S->pfiPGS)
			// Для этого используем getCRule из GRB, но нам нужно немного модифицировать доступ 
			// или использовать буфер.
			// В GRB.cpp у тебя есть метод getCRule. 
			// Давай получим правило напрямую через грамматику.

			GRB::Rule rule = GRB::getGreibach().getRule(nrule);
			char rbuf[300];
			rule.getCRule(rbuf, nchain); // Получили строку вида "S->pfiPGS"

			treeBuilder.processRule(std::string(rbuf));
		}

		treeBuilder.generateGraphImage("parse_tree.dot", "parse_tree.png");
		
		}
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
		std::cout << DOK_CREATE << std::endl;
		std::cout << LOG_CREATE << std::endl;
	}
	catch (Error::ERROR e)
	{
		std::cout << "Ошибка " << e.id << ": " << e.message << ", строка " << e.position.line << ", позиция " << e.position.col << std::endl;
		system("pause");
	}
	return 0;
}