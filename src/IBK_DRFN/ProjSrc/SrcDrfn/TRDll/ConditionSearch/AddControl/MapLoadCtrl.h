#if !defined(AFX_MAPLOADCTRL_H__C21F6089_E2DD_44A2_882D_F258DBF11103__INCLUDED_)
#define AFX_MAPLOADCTRL_H__C21F6089_E2DD_44A2_882D_F258DBF11103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapLoadCtrl.h : header file
//

/************************************************************************/
/*																		*/
/*							Map Loader Control							*/
/*							(Mouse Control)								*/
/*																		*/
/*							작성일 : 2002. 10~							*/
/*						작성자 : 우동우 (TEXEN SYSTEM)					*/
/*																		*/
/************************************************************************/
//#
//#   # UPDATA
//#
//#			woo dong woo		2003. 2		: Condition File Loading


//# 삼성증권
//#include "../../../Runtime/CodeMaster/CodeMstInfo.h"	-hidden-
//#include "../../../Runtime/CodeMaster/MasterDefine.h"	-hidden-


#import "msxml.dll"
using namespace MSXML;

#include <afxtempl.h>   //#<= CList
#include "../DefineFiles/server_typedef.h"

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "TEditCtrlWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CMapLoadCtrl window
typedef struct tagControlInfo
{
	int nResourceID;
	CString	strControlType;
	double nLine;
} ControlInfo;


class CMapLoadCtrl : public CStatic
{
	// Construction
public:
	CMapLoadCtrl();
	virtual ~CMapLoadCtrl();
	
public:
	int m_nOneLineHeight;								//# Map One Line Height 
	CString					m_strAfterMapTag;
	int m_TopExp; //유진 

protected:

	CWnd*	m_pwndMain;
	CWnd*	m_pwndMsgTarget;
	BOOL m_bIndustry;
	int  m_nLeftAdd;  
    int  m_nTopAdd;
    int  m_nLineGapAdd;

	BOOL m_bSep; // 조건검색 or 기본검색

	CBrush m_brush;

  
	//## Condition Map Values ##
	CString				m_strAfterMapFileName;
	XMLTreeInfo			m_stNowXMLInfo;
		
	CList <ControlInfo, ControlInfo&>		m_listControlInfo;

	//## System ##
	COLORREF		m_clrSkin;
	CFont			m_fontDefault, m_fontBold;
	CBrush			m_brushDefault;

	//## Map Control ##
	CStatic*		m_pStatic;	//# STATIC 컨트롤
	CTEditCtrlWnd*	m_pEdit;		//# EDIT 컨트롤
	CComboBox*	m_pComboBox; //# COMBOBOX 컨트롤
	CButton*		m_pRadio;
	
	//## system Control ##
	CButton*		m_pNotCheckBtn;
	CButton*		m_pGubunCheckBtn;
	CStatic*		m_pConditionTitleStatic;
	CButton*		m_pRadioFinancial1;
	CButton*		m_pRadioFinancial2;
	
	CfxImgButton*	m_pUpdateBtn;
	CfxImgButton*	m_pOKBtn;
	CfxImgButton*	m_pBaseBtn;

	int				m_nAfterMapTagPos;
	CStringArray	m_saDelCtrlData;


	//## Map Loader Values ##
	MSXML::IXMLDOMDocumentPtr	m_domConditionMap;

	CStringArray m_saName_Kospi, m_saCode_Kospi;
	CStringArray m_saName_Kosdaq, m_saCode_Kosdaq;
	CStringArray m_saName_Member, m_saCode_Member;

	// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정
	CMap<CString,LPCSTR,CString,CString> m_mapRadioCheck;

public:
	void ChangeSkinColor(COLORREF clrBk);
	CString GetAppFileRoot();
	
	BOOL Create(CWnd* pParentWnd, UINT nID);
	BOOL SetInit(BOOL bIndustry);
	
	BOOL GetLoad_MarketInfo();

	void CreateUpdateBtn();
	void DestroyUpdateBtn();
	BOOL CheckUpdataBtnDrawState();
	
	void CreateOKBtn();
	void DestroyOKBtn();
	BOOL CheckOKBtnDrawState();
	
