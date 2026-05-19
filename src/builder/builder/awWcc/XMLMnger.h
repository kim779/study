#pragma once
#include "StdAfx.h"
#include "XMLFile.h"
#include "../h/mapxml.H"


class CXMLMnger
{
public:
	CXMLMnger();
	~CXMLMnger();
public:
	CNode m_aryElement;
	CNode m_aryGlobal;
	CNode m_aryForm;
	CNode m_aryResource;
	CNode m_aryTRlayout;
	CNode m_aryLayoutEdit;
	CNode m_aryRTS;
	CNode m_aryCell;

	int GetCellNode(CNode &aryElement, int* idx);
	int GetFormNode(CNode &aryElement, int* idx);

public:
	int LoadFile(LPTSTR lpszXMLFilePath);
private:
	void SetNode(CXMLElement *pNode, CNode &_aryElement);
	void SetAxisNode(CNode &aryElement, CString strElementName);
	void DeleteNode();
};