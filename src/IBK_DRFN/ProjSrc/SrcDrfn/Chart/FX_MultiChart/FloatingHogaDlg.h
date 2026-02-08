#if !defined(AFX_FLOATINGHOGADLG_H__21539FF9_3BC1_42EA_887C_7A7F9BAB708D__INCLUDED_)
#define AFX_FLOATINGHOGADLG_H__21539FF9_3BC1_42EA_887C_7A7F9BAB708D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatingHogaDlg.h : header file
//
#include "Resource.h"
#include "./define/ChartMsgDef.h"
#include "./control/RscDlg.h"
// By Jeon Yongbae  2008/07/09
//<<20100308_JS.Kim 솔로몬
//#include "../../../common/CommHeaderDef.h"
//>>

#include "../chart_common/PowLabel.h"

#include "../chart_common/Grid/Util.h"
//#include "../chart_common/Grid/GridCtrl.h"
#include "../chart_common/Grid/TR_drds/HogaData.h"
//#include "../chart_common/Grid/TR_drds/D49890.h"
#include "../../inc/ExGridCtrl.h"
#include "../../inc/ExOCXDLL.h"
#include "../chart_common/ILButton.h"

#include "../CommonTR/DrdsS31.h"
//<<20100308_JS.Kim 솔로몬
#include "../CommonTR/REAL_CAH1_GNF.h"
//>>
//#ifndef __DATA_UTIL_H
//	#include "../../COMMON_BASE\Drds\DataUtil.h"
//#endif

//#include "../../../DrMLib/DrMiddleLib.h"
//#include "../../COMMON_BASE/drds/DRDSDEF.h"

/////////////////////////////////////////////////////////////////////////////
// CFloatingHogaDlg dialog
//2006.08.08 {{ 6~10호가 데이터 제거. 5호가까지만 사용함}}
//////////////////////////////////////////////////////////////////////////
		
class CFloatingHogaDlg : public CRscDlg
{
// Construction
public:
	void SetMarkRealCurPrice(char *pData, int nSize); //현재가 표시...

	CFloatingHogaDlg(CWnd* pParent = NULL);   // standard constructor
	HBITMAP			m_hBitmap;
	
	CMap<WORD, WORD, CString, CString>	m_mapHogaReal;

	//2005. 07. 14 ===============================================================
	CString			m_strCode;
	CString			m_strOldCode;

	CExGridCtrl*		m_pGridCtrl;
	CRect			m_rectGrid;										// 그리드 윈도우 크기

	LOGFONT			m_logGridFont;
	CFont			m_GridFont;

	TenHogaData		m_10HogaData;				// 매도10 ~ 매수10 호가 저장
	CUtil			m_Util;											// 각종 Util 클래스

	CArray			<CHogaData*, CHogaData*>				m_HogaData;
	void			HogaMemoryFree();
	void			SendTrHoga();
	void			RecvHogaData(int nLength , char* pData);

	//<<20100308_JS.Kim 솔로몬
	//void			SetRealHogaData(char* pstEUREAL_KXH1);
	void			SetRealHogaData(char* pstREAL_CAH1);
	//>>
//	void			SetRealHogaData(long m_pRealData);
	void			SetRealHogaData_Sub(CString strHoga,/* CString strNumber, CString strChangeVol,*/ CString strPreClosePrice, BOOL bMaeDo);
	void			SetRealHogaData_Rem(CString strNumber, BOOL bMaeDo);
	int				CheckRealHogaDataWithinHogaData(CString strData, int *nIndex);
	
	void			DisplayClear_AllHogaData();
	void			SetHogaData(char *pData, int nSize, BOOL bGetBasicData=FALSE);
	void			SetHogaData_Sub(CString strHoga, CString strNumber, CString strPreClosePrice, BOOL bMaeDo);
	void			CompareNUpdateHogaData(int nRow, int nCol, CString strText, int nMask, COLORREF crFgClr);	
	COLORREF		GetHogaGridBkColor(int nHogaRange);// 호가에 해당하는 그리드 색상정보를 얻어 온다.


