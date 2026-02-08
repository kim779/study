// CondTarget.cpp: implementation of the CCondTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "LogMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogMan::CLogMan()
{
	m_bLogging = FALSE;
}

CLogMan::~CLogMan()
{
}

void CLogMan::AddLog(CString strMsg)
{
	if (m_bLogging)
		m_arLogMsg.Add(strMsg);
}

void CLogMan::SaveLogFile(CString strPath)
{
	if (!m_bLogging)
		return;

	CTime time = CTime::GetCurrentTime();
	CString strFilePath;
	strFilePath.Format(_T("%s\\MultiChart%04d%02d%02d_%02d%02d%02d.log"), strPath, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	CStdioFile File;
	CFileException ex;
	
	if (!File.Open(strFilePath, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex))
	{
		ex.ReportError();
		return;
	}
	
	TRY
	{
		int nSize = m_arLogMsg.GetSize();
		for (int i=0; i<nSize; i++)
		{
			File.WriteString(m_arLogMsg.GetAt(i));
			File.WriteString(_T("\n"));
		}

		File.Close();
	}
	CATCH (CFileException, e)
	{
		return ;
	}
	END_CATCH

}
