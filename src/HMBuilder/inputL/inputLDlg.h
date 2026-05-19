
// inputLDlg.h : 헤더 파일
//

#pragma once
#include "fxGrid.h"
#include "resource.h"
#include "afxwin.h"

// CinputLDlg 대화 상자
class CinputLDlg : public CDialogEx
{
// 생성입니다.
public:
	CinputLDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_INPUTL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	CfxGrid m_Grid;
	CString m_root;

// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CtlCombo;

private:
	bool WriteFile(CString sData);
	bool LoadFile(CString& sData);
	CString Parser(CString &srcstr, CString substr);
	int hexToInt(wchar_t* szHex);
public:
	afx_msg void OnDestroy();
};
