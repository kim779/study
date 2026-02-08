// CommentData.cpp: implementation of the CCommentData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommentData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommentData::CCommentData()
{

}

CCommentData::~CCommentData()
{

}

void CCommentData::SetComment(LPCTSTR lpTitle,LPCTSTR lpComment)
{
	m_mapComment.SetAt(lpTitle,lpComment);
}

void CCommentData::Init()
{
	m_mapComment.RemoveAll();
}

CString CCommentData::GetComment(LPCTSTR lpTitle)
{
	CString strComment;
	m_mapComment.Lookup(lpTitle,strComment);
	return strComment;
}

CString CCommentData::GetComment(long nSize, LPCTSTR lpTitle, ...)
{
	CString strRetValue;
	CString strComment;
	BOOL bSetValue = FALSE;
	BOOL bRetValue = m_mapComment.Lookup(lpTitle,strComment);
	if(bRetValue)
	{
		strRetValue = strComment;
	}

	int i = 0, first = 0;
	LPCTSTR lpTitleInVaList = NULL;
	va_list marker;
	nSize--;
	va_start( marker, lpTitle );     /* Initialize variable arguments. */
	while( nSize > 0)
	{
		lpTitleInVaList = va_arg( marker, LPCTSTR);
		bRetValue = m_mapComment.Lookup(lpTitleInVaList,strComment);
		if(bRetValue)
		{
			if(!strRetValue.IsEmpty())
			{
				strRetValue += ',';
			}			
			strRetValue += strComment;
		}
		nSize--;
	}
	va_end( marker );              /* Reset variable arguments.      */

	return strRetValue;
}

void CCommentData::SetCommentByString(LPCTSTR lpComment)
{
	CString strInputData = lpComment;
	int nShifter = 0;
	int nPos = 0;
	CString strDataDivided;
	CString strTitle;
	CString strContents;	
	while(1)
	{
		strDataDivided = Parser(strInputData,&nShifter);		
		if(strDataDivided.IsEmpty()) break;
		nPos = strDataDivided.Find('=');
		if(nPos!=-1)
		{
			strTitle = strDataDivided.Left(nPos);
			strContents = strDataDivided.Mid(nPos+1);
		}
		else
		{
			continue;
		}
		m_mapComment.SetAt(strTitle,strContents);
	}
	
}


CString CCommentData::Parser(LPCTSTR lpInputData, int *pNShifter ) 
{
	if(!pNShifter) return "";
	CString strInputData;
	int nPosStart = *pNShifter;
	int nPosEnd = 0;
	strInputData = lpInputData;
	nPosEnd = strInputData.Find(',',nPosStart);
	if(nPosEnd==-1)
	{
		nPosEnd = strInputData.GetLength();
		if(nPosStart>=nPosEnd)
		{
			return "";
		}
	}
	*pNShifter = nPosEnd + 1;
	return strInputData.Mid(nPosStart,nPosEnd-nPosStart);
}
