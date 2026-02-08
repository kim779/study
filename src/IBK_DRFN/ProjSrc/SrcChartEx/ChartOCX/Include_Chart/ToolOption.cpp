// ToolOption.cpp: implementation of the CToolOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolOption.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()

#include "ObjTool.h"
#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToolOption::CToolOption(CString strToolOption)
{
	SetAll(strToolOption);
}

CToolOption::~CToolOption()
{
	DeleteAll();
}

void CToolOption::SetListToolOption(CTypedPtrList<CObList, CObjTool*> * plstToolOption, const CString &strOption)
{
	CString strT = strOption;

	while(!strT.IsEmpty())	{
		CString strObj = CDataConversion::GetStringData(strT, "\r\n");
		if(!strObj.IsEmpty() && strObj.Find(";") >= 0)	{
			CString strName = CDataConversion::GetStringData(strObj, ";");
			CString strOption1 = CDataConversion::GetStringData(strObj, ";");
			CString strOption2 = CDataConversion::GetStringData(strObj, ";");
			CObjTool * pObjTool = new CObjTool(strName, strOption1, strOption2);
			plstToolOption->AddTail(pObjTool);
		}
	}
}

void CToolOption::Reset(CString strToolOption)
{
	DeleteAll();
	SetAll(strToolOption);	
}

void CToolOption::DeleteAll()
{
	POSITION pos = lstToolInner.GetHeadPosition();
	while(pos){
		CObjTool * pToolInner = lstToolInner.GetNext(pos);
		delete pToolInner;
		pToolInner = NULL;
	}
	lstToolInner.RemoveAll();

	pos = lstToolOuter.GetHeadPosition();
	while(pos){
		CObjTool * pToolOut = lstToolOuter.GetNext(pos);
		delete pToolOut;
		pToolOut = NULL;
	}
	lstToolOuter.RemoveAll();

	pos = lstToolMenu.GetHeadPosition();
	while(pos){
		CObjTool * pToolMenu = lstToolMenu.GetNext(pos);
		delete pToolMenu;
		pToolMenu = NULL;
	}
	lstToolMenu.RemoveAll();
}

void CToolOption::SetAll(const CString &strToolbarOption)
{
	CString szToolbarOption = strToolbarOption;
	CString strInner = CDataConversion::GetStringData(szToolbarOption, CString( _MTEXT( C4_OUTER_TOOL_OPTION)) + "\r\n");
	SetListToolOption(&lstToolInner, strInner);

	// (2004.04.09, 배승원) 기본 수치조회창 기능관련하여, RButton ToolOption이 삭제되었다.
	//		이경우 "RButton"이란 Separator가 없을 수 있다. 이를 대비한다.
	CString strOut;
	if( 0 <= szToolbarOption.Find( "RButton"))
		strOut = CDataConversion::GetStringData(szToolbarOption, "RButton\r\n");
	else
	{
		strOut = szToolbarOption;
		szToolbarOption.Empty();
	}
	
	SetListToolOption(&lstToolOuter, strOut);
	CString strMenu = szToolbarOption;
	SetListToolOption(&lstToolMenu, strMenu);
}

CTypedPtrList<CObList, CObjTool*> * CToolOption::GetToolOptionList(CToolOptionInfo::OPTION nOption)
{
	switch(nOption)	{
	case CToolOptionInfo::INNER:
		return &lstToolInner;
		break;
	case CToolOptionInfo::OUTER:
		return &lstToolOuter;
		break;
	case CToolOptionInfo::MENU:
		return &lstToolMenu;
		break;
	default:
		return NULL;
		break;
	}	
}

BOOL CToolOption::UseTool(CToolOptionInfo::OPTION nOption, CToolOptionInfo::TOOLOPTION nToolType)
{
	CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(nOption);

	if(lstTool == NULL || lstTool->GetCount() <= 0)
		return FALSE;
	POSITION pos = lstTool->GetHeadPosition();
	while(pos)	{
		CObjTool* pObjTool = lstTool->GetNext(pos);
		if(pObjTool->GetType() == nToolType)
		//if(lstTool->GetNext(pos)->GetType() == nToolType)
			return TRUE;
	}
	return FALSE;
}

