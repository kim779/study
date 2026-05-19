// FormItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormItem.h"
// updateXX__2014
#include "../undoredo.h"

// CFormItem

CFormItem::CFormItem(struct _mapH* mapH)
{
	m_mapH = mapH;

	m_fields.RemoveAll();
	const CString	ff[] = 
	{
		_T("name"),	// 0
		_T("fName"),	// 1
		_T("desc"),	// 2
		_T("auxS"),	// 3
		_T("scpN"),	// 4
		_T("editS"),	// 5
		_T("str"),	// 6
		_T("str2"),	// 7
		_T("dat"),	// 8
		_T("rts"),	// 9
		_T("optionimg"),// 10
		_T("refs"),	// 11
		_T("help"),	// 12
		_T("optionimg2"),// 13
		_T("optionimg3"),// 14
		_T("")
	};

	for (int ii = 0; ; ii++)
	{
		if (ff[ii].IsEmpty())
			break;

		m_fields.SetAt(ff[ii], (void *)ii);
	}
}

CFormItem::CFormItem(struct _mapH* mapH, CUndoRedo* urdo)
{
	m_mapH = mapH;

	int nLen = 0;
	if (urdo->mx_formN > 0)
	{
		mapH->formR = (_formR *) new char[sizeof(_formR)];
		ZeroMemory(mapH->formR, sizeof(_formR));

		struct _formR* formSrc = (_formR *) urdo->mx_formR;
		struct _formR* formDes = (_formR *) mapH->formR;
		struct _formR* backDes = NULL;

		for (int ii = 0; ii < urdo->mx_formN; ii++)
		{
			if (formDes == NULL)
			{
				formDes = (_formR *) new char[sizeof(_formR)];
				ZeroMemory(formDes, sizeof(_formR));
				backDes->next = formDes;
			}
			CopyMemory(formDes, formSrc, sizeof(_formR));

			if (formSrc->name != NULL)
			{
				nLen = strlen(formSrc->name) + 1;
				formDes->name = new char[nLen];
				strcpy_s(formDes->name, nLen, formSrc->name);

			}
			if (formSrc->fName != NULL)
			{
				nLen = strlen(formSrc->fName) + 1;
				formDes->fName = new char[nLen];
				strcpy_s(formDes->fName, nLen, formSrc->fName);
			}
			if (formSrc->desc != NULL)
			{
				nLen = strlen(formSrc->desc) + 1;
				formDes->desc = new char[nLen];
				strcpy_s(formDes->desc, nLen, formSrc->desc);
			}
			if (formSrc->auxS != NULL)
			{
				nLen = strlen(formSrc->auxS) + 1;
				formDes->auxS = new char[nLen];
				strcpy_s(formDes->auxS, nLen, formSrc->auxS);
			}
			if (formSrc->editS != NULL)
			{
				nLen = strlen(formSrc->editS) + 1;
				formDes->editS = new char[nLen];
				strcpy_s(formDes->editS, nLen, formSrc->editS);
			}
			if (formSrc->str != NULL)
			{
				nLen = strlen(formSrc->str) + 1;
				formDes->str = new char[nLen];
				strcpy_s(formDes->str, nLen, formSrc->str);
			}
			if (formSrc->str2 != NULL)
			{
				nLen = strlen(formSrc->str2) + 1;
				formDes->str2 = new char[nLen];
				strcpy_s(formDes->str2, nLen, formSrc->str2);

			}
			if (formSrc->dat != NULL)
			{
				nLen = strlen(formSrc->dat) + 1;
				formDes->dat = new char[nLen];
				strcpy_s(formDes->dat, nLen, formSrc->dat);
			}
			if (formSrc->rtssy != NULL)
			{
				nLen = strlen(formSrc->rtssy) + 1; 
				formDes->rtssy = new char[nLen];
				strcpy_s(formDes->rtssy, nLen, formSrc->rtssy);
			}
			if (formSrc->optionimg != NULL)
			{
				nLen = strlen(formSrc->optionimg) + 1;
				formDes->optionimg = new char[nLen];
				strcpy_s(formDes->optionimg, nLen, formSrc->optionimg);
			}
			if (formSrc->refS != NULL)
			{
				nLen = strlen(formSrc->refS)+ 1;
				formDes->refS = new char[nLen];
				strcpy_s(formDes->refS, nLen, formSrc->refS);
			}
			if (formSrc->help != NULL)
			{
				nLen = strlen(formSrc->help) + 1;
				formDes->help = new char[nLen];
				strcpy_s(formDes->help, nLen, formSrc->help);
			}
			if (formSrc->optionimg2 != NULL)
			{
				nLen = strlen(formSrc->optionimg2) + 1;
				formDes->optionimg2 = new char[nLen];
				strcpy_s(formDes->optionimg2, nLen, formSrc->optionimg2);
			}
			if (formSrc->optionimg3 != NULL)
			{
				nLen = strlen(formSrc->optionimg3) + 1;
				formDes->optionimg3 = new char[nLen];
				strcpy_s(formDes->optionimg3, nLen, formSrc->optionimg3);
			}
			if (formSrc->scpN != NULL)
			{
				nLen = strlen(formSrc->scpN) + 1;
				formDes->scpN = new char[nLen];
				strcpy_s(formDes->scpN, nLen, formSrc->scpN);
			}
			formDes->auxR = NULL;

			// margin
			if (formSrc->margin && formSrc->margin->GetCount() > 0)
			{
				formDes->margin = new CStringArray;
				formDes->margin->Copy(*formSrc->margin);
			}

			// layout
			formDes->layout = new CMapStringToPtr;
			formDes->layout->RemoveAll();
			if (formSrc->layout->GetCount() > 0)
			{
				CString sKey;
				struct _Layout *layout, *layoutTmp;				
				for (POSITION pos = formSrc->layout->GetStartPosition(); pos;)
				{
					layout = new _Layout;
					formSrc->layout->GetNextAssoc(pos, sKey, (void*&)layoutTmp);

					layout->bfirst	= false;
					layout->properties = layoutTmp->properties;
					layout->left	= layoutTmp->left;
					layout->right	= layoutTmp->right;
					layout->top	= layoutTmp->top;
					layout->bottom	= layoutTmp->bottom;					

					formDes->layout->SetAt(sKey, (void*&)layout);
				}
			}

			switch (formSrc->kind)
			{
			case FM_GRID:
			case FM_GRIDEX: 
			case FM_TABLE:
				if (formSrc->auxR != NULL)
				{
					formDes->auxR = new char[sizeof(_repR)];
					CopyMemory((char*)formDes->auxR, (char*)formSrc->auxR, sizeof(_repR));

					struct _repR* repSrc = (_repR *) formSrc->auxR;
					struct _repR* repDes = (_repR *) formDes->auxR;

					struct _cellR* cellSrc = repSrc->cellR;
					struct _cellR* cellDes = repDes->cellR;
					struct _cellR* cellBak = NULL;

					if (cellSrc == NULL)
						break;

					cellDes = (_cellR*) new char[sizeof(_cellR)];
					repDes->cellR = cellDes;
					while (cellSrc != NULL)
					{
						if (cellDes == NULL)
						{
							cellDes = (_cellR*) new char[sizeof(_cellR)];
							ZeroMemory(cellDes, sizeof(_cellR));
							cellBak->next = cellDes;
						}
						CopyMemory((char*)cellDes, (char*)cellSrc, sizeof(_cellR));
						if (cellSrc->fName)
						{
							nLen = strlen(cellSrc->fName) + 1;
							cellDes->fName = new char[nLen];
							strcpy_s(cellDes->fName, nLen, cellSrc->fName);
						}
						if (cellSrc->margin != NULL && cellSrc->margin->GetCount() > 0)
						{
							cellDes->margin = new CStringArray;
							cellDes->margin->Copy(*cellSrc->margin);
						}
						cellSrc = cellSrc->next;
						cellBak = cellDes;
						cellDes = NULL;
					}
				}
				break;
			default:
				break;;
			}

			formSrc = formSrc->next;
			backDes = formDes;
			formDes = NULL;
		}
	}

	void* value;
	CString	skey;
	for (POSITION pos = urdo->mx_fields.GetStartPosition(); pos != NULL; )
	{
		urdo->mx_fields.GetNextAssoc(pos, skey, value);
		m_fields.SetAt(skey, (void *) value);
	}
}

