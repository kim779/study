// DataChk.h: interface for the CDataChk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACHK_H__FA42563A_3BFA_4276_97A7_3B44CC2382E8__INCLUDED_)
#define AFX_DATACHK_H__FA42563A_3BFA_4276_97A7_3B44CC2382E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDataChk  
{
public:
	CDataChk(CWnd *pView, struct _param *param, CString &fname, bool bMake);
	virtual ~CDataChk();

public:

private:
	bool	m_bVol;
	bool	m_bName;

	int	m_iFPoint;
	int	m_iDtCount;
	int	m_iDtDisplay;
	int	m_iTickDisplay;	// DS_DISPLT, DS_DISPRT
	int	m_iBongKind;
	
	int	m_iDtUnit;
	int	m_iDtIndex;
	int	m_iPMACnt;
	int	m_iVMACnt;
	int	m_aiPMA[4];
	int	m_aiVMA[4];

	CWnd*	m_pwndMain;

	CString	m_strFileName;
	CString	m_strFName;
	CString	m_strGexDir;
	CString	m_strTabDir;

	COLORREF	m_rgbFG;
	COLORREF	m_rgbBG;
	

private:
	void	CheckData(CString root, CString name);
	void	MakeDat(CString fname);
	void	InitInfo(struct _cntInfo *cntInfo, char *pData);
	void	MakeEnv(CString fname, CString bname = _T(""));
	void	MakeGrp(CString fname, CString bname = _T(""));
	void	GetCntInfo(int &rcnt, int &gcnt, int &realGCnt);
	void	ParseParam(CString options);
	int	GetPalette(int index);

	bool	MakeData();

public:
	void	ReMakeData();
};

#endif // !defined(AFX_DATACHK_H__FA42563A_3BFA_4276_97A7_3B44CC2382E8__INCLUDED_)
