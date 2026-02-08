// SymbolSingleton.cpp: implementation of the CSymbolSingleton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "SymbolSingleton.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "SymbolTool.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSymbolSingleton::CSymbolSingleton()
{
	m_pSelectTool = new CSelectTool();
	m_pFigureSquareTool = new CFigure_Square_Tool();
	m_pFigureCircleTool = new CFigure_Circle_Tool();
	m_pFigureTriangleTool = new CFigure_Triangle_Tool();
	m_pTextTool = new CTextTool();
	m_pArrowLeftTool = new CArrow_Left_Tool();
	m_pArrowRightTool = new CArrow_Right_Tool();
	m_pArrowUpTool = new CArrow_Up_Tool();
	m_pArrowDownTool = new CArrow_Down_Tool();
	m_pArrowLeftTopTool = new CArrow_LeftTop_Tool();
	m_pArrowLeftBottomTool = new CArrow_LeftBottom_Tool();
	m_pArrowRightTopTool = new CArrow_RightTop_Tool();
	m_pArrowRightBottomTool = new CArrow_RightBottom_Tool();
	m_pNumberOneTool = new CNumber_One_Tool();
	m_pNumberTwoTool = new CNumber_Two_Tool();
	m_pNumberThreeTool = new CNumber_Three_Tool();
	m_pNumberFourTool = new CNumber_Four_Tool();
	m_pNumberFiveTool = new CNumber_Five_Tool();
	m_pNumberSixTool = new CNumber_Six_Tool();
	m_pNumberSevenTool = new CNumber_Seven_Tool();
	m_pNumberEightTool = new CNumber_Eight_Tool();
	m_pNumberNineTool = new CNumber_Nine_Tool();
	//m_pNumberTenTool = new CNumber_Ten_Tool();
	m_pTextATool = new CText_A_Tool();
	m_pTextBTool = new CText_B_Tool();
	m_pTextCTool = new CText_C_Tool();
	m_pTextDTool = new CText_D_Tool();
	m_pTextETool = new CText_E_Tool();
	m_pTextFTool = new CText_F_Tool();
	m_pTextGTool = new CText_G_Tool();
	m_pTextHTool = new CText_H_Tool();

	m_pChar1Tool = new CChar_1_Tool();
	m_pChar2Tool = new CChar_2_Tool();
	m_pChar3Tool = new CChar_3_Tool();
	m_pChar4Tool = new CChar_4_Tool();
	m_pChar5Tool = new CChar_5_Tool();
	m_pChar6Tool = new CChar_6_Tool();
}

CSymbolSingleton::~CSymbolSingleton()
{
	delete m_pSelectTool;
	delete m_pFigureSquareTool;
	delete m_pFigureCircleTool;
	delete m_pFigureTriangleTool;
	delete m_pTextTool;
	delete m_pArrowLeftTool;
	delete m_pArrowRightTool;
	delete m_pArrowUpTool;
	delete m_pArrowDownTool; 
	delete m_pArrowLeftTopTool;
	delete m_pArrowLeftBottomTool; 
	delete m_pArrowRightTopTool;
	delete m_pArrowRightBottomTool;
	delete m_pNumberOneTool; 
	delete m_pNumberTwoTool; 
	delete m_pNumberThreeTool;
	delete m_pNumberFourTool;
	delete m_pNumberFiveTool;
	delete m_pNumberSixTool;
	delete m_pNumberSevenTool;
	delete m_pNumberEightTool;
	delete m_pNumberNineTool;
	//delete m_pNumberTenTool;
	delete m_pTextATool;
	delete m_pTextBTool;
	delete m_pTextCTool;
	delete m_pTextDTool;
	delete m_pTextETool;
	delete m_pTextFTool;
	delete m_pTextGTool;
	delete m_pTextHTool;

	delete m_pChar1Tool;
	delete m_pChar2Tool;
	delete m_pChar3Tool;
	delete m_pChar4Tool;
	delete m_pChar5Tool;
	delete m_pChar6Tool;
}

