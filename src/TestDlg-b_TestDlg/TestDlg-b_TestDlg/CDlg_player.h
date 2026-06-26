#pragma once


// CDlg_player 대화 상자

class CDlg_player : public CDialog
{
	DECLARE_DYNAMIC(CDlg_player)

public:
	CDlg_player(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_player();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PLAYER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
