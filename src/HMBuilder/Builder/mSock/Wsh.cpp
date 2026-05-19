// Wsh.cpp : implementation file
//

#include "stdafx.h"
#include "Wsh.h"
#include "Zip.h"
#include "../h/axis.h"
#include "../h/axisfm.h"
#include "../h/axisrsm.h"
#include "../h/axisfire.h"
#include "../h/axisanm.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	snAXIS	"updateAXIS"
#define	snRSC	"updateRSC"
#define	nEXTRA	32 * 1024

/////////////////////////////////////////////////////////////////////////////
// CWsh

IMPLEMENT_DYNCREATE(CWsh, CWinThread)

CWsh::CWsh()
{
	m_state   = stNONE;
	m_current = 0;
	m_order   = 0;
	m_bytes   = 0;
	m_datb    = NULL;

	m_list.RemoveAll();
	m_zip = new CZip();
	m_axisOnly = false;
	m_que.RemoveAll();
	m_queRTM.RemoveAll();
}

CWsh::~CWsh()
{
	ClearUpdateList();

	delete m_zip;
	if (m_datb)
		delete[] m_datb;
}

BOOL CWsh::InitInstance()
{
	m_alive = true;
	return TRUE;
}

int CWsh::ExitInstance()
{
	for (int ii = (int)m_que.GetUpperBound(); ii >= 0; ii--)
	{
		CMQue* pQue =  (CMQue*)m_que.GetAt(ii);
		delete pQue;
	}
	m_que.RemoveAll();

	for (int ii = (int)m_queRTM.GetUpperBound(); ii >= 0; ii--)
	{
		CMQue* pQue =  (CMQue*)m_queRTM.GetAt(ii);
		delete pQue;
	}
	m_queRTM.RemoveAll();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CWsh, CWinThread)
	//{{AFX_MSG_MAP(CWsh)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWsh message handlers

int CWsh::Run()
{
	CMQue *pMQ;
	CSingleLock	syncLock(&m_event);

	while (m_alive)
	{
		if (m_que.GetUpperBound() >= 0)
		{
			m_section.Lock();
			pMQ = (CMQue *)m_que.GetAt(0);
			m_que.RemoveAt(0);
			m_section.Unlock();

			DoParse(pMQ);
	//		delete [] pMQ->m_pBytes;
			delete pMQ;
			continue;
		}

		if (m_queRTM.GetUpperBound() >= 0)
		{
			m_sectionRTM.Lock();
			pMQ = (CMQue *)m_queRTM.GetAt(0);
			m_queRTM.RemoveAt(0);
			m_sectionRTM.Unlock();

			DoParse(pMQ);
	//		delete [] pMQ->m_pBytes;
			delete pMQ;
			continue;
		}

		syncLock.Lock(3000);
		syncLock.Unlock();
	}
	return 0;
}

void CWsh::Dispatch(int ssm, char* pBytes, int nBytes)
{
	CMQue*	mque = new CMQue();

	if (pBytes && nBytes > 0)
	{
		mque->m_pBytes = new char[nBytes];
		CopyMemory(mque->m_pBytes, pBytes, nBytes);
		if (ssm == ssM_RM) 
		{
			switch (((struct _rsmH *)pBytes)->resK)
			{
			case resK_LRSC:
//			case resK_AXIS:
				if (((struct _rsmH *)pBytes)->resF != resF_ONLY && ((struct _rsmH *)pBytes)->resF != resF_LAS)
					break;
				DoRequest();
				break;
			case resK_RSP:
				if (m_state == stRSC)
					DoRequest();
				break;
			}
		}
	}

	mque->m_ssm = ssm;
	mque->m_nBytes = nBytes;

	if (ssm == ssM_SM)
	{
		m_sectionRTM.Lock();
		m_queRTM.Add(mque);
		m_sectionRTM.Unlock();
	}
	else
	{
		m_section.Lock();
		m_que.Add(mque);
		m_section.Unlock();
	}
	m_event.SetEvent();
}

