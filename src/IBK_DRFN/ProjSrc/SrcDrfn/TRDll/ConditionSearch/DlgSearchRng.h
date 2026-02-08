#pragma once


// CDlgSearchDlg 대화 상자입니다.
#include "resource.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"

class CDlgSearchRng : public CDialog
{
	DECLARE_DYNAMIC(CDlgSearchRng)

public:
	CDlgSearchRng(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgSearchRng();

	void SetInit();
	CString m_stSavePath;// 화면번호 : KHD
// 대화 상자 데이터입니다.
	enum { IDD = IDD_SEARCHRNG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	CWnd*	m_pParentWnd;
	
	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnCancel;

	CStatic	m_ctrlTopRect;
	CStatic	m_ctrlBottomRect;
	CButton	m_btnTopGroup;
	CButton	m_btnBottonGroup;
	
	CStatic m_ctrlNotice;

	CButton	m_ctrlIssuesForAdministration;		// 관리종목
	CButton	m_ctrlInvestAttention;				// 투자주의
	CButton m_ctrlInvestWarning;				// 투자경고
	CButton	m_ctrlPreInvestRisk;				// 투자위험예고
	
	CButton	m_ctrlInvestRisk;					// 투자위험
	CButton	m_ctrlDealStop;						// 거래정지
	CButton	m_ctrlTidyDeal;						// 정리매매
	CButton m_ctrlAttentionInvestment;			// 투자유의 -> 투자주의환기

	CButton	m_ctrlInsincerityNotice;			// 불성실공시
	CButton m_ctrlPreferenceStock;				// 우선주

// 	CButton m_ctrlConfidencelimit;				// 신용제한종목 - new
	CButton m_ctrlOver;							// 이상급등

	char    sUnder[1];				/* 관리종목                     */
	char	sInvestAttention[1];	/* 투자주의						*/
	char	sInvestWarning[1];		/* 투자경고						*/
	char	sPreInvestRisk[1];		/* 투자위험예고					*/

	char	sInvestRisk[1];			/* 투자위험						*/
	char    sStop[1];               /* 거래정지종목                 */
	char    sJrmm[1];               /* 정리매매종목                 */
	char	sAttentionInvestment[1];/* 투자주의환기					*/
	
	char    sDishonest[1];          /* 불성실공시종목               */
	char    sUsjg[1];				/* 우선주   (0:해당없음,1:해당) */
    
    char    sDrat[3];               /* 증거금비율                   */
    char    sMrktalrm[3];           /* 시장경보
                                       1:투자주의, 2:투자경고
                                       3:투자위험, 4:투자위험예고	*/

	CButton	m_ctrlEquityCapital;				// 자본금
	CButton m_ctrlPriceRange;					// 가격대
	CButton	m_ctrlAggregateValueOfListedStocks;	// 시가총액
	CButton m_ctrlTradingVolume;				// 거래량
	CButton	m_ctrlParValue;						// 액면가

	CEdit	m_ctrlEquityCapitalAbove;					// 자본금 이상 조건
	CEdit	m_ctrlEquityCapitalBelow;					// 자본금 이하 조건
	CEdit	m_ctrlPriceRangeAbove;						// 가격대 이상 조건
	CEdit	m_ctrlPriceRangeBelow;						// 가격대 이하 조건
	CEdit	m_ctrlAggregateValueOfListedStocksAbove;	// 시가총액 이상 조건
	CEdit	m_ctrlAggregateValueOfListedStocksBelow;	// 시가총액	이하 조건
	CEdit	m_ctrlTradingVolumeAbove;					// 거래량 이상 조건
	CEdit	m_ctrlTradingVolumeBelow;					// 거래량 이하 조건
	CEdit	m_ctrlParValueAbove;						// 액면가 이상 조건
	CEdit	m_ctrlParValueBelow;						// 액면가 이하 조건

	CStatic	m_ctrlEquityCapitalAboveText;
	CStatic	m_ctrlEquityCapitalBelowText;
	CStatic	m_ctrlPriceRangeAboveText;					
	CStatic	m_ctrlPriceRangeBelowText;					
	CStatic	m_ctrlAggregateValueOfListedStocksAboveText;
	CStatic	m_ctrlAggregateValueOfListedStocksBelowText;
	CStatic	m_ctrlTradingVolumeAboveText;				
	CStatic	m_ctrlTradingVolumeBelowText;				
	CStatic	m_ctrlParValueAboveText;					
	CStatic	m_ctrlParValueBelowText;					

	CBrush	m_brush;
	CFont*	m_pFont;
	CFont*	m_pFontBold;

	void SetInit_Data();
	void SaveValue();
	BOOL VerificationValues();
	BOOL VerificationInput(UINT ID);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnCheckEquityCapital();				
	afx_msg void OnCheckPriceRange();					
	afx_msg void OnCheckAggregateValueOfListedStocks();	
	afx_msg void OnCheckTradingVolume();				
	afx_msg void OnCheckParValue();
	
	afx_msg void OnChangeEquityCapitalAbove();
	afx_msg void OnChangeEquityCapitalBelow();
	afx_msg void OnChangePriceRangeAbove();
	afx_msg void OnChangePriceRangeBelow();
	afx_msg void OnChangeOnCheckAggregateValueOfListedStocksAbove();
	afx_msg void OnChangeOnCheckAggregateValueOfListedStocksBelow();
	afx_msg void OnChangeTradingVolumeAbove();
	afx_msg void OnChangeTradingVolumeBelow();
	afx_msg void OnChangeParValueAbove();
	afx_msg void OnChangeParValueBelow();
};
