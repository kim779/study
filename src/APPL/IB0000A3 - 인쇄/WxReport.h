// WxReport.h: interface for the CWxReport class.
//
//////////////////////////////////////////////////////////////////////

#pragma once


class CMainDlg;
class CWxReport  
{
public:
	CWxReport();
	virtual ~CWxReport();

	CRect		m_pDrawRect;
	CMainDlg*	m_pMain{};	
	CFont		m_pFont, m_pTitleFont, m_pHdrFont, m_pMultiLineFont;
	int		m_iBottomMargin{}, m_iSpaceHeight{};


	int		m_iLineHeight{}, m_iHdrLineHeight{};
	double		m_lscaley{}, m_lscalex{};
	void		SetDrawRect(CDC* pDC,  CRect rect);
	int		DrawTitle(CDC* pDC,  CRect rect, CString sTitle, int* irowcnt, Line* stleft, Line* stright);
	int		DrawGrid(CDC* pDC,  CRect rect, int icolcnt, int irowcnt, Line* stLine/*int* iTxtType, CString* sText*/);
	int		DrawListGrid(CDC* pDC,  CRect rect, int icolcnt, int iblrowcnt, Line* stLine, COutDataListArray& arDataList, int& inext);
	int		DrawTextLine(CDC* pDC,  CRect rect, CString sDtText, int iCnt, Line* stLine);
	int		DrawTextMultiLine(CDC* pDC, CRect rect, CString sDtText, int irowcnt, CStringArray& arLines, int& inext);
	void		DrawBox(CDC* pDC, CRect& rect);

	int			DrawPage(int& icontentidx, int ipage, CDC* pDC, CRect rect, int inext=0);
	void		DrawBottom(CDC* pDC, CRect rect, int itotpage, int ipage);
	void		DrawBackGround(CDC* pDC, CRect rect, CBitmap* hbitmap);
	CSize		GetBitmapSize(HBITMAP hBitmap);
	HANDLE		DDBToDIB(const CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);
	void		DrawRectangle(CDC* pDC, CRect rect);
	void		DrawBitmap(CString sImgpath, CDC* pDC, CRect rect);
	void		DrawBitmap(CBitmap* hbitmap, CDC* pDC, int ileft, int itop);



	



	int			GetTotPage(CRect rect);
	LOGFONT		GetLogFont(int fsize,  bool bold);
	void		SetDrawRect(const CRect& rect){m_pDrawRect =rect; };
	void		Print();
	int			DrawScreen(CDC* pDC, CRect rect, int idrawpage=0);
};
