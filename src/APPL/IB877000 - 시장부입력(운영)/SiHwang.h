#pragma once
#include <vector>

#include "AxisExt.h"
#include "grid/GridCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"

#define IDC_GRID		1000
#define IDC_TITLE		1001
#define IDC_BODY		1002
#define IDC_BTN_INSERT	1003
#define IDC_BTN_UPDATE	1004
#define IDC_BTN_DELETE	1005
#define IDC_CB_SUBG		1006
#define IDC_BTN_PRETAG	1007
#define IDC_DATE		1008
#define IDC_TIME		1009
#define IDC_CHKINPUT	1010
#define IDC_BTN_TMPL1	1011

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSiHwang : public CWnd, public CAxisExt
{
public:
	CSiHwang(CWnd *pParent);
	void SafeDelete( CWnd *pWnd );
	std::shared_ptr<CfxImgButton> CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp);

public:
	std::unique_ptr<CGridCtrl> m_pGrid;
	//CEdit *m_pTitle, *m_pBody, *m_pDate, *m_pTime;
	std::unique_ptr <CRichEditCtrl> m_pTitle, m_pBody, m_pDate, m_pTime;
	std::shared_ptr <CfxImgButton> m_pInsert, m_pUpdate, m_pDelete, m_pPreTag, m_pTmp1Tag;
	std::unique_ptr <CButton> m_pChkSendTime;
	std::unique_ptr <CComboBox>m_pSubg;
	CBitmap *m_hBtn2[3], *m_hBtn6[3];

public:
	void RequestPIBO7710(LPCSTR nkey=NULL);
	void ResponsePIBO7710(LPCSTR dat, int len);
	void RequestPIBONEWS(LPCSTR keyv);
	void ResponsePIBONEWS(LPCSTR dat, int len);
	void ResponsePIDOSIHW(LPCSTR dat, int len);

public:
	std::vector< std::vector<char> > m_sihw_queue;	
	BOOL m_bWait;
	CString m_nkey, m_usid, m_news_keyv, m_news_date, m_news_gubn, m_news_subg, m_news_seqn, m_news_time;
	int m_news_subg_idx;
	CRect m_rcTitle, m_rcBody, m_rcControl;
	int m_news_srow;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSiHwang)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

public:
	virtual ~CSiHwang();

protected:
	//{{AFX_MSG(CSiHwang)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnScrollEnd( WPARAM wParam, LPARAM lParam );
	afx_msg void OnInsert();
	afx_msg void OnUpdate();
	afx_msg void OnDelete();
	afx_msg void OnPreTag();
	afx_msg void OnChkInp();
	afx_msg void OnTmpl1();
};

