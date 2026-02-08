#pragma once
#include "StdAfx.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#define	HANGULMSK	0x80
class CURLEnc
{
public:
	CURLEnc();
	virtual ~CURLEnc();

	std::string URLEncodeUTF8(const std::string& strUTF8);
	std::string URLEncodeUniqueUTF8(const std::string& strUTF8);
	std::string ToUpperCase(const std::string& input);
	CString ExtractHangulCharacters(const CString& input);
};