	void CreateBaseBtn();
	void DestroyBaseBtn();
	BOOL CheckBaseBtnDrawState();
	
	void CreateConditionTitleStatic();
	void DestroyConditionTitleStatic();
	void SetConditionTitleText(CString strConditionTitle);
	BOOL CheckConditionTitleStaticDrawState();
	
	void CreateNotCheckBtn(int nNotUser);
	void DestroyNotCheckBtn();
	BOOL CheckNotCheckBtnDrawState();
	void SetNotCheckValues(int nState);
	BOOL GetNotCheckValues();

	void CreateGubunCheckBtn(int nGubun, BOOL bEnable = TRUE);
	void DestroyGubunCheckBtn();
	BOOL CheckGubunCheckBtnDrawState();
	void SetGubunCheckValues(int nState);
	BOOL GetGubunCheckValues();
	BOOL CheckCondiGubunState(CString strTagID);
		
	void DestoryAllCtrl();
	
	//## Real Condition Map Drew ##
	BOOL SetLoadTagIDToXMLFile(CString strTagID);	//#<= Not User
	BOOL SetLoadXMLFile(CString strFileName);
	
	BOOL SetDrewConditionMap(XMLTreeInfo *pstXMLTreeInfo, int nGruop, BOOL bModifyBtnView = TRUE, BOOL bReDraw = FALSE, CString strElement = "");

	
	//## Data ##
	BOOL GetMapValuesToStruct(CString strTagID,
		User_Define &stResultUserDefine, 
		CString &strResultComposition,
		BOOL &bHaveTerm, int nGroup, CString strElement = "");

	void SetBase();

	void CreateFinancialRadioBtn();
	BOOL CheckFinacialRadioBtnDrawState();
	void DestroyFinancialRadioBtn();

  virtual CString GetFolderMap(){
    return (CString)FOLDER_MAP;
  }

  virtual void DestroyConditionItem(CString strCtrlType, int nResourceID){}

	virtual void SetDefultToMapValues(XMLTreeInfo *pstXMLTreeInfo, BOOL bOtherBtnSet = TRUE);

	BOOL GetChartDefultData(CString strConditionName, CStringArray &saDefultData);
	void SetChartDefultToMapValues(CString strTagID, CStringArray* psaDefultData);
	
	BOOL GetUserPattenData(CString strName, CStringArray &saUserPattenInfo);
	void SetUserPattenDefultToMapValues(CString strTagID, CStringArray* psaDefultData);

	
	void SetStructToMapValues(CString strTagID, User_Define* stUserDefine);
	
	//## Check ##
	BOOL CheckConditionMapDrewState();
	
	BOOL LoadConditionMapXMLFile(CString strFileName);


	void SetStructDataClear(User_Define &stUserDefine);
	
	void EuBtnUpdate();
	void EuBtnBase();
	CWnd* GetBtnWnd() {return m_pUpdateBtn;};
	CWnd* GetBaseBtnWnd() {return m_pBaseBtn;};
	void SetAddLeftPoint(int nLeftAdd) {m_nLeftAdd = nLeftAdd;};

protected:
	
	void DestroyConditionMap();
	
	//## Values Search ##
	CString GetTagToNodeData(CString strTagID, CString strNode);
	CString GetTagToControlNodeValues(CString strTagID, CString strNode, 
		int nNodeIndex);
	
	// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 >>
	void CMapLoadCtrl::SetRadioCheckState(CString strKey, CString strData, BOOL bChecked);
	CString CMapLoadCtrl::GetRadioCheckState(CString strKey);
	// 2012.4.17 박성경: 라디오 버튼 저장안되는 오류 수정 <<

	void OnSize_ConditionTitleStatic();
	
	// Overrides
	// ClassWizard generated virtual function overrides

	//{{AFX_VIRTUAL(CMapLoadCtrl)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CMapLoadCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	
	afx_msg void OnBtnUpdate();
	afx_msg void OnBtnOK();
	afx_msg void OnBtnBase();
	afx_msg void OnBtnNotCheck();
	afx_msg void OnBtnGubunCheck();
	afx_msg void OnBtnFinancial(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPLOADCTRL_H__C21F6089_E2DD_44A2_882D_F258DBF11103__INCLUDED_)
