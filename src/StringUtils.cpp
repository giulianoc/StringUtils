/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   JSONUtils.cpp
 * Author: giuliano
 *
 * Created on March 29, 2018, 6:27 AM
 */

#include "StringUtils.h"
#include <algorithm>
#include <format>
#include <locale>
#include <regex>
#include <utility>
#ifdef UTFCPP
	#include <utf8.h>
#endif


string StringUtils::ltrim(string s)
{
	auto it = find_if(s.begin(), s.end(), [](char c) { return !isspace<char>(c, locale::classic()); });
	s.erase(s.begin(), it);

	return s;
}

string StringUtils::rtrim(string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c) { return !isspace<char>(c, locale::classic()); });
	s.erase(it.base(), s.end());

	return s;
}

string StringUtils::trim(string s) { return ltrim(rtrim(std::move(s))); }

string StringUtils::ltrimNewLineToo(string s)
{
	auto it = ranges::find_if(s, [](char c) { return !isspace<char>(c, locale::classic()) && c != '\n'; });
	s.erase(s.begin(), it);

	return s;
}

string StringUtils::rtrimNewLineToo(string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c) { return !isspace<char>(c, locale::classic()) && c != '\n'; });
	s.erase(it.base(), s.end());

	return s;
}

string StringUtils::trimNewLineToo(string s) { return ltrimNewLineToo(rtrimNewLineToo(std::move(s))); }

string StringUtils::ltrimTabToo(string s)
{
	auto it = ranges::find_if(s, [](char c) { return !isspace<char>(c, locale::classic()) && c != '\t'; });
	s.erase(s.begin(), it);

	return s;
}

string StringUtils::rtrimTabToo(string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c) { return !isspace<char>(c, locale::classic()) && c != '\t'; });
	s.erase(it.base(), s.end());

	return s;
}

string StringUtils::trimTabToo(string s) { return ltrimTabToo(rtrimTabToo(std::move(s))); }

string StringUtils::ltrimNewLineAndTabToo(string s)
{
	auto it = ranges::find_if(s, [](char c) { return !isspace<char>(c, locale::classic()) && c != '\n' && c != '\t'; });
	s.erase(s.begin(), it);

	return s;
}

string StringUtils::rtrimNewLineAndTabToo(string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c) { return !isspace<char>(c, locale::classic()) && c != '\n' && c != '\t'; });
	s.erase(it.base(), s.end());

	return s;
}

string StringUtils::trimNewLineAndTabToo(string s) { return ltrimNewLineAndTabToo(rtrimNewLineToo(std::move(s))); }

string StringUtils::lowerCase(const string &str)
{
	if (str.empty())
		return str;
	string lowerCase;
	lowerCase.resize(str.size());
	ranges::transform(str, lowerCase.begin(), [](unsigned char c) { return tolower(c); });

	return lowerCase;
}

string StringUtils::upperCase(const string &str)
{
	if (str.empty())
		return str;
	string upperCase;
	upperCase.resize(str.size());
	ranges::transform(str, upperCase.begin(), [](unsigned char c) { return toupper(c); });

	return upperCase;
}

bool StringUtils::isNumber(string text)
{
	return !text.empty() && ranges::find_if(text.begin(), text.end(), [](unsigned char c) { return !isdigit(c); }) == text.end();
}

bool StringUtils::equalCaseInsensitive(const string& s1, const string& s2)
{
	return s1.length() != s2.length() ? false : equal(s1.begin(), s1.end(), s2.begin(), [](int c1, int c2) { return toupper(c1) == toupper(c2); });
}

