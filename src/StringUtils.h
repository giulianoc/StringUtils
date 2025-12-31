/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   StringUtils.h
 * Author: giuliano
 *
 * Created on March 29, 2018, 6:27 AM
 */

#pragma once

#include <format>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

/*
switch (hash_case(contentType))
{
	case "audio"_case:
	{
		// do something
		break;
	}
	default:
	{
		// do something
		break;
	}
}
*/
// hash_djb2a
inline constexpr auto hash_case(const std::string_view sv)
{
	unsigned long hash{5381};
	for (const unsigned char c : sv)
	{
		hash = ((hash << 5) + hash) ^ c;
	}
	return hash;
}
inline constexpr auto operator""_case(const char *str, size_t len) { return hash_case(std::string_view{str, len}); }

class StringUtils
{

public:
	static std::string ltrim(std::string s);
	static std::string rtrim(std::string s);
	static std::string trim(std::string s);

	static std::string_view trim(std::string_view sv);
	static std::optional<int64_t> toInt64(std::string_view sv, int base = 10);

	static std::string ltrimNewLineToo(std::string s);
	static std::string rtrimNewLineToo(std::string s);
	static std::string trimNewLineToo(std::string s);

	static std::string ltrimTabToo(std::string s);
	static std::string rtrimTabToo(std::string s);
	static std::string trimTabToo(std::string s);

	static std::string ltrimNewLineAndTabToo(std::string s);
	static std::string rtrimNewLineAndTabToo(std::string s);
	static std::string trimNewLineAndTabToo(std::string s);

	static std::string lowerCase(const std::string_view& str);
	static std::string upperCase(const std::string_view& str);

	static bool isNumber(std::string text);

	static bool equalCaseInsensitive(const std::string &s1, const std::string &s2);

	// conta quante volte abbiamo pat in txt usando l'algoritmo KMP
	// Time Complexity: O(M + N)
	// Auxiliary Space: O(M) As an array of size M is used to store the longest
	// prefix suffix values for the pattern.
	static int kmpSearch(std::string pat, std::string txt);

	static std::string lastURIPath(const std::string &uri);
	static std::string uriPathPrefix(std::string uri, bool errorIfMissing = false);

	static std::vector<std::string> split(const std::string& str, char delimiter);
	static std::string replaceAll(std::string_view source, const std::string_view from, const std::string_view to);

	template <typename T>
	static T getValue(const std::string &s)
	{
		if constexpr (std::is_same_v<T, std::string>)
			return s;
		else if constexpr (std::is_same_v<T, bool>)
			return lowerCase(s) == "true";
		else if constexpr (std::is_same_v<T, int32_t>)
			return stol(s);
		else if constexpr (std::is_same_v<T, int64_t>)
			return stoll(s);
		else
		{
			const std::string errorMessage = std::format("Type unknown: {}", typeid(T).name());
			SPDLOG_ERROR(errorMessage);
			throw std::runtime_error(errorMessage);
		}
	}

#ifdef UTFCPP
	static std::string u16ToUtf8(const std::u16string &in);
	static std::u16string utf8ToU16(const std::string &in);

	static std::string u32ToUtf8(const std::u32string &in);
	static std::u32string utf8ToU32(const std::string &in);
#endif

  private:
	// usato da kmpSearch
	static void computeLPSArray(std::string pat, int M, int lps[]);
};
