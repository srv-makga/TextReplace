#pragma once

#include <string>
#include <regex>
#include <tchar.h>

namespace std
{
	using tstringstring = basic_stringstream<TCHAR, char_traits<TCHAR>, allocator<TCHAR>>;
	using tstring = basic_string <TCHAR, char_traits<TCHAR>, allocator<TCHAR>>;
	using tifstream = basic_ifstream<TCHAR, char_traits<TCHAR>>;
	using tofstream = basic_ofstream<TCHAR, char_traits<TCHAR>>;
	using tregex = basic_regex<TCHAR>;
	using tregex_token_iterator = regex_token_iterator<tstring::const_iterator>;
}