// RushMan.cpp : implementation file
//

#include "stdafx.h"
#include "RushTester.h"
#include "RushMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	cCR	0x0D
#define	cLF	0x0A
#define	cEOF	0x1A

#define	TID_NEWCONNECT	10

/////////////////////////////////////////////////////////////////////////////
// CRushMan

CRushMan::CRushMan()
{
	m_nAbsConnectorIdx = 0;
}

CRushMan::~CRushMan()
{
}


BEGIN_MESSAGE_MAP(CRushMan, CWnd)
	//{{AFX_MSG_MAP(CRushMan)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CONNECTOR_MSG, OnConnectorMsg)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRushMan message handlers

void CRushMan::SetUser(CString Id, CString Pwd, CString uuid)
{
	m_sId  = Id;
	m_sPwd = Pwd;
	m_suuid = uuid;
}

void CRushMan::InitQuery(int QueryInterval, int ResponseWaitTime)
{
	m_nQueryInterval = QueryInterval >= 0 ? QueryInterval : 0;
	m_nResponseWaitTime = ResponseWaitTime >= 0 ? ResponseWaitTime : 0;
}

void CRushMan::StartConnect(CString Ip, int Port, int ConnectCount, int ConInterval, CString Opt)
{
	if (m_sId.IsEmpty())
	{
		AfxMessageBox("No UserId is registered");
		return;
	}

	if (m_sPwd.IsEmpty())
	{
		AfxMessageBox("No UserPwd is registered");
		return;
	}

	m_sIp		= Ip;
	m_nPort		= Port >= 0 ? Port : 80;
	m_nConnectCount	= ConnectCount > 0 ? ConnectCount : 1;
	m_nConnectInterval = ConInterval >= 0 ? ConInterval : 0;
	m_sOpt		= Opt;

	NewConnect(m_sOpt);
	if (m_nConnectCount > 1)
	{
		if (m_nConnectInterval == 0)
		{
			while (NewConnect(m_sOpt) < m_nConnectCount -1)
				;
		}
		else
			SetTimer(TID_NEWCONNECT, m_nConnectInterval, NULL);
	}
}

int CRushMan::NewConnect(CString Opt)
{
	CConnector* pCon = new CConnector(this, m_nAbsConnectorIdx++);
	pCon->CreateWnd(100 + m_nAbsConnectorIdx);

	if(m_arConnector.GetSize() == 0)
		m_sId = "fits01";
	else
	{
		int idata = m_arConnector.GetSize() % 10;
		switch(idata)
		{
		case 0:
			m_sId = "fits01";
		case 1:
			m_sId = "fits02";
		case 2:
			m_sId = "fits03";
		case 4:
			m_sId = "fits04";
		case 5:
			m_sId = "fits05";
		case 6:
			m_sId = "fits06";
		case 7:
			m_sId = "fits07";
		case 8:
			m_sId = "fits08";
		case 9:
			m_sId = "fits09";
		}
	}

	pCon->SetUser(m_sId, m_sPwd, m_suuid);
	pCon->InitQuery(&m_arQueryData, m_nQueryInterval, m_nResponseWaitTime);
	pCon->DoConnect(m_sIp, m_nPort, Opt);
	pCon->SetQueryData(m_iCount, m_iQueryGubn);

	return m_arConnector.Add(pCon);
}

void CRushMan::DisConnect(int idx)
{
	if (idx < 0 || idx >= m_arConnector.GetSize()) 
		return;

	int nAbsIndex;
	CConnector* pCon;
	
	pCon = (CConnector*)m_arConnector.GetAt(idx);
	m_arConnector.RemoveAt(idx);
	nAbsIndex = pCon->m_nAbsIndex;
	pCon->DestroyWindow();
	delete pCon;

	GetParent()->SendMessage(WM_RUSHMAN_MSG, MAKEWPARAM(WL_DISCONNECT, nAbsIndex), 0);
}

void CRushMan::OnDestroy() 
{
	DisConnectAll();
	ClearQueryData();

	CWnd::OnDestroy();
}

