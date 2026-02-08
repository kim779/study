// LogMan.h: interface for the CLogMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMAN_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_LOGMAN_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

class CLogMan
{
public:
	CLogMan();
	virtual ~CLogMan();

	BOOL				m_bLogging;
	CStringArray		m_arLogMsg;

	void	ResetLog()		{ m_arLogMsg.RemoveAll();	};


	void	AddLog(CString strMsg);
	void	SaveLogFile(CString strPath);

	void	SetLogging(BOOL bLogging)	{ m_bLogging = bLogging;	};

};

#endif // !defined(AFX_LOGMAN_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
