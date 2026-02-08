#if !defined(AFX_RCVSOCKET_H__C25ECC9D_4523_4C4F_A261_643E75387719__INCLUDED_)
#define AFX_RCVSOCKET_H__C25ECC9D_4523_4C4F_A261_643E75387719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RcvSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRcvSocket command target

class CSocketInterface;
class CRcvSocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CRcvSocket(CSocketInterface* pInterface);
	virtual ~CRcvSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRcvSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CRcvSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:

private:
	CSocketInterface*	m_pInterface;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RCVSOCKET_H__C25ECC9D_4523_4C4F_A261_643E75387719__INCLUDED_)