void CRushMan::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TID_NEWCONNECT:
		KillTimer(nIDEvent);
		if (NewConnect(m_sOpt) < m_nConnectCount -1)
			SetTimer(TID_NEWCONNECT, m_nConnectInterval, NULL);
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CRushMan::DisConnectAll()
{
	KillTimer(TID_NEWCONNECT);
	if (m_arConnector.GetSize() > 0)
	{
		for (int ii = m_arConnector.GetSize() -1; ii >= 0; ii--)
			DisConnect(ii);
	}
}


LRESULT CRushMan::OnConnectorMsg(WPARAM wParam, LPARAM lParam)
{
	int	nAbsIdx = HIWORD(wParam);

	switch (LOWORD(wParam))
	{
	case WL_DISCONNECT:
		DisConnect(AbsToIdx(nAbsIdx));
		break;
	case WL_ENABLEFALSE:
		GetParent()->SendMessage(WM_RUSHMAN_MSG, wParam, lParam);
		break;
	case WL_ENABLETRUE:
		GetParent()->SendMessage(WM_RUSHMAN_MSG, wParam, lParam);
		break;
	default:
		break;
	}

	GetParent()->SendMessage(WM_RUSHMAN_MSG, wParam, lParam);
	return 0;
}

int CRushMan::AbsToIdx(int abs)
{
	for (int ii = 0; ii < m_arConnector.GetSize(); ii++)
	{
		CConnector* pCon = (CConnector*)m_arConnector.GetAt(ii);
		if(pCon->m_nAbsIndex == abs)
			return ii;
	}

	return -1;
}

int CRushMan::GetCount()
{
	return m_arConnector.GetSize();
}

int CRushMan::GetLoginCount()
{
	int nCnt = 0;
	for (int ii = 0; ii < m_arConnector.GetSize(); ii++)
	{
		CConnector* pCon = (CConnector*)m_arConnector.GetAt(ii);
		if (pCon->m_connectInfo.m_timeConnected  != 0)
			nCnt++;
	}
	return nCnt;
}

int CRushMan::GetAlertCount()
{
	int nCnt = 0;
	for (int ii = 0; ii < m_arConnector.GetSize(); ii++)
	{
		CConnector* pCon = (CConnector*)m_arConnector.GetAt(ii);
		if (pCon->m_connectInfo.m_timeLastAlert  != 0)
		{
			CTimeSpan ts = CTime::GetCurrentTime() - pCon->m_connectInfo.m_timeLastAlert;
			if (ts.GetSeconds() < 15)
				nCnt++;
		}
	}
	return nCnt;
}

int CRushMan::GetAliveCount()
{
	int nCnt = 0;
	for (int ii = 0; ii < m_arConnector.GetSize(); ii++)
	{
		CConnector* pCon = (CConnector*)m_arConnector.GetAt(ii);
		if (pCon->m_connectInfo.m_timeLastOub  != 0)
		{
			if (m_nQueryInterval > 0)
			{
				CTimeSpan ts = CTime::GetCurrentTime() - pCon->m_connectInfo.m_timeLastOub;
				if (ts.GetSeconds() <= m_nQueryInterval / 1000 + 4)
					nCnt++;
			}
		}
	}
	return nCnt;
}

CString CRushMan::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return _T("");
}

void CRushMan::ClearQueryData()
{
	for (int ii = m_arQueryData.GetSize()-1; ii >= 0; ii--)
	{
		CQueryData* pQuery = (CQueryData*)m_arQueryData.GetAt(ii);
		delete[] pQuery->m_pData;
		delete pQuery;
		m_arQueryData.RemoveAt(ii);
	}
}

