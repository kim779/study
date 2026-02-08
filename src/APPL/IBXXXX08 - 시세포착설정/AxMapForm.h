#pragma once
#include "resource.h"
#include <afxtempl.h>

#define	WM_KEYMSG	WM_USER + 100
#define	WM_AMTPOPUPMSG	WM_USER + 101
#define	WM_HYPERLINK	WM_USER + 102
#define	WM_SUBMITACC	WM_USER + 103
#define WM_INITPORC	WM_USER + 104

#define	FD_SUPPRESSPM	1
#define	FD_SUPPRESSPLUS	2
#define	FD_SUPPRESSZERO	4

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CAxMapForm : public CFormView
{
protected:
	CAxMapForm(UINT nIDTemplate = 0);
	DECLARE_DYNCREATE(CAxMapForm)

public:
	//{{AFX_DATA(CAxMapForm)
	//}}AFX_DATA

public:
	CWnd* m_pParent{};
	CArray	<HWND, HWND>	m_tabOrder;
	bool			m_bEnter{};
	CStringArray		m_accountArr;

	enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT};

private:
	CWnd			*m_pFocusWnd{};
	CMapStringToString	m_axOrderMap;

public:
	void EnDecrypt(char *pass, int nCnt);
	void SetString(char *pChar, int nMinLen, CString sStr);
	BOOL IsFOptionAcc(CString sAcc);
	BOOL IsStockAcc(CString sAcc);
	CString GetString(char *pChar, int nMinLen);
	void		ClipChildren(CDC *pDC);
	void		SetAccountInfo(CString sAccount, CString sPass, CString sAccName);
	void		SetAccountInfo(CString str);
	void		SetAccountINIT(CString str);
	void		RemoveOrderMap(CString sKey);
	CString		GetOrderMap(CString sKey);
	void		SetOrderMap(CString sKey,CString sOrderInfo);
	virtual void	ContractNotify(CString sAcc, CString sAccGubn, CString sAdmin, CString sOrder, CString str);
	CString		GetOrderMapKey();
	void		ParsingNotice(CString str);
	CString		GetHsga(CString sCode);
	int		GetJKind(CString sCode);
	CString		GetJName(CString sJCode);
	double		GetPriceUnit(double price, CString sJCode, bool bPlus = true);
	CString		GetAccount(int nIdx);
	int		SetAccountCBO(CComboBox *pCombo, CString sAccKey = "");
	CString		SetComma(CString src);
	CString		getFormatData(CString src, bool bComma, int nSuppress);
	void		drawContrast(CDC *pDC, CRect cellRc, char cStyle, COLORREF clr);
	BOOL		SendTR(CString name, BYTE type = 0, CString data = "", int key = 0);
	void		DeleteMapObject(CWnd *pObjWnd, int nObjKey);
	CWnd*		CreateMapObject(CString sMapName, int &nObjKey, CPoint point = CPoint(0, 0));
	int		Variant(int cmd, int data = 0);
	CString		Variant(int cmd, CString data = "");
	
	COLORREF	GetIndexColor(int index);
	CBitmap*	getAxBitmap(CString path);
	CFont*		getAxFont(CString fName = "±¼¸²Ã¼", int point = 9, bool bItalic = false, int nBold = 0);
	CBrush*		getAxBrush(COLORREF clr);
	CPen*		getAxPen(COLORREF clr, int width, int style);

	void		SetView(int type, CString data);
	
	// utitlities
	CString		Parser(CString &srcstr, CString substr);
	CRect		adjustRect(CRect screenRect);
	
	void		PostNcDestroy();
	void		CreateMap(CWnd *pParent, CSize szForm);

// Overrides
	//{{AFX_VIRTUAL(CAxMapForm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

//	virtual void OnCmd(UINT message, WPARAM wParam, LPARAM lParam);
// Implementation
protected:
	virtual ~CAxMapForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CAxMapForm)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnTabNext(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

