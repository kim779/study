// FormItem1.cpp: implementation of the CFormItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FormItem.h"

#include <afxtempl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormItem::CFormItem(struct _mapH *mapH)
{
	m_mapH = mapH;
	m_field = "00name\t01fName\t02help\t03auxS\t04scpN\t05editS\t06refS\t07image\t08str\t09str2\t10dat\t";
}

CFormItem::~CFormItem()
{

}

int CFormItem::getFormCount()
{
	struct _formR *tmp = m_mapH->formR;
	if (tmp == NULL)
		return 0;
	int cnt = 1;
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
		cnt++;
	}
	return cnt;
}

struct _formR* CFormItem::getFormPoint(int idx)
{
	if (idx < 0 || idx >= getFormCount())
		return (_formR*)NULL;

	struct _formR *tmp;
	tmp = m_mapH->formR;
	for (int ii = 0; ii < idx; ii++)
		tmp = tmp->next;
	return tmp;
}

void CFormItem::removeLastForm(int lastIdx)
{
	int cnt = getFormCount();
	if (cnt > lastIdx)
		for (int ii = lastIdx; ii < cnt; ii++)
			removeForm(lastIdx);
}

void CFormItem::removeForm(int idx)
{
	struct _formR *tmp = m_mapH->formR, *tmp2;
	if (idx == 0)
	{
		tmp2 = tmp->next;
		m_mapH->formR = tmp2;
		FreeCell(tmp);
		free(tmp);
		return;
	}
	tmp = getFormPoint(idx - 1);
	tmp2 = tmp->next;
	tmp->next = tmp2->next;
	FreeCell(tmp2);
	free(tmp2);
}

int CFormItem::insertForm(int idx, _formR *formR)
{
	struct _formR *tmp = m_mapH->formR, *tmp2;
	if (formR == NULL)
	{
		tmp2 = (struct _formR *)malloc(sizeof(_formR));
		ZeroMemory(tmp2, sizeof(_formR));
	}
	else
		tmp2 = formR;

	if (idx >= 0)
	{
		tmp = getFormPoint(idx - 1);
		tmp2->next = tmp->next;
		tmp->next = tmp2;
		return idx;
	}
	else
	{
		tmp2->next = m_mapH->formR->next;
		m_mapH->formR = tmp2;
		return 0;
	}
	return -1;
}

int CFormItem::addForm(_formR *formR)
{
	struct _formR *tmp = m_mapH->formR, *tmp2;
	int cnt = getFormCount();

	if (formR == NULL)
	{
		tmp2 = (struct _formR *)malloc(sizeof(_formR));
		ZeroMemory(tmp2, sizeof(_formR));
	}
	else
		tmp2 = formR;

	if (cnt == 0)
	{
		tmp = tmp2;
		tmp->next = NULL;
		m_mapH->formR = tmp;
	}
	else
	{
		tmp = getFormPoint(cnt - 1);
		tmp->next = tmp2;
		tmp2->next = NULL;
	}

	return cnt;
}

void CFormItem::RemoveAllForm()
{
	removeLastForm(0);
}

void CFormItem::FreeCell(_formR *formR)
{
	if (formR->scpN)
		DeleteFile(formR->scpN);
	if (formR->auxR)
	{
		struct _repR *repR   = (struct _repR *)formR->auxR;
		CCellItem *cellItem = new CCellItem(repR);
		cellItem->RemoveAllCell();
		delete cellItem;
		delete[] formR->auxR;
	}
	if (formR->name)
		delete []formR->name;
	if (formR->fName)
		delete []formR->fName;
	if (formR->help)
		delete []formR->help;
	if (formR->auxS)
		delete []formR->auxS;
	if (formR->scpN)
		delete []formR->scpN;
	if (formR->editS)
		delete []formR->editS;
	if (formR->refS)
		delete []formR->refS;
	if (formR->image)
		delete []formR->image;
	if (formR->str)
		delete []formR->str;
	if (formR->str2)
		delete []formR->str2;
	if (formR->dat)
		delete []formR->dat;
}

void CFormItem::moveForm(int from, int to)
{
	int allCnt = getFormCount();
	if (from >= allCnt || to > allCnt)
		return;
	struct _formR *fromR, *toR;
	fromR = getFormPoint(from);
	
	if (to == 0)
		toR = NULL;
	else
		toR = getFormPoint(to - 1);
	
	if (from == 0)
		m_mapH->formR = fromR->next;
	else
		getFormPoint(from - 1)->next = fromR->next;

	if (toR == NULL)
	{
		fromR->next = m_mapH->formR;
		m_mapH->formR = fromR;
	}
	else
	{
		fromR->next = toR->next;
		toR->next = fromR;
	}
}

char* CFormItem::SetStr(char *map, CString data)
{
	if (map)
		delete[] map;
	map = new char[data.GetLength() + 1];
	strcpy_s(map, data.GetLength() + 1, (LPCTSTR)data);
	return map;
}

CString CFormItem::GetStr(char *map)
{
	if (map == NULL || strlen(map) == 0)
		return "";
	return CString(map, strlen(map));
}

CString CFormItem::getFormStr(int idx, CString field)
{
	CString fnum = _T(""), rtVal = _T("");
	int pos = m_field.Find(field);
	if (pos == -1 || !getFormPoint(idx))
		return "";
		
	while(--pos >= 0 && m_field.GetAt(pos) != '\t')
		fnum = m_field.GetAt(pos) + fnum;

	switch(atoi(fnum))
	{
	case 0:		// name
		rtVal = GetStr(getFormPoint(idx)->name);
		break;
	case 1:		// fName
		rtVal = GetStr(getFormPoint(idx)->fName);
		break;
	case 2:		// help
		rtVal = GetStr(getFormPoint(idx)->help);
		break;
	case 3:		// auxS
		rtVal = GetStr(getFormPoint(idx)->auxS);
		break;
	case 4:		// scpN
		rtVal = GetStr(getFormPoint(idx)->scpN);
		break;
	case 5:		// editS
		rtVal = GetStr(getFormPoint(idx)->editS);
		break;
	case 6:		// refS
		rtVal = GetStr(getFormPoint(idx)->refS);
		break;
	case 7:		// image
		rtVal = GetStr(getFormPoint(idx)->image);
		break;
	case 8:		// str
		rtVal = GetStr(getFormPoint(idx)->str);
		break;
	case 9:		// str2
		rtVal = GetStr(getFormPoint(idx)->str2);
		break;
	case 10:	// dat
		rtVal = GetStr(getFormPoint(idx)->dat);
		break;
	}
	return rtVal;
}

void CFormItem::setFormStr(int idx, CString field, CString dat)
{
	CString fnum = _T("");
	int pos = m_field.Find(field);
	if (pos == -1 || !getFormPoint(idx))
		return;
		
	while(--pos >= 0 && m_field.GetAt(pos) != '\t')
		fnum = m_field.GetAt(pos) + fnum;

	switch(atoi(fnum))
	{
	case 0:		// name
		getFormPoint(idx)->name = SetStr(getFormPoint(idx)->name, dat);
		break;
	case 1:		// fName
		getFormPoint(idx)->fName = SetStr(getFormPoint(idx)->fName, dat);
		break;
	case 2:		// help
		getFormPoint(idx)->help = SetStr(getFormPoint(idx)->help, dat);
		break;
	case 3:		// auxS
		getFormPoint(idx)->auxS = SetStr(getFormPoint(idx)->auxS, dat);
		break;
	case 4:		// scpN
		getFormPoint(idx)->scpN = SetStr(getFormPoint(idx)->scpN, dat);
		break;
	case 5:		// editS
		getFormPoint(idx)->editS = SetStr(getFormPoint(idx)->editS, dat);
		break;
	case 6:		// refS
		getFormPoint(idx)->refS = SetStr(getFormPoint(idx)->refS, dat);
		break;
	case 7:		// image
		getFormPoint(idx)->image = SetStr(getFormPoint(idx)->image, dat);
		break;
	case 8:		// str
		getFormPoint(idx)->str = SetStr(getFormPoint(idx)->str, dat);
		break;
	case 9:		// str2
		getFormPoint(idx)->str2 = SetStr(getFormPoint(idx)->str2, dat);
		break;
	case 10:	// dat
		getFormPoint(idx)->dat = SetStr(getFormPoint(idx)->dat, dat);
		break;
	}
}

int CFormItem::getIdxOfPtr(_formR *ptr)
{
	struct _formR *tmp = m_mapH->formR;

	for (int ii = 0; ii < getFormCount(); ii++)
	{
		if (tmp == ptr)
			return ii;
		tmp = tmp->next;
	}
	
	return -1;
}
