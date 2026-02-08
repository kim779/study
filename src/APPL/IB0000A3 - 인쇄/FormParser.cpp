// FormParser.cpp: implementation of the CFormParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB0000A3.h"
#include "FormParser.h"
#include "MainDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormParser::CFormParser(CString sIniFile)
{
	m_strIniFile = sIniFile;
	m_hBackground = NULL;
}

CFormParser::~CFormParser()
{

}


void CFormParser::LoadForm()
{
//Query
//Report
	CString strSection, sRead[5], sList[10];
	int	iTmp{};


	m_iContentCount = 0;
	GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("TYPE"), 0); //

	
	m_iPageNumAlign = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("PAGEALIGN"), 0);
	m_iGridTextMargin = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("GRIDTEXTMARGIN"), 5);
	m_iTextMargin = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("TEXTMARGIN"), 10);
	m_strPrnDocName = GetConfigFileString(m_strIniFile, _T("REPORT"), _T("DOCNAME"), "문서");

	sRead[0] = GetConfigFileString(m_strIniFile, _T("REPORT"), _T("BACKGROUND"), "");
	if(sRead[0] != "")
		m_hBackground = m_pMain->getBitmap(sRead[0]);
	
	m_iMargin[0] = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("LEFTMARGIN"), 10);
	m_iMargin[1] = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("TOPMARGIN"), 10);
	m_iMargin[2] = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("RIGHTTMARGIN"), 10) ;
	m_iMargin[3] = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("BOTTOMMARGIN"), 10) ;

	m_iPaperMode = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("PAPERMODE"), 1);
	m_iSpaceHeight = GetConfigFileInt(m_strIniFile, _T("REPORT"), _T("SPACEHEIGHT"), 10);
	
	
	sRead[0] = GetConfigFileString(m_strIniFile, _T("REPORT"), _T("PAGENUMBER"), "");
	SplitString(sRead[0], DELIMITER, sList);

	if(sList[0] == "Y")
	{
		m_iPageNumType = CInt(sList[1]);
		m_strPageNumToken[0] = sList[2];
		m_strPageNumToken[1] = sList[3];
	}
	else m_iPageNumType = -1;

	m_strTitle = GetConfigFileString(m_strIniFile, _T("HEADER"), _T("TITLE"), "");
	m_iTitleRow[0] = GetConfigFileInt(m_strIniFile, _T("HEADER"), _T("LEFTLBLCOUNT"), 0);
	m_clHdrLeft.SetCount( m_iTitleRow[0] );
	
	m_iTitleRow[1] = GetConfigFileInt(m_strIniFile, _T("HEADER"), _T("RIGHTLBLCOUNT"), 0);
	m_clHdrRight.SetCount( m_iTitleRow[1] );

	struct	Line	stLine;
	CString	strApp[] = {"LEFTLBL_", "RIGHTLBL_"};
//header
	int ir = 0;

	for(ir=0; ir<2; ir++)
	{
		for(int i=0; i<m_iTitleRow[ir]; i++)
		{
			strSection.Format("%s%02d", strApp[ir], i+1);
			sRead[0] = GetConfigFileString(m_strIniFile, _T("HEADER"), strSection, "");

			iTmp = SplitString(sRead[0], DELIMITER, sList);
			for(int ic=0; ic<iTmp; ic++)
			{
				stLine.iDataType[ic] = TXTDRAWTYPE( SplitString(sList[ic], SUBDELIMITER) );
				stLine.sText[ic] = sList[ic];
			}

			if(ir == 0)
				m_clHdrLeft.SetLine(i, stLine);
			else m_clHdrRight.SetLine(i, stLine);
		}
	}

	//bottom
	m_iBotRow[0] = GetConfigFileInt(m_strIniFile, _T("BOTTOM"), _T("LEFTLBLCOUNT"), 0);
	m_clBotLeft.SetCount( m_iBotRow[0] );
	
	m_iBotRow[1] = GetConfigFileInt(m_strIniFile, _T("BOTTOM"), _T("RIGHTLBLCOUNT"), 0);
	m_clBotRight.SetCount( m_iBotRow[1] );
	for( ir=0; ir<2; ir++)
	{
		for(int i=0; i<m_iBotRow[ir]; i++)
		{
			strSection.Format("%s%02d", strApp[ir], i+1);
			sRead[0] = GetConfigFileString(m_strIniFile, _T("BOTTOM"), strSection, "");

			iTmp = SplitString(sRead[0], DELIMITER, sList);
			for(int ic=0; ic<iTmp; ic++)
			{
				stLine.iDataType[ic] = TXTDRAWTYPE( SplitString(sList[ic], SUBDELIMITER) );
				stLine.sText[ic] = sList[ic];
			}

			if(ir == 0)
				m_clBotLeft.SetLine(i, stLine);
			else m_clBotRight.SetLine(i, stLine);
		}
	}

	//contents
	m_iContentCount = GetConfigFileInt(m_strIniFile, _T("CONTENT"), _T("LINETYPECOUNT"), 0); //
	for(int i=0; i<m_iContentCount;i++)
	{
		strSection.Format("LINE%02dCOUNT",  i+1);
		iTmp = GetConfigFileInt(m_strIniFile, _T("CONTENT"), strSection, 0);

		strSection.Format("LINE%02dTYPE",  i+1);
		sRead[0] = GetConfigFileString(m_strIniFile, _T("CONTENT"), strSection, "");

		strSection.Format("LINE%02dDATA",  i+1);
		ParserLine(i, iTmp, sRead[0], strSection);
	}
}

