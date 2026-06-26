#pragma once
#include "afxdialogex.h"

// CDlg_Grid 대화 상자
const	int maxVIEW = 1;

struct	_gridHdr {
	UINT	stid;		// string table id
	UINT	width;		// fixed width
	UINT	attr;		// fixed attribute

	CString	symbol;		// nonfixed symbol
	UINT	format;		// nonfixed format
	UINT	attrx;		// nonfixed attr

	UINT	fidx;		// field index
};

#define ROUND_PANEL		1
#define ROUND_CONTENTS	2
#define P_DELI	0x7f
#define P_TAB	0x09
#define P_NEW	0x0a
#define P_PER	0x25
#define MAXROW	200

#define IDS_TITLE                       1
#define IDS_EXPECT                      2
#define IDS_CONFIG                      3
#define IDS_DEFAULTFONT                 4
#define IDS_GH_RCOD                     5
#define IDS_GH_CODE                     6
#define IDS_GH_NAME                     7
#define IDS_GH_CURR                     8
#define IDS_GH_DIFF                     9
#define IDS_GH_RATE                     10
#define IDS_GH_VOL                      11
#define IDS_GH_ASK                      12
#define IDS_GH_BID                      13
#define IDS_GH_TASKSIZE                 14
#define IDS_GH_TBIDSIZE                 15
#define IDS_GH_OPEN                     16
#define IDS_GH_HIGH                     112
#define IDS_GH_LOW                      113
#define IDS_GH_ANTIPRC                  121
#define IDS_GH_ANTIVOL                  122
#define IDS_GH_PRECURR                  142
#define IDS_GH_PREHIGH                  143
#define IDS_GH_PRELOW                   144
#define IDS_GH_PREOPEN                  154
#define IDS_GUIDE1                      201
#define IDC_INTERNM0                    5000
#define IDC_INTERNM1                    5001
#define IDC_INTERNM2                    5002
#define IDC_INTERNM3                    5003
#define IDC_EXPECT                      5100
#define IDC_CONFIG                      5200
#define IDC_MINUS                       5300
#define IDC_PLUS                        5400
#define IDC_HAWKEYE                     5500
#define IDD_MAPDLG                      15000




class CDlg_Grid : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Grid)

public:
	CDlg_Grid(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Grid();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

	std::array<std::unique_ptr<class AxGrid>, maxVIEW> _grid{};

	int		m_gridW{};
	
	enum _viewType { viewTWO = 2, viewTHREE = 3, viewFOUR = 4 } m_viewType;
	CFont* m_font{};
	CFont* m_fontB{};

	CString m_root{}, m_name{};

	CBrush		m_bkBrush{};
	int		m_fontsize{};
	COLORREF	m_blinkColor{};
	COLORREF	m_bookmarkColor{};

	void init();
	void initControl();
	void	initialGrid(int size = true);
	void loadFieldInfo();
	void loadGridinfo();
	int	loadInterName(bool bcursel = true);
	void repositionView();

	CString parseX(CString& srcstr, CString substr);

	bool		m_blink{};
	bool IsBlink();

	CComboBox	m_internm[maxVIEW];
	std::vector<_gridHdr> 		_vgridHdr{};

	int _httoi(const TCHAR* value);
	void restoreMF();

	COLORREF GetColor(int color);
	int	getFlashIndex();

	CFont* getAxFont(CString fName = "굴림체", int point = 9, int style = 0);
	CBitmap* getAxBitmap(CString path);

	double TOfabs(CString string);
	double TOf(CString string);
	CString idTOstring(UINT id);
	CString	charTOstring(char* datB, int datL);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

public:


#ifndef DF_NOHTS
	std::unique_ptr <CCategory>	m_category[maxVIEW]{};
	CfxImgButton	m_hawk;
	CfxImgButton	m_expect;
	CfxImgButton	m_config;
	CfxImgButton	m_minus;
	CfxImgButton	m_plus;
#endif
	CButton	m_hawk;
	CButton	m_expect;
	CButton	m_config;
	CButton	m_minus;
	CButton	m_plus;
	 
	HBITMAP		m_bitmap2_nm{}, m_bitmap2_dn{}, m_bitmap2_hv{};
	HBITMAP		m_bitmapM_nm{}, m_bitmapM_dn{}, m_bitmapM_hv{};
	HBITMAP		m_bitmapP_nm{}, m_bitmapP_dn{}, m_bitmapP_hv{};
	HBITMAP		m_bitmapH_nm{}, m_bitmapH_dn{}, m_bitmapH_hv{};


	inline UINT AFXAPI HashDataAXIS(LPCSTR key)
	{
		UINT nHash = 0;
		while (*key)
			nHash = (nHash << 5) + nHash + *key++;
		return nHash;
	}

};
