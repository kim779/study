// ToolBarManager.h: interface for the ToolBarManager class.
// 툴바DLL의 인터페이스.  순수가상클래스(인터페이스)로 만들었으면 더 좋았을 것을... T_T
// 앗. 그리 만들면 COM이 되어버리는군. 음~ 이래서 COM나왔나보다.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
#define AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_

#include "../BlockBaseEnum.h"
#include "../BlockBaseData.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.05.25, 배승원) 내부 ToolBar를 보이고, 숨기는 Interface를 제공한다.
//		별도의 Flag을 두어, ToolBar를 OCX의 영역보다 아래쪽에 위치되도록 한다.
#define _OCX_INNERTOOLBAR_HEIGHT 17

class CToolBarManagerImplementation;
class CCommonInterface;
class CPacketList;
class CPacketListManager;
class CXScaleManager;
class CIndicatorList;
class CMainBlock;

interface IToolBarManager
{
// (2006/2/9 - Seung-Won, Bae) Destroy by OCX (Created in ChartToolBar.dll)
public:
	virtual ~IToolBarManager()	{}

public:
	// 툴바를 생성한다.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile) = 0;
	// 툴바의 위치 지정
	virtual void ToolBarPosChanged(const CRect rectToolBarPos) = 0;
	// Slider의 최소값을 세팅한다.
	virtual void SetSliderMin(const int nSliderMin) = 0;
	// 데이타클래스들을 세팅한다.
	virtual void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList) = 0;
	// 그래프 타입을 콤보에 세팅한다.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex) = 0;
	// 선굵기 설정을 콤보에 세팅한다.
	virtual void SetLineThickness(int nThickness) = 0;
	// 단일지표여부를 세팅한다.
	virtual void SetOneChart(bool bOneChart, bool p_bIsNoZoomChart) = 0;
	// 그래프 색상을 세팅한다.
	virtual void SetGraphColor(COLORREF clrGraphColor) = 0;

	// 툴바의 사이즈를 리턴한다. (OCX 크기 재조정하기 위함)
	virtual CRect GetToolBarRect() const = 0;
	// 데이타의 시작인덱스와 끝인덱스를 가져온다.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex) = 0;
	// MaxRange각을 가져온다.
	virtual int GetMaxRange() = 0;
	// 툴바Dialog의 윈도우핸들을 리턴한다.
	virtual HWND GetToolBarHwnd() = 0;

	// 툴바를 다시그린다.
	virtual void OnDraw() = 0;
	// 스크롤바 사용유무를 리턴한다.
	virtual bool IsToolScrollBar() = 0;
	// 툴바에 해당 툴아이템이 있는지 확인한다.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// 툴바에 해당 툴아이템을 사용하는지 확인한다.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// 툴버튼을 릴리즈 시킨다.
	virtual void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;

	//---------------수치조회창관련
	virtual void OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption) = 0;	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG

	// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn) = 0;

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView) = 0;
};

interface IChartCtrl;
class CToolBarManager : public IToolBarManager
{
public:
	CToolBarManager( IChartCtrl *p_pIChartCtrl);
	virtual ~CToolBarManager();

	// 툴바를 생성한다.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile);
	// 툴바의 위치 지정
	virtual void ToolBarPosChanged(const CRect rectToolBarPos);
	// Slider의 최소값을 세팅한다.
	virtual void SetSliderMin(const int nSliderMin);
	// 데이타클래스들을 세팅한다.
	virtual void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);
	// 그래프 타입을 콤보에 세팅한다.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
	// 선굵기 설정을 콤보에 세팅한다.
	virtual void SetLineThickness(int nThickness);
	// 단일지표여부를 세팅한다.
	virtual void SetOneChart(bool bOneChart, bool p_bIsNoZoomChart);
	// 그래프 색상을 세팅한다.
	virtual void SetGraphColor(COLORREF clrGraphColor);

	// 툴바의 사이즈를 리턴한다. (OCX 크기 재조정하기 위함)
	virtual CRect GetToolBarRect() const;
	// 데이타의 시작인덱스와 끝인덱스를 가져온다.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex);
	// MaxRange각을 가져온다.
	virtual int GetMaxRange();
	// 툴바Dialog의 윈도우핸들을 리턴한다.
	virtual HWND GetToolBarHwnd();

	// 툴바를 다시그린다.
	virtual void OnDraw();
	// 스크롤바 사용유무를 리턴한다.
	virtual bool IsToolScrollBar();
	// 툴바에 해당 툴아이템이 있는지 확인한다.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	
	// 툴바에 해당 툴아이템을 사용하는지 확인한다.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);
	// 툴버튼을 릴리즈 시킨다.
	virtual void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption);

	//---------------수치조회창관련
	virtual void OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption);	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG

	// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
	virtual BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView);

private:
	CToolBarManagerImplementation* m_pManagerImpl;
};

#endif // !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
