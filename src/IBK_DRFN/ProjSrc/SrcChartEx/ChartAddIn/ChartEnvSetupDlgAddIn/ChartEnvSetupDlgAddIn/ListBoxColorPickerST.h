//
//	Class:		CListBoxColorPickerST
//
//	Compiler:	Visual C++
//	Tested on:	Visual C++ 6.0
//
//	Version:	See GetVersionC() or GetVersionI()
//
//	Created:	26/June/2000
//	Updated:	04/October/2001
//
//	Author:		Davide Calabro'		davide_calabro@yahoo.com
//									http://www.softechsoftware.it
//
//	Remarks:	Based on the control by James R. Twine
//				and Mark Jackson (mark@mjsoft.co.uk)
//
//	Disclaimer
//	----------
//	THIS SOFTWARE AND THE ACCOMPANYING FILES ARE DISTRIBUTED "AS IS" AND WITHOUT
//	ANY WARRANTIES WHETHER EXPRESSED OR IMPLIED. NO REPONSIBILITIES FOR POSSIBLE
//	DAMAGES OR EVEN FUNCTIONALITY CAN BE TAKEN. THE USER MUST ASSUME THE ENTIRE
//	RISK OF USING THIS SOFTWARE.
//
//	Terms of use
//	------------
//	THIS SOFTWARE IS FREE FOR PERSONAL USE OR FREEWARE APPLICATIONS.
//	IF YOU USE THIS SOFTWARE IN COMMERCIAL OR SHAREWARE APPLICATIONS YOU
//	ARE GENTLY ASKED TO DONATE 1$ (ONE U.S. DOLLAR) TO THE AUTHOR:
//
//		Davide Calabro'
//		P.O. Box 65
//		21019 Somma Lombardo (VA)
//		Italy
//
#ifndef _LISTBOXCOLORPICKERST_
#define _LISTBOXCOLORPICKERST_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

void DDX_ListBoxColorPickerST(CDataExchange *pDX, int nIDC, COLORREF& prgbColor);

class CListBoxColorPickerST : public CListBox
{
public:
	CListBoxColorPickerST();
	virtual	~CListBoxColorPickerST();

	int AddString(LPCTSTR lpszItem, COLORREF crColor);
	int InsertString(int nIndex, LPCTSTR lpszString, COLORREF crColor);
	int DeleteString(int nIndex);
	int ReplaceString(int nIndex, LPCTSTR lpszString, COLORREF crColor);
	void ResetContent();

	int SetColor(int nIndex, COLORREF crColor, BOOL bRepaint = TRUE);
	COLORREF GetColor(int nIndex);
	COLORREF GetSelectedColor();

	int SetItemData(int nIndex, DWORD dwItemData);
	DWORD GetItemData(int nIndex);
	int SetItemDataPtr(int nIndex, void* pData);
	void* GetItemDataPtr(int nIndex);

	int SelectColor(COLORREF crColor);
	int GetTextAndColor(int nIndex, LPTSTR lpszBuffer, COLORREF* lpcrColor);
	int GetTextAndColor(int nIndex, CString& rString, COLORREF* lpcrColor);

	static short GetVersionI()		{return 11;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.1");}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxColorPickerST)
	//}}AFX_VIRTUAL
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected:
	//{{AFX_MSG(CListBoxColorPickerST)
	afx_msg void OnDestroy();
	//}}AFX_MSG

private:
	void FreeResources();
	int ReplaceItemData(int nIndex, DWORD dwItemData, LPVOID pData, COLORREF crColor, BYTE byMask);
	void DeleteItemData(int nIndex);

#pragma pack(1)
	typedef struct _STRUCT_LBDATA
	{
		DWORD		dwItemData;	// 32-bit value associated with an item in the list box
		LPVOID		pData;		// Pointer associated with an item in the list box
		COLORREF	crColor;	// Color associated with an item in the list box
	} STRUCT_LBDATA;
#pragma pack()

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif
