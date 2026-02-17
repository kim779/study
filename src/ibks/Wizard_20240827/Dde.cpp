// Dde.cpp : implementation file
//

#include "pch.h"
#include "Wizard.h"
#include "Dde.h"
#include "../h/axisvar.h"
#include "../h/axis.h"
#include "../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_DDE		100
#define	TMI_DDE		1000
#define	prefixDDE	'.'
#define	maxDDE		10000
#define	minDDE		64

static CDde*	mDDE = (CDde*) 0;

/////////////////////////////////////////////////////////////////////////////
// CDde

CDde::CDde(CGuard* guard)
{
	mDDE      = this;
	m_inst    = 0;
	m_dde     = flagDDE::readyDDE;
	m_posting = false;
	m_minor   = 0;
	m_conv    = -1;

	m_service = m_topic = (HSZ)0;

	m_guard = guard;
}

CDde::~CDde()
{
	if (m_dde != flagDDE::readyDDE)
	{
		DdeNameService(m_inst, m_service, 0L, DNS_UNREGISTER);
		DdeFreeStringHandle(m_inst, m_service);
		DdeFreeStringHandle(m_inst, m_topic);
		DdeUninitialize(m_inst);
	}

	m_que.RemoveAll();
	m_keys.RemoveAll();
	m_item.RemoveAll();

	CString	key;
	CPost*	post;

	for (POSITION pos = m_post.GetStartPosition(); pos; )
	{
		m_post.GetNextAssoc(pos, key, (CObject *&)post);
		delete post;
	}
	m_post.RemoveAll();
}


BEGIN_MESSAGE_MAP(CDde, CWnd)
	//{{AFX_MSG_MAP(CDde)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDde message handlers

int CDde::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	m_guard = ((CWizardApp *)AfxGetApp())->GetGuard();
	m_profile.Format(_T("%s\\%s\\axisDDE.ini"), LPCTSTR(m_guard->m_root), TABDIR);

/*	if (!DdeInitialize(&m_inst, (PFNCALLBACK) DdeCallback,
		CBF_FAIL_EXECUTES|CBF_FAIL_POKES|CBF_SKIP_REGISTRATIONS|CBF_SKIP_UNREGISTRATIONS, 0L))
	{
		char	tmpb[64];

		m_guard = ((CWizardApp *)AfxGetApp())->GetGuard();
		m_profile.Format("%s\\%s\\axisDDE.ini", m_guard->m_root, TABDIR);

		m_dde = flagDDE::stopDDE;
		GetPrivateProfileString("Informations", "Service", "AX", tmpb, sizeof(tmpb), m_profile);
		m_service = DdeCreateStringHandle(m_inst, tmpb, CP_WINANSI);
		DdeNameService(m_inst, m_service, 0L, DNS_REGISTER);
		GetPrivateProfileString("Informations", "Topic", "KS", tmpb, sizeof(tmpb), m_profile);
		m_topic = DdeCreateStringHandle(m_inst, tmpb, CP_WINANSI);

		SetTimer(TM_DDE, TMI_DDE, NULL);
		return 0;
	}
	return -1;
	*/
	return 0;
}

void CDde::Service()
{
	CString	topic;
	CPost*	post;
	CString	codes = _T("");
	int	items = 0;
	bool	busy = false;

	m_lock.Lock();
	for (POSITION pos = m_post.GetStartPosition(); pos; )
	{
		m_post.GetNextAssoc(pos, topic, (CObject *&)post);
		codes += topic;
		codes += '\t';
		if (post->m_stat & jobDS)
		{
			if (post->m_stat & (addDS|delDS))
				busy = true;
			continue;
		}
		if (post->m_stat & (addDS|delDS))
		{
			topic += '\t';
			topic += post->m_item;
			if (Write(topic, post->m_key, true))
			{
				post->m_stat &= ~(addDS|delDS);
				post->m_stat |= jobDS;
				items++;
			}
			else	busy = true;
		}
		if (items >= 10)
		{
			busy = true;
			break;
		}
	}

	if (!busy && !codes.IsEmpty())
	{
		for (m_minor = 0; !codes.IsEmpty(); )
		{
			topic = _T("\t");
			for (int ii = 0; ii < minDDE; ii++)
			{
				items = codes.Find('\t');
				if (items == -1)
				{
					codes.Empty();
					break;
				}
				topic += codes.Left(items++);
				topic += '\t';
				codes = codes.Mid(items);
			}
			Write(topic, ++m_minor, false);
		}
	}

	m_posting = busy;
	m_lock.Unlock();
}