CFormItem::~CFormItem()
{
}

int CFormItem::AddForm(struct _formR* formR)
{
	struct _formR *tmp = m_mapH->formR, *tmp2;
	int	count = GetCount();

	if (formR == NULL)
	{
		tmp2 = (struct _formR *)malloc(sizeof(_formR));
		ZeroMemory(tmp2, sizeof(_formR));		
	}
	else
		tmp2 = formR;

	// Layout
	if (tmp2->layout == NULL)
	{
		tmp2->layout = new CMapStringToPtr;
		tmp2->layout->RemoveAll();
	}

	if (count == 0)
	{
		tmp = tmp2;
		tmp->next = NULL;
		m_mapH->formR = tmp;
	}
	else
	{
		tmp = GetForm(count - 1);
		tmp->next = tmp2;
		tmp2->next = NULL;
	}

	return count;
}

// Layout 
void CFormItem::SetLayoutInfo(CString sKey, CString sPreKey)
{
	struct _formR *tmp;
	struct _Layout *playout, *pPrelayout;
	for (int ii = 0; ii < GetCount(); ii++)
	{
		tmp = GetForm(ii);
		// Save Infomation
		if (tmp->layout->Lookup(sPreKey, (void*&)pPrelayout))
		{
			pPrelayout->left = tmp->left;
			pPrelayout->right = tmp->right;
			pPrelayout->top = tmp->top;
			pPrelayout->bottom = tmp->bottom;
			pPrelayout->bfirst = false;

			if (tmp->properties & PR_VISIBLE)
				pPrelayout->properties |= PR_VISIBLE;
			else
				pPrelayout->properties &= ~PR_VISIBLE;
		}

		// Change Setting
		tmp->layout->Lookup(sKey, (void*&)playout);

		tmp->left = playout->left;
		tmp->right = playout->right;
		tmp->top = playout->top;
		tmp->bottom = playout->bottom;		

		if (playout->properties & PR_VISIBLE)
			tmp->properties |= PR_VISIBLE;
		else
			tmp->properties &= ~PR_VISIBLE;
	}
}

