// mdebug.cpp 
//

#include "stdafx.h"
#include <stdarg.h>

#include "mdebug.h"

/**
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
**/

namespace mdebug 
{


const char* endl = "<\r\n>";
	
CDebug::CDebug(const char * prefix)
: m_str(_T("")), m_autoSpace(true)
{
	m_prefix = prefix;
	if (!m_prefix.IsEmpty())
		m_prefix += ": ";
}

CDebug::~CDebug()
{

}

const int MAX_OUTPUTBUFFER = 4 * 1024;
const int MAX_LEN = 512;

void CDebug::Out(const char* format, ...)
{
	char temp[MAX_OUTPUTBUFFER]{};

	va_list argList;
	va_start(argList, format);
	vsprintf(temp, format, argList);
	va_end(argList);

	if (lstrlen(temp) >= MAX_LEN)
		temp[MAX_LEN - 1] = 0x00;

#ifdef _DEBUG
	TRACE("%s\n", temp);
#else
	OutputDebugString(temp);
#endif
}

const char* CDebug::Get() const
{
	return (const char* )m_str;
}

CDebug& CDebug::operator= (const char* str) 
{
	m_str = str;
	if (m_str.GetLength() >= MAX_LEN)
		m_str.Delete(MAX_LEN, m_str.GetLength() - MAX_LEN + 1);

	m_str.Insert(0, m_prefix);
#ifdef _DEBUG
	TRACE("%s\n", m_str);
#else
	OutputDebugString(m_str);	
#endif

	return *this;
}

CDebug& CDebug::operator << (const char* str)
{
	if (strcmp(str, endl) == 0) {
		*this = m_str;
		m_str.Empty();
	}
	else {
		if (!m_str.IsEmpty() && m_autoSpace)
			m_str += ' ';
		m_str += str;
	}
	return *this;
}

CDebug& CDebug::operator << (const int value)
{
	char buffer[11];
	sprintf(buffer, "%d", value);
	return *this << buffer;
}

CDebug& CDebug::operator << (const double value)
{
	char buffer[MAX_PATH];

	sprintf(buffer, "%f", value);
	return *this << buffer;
}

CDebug& CDebug::operator << (const char value)
{
	char buffer[2];

	sprintf(buffer, "%c", value);
	return *this << buffer;
}

CDebug& CDebug::operator << (const unsigned char value)
{
	char buffer[5];

	sprintf(buffer, "0x%02X", value);
	return *this << buffer;
}

CDebug& CDebug::operator << (const RECT* rc)
{
	char buffer[128];

	sprintf(buffer, "(%d:%d:%d:%d)", rc->left, rc->top, rc->right, rc->bottom);

	return *this << buffer;
}

CDebug& CDebug::operator << (const POINT* pt)
{
	char buffer[128];

	sprintf(buffer, "(%d:%d)", pt->x, pt->y);

	return *this << buffer;
}

CDebug& CDebug::operator << (const ETCType& value)
{
	return *this << value.str;
}

void CDebug::SetAutoSpace(bool newValue)
{
	m_autoSpace = newValue;
}

/************************************************************************/
/* Decimal, Today, Now definition                                       */
/************************************************************************/

Decimal::Decimal(int value, int len, bool fillZero)
{
	char format[10]{};

	sprintf(format, "%%%s%dd", fillZero ? "0": "", len);

	str.Format(format, value);
}

Decimal::Decimal(double value, int len, int decimalLen, bool fillZero)
{
	char format[10]{};

	sprintf(format, "%%%s%d.%df", fillZero ? "0": "", len, decimalLen);

	str.Format(format, value);
}

Today::Today(const char* dateFormat)
{
	str = CTime::GetCurrentTime().Format(dateFormat);
}

Now::Now(const char* timeFormat)
{
	str = CTime::GetCurrentTime().Format(timeFormat);
}

FormatString::FormatString(const char *format, ...)
{
	char temp[MAX_OUTPUTBUFFER]{};
	va_list argList;
	va_start(argList, format);
	vsprintf(temp, format, argList);
	va_end(argList);

	if (lstrlen(temp) >= MAX_LEN)
		temp[MAX_LEN - 1] = 0x00;

	str = temp;
}


template <typename T>
void AddHexa(CString& str, CString& s, T value, BOOL insertSpace)
{
	s.Format("%02X", value);
	if (insertSpace)
		str += ' ';
	str += s;
}

Hexa::Hexa(const char* value)
{
	CString s;
	for (int i = 0; i < lstrlen(value); i++) {
		AddHexa<char>(str, s, value[i], i);
	}
}

Hexa::Hexa(const unsigned char* value, int len)
{
	CString s;
	for (int i = 0; i < len; i++) {
		AddHexa<unsigned char>(str, s, value[i], i);
	}
}




} // namespace mdebug