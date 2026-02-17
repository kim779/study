#pragma once

// WizardPropPage.h : CWizardPropPage 속성 페이지 클래스의 선언입니다.


// CWizardPropPage : 구현에 대해서는 WizardPropPage.cpp을(를) 참조하세요.

class CWizardPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CWizardPropPage)
	DECLARE_OLECREATE_EX(CWizardPropPage)

// 생성자입니다.
public:
	CWizardPropPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_WIZARD };

// 구현입니다.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 메시지 맵입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