vector<string> StringUtils::split(const string& str, char delimiter) {
	vector<string> result;

	size_t pos = 0, prev = 0;
	while ((pos = str.find(delimiter, prev)) != string::npos) {
		result.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	result.push_back(str.substr(prev));

	return result;
}

string StringUtils::replaceAll(string s, const string& from, const string& to) {
	if (from.empty() || s.empty()) return s;
	size_t pos = 0;
	while ((pos = s.find(from, pos)) != std::string::npos) {
		s.replace(pos, from.size(), to);
		pos += to.size();
	}
	return s;
}

int StringUtils::kmpSearch(string pat, string txt)
{
	int M = pat.length();
	int N = txt.length();

// Create lps[] that will hold the longest
// prefix suffix values for pattern
#ifdef _WIN32
	int *lps = new int[M];
#else
	int lps[M];
#endif
	int j = 0; // index for pat[]

	// Preprocess the pattern (calculate lps[]
	// array)
	computeLPSArray(pat, M, lps);

	int i = 0; // index for txt[]
	int res = 0;
	int next_i = 0;

	while (i < N)
	{
		if (pat[j] == txt[i])
		{
			j++;
			i++;
		}
		if (j == M)
		{

			// When we find pattern first time,
			// we iterate again to check if there
			// exists more pattern
			j = lps[j - 1];
			res++;
		}

		// Mismatch after j matches
		else if (i < N && pat[j] != txt[i])
		{

			// Do not match lps[0..lps[j-1]]
			// characters, they will match anyway
			if (j != 0)
				j = lps[j - 1];
			else
				i = i + 1;
		}
	}
#ifdef _WIN32
	delete[] lps;
#endif
	return res;
}

void StringUtils::computeLPSArray(string pat, int M, int lps[])
{

	// Length of the previous longest
	// prefix suffix
	int len = 0;
	int i = 1;
	lps[0] = 0; // lps[0] is always 0

	// The loop calculates lps[i] for
	// i = 1 to M-1
	while (i < M)
	{
		if (pat[i] == pat[len])
		{
			len++;
			lps[i] = len;
			i++;
		}
		else // (pat[i] != pat[len])
		{

			// This is tricky. Consider the example.
			// AAACAAAA and i = 7. The idea is similar
			// to search step.
			if (len != 0)
			{
				len = lps[len - 1];

				// Also, note that we do not
				// increment i here
			}
			else // if (len == 0)
			{
				lps[i] = len;
				i++;
			}
		}
	}
}

string StringUtils::lastURIPath(const string& uri)
{
	size_t lastSlashIndex = uri.find_last_of('/');
	if (lastSlashIndex == string::npos)
		return "";
	else
	{
		string lastURIPath = uri.substr(lastSlashIndex + 1);
		size_t startParameterIndex = lastURIPath.find_last_of('?');
		if (startParameterIndex == string::npos)
			return lastURIPath;
		else
			return lastURIPath.substr(0, startParameterIndex);
	}
}

string StringUtils::uriPathPrefix(string uri, bool errorIfMissing)
{
	size_t lastSlashIndex = uri.find_last_of('/');
	if (lastSlashIndex == string::npos)
	{
		if (errorIfMissing)
			throw runtime_error(std::format(
				"No uriPathPrefix found"
				", uri: {}",
				uri
			));
		else
			return "";
	}
	return uri.substr(0, lastSlashIndex);
}

#ifdef UTFCPP
string StringUtils::u32ToUtf8(const u32string &in)
{
	// tenta di costruire una std::string da caratteri char32_t (Unicode code points a 32 bit), ma std::string è fatta per char (8 bit).
	// Questo codice non effettua una conversione UTF-32 → UTF-8, ma semplicemente copia i byte più bassi di ogni char32_t, con risultati non validi
	// return string(in.begin(), in.end());

	// Attualmente lo standard non prevede questa conversione, bisogna quindi usare una libreria esterna come
	// utfcpp semplice e leggera
	string out;
	utf8::utf32to8(in.begin(), in.end(), std::back_inserter(out));
	return out;
	/*
	wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(in);
	*/
}

u32string StringUtils::utf8ToU32(const string &in)
{
	// sbagliato, vedi commento sopra
	// return u32string(in.begin(), in.end());

	std::u32string out;
	utf8::utf8to32(in.begin(), in.end(), std::back_inserter(out));
	return out;
	/*
	wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(in);
	*/
}

string StringUtils::u16ToUtf8(const u16string &in)
{
	// sbagliato, vedi commento sopra
	// return string(in.begin(), in.end());

	std::string out;
	utf8::utf16to8(in.begin(), in.end(), std::back_inserter(out));
	return out;
	/*
	wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(in);
	*/
}

u16string StringUtils::utf8ToU16(const string &in)
{
	// sbagliato, vedi commento sopra
	return {in.begin(), in.end()};

	/*
	std::u16string out;
	utf8::utf8to16(in.begin(), in.end(), std::back_inserter(out));
	return out;
	*/
	/*
	wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
	return conv.from_bytes(in);
	*/
}
#endif
