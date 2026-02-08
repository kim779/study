#pragma once
// ControlWnd.h : header file
//
#include "InfoPopup.h"
#include "jinfo.h"

#include <AFXTEMPL.H>

typedef struct _sGrade {
	CString strName;
	CString strCode;
	CString strGrade;
} sGrade;

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control CX_Symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	virtual ~CControlWnd();

// Attributes
public:
	bool		m_capture, m_bCheck, m_bMouseOver;
	CWnd*		m_pParent;
	std::unique_ptr<CInfoPopup> m_pPopup;
	CParam		m_Param;
	CString		m_sData, m_rtnStr, m_sUserID, m_sRoot;
	BOOL		m_bTracking, m_bHover;
	COLORREF	m_boxColor, m_boxColorHover, m_pRgbHover, m_tRgbHover;

	Cjinfo		m_jinfo;

	CString		m_sCode{};
	bool		m_ujongsise;
	typedef enum memo_state { MEMO_NOT_YET = 0, MEMO_EXIST, MEMO_NOEXSIST }_MEMO;
	int		m_memostate;
	CBitmap*	m_bitmap;
	CBitmap*	m_pTBmp;
	CBitmap*	m_pBBmp;
	CBitmap*	m_pRBmp;

	CString	m_os;
	CString	m_cpu;
	CString	m_h_hdd_key;
	CString	m_h_drive_type;
	CString	m_h_controler_type;
	CString	m_h_model_number;
	CString	m_h_serial_number;
	CString	m_n_subnetmask;
	CString	m_n_gateway;
	CString	m_n_dhcp_enabled;
	CString	m_h_drive_type2;
	CString	m_h_controler_type2;
	CString	m_n_dhcp_enabled2;
	CString	m_n_ip_address;
	CString	m_n_mac_key;
	CString	m_n_adap_name;
	CString	m_n_adap_desc;
	CString	m_n_physical_mac;
	CString	m_n_current_mac;

	CString		m_strCaseTip;

	std::unique_ptr<CToolTipCtrl>	m_pTipCtrl;
	BOOL	m_bBmpSlice, m_bFromDLL;

	char		m_chjgub;

	bool		m_bSideTime;

	void load_jinfo(char* pData);
	void load_jinfo3(char* pData);

	CString m_slog;
	int m_trKey = -1;   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!2016 복수종목화면에서만 사용!!!!!!!!!!!!!!!!!!!!!!!!!!!
	BOOL m_bServerMode{};  //서버저장모드인지
	void oubMemoPopup(char* pdata);

	enum { TYPE_KRX = 1, TYPE_NXT };
	int m_iMarket = TYPE_KRX;
	CString m_sRtsCode{};
	CString m_sSearchkrgb{};
	CString m_sID{};

	DllInvoker codedll{};
	BOOL IsNXTAble(CString sCode);
	CMapStringToString m_mapCodeToNXT{};
	int GetMKgubn(CString sCode);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString addComma(CString data);
	CString Variant(int comm, CString data);
	void	WriteAlert(CString str);
	void	parseAlert(struct _alertR* alert);
	CString	Parser(CString &srcstr, CString substr);
	void	drawTriangle(CDC *pDC, CRect rc, COLORREF clr);
	void	drawIndicator1(CDC* dc, CRect rect, COLORREF clr);
	void	drawIndicator2(CDC* dc, CRect rect, COLORREF clr);
	void	drawBitmap(CDC *pDC, CRect rect, CBitmap *pBitmap);
	void	DrawBitmap(CDC* pDC, CRect rect);
	void	SetParam(struct _param *pParam);
	LOGFONT	setFont(CString fName, int fsize, int style);
	COLORREF GetIndexColor(int index);
	bool	GetCheck();
	void	SetCheck(bool bCheck);
	BOOL	GetState();
	void	checkMemo(CString sKey);
	int	checkFileMemo(CString sKey);
//	void	loadBitmap(CString jgub, CString kind);
	void	loadBitmap(CString jrab, CString nrat, CString jgub, CString kind, CString rned = "", CString dist = "" , CString vist = "", CString vitime = "", CString vitype = "");

	CPen*	getAxPen(COLORREF clr, int width, int style);
	CBrush* getAxBrush(COLORREF clr);
	CFont*	getAxFont(CString fName, int point, int style);
	CBitmap* getBitmap(CString path);

	BOOL	SendTR_Dll(char* datb, int datl);
	void	SendTR_Control(CString name, char* datb, int datl, char stat);	

	BOOL	SendMemoTR_Dll(char* datb, int datl, int trkey, CString sKey);
	void	SendMemoTR_Control(CString name, char* datb, int datl, char stat, int trkey, CString sKey);

	void	LoadLoanGrade(void);

	CPoint		m_ptUp;
	CArray< sGrade, sGrade > m_arrGrade;

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	BOOL m_visible;
	afx_msg void OnVisibleChanged();
	CString m_sInfo;
	afx_msg void OnSInfoChanged();
	CString m_sMkMsg;
	afx_msg void OnSMkMsgChanged();
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg BSTR GetProperties();
	afx_msg void SetSideTime(BOOL flag);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};