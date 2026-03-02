#pragma once
////////////////////////////////////////////////////////////////////////////
// CGroupWnd window

#include "BaseWnd.h"
#include <AFXTEMPL.H>
#include "sharemsg.h"
#include "../../h/interst.h"

class CGridWnd;

class CGroupWnd : public CBaseWnd
{
	// Construction
public:
	CGroupWnd(CWnd *pMainWnd);

	// Attributes
public:
	CString m_strWaveFile;
	BOOL m_bWaveApply;

	COLORREF m_clrKospi;
	COLORREF m_clrKosdaq;
	BOOL m_bMKClrApply;
	BOOL m_bPopupDlg;
	BOOL m_bDispCode;
	COLORREF m_clrbookMark;	   //북마크 색
	COLORREF m_clrbookMarkPen; //북마크펜 색

	int m_RecomGroup[30]; //추천종목 보이는 위치
	int m_iDragIn;	      // 드래그인으로 들어오는 카운트 횟수 집계

	bool m_bActiveArrange; //정렬 실행 여부
	bool m_bNewsNChart;    // 2011.12.02	KSJ
	bool m_bMargin;	       // 2011.12.05	KSJ
	bool m_bInfo;	       // 2011.12.05	KSJ
	bool m_bTicker;	       // 2011.12.12	KSJ

	// Operations
public:
	// Overrides

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupWnd)
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CGroupWnd();
	// Generated message map functions

public:
	int GetDragInCount() { return m_iDragIn; }
	void SetDragInCount(int data);
	void AddDragInCount();
	void SendUini();

	int GetGroupCount();
	void GetMinMaxWidth(int &nMinWidth, int &nMaxWidth);
	BOOL IsValidCode(CString strCode);
	int GetRowCount();
	int GetFontSize() { return m_fontsize; }
	BOOL GetCurrBold() { return m_bCurr; }
	BOOL GetNameBold() { return m_bName; }
	BOOL GetAllBold() { return m_bAllBold; }
	CString GetFontName() { return m_fontname; }
	CString GetLinkmap() { return m_linkMap; }

	char GetBkUse() { return m_chUse; }
	COLORREF GetBkColor1() { return m_bkColor1; }
	COLORREF GetBkColor2() { return m_bkColor2; }
	COLORREF GetRTMColor() { return m_rtmColor; }
	COLORREF GetBookMarkColor() { return m_clrbookMark; }
	COLORREF GetBookMarkPenColor() { return m_clrbookMarkPen; }
	COLORREF GetFgColor(int nIndex) { return m_fgColor[nIndex]; }
	int GetRtmAction() { return m_rtmAction; }
	int GetDblAction() { return m_dblAction; }
	int GetSelAction() { return m_selAction; }
	int GetOverLapAction() { return m_overlap; }
	int GetOverLapAlert() { return m_overlapAlert; }

	int GetLine() { return m_line; }
	int GetRowHeight() { return m_rowH; }
	void SetGridBkColor(COLORREF color);
	void AutoExpectOper(int param);
	void saveWhenGroupClosed(int index);

	int sumEachGroupCount();
	void testSaveFile();
	void testSaveFile(CString dt);
	void InitSetGroup(); //초기에 그룹 선택하게 하는 함수
	void RefreshGrid();
	std::pair<int, std::vector<std::shared_ptr<struct _intersx>>> WriteFileSumtoEachGroup(UINT curruntGroup);
	CString MakePacket(CString &code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""));
	void selectFirst();
	void saveGroupIndex(int index);
	void ReSetupGrid();
	void saveOverLap(int overlap);

	void SetEqualizerField(int param);

	CGridWnd *GetGridWndUpjn();
	void setIsRecommand(bool bFlag);
	bool GetIsRecommand();

	void saveRecommandGroup();

	int getRecommandCursel(int index) { return m_RecomGroup[index]; }
	void arrangeGroup(int type);
	int GetmnGroupCount() { return m_nGroup; }

	void checktheRecommand(); //지수 실시간 변동시 그리드윈도우에 업데이트 실시

	bool isModified(int kind);
	void UpdateFromTick(const std::vector<int>& slotIndices);
protected:
	//{{AFX_MSG(CGroupWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// virtual
	void OperInit();
	void OperDraw(CDC *pDC);
	void OperDestory();
	void OperResize(int cx, int cy);
	void RecvOper(int kind, CRecvData *rdata);
	//void RecvRTS(CRecvData *rdata);

