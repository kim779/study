#pragma once
// base.h : header file
//
//수정--이소진.09.05.26
#define	CCOD		"1301"
#define	HCOD		"1301"
#define	FCOD		"30301"
#define	OCOD		"40301"
#define	PCOD		"PCOD"
#define	UCOD		"20301"
#define ELWCOD		"E1301"
#define SINJUCOD	"scode"
#define THIRDCOD	"1301"
#define MCSYMBOL	"IB7020"

struct Maplist
{
	class CPane* paneWnd;
	CRect	rc{};

	Maplist(CPane* pwnd)
	{
		paneWnd = pwnd;
	}
	Maplist(CPane* pwnd, CRect r)
	{
		paneWnd = pwnd;
		rc = r;
	}

	Maplist()
	{
		paneWnd = NULL;
		rc = CRect(0, 0, 0, 0);
	}
};



typedef CArray<Maplist, Maplist> CMaplistArray;

/////////////////////////////////////////////////////////////////////////////
// CBase window

class CBase : public CWnd
{
// Construction
public:
	CBase();
	virtual ~CBase();

// Attributes
protected:
	CWnd*		m_parent;
	
	CString		m_user;
	CString		m_domino;

	class CConfigBar*	m_config;

//	CArray < class CPane* , class CPane* > m_sList;
	CMaplistArray m_sList;//수정--이소진.09.05.26

	std::vector<std::unique_ptr<CPane>> _vList;

	int			m_iWidth, m_iHeight, m_iFontSize;
	double		m_lyRate;
	CString		m_strFontName;
	CMapStringToString	m_astrSaveList;
	int			m_iCurMap;
	CString		m_strCurKey;
	CString		m_strCurrValue;		//사용자가 호가컨트롤에서 클릭한 데이타
	
	CString     m_strAcc;
// Operations
public:
	BOOL		CreateMap(CWnd*, CRect);
	COLORREF	GetColor(int index);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBase)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	int			m_TriggerKey;
	//CWnd*		m_temp;
	std::unique_ptr <CWnd> m_temp;
	// Generated message map functions
protected:
	void	DLLAxis(int type, int key, LPARAM lParam);
	BOOL	LoadInfo();
	BOOL	LoadInfo(CString section);
	BOOL	LoadType(CStringArray& ary, CString dat);
	CRect	GetPaneRect(int shape, int pos);
	CSize	GetPaneSize(int shape, int ipos);
	void	ResetMapSize(int ibarH, int cx, int cy);

	int		GetType(int type, int pos);
	void	ChangeColor();
	void	ChangeFont(int pointFont, CString fontName);
	void	PosChangeBar(int cx, int cy);
	void	Configuration(CString domino);

	CString		Variant(int comm, CString data = _T(""));
	
	//{{AFX_MSG(CBase)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg	LONG OnUSER(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnXMSG2(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnXMSG3(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnLoad(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnDataTrans(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyPassword(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	int ToValidType(int type);
	CBitmap* m_bmp[2][3];
	CBitmap* m_bmpExpand;
	BOOL	m_bmpReady;
	CBitmap*	GetBitmap(const char* bmpName);
	CString		GetSkinName();

public:
	CString m_strTrigger;
	CString m_strCode;
	CString m_data;
	CString		m_home;
	void LoadBitmap();
	COLORREF	m_bgColor, m_contentsbgColor, m_contentslineColor;

	BOOL	ImageReady() const 
	{
		return m_bmpReady;
	}

	void	DrawButton(CDC* dc, CRect rc, BOOL active, const char* text);
	int		DrawExpand(CDC* dc, CRect rc, UINT stat);
	int		GetExpandWidth();
	COLORREF	GetSkinColor();
	COLORREF	GetSkinColor(int index);

	COLORREF	GetIndexColor(int index);
	void		ParseTriggerValue(CString sData);
	void		SendDataToMap(int iMapKey, CString sKey, CString sData);
	void		SendToMap(int iMapKey, CString sKey, CString sVal, bool bPhonepad = false);
	int			GetCodeType(CString code);



	DWORD LoadInfo_C(CString sKey, char* szdata);

};

#define EXSTAT_NONE			0x00
#define EXSTAT_LEFT_DOWN	0x01
#define EXSTAT_LEFT_HOVER	0x02
#define EXSTAT_RIGHT_DOWN	0x03
#define EXSTAT_RIGHT_HOVER	0x04