CString CFormParser::GetInputValue(int itype, CString sData)
{
	if(itype != INDATATYPE) return sData;

	CString sResult = "";
	const int idx = 0;
	if( isNumber(sData) )
	{
		sResult = m_pMain->GetInputData(CInt(sData));
	}
	else
	{
		int idxlist[10]{}, iloop = 0;
		CString stmp = "", sToken[10];
		int i = 0;
		for(i=0; i<10; i++)
		{
			sToken[i] = "";
			idxlist[i] = 0;
		}

		for( i=0; i<sData.GetLength(); i++)
		{
			if( isNum(sData.GetAt(i) ) )
				stmp += sData.GetAt(i);
			else
			{
				sToken[iloop] += sData.GetAt(i);
				if(stmp != "")
				{
					idxlist[iloop++] = CInt(stmp);
					stmp = "";
				}
			}
		}

		if(stmp != "")
		{
			idxlist[iloop++] = CInt(stmp);
			stmp = "";
		}

		
		
		for(i=0; i<iloop; i++)
		{
			sResult += m_pMain->GetInputData(idxlist[i]) + sToken[i];
		}
	}

	return sResult;
}

void CFormParser::setOutputValue()
{
	struct	Line	stLine[MAXCOUNT], stLine1;

	for(int i=0; i<m_iContentCount; i++)
	{
		GetLine(i, stLine);
		for(int row=0; row <m_iTextLineCnt[i]; row++)
		{
			stLine1 = stLine[row];
			for(int ic=0; ic<stLine1.iFieldCnt; ic++)
			{
				if( stLine1.iDataType[ic] == OUTDATATYPE) //out
				{
					if( m_iDrawType[i] != MULTILINETEXTDRAW)
					{
						stLine1.sText[ic] = m_pMain->GetOutputData( stLine1.sText[ic] );
						stLine1.iDataType[ic] = LBLDATATYPE;//LBL 로변경 
					}
				}

				if( stLine1.iDataType[ic] == DOMINODATATYPE)
				{
					stLine1.sText[ic] = m_pMain->GetDominoData( stLine1.sText[ic] );
					stLine1.iDataType[ic] = LBLDATATYPE;//LBL 로변경 
				}
			}

			m_clLines[i].SetLine(row, stLine1);
		}
	}
}

void CFormParser::ParserLine(int index, int ilineCnt, CString sType, CString strSection)
{
	CString sLine[MAXCOUNT], sList[MAXCOUNT];
	CString sTmp = SplitString(sType, SUBDELIMITER);
	m_iDrawType[index] = DRAWTYPE(sTmp);
	m_strDtText[index] = sType;

	m_iTextLineCnt[index] = ilineCnt;
	struct	Line	stLine;

	CString strSection1, sRead;
	for(int irow=0; irow<ilineCnt; irow++)
	{
		strSection1.Format("%s_%02d",  strSection, irow+1);
		sRead = GetConfigFileString(m_strIniFile, _T("CONTENT"), strSection1, "");
		if(sRead.IsEmpty()) break; 

		stLine.iFieldCnt =  SplitString(sRead, DELIMITER, sList);
		for(int i=0; i<stLine.iFieldCnt; i++)
		{
			stLine.iDataType[i] = TXTDRAWTYPE( SplitString(sList[i], SUBDELIMITER) );
			stLine.sText[i] = GetInputValue(stLine.iDataType[i], sList[i]);
		}

		strSection1.Format("%s_ALIGN%02d",  strSection, irow+1);
		sRead = GetConfigFileString(m_strIniFile, _T("CONTENT"), strSection1, "0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;");
		SplitInt(sRead, DELIMITER, stLine.iTextAlign);

		strSection1.Format("%s_WIDTH%02d",  strSection, irow+1);
		sRead = GetConfigFileString(m_strIniFile, _T("CONTENT"), strSection1, "0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;");
		SplitDoubleD(sRead, DELIMITER, stLine.lWidthDif, 0.01);


		m_clLines[index].SetLine(irow, stLine);
	}

	m_clLines[index].SetCount(m_iTextLineCnt[index]);
	if(m_iDrawType[index] < 3)
	{
		sTmp = m_strDtText[index];
		
		if(m_iDrawType[index] > 0 ) 
		{
			m_iGridRowCnt[index] = CInt( SplitString(sTmp, "*") );
			m_iGridColCnt[index] = CInt(sTmp);
		}
		else m_iTextLineCnt[index] = CInt( SplitString(sTmp, SUBDELIMITER) );
	}
}

int CFormParser::GetMultiLine(int idx, CStringArray& arList)
{
	m_clLines[idx].GetMultiLine(arList);
	return arList.GetSize(); 
}

int CFormParser::GetLine(int idx, struct	Line* stList)
{
	return m_clLines[idx].GetLine(stList);  
}

int CFormParser::GetTitle(int itype,  struct	Line* stList)
{
	if(itype)
	{
		return m_clHdrRight.GetLine(stList);
	}

	return m_clHdrLeft.GetLine(stList);
}


int CFormParser::GetBottom(int itype,  struct	Line* stList)
{
	if(itype)
	{
		return m_clBotRight.GetLine(stList);
	}
	return m_clBotLeft.GetLine(stList);
}


int CFormParser::GetMultiLineSubIndex(int icontentIDX)
{
	Line stList[MAXCOUNT];
	if( m_clLines[icontentIDX].GetLine(stList) > -1)
	{
		return CInt(stList[0].sText[0]);
	}

	return -1;
}