#if !defined(AFX_SOCKETDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_SOCKETDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_

#define		MAX_PACKET_LEN				8192				// UDP 최대 패킷 크기
#define		MAX_FT_PACKET_LEN			1572864				// TCP 최대 패킷 크기
#define		SOCK_COMMAND_CONNECT		100

class CConnecedInfo
{
public: 
	CConnecedInfo(CAsyncSocket* pSocket, CDocument* pDoc) :
	  m_pSocket(pSocket)
	  , m_pDoc(pDoc)
	{
	}

	~CConnecedInfo()
	{
		if(m_pSocket)
			delete m_pSocket;
	}

	CDocument* GetConnectedDoc()
	{
		return m_pDoc;
	}

	bool IsSameAddress(CString strAddress, UINT nPort)
	{
		CString strMyAddress;
		UINT	nMyPort;

		m_pSocket->GetPeerName(strMyAddress, nMyPort);

		return (strMyAddress.CompareNoCase(strAddress)) == 0 && (nMyPort == nPort);
	}
	
	bool IsSameSocket(CAsyncSocket* pSocket)
	{
		return m_pSocket->m_hSocket == pSocket->m_hSocket;
	}

private:
	CAsyncSocket*	m_pSocket;
	CDocument*		m_pDoc;
};

class CFindConnectedInfo
{
public:
	CFindConnectedInfo(CString strAddress, UINT nPort) :
	  m_strAddress(strAddress)
	  , m_nPort(nPort)
	  , m_pSocket(NULL)
	{
	}

	CFindConnectedInfo(CAsyncSocket* pSocket) :
	  m_pSocket(pSocket)
	{
	}

	~CFindConnectedInfo()
	{
	}

	bool operator()(CConnecedInfo* rhs) const
	{
		if(m_pSocket)
			return rhs->IsSameSocket(m_pSocket);
		else
			return rhs->IsSameAddress(m_strAddress, m_nPort);
	}

private:
	CString m_strAddress;
	UINT	m_nPort;

	CAsyncSocket*	m_pSocket;
};

class CSendData
{
public:
	CSendData(int nCommand, LPSTR lpszData = NULL) :
	  m_nCommand(nCommand)
	  , m_lpszData(NULL)
	  , m_nDataSize(0)
	{
		  if(lpszData)
		  {
			  m_lpszData = ::strdup(lpszData);
			  m_nDataSize = ::lstrlen(lpszData);
		  }
	}

	~CSendData()
	{
		if(m_lpszData)
		{
			::free(m_lpszData);
			m_lpszData = NULL;
		}
	}

	int GetCommand()
	{
		return m_nCommand;
	}
	
	UINT GetDataSize()
	{
		return m_nDataSize;
	}

	LPCSTR GetData()
	{
		if(m_lpszData)
			return m_lpszData;
		else
			return _T("");
	}

private:
	int		m_nCommand;
	LPSTR	m_lpszData;
	UINT	m_nDataSize;
};

typedef struct _ST_SENDDATA
{
	int		m_nCommand;
	UINT	m_nStrategyType;
	int		m_nDataSize;
	char	m_szData[1];		// Data 제일 뒤에 ETX_PREV + ETX
} ST_SENDDATA;

#endif // !defined(AFX_SOCKETDEFINE_H__18754815_9512_11D2_AC26_00104B9E52FF__INCLUDED_)