// Layout
void CFormItem::AddLayout(CMapStringToString* map)
{
	if (!map || map->IsEmpty())
		return;

	CString sIndex, sData;
	if (m_mapH->maplayout == nullptr)
		m_mapH->maplayout = new CMapStringToString;
	else
		m_mapH->maplayout->RemoveAll();

	for (POSITION pos = map->GetStartPosition(); pos;)
	{
		map->GetNextAssoc(pos, sIndex, sData);
		m_mapH->maplayout->SetAt(sIndex, sData);
	}
}

// Layout 현재 Layout 상태의 이름 얻기
CString CFormItem::GetCurrLayoutName()
{
	return m_mapH->LayoutName; 
}

void CFormItem::SetCurrLayoutName(CString sName)
{
	ZeroMemory(m_mapH->LayoutName, sizeof(m_mapH->LayoutName));
	CopyMemory(m_mapH->LayoutName, sName, sName.GetLength());
}


void CFormItem::RemoveForm(int index, bool bEmul)
{
	if (m_mapH->formR == NULL)
		return;

	struct _formR *tmp = m_mapH->formR, *tmp2;

	if (index == 0)
	{
		tmp2 = tmp->next;
		m_mapH->formR = tmp2;

		freeCell(tmp, bEmul);
	//	RemoveLayout(tmp);
		free(tmp);
		return;
	}

	tmp = GetForm(index - 1);
	tmp2 = tmp->next;
	tmp->next = tmp2->next;
	freeCell(tmp2, bEmul);
//	RemoveLayout(tmp2);
	free(tmp2);
}

