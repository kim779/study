#pragma once

#define	XML_HEADING	"<?xml version=\"1.0\" encoding=\"euc-kr\"?>"
#define	NEWLINE		"\r\n"

class CXMLWriter
{
public:
	CXMLWriter(void);
	~CXMLWriter(void);

	void WriteString(CFile &cf, const char *str);
	void WriteHeader(CFile &cf, int nTab, BOOL bNewLine, char *node, char *attr, const char *val, ...);
	void WriteHeaderEnd(CFile &cf);
	void WriteNodeStart(CFile &cf, int nTab, char *node);
	void WriteNodeEnd(CFile &cf, int nTab, char *node, BOOL bNewLine = FALSE);
	void WriteAttrHeader(CFile &cf, int nTab, char *head, char *attr, const char *val, BOOL bNewLine = FALSE);
	void WriteAttrStr(CFile &cf, char *attr, const char *val, int nTab = 0);
	void WriteNodeOnlyAttribute(CFile &cf, int nTab, char *node, char *attr, const char *val, ...);

protected:
	CString TabStr(int n);
};
