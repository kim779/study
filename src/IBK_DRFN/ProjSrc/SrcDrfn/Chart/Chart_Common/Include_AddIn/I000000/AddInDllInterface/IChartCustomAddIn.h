// IChartCustomAddIn.h: interface for the IChartCustomAddIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICHARTCUSTOMADDIN_H__32B0A34F_2D5F_4B70_A70C_29C44DCB4187__INCLUDED_)
#define AFX_ICHARTCUSTOMADDIN_H__32B0A34F_2D5F_4B70_A70C_29C44DCB4187__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의! Chart OCX가 이용하는 이 DLL만의 Special Interface이다.
//		따라서 수정시 Chart OCX에게 Header를 전달하여야하며,
//		 특히 AddIn DLL마다 이 Interface Header를 같이 이용할 수 없다.
//			(다른 이름으로 재정의할 것)
//		Chart OCX에게 별도의 Interface를 제공할 필요가 없는 경우,
//		 이 Header를 삭제하며, CSiteCustomAddin::GetAddInCustomInterface()를 정의하지 않아
//		 CSiteCustomAddin::GetAddInCustomInterface()가 재정의되지 않도록 한다.
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartCustomAddIn : public IUnknown  
{
protected:
	virtual ~IChartCustomAddIn()	{}

// Temp Interface I	: TimeMark Interface ~ 현 버전에서만 제공하도록 하는 Interface이다.
public:
	// [04/10/11] TimeMark를 Clear시키는 Interface를 제공한다.
	virtual	void	OnClearTimeMark( void) = 0;
	// [04/10/11] TimeMark를 설정하는 Interface를 제공한다.
	virtual	void	OnSetTimeMark( const char *p_szMarkTime, BOOL p_bWithScroll) = 0;
	// [04/10/11] 시간공유의 수신을 허용하는 Flag을 두어 제한할 수 있도록 한다.
	//				발신은 제한하지 않는다.
	virtual void	OnEnableReceivingTimeShare(BOOL p_bEnable) = 0;

// Temp Interface II	: BoundMark Interface ~ 현 버전에서만 제공하도록 하는 Interface이다.
public:
	// [04/10/13] 상한/하한한 표시를 활성화시키는 Interface를 제공한다.
	virtual void	OnEnableBoundMark( BOOL p_bEnable) = 0;
	// [04/10/13] Get BoundMark Flag
	virtual BOOL	GetBoundMarkFlag( void)	= 0;
	//sy 2005.11.21. ->BoundMark Flag 가 설정되어 있는지의 유무(사용유무).
	virtual bool	IsExistenceBoundMark( void) = 0;

// Temp Interface III	: 사장되는 Option 기능의 Interface를 제공한다.
public:
	// [04/10/18] Image로 저장하는 Interface를 제공한다.
	virtual BOOL	OnSaveToImage( void) = 0;
};

#endif // !defined(AFX_ICHARTCUSTOMADDIN_H__32B0A34F_2D5F_4B70_A70C_29C44DCB4187__INCLUDED_)