// layout remove
void CFormItem::RemoveLayout(struct _formR *tmp)
{
	if (tmp->layout == NULL)
		return;
	
	for (POSITION pos = tmp->layout->GetStartPosition(); pos;)
	{
		CString sData;
		struct _Layout* layout;
		tmp->layout->GetNextAssoc(pos, sData, (void*&)layout);
		delete layout;
	}
	tmp->layout->RemoveAll();
	delete tmp->layout;
	tmp->layout = NULL;
}

void CFormItem::RemoveAll(bool bEmul)
{
	int	count = GetCount();

	for (int ii = 0; ii < count; ii++)
		RemoveForm(0, bEmul);
}

struct _formR* CFormItem::GetForm(int index)
{
	if (index < 0 || index >= GetCount())
		return NULL;

	struct _formR *tmp = m_mapH->formR;
	for (int ii = 0; ii < index; ii++)
		tmp = tmp->next;
	return tmp;
}

int CFormItem::GetCount()
{
	if (m_mapH->formR == NULL)
		return 0;

	struct _formR *tmp = m_mapH->formR;

	int	count = 1;
	while (tmp->next != NULL)
	{
		tmp = tmp->next;
		count++;
	}
	return count;
}

int CFormItem::GetIdxOfPtr(struct _formR* formR)
{
	if (m_mapH->formR && formR)
	{
		struct _formR *tmp = m_mapH->formR;

		for (int ii = 0; ii < GetCount(); ii++)
		{
			if (tmp == formR)
				return ii;
			tmp = tmp->next;
		}
	}

	return -1;
}

void CFormItem::SetFormStr(int index, CString field, CString data)
{
	void*	pos;

	if (!m_fields.Lookup(field, pos))
		return;

	struct	_formR*	formR = GetForm(index);
	if (formR == NULL)
		return;

	switch ((INT)pos)
	{
	case 0:		// name
		formR->name = SetStr(formR->name, data);
		break;
	case 1:		// fName
		formR->fName = SetStr(formR->fName, data);
		break;
	case 2:		// desc
		formR->desc = SetStr(formR->desc, data);
		break;
	case 3:		// auxS
		formR->auxS = SetStr(formR->auxS, data);
		break;
	case 4:		// scpN
		formR->scpN = SetStr(formR->scpN, data);
		break;
	case 5:		// editS
		formR->editS = SetStr(formR->editS, data);
		break;
	case 6:		// str
		formR->str = SetStr(formR->str, data);
		break;
	case 7:		// str2
		formR->str2 = SetStr(formR->str2, data);
		break;
	case 8:		// dat
		formR->dat = SetStr(formR->dat, data);
		break;
	case 9:		// rts
		formR->rtssy = SetStr(formR->rtssy, data);
		break;
	case 10:	// option Image
		formR->optionimg = SetStr(formR->optionimg, data);
		break;
	case 11:	// edit domino
		formR->refS = SetStr(formR->refS, data);
		break;
	case 12:	// hint
		formR->help = SetStr(formR->help, data);
		break;
	case 13:	// option image2
		formR->optionimg2 = SetStr(formR->optionimg2, data);
		break;
	case 14:	// option image3
		formR->optionimg3 = SetStr(formR->optionimg3, data);
		break;
	}
}


