// ToolandRButtonItemMgr.h: interface for the CToolandRButtonItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLANDRBUTTONITEMMGR_H__FB99F7D4_2823_4045_8813_585A32E4484C__INCLUDED_)
#define AFX_TOOLANDRBUTTONITEMMGR_H__FB99F7D4_2823_4045_8813_585A32E4484C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemInfo;
class CToolandRButtonItemMgr  
{
public:
	CToolandRButtonItemMgr();
	virtual ~CToolandRButtonItemMgr();

	//Default Item정보를 수정한다.
	void ChangeItemInfo(const CString& strItmeName, LV_ITEM* plvItem);
	CItemInfo* ChangeItemInfo(const CString &strItemName, const CString &strLimit1, const CString &strLimit2);

	//strItemName에 해당하는 CItemInfo를 얻는다.
	CItemInfo* GetItemInfowithItemName(const CString& strItemName);
	//m_ToolRButtonItemList멤버변수에 모든 Item의 정보를 초기화한다.
	void Init();

private:
	void SetFirst_AnalyticalTool_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	enum {
		FIRST_SUBITEM,
		SECOND_SUBITEM
	};

	enum {
		EXTENSION_REDUCTION,
		NUMERICAL_INQUIRY = 2, //수치조회창
		VIEWDATA, //데이타 조회
		NUMERICAL_INQUIRY_DATAVIEW, //수치데이타 조회
		INDICATOR_SEQUNCE_INQUIRY = 13, //지표연속조회
		AUTO_LINE = 34, //자동추세선
		ANALYTICALTOOL = 35, //십자선
		STOCKPRICEPREDICTION = 64, //주가예측
		Raider = 76, //레이더 차트
	};

	//SubItem의 정보를 Setting한다.
	void SetSubItemInfo(const int nIndex, const CString& strItemName, CItemInfo* pItemInfo);

	//strItemName에 따라서 첫번째의 내용이 다르다.
	//다른 Data 내용을 Item에 따라서 Setting한다.
	void SetFirstSubItemInfo(const int nIndex, const CString& strItemName, CItemInfo* pItemInfo);
	void SetFirst_StockPrice_Prediction_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Auto_Line_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Indicator_Sequance_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Numerical_Inquiry_DataView_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_ViewData_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Numerical_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetFirst_Extension_Recuction_SubItem(const CString& strItemName, CItemInfo* pItemInfo);
	void SetFirst_Raider_SubItem(const CString &strItemName, CItemInfo *pItemInfo);

	//strItemName에 따라서 두번째의 내용이 다르다.
	//다른 Data 내용을 Item에 따라서 Setting한다.
	void SetSecondSubItemInfo(const int nIndex, const CString &strItemName, CItemInfo *pItemInfo);
	void SetSecond_Numerical_Inquiry_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetSecond_Extension_Recuction_SubItem(const CString &strItemName, CItemInfo *pItemInfo);
	void SetSecond_ViewData_SubItem(const CString &strItemName, CItemInfo *pItemInfo);

	//CItemInfo의 정보를 Control하는 List이다.
	CObList m_ToolRButtonItemList;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd)		{	m_hOcxWnd = p_hOcxWnd;	}
};

#endif // !defined(AFX_TOOLANDRBUTTONITEMMGR_H__FB99F7D4_2823_4045_8813_585A32E4484C__INCLUDED_)
