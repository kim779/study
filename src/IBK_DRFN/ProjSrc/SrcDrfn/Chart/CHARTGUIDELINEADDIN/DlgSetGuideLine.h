#if !defined(AFX_DLGSETGUIDELINE_H__566DA12E_84F8_41DE_8E1F_60BF51C2CC32__INCLUDED_)
#define AFX_DLGSETGUIDELINE_H__566DA12E_84F8_41DE_8E1F_60BF51C2CC32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./Control/BitmapItemComboBox.h"
#include "./Control/GraphData.h"
#include "./Control/ColorButton.h"
#include "./resource.h"
#ifndef __COMMON_DEF_H
	#include "./Client/commondef.h"
#endif


// DlgSetGuideLine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGuideLine dialog
//-----------------------------------------------------------------------------
// Author		: ITCNam(ITCastle, Nam)	Date :2005/1/27
// Comments		: 기준선 설정창 다이알로그
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------
static TCHAR* szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
		_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
		_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
		_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
		_T("24")
};


class  CSubGraphData;
class CDlgSetGuideLine : public CDialog
{
// Construction
public:	
	CDlgSetGuideLine(CWnd* pParent = NULL);   // standard constructor
	CConfigSet m_clConfigSet;
	CString m_strCfgFile;
	CString m_strFullAcct;
	CBrush  m_brGDrawColor;
	
		
	void AddDashToAccount(LPCSTR _lpIn, CString& szOut);
	void CreateEnvSetupFile();
	BOOL LoadCfgFile();
	BOOL SaveCfgFile();
	void AddLineType(CComboBox* pCombo, int Type);
	void Initial_ThicknessComboBox();
	void Initial_ColorBtn();
	void Initial_ChkBox();
	void Initial_AcctCombo();
	void Initial_Option();

// Dialog Data
	//{{AFX_DATA(CDlgSetGuideLine)
	enum { IDD = IDD_DLG_GLINE };
	BOOL	m_bNotDrawLine;					//가이드 라인을 그릴지.
	CComboBox	m_cmbAcct;
	CEdit	m_edt1YP;						// 이동평균 첫번째 입력값	 에디트
	CEdit	m_edt2YP;						// 이동평균 두번째 입력값	 에디트
	CEdit	m_edt3YP;						// 이동평균 세번째 입력값	 에디트
	CEdit	m_edt4YP;						// 이동평균 네번째 입력값	 에디트
	CEdit	m_edtPmmPD;						// 평균매입단가 + 입력값	 에디트
	CEdit	m_edtPmmMD;						// 평균매입단가 - 입력값	 에디트
	BOOL	m_bChkYSprice;					//전일가격 시가				 체크 버튼       
	BOOL	m_bChkYHprice;					//전일가격 고가				 체크 버튼       
	BOOL	m_bChkYLprice;					//전일가격 저가				 체크 버튼       
	BOOL	m_bChkYEprice;					//전일가격 종가				 체크 버튼       
	BOOL	m_bChkYSHL;						//전일가격 (시+고+저)/3		 체크 버튼               
	BOOL	m_bChkTSprice;					//당일가격 시가				 체크 버튼       
	BOOL	m_bChkTHprice;					//당일가격 고가				 체크 버튼       
	BOOL	m_bChkTLprice;					//당일가격 저가				 체크 버튼       
	BOOL	m_bChkTEprice;					//당일가격 종가				 체크 버튼       
	BOOL	m_bChkTHL;						//당일가격 (고+저)/2		 체크 버튼               
	BOOL	m_bChkTSHL;						//당일가격 (시+고+저)/3		 체크 버튼               
	BOOL	m_bChkTHLE;						//당일가격 (고+저+(종*2))/4  체크 버튼
	BOOL	m_bChkHighLimit;				//상한가					 체크 버튼               
	BOOL	m_bChkLowLimit;					//하한가					 체크 버튼               
	BOOL	m_bChkPivot2Rst;				//피봇 2차 저항				 체크 버튼               
	BOOL	m_bChkPivot1Rst;				//피봇 1차 저항				 체크 버튼               
	BOOL	m_bChkPivotGLine;				//피봇 기준선				 체크 버튼               
	BOOL	m_bChkPivot1GG;					//피봇 1차 지지				 체크 버튼       
	BOOL	m_bChkPivot2GG;					//피봇 2차 지지				 체크 버튼	
	BOOL	m_bChkDMKPreHPrice;				//Demark 예상고가			 체크 버튼               
	BOOL	m_bChkDMKGLine;					//Demark 기준선				 체크 버튼       
	BOOL	m_bChkDMKPreLPrice;				//Demark 예상저가			 체크 버튼	        
	BOOL	m_bChk2Pyung1;					//이동평균 첫번째 입력값	 체크 버튼               
	BOOL	m_bChk2Pyung2;					//이동평균 두번째 입력값	 체크 버튼               
	BOOL	m_bChk2Pyung3;					//이동평균 세번째 입력값	 체크 버튼               
	BOOL	m_bChk2Pyung4;					//이동평균 네번째 입력값	 체크 버튼	        
	BOOL	m_bChkPmmDanga;					//평균매입 단가				 체크 버튼       
	BOOL	m_bChkPmmPDanga;				//평균매입 단가 +n			 체크 버튼               
	BOOL	m_bChkPmmMDanga;				//평균매입 단가 -n			 체크 버튼  	
	CColorButton m_btnYSprice;				//전일가격 시가				라인 색 설정 버튼
	CColorButton m_btnYHprice;				//전일가격 고가				라인 색 설정 버튼
	CColorButton m_btnYLprice;				//전일가격 저가				라인 색 설정 버튼
	CColorButton m_btnYEprice;				//전일가격 종가				라인 색 설정 버튼
	CColorButton m_btnYSHL;					//전일가격 (시+고+저)/3		라인 색 설정 버튼
	CColorButton m_btnTSprice;				//당일가격 시가				라인 색 설정 버튼
	CColorButton m_btnTHprice;				//당일가격 고가				라인 색 설정 버튼
	CColorButton m_btnTLprice;				//당일가격 저가				라인 색 설정 버튼
	CColorButton m_btnTEprice;				//당일가격 종가				라인 색 설정 버튼
	CColorButton m_btnTHL;					//당일가격 (고+저)/2		라인 색 설정 버튼
	CColorButton m_btnTSHL;					//당일가격 (시+고+저)/3		라인 색 설정 버튼
	CColorButton m_btnTHLE;					//당일가격 (고+저+(종*2))/4 라인 색 설정 버튼
	CColorButton m_btnHighLimit;			//상한가					라인 색 설정 버튼
	CColorButton m_btnLowLimit;				//하한가					라인 색 설정 버튼
	CColorButton m_btnPivot2Rst;			//피봇 2차 저항				라인 색 설정 버튼 
	CColorButton m_btnPivot1Rst;			//피봇 1차 저항				라인 색 설정 버튼
	CColorButton m_btnPivotGLine;			//피봇 기준선				라인 색 설정 버튼
	CColorButton m_btnPivot1GG;				//피봇 1차 지지				라인 색 설정 버튼
	CColorButton m_btnPivot2GG;				//피봇 2차 지지				라인 색 설정 버튼	
	CColorButton m_btnDMKPreHPrice;			//Demark 예상고가			라인 색 설정 버튼
	CColorButton m_btnDMKGLine;				//Demark 기준선				라인 색 설정 버튼
	CColorButton m_btnDMKPreLPrice;			//Demark 예상저가			라인 색 설정 버튼	
	CColorButton m_btn2Pyung1;				//이동평균 첫번째 입력값	라인 색 설정 버튼
	CColorButton m_btn2Pyung2;				//이동평균 두번째 입력값	라인 색 설정 버튼
	CColorButton m_btn2Pyung3;				//이동평균 세번째 입력값	라인 색 설정 버튼
	CColorButton m_btn2Pyung4;				//이동평균 네번째 입력값	라인 색 설정 버튼	
	CColorButton m_btnPmmDanga;				//평균매입 단가				라인 색 설정 버튼
	CColorButton m_btnPmmPDanga;			//평균매입 단가 +n			라인 색 설정 버튼
	CColorButton m_btnPmmMDanga;			//평균매입 단가 -n			라인 색 설정 버튼
	CBitmapItemComboBox m_cmbYSprice;		//전일가격 시가				콤보
	CBitmapItemComboBox m_cmbYHprice;		//전일가격 고가				콤보
	CBitmapItemComboBox m_cmbYLprice;		//전일가격 저가				콤보
	CBitmapItemComboBox m_cmbYEprice;		//전일가격 종가				콤보
	CBitmapItemComboBox m_cmbYSHL;			//전일가격 (시+고+저)/3		콤보
	CBitmapItemComboBox m_cmbTSprice;		//당일가격 시가				콤보
	CBitmapItemComboBox m_cmbTHprice;		//당일가격 고가				콤보
	CBitmapItemComboBox m_cmbTLprice;		//당일가격 저가				콤보
	CBitmapItemComboBox m_cmbTEprice;		//당일가격 종가				콤보
	CBitmapItemComboBox m_cmbTHL;			//당일가격 (고+저)/2		콤보
	CBitmapItemComboBox m_cmbTSHL;			//당일가격 (시+고+저)/3		콤보
	CBitmapItemComboBox m_cmbTHLE;			//당일가격 (고+저+(종*2))/4 콤보	
	CBitmapItemComboBox m_cmbHighLimit;		//상한가					콤보
	CBitmapItemComboBox m_cmbLowLimit;		//하한가					콤보
	CBitmapItemComboBox m_cmbPivot2Rst;		// 피봇 2차 저항			콤보 
	CBitmapItemComboBox m_cmbPivot1Rst;		// 피봇 1차 저항			콤보
	CBitmapItemComboBox m_cmbPivotGLine;	// 피봇 기준선				콤보
	CBitmapItemComboBox m_cmbPivot1GG;		// 피봇 1차 지지			콤보
	CBitmapItemComboBox m_cmbPivot2GG;		// 피봇 2차 지지			콤보	
	CBitmapItemComboBox m_cmbDMKPreHPrice;	// Demark 예상고가			콤보
	CBitmapItemComboBox m_cmbDMKGLine;		// Demark 기준선			콤보
	CBitmapItemComboBox m_cmbDMKPreLPrice;	// Demark 예상저가			콤보	
	CBitmapItemComboBox m_cmb2Pyung1;		// 이동평균 첫번째 입력값	콤보
	CBitmapItemComboBox m_cmb2Pyung2;		// 이동평균 두번째 입력값	콤보
	CBitmapItemComboBox m_cmb2Pyung3;		// 이동평균 세번째 입력값	콤보
	CBitmapItemComboBox m_cmb2Pyung4;		// 이동평균 네번째 입력값	콤보	
	CBitmapItemComboBox m_cmbPmmDanga;		// 평균매입 단가			콤보
	CBitmapItemComboBox m_cmbPmmPDanga;		// 평균매입 단가 +n		콤보
	CBitmapItemComboBox m_cmbPmmMDanga;		// 평균매입 단가 -n		콤보	
	CComboBox			m_cmbAlign;			// 정보표시 (왼쪽/오른쪽/양쪽)
	BOOL	m_bChkName;						// 정보표시 : 이름	체크 버튼       
	BOOL	m_bChkValue;					// 정보표시 : 값	체크 버튼       
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetGuideLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetGuideLine)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnColor(UINT nID);
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETGUIDELINE_H__566DA12E_84F8_41DE_8E1F_60BF51C2CC32__INCLUDED_)
