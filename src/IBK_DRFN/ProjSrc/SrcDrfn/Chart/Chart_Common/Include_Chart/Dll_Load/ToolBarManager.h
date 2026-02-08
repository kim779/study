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

	// TR/Real/RemovePacket등에 의해 Start/End Index를 계산하여 mainblock에 세팅한다.
	virtual void SetData(const int nType, const int nTotalDataCount, const int nPageDataCount, const int nGlanceBalanceMargin = 0) = 0;
	// '이전', '다음'버튼의 작업을 수행한다.
	virtual bool SetMoveScrollPage(bool bMove) = 0;
	// Slider의 최소값을 세팅한다.
	virtual void SetSliderMin(const int nSliderMin) = 0;
	// 데이타클래스들을 세팅한다.
	virtual void SetControlData(CPacketList* pPacketList, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList) = 0;
	// 그래프 타입을 콤보에 세팅한다.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex) = 0;
	// 선굵기 설정을 콤보에 세팅한다.
	virtual void SetLineThickness(int nThickness) = 0;
	// 단일지표여부를 세팅한다.
	virtual void SetOneChart(bool bOneChart) = 0;
	// 그래프 색상을 세팅한다.
	virtual void SetGraphColor(COLORREF clrGraphColor) = 0;
	// ScrollBox의 위치를 세팅한다.
	virtual void SetScrollPos(const int nPos) = 0;
	// ScrollBar에 start index와 end index를 세팅한다.
	virtual void SetStartEndIndex(int &nStart, int &nEnd) = 0;
	// 한 차트화면당 보여지는 데이타갯수가 변할때 호출한다.
	virtual void ChangeOnePageDataCount(const int nOnePageDataCount) = 0;

	// 툴바의 사이즈를 리턴한다. (OCX 크기 재조정하기 위함)
	virtual CRect GetToolBarRect() const = 0;
	// 데이타의 시작인덱스와 끝인덱스를 가져온다.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex) = 0;
	// MaxRange각을 가져온다.
	virtual int GetMaxRange() = 0;
	// 현재의 스크롤박스의 위치를 리턴한다.
	virtual int GetCurrentScrollPos() = 0;
	// 툴바Dialog의 윈도우핸들을 리턴한다.
	virtual HWND GetToolBarHwnd() = 0;
	virtual int	GetNumericalInquiryDlgType() const = 0;

	// 툴바를 다시그린다.
	virtual void OnDraw() = 0;
	// 스크롤바 사용유무를 리턴한다.
	virtual bool IsToolScrollBar() = 0;
	// 툴바에 해당 툴아이템이 있는지 확인한다.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// 툴바에 해당 툴아이템을 사용하는지 확인한다.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;
	// Drag에 의한 ZoomIn작업을 수행한다.
	virtual bool DragZoomIn(CPoint startPoint, CPoint endPoint, int& nStartIndex, int& nEndIndex, const CRect& rectBlock) = 0;
	// 툴버튼을 릴리즈 시킨다.
	virtual void ReleaseButton(CToolOptionInfo::TOOLOPTION enumToolOption) = 0;

	//---------------수치조회창관련
	virtual bool CreateNumericalInquiryDlg(const int nType = -1) = 0;		// 생성
	virtual bool DestroyNumericalInqiuryDlg() = 0;		// 삭제
	virtual bool IsUseNumericalInquiryDlg() = 0;		// 사용여부
	virtual void SetNumericalInquiryDlgMoveState(const short nMove) = 0;	// 수치조회창 이동/고정/마우스따라가기 설정.
	virtual void SetNumericalInquiryDlgBrush(COLORREF clrDlgBrush) = 0;	// 수치조회창 바탕색.
	virtual void SetNumericalInquiryDlgType(UINT nType) = 0;	// 수치조회창형태를 설정한다.
	virtual void OnMouseMove(CDC* pDC, CPoint point) = 0;			// 마우스이동시 수치조회창에 point위치의 데이타를 세팅, Dialog를 이동시킨다.
	// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in Numerical Inquiry Dialog
	virtual void	EnableHorizontalLineInNIDLG( BOOL p_bEnable = TRUE) = 0;

	//---------------툴팁
	virtual void CreateToolTip() = 0;	// 툴팁 생성.
	virtual bool ShowToolTip(CPoint point) = 0;	// 툴팁 보이기.
	virtual void HideToolTip() = 0;	// 툴팁 감추기.
};

