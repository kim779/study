// Component.cpp : implementation file
//

#include "stdafx.h"
#include "dx_Interest.h"
#include "Component.h"
//#include "math.h"
#include "../../h/interSt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAX_GROUPITEMCNT 80

/////////////////////////////////////////////////////////////////////////////
// CComponent
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CComponent, CCmdTarget)

CComponent::CComponent()
{
	EnableAutomation();
	m_sUser.Empty();
	m_sRoot.Empty();
}

CComponent::~CComponent()
{
}


void CComponent::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CComponent, CCmdTarget)
	//{{AFX_MSG_MAP(CComponent)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CComponent, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CComponent)
	DISP_FUNCTION(CComponent, "LoadGroupNames", LoadGroupNames, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CComponent, "SetUserName", SetUserName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CComponent, "LoadCodeList", LoadCodeList, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CComponent, "LoadCdNNmList", LoadCdNNmList, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CComponent, "LoadGroupList", LoadGroupList, VT_BSTR, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CComponent, "GetGroupCnt", GetGroupCnt, VT_I4, VTS_NONE)
	DISP_FUNCTION(CComponent, "GetNextGroupCode", GetNextGroupCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CComponent, "GetPrvGroupCode", GetPrvGroupCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CComponent, "GetGroupCodeIdx", GetGroupCodeIdx, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CComponent, "GetIdxGroupCode", GetIdxGroupCode, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CComponent, "AddItemCd", AddItemCd, VT_BOOL, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IComponent to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5C298934-6200-4DFD-9818-8029A7CC04FB}
static const IID IID_IComponent =
{ 0x5c298934, 0x6200, 0x4dfd, { 0x98, 0x18, 0x80, 0x29, 0xa7, 0xcc, 0x4, 0xfb } };

BEGIN_INTERFACE_MAP(CComponent, CCmdTarget)
	INTERFACE_PART(CComponent, IID_IComponent, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponent message handlers

BSTR CComponent::LoadGroupNames() 
{
	CString strResult;
	strResult = loadGroupNames();

	return strResult.AllocSysString();
}

CString CComponent::loadGroupNames()
{
	char twb[1024]{};
	CString dir, sTmp, sRtn = "", sFileName, sTmp2, sOrder;
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;
		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);
		sTmp2 = twb;

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			CString tmp;
			tmp.Format("%02d %s\t", atoi(sTmp), sTmp2);
			sRtn = tmp + sRtn;
		}
	}

	return sRtn;
}

void CComponent::SetUserName(LPCTSTR sUser) 
{
	m_sUser = sUser;
}

BSTR CComponent::LoadCodeList(LPCTSTR sGroup) 
{
	CString strResult;

	strResult = loadCodeList(sGroup);

	return strResult.AllocSysString();
}

CString CComponent::loadCodeList(CString sGroup)
{
	struct _inters	inter {};
//	int cnt = 0;

	CString dir, sRtn;
	dir.Format("%s\\user\\%s\\portfolio.i%02d", m_sRoot, m_sUser, atoi(sGroup));
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{ 
			nBytesRead = file.Read(&inter, sizeof(inter));
			if (nBytesRead == sizeof(inter))
			{
				CString sCode = getMidString(inter.code, 0, 12);
				sCode.TrimLeft();
//				if (!sCode.IsEmpty())
				{
					sRtn += sCode;
					sRtn += '\t';
				}
			}
		}while((int)nBytesRead);
		file.Close();
		return sRtn;
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return "";
	}
	END_CATCH
		return "";
}

CString CComponent::loadCdNNmList(CString sGroup)
{
	struct _inters	inter;
//	int cnt = 0;

	CString dir, sRtn;
	dir.Format("%s\\user\\%s\\portfolio.i%02d", m_sRoot, m_sUser, atoi(sGroup));
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{ 
			nBytesRead = file.Read(&inter, sizeof(inter));
			if (nBytesRead == sizeof(inter))
			{
				CString sCode = getMidString(inter.code, 0, 12),
					sName = getMidString(inter.name, 0, 30);

				sCode.TrimLeft();
				sName.TrimLeft();
				if (!sCode.IsEmpty())
				{
					sRtn += sCode;
					sRtn += ';';
					sRtn += sName;
					sRtn += '\t';
				}
			}
		} while((int)nBytesRead);
		file.Close();
		return sRtn;
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return "";
	}
	END_CATCH
		return "";
}

CString CComponent::getMidString(char *pStr, int nFirst, int nLen)
{
	if ((int)strlen(pStr + nFirst) < nLen)
		nLen = strlen(pStr + nFirst);
	return CString(pStr + nFirst, nLen);
}