CSymbolTool* CSymbolSingleton::Find(CSymbolData* pSymbolData, const CSymbolBaseData::TOOLTYPE symboltype)
{
	switch(symboltype){
		case CSymbolBaseData::SYMBOL_NONE:			// 선택
			return NULL;
		case CSymbolBaseData::SYMBOL_SELECTION:			// 선택
			return GetTool(pSymbolData, m_pSelectTool);
		case CSymbolBaseData::SYMBOL_SQUARE:			// 사각도형
			return GetTool(pSymbolData, m_pFigureSquareTool);
		case CSymbolBaseData::SYMBOL_CIRCLE:			// 원도형
			return GetTool(pSymbolData, m_pFigureCircleTool);
		case CSymbolBaseData::SYMBOL_TRIANGLE:			// 삼각형
			return GetTool(pSymbolData, m_pFigureTriangleTool);
		case CSymbolBaseData::SYMBOL_TEXT:			// Text
			return GetTool(pSymbolData, m_pTextTool);
		case CSymbolBaseData::SYMBOL_ARROW_LEFT:			// 왼쪽 화살표
			return GetTool(pSymbolData, m_pArrowLeftTool);
		case CSymbolBaseData::SYMBOL_ARROW_RIGHT:			// 오른쪽 화살표
			return GetTool(pSymbolData, m_pArrowRightTool);
		case CSymbolBaseData::SYMBOL_ARROW_UP:  //위쪽 화살표
			return GetTool(pSymbolData, m_pArrowUpTool);
		case CSymbolBaseData::SYMBOL_ARROW_DOWN:  //아래쪽 화살표
			return GetTool(pSymbolData, m_pArrowDownTool);
		case CSymbolBaseData::SYMBOL_ARROW_LEFTTOP:  //왼쪽 위 방향 화살표
			return GetTool(pSymbolData, m_pArrowLeftTopTool);
		case CSymbolBaseData::SYMBOL_ARROW_LEFTBOTTOM: //왼쪽 아래 방향 화살표
			return GetTool(pSymbolData, m_pArrowLeftBottomTool);
		case CSymbolBaseData::SYMBOL_ARROW_RIGHTTOP:  //오른쪽 위 방향 화살표
			return GetTool(pSymbolData, m_pArrowRightTopTool);
		case CSymbolBaseData::SYMBOL_ARROW_RIGHTBOTTOM:  //오른쪽 아래 방향 화살표
			return GetTool(pSymbolData, m_pArrowRightBottomTool);
		case CSymbolBaseData::SYMBOL_NUM_ONE:  //숫자 1
			return GetTool(pSymbolData, m_pNumberOneTool);
		case CSymbolBaseData::SYMBOL_NUM_TWO:  //2
			return GetTool(pSymbolData, m_pNumberTwoTool);
		case CSymbolBaseData::SYMBOL_NUM_THREE: //3
			return GetTool(pSymbolData, m_pNumberThreeTool);
		case CSymbolBaseData::SYMBOL_NUM_FOUR:  //4
			return GetTool(pSymbolData, m_pNumberFourTool);
		case CSymbolBaseData::SYMBOL_NUM_FIVE:  //5
			return GetTool(pSymbolData, m_pNumberFiveTool);
		case CSymbolBaseData::SYMBOL_NUM_SIX:  //6
			return GetTool(pSymbolData, m_pNumberSixTool);
		case CSymbolBaseData::SYMBOL_NUM_SEVEN:  //7
			return GetTool(pSymbolData, m_pNumberSevenTool);
		case CSymbolBaseData::SYMBOL_NUM_EIGHT:  //8
			return GetTool(pSymbolData, m_pNumberEightTool);
		case CSymbolBaseData::SYMBOL_NUM_NINE:  //9
			return GetTool(pSymbolData, m_pNumberNineTool);
		//case CSymbolBaseData::SYMBOL_NUM_TEN:  //10
		//	return GetTool(pSymbolData, m_pNumberTenTool);
		case CSymbolBaseData::SYMBOL_TEXT_A:  //a
			return GetTool(pSymbolData, m_pTextATool);
		case CSymbolBaseData::SYMBOL_TEXT_B: //b
			return GetTool(pSymbolData, m_pTextBTool);
		case CSymbolBaseData::SYMBOL_TEXT_C: //c
			return GetTool(pSymbolData, m_pTextCTool);
		case CSymbolBaseData::SYMBOL_TEXT_D: //d
			return GetTool(pSymbolData, m_pTextDTool);
		case CSymbolBaseData::SYMBOL_TEXT_E: //e
			return GetTool(pSymbolData, m_pTextETool);
		case CSymbolBaseData::SYMBOL_TEXT_F: //f
			return GetTool(pSymbolData, m_pTextFTool);
		case CSymbolBaseData::SYMBOL_TEXT_G: //g
			return GetTool(pSymbolData, m_pTextGTool);
		case CSymbolBaseData::SYMBOL_TEXT_H: //h
			return GetTool(pSymbolData, m_pTextHTool);
		case CSymbolBaseData::SYMBOL_CHAR_1: //h
			return GetTool(pSymbolData, m_pChar1Tool);
		case CSymbolBaseData::SYMBOL_CHAR_2: //h
			return GetTool(pSymbolData, m_pChar2Tool);
		case CSymbolBaseData::SYMBOL_CHAR_3: //h
			return GetTool(pSymbolData, m_pChar3Tool);
		case CSymbolBaseData::SYMBOL_CHAR_4: //h
			return GetTool(pSymbolData, m_pChar4Tool);
		case CSymbolBaseData::SYMBOL_CHAR_5: //h
			return GetTool(pSymbolData, m_pChar5Tool);
		case CSymbolBaseData::SYMBOL_CHAR_6: //h
			return GetTool(pSymbolData, m_pChar6Tool);
		default:
			return NULL;
	}
}