class CToolBarManager : public IToolBarManager
{
public:
	CToolBarManager(CCommonInterface* pCommonInterface);
	virtual ~CToolBarManager();

	// 툴바를 생성한다.
	virtual bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile);
	// 툴바의 위치 지정
	virtual void ToolBarPosChanged(const CRect rectToolBarPos);

	// TR/Real/RemovePacket등에 의해 Start/End Index를 계산하여 mainblock에 세팅한다.
	virtual void SetData(const int nType, const int nTotalDataCount, const int nPageDataCount, const int nGlanceBalanceMargin = 0);
	// '이전', '다음'버튼의 작업을 수행한다.
	virtual bool SetMoveScrollPage(bool bMove);
	// Slider의 최소값을 세팅한다.
	virtual void SetSliderMin(const int nSliderMin);
	// 데이타클래스들을 세팅한다.
	virtual void SetControlData(CPacketList* pPacketList, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);
	// 그래프 타입을 콤보에 세팅한다.
	virtual void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
	// 선굵기 설정을 콤보에 세팅한다.
	virtual void SetLineThickness(int nThickness);
	// 단일지표여부를 세팅한다.
	virtual void SetOneChart(bool bOneChart);
	// 그래프 색상을 세팅한다.
	virtual void SetGraphColor(COLORREF clrGraphColor);
	// ScrollBox의 위치를 세팅한다.
	virtual void SetScrollPos(const int nPos);
	// ScrollBar에 start index와 end index를 세팅한다.
	virtual void SetStartEndIndex(int &nStart, int &nEnd);
	// 한 차트화면당 보여지는 데이타갯수가 변할때 호출한다.
	virtual void ChangeOnePageDataCount(const int nOnePageDataCount);

	// 툴바의 사이즈를 리턴한다. (OCX 크기 재조정하기 위함)
	virtual CRect GetToolBarRect() const;
	// 데이타의 시작인덱스와 끝인덱스를 가져온다.
	virtual bool GetStartEndIndex(int& nStartIndex, int& nEndIndex);
	// MaxRange각을 가져온다.
	virtual int GetMaxRange();
	// 현재의 스크롤박스의 위치를 리턴한다.
	virtual int GetCurrentScrollPos();
	// 툴바Dialog의 윈도우핸들을 리턴한다.
	virtual HWND GetToolBarHwnd();
	virtual int	GetNumericalInquiryDlgType() const;

	// 툴바를 다시그린다.
	virtual void OnDraw();
	// 스크롤바 사용유무를 리턴한다.
	virtual bool IsToolScrollBar();
	// 툴바에 해당 툴아이템이 있는지 확인한다.
	virtual BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	
	// 툴바에 해당 툴아이템을 사용하는지 확인한다.
	virtual BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);
	// Drag에 의한 ZoomIn작업을 수행한다.
	virtual bool DragZoomIn(CPoint startPoint, CPoint endPoint, int& nStartIndex, int& nEndIndex, const CRect& rectBlock);
	// 툴버튼을 릴리즈 시킨다.
	virtual void ReleaseButton(CToolOptionInfo::TOOLOPTION enumToolOption);

	//---------------수치조회창관련
	virtual bool CreateNumericalInquiryDlg(const int nType = -1);		// 생성
	virtual bool DestroyNumericalInqiuryDlg();		// 삭제
	virtual bool IsUseNumericalInquiryDlg();		// 사용여부
	virtual void SetNumericalInquiryDlgMoveState(const short nMove);	// 수치조회창 이동/고정/마우스따라가기 설정.
	virtual void SetNumericalInquiryDlgBrush(COLORREF clrDlgBrush);	// 수치조회창 바탕색.
	virtual void SetNumericalInquiryDlgType(UINT nType);	// 수치조회창형태를 설정한다.
	virtual void OnMouseMove(CDC* pDC, CPoint point);			// 마우스이동시 수치조회창에 point위치의 데이타를 세팅, Dialog를 이동시킨다.
	// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in Numerical Inquiry Dialog
	virtual void	EnableHorizontalLineInNIDLG( BOOL p_bEnable);

	//---------------툴팁
	virtual void CreateToolTip();	// 툴팁 생성.
	virtual bool ShowToolTip(CPoint point);	// 툴팁 보이기.
	virtual void HideToolTip();	// 툴팁 감추기.

private:
	CToolBarManagerImplementation* m_pManagerImpl;
};

#endif // !defined(AFX_TOOLBARMANAGER_H__235DAFB3_898C_471D_8156_332FEDFFC006__INCLUDED_)