int CRushMan::AddQuery(CString QueryFile)
{
	CFile		file;
	CFileFind	fileFind;
	CFileException	eFile;
	int		nFileL, nPos, nVal;
	char		*pData, *pTemp, *pTempPos, *pDataEnd, *pTempEnd;
	CQueryData*	pQuery;
	CString		sFileName;

	// Path와 Extension을 제거하고 파일명만 얻는다.
	nPos = QueryFile.ReverseFind('\\');
	if (nPos < 0)
		return -1;

	sFileName = QueryFile.Mid(nPos + 1);
	nPos = sFileName.ReverseFind('.');
	if (nPos < 0)
		return -1;
	
	sFileName = sFileName.Left(nPos);
	if (sFileName.IsEmpty())
		return -1;

	if (!fileFind.FindFile(QueryFile))
		return -1;

	if (!file.Open(QueryFile, CFile::modeRead, &eFile))
	{
		CString sErr;
		sErr.Format("Fail to Add File(%s)", QueryFile);
		AfxMessageBox(sErr);
		return -1;
	}
	file.SeekToBegin();
	nFileL = file.GetLength();
	pData  = new char[nFileL + 1];
	pTemp  = new char[nFileL + 1];
	file.Read(pTemp, nFileL);
	pTemp[nFileL] = 0x00;

	pDataEnd = pData;
	pTempEnd = pTemp;

	pTempPos = strchr(pTemp, '\\');
	while (pTempPos)
	{
		if (pTempPos[1] == 'x')
		{
			if((((pTempPos[2] >= '0') && (pTempPos[2] <= '9')) || ((pTempPos[2] >= 'a') && (pTempPos[2] <= 'f')) || ((pTempPos[2] >= 'A') && (pTempPos[2] <= 'F'))) &&
			   (((pTempPos[3] >= '0') && (pTempPos[3] <= '9')) || ((pTempPos[3] >= 'a') && (pTempPos[3] <= 'f')) || ((pTempPos[3] >= 'A') && (pTempPos[3] <= 'F'))))
			{
				strncpy(pDataEnd, pTempEnd, pTempPos - pTempEnd);
				pDataEnd += pTempPos - pTempEnd;

				if (pTempPos[2] <= '9')
					nVal = pTempPos[2] - '0';
				else if (pTempPos[2] <= 'F')
					nVal = pTempPos[2] - 'A' + 10;
				else 
					nVal = pTempPos[2] - 'a' + 10;
				nVal *= 16;

				if (pTempPos[3] <= '9')
					nVal += pTempPos[3] - '0';
				else if (pTempPos[3] <= 'F')
					nVal += pTempPos[3] - 'A' + 10;
				else 
					nVal += pTempPos[3] - 'a' + 10;

				pDataEnd[0] = (char)nVal;
				pDataEnd++;
				pDataEnd[0] = 0x00;

				pTempPos += 4;
				pTempEnd = pTempPos;
			}
			else
				pTempPos++;
		}
		else if ((pTempPos[1] == 'n') || (pTempPos[1] == 'r') || (pTempPos[1] == 't'))
		{
			strncpy(pDataEnd, pTempEnd, pTempPos - pTempEnd);
			pDataEnd += pTempPos - pTempEnd;
			switch(pTempPos[1])
			{
			case 'n': pDataEnd[0] = 0x0a; break;
			case 'r': pDataEnd[0] = 0x0d; break;
			case 't': pDataEnd[0] = 0x09; break;
			}
			pDataEnd++;
			pDataEnd[0] = 0x00;

			pTempPos += 2;
			pTempEnd = pTempPos;
		}
		else
			pTempPos++;

		pTempPos = strchr(pTempPos, '\\');
	}

	
	if (strlen(pTempEnd))
	{
		strncpy(pDataEnd, pTempEnd, strlen(pTempEnd));
		pDataEnd[strlen(pTempEnd)] = 0x00;
	}

	delete[] pTemp;

	pQuery = new CQueryData();
	pQuery->m_bOop	 = (sFileName.Mid(2, 1) == "O") || (sFileName.Mid(2, 1) == "o");
	pQuery->m_trCode = sFileName.Mid(3);
	pQuery->m_pData	 = pData;

	m_arQueryData.Add(pQuery);

	return TRUE;
}
//m_arConnector
void CRushMan::Send(int icnt, int igubn, int iQueryInterval)
{
	for(int ii = 0 ; ii < m_arConnector.GetSize() ; ii++)
	{
		CConnector* pCon;
		pCon = (CConnector*)m_arConnector.GetAt(ii);
		pCon->m_iCount= icnt;
		pCon->m_iQueryGubn = igubn;
		pCon->m_nQueryInterval = iQueryInterval;
		pCon->DoQuery();
	}
}