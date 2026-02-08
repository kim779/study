// IAddInManagerImp.cpp: implementation of the IAddInManagerImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddInManager.h"
#include "AddInManagerImp.h"

#include "../Include_Chart/IChartCtrl.h"		// for IChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) Chart OCX의 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Common
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I000000/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll000000( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll000000::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		// (2006/6/22 - Seung-Won, Bae) Check DLL Name
		if( !m_bVCheck000000)
		{
			if( stricmp( p_szAddInModule, "ChartCommonAddIn.dll"))
				AfxMessageBox( "I000000 Interface was used in [" + CString( p_szAddInModule) + "] !");
			m_bVCheck000000 = TRUE;
		}
#endif
		return CIWAddInDll000000::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I000000/IWChartOcx.h"
IChartOCX000000 *CAddInManagerImp::GetIChartOCX000000( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX000000::CreateObject( m_pIChartCtrl, this);
}
// Addin분리 - onlyojt (20100305)
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I003531/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll003531( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll003531::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		// (2006/6/22 - Seung-Won, Bae) Check DLL Name
		if( !m_bVCheck003531)
		{
			if( stricmp( p_szAddInModule, "ChartOutsideCustomAddIn.dll"))
				AfxMessageBox( "I003531 Interface was used in [" + CString( p_szAddInModule) + "] !");
			m_bVCheck003531 = TRUE;
		}
#endif
		return CIWAddInDll003531::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I003531/IWChartOcx.h"
IChartOCX003531 *CAddInManagerImp::GetIChartOCX003531( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX003531::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I003721/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll003721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll003721::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		// (2006/6/22 - Seung-Won, Bae) Check DLL Name
		if( !m_bVCheck003721)
		{
			if( stricmp( p_szAddInModule, "FX_ChartGuideLineAddIn.dll"))
				AfxMessageBox( "I003721 Interface was used in [" + CString( p_szAddInModule) + "] !");
			m_bVCheck003531 = TRUE;
		}
#endif
		return CIWAddInDll003721::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I003721/IWChartOcx.h"
IChartOCX003721 *CAddInManagerImp::GetIChartOCX003721( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX003721::CreateObject( m_pIChartCtrl, this);
}

//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I133721/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll133721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll133721::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck133721)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartGuideLineAddIn.dll"))
				AfxMessageBox( "I133721 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
//			else if( m_pIChartCtrl->GetAddInSiteID() != 91)
//				AfxMessageBox( "I133721 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck133721 = TRUE;
		}
#endif
		return CIWAddInDll133721::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I133721/IWChartOcx.h"
IChartOCX133721 *CAddInManagerImp::GetIChartOCX133721( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX133721::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I333721/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll333721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll333721::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck333721)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartGuideLineAddIn.dll"))
				AfxMessageBox( "I333721 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
//			else if( m_pIChartCtrl->GetAddInSiteID() != 91)
//				AfxMessageBox( "I333721 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck333721 = TRUE;
		}
#endif
		return CIWAddInDll333721::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I333721/IWChartOcx.h"
IChartOCX333721 *CAddInManagerImp::GetIChartOCX333721( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX333721::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I363721/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll363721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll363721::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck363721)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartGuideLineAddIn.dll"))
				AfxMessageBox( "I363721 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
//			else if( m_pIChartCtrl->GetAddInSiteID() != 91)
//				AfxMessageBox( "I363721 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck363721 = TRUE;
		}
#endif
		return CIWAddInDll363721::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I363721/IWChartOcx.h"
IChartOCX363721 *CAddInManagerImp::GetIChartOCX363721( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX363721::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I503721/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll503721( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll503721::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck503721)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartGuideLineAddIn.dll"))
				AfxMessageBox( "I503721 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
//			else if( m_pIChartCtrl->GetAddInSiteID() != 91)
//				AfxMessageBox( "I503721 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck503721 = TRUE;
		}
#endif
		return CIWAddInDll503721::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I503721/IWChartOcx.h"
IChartOCX503721 *CAddInManagerImp::GetIChartOCX503721( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX503721::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I423611/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll423611( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll423611::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck423611)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartPatternAddIn.dll"))
				AfxMessageBox( "I423611 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
//			else if( m_pIChartCtrl->GetAddInSiteID() != 42)
//				AfxMessageBox( "I423611 Interface was used in other Site!\r\nIt is only for DongBu!");
			m_bVCheck423611 = TRUE;
		}
#endif
		return CIWAddInDll423611::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I423611/IWChartOcx.h"
IChartOCX423611 *CAddInManagerImp::GetIChartOCX423611( void)
{
	if( !m_pIChartCtrl) return NULL;
	IChartOCX423611 *pIChartOCX423611 = new CIWChartOCX423611( m_pIChartCtrl, this);
	pIChartOCX423611->AddRef();
	return pIChartOCX423611;
}

//////////////////////////////////////////////////////////////////////
// Koscom
//////////////////////////////////////////////////////////////////////

#ifdef _AIM_KOSCOM
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I133101/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll133101( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll133101::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		// (2006/6/22 - Seung-Won, Bae) Check DLL Name
		if( !m_bVCheck133101)
		{
			if( stricmp( p_szAddInModule, "ChartAnalysisToolAddIn.dll"))
				AfxMessageBox( "I133101 Interface was used in [" + CString( p_szAddInModule) + "] !");
			m_bVCheck133101 = TRUE;
		}
#endif
		return CIWAddInDll133101::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I133101/IWChartOcx.h"
IChartOCX133101 *CAddInManagerImp::GetIChartOCX133101( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX133101::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I134221/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll134221( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll134221::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		// (2006/6/22 - Seung-Won, Bae) Check DLL Name
		if( !m_bVCheck134221)
		{
			if( stricmp( p_szAddInModule, "ChartDataListViewerAddIn.dll"))
				AfxMessageBox( "I134221 Interface was used in [" + CString( p_szAddInModule) + "] !");
			m_bVCheck134221 = TRUE;
		}
#endif
		return CIWAddInDll134221::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I134221/IWChartOcx.h"
IChartOCX134221 *CAddInManagerImp::GetIChartOCX134221( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX134221::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I134661/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll134661( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll134661::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck134661)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartDesignPropertyAddIn.dll"))
				AfxMessageBox( "I134661 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I134661 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck134661 = TRUE;
		}
#endif
		return CIWAddInDll134661::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I134661/IWChartOcx.h"
IChartOCX134661 *CAddInManagerImp::GetIChartOCX134661( void)
{
	if( !m_pIChartCtrl) return NULL;
	IChartOCX134661 *pIChartOCX134661 = new CIWChartOCX134661( m_pIChartCtrl, this);
	pIChartOCX134661->AddRef();
	return pIChartOCX134661;
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I135914/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll135914( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll135914::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck135914)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartEnvSetupDlgAddIn.dll"))
				AfxMessageBox( "I135914 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I135914 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck135914 = TRUE;
		}
#endif
		return CIWAddInDll135914::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I135914/IWChartOcx.h"
IChartOCX135914 *CAddInManagerImp::GetIChartOCX135914( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX135914::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I139092/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll139092( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll139092::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck139092)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartSTIndexAddIn.dll"))
				AfxMessageBox( "I139092 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I133611 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck139092 = TRUE;
		}
#endif
		return CIWAddInDll139092::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I139092/IWChartOcx.h"
IChartOCX139092 *CAddInManagerImp::GetIChartOCX139092( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX139092::CreateObject( m_pIChartCtrl, this);;
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I133691/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll133691( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll133691::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck133691)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartPeriodSummaryAddIn.dll"))
				AfxMessageBox( "I133691 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I133691 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck133691 = TRUE;
		}
#endif
		return CIWAddInDll133691::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I133691/IWChartOcx.h"
IChartOCX133691 *CAddInManagerImp::GetIChartOCX133691( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX133691::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I381931/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll381931( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll381931::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck381931)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartPeriodSummaryAddIn.dll"))
				AfxMessageBox( "I381931 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I381931 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck381931 = TRUE;
		}
#endif
		return CIWAddInDll381931::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I381931/IWChartOcx.h"
IChartOCX381931 *CAddInManagerImp::GetIChartOCX381931( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX381931::CreateObject( m_pIChartCtrl, this);
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I339092/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll339092( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll339092::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck339092)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartSTIndexAddIn.dll"))
				AfxMessageBox( "I339092 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I133611 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck339092 = TRUE;
		}
#endif
		return CIWAddInDll339092::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I339092/IWChartOcx.h"
IChartOCX339092 *CAddInManagerImp::GetIChartOCX339092( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX339092::CreateObject( m_pIChartCtrl, this);;
}
//////////////////////////////////////////////////////////////////////
#undef AFX_ADDINVER_H__32872A69_D935_499F_A617_204B2F00C997__INCLUDED_
#undef AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_
#undef AFX_ICHARTOCX_H__4845EC0A_5ED2_4C08_8ADD_CD7C93EAEFB9__INCLUDED_
#undef AFX_IWADDINDLL_H__65094346_094B_47EE_B618_FD0F5DC225F8__INCLUDED_
#undef AFX_IWCHARTOCX_H__0F4DE5F9_6C69_4321_AB82_ADD6CBF7D475__INCLUDED_
#undef AFX_ICHARTADDIN_H__1628D869_4373_4A7A_85B3_04D9026A7CE5__INCLUDED_
#include "./include_Addin/I509092/IWAddInDll.h"
IAddInDll *CAddInManagerImp::GetCIWAddInDll509092( const char *p_szAddInModule, UINT p_uAddInVersion, HMODULE p_hAddIn)
{
	if( CIWAddInDll509092::IVERSION == p_uAddInVersion)
	{
#ifdef _DEBUG
		if( !m_bVCheck509092)
		{
			// (2006/6/22 - Seung-Won, Bae) Check DLL Name
			if( stricmp( p_szAddInModule, "ChartSTIndexAddIn.dll"))
				AfxMessageBox( "I509092 Interface was used in [" + CString( p_szAddInModule) + "] !");
			// (2006/8/7 - Seung-Won, Bae) Check Site Number
			else if( m_pIChartCtrl->GetAddInSiteID() != 13)
				AfxMessageBox( "I133611 Interface was used in other Site!\r\nIt is only for Koscom!");
			m_bVCheck509092 = TRUE;
		}
#endif
		return CIWAddInDll509092::CreateObject( p_szAddInModule, p_hAddIn, m_pIChartCtrl);
	}
	return NULL;
}
#include "./include_Addin/I509092/IWChartOcx.h"
IChartOCX509092 *CAddInManagerImp::GetIChartOCX509092( void)
{
	if( !m_pIChartCtrl) return NULL;
	return CIWChartOCX509092::CreateObject( m_pIChartCtrl, this);;
}
#endif
