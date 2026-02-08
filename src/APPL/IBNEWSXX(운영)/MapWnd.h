#pragma once

// 파일로딩방식이 필요할 경우 주석처리하라!
#define USE_MEMORY_LOADING

#define IDC_BROWSER		(1000)
#define IDC_BTN_CURR	(1001)	
#define IDC_BTN_CHART	(1002)
#define IDC_BTN_MADO	(1003)
#define IDC_BTN_MASU	(1004)
#define IDC_BTN_PRINT	(1005)
#define IDC_CBN_FONT	(1006)
#define IDC_BTN_REFRESH	(1007)

#include <sys/stat.h>
#include "common.h"
#include "dataio.h"
#include "AxisExt.h"
#include "AxBrowser.h"
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;


class CSortStringArray : public CStringArray
{
public:
	void Sort()
	{
		BOOL bNotDone = TRUE;
		while (bNotDone)
		{
			bNotDone = FALSE;
			for(int pos = 0;pos < GetUpperBound();pos++)
				bNotDone |= CompareAndSwap(pos);
		}
	}
private:
	BOOL CompareAndSwap(int pos)
	{
		CString temp;
		const int posFirst = pos;
		const int posNext = pos + 1;
		
#if 1
		if (ElementAt(posFirst).GetLength() < ElementAt(posNext).GetLength())
		{
			CString tmp = (LPCSTR)ElementAt(posFirst);
			SetAt(posFirst, ElementAt(posNext));
			SetAt(posNext, tmp);
			return TRUE;
		}
#else
		if (GetAt(posFirst).CompareNoCase(GetAt(posNext)) > 0)
		{
			temp = GetAt(posFirst);
			SetAt(posFirst, GetAt(posNext));
			SetAt(posNext, temp);
			return TRUE;
			
		}
#endif
		return FALSE;
	}
};

struct CharFinder
{
public:
	string m_chs;
public:
	CharFinder(LPCSTR chs) : m_chs(chs) {}
	bool operator()(char val) { 
		return find(m_chs.begin(), m_chs.end(), val)!=m_chs.end();
	}
};

class CMapWnd : public CWnd, CAxisExt
{
public:
	CMapWnd(CWnd *pParent);

protected:
	DWORD m_tick;
	BOOL m_bShowCodeBtn;
	int m_fontsize;
	CString m_home, m_user, m_name, m_profile, m_date, m_code;
	CMapStringToString m_domino;
	HGLOBAL m_hGlobal;
	IStream *m_pStream;

protected:
	BOOL m_bIE6;
	CString GetIEVersion();

	static BOOL m_bDataFileLoad;
	static CMapStringToString m_codes, m_names, m_rest, m_rlst;
	static CSortStringArray m_snames;
	static CRect m_rcCurrent;
	static int m_iOpenCount;
	
protected:
	CRect m_rcConfig;
	CRect m_rcCtrl, m_rcBody;
	CBitmap m_hBackground, *m_hRndBmp[4], *m_hBtn2[3], *m_hBtn4[3], *m_hRefresh[3];
	std::unique_ptr< CComboBox> m_pCbFont;
	std::unique_ptr<CfxStatic> m_pStTitle{}, m_pStFont{};
	std::unique_ptr< CfxImgButton> m_pBtnCurr{}, m_pBtnChart{}, m_pBtnMado{}, m_pBtnMasu{}, m_pBtnPrint{};
	CAxBrowser *m_pBrowser;

protected:
	void ReqPIBONEWS(LPCSTR szKey);
	void ParsePIBONEWS(LPCSTR dat, int len);
	void ParseDomino(LPCSTR dat, int len);

protected:
	CString GetName(LPCSTR code);
	CString GetCode(LPCSTR name);
	void LoadCodeData();
	void LoadRestrictData();
	void ReadFileData(LPCSTR path, vector<char> &data);
	void CopyStringMap(CMapStringToString &dst, const CMapStringToString &src);
	void RenderContent(LPCSTR dat, int len);
	void ConvertLink(string &body);
	void AdjustFont(string &body);

protected:
	void CalcSize();
	void LoadConfig();
	void SaveConfig();
	void UpdateTitle();
	void LoadImage();
	void DrawRoundRectangle(CDC *pDC, CRect rc, COLORREF colBk);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void MakeBackground();
	void ChangeTheme();
	void Draw(CDC *pdc);
//	CfxImgButton* CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp );
	std::unique_ptr<CfxImgButton> CreateImgButton(LPCSTR caption, int nID, CBitmap** bmp);
	std::unique_ptr < CfxStatic > CreateStatic(LPCTSTR lpszCaption, COLORREF colBk, COLORREF colFg, COLORREF colBorder=0);
	void testSaveFile3(CString code, CString datB);

public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

public:
	virtual ~CMapWnd();

protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnEvtSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEvtMapn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEvtRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEvtTrigger(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFontChange();
	afx_msg void OnBtnCurr();
	afx_msg void OnBtnChart();
	afx_msg void OnBtnMado();
	afx_msg void OnBtnMasu();
	afx_msg void OnBtnPrint();

public:
	CString m_sHtmlFile{};
};

