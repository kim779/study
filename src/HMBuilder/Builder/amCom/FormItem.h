
#pragma once
	
#include <afxtempl.h>
#include <afxmt.h>
#include "../h/mapform.h"
#include "cellItem.h"

// CFormItem 명령 대상입니다.

class AFX_EXT_CLASS CFormItem
{
public:
	CFormItem(struct _mapH* mapH);
// updateXX__2014
	CFormItem(struct _mapH* mapH, class CUndoRedo* urdo);
	virtual ~CFormItem();

public:
	int	AddForm(struct _formR* formR = NULL);
	void	AddLayout(CMapStringToString* map);
	void	SetLayoutInfo(CString sKey, CString sPreKey);
	CString GetCurrLayoutName();
	void	SetCurrLayoutName(CString sName);
	void	RemoveForm(int index, bool bEmul=false);
	void	RemoveAll(bool bEmul=false);
	void	RemoveLayout(struct _formR *tmp);

	struct _formR*	GetForm(int index);
	int	GetCount();
	
	int	GetIdxOfPtr(struct _formR* formR);
	void	SetFormStr(int index, CString field, CString data);
	CString	GetFormStr(int index, CString field);
	char*	SetStr(char* map, CString data);
	CString	GetStr(char* map);

	void	MoveForm(int from, int to);

// updateXX_2014
public:
	void	freeCell(struct _formR* formR, bool bEmul=false);

	CMapStringToPtr	m_fields;
	struct	_mapH*	m_mapH;
};