void CDde::OnService(CString text, int key)
{
	int	idx;
	CPost*	post;
	CString	topic, item, advise;

	if (key != 0)
		return;

	idx = text.Find('\r');
	if (idx < 0)
		return;

	key = atoi(text.Left(idx));
	text = text.Mid(++idx);

	idx = text.Find('\t');
	if (idx < 0)
	{
		OnError(key);
		return;
	}

	m_lock.Lock();
	topic = text.Left(idx++);
	if (!m_post.Lookup(topic, (CObject*&)post) || post->m_key != key)
	{
		m_lock.Unlock();
		OnError(key);
		return;
	}

	post->m_stat &= ~jobDS;
	advise = post->m_item;
	m_lock.Unlock();
	if (post->m_stat & delDS)
		return;

	item = text.Mid(idx);
	item += '\t';

	for (; !item.IsEmpty() && !advise.IsEmpty(); )
	{
		idx = item.Find('\t');
		if (idx < 0)	break;
		text = item.Left(idx++);
		item = item.Mid(idx);

		idx = advise.Find('\t');
		if (idx < 0)	break;
		if (text.IsEmpty())
		{
			advise = advise.Mid(++idx);
			continue;
		}
		topic += '\t';
		topic += advise.Left(idx++);
		topic += '\t';
		topic += text;
		advise = advise.Mid(idx);
	}

	idx = topic.Find('\t');
	if (idx != -1)
	{
		item = topic.Left(idx++);
		topic = topic.Mid(idx);
		OnAlert(item, topic);
	}
}

void CDde::OnError(int key)
{
	CString	topic;
	CPost*	post;

	m_lock.Lock();
	for (POSITION pos = m_post.GetStartPosition(); pos; )
	{
		m_post.GetNextAssoc(pos, topic, (CObject *&)post);
		if (post->m_key == key)
			post->m_stat &= ~jobDS;
	}
	m_lock.Unlock();
}

void CDde::OnAlert(CString topic, CString item)
{
	if (m_dde != flagDDE::startDDE)
		return;

	m_sync.Lock();
	topic.MakeUpper();
	m_que.Add(topic + "\t" + item);
	m_sync.Unlock();

	PostMessage(WM_CLOSE);
}

void CDde::OnAlert(CString text)
{
	int	idx;

	idx = text.Find('\t');
	if (idx < 0)	return;

	CPost*	post;
	HSZ	hItem;
	CString	topic, item, advise, tmps;

	topic = text.Left(idx++);
//	if (topic.Find('A') == 0)		// 2006.12.08 modified for ELW
//		topic.Delete(0, 1);
	if (!m_post.Lookup(topic, (CObject*&)post))
		return;				// maybe does not happen

	advise = post->m_item;
	text = text.Mid(idx);

	for (; !text.IsEmpty(); )
	{
		idx = text.Find('\t');
		if (idx < 0)	break;

		item = text.Left(idx++);
		text = text.Mid(idx);

		idx = text.Find('\t');
		if (idx < 0)
		{
			tmps = text;
			text.Empty();
		}
		else
		{
			tmps = text.Left(idx++);
			text = text.Mid(idx);
		}

		if (advise.Find(item) != -1)
		{
			m_keys.Lookup(item, item);
			switch (item.GetAt(0))
			{
			case '?':
				item.Delete(0);
				switch (tmps.GetAt(0))
				{
				case '+':
				case '-':
				case ' ':
					tmps.Delete(0);
					break;
				}
				break;
			case '*':
				item.Delete(0);
				switch (tmps.GetAt(0))
				{
				case '1':
				case '2':
				case '3':
					tmps.Delete(0);
					break;
				case '4':
				case '5':
					tmps.SetAt(0, '-');
					break;
				}
				break;
			}
			item.Insert(0, prefixDDE);
			item = topic + item;
			m_sync.Lock();
			m_item.SetAt(item, tmps);
			m_sync.Unlock();
			hItem  = DdeCreateStringHandle(m_inst, item.operator LPCTSTR(), CP_WINANSI);
			DdePostAdvise(m_inst, m_topic, hItem);
			DdeFreeStringHandle(m_inst, hItem);
		}
	}
}

void CDde::OnAdvise(HSZ hItem, bool advise)
{
	TCHAR	tmpb[256]{};

	if (DdeQueryString(m_inst, hItem, tmpb, sizeof(tmpb), CP_WINANSI))
	{
		int	idx, key;
		CPost*	post;
		CString	topic, item, tmps;

		item = tmpb;
		idx = item.Find(prefixDDE);
		if (idx < 0)	return;

		item.MakeUpper();
		topic = item.Left(idx++);
		item = item.Mid(idx);
		GetPrivateProfileString("Advise Items", item, "", tmpb, sizeof(tmpb), m_profile);

		tmps = tmpb;
		tmps.TrimLeft();
		key = tmps.FindOneOf("\t  ;");
		if (key != -1)
			tmps = tmps.Left(key);

		if (!tmps.IsEmpty())
		{
			if (tmps.FindOneOf("*?") == 0)
			{
				item.Insert(0, tmps.GetAt(0));
				tmps.Delete(0, 1);
			}
			m_keys.SetAt(tmps, item);
			item = tmps;
		}

		if (!m_post.Lookup(topic, (CObject*&)post))
		{
			if (!MakeKey(idx))
				return;
			post = new CPost();
			post->m_key = idx;
		}

		m_lock.Lock();
		if (advise)
		{
			if (post->m_item.Find(item+'\t') == -1)
			{
				post->m_item += item;
				post->m_item += '\t';
			}
			post->m_stat |= addDS;
			m_post.SetAt(topic, post);
		}
		else
		{
			item += '\t';
			post->m_item.Replace(item, "");
			if (post->m_item.IsEmpty())
			{
				delete post;
				m_post.RemoveKey(topic);
				m_lock.Unlock();
				return;
			}
			else
			{
				post->m_stat |= delDS;
				m_post.SetAt(topic, post);
			}
		}
		m_posting = true;
		m_lock.Unlock();
	}
}

HDDEDATA CDde::OnAdvise(HSZ hItem)
{
	TCHAR	tmpb[256]{};

	if (DdeQueryString(m_inst, hItem, tmpb, sizeof(tmpb), CP_WINANSI))
	{
		CString	item = tmpb;

		item.MakeUpper();
		if (m_item.Lookup(item, item))
		{
			m_sync.Lock();
			m_item.RemoveKey(item);
			m_sync.Unlock();
			return DdeCreateDataHandle(m_inst, (BYTE*)item.operator LPCTSTR(), item.GetLength()+1, 0, hItem, CF_TEXT, 0);
		}
	}
	return NULL;
}

void CDde::OnExit()
{
	m_conv    = -1;
	m_posting = false;
	m_dde     = flagDDE::stopDDE;

	m_que.RemoveAll();
	m_keys.RemoveAll();
	m_item.RemoveAll();

	CString	topic;
	CPost*	post;
	char	tmpb[256];
	int	key;

	for (POSITION pos = m_post.GetStartPosition(); pos; )
	{
		m_post.GetNextAssoc(pos, topic, (CObject *&)post);
		delete post;
	}
	m_post.RemoveAll();

	for (key = 0; key <= m_minor; key++)
		tmpb[key] = key;
	if (key)
		m_guard->Write(msgK_MAPX, "", tmpb, key, winK_DDE);
}

bool CDde::MakeKey(int& key)
{
	CString	topic;
	CPost*	post;
	CMap	<int, int, int, int&>	mkey;

	for (POSITION pos = m_post.GetStartPosition(); pos; )
	{
		m_post.GetNextAssoc(pos, topic, (CObject *&)post);
		mkey.SetAt(post->m_key, post->m_key);
	}

	int	tmpv;
	for (key = 0; key < maxDDE; key++)
	{
		if (mkey.Lookup(key, tmpv))
			continue;
		return true;
	}
	return false;
}

BOOL CDde::Write(CString text, int key, bool dde)
{
	struct	_userTH* userth{};

	if (dde)
	{
		CString	keys;

		keys.Format(_T("%d\r"), key);
		text.Insert(0, keys);
		key = 0;
	}
	text.Insert(0, CString(' ', L_userTH));
	userth = (struct _userTH *)text.operator LPCTSTR();
	userth->key = key;
	userth->stat = US_PASS;
	CopyMemory(userth->trc, _T("AXISDDE "), L_TRXC);

	return m_guard->Write((char*)text.operator LPCTSTR(), text.GetLength()-L_userTH, winK_DDE);
}

HDDEDATA CALLBACK DdeCallback(UINT uType, UINT uFmt, HCONV hConv, HSZ hTopic, HSZ hItem,
					HDDEDATA hData, DWORD dwData1, DWORD dwData2)
{
	switch (uType)
	{
	case XTYP_CONNECT:
		if (DdeCmpStringHandles(mDDE->m_service, hItem))
			break;

		mDDE->m_conv++;
		mDDE->m_dde = CDde::flagDDE::startDDE;
		return (HDDEDATA)TRUE;
	case XTYP_ADVSTART:
		if (uFmt != CF_TEXT || DdeCmpStringHandles(mDDE->m_topic, hTopic))
			break;
		mDDE->OnAdvise(hItem, true);
		return (HDDEDATA)TRUE;
	case XTYP_ADVSTOP:
		if (mDDE->m_conv == 0)
			mDDE->OnAdvise(hItem, false);
		return (HDDEDATA)TRUE;
	case XTYP_DISCONNECT:
		if (--mDDE->m_conv < 0)
			mDDE->OnExit();
		return (HDDEDATA)TRUE;
	case XTYP_ADVREQ:
		if (uFmt == CF_TEXT)
			return mDDE->OnAdvise(hItem);
		break;
	case XTYP_REQUEST:
	default:
		break;
	}
	return NULL;
}

void CDde::OnClose() 
{
	CString	text;

	while (m_que.GetSize())
	{
		m_sync.Lock();
		text = m_que.GetAt(0);
		m_que.RemoveAt(0);
		m_sync.Unlock();

		OnAlert(text);
	}
}

void CDde::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TM_DDE:
		if (m_posting)	Service();
		break;
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CDde::OnDDE()
{
	if (m_inst)
		return;
	
	if (!DdeInitialize(&m_inst, (PFNCALLBACK) DdeCallback,
		CBF_FAIL_EXECUTES|CBF_FAIL_POKES|CBF_SKIP_REGISTRATIONS|CBF_SKIP_UNREGISTRATIONS, 0L))
	{
		char	tmpb[64];

		m_dde = flagDDE::stopDDE;
		GetPrivateProfileString("Informations", "Service", "AX", tmpb, sizeof(tmpb), m_profile);
		m_service = DdeCreateStringHandle(m_inst, tmpb, CP_WINANSI);
		DdeNameService(m_inst, m_service, 0L, DNS_REGISTER);
		GetPrivateProfileString("Informations", "Topic", "KS", tmpb, sizeof(tmpb), m_profile);
		m_topic = DdeCreateStringHandle(m_inst, tmpb, CP_WINANSI);

		SetTimer(TM_DDE, TMI_DDE, NULL);
	}
}