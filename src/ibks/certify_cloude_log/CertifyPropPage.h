#pragma once

// CertifyPropPage.h : CCertifyPropPage 속성 페이지 클래스의 선언입니다.


// CCertifyPropPage : 구현에 대해서는 CertifyPropPage.cpp을(를) 참조하세요.

class CCertifyPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCertifyPropPage)
	DECLARE_OLECREATE_EX(CCertifyPropPage)

// 생성자입니다.
public:
	CCertifyPropPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_CERTIFY };

// 구현입니다.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 메시지 맵입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

