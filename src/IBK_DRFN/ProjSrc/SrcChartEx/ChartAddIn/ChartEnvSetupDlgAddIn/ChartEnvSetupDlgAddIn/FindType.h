// FindType.h: interface for the CFindType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDTYPE_H__E862AB68_621A_4F1A_86BA_52BD52CB71D2__INCLUDED_)
#define AFX_FINDTYPE_H__E862AB68_621A_4F1A_86BA_52BD52CB71D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BaseData.h"


class CFindType  
{
public:
	static bool DoThisIndicatorUseOnePricePacket( const char *p_szIndicatorName);
	static CBaseData::LIMITHORIZONTALLINE FindLimitValueofHorizontalLine( const char *szGraphName);
	static CBaseData::DRAWPOSITIONTYPE FindDrawPositionType(const CString& strGraphName);
	static CBaseData::COLORLISTITEMTYPE FindColorItemText( const char *szGraphName);
	static CBaseData::TABITEMTYPE FindTabItemText( const char *p_szGraphName);
	static CBaseData::DLGTYPE FindDlgType( const char *p_szGraphName);

	static CBaseData::FILLTYPE FindFillTypeWhenOneSub_NoCondi(const CString& strGraphName);
	static CBaseData::FILLTYPE FindFillTypeWhenOneSub_ThreeCondi(const CString& strGraphName);
	static CBaseData::FILLTYPE FindFillTypeWhenOneSub_TwoCondi(const CString& strGraphName);
	static CBaseData::FILLTYPE FindFillTypeWhenOneSub_OneCondi(const CString& strGraphName);
	static CString FindStochastics(const CString& strGraphName);

};

#endif // !defined(AFX_FINDTYPE_H__E862AB68_621A_4F1A_86BA_52BD52CB71D2__INCLUDED_)
