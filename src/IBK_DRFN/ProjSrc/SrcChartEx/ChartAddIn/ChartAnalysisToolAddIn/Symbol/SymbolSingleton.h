// SymbolSingleton.h: interface for the CSymbolSingleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLSINGLETON_H__8BEDA347_B44B_4E79_A9FD_B8E108FE3855__INCLUDED_)
#define AFX_SYMBOLSINGLETON_H__8BEDA347_B44B_4E79_A9FD_B8E108FE3855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SymbolBaseData.h"

class CSymbolTool;
class CSelectTool;
class CFigure_Square_Tool;
class CFigure_Circle_Tool;
class CFigure_Triangle_Tool;
class CTextTool;
class CArrow_Left_Tool;
class CArrow_Right_Tool;
class CArrow_Up_Tool;
class CArrow_Down_Tool;
class CArrow_LeftTop_Tool;
class CArrow_LeftBottom_Tool;
class CArrow_RightTop_Tool;
class CArrow_RightBottom_Tool;
class CNumber_One_Tool;
class CNumber_Two_Tool;
class CNumber_Three_Tool;
class CNumber_Four_Tool;
class CNumber_Five_Tool;
class CNumber_Six_Tool;
class CNumber_Seven_Tool;
class CNumber_Eight_Tool;
class CNumber_Nine_Tool;
class CNumber_Ten_Tool;
class CText_A_Tool;
class CText_B_Tool;
class CText_C_Tool;
class CText_D_Tool;
class CText_E_Tool;
class CText_F_Tool;
class CText_G_Tool;
class CText_H_Tool;

class CChar_1_Tool;
class CChar_2_Tool;
class CChar_3_Tool;
class CChar_4_Tool;
class CChar_5_Tool;
class CChar_6_Tool;

class CSymbolData;
class CSymbolSingleton  
{
public:
	CSymbolSingleton();
	~CSymbolSingleton();

	CSymbolTool* Find(CSymbolData* pSymbolData, const CSymbolBaseData::TOOLTYPE symboltype);
	void InitialTool();
	
private:
	CSelectTool* m_pSelectTool;
	CFigure_Square_Tool* m_pFigureSquareTool;
	CFigure_Circle_Tool* m_pFigureCircleTool;
	CFigure_Triangle_Tool* m_pFigureTriangleTool;
	CTextTool* m_pTextTool;
	CArrow_Left_Tool* m_pArrowLeftTool;
	CArrow_Right_Tool* m_pArrowRightTool;
	CArrow_Up_Tool* m_pArrowUpTool;
	CArrow_Down_Tool* m_pArrowDownTool;
	CArrow_LeftTop_Tool* m_pArrowLeftTopTool;
	CArrow_LeftBottom_Tool* m_pArrowLeftBottomTool;
	CArrow_RightTop_Tool* m_pArrowRightTopTool;
	CArrow_RightBottom_Tool* m_pArrowRightBottomTool;
	CNumber_One_Tool* m_pNumberOneTool;
	CNumber_Two_Tool* m_pNumberTwoTool;
	CNumber_Three_Tool* m_pNumberThreeTool;
	CNumber_Four_Tool* m_pNumberFourTool;
	CNumber_Five_Tool* m_pNumberFiveTool;
	CNumber_Six_Tool* m_pNumberSixTool;
	CNumber_Seven_Tool* m_pNumberSevenTool;
	CNumber_Eight_Tool* m_pNumberEightTool;
	CNumber_Nine_Tool* m_pNumberNineTool;
	CNumber_Ten_Tool* m_pNumberTenTool;
	CText_A_Tool* m_pTextATool;
	CText_B_Tool* m_pTextBTool;
	CText_C_Tool* m_pTextCTool;
	CText_D_Tool* m_pTextDTool;
	CText_E_Tool* m_pTextETool;
	CText_F_Tool* m_pTextFTool;
	CText_G_Tool* m_pTextGTool;
	CText_H_Tool* m_pTextHTool;

	CChar_1_Tool* m_pChar1Tool;
	CChar_2_Tool* m_pChar2Tool;
	CChar_3_Tool* m_pChar3Tool;
	CChar_4_Tool* m_pChar4Tool;
	CChar_5_Tool* m_pChar5Tool;
	CChar_6_Tool* m_pChar6Tool;

	CSymbolTool* GetTool(CSymbolData* pSymbolData, CSymbolTool* pTool);

// (2008/2/15 - Seung-Won, Bae) for MultiLanguage
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

#endif // !defined(AFX_SYMBOLSINGLETON_H__8BEDA347_B44B_4E79_A9FD_B8E108FE3855__INCLUDED_)
