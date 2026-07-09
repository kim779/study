// FormItem1.h: interface for the CFormItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMITEM1_H__4F8B7A51_40F8_43D3_BB88_1E834C6A24B0__INCLUDED_)
#define AFX_FORMITEM1_H__4F8B7A51_40F8_43D3_BB88_1E834C6A24B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "../form_w.h"
#include "../mapform.h"
#include "CellItem.h"

class AFX_EXT_CLASS CFormItem  
{
public:
	CFormItem(struct _mapH *mapH);
	virtual ~CFormItem();
public:
	void FreeCell(struct _formR *formR);
	void RemoveAllForm();
	int addForm(_formR *formR = NULL);
	int insertForm(int idx, _formR *formR = NULL);
	void removeForm(int idx);
	void removeLastForm(int lastIdx);
	struct _formR* getFormPoint(int idx);
	int getFormCount();
public:
	int getIdxOfPtr(struct _formR* ptr);
	void setFormStr(int idx, CString field, CString dat);
	CString getFormStr(int idx, CString field);
	CString GetStr(char *map);
	char* SetStr(char *map, CString data);
	void moveForm(int from, int to);
protected:
	struct _mapH*	m_mapH;
	CString		m_field;

};

#endif // !defined(AFX_FORMITEM1_H__4F8B7A51_40F8_43D3_BB88_1E834C6A24B0__INCLUDED_)
