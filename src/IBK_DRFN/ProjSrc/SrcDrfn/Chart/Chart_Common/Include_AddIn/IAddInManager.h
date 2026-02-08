// IAddInManager.h: interface for the IAddInManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_)
#define AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	전 AddIn에서 이용되는 OCX측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 Version별 AddIn 관련 Module이 함께 수정되어 내려가는 것이 전제되어야 합니다.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, 배승원) Addin DLL에게 전달되어, Chart Ocx Interface Gate Way 기능을 제공한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IChartOcx050116;
interface IChartOcx050531;

interface IAddInManager : public IUnknown  
{
protected:
	virtual ~IAddInManager()	{}

// (2004.10.07, 배승원) Chart OCX의 Interface를 구하는 Interface를 제공한다.
public:
	virtual IChartOcx050116 *	GetIChartOcx050116( void)	= 0;
	virtual IChartOcx050531 *	GetIChartOcx050531( void)	= 0;
};

#endif // !defined(AFX_IADDINMANAGER_H__45A3C29A_7BFE_48D1_8565_984832A3CBC3__INCLUDED_)