	void			InitGrid();
	void			InitGridRowCol();
	void			ChangeGrid_AllWidth();	
	void			InitGridFormat();

	void			ChangeGrid_RowHeight();
	void			ChangeGrid_ColumnWidth();

	BOOL			m_bExpandRight;									// 주문창 확장 / 축소
	BOOL			m_bExpandBottom;								// 10단, 5단 호가 확장 / 축소
	BOOL			m_bOpenCol;
	BOOL			m_bIsFirstLoad;									// 현재가형주문이 로드 되었는지
	BOOL			m_bIsShowReserveCol;							// 주문예약칼럼 보이기/감추기
	int				m_nStaticWidth[4];								// 주문구분, 수량, 단가, 총금액 스태틱 폭

	BOOL			m_bIsTenHoga;									// 10단계호가 <-> Full호가
	BOOL			m_bUseLimitHoga;								// 상하한을 조회
	int				m_nTotalRowCnt;									// 전체 열갯수
	int				m_nCurPriceRow;									// 현재가열
	int				m_nLowLimitRow;									// 하한가 열
	int				m_n5HogaSumRow;									// 5단계합 표시열
	int				m_nTotalSumRow;									// 전체합 표시열
	int				m_nExtraVolRow;									// 시간외잔량열
	
	int				m_nHogaRangeForColor;							// 호가색상 범위

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 가격, 수량
	CString			m_strCurPrice;									// 현재가
	CString			m_strPreClosePrice;								// 기준가(전일종가)
	int				m_nHighLimit;									// 상한가
	int				m_nLowLimit;									// 하한가
	CString			m_strPreVol;									// 전일거래량	JSJ_ITCastle_Add_040502
	CString			m_strSEqualPer;									// 동시간 대비 전일거래량
	int				m_nMaedoPosJango;								// 매도주문 가능수량
	int				m_nHogaRow;										// 호가 Row
	int				m_nStaticRow;									// Static Row
	int				m_nChartPriceRow;								// 차트에서 받은 가격의 해당호가열
	CString			m_strChartPrice;								// 차트에서 받은 가격
	BOOL			m_bMoveChartPriceRow;							// 차트에서 받은 가격인 경우 해당열로 스크롤이동
	char			m_chSign;


	// ---------------------------------------------------------------------------->>
	// (대비 -> 수익율)   ==> (등락 -> 등락율 -> 수익율)로 수정 
	//BOOL			m_bDaebi;							// 대비	 -> 삭제함				JSJ_ITCastle_040430
	int				m_nThreeStateDaebiType;
	enum			{ e_DAEBI = 0 , e_DAEBI_PER , e_PROFIT_PER };	// 등락 , 등락율 , 수익율
	//============================================================================


	BOOL	m_bIconState;
	CRect	m_dlgOriRect;
	BOOL	m_bCapTure;
	CWnd*	m_pMainDlg;
	// 매수/매도버튼 이미지
	CImageList		m_ImageListOrder;
// Dialog Data
	//{{AFX_DATA(CFloatingHogaDlg)
	enum { IDD = IDD_DLG_FLOATING };
	CPowLabel	m_staticGridFrame;
	CPowLabel	m_staticSymbol;
	CPowLabel	m_staitcTilte;	
	//CExDrawButton	m_ctrlButtonClose;
	CButton		m_ctrlButtonClose;
	CILButton	m_ctrlButtonSell;
	CILButton	m_ctrlButtonBuy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFloatingHogaDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFloatingHogaDlg)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);		
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonSell();
	afx_msg void OnButtonBuy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg long OnGetBroad(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	IDrdsLib* m_pDrdsLib;

	BOOL MakeDrdsConnectAdvise();
	BOOL MakeDrdsUnConnectUnAdvise();
	void OnCodeAdvise(LPCSTR strCode);
	void OnCodeUnAdvise(LPCSTR strCode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATINGHOGADLG_H__21539FF9_3BC1_42EA_887C_7A7F9BAB708D__INCLUDED_)
