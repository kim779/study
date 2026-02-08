// (2005/8/8 - Seung-Won, Bae) From Hanwha Platform Header
// SharedBase.h: interface for the CSharedBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHAREDBASE_H__1C69D7A2_34CC_4AB6_B7F9_D51EB6E03329__INCLUDED_)
#define AFX_SHAREDBASE_H__1C69D7A2_34CC_4AB6_B7F9_D51EB6E03329__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSharedBase  
{
public:
	HWND m_hCHild;
	HWND m_hView;
	HWND m_hCtrl;

public:
	virtual long ChangeSharedData(CString sShareName, BYTE* pData, int nLen, int nStock=0){ return 0l;};
	virtual long GetSharedData(CString sShareName, BYTE* pData, int& nLen, int& nStock){ return 0l;};
	
	CSharedBase()
	{
		m_hCHild = NULL;
		m_hView = NULL;
		m_hCtrl = NULL;
	}
};

// nShareTyp : 1=Set, 2=Get, 4=Start Or조합.
// nShareArea : 0=General, 1=free, 2=all
class CSharedItem
{
public:
	CString m_sharedName;

public:
	int m_nShareTyp;
	int m_nShareArea;
	int m_nSharedGroup;

	CSharedItem()
	{
		m_nShareTyp = -1;
		m_nShareArea = -1;
		m_nSharedGroup = -1;
		m_sharedName = "";
	}
};


#define DE_SHARDSET		1
#define DE_SHARDGET		2 
#define DE_SHARDSTART	4
#define DE_SHARDSKIP	8

#define DE_AREA_GENERAL 0  // 일반적 원칙에 따름.
#define DE_AREA_FREE    1  // 현재 액티브된 버츄얼 데스크에만 적용(예외)
#define DE_AREA_ALL		2  // 전체적으로 적용(예외)

// (2005/8/9 - Seung-Won, Bae) Share Area String (It is need for UI.)
#define DET_AREA_GENERAL	"General"
#define DET_AREA_FREE		"Free"
#define DET_AREA_ALL		"All"

#define AREA_ALL	0
#define AREA_VIEW	1
#define AREA_CHILD	2
#define AREA_GROUP	3
#define AREA_MAIN	4
#endif // !defined(AFX_SHAREDBASE_H__1C69D7A2_34CC_4AB6_B7F9_D51EB6E03329__INCLUDED_)