void CWsh::DoParse(CMQue* pMQ)
{
	switch (pMQ->m_ssm)
	{
	case ssM_OPEN:		// OnConnect(errcode)
		m_pWnd->PostMessage(WFM_EVENT, FEV_OPEN, pMQ->m_nBytes);
		return;
	case ssM_CLOSE:		// OnClose()
		if (m_state == stRSC)
			DoClose();
		m_pWnd->PostMessage(WFM_EVENT, FEV_CLOSE, 0);
		return;
	case ssM_ERROR:		// OnError()
		m_pWnd->SendMessage(WFM_EVENT, FEV_ERROR, (LPARAM)pMQ->m_pBytes);
		return;
	case ssM_RM:		// OnResource()
		break;
	case ssM_UD:		// OnAlert()
		m_pWnd->SendMessage(WFM_PUSH, (WPARAM)pMQ->m_nBytes, (LPARAM)pMQ->m_pBytes);
		return;
	case ssM_SM:		// OnFlash()
		m_pWnd->SendMessage(WFM_ALERT, (WPARAM)pMQ->m_nBytes, (LPARAM)pMQ->m_pBytes);
		return;
	case ssM_SN:		// OnUpdate(MAP)
	case ssM_AP:		// OnApprove()
	default:
		m_pWnd->SendMessage(WFM_AXIS, (WPARAM)pMQ->m_nBytes, (LPARAM)pMQ->m_pBytes);
		return;
	}

	CString	path, tmps;
	CFile	fileH;
	int	bytes;

	struct _rsmH *rsmH = (struct _rsmH *) pMQ->m_pBytes;
	if (rsmH->dirF != dirF_OUTB || m_state == stERR)
		return;

	bytes = atoi(CString(rsmH->datL, sizeof(rsmH->datL)));
	switch (rsmH->resK)
	{
	case resK_REQ:
	case resK_REQ2:
		if (!MakeDir(rsmH, path))
			return;
		switch (rsmH->resF)
		{
		case resF_ONLY:
		case resF_FIR:
			if (!fileH.Open(path, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
				return;
			break;
		case resF_MID:
		case resF_LAS:
			if (fileH.Open(path, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite|CFile::modeNoTruncate))
			{
				fileH.SeekToEnd();
				break;
			}
		default:
			return;
		}
		fileH.Write(&pMQ->m_pBytes[L_rsmH], bytes);
		fileH.Close();

		if (rsmH->resF == resF_ONLY || rsmH->resF == resF_LAS)
		{
			switch (rsmH->resK)
			{
			case resK_REQ:
				if (MakeUpdateList())
					DoRequest();
				else
				{
					ClearUpdateList();
					if (m_axisOnly)
					{
						m_state = stDONE;
						m_pWnd->PostMessage(WFM_EVENT, FEV_RUN, (LPARAM)false);
					}
					else
						DoRequest(!GetUpdateList());
				}
				break;
			case resK_REQ2:
				if (MakeUpdateList())
					DoRequest();
				else
				{
					m_state = stDONE;
					m_pWnd->PostMessage(WFM_EVENT, FEV_RUN, (LPARAM)false);
				}
				break;
			}
		}
		return;

	case resK_RSP:
		switch (m_state)
		{
		case stAXIS:
			m_state = stERR;
			tmps.Format("[%s] Check server resource management program", m_list.GetAt(m_current)->m_name);
			m_pWnd->SendMessage(WFM_EVENT, FEV_ERROR, (LPARAM)tmps.operator LPCTSTR());
			break;
		case stRSC:
			Update(pMQ->m_pBytes, 0);
			break;
		}
		return;

	case resK_ERR:
	case resK_ERR2:
		tmps = CString(&pMQ->m_pBytes[L_rsmH], bytes);
		m_pWnd->SendMessage(WFM_EVENT, FEV_ERROR, (LPARAM)tmps.operator LPCTSTR());
		return;

//	case resK_AXIS:
	case resK_LRSC:
		switch (rsmH->resF)
		{
		case resF_ONLY:
		case resF_FIR:
			if (m_datb)
				delete[] m_datb;
			m_bytes = 0;
			m_datb  = new char[m_list.GetAt(m_current)->m_size + nEXTRA];
		case resF_LAS:
		case resF_MID:
			CopyMemory(&m_datb[m_bytes], &pMQ->m_pBytes[L_rsmH], bytes);
			m_bytes += bytes;
			break;
		case resF_RM:
			if (MakeDir(rsmH, path))
				DeleteFile(path);
		default:
			return;
		}
		break;
	default:
		return;
	}

	BOOL	update = FALSE;
	if (rsmH->resF == resF_LAS || rsmH->resF == resF_ONLY)
	{
		if (!MakeDir(rsmH, path, (rsmH->resK == resK_LRSC) ? true : false))
			return;

		if (!fileH.Open(path, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
			return;
		fileH.Write(m_datb, m_bytes);
		fileH.Close();

		if (rsmH->resK == resK_LRSC)
		{
			MakeDir(rsmH, tmps);
			if (rsmH->resC & resC_ZIP)
			{
				update = m_zip->Decompress((char *)path.operator LPCTSTR(), (char *)tmps.operator LPCTSTR());
				DeleteFile(path);
			}
			else
			{
				DeleteFile(tmps);
				update = MoveFile(path, tmps);
			}
		}
	}
	Update(pMQ->m_pBytes, bytes, update);
	return;
}

bool CWsh::MakeDir(_rsmH * rsmH, CString& path, bool temporary)
{
//	int	pos;
	CString	tmps;

	switch (rsmH->resK)
	{
	case resK_REQ:
		path.Format("%s/%s/infoAXIS.new", m_home, TABDIR);
		break;
	case resK_REQ2:
		path.Format("%s/%s/infoRSC.new", m_home, TABDIR);
		break;
	case resK_LRSC:
/*
		tmps = rsmH->resN;
		path = m_home + '/';
		pos = tmps.Find('/');
		while (pos >= 0)
		{
			path += tmps.Left(pos);
			tmps = tmps.Mid(pos+1);
			if (!IsDir(path))
				return false;

			path += '/';
			pos = tmps.Find('/');
		}
		path += tmps;
		if (temporary)
			path += _T(".tmp");
*/
		break;
//	case resK_AXIS:
/*
		tmps = rsmH->resN;
		pos  = tmps.ReverseFind('/');
		tmps = tmps.Mid(pos+1);
		if (tmps.CompareNoCase("axisver.exe"))
			path.Format("%s/%s/%s", m_home, RUNDIR, _T("axis"));
		else
			path.Format("%s/%s", m_home, RUNDIR);
		if (!IsDir(path))
			return false;
		path += '/';
		path += tmps;
*/
//		break;
	default:
		return false;
	}

	return true;
}

BOOL CWsh::IsDir(CString path)
{
	DWORD	fattr;

	fattr = GetFileAttributes(path);
	if (fattr == 0xffffffff)
		return CreateDirectory(path, NULL);

	return (fattr & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL CWsh::DoRequest(bool info)
{
	char	wb[256];
	CString	str;
	int	datL;

	if (m_state == stDONE)
		return FALSE;

	datL = L_rsmH;
	ZeroMemory(wb, L_rsmH);
	if (info)
	{
		((struct _rsmH *)wb)->dirF = dirF_INB;
		((struct _rsmH *)wb)->resK = resK_REQ2;
		((struct _rsmH *)wb)->resC = resC_INFO;
	}
	else
	{
		if (++m_order > m_list.GetSize())
			return FALSE;

		str  = m_list.GetAt(m_order-1)->m_name;
		((struct _rsmH *)wb)->dirF = dirF_INB;
//		((struct _rsmH *)wb)->resK = (m_state == stAXIS) ? resK_AXIS : resK_RSC;
//		CopyMemory(((struct _rsmH *)wb)->resN, str, str.GetLength());
	}

	m_pWnd->SendMessage(WFM_WRITE, datL, (LPARAM)wb);
	return TRUE;
}

void CWsh::Update(char* pBytes, int nBytes, BOOL update)
{
	if (m_state == stDONE)
		return;

	int	rate, pos;
	CString	path, str;
	struct	_rsmH*	rsmH = (struct _rsmH *)pBytes;

	if (rsmH->resK == resK_RSP || rsmH->resF == resF_FIR || rsmH->resF == resF_ONLY)
		m_current++;

	if (rsmH->resK == resK_RSP && m_current <= m_list.GetSize() && m_list.GetAt(m_current-1)->m_size > 0)
	{
		rate   = 100;
		nBytes = m_list.GetAt(m_current-1)->m_size;
	}
	else if (m_current <= m_list.GetSize() && m_list.GetAt(m_current-1)->m_size > 0)
	{
		if (rsmH->resF == resF_LAS || rsmH->resF == resF_ONLY)
			rate = 100;
		else
			rate = (int)((m_bytes*100) / (m_list.GetAt(m_current-1)->m_size));
	}
	else
		rate = 100;

	if (rsmH->resK == resK_RSP || rsmH->resF == resF_LAS || rsmH->resF == resF_ONLY)
	{
		path.Format("%s/%s/updateX", m_home, TABDIR);
		str.Format("%d", m_current);
		WritePrivateProfileString((m_state == stAXIS) ? snAXIS : snRSC, _T("done"), str, path);

		if (update && m_state == stRSC && rsmH->resK != resK_RSP)
		{
			str.Format("%s\n", m_list.GetAt(m_current-1)->m_info);
			m_file.WriteString(str);
		}
	}

//	if (rsmH->resK == resK_RSP)
//		path =  m_list.GetAt(m_current-1)->m_name;
//	else
//		path = rsmH->resN;
	pos  = path.ReverseFind('/');
	if (pos != -1)
		path = path.Mid(pos+1);
	str.Format("%s\t%d", path, nBytes);
	m_pWnd->SendMessage(WFM_STAT, rate, (LPARAM)str.operator LPCTSTR());

	if ((rsmH->resK == resK_RSP || rsmH->resF == resF_LAS || rsmH->resF == resF_ONLY) && m_current >= m_list.GetSize())
	{
		if (m_state == stAXIS)
		{
			path.Format("%s/%s/infoAXIS", m_home, TABDIR);
			str.Format("%s/%s/infoAXIS.new", m_home, TABDIR);
		}
		else
		{
			CString	text;

			m_file.Close();
			path.Format("%s/%s/infoRSC", m_home, TABDIR);
			str.Format("%s/%s/infoRSC.tmp", m_home, TABDIR);
			text.Format("%s/%s/infoRSC.new", m_home, TABDIR);
			if (GetFileAttributes(str) == 0xffffffff)
				str = text;
			else
				DeleteFile(text);
		}

		if (GetFileAttributes(str) != 0xffffffff)
		{
			DeleteFile(path);
			MoveFile(str, path);
		}
		path.Format("%s/%s/%s", m_home, TABDIR, (m_state == stAXIS) ? snAXIS : snRSC);
		DeleteFile(path);

		path.Format("%s/%s/updateX", m_home, TABDIR);
		WritePrivateProfileSection((m_state == stAXIS) ? snAXIS : snRSC, _T(""), path);
		m_pWnd->PostMessage(WFM_EVENT, FEV_RUN, (LPARAM)(m_state == stAXIS) ? true : false);
		m_state = stDONE;
	}
}

bool CWsh::GetUpdateList()
{
	char	wb[256];
	int	rc, count;
	CString	path, infos, str, section, text;

	if (m_state == stNONE || m_state == stDONE)
	{
		m_state = stAXIS;
		section = snAXIS;
	}
	else
	{
		m_state = stRSC;
		section = snRSC;
		
		path.Format("%s/%s/%s", m_home, RUNDIR, MENUDAT);
		if (GetFileAttributes(path) == 0xffffffff)
			return false;
	}

	infos.Format("%s/%s/%s", m_home, TABDIR, section);
	if (GetFileAttributes(infos) == 0xffffffff)
		return false;

	path.Format("%s/%s/updateX", m_home, TABDIR);
	rc = GetPrivateProfileString(section, _T("done"), _T(""), wb, sizeof(wb), path);
	if (rc <= 0)
		return false;

	m_order = atoi(wb);
	rc = GetPrivateProfileString(section, _T("count"), _T(""), wb, sizeof(wb), path);
	count = atoi(wb);
	if (m_order == count)
		return false;

	CFileStatus	fs;
	int	value, total;
	time_t	utime, ctime;

	if (CFile::GetStatus(path, fs))
	{
		utime = fs.m_mtime.GetTime();
		time(&ctime);
		if (ctime - utime > 60)
			return false;
	}

	CStdioFile file;
	char	twb[64];
	int	size, vers, key;

	if (!file.Open(infos, CFile::modeRead))
		return false;

	m_list.RemoveAll();
	value = total = 0;
	for (int ii = 0; ii < count; ii++)
	{
		if (!file.ReadString(str))
			break;

		vers = size = 0;
		sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %d %s", wb, sizeof(wb), &vers, &key, &size, twb, sizeof(twb));
		if (size == 0)
			continue;
		text.Format("%s/%s", twb, wb);

		Cupdate* update = new Cupdate;
		update->m_name = text;
		update->m_size = size;
		update->m_info = str;
		if (ii >= m_order)
		{
			value += update->m_size;
			total++;
		}

		m_list.Add(update);
	}
	file.Close();
	if (value <= 0)
	{
		ClearUpdateList();
		return false;
	}

	str.Format("%s/%s/infoRSC.tmp", m_home, TABDIR);
	if (!m_file.Open(str, CFile::modeWrite))
		return false;

	m_file.SeekToEnd();

	m_current = m_order;
	str.Format("%d\t%d", total, value);
	m_pWnd->SendMessage(WFM_EVENT, FEV_SIZE, (LPARAM)str.operator LPCTSTR());

	if (m_state == stRSC)
	{
		path.Format("%s/%s/infoRSC", m_home, TABDIR);
		LoadVersion(path);
	}
	return true;
}

class Citem : public CObject
{
public:
	int	m_vers;
	int	m_key;
};

bool CWsh::MakeUpdateList()
{
	CString	path, infos, str, text;
	char	wb[256];
	int	vers, size, value;
	CStringArray	arr;
	Citem*	item;
	Cupdate* update;
	CMapStringToOb	clist;
	CStdioFile file;

	ClearUpdateList();
	clist.RemoveAll();
	m_order = m_current = 0;
	if (m_state == stAXIS)
	{
		path.Format("%s/%s/infoAXIS", m_home, TABDIR);
		LoadFile(path, arr);
		for (int ii = 0; ii < arr.GetSize(); ii++)
		{
			str  = arr.GetAt(ii);
			str.TrimLeft(); str.TrimRight();
			if (str.IsEmpty())
				continue;

			vers = size = 0;
			sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d", wb, sizeof(wb), &vers, &size);
			if (!strlen(wb))
				continue;

			item = new Citem;
			item->m_vers = vers;
			clist.SetAt(wb, item);
		}

		path.Format("%s/%s/infoAXIS.new", m_home, TABDIR);
		LoadFile(path, arr);

		value = 0;
		path.Format("%s/%s/updateX", m_home, TABDIR);
		WritePrivateProfileSection(snAXIS, _T(""), path);
		infos.Format("%s/%s/%s", m_home, TABDIR, snAXIS);
		if (!file.Open(infos, CFile::modeCreate|CFile::modeWrite))
			return false;

		for (int ii = 0; ii < arr.GetSize(); ii++)
		{
			str  = arr.GetAt(ii);
			str.TrimLeft(); str.TrimRight();
			if (str.IsEmpty())
				continue;

			vers = size = 0;
			sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d", wb, sizeof(wb), &vers, &size);
			if (!strlen(wb))
				continue;

			if (clist.Lookup(wb, (CObject *&)item))
			{
				if (vers <= item->m_vers)
					continue;
			}

			if (size == 0)
			{
				str.Format("%s/%s/%s", m_home, RUNDIR, wb);
				DeleteFile(str);
				continue;
			}

			str.Format("%s/%s@%d", RUNDIR, wb, vers);
			update = new Cupdate;
			update->m_name = str;
			update->m_size = size;
			m_list.Add(update);
			value += size;

			str.Format("%d", m_list.GetSize());
			text.Format("%s:%d\n", update->m_name, update->m_size);
			file.WriteString(text);
		}
		file.Close();

		for (POSITION pos = clist.GetStartPosition(); pos != NULL; )
		{
			clist.GetNextAssoc(pos, str, (CObject *&)item);
			delete item;
		}
		clist.RemoveAll();

		if (m_list.GetSize() > 0 && value > 0)
		{
			WritePrivateProfileString(snAXIS, _T("done"), _T("0"), path);
			str.Format("%d", m_list.GetSize());
			WritePrivateProfileString(snAXIS, _T("count"), str, path);

			str.Format("%d\t%d", m_list.GetSize(), value);
			m_pWnd->SendMessage(WFM_EVENT, FEV_SIZE, (LPARAM)str.operator LPCTSTR());
			return true;
		}

		path.Format("%s/%s/infoAXIS", m_home, TABDIR);
		str.Format("%s/%s/infoAXIS.new", m_home, TABDIR);
		if (GetFileAttributes(str) != 0xffffffff)
		{
			DeleteFile(path);
			MoveFile(str, path);
		}
		DeleteFile(infos);
		return false;
	}

	char	twb[64];
	int	key, count;
	struct	_verM*	verM;
	bool	force = false;

	path.Format("%s/%s/%s", m_home, RUNDIR, MENUDAT);
	if (GetFileAttributes(path) == 0xffffffff)
		force = true;

	path.Format("%s/%s/infoRSC", m_home, TABDIR);
	LoadFile(path, arr);
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		str  = arr.GetAt(ii);
		str.TrimLeft(); str.TrimRight();
		if (str.IsEmpty())
			continue;

		vers = size = key = 0;
		sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %d %s", wb, sizeof(wb), &vers, &key, &size, twb, sizeof(twb));
		if (!strlen(wb) || !strlen(twb))
			continue;

		if (force)
		{
			str = wb;
			if (!str.CompareNoCase(MENUDAT))
				continue;
		}

		str.Format("%s/%s", twb, wb);
		item = new Citem;
		item->m_vers = vers;
		item->m_key  = key;
		clist.SetAt(str, item);
	}

	path.Format("%s/%s/infoRSC.new", m_home, TABDIR);
	LoadFile(path, arr);
	if (arr.GetSize() > 0)
		verM = (struct _verM *)new char[sizeof(struct _verM) * arr.GetSize()];

	infos.Format("%s/%s/%s", m_home, TABDIR, snRSC);
	if (!file.Open(infos, CFile::modeCreate|CFile::modeWrite))
		return false;

	path.Format("%s/%s/infoRSC.tmp", m_home, TABDIR);
	m_file.Open(path, CFile::modeCreate|CFile::modeWrite);

	value = count = 0;
	path.Format("%s/%s/updateX", m_home, TABDIR);
	WritePrivateProfileSection(snRSC, _T(""), path);
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		str  = arr.GetAt(ii);
		str.TrimLeft(); str.TrimRight();
		if (str.IsEmpty())
			continue;

		vers = size = 0;
		sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %d %s", wb, sizeof(wb), &vers, &key, &size, twb, sizeof(twb));
		if (!strlen(wb) || !strlen(twb))
			continue;

		text.Format("%s/%s", twb, wb);
		if (text.Find("map/") == 0 && size)
		{
			CopyMemory(verM[count].name, wb, L_MAPN);
			verM[count].ver = vers;
			count++;
		}

		if (clist.Lookup(text, (CObject *&)item))
		{
			if (!(vers > item->m_vers || (vers == item->m_vers && key > item->m_key)))
			{
				str += '\n';
				m_file.WriteString(str);
				continue;
			}
		}

		if (size == 0)
		{
			CString	tmps;
			tmps.Format("%s/%s/%s", m_home, twb, wb);
			DeleteFile(tmps);
			continue;
		}

		update = new Cupdate;
		update->m_name = text;
		update->m_size = size;
		update->m_info = str;
		value += size;
		m_list.Add(update);
		str += '\n';
		file.WriteString(str);
	}
	file.Close();
	if (count > 0)
		m_pWnd->SendMessage(WFM_VERS, count, (LPARAM)verM);

	if (arr.GetSize() > 0)
		delete[] verM;

	for (POSITION pos = clist.GetStartPosition(); pos != NULL; )
	{
		clist.GetNextAssoc(pos, str, (CObject *&)item);
		delete item;
	}
	clist.RemoveAll();

	if (m_list.GetSize() > 0 && value > 0)
	{
		WritePrivateProfileString(snRSC, _T("done"), _T("0"), path);
		str.Format("%d", m_list.GetSize());
		WritePrivateProfileString(snRSC, _T("count"), str, path);

		str.Format("%d\t%d", m_list.GetSize(), value);
		m_pWnd->SendMessage(WFM_EVENT, FEV_SIZE, (LPARAM)str.operator LPCTSTR());
		return true;
	}
	m_file.Close();
	path.Format("%s/%s/infoRSC", m_home, TABDIR);
	str.Format("%s/%s/infoRSC.tmp", m_home, TABDIR);
	text.Format("%s/%s/infoRSC.new", m_home, TABDIR);
	if (GetFileAttributes(str) == 0xffffffff)
		str = text;
	else
		DeleteFile(text);

	if (GetFileAttributes(str) != 0xffffffff)
	{
		DeleteFile(path);
		MoveFile(str, path);
	}
	DeleteFile(infos);
	return false;
}

void CWsh::ClearUpdateList()
{
	for (int ii = 0; ii < m_list.GetSize(); ii++)
		delete m_list.GetAt(ii);
	m_list.RemoveAll();
}

int CWsh::LoadFile(CString path, CStringArray& arr)
{
	CString	tmps;
	CStdioFile	file;

	arr.RemoveAll();
	if (!file.Open(path, CFile::modeRead|CFile::typeText))
		return 0;

	while (file.ReadString(tmps))
	{
		tmps.TrimLeft();
		if (!tmps.IsEmpty())
			arr.Add(tmps);
	}
	file.Close();
	return (int)arr.GetSize();
}

void CWsh::LoadVersion(CString path)
{
	CStringArray	arr;

	if (LoadFile(path, arr) > 0)
	{
		CString	str;
		char	wb[128], twb[128];
		int	vers, key, size, count = 0;
		struct	_verM*	verM;

		verM = (struct _verM *)new char[sizeof(struct _verM) * arr.GetSize()];
		for (int ii = 0, count = 0; ii < arr.GetSize(); ii++)
		{
			str  = arr.GetAt(ii);
			vers = size = key = 0;
			sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %d %s", wb, sizeof(wb), &vers, &key, &size, twb, sizeof(twb));
			if (size == 0)
			{
				str.Format("%s/%s/%s", m_home, twb, wb);
				DeleteFile(str);
				continue;
			}
			if (strlen(wb) && strlen(twb) && vers)
			{
				CopyMemory(verM[count].name, wb, L_MAPN);
				verM[count].ver = vers;
				count++;
			}
		}

		if (count > 0)
			m_pWnd->SendMessage(WFM_VERS, count, (LPARAM)verM);
		delete[] verM;
	}
}

void CWsh::DoClose()
{
	CString	path, text;

	m_file.Close();
	path.Format("%s/%s/infoRSC", m_home, TABDIR);
	text.Format("%s/%s/infoRSC.tmp", m_home, TABDIR);
	if (GetFileAttributes(text) != 0xffffffff)
	{
		DeleteFile(path);
		MoveFile(text, path);
	}
}

int CWsh::GetQueuingCount()
{
	int	count;

	m_section.Lock();
	count = (int)m_que.GetSize();
	m_section.Unlock();
	return count;
}

CMQue::CMQue()
{
	m_ssm    = 0;
	m_nBytes = 0;
	m_pBytes = (char *) 0;
}

CMQue::~CMQue()
{
	if (m_pBytes)
		delete [] m_pBytes;
}
