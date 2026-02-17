// Wsh.cpp : implementation file
//

#include "pch.h"
#include "Sock.h"
#include "Wsh.h"
#include "Zip.h"
#include "../h/axis.h"
#include "../h/axisfm.h"
#include "../h/axisrsm.h"
#include "../h/axisfire.h"
#include "../h/axisanm.h"
#include "../h/axisvar.h"

#include "Log.h"

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
	m_state   = stFlag::stNONE;
	m_current = 0;
	m_order   = 0;
	m_bytes   = 0;
	m_datb    = NULL;

	m_list.RemoveAll();
	m_zip = new CZip();
	m_axisOnly = false;
	m_que.RemoveAll();
	m_queRTM.RemoveAll();

	m_running = false;
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
	for (int ii = m_que.GetUpperBound(); ii >= 0; ii--)
		delete m_que.GetAt(ii);
	m_que.RemoveAll();

	for (int ii = m_queRTM.GetUpperBound(); ii >= 0; ii--)
		delete m_queRTM.GetAt(ii);
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
		if (m_que.GetUpperBound() < 0)
		{
			syncLock.Lock(3000);
			syncLock.Unlock();
			continue;
		}
		m_section.Lock();
		pMQ = (CMQue *)m_que.GetAt(0);
		m_que.RemoveAt(0);
		m_section.Unlock();

		if (pMQ->m_ssm != ssM_SM)
		{
			DoParse(pMQ);
			delete pMQ;
		}
		else	DoParse(pMQ);
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
			case resK_RSC:
			case resK_AXIS:
				if (((struct _rsmH *)pBytes)->resF != resF_ONLY && ((struct _rsmH *)pBytes)->resF != resF_LAS)
					break;
				DoRequest();
				break;
			case resK_RSP:
				if (m_state == stFlag::stRSC)
					DoRequest();
				break;
			}
		}
	}

	mque->m_ssm = ssm;
	mque->m_nBytes = nBytes;

	m_section.Lock();
	m_que.Add(mque);
	m_section.Unlock();
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
		if (m_state == stFlag::stRSC)
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
		ParseRTM(pMQ);
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
	if (rsmH->dirF != dirF_OUTB || m_state == stFlag::stERR)
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
						m_state = stFlag::stDONE;
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
					m_state = stFlag::stDONE;
					m_pWnd->PostMessage(WFM_EVENT, FEV_RUN, (LPARAM)false);
				}
				break;
			}
		}
		return;

	case resK_RSP:
		switch (m_state)
		{
		case stFlag::stAXIS:
			m_state = stFlag::stERR;
			tmps.Format(_T("[%s] Check server resource management program"), LPCTSTR(m_list.GetAt(m_current)->m_name));
			m_pWnd->SendMessage(WFM_EVENT, FEV_ERROR, (LPARAM)tmps.operator LPCTSTR());
			break;
		case stFlag::stRSC:
			Update(pMQ->m_pBytes, 0);
			break;
		}
		return;

	case resK_ERR:
	case resK_ERR2:
		tmps = CString(&pMQ->m_pBytes[L_rsmH], bytes);
		m_pWnd->SendMessage(WFM_EVENT, FEV_ERROR, (LPARAM)tmps.operator LPCTSTR());
		return;

	case resK_AXIS:
	case resK_RSC:
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
		if (!MakeDir(rsmH, path, true))
			return;

		if (!fileH.Open(path, CFile::modeCreate|CFile::typeBinary|CFile::modeWrite))
			return;
		fileH.Write(m_datb, m_bytes);
		fileH.Close();

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

		if (rsmH->resK != resK_RSC)
			update = FALSE;
	}
	Update(pMQ->m_pBytes, bytes, update);
}

void CWsh::ParseRTM(CMQue* pMQ)
{
	m_sectionRTM.Lock();
	m_queRTM.Add(pMQ);
	m_sectionRTM.Unlock();
	m_eventRTM.SetEvent();
}

void CWsh::SendRTM(CMQue* pMQ)
{
	m_pWnd->SendMessage(WFM_EVENT, FEV_ANM, (LPARAM)pMQ);
}

bool CWsh::MakeDir(_rsmH * rsmH, CString& path, bool temporary)
{
	int	pos;
	CString	tmps;

	switch (rsmH->resK)
	{
	case resK_REQ:
		path.Format(_T("%s/%s/infoAXIS.new"), m_home.GetString(), TABDIR);
		break;
	case resK_REQ2:
		path.Format(_T("%s/%s/infoRSC.new"), m_home.GetString(), TABDIR);
		break;
	case resK_RSC:
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
		break;		
	case resK_AXIS:
		tmps = rsmH->resN;
		pos  = tmps.ReverseFind('/');
		tmps = tmps.Mid(pos+1);
		if (tmps.CompareNoCase("axisver.exe"))
			path.Format(_T("%s/%s/%s"), m_home.GetString(), RUNDIR, _T("axis"));
		else
			path.Format(_T("%s/%s"), m_home.GetString(), RUNDIR);
		if (!IsDir(path))
			return false;
		path += '/';
		path += tmps;
		if (temporary)
			path += _T(".tmp");
		break;
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

	if (m_state == stFlag::stDONE)
		return FALSE;

	datL = L_rsmH;
	ZeroMemory(wb, L_rsmH);
	if (info)
	{
		((struct _rsmH *)wb)->dirF = dirF_INB;
		((struct _rsmH *)wb)->resK = resK_REQ2;
		((struct _rsmH *)wb)->resC = resC_INFO|resC_EXT;
		((struct _rsmH*)wb)->rsvB[0] = rsvB_NEW;
	}
	else
	{
		if (++m_order > m_list.GetSize())
			return FALSE;

		struct	_rsmH*	rsmH = (struct _rsmH *)wb;

		str  = m_list.GetAt(m_order-1)->m_name;
		rsmH->dirF = dirF_INB;
		rsmH->resC = resC_EXT;
		rsmH->resK = (m_state == stFlag::stAXIS) ? resK_AXIS : resK_RSC;
		CopyMemory(rsmH->resN, str, min(str.GetLength(), sizeof(rsmH->resN)-1));
		rsmH->rsvB[0] = rsvB_NEW;
	}

	m_pWnd->SendMessage(WFM_WRITE, datL, (LPARAM)wb);
	return TRUE;
}

void CWsh::Update(char* pBytes, int nBytes, BOOL update)
{
	if (m_state == stFlag::stDONE)
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
		path.Format(_T("%s/%s/updateX"), m_home.GetString(), TABDIR);
		str.Format(_T("%d"), m_current);
		WritePrivateProfileString((m_state == stFlag::stAXIS) ? snAXIS : snRSC, _T("done"), str, path);

		if (update && m_state == stFlag::stRSC && rsmH->resK != resK_RSP)
		{
			str.Format(_T("%s\n"), LPCTSTR(m_list.GetAt(m_current-1)->m_info));
			m_file.WriteString(str);
		}
	}

	if (rsmH->resK == resK_RSP)
		path =  m_list.GetAt(m_current-1)->m_name;
	else
		path = rsmH->resN;
	pos  = path.ReverseFind('/');
	if (pos != -1)
		path = path.Mid(pos+1);
	str.Format(_T("%s\t%d"), path.GetString(), nBytes);
	m_pWnd->SendMessage(WFM_STAT, rate, (LPARAM)(char *)str.operator LPCTSTR());

	if ((rsmH->resK == resK_RSP || rsmH->resF == resF_LAS || rsmH->resF == resF_ONLY) && m_current >= m_list.GetSize())
	{
		if (m_state == stFlag::stAXIS)
		{
			path.Format(_T("%s/%s/infoAXIS"), m_home.GetString(), TABDIR);
			str.Format(_T("%s/%s/infoAXIS.new"), m_home.GetString(), TABDIR);
		}
		else
		{
			CString	text;

			m_file.Close();
			path.Format(_T("%s/%s/infoRSC"), m_home.GetString(), TABDIR);
			str.Format(_T("%s/%s/infoRSC.tmp"), m_home.GetString(), TABDIR);
			text.Format(_T("%s/%s/infoRSC.new"), m_home.GetString(), TABDIR);
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
		path.Format(_T("%s/%s/%s"), m_home.GetString(), TABDIR, (m_state == stFlag::stAXIS) ? snAXIS : snRSC);
		DeleteFile(path);

		path.Format(_T("%s/%s/updateX"), m_home.GetString(), TABDIR);
		WritePrivateProfileSection((m_state == stFlag::stAXIS) ? snAXIS : snRSC, _T(""), path);
		m_pWnd->PostMessage(WFM_EVENT, FEV_RUN, (LPARAM)(m_state == stFlag::stAXIS) ? true : false);
		m_state = stFlag::stDONE;
	}
}

bool CWsh::GetUpdateList()
{
	char	wb[256] = {0, };
	int	rc, count;
	CString	path, infos, str, section, text;

	if (m_state == stFlag::stNONE || m_state == stFlag::stDONE)
	{
		m_state = stFlag::stAXIS;
		section = snAXIS;
	}
	else
	{
		m_state = stFlag::stRSC;
		section = snRSC;
		
		path.Format(_T("%s/%s/%s"), m_home.GetString(), RUNDIR, MENUDAT);
		if (GetFileAttributes(path) == 0xffffffff)
			return false;
	}

	infos.Format(_T("%s/%s/%s"), m_home.GetString(), TABDIR, section.GetString());
	if (GetFileAttributes(infos) == 0xffffffff)
		return false;

	path.Format(_T("%s/%s/updateX"), m_home.GetString(), TABDIR);
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
		sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d %d %s", wb, _countof(wb), &vers, &key, &size, twb, _countof(twb));
		if (size == 0)
			continue;
		text.Format(_T("%s/%s"), twb, wb);

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

	str.Format(_T("%s/%s/infoRSC.tmp"), m_home.GetString(), TABDIR);
	m_file.Open(str, CFile::modeWrite);
	m_file.SeekToEnd();

	m_current = m_order;
	str.Format(_T("%d\t%d"), total, value);
	m_pWnd->SendMessage(WFM_EVENT, FEV_SIZE, (LPARAM)str.operator LPCTSTR());

	if (m_state == stFlag::stRSC)
	{
		path.Format(_T("%s/%s/infoRSC"), m_home.GetString(), TABDIR);
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
	char	wb[256] = { 0, };
	int	vers, size, value;
	CStringArray	arr;
	Citem*	item;
	Cupdate* update;
	CMapStringToOb	clist;
	CStdioFile file;

	ClearUpdateList();
	clist.RemoveAll();
	m_order = m_current = 0;
	if (m_state == stFlag::stAXIS)
	{
		path.Format(_T("%s/%s/infoAXIS"), m_home.GetString(), TABDIR);
		LoadFile(path, arr);
		for (int ii = 0; ii < arr.GetSize(); ii++)
		{
			str  = arr.GetAt(ii); str.Trim();
			if (str.IsEmpty()) continue;

			vers = size = 0;
			sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d", wb, _countof(wb), &vers, &size);
			if (!strlen(wb))
				continue;

			item = new Citem;
			item->m_vers = vers;
			clist.SetAt(wb, item);
		}

		path.Format(_T("%s/%s/infoAXIS.new"), m_home.GetString(), TABDIR);
		LoadFile(path, arr);

		value = 0;
		path.Format(_T("%s/%s/updateX"), m_home.GetString(), TABDIR);
		WritePrivateProfileSection(snAXIS, _T(""), path);
		infos.Format(_T("%s/%s/%s"), m_home.GetString(), TABDIR, snAXIS);
		if (!file.Open(infos, CFile::modeCreate|CFile::modeWrite))
			return false;

		for (int ii = 0; ii < arr.GetSize(); ii++)
		{
			str  = arr.GetAt(ii);
			str.TrimLeft(); str.TrimRight();
			if (str.IsEmpty())
				continue;

			vers = size = 0;
			sscanf_s((char *)str.operator LPCTSTR(), "%s %d %d", wb, _countof(wb), &vers, &size);
			if (!strlen(wb))
				continue;

			if (clist.Lookup(wb, (CObject *&)item))
			{
				if (vers <= item->m_vers)
					continue;
			}

			if (size == 0)
			{
				str.Format(_T("%s/%s/%s"), m_home.GetString(), RUNDIR, wb);
				DeleteFile(str);
				continue;
			}

			str.Format(_T("%s/%s@%d"), RUNDIR, wb, vers);
			update = new Cupdate;
			update->m_name = str;
			update->m_size = size;
			m_list.Add(update);
			value += size;

			str.Format(_T("%d"), m_list.GetSize());
			text.Format(_T("%s:%d\n"), update->m_name.GetString(), update->m_size);
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
			str.Format(_T("%d"), m_list.GetSize());
			WritePrivateProfileString(snAXIS, _T("count"), str, path);

			str.Format(_T("%d\t%d"), m_list.GetSize(), value);
			m_pWnd->SendMessage(WFM_EVENT, FEV_SIZE, (LPARAM)str.operator LPCTSTR());
			return true;
		}

		path.Format(_T("%s/%s/infoAXIS"), m_home.GetString(), TABDIR);
		str.Format(_T("%s/%s/infoAXIS.new"), m_home.GetString(), TABDIR);
		if (GetFileAttributes(str) != 0xffffffff)
		{
			DeleteFile(path);
			MoveFile(str, path);
		}
		DeleteFile(infos);
		return false;
	}

	char	twb[64], keys[16];
	int	count;
	struct	_verM*	verM = nullptr;
	bool	force = false;
	enum	{ verNONE = 0, verMAP, verDEV } version;

	path.Format(_T("%s/%s/%s"), m_home.GetString(), RUNDIR, MENUDAT);
	if (GetFileAttributes(path) == 0xffffffff)
		force = true;

	path.Format(_T("%s/%s/infoRSC"), m_home.GetString(), TABDIR);
	LoadFile(path, arr);
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		str  = arr.GetAt(ii);
		str.TrimLeft(); str.TrimRight();
		if (str.IsEmpty())
			continue;

		vers = size = 0;
//		sscanf((char *)str.operator LPCTSTR(), "%s %d %s %d %s", wb, &vers, keys, &size, twb);
		sscanf_s((char*)str.operator LPCTSTR(), "%s %d %s %d %s", wb, _countof(wb), &vers, 
								keys, _countof(keys), &size, twb, _countof(twb));

		if (!strlen(wb) || !strlen(keys) || !strlen(twb))
			continue;

		if (force)
		{
			str = wb;
			if (!str.CompareNoCase(MENUDAT))
				continue;
		}

		str.Format(_T("%s/%s"), twb, wb);
		item = new Citem;
		item->m_vers = vers;
		item->m_key  = atoi(keys);
		clist.SetAt(str, item);
	}

	path.Format(_T("%s/%s/infoRSC.new"), m_home.GetString(), TABDIR);
	LoadFile(path, arr);
	if (arr.GetSize() > 0)
		verM = (struct _verM *)new char[sizeof(struct _verM) * arr.GetSize()];

	infos.Format(_T("%s/%s/%s"), m_home.GetString(), TABDIR, snRSC);
	if (!file.Open(infos, CFile::modeCreate|CFile::modeWrite))
		return false;

	path.Format(_T("%s/%s/infoRSC.tmp"), m_home.GetString(), TABDIR);
	m_file.Open(path, CFile::modeCreate|CFile::modeWrite);

	value = count = 0;
	path.Format(_T("%s/%s/updateX"), m_home.GetString(), TABDIR);
	WritePrivateProfileSection(snRSC, _T(""), path);
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		str  = arr.GetAt(ii);
		str.TrimLeft(); str.TrimRight();
		if (str.IsEmpty())
			continue;

		vers = size = 0;
		sscanf_s((char *)str.operator LPCTSTR(), "%s %d %s %d %s", wb, _countof(wb), &vers, 
								keys, _countof(keys), &size, twb, _countof(twb));
		if (!strlen(wb) || !strlen(keys) || !strlen(twb))
			continue;

		if (size == 0)
		{
			text.Format(_T("%s/%s/%s"), m_home.GetString(), twb, wb);
			DeleteFile(text);
			continue;
		}

		version = verNONE;
		text.Format(_T("%s/%s"), twb, wb);
		if (verM && size && (text.Find("map/") == 0 || text.Find("dev/") == 0))
		{
			strcpy_s(verM[count].name, sizeof(verM[count].name), wb);
			CopyMemory(verM[count].vers, keys, L_VERS);
			strcpy_s(verM[count].path, sizeof(verM[count].path), twb);
			verM[count].request = false;
			version = (text.Find("map/") == 0) ? verMAP : verDEV;
		}

		if (clist.Lookup(text, (CObject *&)item))
		{
			if (atoi(keys) <= item->m_key)
			{
				str += '\n';
				m_file.WriteString(str);
				if (version != verNONE)
					count++;
				continue;
			}
		}

		switch (version)
		{
		case verMAP:
			if (m_runDown)
			{
				verM[count].request = true;
				count++;
				continue;
			}
		case verDEV:
			count++;
		default:
			break;
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
	path.Format(_T("%s/%s/infoRSC"), m_home.GetString(), TABDIR);
	str.Format(_T("%s/%s/infoRSC.tmp"), m_home.GetString(), TABDIR);
	text.Format(_T("%s/%s/infoRSC.new"), m_home.GetString(), TABDIR);
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
	return arr.GetSize();
}

void CWsh::LoadVersion(CString path)
{
	CStringArray	arr;

	if (LoadFile(path, arr) > 0)
	{
		CString	str;
		char	wb[128], twb[128], keys[16];
		int	vers, size, count = 0;
		struct	_verM*	verM;

		verM = (struct _verM *)new char[sizeof(struct _verM) * arr.GetSize()];
		for (int ii = 0; ii < arr.GetSize(); ii++)
		{
			str  = arr.GetAt(ii);
			vers = size = 0;
			sscanf_s((char*)str.operator LPCTSTR(), "%s %d %s %d %s", wb, _countof(wb), &vers, 
								keys, _countof(keys), &size, twb, _countof(twb));
			if (size == 0)
			{
				str.Format(_T("%s/%s/%s"), m_home.GetString(), twb, wb);
				DeleteFile(str);
				continue;
			}
			if (strlen(wb) && strlen(twb) && vers)
			{
				CopyMemory(verM[count].name, wb, L_MAPN);
				CopyMemory(verM[count].vers, keys, L_VERS);
				count++;
			}
		}

		if (count > 0)
			m_pWnd->SendMessage(WFM_VERS, count, (LPARAM)verM);
		delete[] verM;
	}
}

int CWsh::GetQueuingCount()
{
	int	count;

	m_sectionRTM.Lock();
	count = m_queRTM.GetSize();
	m_sectionRTM.Unlock();
	return count;
}

void CWsh::SetDownloadMode(bool axisOnly, int runtime)
{
	if (runtime < 0)
		m_axisOnly = axisOnly;
	else
		m_runDown = (runtime > 0) ? true : false;
}

void CWsh::DoClose()
{
	CString	path, text;

	m_file.Close();
	path.Format(_T("%s/%s/infoRSC"), m_home.GetString(), TABDIR);
	text.Format(_T("%s/%s/infoRSC.tmp"), m_home.GetString(), TABDIR);
	if (GetFileAttributes(text) != 0xffffffff)
	{
		DeleteFile(path);
		MoveFile(text, path);
	}
}

UINT dispatchRTM(LPVOID lpvoid)
{
	CWsh* pWsh = (CWsh *) lpvoid;
	if (pWsh == NULL) return 0;

	CMQue *pMQ;
	CSingleLock	syncLock(&pWsh->m_eventRTM);

	while (pWsh->m_running)
	{
		if (pWsh->m_queRTM.GetUpperBound() < 0)
		{
			syncLock.Lock(3000);
			syncLock.Unlock();
			continue;
		}
		pWsh->m_sectionRTM.Lock();
		pMQ = (CMQue *)pWsh->m_queRTM.GetAt(0);
		pWsh->m_queRTM.RemoveAt(0);
		pWsh->m_sectionRTM.Unlock();
		pWsh->SendRTM(pMQ);
		delete pMQ;
	}
	return 0;
}

void CWsh::BeginRTM()
{
	if (m_running) return;

	m_running = true;
	m_thread  = AfxBeginThread((AFX_THREADPROC) dispatchRTM, (LPVOID) this, THREAD_PRIORITY_HIGHEST);
}

void CWsh::TerminateRTM()
{
	if (!m_running)
		return;
	
	m_running = false;
	m_eventRTM.SetEvent();
	m_thread->PostThreadMessage(WM_QUIT, 0, 0);

	switch (WaitForSingleObject(m_thread->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_thread->m_hThread, 0);
		break;
	}
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