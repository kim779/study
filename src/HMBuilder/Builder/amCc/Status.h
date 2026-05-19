#pragma once

// CStatus 대화 상자입니다.

#define	maxMSGs		4

class CStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CStatus)

public:
	CStatus(CString mapN, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CStatus();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COMPILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_step;
	CString m_error;
	CListCtrl m_list;
	CString m_mapN;

protected:
	CStatic*	m_bitmap[maxMSGs];
	CStatic*	m_msg[maxMSGs];
	bool		m_estate;
	CWnd*		m_parent;

public:
	void	SetRange(int lower, int upper);
	void	SetStep(int step);
	void	StepIt();
	void	SetMessage(int err, int kind, CString name, CString event,
		int message, CString param, int line);
	void	SetTotalError(int error);
	void	ShowBitmap(int idx = -1, bool show = false);

protected:
	void	showMessage(bool show = true);
	BOOL	addColumn(LPCTSTR str, int item, int width);
	BOOL	addItem(CString& str, int item, int subItem);
	CString	getFormKind(int kind = -1);
	CString	getMessage(int message, CString param);
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
