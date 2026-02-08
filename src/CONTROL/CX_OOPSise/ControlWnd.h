#pragma once
// ControlWnd.h : header file
//
#define			MAX_DATA		100

struct DrawList
{
	int	icol = 0;
	int	iType[MAX_DATA]{};	//0-lbl, 1-send data
	CString symbol[MAX_DATA];
	int	srtssymbol[MAX_DATA]{};
	CString stext[MAX_DATA];
	double	lWidthR[MAX_DATA]{};
	int	iVLine[MAX_DATA]{};
	int	iHLine[MAX_DATA]{};
	int	iDataFormat[MAX_DATA]{};	//txtformat 0-lbl, 1-+/-comma, 2-comma, 3-+/-signcolor, 4-signcolor,//5-diff, 6-yyyy-mm-dd, 7-yy-mm-dd, 8-HH:MM:SS, 9-MM:SS

	CString	sType[MAX_DATA];
	CString suffixes[MAX_DATA];
	CString suffixesA[MAX_DATA];

	CRect	rc[MAX_DATA];
	int	iTxtAlign[MAX_DATA]{};
	int	iFontBold[MAX_DATA]{};
	int	ifgcolor[MAX_DATA]{};
	int	ibgcolor[MAX_DATA]{};
	int	iorgfgcolor[MAX_DATA]{};

	DrawList(CString sList1, CString sList2, CString sList3, CString sList4, CString sList5, CString sList6,
				CString sList7, CString sList8, CString sList9, CString sList10)
	{
		
		icol = SplitString(sList1, ";", symbol);
		SplitInt(sList2, ";", iType);

		for (int ii = 0; ii < icol; ii++)
		{
			symbol[ii].Trim();
			suffixes[ii] = _T("");
			srtssymbol[ii] = atoi(symbol[ii].Right(3));

			if (iType[ii] == 0)
				stext[ii] = symbol[ii];

			if (iType[ii] == 2) 
			{
				const	int	itmp = symbol[ii].Find("-");
				if (itmp > -1)
				{
					suffixes[ii] = symbol[ii].Right(symbol[ii].GetLength() - itmp - 1);
					suffixes[ii].Trim();
					symbol[ii] = symbol[ii].Left(itmp);
					symbol[ii].Trim();
					srtssymbol[ii] = atoi(symbol[ii].Right(3));
				}
			}

			if (iType[ii] == 3) 
			{
				int	itmp = symbol[ii].Find("-");
				if (itmp > -1)
				{
					suffixesA[ii] = symbol[ii].Left(itmp);
					symbol[ii] = symbol[ii].Right(symbol[ii].GetLength() - itmp - 1);
					symbol[ii].Trim();

					itmp = symbol[ii].Find("-");
					if (itmp > -1)
					{
						suffixes[ii] = symbol[ii].Right(symbol[ii].GetLength() - itmp - 1) ;
						symbol[ii] = symbol[ii].Left(itmp);
						symbol[ii].Trim();
					}

					srtssymbol[ii] = atoi(symbol[ii].Right(3));
				}
			}
		}

		SplitDouble(sList3, ";", lWidthR);
		SplitInt(sList4, ";", iVLine);
		SplitInt(sList5, ";", iHLine);
		SplitInt(sList6, ";", iDataFormat);
		SplitInt(sList7, ";", ifgcolor);

		for (int ii = 0; ii < icol; ii++)
			iorgfgcolor[ii] = ifgcolor[ii];
		SplitInt(sList8, ";", iTxtAlign);
		SplitInt(sList9, ";", iFontBold);
		SplitInt(sList10, ";", ibgcolor);
	};

	DrawList()
	{
		
	};
};
typedef CArray<DrawList, DrawList> CDrawListArray;


/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();

// Attributes
public:
	CString	m_strCode;

// Operations
public:
	void	SetParam(struct	_param* param, CWnd *parent);

protected:
	CWnd*	m_pMainWindow;
	CString	m_strImagePath;
	CFont*	m_pFont, *m_pBoldFont;
	BOOL	m_bRecv;
	CDrawListArray	m_arDrawList;
	char	m_key;
	CString	m_strParamName, m_strRealCode, m_strCodeType;

	int	m_iRowHeight, m_iLineColorIdx, m_iSymbolCnt;
	COLORREF m_clrLineColor, m_clrBgColor, m_clrDataFgColor, m_clrLblFgColor;
	COLORREF m_clrColor[10];
	CString	m_strRtsSymbol;
//	CMapStringToString	m_arRealCodeMap;
	CMap < int, int&, DWORD, DWORD& > m_arRealCodeMap;

	int	m_iVOutLine, m_iHOutLine;
	CString	m_strDateSplit, m_strTimeSplit;
	

// Operations
protected:

	COLORREF getAxColor(int color);
	HCURSOR	getAxCursor(int ikind);
	CFont*	getAxFont(CString fName, int point=9, bool bBold=false, bool bItalic=false);
	CFont*	getAxFont(struct _fontR* font);
	HBITMAP getBitmap(CString path, int idir);
	CPen*	getAxPen(COLORREF clr, int width, int style);

	CString Variant(int comm, CString data);
	LRESULT SendAxisMessage(WPARAM wParam, LPARAM lParam);
	void	SendTR(CString sCode);
	void	sendTR(CString trCode, CString sdatB, int istat);
	void	ParseRecvData(CString sData);
	void	ParseRealData(CString sData);
	void	ParseRealData(struct _alertR* alertR);

	void	Draw(CDC* pDC, CRect rcClient);
	void	ResizePanelBox();
	void	SetTotValue(BOOL bRedraw=FALSE);
	CString	SetDataFormat(CString sData, int iDataFormat, int iOrgFgColor, int& iFgcolor, CString& stype);
	int	GetTextWidth(CDC* pDC, CString sText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	LONG OnUser(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void Send(LPCTSTR sCode);
	afx_msg BSTR GetValue(LPCTSTR sCode);
	afx_msg void Clear();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};