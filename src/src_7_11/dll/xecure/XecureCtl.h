
#pragma once

#include <afxmt.h>
#define BLOCK_SIZE 16

// XecureCtl.h : Declaration of the CXecureCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl : See XecureCtl.cpp for implementation.

__declspec(dllexport) char* WINAPI axEncrypt(char* src, char* keys, int mode);

class CXecureCtrl : public COleControl
{
	DECLARE_DYNCREATE(CXecureCtrl)

// Constructor
public:
	CXecureCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXecureCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
private:

	LEA_KEY m_key;
	
	enum	{encHELLO, encOK, encERR, encRUN} m_enc;

	int		m_nBytes;
	char		m_pBytes[BLOCK_SIZE*4096 + 1];
protected:
	~CXecureCtrl();

	bool makeHalfkey(unsigned char* pAppkey);

	DECLARE_OLECREATE_EX(CXecureCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CXecureCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CXecureCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CXecureCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CXecureCtrl)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CXecureCtrl)
	afx_msg BOOL Encrypt(long pBytes, long nBytes);
	afx_msg BOOL Decrypt(long pBytes, long nBytes);
	afx_msg long Xecure(long pBytes, long nBytes);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CXecureCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CXecureCtrl)
	dispidEncrypt = 1L,
	dispidDecrypt = 2L,
	dispidXecure = 3L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

