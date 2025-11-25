#include "SemanticAnalyze.h"
#include "IT.h"
#include "LT.h"
#include "Error.h"
#include "LexAnalysis.h"
#include "Log.h"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Semantic
{
	namespace
	{
		struct FunctionContext
		{
			IT::IDDATATYPE returnType = IT::IDDATATYPE::UNDEF;
			int entryIndex = TI_NULLIDX;
		};

		struct ExpressionInfo
		{
			IT::IDDATATYPE resultingType = IT::IDDATATYPE::UNDEF;
			bool hasArithmetic = false;
			bool hasBitwise = false;
			bool invalidArithmeticOperand = false;
			bool invalidBitwiseOperand = false;
			bool divisionByZero = false;
			bool usedVoidOperand = false;
			bool hasOperands = false;
		};

		struct StdFunctionInfo
		{
			IT::IDDATATYPE returnType = IT::IDDATATYPE::UNDEF;
			std::vector<std::vector<IT::IDDATATYPE>> paramAlternatives;
			bool usesParentheses = true;
		};

		bool isStdFunctionLex(char lex)
		{
			switch (lex)
			{
			case LEX_COMPSCROLLS:
			case LEX_CONSOLIDATE:
			case LEX_MIGHTINESS:
			case LEX_FILAMENT:
			case LEX_PRINT:
				return true;
			default:
				return false;
			}
		}

		bool getStdFunctionInfo(char lex, StdFunctionInfo& info)
		{
			switch (lex)
			{
			case LEX_COMPSCROLLS:
				info.returnType = COMP_SCROL_TYPE;
				info.paramAlternatives = { {IT::IDDATATYPE::STR}, {IT::IDDATATYPE::STR} };
				info.usesParentheses = true;
				return true;
			case LEX_CONSOLIDATE:
				info.returnType = CONSOLIDATE_TYPE;
				info.paramAlternatives = { {IT::IDDATATYPE::STR}, {IT::IDDATATYPE::STR} };
				info.usesParentheses = true;
				return true;
			case LEX_MIGHTINESS:
				info.returnType = MIGHTINESS_TYPE;
				info.paramAlternatives = { {IT::IDDATATYPE::INT}, {IT::IDDATATYPE::INT} };
				info.usesParentheses = true;
				return true;
			case LEX_FILAMENT:
				info.returnType = FILAMENT_TYPE;
				info.paramAlternatives = { {IT::IDDATATYPE::STR} };
				info.usesParentheses = true;
				return true;
			case LEX_PRINT:
				info.returnType = IT::IDDATATYPE::VOID;
				info.paramAlternatives = { {IT::IDDATATYPE::STR, IT::IDDATATYPE::INT, IT::IDDATATYPE::BOOL, IT::IDDATATYPE::CHAR} };
				info.usesParentheses = false;
				return true;
			default:
				break;
			}
			return false;
		}

		int getLineByLexIndex(const Lexer::LEX& tables, int lexIdx)
		{
			if (lexIdx >= 0 && lexIdx < tables.lextable.size)
				return tables.lextable.table[lexIdx].sn;
			if (tables.lextable.size > 0)
				return tables.lextable.table[tables.lextable.size - 1].sn;
			return 0;
		}

		void reportError(Log::LOG& log, const Lexer::LEX& tables, int errorId, int lexIdx, bool& status)
		{
			status = false;
			int line = getLineByLexIndex(tables, lexIdx);
			Log::writeError(log.stream, Error::GetError(errorId, line, 0));
		}

		int findClosingBracket(const LT::LexTable& lexTable, int openIdx, char openLex, char closeLex)
		{
			int depth = 0;
			for (int i = openIdx; i < lexTable.size; ++i)
			{
				char lex = lexTable.table[i].lexema;
				if (lex == openLex)
					++depth;
				else if (lex == closeLex)
				{
					--depth;
					if (depth == 0)
						return i;
				}
			}
			return -1;
		}

		int findLexeme(const LT::LexTable& lexTable, int startIdx, char target)
		{
			for (int i = startIdx; i < lexTable.size; ++i)
				if (lexTable.table[i].lexema == target)
					return i;
			return -1;
		}

		bool isArithmeticOperator(char lex)
		{
			return lex == LEX_PLUS || lex == LEX_MINUS || lex == LEX_STAR || lex == LEX_DIRSLASH;
		}

		bool isDeclarationId(const LT::LexTable& lexTable, int idx)
		{
			if (idx < 1) return false;
			char prev = lexTable.table[idx - 1].lexema;
			if (prev == LEX_INTSTRBOOLCHAR && idx >= 2)
			{
				char beforePrev = lexTable.table[idx - 2].lexema;
				if (beforePrev == LEX_ELDER || beforePrev == LEX_ACTION || beforePrev == LEX_COMMA || beforePrev == LEX_LEFTHESIS)
					return true;
			}
			if (prev == LEX_HOLLOW && idx >= 2 && lexTable.table[idx - 2].lexema == LEX_ACTION)
				return true;
			return false;
		}

		bool isTypeTokenContextValid(const LT::LexTable& lexTable, int idx)
		{
			if (idx < 1) return false;
			char prev = lexTable.table[idx - 1].lexema;
			return prev == LEX_ELDER || prev == LEX_ACTION || prev == LEX_COMMA || prev == LEX_LEFTHESIS;
		}

		bool detectDivisionByZero(Lexer::LEX& tables, int operatorIdx, int endIdx)
		{
			const auto& lexTable = tables.lextable;
			for (int j = operatorIdx + 1; j < endIdx && j < lexTable.size; ++j)
			{
				char lex = lexTable.table[j].lexema;
				if (lex == LEX_LEFTBRAC || lex == LEX_RIGHTBRAC || lex == LEX_LEFTHESIS || lex == LEX_RIGHTTHESIS || lex == LEX_COMMA)
					continue;
				int idxTI = lexTable.table[j].idxTI;
				if (idxTI == TI_NULLIDX)
					return false;
				IT::Entry& entry = tables.idtable.table[idxTI];
				if (entry.idtype == IT::IDTYPE::L && entry.iddatatype == IT::IDDATATYPE::INT && entry.value.vint == 0)
					return true;
				return false;
			}
			return false;
		}

		ExpressionInfo analyzeExpression(Lexer::LEX& tables, int startIdx, int endIdx)
		{
			ExpressionInfo info;
			auto& lexTable = tables.lextable;
			IT::IDDATATYPE lastOperandType = IT::IDDATATYPE::UNDEF;

			for (int j = startIdx; j < endIdx && j < lexTable.size; ++j)
			{
				char lex = lexTable.table[j].lexema;

				if (isArithmeticOperator(lex))
				{
					// Проверяем тип предыдущего операнда только если он определен
					if (lastOperandType != IT::IDDATATYPE::UNDEF && lastOperandType != IT::IDDATATYPE::INT)
						info.invalidArithmeticOperand = true;
					if (lex == LEX_DIRSLASH && detectDivisionByZero(tables, j, endIdx))
						info.divisionByZero = true;
					info.hasArithmetic = true;
					// Сбрасываем lastOperandType, чтобы проверить следующий операнд
					lastOperandType = IT::IDDATATYPE::UNDEF;
					continue;
				}

				if (lex == LEX_BIND || lex == LEX_ALLY)
				{
					if (lastOperandType != IT::IDDATATYPE::UNDEF && lastOperandType != IT::IDDATATYPE::INT)
						info.invalidBitwiseOperand = true;
					info.hasBitwise = true;
					continue;
				}

				if (lex == LEX_BANISH)
				{
					info.hasBitwise = true;
					lastOperandType = IT::IDDATATYPE::UNDEF;
					continue;
				}

				if (isStdFunctionLex(lex))
				{
					StdFunctionInfo stdInfo;
					if (getStdFunctionInfo(lex, stdInfo))
					{
						if (stdInfo.returnType == IT::IDDATATYPE::VOID)
						{
							info.usedVoidOperand = true;
						}
						else
						{
							if (info.resultingType == IT::IDDATATYPE::UNDEF)
								info.resultingType = stdInfo.returnType;
							lastOperandType = stdInfo.returnType;
							info.hasOperands = true;
							if (info.hasArithmetic && stdInfo.returnType != IT::IDDATATYPE::INT)
								info.invalidArithmeticOperand = true;
							if (info.hasBitwise && stdInfo.returnType != IT::IDDATATYPE::INT)
								info.invalidBitwiseOperand = true;
						}

						if (stdInfo.usesParentheses)
						{
							int closing = findClosingBracket(lexTable, j + 1, LEX_LEFTBRAC, LEX_RIGHTBRAC);
							if (closing != -1)
								j = closing;
						}
						else
						{
							if (j + 1 < endIdx)
								++j;
						}
					}
					continue;
				}

				if (lex == LEX_ID || lex == LEX_LITERAL)
				{
					int idxTI = lexTable.table[j].idxTI;
					if (idxTI == TI_NULLIDX)
						continue;

					IT::Entry& operand = tables.idtable.table[idxTI];
					IT::IDDATATYPE operandType = operand.iddatatype;

					bool isCall = (lex == LEX_ID &&
						j + 1 < endIdx &&
						lexTable.table[j + 1].lexema == LEX_LEFTBRAC &&
						(operand.idtype == IT::IDTYPE::F || operand.idtype == IT::IDTYPE::S));

					if (isCall)
					{
						int closing = findClosingBracket(lexTable, j + 1, LEX_LEFTBRAC, LEX_RIGHTBRAC);
						if (closing != -1)
							j = closing;
					}

					if (operandType == IT::IDDATATYPE::VOID)
						info.usedVoidOperand = true;

					if (info.resultingType == IT::IDDATATYPE::UNDEF)
						info.resultingType = operandType;

					// Проверяем типы только для определенных типов (не UNDEF)
					if (operandType != IT::IDDATATYPE::UNDEF)
					{
						if (info.hasArithmetic && operandType != IT::IDDATATYPE::INT)
							info.invalidArithmeticOperand = true;
						if (info.hasBitwise && operandType != IT::IDDATATYPE::INT)
							info.invalidBitwiseOperand = true;
					}

					lastOperandType = operandType;
					info.hasOperands = true;
				}
			}

			// Для арифметических выражений результат всегда INT
			if (info.hasArithmetic)
			{
				info.resultingType = IT::IDDATATYPE::INT;
			}
			// Для битовых выражений результат тоже всегда INT
			if (info.hasBitwise)
			{
				info.resultingType = IT::IDDATATYPE::INT;
			}

			return info;
		}

		int registerFunctionParams(Lexer::LEX& tables, int openIdx, int closeIdx, Log::LOG& log, bool& status)
		{
			int count = 0;
			auto& lexTable = tables.lextable;

			for (int j = openIdx + 1; j < closeIdx; ++j)
			{
				if (lexTable.table[j].lexema != LEX_ID)
					continue;

				int idxTI = lexTable.table[j].idxTI;
				if (idxTI == TI_NULLIDX)
					continue;

				IT::Entry& param = tables.idtable.table[idxTI];
				if (param.idtype != IT::IDTYPE::P)
					continue;

				++count;
				if (param.iddatatype == IT::IDDATATYPE::UNDEF)
					reportError(log, tables, 303, j, status);
			}

			return count;
		}

		void handleVariableDeclaration(Lexer::LEX& tables, Log::LOG& log, int lexIdx,
			std::unordered_set<int>& declaredVars, bool& status)
		{
			auto& lexTable = tables.lextable;
			if (lexIdx + 1 >= lexTable.size || lexTable.table[lexIdx + 1].lexema != LEX_INTSTRBOOLCHAR)
			{
				reportError(log, tables, 303, lexIdx, status);
				return;
			}

			if (lexIdx + 2 >= lexTable.size || lexTable.table[lexIdx + 2].lexema != LEX_ID)
			{
				reportError(log, tables, 304, lexIdx, status);
				return;
			}

			int idxTI = lexTable.table[lexIdx + 2].idxTI;
			if (idxTI == TI_NULLIDX)
				return;

			if (!declaredVars.insert(idxTI).second)
				reportError(log, tables, 305, lexIdx + 2, status);
		}

		int handleFunctionSignature(Lexer::LEX& tables, Log::LOG& log, int actionIdx,
			std::unordered_set<int>& declaredFunctions,
			std::unordered_map<int, int>& declaredParamCounts,
			bool& status)
		{
			auto& lexTable = tables.lextable;

			if (actionIdx + 1 >= lexTable.size)
			{
				reportError(log, tables, 303, actionIdx, status);
				return TI_NULLIDX;
			}

			char typeLex = lexTable.table[actionIdx + 1].lexema;
			if (typeLex != LEX_INTSTRBOOLCHAR && typeLex != LEX_HOLLOW)
			{
				reportError(log, tables, 303, actionIdx, status);
				return TI_NULLIDX;
			}

			int idIdx = actionIdx + 2;
			if (idIdx >= lexTable.size || lexTable.table[idIdx].lexema != LEX_ID)
			{
				reportError(log, tables, 304, actionIdx, status);
				return TI_NULLIDX;
			}

			int funcIdx = lexTable.table[idIdx].idxTI;
			if (funcIdx == TI_NULLIDX)
				return TI_NULLIDX;

			if (!declaredFunctions.insert(funcIdx).second)
				reportError(log, tables, 305, idIdx, status);

			int paramsOpenIdx = idIdx + 1;
			if (paramsOpenIdx < lexTable.size && lexTable.table[paramsOpenIdx].lexema == LEX_LEFTHESIS)
			{
				int paramsCloseIdx = findClosingBracket(lexTable, paramsOpenIdx, LEX_LEFTHESIS, LEX_RIGHTTHESIS);
				if (paramsCloseIdx != -1)
				{
					int declaredCount = registerFunctionParams(tables, paramsOpenIdx, paramsCloseIdx, log, status);
					declaredParamCounts[funcIdx] = declaredCount;
					if (declaredCount > MAX_PARAMS_COUNT)
						reportError(log, tables, 306, paramsOpenIdx, status);
				}
			}

			return funcIdx;
		}

		std::vector<IT::IDDATATYPE> collectCallArguments(Lexer::LEX& tables, int openIdx, int closeIdx)
		{
			std::vector<IT::IDDATATYPE> args;
			auto& lexTable = tables.lextable;

			for (int j = openIdx + 1; j < closeIdx; ++j)
			{
				char lex = lexTable.table[j].lexema;

				if (isStdFunctionLex(lex))
				{
					StdFunctionInfo stdInfo;
					if (getStdFunctionInfo(lex, stdInfo))
					{
						args.push_back(stdInfo.returnType);
						if (stdInfo.usesParentheses)
						{
							int nestedClose = findClosingBracket(lexTable, j + 1, LEX_LEFTBRAC, LEX_RIGHTBRAC);
							if (nestedClose != -1)
								j = nestedClose;
						}
						else
						{
							if (j + 1 < closeIdx)
								++j;
						}
					}
					continue;
				}

				if (lex != LEX_ID && lex != LEX_LITERAL)
					continue;

				int idxTI = lexTable.table[j].idxTI;
				if (idxTI == TI_NULLIDX)
					continue;

				IT::Entry& entry = tables.idtable.table[idxTI];
				IT::IDDATATYPE type = entry.iddatatype;

				if (lex == LEX_ID &&
					j + 1 < closeIdx &&
					lexTable.table[j + 1].lexema == LEX_LEFTBRAC &&
					(entry.idtype == IT::IDTYPE::F || entry.idtype == IT::IDTYPE::S))
				{
					int nestedClose = findClosingBracket(lexTable, j + 1, LEX_LEFTBRAC, LEX_RIGHTBRAC);
					if (nestedClose != -1)
						j = nestedClose;
				}

				args.push_back(type);
			}

			return args;
		}

		void handleFunctionCall(Lexer::LEX& tables, Log::LOG& log, int lexIdx,
			std::unordered_map<int, int>& declaredParamCounts,
			bool& status)
		{
			auto& lexTable = tables.lextable;
			if (lexIdx + 1 >= lexTable.size || lexTable.table[lexIdx + 1].lexema != LEX_LEFTBRAC)
				return;

			int funcIdx = lexTable.table[lexIdx].idxTI;
			if (funcIdx == TI_NULLIDX)
				return;

			IT::Entry& functionEntry = tables.idtable.table[funcIdx];
			if (functionEntry.idtype != IT::IDTYPE::F && functionEntry.idtype != IT::IDTYPE::S)
				return;

			int closeIdx = findClosingBracket(lexTable, lexIdx + 1, LEX_LEFTBRAC, LEX_RIGHTBRAC);
			if (closeIdx == -1)
			{
				reportError(log, tables, 308, lexIdx, status);
				return;
			}

			auto args = collectCallArguments(tables, lexIdx + 1, closeIdx);
			int actualCount = static_cast<int>(args.size());
			if (actualCount > MAX_PARAMS_COUNT)
				reportError(log, tables, 307, lexIdx, status);

			int expectedCount = functionEntry.value.params.count;
			auto it = declaredParamCounts.find(funcIdx);
			if (it != declaredParamCounts.end())
				expectedCount = it->second;

			if (expectedCount != actualCount)
				reportError(log, tables, 308, lexIdx, status);

			int comparable = std::min(static_cast<int>(args.size()), functionEntry.value.params.count);
			for (int i = 0; i < comparable; ++i)
			{
				if (!functionEntry.value.params.types)
					break;

				IT::IDDATATYPE expected = functionEntry.value.params.types[i];
				if (expected == IT::IDDATATYPE::UNDEF || args[i] == IT::IDDATATYPE::UNDEF)
					continue;

				if (expected != args[i])
				{
					reportError(log, tables, 309, lexIdx, status);
					break;
				}
			}
		}

		void handleStdFunctionCall(Lexer::LEX& tables, Log::LOG& log, int lexIdx, char lex, bool& status)
		{
			StdFunctionInfo info;
			if (!getStdFunctionInfo(lex, info))
				return;

			std::vector<IT::IDDATATYPE> args;
			auto& lexTable = tables.lextable;

			if (info.usesParentheses)
			{
				if (lexIdx + 1 >= lexTable.size || lexTable.table[lexIdx + 1].lexema != LEX_LEFTBRAC)
				{
					reportError(log, tables, 308, lexIdx, status);
					return;
				}

				int closeIdx = findClosingBracket(lexTable, lexIdx + 1, LEX_LEFTBRAC, LEX_RIGHTBRAC);
				if (closeIdx == -1)
				{
					reportError(log, tables, 308, lexIdx, status);
					return;
				}

				args = collectCallArguments(tables, lexIdx + 1, closeIdx);
			}
			else
			{
				int argIdx = lexIdx + 1;
				if (argIdx >= lexTable.size)
				{
					reportError(log, tables, 308, lexIdx, status);
					return;
				}

				char nextLex = lexTable.table[argIdx].lexema;
				if (nextLex != LEX_ID && nextLex != LEX_LITERAL)
				{
					reportError(log, tables, 308, lexIdx, status);
					return;
				}

				int idxTI = lexTable.table[argIdx].idxTI;
				if (idxTI == TI_NULLIDX)
				{
					reportError(log, tables, 308, lexIdx, status);
					return;
				}
				args.push_back(tables.idtable.table[idxTI].iddatatype);
			}

			if (args.size() != info.paramAlternatives.size())
			{
				reportError(log, tables, 308, lexIdx, status);
				return;
			}

			for (size_t i = 0; i < args.size(); ++i)
			{
				bool matches = false;
				for (IT::IDDATATYPE expected : info.paramAlternatives[i])
				{
					if (expected == args[i])
					{
						matches = true;
						break;
					}
				}
				if (!matches)
				{
					reportError(log, tables, 309, lexIdx, status);
					return;
				}
			}
		}

		void handleAssignment(Lexer::LEX& tables, Log::LOG& log, int eqIdx, bool& status)
		{
			auto& lexTable = tables.lextable;
			if (eqIdx == 0)
				return;

			int lhsIdx = eqIdx - 1;
			while (lhsIdx >= 0 && lexTable.table[lhsIdx].idxTI == TI_NULLIDX)
				--lhsIdx;
			if (lhsIdx < 0)
				return;

			int entryIdx = lexTable.table[lhsIdx].idxTI;
			if (entryIdx == TI_NULLIDX)
				return;

			IT::Entry& leftEntry = tables.idtable.table[entryIdx];
			IT::IDDATATYPE leftType = leftEntry.iddatatype;

			int semicolonIdx = findLexeme(lexTable, eqIdx, LEX_SEMICOLON);
			if (semicolonIdx == -1)
				semicolonIdx = lexTable.size;

			ExpressionInfo expr = analyzeExpression(tables, eqIdx + 1, semicolonIdx);

			if (expr.divisionByZero)
				reportError(log, tables, 318, eqIdx, status);

			if (expr.usedVoidOperand)
				reportError(log, tables, 314, eqIdx, status);

			// Для битовых выражений результат всегда INT
			if (expr.hasBitwise)
			{
				if (expr.invalidBitwiseOperand)
					reportError(log, tables, 314, eqIdx, status);
				if (leftType != IT::IDDATATYPE::INT)
					reportError(log, tables, 314, eqIdx, status);
				return;
			}

			// Для арифметических выражений результат всегда INT
			if (expr.hasArithmetic)
			{
				if (expr.invalidArithmeticOperand)
					reportError(log, tables, 314, eqIdx, status);
				if (leftType != IT::IDDATATYPE::INT)
				{
					reportError(log, tables, 314, eqIdx, status);
				}
				if (leftType == IT::IDDATATYPE::STR)
				{
					reportError(log, tables, 316, eqIdx, status);
				}
				return; // Для арифметических выражений проверка завершена
			}

			// Для неарифметических и небитовых выражений проверяем совпадение типов
			if (!expr.hasBitwise && expr.hasOperands &&
				expr.resultingType != IT::IDDATATYPE::UNDEF && leftType != expr.resultingType)
			{
				reportError(log, tables, 314, eqIdx, status);
			}
		}

		void handleReturn(Lexer::LEX& tables, Log::LOG& log, int lexIdx,
			std::vector<FunctionContext>& stack, bool& status)
		{
			if (stack.empty())
			{
				reportError(log, tables, 315, lexIdx, status);
				return;
			}

			FunctionContext current = stack.back();
			int semicolonIdx = findLexeme(tables.lextable, lexIdx, LEX_SEMICOLON);
			if (semicolonIdx == -1)
				semicolonIdx = tables.lextable.size;

			if (current.returnType == IT::IDDATATYPE::VOID)
			{
				if (semicolonIdx > lexIdx + 1)
					reportError(log, tables, 315, lexIdx, status);
				return;
			}

			if (semicolonIdx <= lexIdx + 1)
			{
				reportError(log, tables, 315, lexIdx, status);
				return;
			}

			ExpressionInfo expr = analyzeExpression(tables, lexIdx + 1, semicolonIdx);

			if (expr.usedVoidOperand || !expr.hasOperands)
			{
				reportError(log, tables, 315, lexIdx, status);
				return;
			}

			if (expr.hasArithmetic && current.returnType != IT::IDDATATYPE::INT)
			{
				reportError(log, tables, 315, lexIdx, status);
				return;
			}

			if (expr.hasBitwise && current.returnType != IT::IDDATATYPE::INT)
			{
				reportError(log, tables, 315, lexIdx, status);
				return;
			}

			if (expr.resultingType != IT::IDDATATYPE::UNDEF && expr.resultingType != current.returnType)
				reportError(log, tables, 315, lexIdx, status);
		}

		void handleCondition(Lexer::LEX& tables, Log::LOG& log, int lexIdx, bool& status)
		{
			auto& lexTable = tables.lextable;
			
			// Ищем открывающую скобку [ после patrol/charge (без ограничения)
			int openIdx = findLexeme(lexTable, lexIdx, LEX_LEFTHESIS);
			if (openIdx == -1 || openIdx > lexIdx + 10)
			{
				reportError(log, tables, 317, lexIdx, status);
				return;
			}

			int closeIdx = findClosingBracket(lexTable, openIdx, LEX_LEFTHESIS, LEX_RIGHTTHESIS);
			if (closeIdx == -1)
			{
				reportError(log, tables, 317, lexIdx, status);
				return;
			}

			// Собираем операнды и операторы из условия
			int validOperandCount = 0;  // операнды с правильным типом (INT или BOOL)
			int totalOperandCount = 0;   // все операнды (включая UNDEF)
			int comparisonOpCount = 0;
			bool invalidOperand = false;

			for (int j = openIdx + 1; j < closeIdx; ++j)
			{
				char lex = lexTable.table[j].lexema;
				
				// Пропускаем разделители и скобки
				if (lex == LEX_COMMA || lex == LEX_LEFTBRAC || lex == LEX_RIGHTBRAC || 
				    lex == LEX_LEFTHESIS || lex == LEX_RIGHTTHESIS || lex == LEX_SEMICOLON)
					continue;

				// Обработка операторов сравнения
				if (lex == LEX_MORE || lex == LEX_LESS || lex == LEX_EQUALS || lex == LEX_NONEQUALS)
				{
					comparisonOpCount++;
					continue;
				}

				// Обработка = после > или < (для >= и <=)
				if (lex == LEX_ASSIGN)
				{
					// Пропускаем = в >= и <=
					continue;
				}

				// Обработка операндов (идентификаторы и литералы)
				if (lex == LEX_ID || lex == LEX_LITERAL)
				{
					totalOperandCount++;
					
					int idxTI = lexTable.table[j].idxTI;
					if (idxTI == TI_NULLIDX)
					{
						// Неопределенный идентификатор - пропускаем (это ошибка 300, не 317)
						continue;
					}

					IT::Entry& entry = tables.idtable.table[idxTI];
					IT::IDDATATYPE operandType = entry.iddatatype;

					// Проверка типа операнда - должен быть INT или BOOL
					if (operandType == IT::IDDATATYPE::INT || operandType == IT::IDDATATYPE::BOOL)
					{
						validOperandCount++;
					}
					else if (operandType != IT::IDDATATYPE::UNDEF)
					{
						// Неправильный тип операнда
						invalidOperand = true;
						break;
					}
					// Если UNDEF - пропускаем, это будет обработано как ошибка 300
				}
			}

			// Проверка корректности условия
			if (invalidOperand)
			{
				reportError(log, tables, 317, lexIdx, status);
				return;
			}

			// Если есть оператор сравнения, должно быть минимум 2 операнда (даже если некоторые UNDEF)
			// Но если все операнды UNDEF, это не ошибка 317 (это ошибка 300)
			if (comparisonOpCount > 0)
			{
				// Есть оператор сравнения - должно быть минимум 2 операнда
				if (totalOperandCount < 2)
				{
					reportError(log, tables, 317, lexIdx, status);
					return;
				}
				// Если есть хотя бы 2 операнда, но не все имеют правильный тип - это нормально
				// (неправильные типы будут обработаны как ошибка 300)
			}
			else
			{
				// Нет оператора сравнения - должен быть хотя бы 1 операнд
				if (totalOperandCount == 0)
				{
					reportError(log, tables, 317, lexIdx, status);
					return;
				}
			}

			// Во всех остальных случаях - валидно
		}
	}

	bool semanticsCheck(Lexer::LEX& tables, Log::LOG& log)
	{
		bool semOk = true;
		auto& lexTable = tables.lextable;

		std::unordered_set<int> declaredVars;
		std::unordered_set<int> declaredFunctions;
		std::unordered_map<int, int> declaredParamCounts;
		std::vector<FunctionContext> functionStack;
		int pendingFunctionIdx = TI_NULLIDX;

		for (int i = 0; i < lexTable.size; ++i)
		{
			char lex = lexTable.table[i].lexema;

			switch (lex)
			{
			case LEX_ELDER:
				handleVariableDeclaration(tables, log, i, declaredVars, semOk);
				break;
			case LEX_INTSTRBOOLCHAR:
				if (!isTypeTokenContextValid(lexTable, i))
					reportError(log, tables, 304, i, semOk);
				break;
			case LEX_ACTION:
				pendingFunctionIdx = handleFunctionSignature(tables, log, i, declaredFunctions, declaredParamCounts, semOk);
				break;
			case LEX_LEFTBRACE:
				if (pendingFunctionIdx != TI_NULLIDX)
				{
					FunctionContext ctx;
					ctx.returnType = tables.idtable.table[pendingFunctionIdx].iddatatype;
					ctx.entryIndex = pendingFunctionIdx;
					functionStack.push_back(ctx);
					pendingFunctionIdx = TI_NULLIDX;
				}
				break;
			case LEX_BRACELET:
				if (!functionStack.empty())
					functionStack.pop_back();
				break;
			case LEX_ID:
				if (!isDeclarationId(lexTable, i))
					handleFunctionCall(tables, log, i, declaredParamCounts, semOk);
				break;
			case LEX_ASSIGN:
				handleAssignment(tables, log, i, semOk);
				break;
			case LEX_COMEBACK:
				handleReturn(tables, log, i, functionStack, semOk);
				break;
			case LEX_COMPSCROLLS:
			case LEX_CONSOLIDATE:
			case LEX_MIGHTINESS:
			case LEX_FILAMENT:
			case LEX_PRINT:
				handleStdFunctionCall(tables, log, i, lex, semOk);
				break;
			case LEX_CHARGE:
			case LEX_PATROL:
				handleCondition(tables, log, i, semOk);
				break;
			default:
				break;
			}
		}

		for (int idx = 0; idx < tables.idtable.size; ++idx)
		{
			IT::Entry& entry = tables.idtable.table[idx];
			if ((entry.idtype == IT::IDTYPE::V || entry.idtype == IT::IDTYPE::F || entry.idtype == IT::IDTYPE::P) &&
				entry.iddatatype == IT::IDDATATYPE::UNDEF)
			{
				reportError(log, tables, 300, entry.idxfirstLE, semOk);
			}
		}

		return semOk;
	}
}