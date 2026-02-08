// ToolOption.h: interface for the CToolOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLOPTION_H__D8725AA2_C7A2_11D5_9371_0050FC28B229__INCLUDED_)
#define AFX_TOOLOPTION_H__D8725AA2_C7A2_11D5_9371_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "BlockBaseData.h"

class CObjTool;

class CToolOption  
{
public:
	void SetTool(CToolOptionInfo::OPTION nOption, 
				 CToolOptionInfo::TOOLOPTION nToolType, 
				 CString strOption1, 
				 CString strOption2, 
				 BOOL bAdd);
	CObjTool * GetTool(CToolOptionInfo::TOOLOPTION nToolType);
	CObjTool * GetTool(CToolOptionInfo::OPTION nOption, CToolOptionInfo::TOOLOPTION nToolType);
	BOOL UseTool(CToolOptionInfo::OPTION nOption, CToolOptionInfo::TOOLOPTION nToolType);
	BOOL UseTool(CToolOptionInfo::OPTION nOption, const CString& strToolName);
	CTypedPtrList<CObList, CObjTool*> * GetToolOptionList(CToolOptionInfo::OPTION nOption);
	void Reset(CString strToolbarOption);
	CToolOption(CString strToolarOption);
	virtual ~CToolOption();	

private:
	BOOL DeleteTool(CTypedPtrList<CObList, CObjTool*> * plstToolOption, CToolOptionInfo::TOOLOPTION nToolType);
	void SetAll(const CString& strToolOption);
	void DeleteAll();
	void SetListToolOption(CTypedPtrList<CObList, CObjTool*> * plstToolOption, const CString& strOption);
	CTypedPtrList<CObList, CObjTool*> lstToolInner;
	CTypedPtrList<CObList, CObjTool*> lstToolOuter;
	CTypedPtrList<CObList, CObjTool*> lstToolMenu;
};

#endif // !defined(AFX_TOOLOPTION_H__D8725AA2_C7A2_11D5_9371_0050FC28B229__INCLUDED_)
