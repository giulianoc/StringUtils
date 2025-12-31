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
#include <charconv>
#include <format>
#include <locale>
#include <ranges>
#include <regex>
#include <utility>
#ifdef UTFCPP
	#include <utf8.h>
#endif


std::string StringUtils::ltrim(std::string s)
{
	const auto it = std::ranges::find_if(s, [](const char c)
	{
		return !std::isspace<char>(c, std::locale::classic());
	});
	s.erase(s.begin(), it);

	return s;
}

std::string StringUtils::rtrim(std::string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic());
	});
	s.erase(it.base(), s.end());

	return s;
}

std::string StringUtils::trim(std::string s) { return ltrim(rtrim(std::move(s))); }

std::string StringUtils::ltrimNewLineToo(std::string s)
{
	auto it = std::ranges::find_if(s, [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic()) && c != '\n';
	});
	s.erase(s.begin(), it);

	return s;
}

std::string StringUtils::rtrimNewLineToo(std::string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic()) && c != '\n';
	});
	s.erase(it.base(), s.end());

	return s;
}

std::string StringUtils::trimNewLineToo(std::string s) { return ltrimNewLineToo(rtrimNewLineToo(std::move(s))); }

std::string StringUtils::ltrimTabToo(std::string s)
{
	auto it = std::ranges::find_if(s, [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic()) && c != '\t';
	});
	s.erase(s.begin(), it);

	return s;
}

std::string StringUtils::rtrimTabToo(std::string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic()) && c != '\t';
	});
	s.erase(it.base(), s.end());

	return s;
}

std::string StringUtils::trimTabToo(std::string s) { return ltrimTabToo(rtrimTabToo(std::move(s))); }

std::string StringUtils::ltrimNewLineAndTabToo(std::string s)
{
	auto it = std::ranges::find_if(s, [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic()) && c != '\n' && c != '\t';
	});
	s.erase(s.begin(), it);

	return s;
}

std::string StringUtils::rtrimNewLineAndTabToo(std::string s)
{
	auto it = find_if(s.rbegin(), s.rend(), [](char c)
	{
		return !std::isspace<char>(c, std::locale::classic()) && c != '\n' && c != '\t';
	});
	s.erase(it.base(), s.end());

	return s;
}

std::string StringUtils::trimNewLineAndTabToo(std::string s) { return ltrimNewLineAndTabToo(rtrimNewLineToo(std::move(s))); }

std::string_view StringUtils::trim(const std::string_view sv)
{
	size_t b = 0, e = sv.size();
	while (b < e && std::isspace(static_cast<unsigned char>(sv[b]))) ++b;
	while (e > b && std::isspace(static_cast<unsigned char>(sv[e - 1]))) --e;
	return sv.substr(b, e - b);
}

std::optional<int64_t> StringUtils::toInt64(std::string_view sv, const int base)
{
	sv = trim(sv);
	if (sv.empty())
		return std::nullopt;

	int64_t val = 0;
	auto first = sv.data();
	auto last  = sv.data() + sv.size();
	auto res = std::from_chars(first, last, val, base);

	if (res.ec == std::errc() && res.ptr == last)
		return val;
	return std::nullopt;
}

std::string StringUtils::lowerCase(const std::string_view& str)
{
	if (str.empty())
		return std::string(str);
	std::string lowerCase;
	lowerCase.resize(str.size());
	std::ranges::transform(str, lowerCase.begin(), [](unsigned char c) { return tolower(c); });

	return lowerCase;
}

std::string StringUtils::upperCase(const std::string_view& str)
{
	if (str.empty())
		return std::string(str);
	std::string upperCase;
	upperCase.resize(str.size());
	std::ranges::transform(str, upperCase.begin(), [](unsigned char c) { return toupper(c); });

	return upperCase;
}

bool StringUtils::isNumber(std::string text)
{
	return !text.empty() && std::ranges::find_if(text.begin(), text.end(), [](unsigned char c) { return !isdigit(c); }) == text.end();
}

bool StringUtils::equalCaseInsensitive(const std::string& s1, const std::string& s2)
{
	return s1.length() != s2.length() ? false : equal(s1.begin(), s1.end(), s2.begin(), [](int c1, int c2) { return toupper(c1) == toupper(c2); });
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter)
{
	std::vector<std::string> result;

	size_t pos = 0, prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::string::npos) {
		result.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	result.push_back(str.substr(prev));

	return result;
}

std::string StringUtils::replaceAll(std::string_view source, const std::string_view from, const std::string_view to)
{
	if (from.empty() || source.empty())
		return std::string(source);

	std::string result;
	result.reserve(source.size() + (to.size() > from.size() ? 16 : 0)); // stima semplice

	size_t start = 0;
	while (true) {
		const size_t pos = source.find(from, start);
		if (pos == std::string_view::npos)
			break;

		result.append(source.substr(start, pos - start));
		result.append(to);
		start = pos + from.size();
	}

	result.append(source.substr(start));
	return result;
}
/*
std::string StringUtils::replaceAll(const std::string& source, const std::string& from, const std::string& to) {
	if (from.empty() || source.empty()) return source;
	std::string destination = source;
	size_t pos = 0;
	while ((pos = destination.find(from, pos)) != std::string::npos) {
		destination.replace(pos, from.size(), to);
		pos += to.size();
	}
	return destination;
}
*/

int StringUtils::kmpSearch(std::string pat, std::string txt)
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

void StringUtils::computeLPSArray(std::string pat, int M, int lps[])
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

std::string StringUtils::lastURIPath(const std::string& uri)
{
	size_t lastSlashIndex = uri.find_last_of('/');
	if (lastSlashIndex == std::string::npos)
		return "";
	std::string lastURIPath = uri.substr(lastSlashIndex + 1);
	const size_t startParameterIndex = lastURIPath.find_last_of('?');
	if (startParameterIndex == std::string::npos)
		return lastURIPath;
	return lastURIPath.substr(0, startParameterIndex);
}

std::string StringUtils::uriPathPrefix(std::string uri, bool errorIfMissing)
{
	size_t lastSlashIndex = uri.find_last_of('/');
	if (lastSlashIndex == std::string::npos)
	{
		if (errorIfMissing)
		{
			const std::string errorMessage = std::format(
				"No uriPathPrefix found"
				", uri: {}",
				uri
			);
			SPDLOG_ERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}
		return "";
	}
	return uri.substr(0, lastSlashIndex);
}

#ifdef UTFCPP
std::string StringUtils::u32ToUtf8(const std::u32string &in)
{
	// tenta di costruire una std::string da caratteri char32_t (Unicode code points a 32 bit), ma std::string è fatta per char (8 bit).
	// Questo codice non effettua una conversione UTF-32 → UTF-8, ma semplicemente copia i byte più bassi di ogni char32_t, con risultati non validi
	// return std::string(in.begin(), in.end());

	// Attualmente lo standard non prevede questa conversione, bisogna quindi usare una libreria esterna come
	// utfcpp semplice e leggera
	std::string out;
	utf8::utf32to8(in.begin(), in.end(), std::back_inserter(out));
	return out;
	/*
	wstring_convert<codecvt_utf8<char32_t>, char32_t> conv;
	return conv.to_bytes(in);
	*/
}

std::u32string StringUtils::utf8ToU32(const std::string &in)
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

std::string StringUtils::u16ToUtf8(const std::u16string &in)
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

std::u16string StringUtils::utf8ToU16(const std::string &in)
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
