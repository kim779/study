// ExEdit.h: interface for the CExEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXEDIT_H__D6370A44_19DC_4797_8EDB_92FF270DE1A8__INCLUDED_)
#define AFX_EXEDIT_H__D6370A44_19DC_4797_8EDB_92FF270DE1A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderEdit;
class CExEdit : public CEdit  
{
	DECLARE_DYNAMIC(CExEdit)
// Construction
public:
	CExEdit(BOOL bUseDefSkin = FALSE);

// Attributes
protected:
	CLoaderEdit*		m_pLoader;

// Operations
public:
	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2005/11/16
	// Return		: 메모리 포인터나 long형 값
	// Paramter
	//		LPVOID lpAction : 메모리 포인터 (구조체, 클래스등등..)
	// Comments		: 정의된 함수이외에 작업을 해야 하는 경우 ActionControl 함수를
	//				  사용해 클래스 수정없이 프로젝트 진행 가능.
	//				  파라메터와 리턴값은 각 화면에서의 요구사항데로 정의 가능함
	//				  ActionControl을 사용해 추가된 사항은 추후 클래스 수정으로 함수가 추가되지만
	//				  원활한 프로젝트 진행을 위해 임시로 이 함수를 사용합니다.
	//-----------------------------------------------------------------------------	
	long		ActionControl(LPVOID lpAction);

	void	UseNumericType(BOOL bUse);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExEdit)
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXEDIT_H__D6370A44_19DC_4797_8EDB_92FF270DE1A8__INCLUDED_)
