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

#include <string>
#include <format>
#include <vector>

using namespace std;

/*
switch (hash_case(contentType))
{
	case "audio"_case:
	...
}
*/
// hash_djb2a
inline constexpr auto hash_case(const string_view sv)
{
	unsigned long hash{5381};
	for (const unsigned char c : sv)
	{
		hash = ((hash << 5) + hash) ^ c;
	}
	return hash;
}
inline constexpr auto operator""_case(const char *str, size_t len) { return hash_case(string_view{str, len}); }

class StringUtils
{

public:
	static string ltrim(string s);
	static string rtrim(string s);
	static string trim(string s);

	static string ltrimNewLineToo(string s);
	static string rtrimNewLineToo(string s);
	static string trimNewLineToo(string s);

	static string ltrimTabToo(string s);
	static string rtrimTabToo(string s);
	static string trimTabToo(string s);

	static string ltrimNewLineAndTabToo(string s);
	static string rtrimNewLineAndTabToo(string s);
	static string trimNewLineAndTabToo(string s);

	static string lowerCase(const string &str);
	static string upperCase(const string &str);

	static bool isNumber(string text);

	static bool equalCaseInsensitive(const string &s1, const string &s2);

	// conta quante volte abbiamo pat in txt usando l'algoritmo KMP
	// Time Complexity: O(M + N)
	// Auxiliary Space: O(M) As an array of size M is used to store the longest
	// prefix suffix values for the pattern.
	static int kmpSearch(string pat, string txt);

	static string lastURIPath(const string &uri);
	static string uriPathPrefix(string uri, bool errorIfMissing = false);

	static vector<string> split(const string& str, char delimiter);
	static string replaceAll(string s, const string &from, const string &to);

	template <typename T>
	static T getValue(const string &s)
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
			throw runtime_error(std::format("Type unknonwn: {}", typeid(T).name()));
		}
	}

#ifdef UTFCPP
	static string u16ToUtf8(const u16string &in);
	static u16string utf8ToU16(const string &in);

	static string u32ToUtf8(const u32string &in);
	static u32string utf8ToU32(const string &in);
#endif

  private:
	// usato da kmpSearch
	static void computeLPSArray(string pat, int M, int lps[]);
};