// 모든 Tool을 초기화 한다.
void CSymbolSingleton::InitialTool()
{
	m_pSelectTool->Initial();
	m_pArrowLeftTool->Initial();
	m_pArrowRightTool->Initial();
	m_pFigureSquareTool->Initial();
	m_pFigureCircleTool->Initial();
	m_pFigureTriangleTool->Initial();
	m_pArrowUpTool->Initial();
	m_pArrowDownTool->Initial();
	m_pArrowLeftTopTool->Initial();
	m_pArrowLeftBottomTool->Initial();
	m_pArrowRightTopTool->Initial();
	m_pArrowRightBottomTool->Initial();
	m_pNumberOneTool->Initial();
	m_pNumberTwoTool->Initial();
	m_pNumberThreeTool->Initial();
	m_pNumberFourTool->Initial();
	m_pNumberFiveTool->Initial();
	m_pNumberSixTool->Initial();
	m_pNumberSevenTool->Initial();
	m_pNumberEightTool->Initial();
	m_pNumberNineTool->Initial();
	//m_pNumberTenTool->Initial();
	m_pTextATool->Initial();
	m_pTextBTool->Initial();
	m_pTextCTool->Initial();
	m_pTextDTool->Initial();
	m_pTextETool->Initial();
	m_pTextFTool->Initial();
	m_pTextGTool->Initial();
	m_pTextHTool->Initial();

	m_pChar1Tool->Initial();
	m_pChar2Tool->Initial();
	m_pChar3Tool->Initial();
	m_pChar4Tool->Initial();
	m_pChar5Tool->Initial();
	m_pChar6Tool->Initial();
}

// private ====================================================================
CSymbolTool* CSymbolSingleton::GetTool(CSymbolData* pSymbolData, CSymbolTool* pTool)
{
	if(pTool != NULL)
		pTool->SetSymbolData(pSymbolData);

	return pTool;
}

// (2008/2/15 - Seung-Won, Bae) for MultiLanguage
void CSymbolSingleton::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_pSelectTool->SetOcxHwnd( p_hOcxWnd);
	m_pFigureSquareTool->SetOcxHwnd( p_hOcxWnd);
	m_pFigureCircleTool->SetOcxHwnd( p_hOcxWnd);
	m_pFigureTriangleTool->SetOcxHwnd( p_hOcxWnd);
	m_pTextTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowLeftTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowRightTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowUpTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowDownTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowLeftTopTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowLeftBottomTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowRightTopTool->SetOcxHwnd( p_hOcxWnd);
	m_pArrowRightBottomTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberOneTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberTwoTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberThreeTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberFourTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberFiveTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberSixTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberSevenTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberEightTool->SetOcxHwnd( p_hOcxWnd);
	m_pNumberNineTool->SetOcxHwnd( p_hOcxWnd);
}