public:						     	// 2012.02.13 KSJ 이벤트로 보내면 데이터가 깨져서 직접 메소드 호출함.
	void RecvRTSx(LPARAM lParam); 			// 2012.01.19 KSJ Alertx 추가
	void SelectOper();
	void UpdateDraw();

protected:
	void loadcfg();
	void loadfield();
	void savecfg();

	// members
	void SetArray(CString rates);
	void MakeGridRect(CRect rcFull);
	void CreateGrid();
	void InitGrid();
	void SendGrid(int nIndex, CGridData *sdata);
	void SendGrid(int nIndex, UINT kind);
	void SendUpjnGrid(CGridData *sdata);
	int OverOper(int nIndex, CGridData *sdata);
	void DestroyGrid();
	void DrawDivide(CDC *pDC);
	void DrawTitle(CDC *pDC);
	void DrawMove(CRect rect);
	int GroupPlus(BOOL bExpand);
	int GroupMinus(BOOL bExpand);
	void IniGroupDel(); // 2011.12.12 KSJ 마지막 그룹의 ini삭제
	int GetIndexDivide(CPoint pt);
	CRect GetArea(int nIndex);
	CRect GetMovedRect(int nIndex, int xPos);
	void ExpectOper(int param);

	void DrawHighlight(CDC *pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow);
	void DrawHigV(CDC *pDC, CRect rect, COLORREF clrLight, COLORREF clrShadow);
	int SetMovedRect(int nIndex, int xPos);

	
	int _httoi(const TCHAR *value);
	void CloseGrid(int nIndex);
	// sub utils
	int getTotalSize();
	void InvalidateDivide();

	void addGridWnd();     //그리드 메모리 재구성
	void AllGroupSave();   //그룹 추가 삭제전후에 저장하기
	void loadGroupCheck(); //체크된 그룹 설정 가져오기 2011.12.13
	// members
	bool m_isRecommand; //현재 추천종목을 보여주고 있는건지
	int m_cntGroup; //그룹 개수


	std::vector<CGridWnd*> _cacheGrid;

	int m_nGroup;
	CBrush *m_pBrush;
	COLORREF m_clrBack;
	CArray<int, int> m_arRATE;
	CArray<CRect, CRect> m_arRECT;
	CArray<CRect, CRect> m_arDIVIDE;
	CArray<CRect, CRect> m_arUpDIV;
	CArray<CRect, CRect> m_arDnDIV;
	BOOL m_bDivide;
	int m_xPos;
	int m_xPosMove;
	int m_nIndex;
	int m_nCurSel;
	CRect m_rcDIVIDE;
	CRect m_rcUpDIV;
	CRect m_rcDnDIV;
	//	CArray <class CGridWnd*, class CGridWnd*> m_GridWnd;
	std::array<std::unique_ptr<class CGridWnd>, MAX_GROUP> m_GridWnd;
	class CGridWnd *m_GridWndUpjn;
	std::unique_ptr<class DlgAutoSave> m_pdlgsave;

	CMapWordToPtr m_mapExpectCol;

	BOOL m_bCurr;
	int m_fontsize;
	CString m_fontname;
	CString m_linkMap;
	char m_chUse;
	COLORREF m_bkColor1;
	COLORREF m_bkColor2;
	COLORREF m_rtmColor;
	COLORREF m_fgColor[4];
	enum _rtmAction m_rtmAction;
	enum _dblAction m_dblAction;
	int m_selAction;

	int m_overlap;	    //중복등록
	int m_overlapAlert; //중복등록 경고창
	BOOL m_bAutoSave;   // 2016.12.09 KSJ 종료시 자동저장 체크

	int m_line;
	BOOL m_bMAP;
	CString m_szTITLE;
	CRect m_rcTitle;
	int m_rowH;

	// ADD PSH 20070917
	BOOL m_bName;
	CString m_tempCode;
	// END ADD

	BOOL m_bAllBold; // 2012.11.28 KSJ 전체 굵게

	void OnDestroySave();

public:
	BOOL IsNewDrop();
	BOOL m_bAutoCloseSave;
	const std::unordered_map<int, int>& getRSymbol();
	bool isCodeSymbol(CString code);

	std::map<CString, int> _symbol;
	std::map<int, CString> _GroupName;
	int m_iTime;

	// [hana] 메인에서 가상화면으로 옮겼을때... 
	bool _bVirtualDraw = true;

	void setVirtualDraw(bool bflag){
		_bVirtualDraw = bflag;
	}

	bool getVirtualDraw(){
		return _bVirtualDraw;
	}
	
	void initAlert();
	BOOL CheckGetRTSHoga();
};
