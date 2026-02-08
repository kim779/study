// CommentData.h: interface for the CCommentData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMENTDATA_H__1D3ECD03_0CD8_410C_ACC8_903C3C523980__INCLUDED_)
#define AFX_COMMENTDATA_H__1D3ECD03_0CD8_410C_ACC8_903C3C523980__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCommentData  
{
public:
	CCommentData();
	virtual ~CCommentData();
	void Init();
	void SetCommentByString(LPCTSTR lpComment);
	void SetComment(LPCTSTR lpTitle,LPCTSTR lpComment);
	CString GetComment(long nSize, LPCTSTR lpTitle, ...);
	CString GetComment(LPCTSTR lpTitle);
private:
	CString Parser(LPCTSTR lpInputData, int *pNShifter );
	CMapStringToString m_mapComment;
};

#endif // !defined(AFX_COMMENTDATA_H__1D3ECD03_0CD8_410C_ACC8_903C3C523980__INCLUDED_)
