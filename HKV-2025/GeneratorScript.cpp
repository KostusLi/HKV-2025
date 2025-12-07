//#include "Header.h"
//#include "GeneratorScript.h"
//#include "Error.h"
//#include <fstream>
//#include <sstream>
//#include <regex>
//#include <cwchar>
//#include <cstdlib>
//#include <direct.h>
//
//using namespace std;
//
//namespace
//{
//	string trim(const string& s)
//	{
//		const auto start = s.find_first_not_of(" \t\r\n");
//		if (start == string::npos) return {};
//		const auto end = s.find_last_not_of(" \t\r\n");
//		return s.substr(start, end - start + 1);
//	}
//
//	string replaceKeywords(const string& line)
//	{
//		string res = line;
//		// Basic keyword replacements to JS
//		static const pair<regex, string> replacements[] = {
//			{ regex(R"(\baction\b)"), "function" },
//			{ regex(R"(\btemple\b)"), "" },
//			{ regex(R"(\belder\b)"), "let" },
//			{ regex(R"(\bsquire\b)"), "" },
//			{ regex(R"(\bscroll\b)"), "" },
//			{ regex(R"(\brune\b)"), "" },
//			{ regex(R"(\bhonor\b)"), "" },
//			{ regex(R"(\bhollow\b)"), "" },
//			{ regex(R"(\bcomeback\b)"), "return" },
//			{ regex(R"(\bcharge\b)"), "if" },
//			{ regex(R"(\bbackup\b)"), "else" },
//			{ regex(R"(\bpatrol\b)"), "while" },
//			{ regex(R"(\bcouncil\b)"), "switch" },
//			{ regex(R"(\bpath\b)"), "case" },
//			{ regex(R"(\btiresome\b)"), "default" },
//			{ regex(R"(\bnewleaf\b)"), "newleaf()" },
//			{ regex(R"(\bconfession\b)"), "confession" },
//			{ regex(R"(\bally\b)"), "||" },
//			{ regex(R"(\bbind\b)"), "&&" },
//			{ regex(R"(\bbanish\b)"), "~" }
//		};
//
//		for (const auto& [re, val] : replacements)
//			res = regex_replace(res, re, val);
//
//		// Logical literals
//		res = regex_replace(res, regex(R"(\bfaith\b)"), "true");
//		res = regex_replace(res, regex(R"(\bexile\b)"), "false");
//
//		// Comparisons / bitwise
//		res = regex_replace(res, regex(R"(?)"), "!=");
//		res = regex_replace(res, regex(R"(!)"), "=="); // language: ! means equals
//		res = regex_replace(res, regex(R"(@)"), "<=");
//		res = regex_replace(res, regex(R"(#)"), ">=");
//		res = regex_replace(res, regex(R"(&)"), "&&");
//		res = regex_replace(res, regex(R"(\|)"), "||");
//		res = regex_replace(res, regex(R"(~)"), "~");
//		return res;
//	}
//}
//
//namespace GenerScript
//{
//	void GenerateJS(const Parm::PARM& parm, Log::LOG& log)
//	{
//		// Resolve input path (wide paths for Windows)
//		wchar_t fullIn[_MAX_PATH]{};
//		if (_wfullpath(fullIn, parm.in, _MAX_PATH) == nullptr)
//			throw ERROR_THROW(600); // invalid input path
//
//		ifstream src(fullIn);
//		if (!src.is_open())
//			throw ERROR_THROW(601); // cannot open input
//
//		// Base directory of input
//		std::wstring baseDir(fullIn);
//		size_t slash = baseDir.find_last_of(L"\\/"); 
//		if (slash != std::wstring::npos)
//			baseDir = baseDir.substr(0, slash);
//		else
//			baseDir = L".";
//
//		// Build ../JSCode path
//		std::wstring jsDir = baseDir + L"\\..\\JSCode";
//		wchar_t fullOutDir[_MAX_PATH]{};
//		if (_wfullpath(fullOutDir, jsDir.c_str(), _MAX_PATH) == nullptr)
//			throw ERROR_THROW(602); // cannot build output path
//
//		// Create directories recursively
//		std::wstring dirAcc;
//		std::wstringstream ss(fullOutDir);
//		std::wstring segment;
//		while (std::getline(ss, segment, L'\\'))
//		{
//			if (segment.empty()) continue;
//			if (!dirAcc.empty()) dirAcc += L"\\";
//			dirAcc += segment;
//			_wmkdir(dirAcc.c_str()); // ignore if exists
//		}
//
//		std::wstring outPath = std::wstring(fullOutDir) + L"\\code.js";
//		ofstream dst(outPath.c_str(), ios::trunc);
//		if (!dst.is_open())
//			throw ERROR_THROW(602); // cannot create output
//
//		// Prolog with helpers and wrapper
//		dst << "// Auto-generated from HKV to JavaScript\n";
//		dst << "(() => {\n";
//		dst << "  function compare_scrolls(a, b) { return String(a) === String(b); }\n";
//		dst << "  function consolidate(a, b) { return String(a) + String(b); }\n";
//		dst << "  function mightiness(a, b) { return (a > b ? a : b); }\n";
//		dst << "  function filament(s) { return String(s).length; }\n";
//		dst << "  function confession(x) { console.log(x); }\n";
//		dst << "  function newleaf() { console.log(\"\"); }\n";
//		dst << "  function ally(a, b) { return a || b; }\n";
//		dst << "  function bind(a, b) { return a && b; }\n";
//		dst << "  function banish(a) { return ~a; }\n";
//
//		string line;
//		while (std::getline(src, line))
//		{
//			const string t = trim(line);
//			if (t.empty())
//			{
//				dst << "\n";
//				continue;
//			}
//
//			string converted = replaceKeywords(t);
//
//			// Arguments: [a, b] -> (a, b)
//			converted = regex_replace(converted, regex(R"(\[)"), "(");
//			converted = regex_replace(converted, regex(R"(\])"), ")");
//
//			// confession x;  -> confession(x);
//			converted = regex_replace(converted, regex(R"(\bconfession\s+([^;]+))"), "confession($1)");
//
//			dst << "  " << converted << "\n";
//		}
//
//
//		if (log.stream)
//		{
//			*log.stream << "JavaScript code generated to: " << std::string(outPath.begin(), outPath.end()) << std::endl;
//		}
//	}
//}
//