BSTR CComponent::LoadCdNNmList(LPCTSTR sGroup) 
{
	CString strResult;

	strResult = loadCdNNmList(sGroup);

	return strResult.AllocSysString();
}

CString CComponent::Parser(CString &srcstr, CString substr)
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
	return "";
}

CString CComponent::getGroupOrder()
{
	char twb[1024]{};
	CString dir, sTmp, sFileName, sTmp2, sOrder;

	if (m_sUser.IsEmpty())
		TRACE("dx_Interest Component Err : User = []");

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	for (int ii = 0; ii < 100; ii++)
	{
		sTmp.Format("%02d", ii);
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);
		if (strlen(twb))
			sTmp2 += sTmp + ";";
	}

	GetPrivateProfileString("GROUPORDER", "00", sTmp2, twb, sizeof(twb), dir);
	sOrder = twb;
	return sOrder;
}

int CComponent::getGroupCnt()
{
	char	twb[1024]{};
	int	nCnt;
	CString dir, sTmp, sTmp2, sOrder, sFileName;
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	nCnt = 0;
	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;

		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			nCnt++;
		}
	}
	return nCnt;
}

CString	CComponent::loadGroupList(CString GroupCode, int nCount)
{
	char	twb[1024]{};
	CString dir, sTmp, sRtn = "", sFileName, sTmp2, sOrder;
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	bool	bStarted = false;
	int	nAddCnt	 = 0;
	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;
		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);
		sTmp2 = twb;

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			if (atoi(sTmp) == atoi(GroupCode))
			{
				bStarted = true;
			}

			if (bStarted)
			{
				CString tmp;
				tmp.Format("%02d %s\t", atoi(sTmp), sTmp2);
				sRtn = sRtn + tmp;

				nAddCnt++;
				if (nAddCnt == nCount)
					return sRtn;
			}
		}
	}
	return sRtn;
}

CString	CComponent::getPrvGroupCode(CString GroupCode)
{
	// GroupCode가 "" 이면 첫번째 GroupCode Return
	char	twb[1024]{};
	CString dir, sTmp, sTmp2, sOrder, sFileName;
	CString sPrv = "";
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	GroupCode.TrimRight();
	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;
		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);
		sTmp2 = twb;

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			if (GroupCode == "")
				return sTmp;

			if (atoi(sTmp) == atoi(GroupCode))
				return sPrv;
			sPrv = sTmp;
		}
	}
	return "";
}

CString	CComponent::getNextGroupCode(CString GroupCode)
{
	char	twb[1024]{};
	bool	bFindNext;
	CString dir, sTmp, sTmp2, sOrder, sFileName;
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	bFindNext = false;
	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;
		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);
		sTmp2 = twb;

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			if (bFindNext)
				return sTmp;

			if (atoi(sTmp) == atoi(GroupCode))
				bFindNext = true;
		}
	}
	return "";
}
	
int CComponent::getGroupCodeIdx(CString GroupCode)
{
	char	twb[1024]{};
	int		nCnt;
	CString dir, sTmp, sTmp2, sOrder, sFileName;
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	nCnt = 0;
	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;
		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			if (atoi(sTmp) == atoi(GroupCode))
				return nCnt;
			nCnt++;
		}
	}
	return nCnt;
}

CString	CComponent::getIdxGroupCode(CString sIndex) 
{
	char	twb[1024]{};
	int		nCnt;
	CString dir, sTmp, sTmp2, sOrder, sFileName;
	CFileFind	finder;

	dir.Format("%s\\user\\%s\\portFolio.ini", m_sRoot, m_sUser);
	sOrder = getGroupOrder();

	nCnt = 0;
	while(!sOrder.IsEmpty())
	{
		sTmp = Parser(sOrder, ";");
		if (sTmp.IsEmpty() || sTmp.GetLength() != 2)
			break;
		sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(sTmp));
		GetPrivateProfileString("GROUPNAME", sTmp, "", twb, sizeof(twb), dir);

		if (!sTmp.IsEmpty() && finder.FindFile(sFileName))
		{
			if (nCnt == atoi(sIndex))
				return sTmp;
			nCnt++;
		}
	}
	return "";
}