BOOL CToolOption::UseTool(CToolOptionInfo::OPTION nOption, const CString& strToolName)
{
	if(strToolName.IsEmpty())	return FALSE;
	CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(nOption);

	if(lstTool == NULL || lstTool->GetCount() <= 0)
		return FALSE;
	POSITION pos = lstTool->GetHeadPosition();
	while(pos)	{
		if(lstTool->GetNext(pos)->GetName() == strToolName)
			return TRUE;
	}
	return FALSE;
}

CObjTool * CToolOption::GetTool(CToolOptionInfo::TOOLOPTION nToolType)
{
	if(UseTool(CToolOptionInfo::INNER, nToolType))	{
		CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(CToolOptionInfo::INNER);
		if(lstTool && lstTool->GetCount() > 0)	{
			POSITION pos = lstTool->GetHeadPosition();
			while(pos)	{
				CObjTool * pTool = lstTool->GetNext(pos);
				if(pTool->GetType() == nToolType)
					return pTool;
			}
		}
	}
	if(UseTool(CToolOptionInfo::OUTER, nToolType))	{
		CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(CToolOptionInfo::OUTER);
		if(lstTool && lstTool->GetCount() > 0)	{
			POSITION pos = lstTool->GetHeadPosition();
			while(pos)	{
				CObjTool * pTool = lstTool->GetNext(pos);
				if(pTool->GetType() == nToolType)
					return pTool;
			}
		}
	}
	if(UseTool(CToolOptionInfo::MENU, nToolType))	{
		CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(CToolOptionInfo::MENU);
		if(lstTool && lstTool->GetCount() > 0)	{
			POSITION pos = lstTool->GetHeadPosition();
			while(pos)	{
				CObjTool * pTool = lstTool->GetNext(pos);
				if(pTool->GetType() == nToolType)
					return pTool;
			}
		}
	}

	return NULL;
}

CObjTool * CToolOption::GetTool(CToolOptionInfo::OPTION nOption, CToolOptionInfo::TOOLOPTION nToolType)
{
	if(nOption == CToolOptionInfo::INNER)	{
		CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(CToolOptionInfo::INNER);
		if(lstTool && lstTool->GetCount() > 0)	{
			POSITION pos = lstTool->GetHeadPosition();
			while(pos)	{
				CObjTool * pTool = lstTool->GetNext(pos);
				if(pTool->GetType() == nToolType)
					return pTool;
			}
		}
	}
	else if(nOption == CToolOptionInfo::OUTER)	{
		CTypedPtrList<CObList, CObjTool*> * lstTool = GetToolOptionList(CToolOptionInfo::OUTER);
		if(lstTool && lstTool->GetCount() > 0)	{
			POSITION pos = lstTool->GetHeadPosition();
			while(pos)	{
				CObjTool * pTool = lstTool->GetNext(pos);
				if(pTool->GetType() == nToolType)
					return pTool;
			}
		}
	}
	return NULL;
}
// 03.22.2002  기존 Tool List에 있는 Tool을 삭제 또는 변경 또는 추가한다
void CToolOption::SetTool(CToolOptionInfo::OPTION nOption, CToolOptionInfo::TOOLOPTION nToolType, CString strOption1, CString strOption2, BOOL bAdd)
{
	if(bAdd)	{
		CString strName = _MTEXT( m_eToolOptions[ nToolType]);		
		if(UseTool(nOption, nToolType))	{
			CObjTool * pObjTool = GetTool(nToolType);
			pObjTool->SetOption(strOption1, strOption2);
		}
		else	{
			CObjTool * pObjTool = new CObjTool(strName, strOption1, strOption2);
			GetToolOptionList(nOption)->AddTail(pObjTool);
		}
	}
	else	{
		if(!UseTool(nOption, nToolType))
			return;
		DeleteTool(GetToolOptionList(nOption), nToolType);
	}
}

// 03.22.2002  기존 Tool List에 있는 Tool을 삭제한다
BOOL CToolOption::DeleteTool(CTypedPtrList<CObList, CObjTool*> * plstToolOption, CToolOptionInfo::TOOLOPTION nToolType)
{
	POSITION pos = plstToolOption->GetHeadPosition();
	while(pos)	{
		CObjTool * pTool = plstToolOption->GetAt(pos);
		if(pTool->GetType() == nToolType)	{
			plstToolOption->RemoveAt(pos);
			delete pTool;
			pTool = NULL;
			return TRUE;
		}
		plstToolOption->GetNext(pos);
	}
	return FALSE;
}