// DataFormat.h: interface for the CDataFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFORMAT_H__75C6FEA7_6F17_4146_981A_4678E092653A__INCLUDED_)
#define AFX_DATAFORMAT_H__75C6FEA7_6F17_4146_981A_4678E092653A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataFormat  
{
public:
	CDataFormat(class CObjMgr *pObjMgr);
	virtual ~CDataFormat();

public:
//	int	m_iRequestCnt;
//	int	m_iOutID2;	// second symbol kind
//	int	m_iOutIDS;	// single symbol kind

//	DWORD	m_dwGrpCnt;
//	DWORD	m_dwRgnCnt;
	
private:
	int	m_iGrpHLen;
	int	m_iVolumeOption;
	int	m_iRequestCnt;
	int	m_iOutID2;	// second symbol kind
	int	m_iOutIDS;	// single symbol kind

	DWORD	m_dwGrpCnt;
	DWORD	m_dwRgnCnt;

	char*	m_pcGrpHeader;
	char*	m_pcGrp;				// [envinfo + region + grp] => region + grp부분

	char	m_acPVGrp[SZ_REGION + SZ_GRAPH];		// PV
	char	m_acRCGrp[SZ_REGION + SZ_GRAPH];		// 역시계곡선
	char	m_acTNGrp[SZ_REGION + SZ_GRAPH];		// 삼선전환도
	char	m_acPFGrp[SZ_REGION + SZ_GRAPH];		// P&F
	char	m_acKGGrp[SZ_REGION + SZ_GRAPH];		// Kagi
	char	m_acRKGrp[SZ_REGION + SZ_GRAPH];		// Renko
	char	m_acVolGrp[SZ_REGION*2 + SZ_GRAPH*2];	// EQV,ECV

	CString	m_strDataSymbol;
	CString	m_strOutSymbol;	// main request symbol
	CString	m_strOutSymbol2;	// second request symbol
	CString	m_strOutSymbolS;	// single request symbol
	CString	m_strEnvPath;
	
	class CObjMgr*	m_pObjMgr;

	struct _graph		m_grpAVol;
	struct _graph		m_grpBalance;
	struct _graph		m_grpKagi;
	struct _envInfo*	m_pEnvInfo;			// [envinfo + region + grp] => envinfo부분

public:
	int	GetMainGraph() { return m_pEnvInfo->datainfo.wMainGrpKind; } 
	struct _graph*	GetAVolGrp() { return &m_grpAVol; }
	struct _graph*	GetBalanceGrp() { return &m_grpBalance; }
	struct _graph*	GetKagiGrp() { return &m_grpKagi; }
	int	GetRequestCount() { return m_iRequestCnt; }
	void	SetRequestCount(int iRequestCnt) { m_iRequestCnt = iRequestCnt; }
	int	GetOuntID2() { return m_iOutID2; }
	int	GetOuntIDS() { return m_iOutIDS; }
	

	void	InitData();
	void	VolumeInit();
	char*	GetGrpHeader(int* piLen);
	char*	GetGrpHeader2(int* piLen);
	char*	GetGrpHeaderS(int* piLen);
	struct _envInfo* GetEnvInfo();				// _envInfo
	char*	GetGrpInfo(int& iRgnCnt, int& iGrpCnt);
	char*	GetSaveGrpInfo(int &rcnt, int &gcnt);
	
	void	SetFgraph(char *pEnvInfo, char *pGrpInfo, bool bInit = false);
	void	SetFgraph(char *pGrpInfo, int rcnt, int gcnt);

	void	ChangeMain(int iKind);
	void	ChangeIndex(int index, int gap);
	void	ChangeUnit(int unit);
	void	SaveDay(int iDay, bool bDisplay = true);
	int	GetDispDay();
	void	SetSingleGrp(int iKind);

	int	GetMainGrpRegion();
	void	MakeHeader(LPCTSTR lpszCode = NULL);
	//** add by macho@20061229 <start>
	void	MakeHeader_code();
	void	MakeHeader_index();
	void	MakeHeader_future();
	void	MakeHeader_option();
	void	MakeHeader_cofuture();
	void	MakeHeader_cooption();
	void	MakeHeader_foreign();
	// 20090915 : ADD
	void	MakeHeader_commoditiesfuture();
	//** add by macho@20061229 <end>
	void	MakeHeader2();
	void	MakeHeaderS();
	void	CheckHeader2();
	int	CheckHeaderS(int gkind);
	int	isExtraTR(int gkind);

	void	ReadEnvPath();
	int	ReadVolumeOption();
	int	WhichFOCode(LPCTSTR code);
};

#endif // !defined(AFX_DATAFORMAT_H__75C6FEA7_6F17_4146_981A_4678E092653A__INCLUDED_)
