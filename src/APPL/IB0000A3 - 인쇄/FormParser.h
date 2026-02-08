// FormParser.h: interface for the CFormParser class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CLine
{
//	CLinesArray				m_arLines;
	struct	Line			m_stLines[MAXCOUNT]{};
	int						m_iCnt{};
	CStringArray			m_arMultiLines{};

public:
	void					SetCount(int icnt){m_iCnt = icnt;};
	int						GetCount(int icnt){return m_iCnt;};
	void					SetLine(int index, struct	Line line){m_stLines[index]=line;};
	void					SetMultiLine(CString sLine){m_arMultiLines.Add(sLine);};

	void					GetLine(int index, struct	Line line) {line=m_stLines[index];};
	void					GetMultiLine(CStringArray& astrList) {astrList.Copy(m_arMultiLines);};
	int						GetLine(struct Line* pList)
	{
		for(int i=0;i<m_iCnt;i++)
		{
			pList[i] = m_stLines[i];
		}

		return m_iCnt;
	}

};

class CMainDlg;
class CFormParser  
{
public:
	CFormParser(CString sIniFile=_T(""));
	virtual ~CFormParser();

	CMainDlg*				m_pMain;
	CString					m_strIniFile;
	int						m_iContentCount;
	CString					m_strTitle;
	int						m_iTitleRow[2], m_iBotRow[2];
	int						m_iMargin[4];
	int						m_iPaperMode;
	int						m_iPageNumAlign;//0:left, 1:center, 2:right

	int						m_iDrawType[MAXCOUNT];//space0?grid1?text2
	int						m_iTextLineCnt[MAXCOUNT];
	int						m_iPageNumType;//0:page/totpage, 1:page
	int						m_iSpaceHeight;
	CString					m_strPageNumToken[3];

	CString					m_strDtText[MAXCOUNT];
	class	CLine			m_clLines[MAXCOUNT];
	class	CLine			m_clHdrLeft, m_clHdrRight, m_clBotLeft, m_clBotRight;
	int						m_iGridRowCnt[MAXCOUNT], m_iGridColCnt[MAXCOUNT];

	int						m_iGridTextMargin, m_iTextMargin;
	CString					m_strPrnDocName;
	CBitmap*				m_hBackground;


	void					setFile(CString sFile){m_strIniFile=sFile;};
	void					LoadForm();
	void					ParserLine(int index, int ilineCnt, CString sType, CString strSection);

	int						GetLineCount(int idx){return m_iTextLineCnt[idx];};
	int						GetColCount(int idx){return m_iGridColCnt[idx];};
	int						GetRowCount(int idx){return m_iGridRowCnt[idx];};
	int						GetDrawType(int idx){return m_iDrawType[idx];};
	CString					GetDTText(int idx){return m_strDtText[idx];};
	int						GetLine(int idx, struct	Line* stList);
	int						GetMultiLine(int idx, CStringArray& arList);
	int						GetTitle(int itype, struct	Line* stList);
	int						GetBottom(int itype, struct	Line* stList);

	int						GetMultiLineSubIndex(int icontentIDX);


	CString					GetInputValue(int itype, CString sData);
	void					setOutputValue();


	


};

