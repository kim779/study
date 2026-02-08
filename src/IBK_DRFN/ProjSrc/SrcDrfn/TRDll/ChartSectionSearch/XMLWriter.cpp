#include "stdafx.h"
#include "XMLWriter.h"

CXMLWriter::CXMLWriter(void)
{
}

CXMLWriter::~CXMLWriter(void)
{
}

void CXMLWriter::WriteString(CFile &cf, const char *str)
{
	CString	buf;
	buf.Format("%s\r\n", str);
	cf.Write(buf, buf.GetLength());
}

void CXMLWriter::WriteHeader(CFile &cf, int nTab, BOOL bNewLine, char *node, char *attr, const char *val, ...)
{
	CString tab = TabStr(nTab);
	CString str;
	CStringArray	arrAttr, arrValue;

	arrAttr.Add(attr);
	arrValue.Add(val);

	va_list arg;
	va_start( arg, val );

	while(1)
	{
		str = va_arg( arg, char*);
		if(str.GetLength() == 0) break;
		arrAttr.Add(str);
		str = va_arg( arg, char*);
		arrValue.Add(str);
	}
	va_end( arg );  

	CString	buf, tmp;
	buf.Format("%s<%s", tab, node);
	for(int i = 0; i < arrAttr.GetSize(); i++)
	{
		tmp.Format(" %s=\'%s\'", arrAttr.GetAt(i), arrValue.GetAt(i));
		buf = buf + tmp;
	}

	if(bNewLine == TRUE)
		buf = buf + ">\r\n";
	else
		buf = buf + ">";

	cf.Write(buf, buf.GetLength());
}

void CXMLWriter::WriteHeaderEnd(CFile &cf)
{
	CString	buf;
	buf.Format(">\r\n");
	cf.Write(buf, buf.GetLength());
}

void CXMLWriter::WriteNodeStart(CFile &cf, int nTab, char *node)
{
	CString tab = TabStr(nTab);
	CString	buf;
	buf.Format("%s<%s>\r\n", tab, node);
	cf.Write(buf, buf.GetLength());
}

void CXMLWriter::WriteNodeEnd(CFile &cf, int nTab, char *node, BOOL bNewLine)
{
	CString tab = TabStr(nTab);

	CString	newLine="";
	if(bNewLine)
		newLine = NEWLINE;

	CString	buf;
	buf.Format("%s</%s>\r\n%s", tab, node, newLine);
	cf.Write(buf, buf.GetLength());
}

void CXMLWriter::WriteAttrHeader(CFile &cf, int nTab, char *head, char *attr, const char *val, BOOL bNewLine)
{
	CString tab = TabStr(nTab);

	CString newLine = "";
	if(bNewLine)
		newLine = NEWLINE;

	CString	buf;
	buf.Format("%s<%s %s=\'%s\'>%s", tab, head, attr, val, newLine);
	cf.Write(buf, buf.GetLength());
}

void CXMLWriter::WriteAttrStr(CFile &cf, char *attr, const char *val, int nTab)
{
	CString tab = TabStr(nTab);

	CString	buf;
	buf.Format("%s<%s>%s</%s>\r\n", tab, attr, val, attr);
	cf.Write(buf, buf.GetLength());
}

CString CXMLWriter::TabStr(int n)
{
	CString tab = "";
	for(int i = 0; i < n; i++)
		tab += "\t";

	return tab;
}

void CXMLWriter::WriteNodeOnlyAttribute(CFile &cf, int nTab, char *node, char *attr, const char *val, ...)
{
	CString tab = TabStr(nTab);
	CString str;
	CStringArray	arrAttr, arrValue;

	arrAttr.Add(attr);
	arrValue.Add(val);

	va_list arg;
	va_start( arg, val );

	while(1)
	{
		str = va_arg( arg, char*);
		if(str.GetLength() == 0) break;
		arrAttr.Add(str);
		str = va_arg( arg, char*);
		arrValue.Add(str);
	}
	va_end( arg );  

	CString	buf, tmp;
	buf.Format("%s<%s", tab, node);
	for(int i = 0; i < arrAttr.GetSize(); i++)
	{
		tmp.Format(" %s=\'%s\'", arrAttr.GetAt(i), arrValue.GetAt(i));
		buf = buf + tmp;
	}

	buf = buf + "/>\r\n";
	cf.Write(buf, buf.GetLength());
}