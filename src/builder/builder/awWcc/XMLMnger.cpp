#include "XMLMnger.h"

CXMLMnger::CXMLMnger()
{

}
CXMLMnger::~CXMLMnger()
{
	DeleteNode();
}

int CXMLMnger::LoadFile(LPTSTR lpszXMLFilePath)
{
	CXMLFile xmlFile;
	if (xmlFile.LoadFromFile(lpszXMLFilePath))
	{
		CXMLElement *pRoot, *pNext;
		pRoot = xmlFile.GetRoot();
		pNext = pRoot->GetNextChild();
		if (!pNext) return -1;
		pNext = pRoot->GetNextChild();
		DeleteNode();
		SetNode(pNext, m_aryElement);
		SetAxisNode(m_aryGlobal, GLOBAL_DIVISION);
		SetAxisNode(m_aryForm, FORM_DIVISION);
		SetAxisNode(m_aryResource, RESOURCE_DIVISION);
		SetAxisNode(m_aryTRlayout, TRLAYOUT_DIVISION);
		SetAxisNode(m_aryLayoutEdit, LAYOUTEDIT_DIVISION);
		SetAxisNode(m_aryRTS, RTS_DIVISION);
		//SetCellNode(m_aryCell, COLINFO, int& idx);
		return 0;
	}
	else
	{
		return -1;
	}
}

void CXMLMnger::DeleteNode()
{
	ElementNode* pElementNode = NULL;
	for (int ii = m_aryElement.GetCount()-1; ii >= 0; ii--)
	{
		pElementNode = m_aryElement.GetAt(ii);
		delete pElementNode;
	}
	m_aryElement.RemoveAll();
}

void CXMLMnger::SetNode(CXMLElement *pNode, CNode &_aryElement)
{
	CXMLElement* pChild = pNode->GetFirstChild();
	while (pChild)
	{
		int Type = pChild->GetElementType();
		switch (Type)
		{
		case XET_INVALID:
			break;
		case XET_TAG:
		{
			ElementNode* pElementNode = new ElementNode;
			pElementNode->strElement = pChild->GetElementName();
			_aryElement.Add(pElementNode);
		}
			break;
		case XET_ATTRIBUTE:
			break;
		case XET_TEXT:
			_aryElement.GetAt(_aryElement.GetCount() - 1)->strValue = pChild->GetElementName();
			break;
		default:
			break;
		}
		SetNode(pChild, _aryElement);
		pChild = pNode->GetNextChild();
	}
}

int CXMLMnger::GetCellNode(CNode &aryElement, int* idx)
{
	ElementNode* pNode;
	aryElement.RemoveAll();
	int cnt = 0;
	bool bCol = false;
	for (int ii = *idx; ii < m_aryElement.GetCount(); ii++)
	{
		if (m_aryElement.GetAt(ii)->strElement == COLINFO)
		{
			if (!bCol) bCol = true;
			cnt++;
			pNode = m_aryElement.GetAt(ii);
			aryElement.Add(pNode);
			ii++;
			for (; ii < m_aryElement.GetCount(); ii++)
			{
				pNode = m_aryElement.GetAt(ii);
				if (!pNode->strValue.IsEmpty())
				{
					aryElement.Add(pNode);
				}
				else
				{
					ii--;
					break;
				}
			}
		}
		else
		{
			pNode = m_aryElement.GetAt(ii);
			if (pNode->strValue.IsEmpty() && pNode->strElement != COLINFO && bCol)
			{
				*idx = ii;
				break;
			}
		}
	}
	return cnt;
}

int CXMLMnger::GetFormNode(CNode &aryElement, int* idx)
{
	CMapStringToPtr mapFormID;
	ElementNode* pFormElement;

	mapFormID.SetAt(LABEL, (void*)NULL);
	mapFormID.SetAt(BOX, (void*)NULL);
	mapFormID.SetAt(GROUP, (void*)NULL);
	mapFormID.SetAt(PANEL, (void*)NULL);
	mapFormID.SetAt(BUTTON, (void*)NULL);
	mapFormID.SetAt(EDIT, (void*)NULL);
	mapFormID.SetAt(COMBO, (void*)NULL);
	mapFormID.SetAt(OUTPUT, (void*)NULL);
	mapFormID.SetAt(MEMO, (void*)NULL);
	mapFormID.SetAt(GRID, (void*)NULL);
	mapFormID.SetAt(GRIDEX, (void*)NULL);
	mapFormID.SetAt(TABLE, (void*)NULL);
	mapFormID.SetAt(TREEVIEW, (void*)NULL);
	mapFormID.SetAt(OBJECT, (void*)NULL);
	mapFormID.SetAt(USRTAB, (void*)NULL);
	mapFormID.SetAt(BROWSER, (void*)NULL);
	mapFormID.SetAt(CONTROL, (void*)NULL);

	aryElement.RemoveAll();
	int index = -1;
	void*	rtValue;
	for (int ii = *idx; ii < m_aryElement.GetCount(); ii++)
	{
		pFormElement = m_aryElement.GetAt(ii);
		if (mapFormID.Lookup(pFormElement->strElement, rtValue))
		{
			index = ii+1;
			aryElement.Add(pFormElement);
			break;
		}
	}

	if (index == -1)
		return 0;
	int ii;
	for (ii = index; ii < m_aryElement.GetCount(); ii++)
	{
		pFormElement = m_aryElement.GetAt(ii);
		if (pFormElement->strValue != "")
		{
			aryElement.Add(pFormElement);
		}
		else
		{	
			break;
		}
	}
	*idx = ii;
	return 1;
}

void CXMLMnger::SetAxisNode(CNode &aryElement, CString strElementName)
{
	int index = -1;
	TRACE("%d\n",m_aryElement.GetCount());
	for (int ii = 0; ii < m_aryElement.GetCount(); ii++)
	{
		if (m_aryElement.GetAt(ii)->strElement == strElementName)
		{
			index = ii+1;
			break;
		}
	}

	if (index == -1)
		return;

	ElementNode* pGlobalElement;
	CString tmp("");
	for (int ii = index; ii < m_aryElement.GetCount(); ii++)
	{
		pGlobalElement = m_aryElement.GetAt(ii);
		tmp = pGlobalElement->strValue;


		if (pGlobalElement->strElement == GLOBAL_DIVISION ||
			pGlobalElement->strElement == FORM_DIVISION ||
			pGlobalElement->strElement == RESOURCE_DIVISION ||
			pGlobalElement->strElement == TRLAYOUT_DIVISION ||
			pGlobalElement->strElement == RTS_DIVISION ||
			pGlobalElement->strElement == LAYOUTEDIT_DIVISION)
			break;

		tmp.Replace("\n", "");
		tmp.Replace("\t", "");
		if (tmp != "")
		{
			aryElement.Add(pGlobalElement);
		}
	}
}