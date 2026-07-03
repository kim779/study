#pragma once

// SockPropPage.h : CSockPropPage 속성 페이지 클래스의 선언입니다.


// CSockPropPage : 구현에 대해서는 SockPropPage.cpp을(를) 참조하세요.

class CSockPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CSockPropPage)
	DECLARE_OLECREATE_EX(CSockPropPage)

// 생성자입니다.
public:
	CSockPropPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_SOCK };

// 구현입니다.
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 메시지 맵입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

