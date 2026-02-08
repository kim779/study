#pragma once
// ControlWnd.h : header file
//
//#include "InfoPopup.h"
//#include "jinfo.h"

#include "axwnd.h"
#include "symbolwnd.h"
#include "fxcodectrl.h"
#include "fximgbutton.h"


// m_nGubn ... 구분자로...
#define GU_NONE		-1
#define	GU_CODE		0	// 주식 
#define	GU_FUTURE	1	// 선물 
#define	GU_OPTION	2	// 옵션 
#define	GU_INDEX	3	// 업종 
#define	GU_FOCODE	4	// 선물  + 옵션
#define	GU_POPTION	5	// 주식옵션 
#define	GU_ELWCODE	6	// ELW 
#define GU_SCODE	7	// 신주인수권 
#define GU_BOND		8	// 채권(상품)
#define GU_SBOND	9	// 소매채권
#define GU_FCODE	10	// 주식선물 
#define GU_FOSTOCK	11	// 주식선물 + 옵션 
#define GU_FOREIGN	12	// 해외


/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CAxWnd
{
// Construction
public:
	CControlWnd(CWnd *pWnd);
	virtual ~CControlWnd();


// Attributes
public:
	CString		m_sData, m_sUserID, m_sRoot;
	COLORREF	m_boxColor, m_boxColorHover, m_bgCol, m_bgColHover;
	int		m_bgColor;
	CString		m_sCode;
	bool		m_ujongsise;	
	int		m_nFirst;	// 처츰 한번만 ...
	bool		m_bLastText;	// 4번째 텍스트 
	bool		m_bInit;	// 첫번째만 작동하지 않게...
	bool		m_bCate;	// 카테고리 버튼 
	bool		m_bSymbol;	// 심볼이름 
	bool		m_bInter;	// 관심 
	bool		m_bText;	// 마지막 텍스트처리
	bool		m_bTrigger;	// Trigger  
	bool		m_bDomino;	// Domino
	bool		m_bFocus;	// 포커스가 다른 곳으로 가는 곳을 막음.
	bool		m_bNext;	// 주문화면과 아닌 화면 구분 
	bool		m_bModify;
	bool		m_bSearch;	// 마지막 텍스트처리
	int		m_nGubn;

	CRect		m_rcText;
	CRect		m_ClientRect;

protected:
	std::unique_ptr<CSymbolWnd>	m_pSymbol;
	std::unique_ptr<CfxCodeCtrl>	m_pCode;
	std::unique_ptr<CfxImgButton>	m_pBtnCategory;
	std::unique_ptr<CfxImgButton>	m_pBtnInter;

public: 
	CString loadCodeList(int nGroup);
	void	Send(CString szCode);
	void	SetEditFocus(BOOL bflag = FALSE);

private:
	HINSTANCE	m_hCategory;
	CWnd		*m_pCategory;
	
	BOOL		m_bTabStop;
	static bool	m_bTimer;

	// 이미지 패스...
	CString		m_szBtnSearch;
	CString		m_szBtnHistory;
	CString		m_szBtnInter;
	CString		m_szBtnCategory;
	CString		m_Text1, m_Text2, m_Text3, m_Text4;

// Operations
private:
	void OnTrigger(CString str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetParam(struct _param *pParam);
	void	InitControl(CRect rect);
	void	clean();
	CString	GetHNam(CString code);

protected: 

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnCode();
	afx_msg void OnBtnInter();
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	BOOL m_visible;
	afx_msg void OnVisibleChanged();
	afx_msg BSTR GetData();
	afx_msg void SetData(LPCTSTR lpszNewValue);
	afx_msg BOOL GetTabStop();
	afx_msg void SetTabStop(BOOL bNewValue);
	afx_msg short GetUnitData();
	afx_msg void SetUnitData(short nNewValue);
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg BSTR GetProperties();
	afx_msg void SetCode(LPCTSTR code);
	afx_msg BSTR GetName();
	afx_msg void Text1(LPCTSTR str);
	afx_msg void Text2(LPCTSTR str);
	afx_msg void Text3(LPCTSTR str);
	afx_msg void Text4(LPCTSTR str);
	afx_msg void SetFocus();
	afx_msg void Trigger(LPCTSTR name, BOOL all);
	afx_msg void Enable(BOOL bflags);
	afx_msg void Clear();
	afx_msg void SetUnit(short unit);
	afx_msg BOOL IsKonex();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
private:
	bool Resize();
};