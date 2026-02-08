// ChartObjectWrapperHelper.h: interface for the CChartObjectWrapperHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTOBJECTWRAPPERHELPER_H__7075D85A_F0C6_4DC6_96ED_7244B0FB64B7__INCLUDED_)
#define AFX_CHARTOBJECTWRAPPERHELPER_H__7075D85A_F0C6_4DC6_96ED_7244B0FB64B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"

class CBlockIndex;
class CInsertingBlockHorzScaleData;
class CVerticalScaleGroupDataList;
class CMovingGraphVertScaleData;
class CHorizontalScalePtrDataList;
class CVerticalScalePtrDataList;
class CAnalysisToolSettingData;
class CTextPosition;
class CPenEnvironment;


typedef bool (WINAPI* fpInsertBlock)(CWnd* , CInsertingBlockHorzScaleData* );
typedef bool (WINAPI* fpMoveGraph)(CWnd* , const CVerticalScaleGroupDataList& , CMovingGraphVertScaleData& );
typedef bool (WINAPI* fpSetHorzScale)(CWnd* , CHorizontalScalePtrDataList* );
typedef bool (WINAPI* fpSetVertScale)(CWnd* , CVerticalScalePtrDataList* );
typedef bool (WINAPI* fpSetAnalysisTool)(CWnd* , const int, CTextPosition* , CPenEnvironment* );


class CChartObjectWrapperHelper : public CLoadDllLib
{
public:
	fpInsertBlock m_pfpInsertBlock;
	fpMoveGraph m_pfpMoveGraph;
	fpSetHorzScale m_pfpSetHorzScale;
	fpSetVertScale m_pfpSetVertScale;
	fpSetAnalysisTool m_pfpSetAnalysisTool;

	CChartObjectWrapperHelper(LPCSTR _szDll)
	{
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"ChartObjectWrapper.dll");
		else
			m_szDllName = _T("ChartObjectWrapper.dll");

		InitializeAllData();
		LoadLib();
	}

	virtual ~CChartObjectWrapperHelper()
	{
	}


	// block insert
	bool InsertBlock(CWnd* pParent, CInsertingBlockHorzScaleData* pInsertingHorzScaleData)
	{
		if(!m_hLib)
			return false;

		m_pfpInsertBlock = (fpInsertBlock)::GetProcAddress(m_hLib, "InsertBlock");
		if(m_pfpInsertBlock == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;
		}
		return m_pfpInsertBlock(pParent, pInsertingHorzScaleData);
	}

	// graph move
	bool MoveGraph(CWnd* pParent, const CVerticalScaleGroupDataList& vertScaleGroupDataList, CMovingGraphVertScaleData& movingVertScaleData)
	{
		if(!m_hLib)
			return false;

		m_pfpMoveGraph = (fpMoveGraph)::GetProcAddress(m_hLib, "MoveGraph");
		if(m_pfpMoveGraph == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;
		}
		return m_pfpMoveGraph(pParent, vertScaleGroupDataList, movingVertScaleData);
	}

	// horz scale 설정 Dlg
	bool SetHorzScale(CWnd* pParent, CHorizontalScalePtrDataList* pHorzScalePtrDataList)
	{
		if(!m_hLib)
			return false;

		m_pfpSetHorzScale = (fpSetHorzScale)::GetProcAddress(m_hLib, "SetHorzScale");
		if(m_pfpSetHorzScale == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;
		}
		return m_pfpSetHorzScale(pParent, pHorzScalePtrDataList);
	}

	// vert scale 설정 Dlg
	bool SetVertScale(CWnd* pParent, CVerticalScalePtrDataList* pVertScalePtrDataList)
	{
		if(!m_hLib)
			return false;

		m_pfpSetVertScale = (fpSetVertScale)::GetProcAddress(m_hLib, "SetVertScale");
		if(m_pfpSetVertScale == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;
		}
		return m_pfpSetVertScale(pParent, pVertScalePtrDataList);
	}

	// analysis tool 설정 Dlg
	bool SetAnalysisTool(CWnd* pParent, const int nToolType, CTextPosition* pTextPosition, CPenEnvironment* pPenEnvironmnet)
	{
		if(!m_hLib)
			return false;

		m_pfpSetAnalysisTool = (fpSetAnalysisTool)::GetProcAddress(m_hLib, "SetAnalysisTool");
		if(m_pfpSetAnalysisTool == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return false;
		}
		return m_pfpSetAnalysisTool(pParent, nToolType, pTextPosition, pPenEnvironmnet);
	}

private:
	void InitializeAllData()
	{
		m_pfpInsertBlock = NULL;
		m_pfpMoveGraph = NULL;
		m_pfpSetHorzScale = NULL;
		m_pfpSetVertScale = NULL;
		m_pfpSetAnalysisTool = NULL;
		MakeNull(m_pfpInsertBlock);
		MakeNull(m_pfpMoveGraph);
		MakeNull(m_pfpSetHorzScale);
		MakeNull(m_pfpSetVertScale);
		MakeNull(m_pfpSetAnalysisTool);
	}
};

#endif // !defined(AFX_CHARTOBJECTWRAPPERHELPER_H__7075D85A_F0C6_4DC6_96ED_7244B0FB64B7__INCLUDED_)
