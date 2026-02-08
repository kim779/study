// ObjTool.h: interface for the CObjTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJTOOL_H__D8725AAA_C7A2_11D5_9371_0050FC28B229__INCLUDED_)
#define AFX_OBJTOOL_H__D8725AAA_C7A2_11D5_9371_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CObjTool : public CObject  
{
public:
	void SetOption(CString& strOption1, CString& strOption2)	{m_str1stOption = strOption1; m_str2ndOption = strOption2;}
	CString GetName()	{return m_strName;}
	CToolOptionInfo::TOOLOPTION GetType()	{return m_nType;}
	void GetOption(CString& strOption1, CString& strOption2)	{strOption1 = m_str1stOption; strOption2 = m_str2ndOption;}

	CObjTool(CString strName, CString strOption1, CString strOption2);
	virtual ~CObjTool();

private:
	int GetToolOptionIndex(const CString& strName);
	void SetType(const CString& strName);
	CString m_strName;
	CString m_str1stOption;
	CString m_str2ndOption;
	CToolOptionInfo::TOOLOPTION m_nType;
};

#endif // !defined(AFX_OBJTOOL_H__D8725AAA_C7A2_11D5_9371_0050FC28B229__INCLUDED_)
