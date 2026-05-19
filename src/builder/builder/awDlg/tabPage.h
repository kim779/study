#pragma once

#include "resource.h"
#include "../h/mapvar.h"

// CTabPage 대화 상자입니다.

class AFX_EXT_CLASS CTabPage : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPage)

public:
	CTabPage(CWnd* pParent = NULL)	{}  // 표준 생성자입니다.
	CTabPage(CWnd* pParent, class mapForm* mapForm, int index);
	virtual ~CTabPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_UTABITEM };

protected:
	int	m_sel;
	int	m_index;
	class mapForm*	m_mapH;
	CEdit*	m_text[MAX_TAB];
	CEdit*	m_ID[MAX_TAB];
	CButton* m_visible[MAX_TAB];

	void	setTextIndex(int id);
	void	setIDIndex(int id);
	void	setVisibleIndex(int id);
	CString parse(CString &src, CString sub);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSetFocusCaption1();
	afx_msg void OnSetFocusCaption2();
	afx_msg void OnSetFocusCaption3();
	afx_msg void OnSetFocusCaption4();
	afx_msg void OnSetFocusCaption5();
	afx_msg void OnSetFocusCaption6();
	afx_msg void OnSetFocusCaption7();
	afx_msg void OnSetFocusCaption8();
	afx_msg void OnSetFocusCaption9();
	afx_msg void OnSetFocusCaption10();
	afx_msg void OnSetFocusCaption11();
	afx_msg void OnSetFocusCaption12();
	afx_msg void OnSetFocusCaption13();
	afx_msg void OnSetFocusCaption14();
	afx_msg void OnSetFocusCaption15();
	afx_msg void OnSetFocusCaption16();
	afx_msg void OnSetFocusID1();
	afx_msg void OnSetFocusID2();
	afx_msg void OnSetFocusID3();
	afx_msg void OnSetFocusID4();
	afx_msg void OnSetFocusID5();
	afx_msg void OnSetFocusID6();
	afx_msg void OnSetFocusID7();
	afx_msg void OnSetFocusID8();
	afx_msg void OnSetFocusID9();
	afx_msg void OnSetFocusID10();
	afx_msg void OnSetFocusID11();
	afx_msg void OnSetFocusID12();
	afx_msg void OnSetFocusID13();
	afx_msg void OnSetFocusID14();
	afx_msg void OnSetFocusID15();
	afx_msg void OnSetFocusID16();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnCheck01();
	afx_msg void OnCheck02();
	afx_msg void OnCheck03();
	afx_msg void OnCheck04();
	afx_msg void OnCheck05();
	afx_msg void OnCheck06();
	afx_msg void OnCheck07();
	afx_msg void OnCheck08();
	afx_msg void OnCheck09();
	afx_msg void OnCheck10();
	afx_msg void OnCheck11();
	afx_msg void OnCheck12();
	afx_msg void OnCheck13();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	afx_msg void OnCheck16();
};
