#pragma once

#include "../../h/mapform.h"
#include "libWcc.h"
#include "msxml.h"

class xmlList
{
public:
	CString elm;
	CString value;
	xmlList* next;
	xmlList* sub;
public:
	xmlList()
	{
		next = NULL;
		sub = NULL;
	}
};

class CXMLparser
{
public:
	CXMLparser();
	virtual ~CXMLparser();

	void parserRun(CString path);
public:
	//CArray<struct xmlList*, struct xmlList*> m_aryXml;
	xmlList* m_pXmlList;
private:
	HRESULT CheckLoad(IXMLDOMDocument *pDoc);
	HRESULT ReportError(IXMLDOMParseError *pXMLError);
	bool IterateChildNodes(IXMLDOMNode *pNode, xmlList* pXmlList);
	char* convertUtf8ToAnsi(char* buff);
};


