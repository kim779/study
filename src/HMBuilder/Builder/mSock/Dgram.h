#if !defined(AFX_DGRAM_H__9EC1F0DC_3092_4C9E_81CE_091703ECC43B__INCLUDED_)
#define AFX_DGRAM_H__9EC1F0DC_3092_4C9E_81CE_091703ECC43B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dgram.h : header file
//
#include "../h/axisfm.h"

/////////////////////////////////////////////////////////////////////////////
// CDgram command target

class CDgram : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CDgram(class CWsh* rsm, class CWsh* wsh);
	virtual ~CDgram();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDgram)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDgram)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
public:
	bool	OpenDgram(int port);
	void	CloseDgram();
	bool	WriteData(char* ip, char* datB, int datL, BOOL route);

protected:
	class CWsh*	m_rsm;
	class CWsh*	m_wsh;
	int		m_port;
	CString		m_ip;

//#ifdef	_DEBUG
	class CLog*	m_log;
//#endif

private:
	int	m_frameL;
	char	m_frameB[L_fmH+maxIOs];		// send packet frame

	int	m_nBytes;
	char	m_pBytes[L_fmH+maxIOs];		// receive packet frame
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGRAM_H__9EC1F0DC_3092_4C9E_81CE_091703ECC43B__INCLUDED_)
