#pragma once

#include "../../../h/jmcode.h"
/////////////////////////////////////////////////////////////////////////////
// QuickSort
template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);

		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}


template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii, jj;
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];
	do {
		if (bAscending) 
		{
			while (pArr[jj].Name < str.Name) jj++;
			while (pArr[ii].Name > str.Name) ii--;
		} 
		else 
		{
			while (pArr[jj].Name > str.Name) jj++;
			while (pArr[ii].Name < str.Name) ii--;
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}

			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);
}

// CCodeCombo
class CControlwnd;
class CCodeCombo : public CComboBox
{
	DECLARE_DYNAMIC(CCodeCombo)
	enum { DM_NONE, DM_HISTORY, DM_CODELIST};
	struct _JCode
	{
		CString Code;
		CString Name;
		CString up;
		CString bBal;
		CString Balcode;
	};

	struct OCode
	{
		CString	Price;
		char	Atm;
		struct CALL
		{
			char	Exist;
			CString Name;
			CString	Code;
		} Call[11];
		struct PUT
		{
			char	Exist;
			CString	Name;
			CString	Code;
		} Put[11];
	};
	struct	OCode	oCode;

	class CControlwnd* m_pParent;
	CString			m_sCode, m_pTemp;
	HWND			m_hList;
	class CSubList		*m_pSubList;
	CFont			*m_pFont;

	CMapStringToString m_mapCode;
	CArray <_JCode, _JCode> m_pHCode;	// history list
	CQArray <_JCode, _JCode> m_pJCode;	// code list
	CQArray <_JCode, _JCode> m_pSearchCode;	// code list
	CStringArray		m_arString;	// text list
	
	CWnd			*m_pWizard;	// Wizard Pointer
	int m_Unit;
	int			m_DataMode;
	CString			m_sSymbol;	// history symbol
	int			m_CellHeight;
public:
	int			m_ifind;

public:
	CCodeCombo();
	CCodeCombo(CControlwnd *pParent);
	bool	CodeLoad(CString tabPath);
	bool	CheckType(CString strcode);
	
	void	SetFont(CFont* pFont, bool redraw = true);
	void	AddString(LPCTSTR lpszString);
	void	RemoveString(int index);
	CString	GetText(int index);
	void	SetWizard(CWnd *pWizard) { m_pWizard = pWizard; };

	void	InitCodeList();
	void	InitHistory(CString sSymbol, CString history);
	void	RemoveHistory(CString sSymbol,CString history, CString strcode);
	void	SetUnit(int unit) { m_Unit = unit; }
	virtual ~CCodeCombo();
	void	ResetCode();
	bool	IsCodeListMode();
	void	ResetString()	{ m_arString.RemoveAll(); };
	CString	GetSelectCode();
	void	SearchCode(CString code, bool benter = false);
	void	SearchNumberCode(CString code);
	bool	IsValidCode(CString sCode);
	void	Key(int key);
	bool	IsHistoryMode();
	CString GetCode(CString strname);
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	afx_msg void OnCbnSelchange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnCbnDropdown();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