boolean CComponent::addItemCd(CString GroupCode, CString ItemCd)
{
	CString	sFileName, sName;
	sFileName.Format("%s\\user\\%s\\portFolio.i%02d", m_sRoot, m_sUser, atoi(GroupCode));

	if (IsCodeInGroup(GroupCode, ItemCd))
	{	
		MessageBox(AfxGetMainWnd()->m_hWnd, "그룹에 포함되어 있는 종목입니다", "관심종목", MB_OK);
		return false;
	}

	CFile	file; CFileFind finder;

	if (!finder.FindFile(sFileName))
	{
		if (!file.Open(sFileName, CFile::modeCreate | CFile::modeReadWrite))
			return false;
	}
	else
	{
		if (!file.Open(sFileName, CFile::modeReadWrite))
			return false;
	}

	const int fileL = gsl::narrow_cast<int>(file.GetLength());
	if ((fileL / sizeof(struct _inters)) >= MAX_GROUPITEMCNT)
	{
		file.Close(); return false;
	}

	struct _inters	inter{};
	CString fileB;
	if (file.Read(fileB.GetBuffer(fileL), fileL) != (unsigned char)fileL)
	{
		file.Close();
		fileB.ReleaseBuffer();
		return false;
	}
	
	fileB.ReleaseBuffer();
	inter.gubn[0] = '1';
	strncpy(inter.code, (LPCTSTR)ItemCd, min(sizeof(inter.code), ItemCd.GetLength()));
	sName = getCodeNm(ItemCd);
	strncpy(inter.name, (LPCTSTR)sName, min(sizeof(inter.name), sName.GetLength()));

	file.SeekToBegin();

	file.Write(&inter, sizeof(inter));
	file.Write(fileB, fileL);
	file.Close();

	return true;
}

CString CComponent::getCodeNm(CString ItemCd)
{
	CFile		file;
	CFileFind	filefind;
	CString		sCode, sName;
	struct		_hjcode hjcode {};

	CString dir; dir.Format("%s\\tab\\hjcode.dat", m_sRoot);

	ItemCd.TrimRight(); if (ItemCd.IsEmpty()) return "";

	if (filefind.FindFile(dir) && file.Open(dir, CFile::modeRead))
	{
		while (file.Read(&hjcode, sizeof(_hjcode)) == sizeof(_hjcode))
		{
			sCode = CString(hjcode.code, sizeof(hjcode.code));
			sCode.TrimRight();
			if (sCode.Left(1) == "A")
				sCode = sCode.Right(sCode.GetLength() -1);
			if (sCode.CompareNoCase(ItemCd) == 0)
			{
				file.Close();
				sName = CString(hjcode.name, sizeof(hjcode.name));
				sName.TrimRight();
				return  sName;
			}
		}
		file.Close();
	}
	return "";
}

long CComponent::GetGroupCnt() 
{
	return getGroupCnt();
}

BSTR CComponent::LoadGroupList(LPCTSTR GroupCode, long nCount) 
{
	CString strResult;
	strResult = loadGroupList(GroupCode, nCount);

	return strResult.AllocSysString();
}

BSTR CComponent::GetNextGroupCode(LPCTSTR GroupCode) 
{
	CString strResult;
	strResult = getNextGroupCode(GroupCode);

	return strResult.AllocSysString();
}

BSTR CComponent::GetPrvGroupCode(LPCTSTR GroupCode) 
{
	CString strResult;
	strResult = getPrvGroupCode(GroupCode);

	return strResult.AllocSysString();
}

long CComponent::GetGroupCodeIdx(LPCTSTR GroupCode) 
{
	return getGroupCodeIdx(GroupCode);
}

BSTR CComponent::GetIdxGroupCode(LPCTSTR sIndex) 
{
	CString strResult;
	strResult = getIdxGroupCode(sIndex);
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

BOOL CComponent::AddItemCd(LPCTSTR GroupCode, LPCTSTR ItemCd) 
{
	return addItemCd(GroupCode, ItemCd);
}

BOOL CComponent::IsCodeInGroup(CString sGroup, CString Code)
{
	struct _inters	inter {};

	CString dir, sRtn;
	dir.Format("%s\\user\\%s\\portfolio.i%02d", m_sRoot, m_sUser, atoi(sGroup));
	TRY
	{
		BOOL  bInGroup = false;
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{ 
			nBytesRead = file.Read(&inter, sizeof(inter));
			if (nBytesRead == sizeof(inter))
			{
				CString sCode = getMidString(inter.code, 0, 12);
				sCode.TrimLeft();
				if (!Code.CompareNoCase(sCode))
				{
					bInGroup = true;
					break;
				}				
			}
		}while((int)nBytesRead);
		file.Close();
		return bInGroup;
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		return false;
	}
	END_CATCH
}
