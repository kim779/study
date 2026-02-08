#ifndef __NEOCOMPONENT__RESIZE__CONTROL__FONT__HEADER__
#define __NEOCOMPONENT__RESIZE__CONTROL__FONT__HEADER__

// #include "../include/NeoRezFont.h"

// ###############################################################################
// * START : 2001. 01. 30 늦은밤.. 이준옥 (주)두리정보통신 기술연구소
// * 데스크에서 사용하는 폰트는 현재 굴림체 9를 기준으로 한다.
// * 폰트의 변경은 작게, 더작게, 보통, 크게, 더크게 의 5단계를 갖는다.
// *                             9     11     13
// * 스텝의 정보는 필요에따라 Data/NeoDesk.cfg 에 지정한다.
// * 
// * 처리과정
// *    1. 컨트롤생성후 "RMSG_FORMINITIALCOMPLETED" 파라미터(0, 0) 메시지를 받을때
// *       현상태를 기본으로 하여 정보를 저장한다.
// *    2. 메인에서 폰트를 바꾸거나 데스크에서 폰트를 변경할 필요가 있을 때 
// *       데스크에서 "RMSG_FORMSTATECHANGE" 메시지를 보낸다.
// *       WPARAM :
// *	   LPARAM : STNEOFONTINFO 구조체 포인터
// *
// * 추가되는 내용 및 설명은 아래에 추가하기 바랍니다.
// ###############################################################################

typedef struct {
	char sFontName[20+1];
	int	 nFontStep;			// Font Step
} STNEOFONTINFO;

typedef struct {
//	int	 nWidth;			// Font Width
//	int	 nHeight;			// Font Height
	char sFontName[20+1];
} STRESIZEFONT;


class CNeoReszCtl
{
public:
	// *
	CNeoReszCtl() : m_pCtrl(NULL)
	{
	}

	CNeoReszCtl(CWnd* _pWnd)
	{
		m_pCtrl = _pWnd;
	}

	~CNeoReszCtl()
	{
	}

	// Variable
	int		m_nBaseStep,	// 기본스텝 (9)
			m_nCurStep;		// 현재스텝

	CRect	m_BaseRect,		// 기본스텝에서의 기본 위치영역
			m_CurRect;		// 현재의 위치영역

	CFont*	m_BaseFont,		// 기본스텝에서의 기본 폰트정보
			m_CurFont;		// 현재의 폰트정보


	CWnd*	m_pCtrl;		// 윈도우 핸들

	// Operation
	void	Resize(STNEOFONTINFO *_pNewInfo)
	{
	}

};

#endif // __NEOCOMPONENT__RESIZE__CONTROL__FONT__HEADER__
