// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__B7ACD87E_A480_4102_9AB9_5C85F1451AC2__INCLUDED_)
#define AFX_LOG_H__B7ACD87E_A480_4102_9AB9_5C85F1451AC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum	dirLog {dirSTR, dirENC, dirDEC};

class CLog  
{
public:
	CLog(char *logName);
	virtual ~CLog();

	void	Trace(char *msg, int len, dirLog dir);
private:

	FILE*	m_logDs;
	CString	m_fName;

	void	Parse(BYTE *msg, int len);

};

#endif // !defined(AFX_LOG_H__B7ACD87E_A480_4102_9AB9_5C85F1451AC2__INCLUDED_)
