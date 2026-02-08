#include "stdafx.h"
#include "CURLEnc.h"

CURLEnc::CURLEnc()
{

}

CURLEnc::~CURLEnc()
{

}

std::string CURLEnc::URLEncodeUTF8(const std::string& strUTF8) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : strUTF8) {
		// 안전한 문자는 그대로 유지, 특수문자는 URL 인코딩
		if (c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		}
		else if (c == ' ') {
			escaped << '+';
		}
		else {
			escaped << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(c));
		}
	}

	return escaped.str();
}

std::string CURLEnc::URLEncodeUniqueUTF8(const std::string& strUTF8) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : strUTF8) {
		// 안전한 문자는 그대로 유지, 특수문자는 URL 인코딩
	//	if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
		if (c == ':' || c == '/' || c == '?' || c == '&' || c == '=') {
			escaped << '%' << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(c));
		}
		else
			escaped << c;
	}

	return escaped.str();
}

std::string CURLEnc::ToUpperCase(const std::string& input) {
	std::string result = input;

	for (char& c : result) {
		c = std::toupper(c);
	}

	return result;
}

#define	HANGULMSK	0x80
CString CURLEnc::ExtractHangulCharacters(const CString& input) {
	CString result;
	CString stmp;

	int len = input.GetLength();
	int i = 0;
	WORD wHangul;
	while (i < len) {
		wchar_t ch = input[i];

		char cName;
		cName = input.GetAt(i);
		if (cName & HANGULMSK)
		{
			stmp = input.Mid(i, 2);
			result += stmp;
			i++;
		}
		else if (cName == '(' || cName == ')')
		{
			stmp = input.Mid(i, 1);
			result += stmp;
		}

		i++;
	}
	return result;
}