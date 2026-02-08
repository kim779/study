#if !defined(AFX_LOGINDLG_H__A125437D_C7C6_4D73_B5C0_F15DE2896111__INCLUDED_)
#define AFX_LOGINDLG_H__A125437D_C7C6_4D73_B5C0_F15DE2896111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//****************************************************************************
// INCLUDE HAEDER
//****************************************************************************
#include "resource.h"
#include "../ToolKit/Utility/KeyHook/KeyHook.h"
#include "../ToolKit/Utility/FileBitmap/FileBitmap.h"
#include "../ToolKit/Utility/ILButton/ILButton.h"
#include "../ToolKit/SocketModule/Packet.h"

class CLoginDlg : public CDialog
{
public:
	HWND		m_hSocket;
	HWND		m_hSocketDlg;
	void*		m_pBuffer;
	int			m_nSetupType;
	int			m_nIDType;
	int			m_nUserGB;
	int			m_nMethod;
	int			m_nAuthSkip;
	int			m_nProgrameType;
	CString		m_szRootPath;
	CString		m_szIni;
	CString		m_szDat;
	CString		m_szLoginFile;
	CString		m_szCmd;
	CString		m_szRealID;
	CString		m_szMessage;
	BOOL		m_bVerCheckCompleted;
	BOOL		m_bUseCert;
	BOOL		m_bSiseOnly;
	BOOL		m_bAutoPass;
	BOOL		m_bIDSave;

	HICON		m_hIcon;	
	CFont		m_boldFont;
	KEYENTRY	m_entry;

	CFileBitmap m_LoginBackBitMap;
	CImageList  m_LoginBtnImgList;

public:

	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	CLoginDlg(HWND		hSockDlg, 
		      void*		pBuffer, 
			  void*		pUserInfo, 
			  LPCTSTR	szRootPath, 
			  int		nSetupType,
			  CWnd*		pParent=NULL);

	void			SetImeMode				(DWORD mode);
	void			OnConnect				();
	void			TestSocket				();
	void			TestMode				();
	void			SetConnectServer		();
	BOOL			LoadDlgData				();
	BOOL			CheckAndSaveUserinfo	();
	int				VersionCheck			();
	void			OnConnectVersion		();
	void			CreateSocket			();
	void			SetupUserFile			();
	void			WriteLoginInfo			();
	BOOL			ClientIPCheck			(CString szClientIP);
	void			LoginTRSend				(HWND hSocket);
	void			ShowEndMessage			(CString _szMsg);
	short			Send2Server				(HWND hSock, LPCSTR _szTr, LPVOID szData, int nLen);
	BOOL			RealSend				(HWND hWnd, LPVOID pData, int nLen);

	BOOL			CheckIPAuth				(char* szAuth);
	void			MakeUserData			(STLOGINSUCCESS* pLoginData, int nBufLen);
	void			InitControl				();

	void			LoadImages				();
	void			MoveCtrls				();

	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGINDIG };
	CString		m_szID;
	CString		m_szPassWd;
	CILButton	m_btnSetting;
	CILButton	m_btnConnect;
	CILButton	m_btnExit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg UINT OnNcHitTest( CPoint point );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LOGINDLG_H__A125437D_C7C6_4D73_B5C0_F15DE2896111__INCLUDED_)