CString CFormItem::GetFormStr(int index, CString field)
{
	void*	pos;

	if (!m_fields.Lookup(field, pos))
		return _T("");

	struct	_formR* formR = GetForm(index);
	if (formR == NULL)
		return _T("");

	CString	rtVal = _T("");
	switch ((INT)pos)
	{
	case 0:		// name
		rtVal = GetStr(formR->name);
		break;
	case 1:		// fName
		rtVal = GetStr(formR->fName);
		break;
	case 2:		// desc
		rtVal = GetStr(formR->desc);
		break;
	case 3:		// auxS
		rtVal = GetStr(formR->auxS);
		break;
	case 4:		// scpN
		rtVal = GetStr(formR->scpN);
		break;
	case 5:		// editS
		rtVal = GetStr(formR->editS);
		break;
	case 6:		// str
		rtVal = GetStr(formR->str);
		break;
	case 7:		// str2
		rtVal = GetStr(formR->str2);
		break;
	case 8:		// dat
		rtVal = GetStr(formR->dat);
		break;
	case 9:		// rts
		rtVal = GetStr(formR->rtssy);
		break;
	case 10:	// option image
		rtVal = GetStr(formR->optionimg);
		break;
	case 11:	// edit domino
		rtVal = GetStr(formR->refS);
		break;
	case 12:	// hint
		rtVal = GetStr(formR->help);
		break;
	case 13:	// option image2
		rtVal = GetStr(formR->optionimg2);
		break;
	case 14:	// option image3
		rtVal = GetStr(formR->optionimg3);
		break;
	}
	return rtVal;
}

char* CFormItem::SetStr(char* map, CString data)
{
	int	len = data.GetLength();
	if (map)
		delete[] map;
	map = new char[len + 1];
	CopyMemory(map, (LPCTSTR)data, len);
	map[len] = '\0';
	return map;
}

CString CFormItem::GetStr(char* map)
{
	if (map == NULL || strlen(map) == 0)
		return _T("");

	return CString(map, (int)strlen(map));
}

void CFormItem::MoveForm(int from, int to)
{
	int	count = GetCount();

	if (from >= count || to > count)
		return;

	struct _formR *fromR, *toR;
	fromR = GetForm(from);
	
	if (to == 0)
		toR = NULL;
	else
		toR = GetForm(to - 1);
	
	if (from == 0)
		m_mapH->formR = fromR->next;
	else
		GetForm(from - 1)->next = fromR->next;

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

void CFormItem::freeCell(struct _formR* formR, bool bEmul)
{
	if (formR->scpN && !bEmul)
		DeleteFile((LPCTSTR)formR->scpN);

	if (formR->auxR)
	{
		struct _repR *repR = (struct _repR *)formR->auxR;
		CCellItem *cellItem = new CCellItem(repR);
		cellItem->RemoveAll();
		delete cellItem;
		delete[] formR->auxR;
	}
	
	for (POSITION pos = formR->layout->GetStartPosition(); pos;)
	{
		CString sData;
		struct _Layout* layout;
		formR->layout->GetNextAssoc(pos, sData, (void*&)layout);
		delete layout;
	}
	formR->layout->RemoveAll();	
	delete formR->layout;

	if (formR->margin)
	{
		formR->margin->RemoveAll();
		delete formR->margin;
	}

	if (formR->name)
		delete[] formR->name;
	if (formR->fName)
		delete[] formR->fName;
	if (formR->desc)
		delete[] formR->desc;
	if (formR->auxS)
		delete[] formR->auxS;
	if (formR->scpN)
		delete[] formR->scpN;
	if (formR->editS)
		delete[] formR->editS;
	if (formR->str)
		delete[] formR->str;
	if (formR->str2)
		delete[] formR->str2;
	if (formR->dat)
		delete[] formR->dat;
	if (formR->rtssy)
		delete[] formR->rtssy;
	if (formR->optionimg)
		delete[] formR->optionimg;
	if (formR->refS)
		delete[] formR->refS;
	if (formR->help)
		delete[] formR->help;
	if (formR->optionimg2)
		delete[] formR->optionimg2;
	if (formR->optionimg3)
		delete[] formR->optionimg3;
}
