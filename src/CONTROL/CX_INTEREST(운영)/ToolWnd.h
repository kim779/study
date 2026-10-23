#pragma once
// ToolWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolWnd window
#include "BaseWnd.h"

#include "../../control/fx_misc/fxImgButton.h"
//#include "../../control/fx_misc/fxSpin.h"
#include "fxSpin.h"

#include "btntab.h"
#include "NewMenu.h"	// ADD PSH 20070912


class CToolWnd : public CBaseWnd
{
// Construction
public:
	CToolWnd(CWnd* pMainWnd);

// Attributes
public:

// Operations
public:
	void ReloadList();
	void ReloadList2();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	void OperInit();
	void OperDraw(CDC* pDC);
	void OperDestory();
	void OperResize(int cx, int cy);
	void OperPallette();
	
// Implementation
public:
	virtual ~CToolWnd();

	// Generated message map functions
public:
	//{{AFX_MSG(CToolWnd)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	
	void	saveGroupwhenClose(int index);
	
	void	clickBtn();	
	HBITMAP getBitmap(CString path);
	CRect	AddDivide(CRect rect);	
	CRect	GetNextRect(CRect rcPrev, CSize size, CSize gap);
	CRect	MoveButton(CRect rcPrev, CSize size, CSize gap, CfxImgButton* pButton);
	void	DrawSEP(CDC* pDC, CRect rect);
	void	HideControl(CRect rcBasket);
	void	loadcfg();
	CString	loadFieldnm(int fno = -1);
	void	loadTabItem();
	int		loadActiveField();

	void	savecfg();
	void	SetSel(int nIndex);
	void	SelectGroup(UINT kind);
	void	SendTree(int nIndex);
	int		Reload();
	void	SelectMode(int nmode);
	// ADD PSH 20079017
	void	RemoveAllMenu();
	// END ADD
	void testSaveFile();
	
	void dominoSelectGroup(int index);
	void saveGroupIndex(int index);		//관심그룹 index 저장
	void SetGroupDataView();			//그룹을 어떻게 표시할건지 설정 요청(한 그룹에 보던지, 넘기던지)
	
	CString GetFieldChangeBtn();
	void	clickmovePageBtn(int type);
	int countInterest(int group);
	void SetCbPageView();
	void CheckPrevNextBtn();
	void SetViewType();

// window members
	CfxImgButton	m_btPREV;	// 트리 펼치기
	CfxImgButton	m_btNEXT;	// 트리 숨기기
	CfxImgButton	m_btUP;		// 툴바 줄이기
	CfxImgButton	m_btDN;		// 툴바 늘이기	
	CfxImgButton	m_btBASKET;	// 바구니 
	CfxImgButton	m_btTEXT[2];	// 설정/예상/엑셀
	CfxImgButton	m_btHAWKEYE;	// 호크아이
	CfxImgButton	m_btMINUS;	// 마이너스
	CfxImgButton	m_btPLUS;	// 플러스
	CfxImgButton	m_btTool;	// 도구
	CfxImgButton	m_btTURN;	// 돌려보기
	CfxImgButton	m_btCHART;	// 차트 버트
	CfxImgButton	m_btAddSelect;	// 보이는 화면에만 추가
	CfxImgButton	m_btAddVisible;	// 보이는 화면에만 추가
	CfxImgButton	m_btAddFull;	// 최대한 담고 다음 화면으로 추가 
	CfxImgButton	m_btEqualizer;	// 이퀄라이저 
	
	CfxImgButton	m_btChangeField;	
	CfxImgButton	m_btPrevView;	
	CfxImgButton	m_btNextView;	
	CComboBox	m_cbArrange;
	CComboBox	m_cbViewType;
	CComboBox	m_cbPageView;
	
	CfxSpin*	m_spinTIME;		// 돌려보기 시간
	std::unique_ptr<class CIHCheck>		m_chkAUTO;		// Auto예상
	class CIHCheck*	m_chkEXPAND;		// 분할모드 1 : 확장하면서 그룹추가 0 : 분할하면서 추가
	CBtnTab		m_btntab;
	
	
	CArray<CRect, CRect>	m_arDIVIDE;
	CArray<CWnd*, CWnd*>	m_arCONTROL;

	CRect		m_rcSECOND;
	CRect		m_rcFIELD;
	
	CComboBox	m_cbFIELD;
	CComboBox	m_cbGROUP;

	// ADD PSH 20070911
	CfxImgButton	m_btField;		// 필드 
	CfxImgButton	m_btFieldDrop;
	CNewMenu		m_menuField;
	// END ADD

// data memebers
	CString		m_fileIMG;
	CString		m_fileFIELD;
	CString		m_filePPOLIO;
	CBrush*		m_pBrush;
	CString		m_activefield;	
	BOOL		m_bBig;	

	// ADD PSH 20070907
	COLORREF    m_clrOutLine;
	BOOL		m_bMoveCfg;
	int			m_nMoveSave;
	
	int			m_selectedTab;
	CCriticalSection	m_sync;
	// END ADD
};