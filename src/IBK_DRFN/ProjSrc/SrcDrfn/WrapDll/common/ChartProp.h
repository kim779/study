/**
	@file		ChartProp.h
	@brief		실시간 데이터 관리
	@author		sashia
	@date		2009/10/06
*/

#if !defined(AFX_CHARTPROP_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_CHARTPROP_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../SrcSite/include/axisGrid.h"

#pragma	pack(1)

typedef struct ST_CHART_PACKET {
	BOOL	bReal;
	int		nPacketSize;
	CString	szPacketName;
	CString	szFID;
	CString	szRealFID;
	CString	szTextType;

	ST_CHART_PACKET() {
		bReal			= FALSE;
		nPacketSize		= 0;
		szPacketName	= _T("");
		szFID			= _T("");
		szRealFID		= _T("");
		szTextType		= _T("");
	};
} *LPST_CHART_PACKET;

typedef		CArray<ST_CHART_PACKET, ST_CHART_PACKET>	CPackArray;

class CChartProp : public CObject
{
	DECLARE_DYNAMIC(CChartProp);
public:
	CChartProp();
	CChartProp(const CChartProp& obj)	{ *this = obj;	};
	virtual ~CChartProp();

	void		DefaultSet();

	BOOL		m_bReal;

protected:
	BOOL		m_bABS;
	int			m_nRealKey;
	int			m_nRowIdx;
	CString		m_strPacketName;
	CString		m_strKey;
	CString		m_strRQ;
	CString		m_strTitle;
	CString		m_strCode;
	CString		m_strTRCode;
	CString		m_strDiffRealSym;

	GRID_O		m_stGridOut;

	CPackArray	m_arrPacket;

public:
	virtual		void	Copy(const CChartProp& item);
	virtual		CChartProp& operator = (const CChartProp& item);

	void		RemoveAll();

	void		SetABS(BOOL bABS)				{ m_bABS = bABS;	};
	BOOL		IsABS()							{ return m_bABS;	};
	void		SetRealKey(int nRealKey)		{ m_nRealKey = nRealKey;	};
	int			GetRealKey()					{ return m_nRealKey;	};
	void		SetRowIdx(int nRowIdx)			{ m_nRowIdx = nRowIdx;	};
	int			GetRowIdx()						{ return m_nRowIdx;	};
	void		ResetPacketName();
	CString		GetPackName()					{ return m_strPacketName;	};
	void		SetKey(CString strKey)			{ m_strKey = strKey;	};
	CString		GetKey()						{ return m_strKey;	};
	void		SetRQ(CString strRQ)			{ m_strRQ = strRQ;	};
	CString		GetRQ()							{ return m_strRQ;	};
	void		SetTitle(CString strTitle)		{ m_strTitle = strTitle;	};
	CString		GetTitle()						{ return m_strTitle;	};
	void		SetCode(CString strCode)		{ m_strCode = strCode;	};
	CString		GetCode()						{ return m_strCode;	};
	void		SetTRCode(CString strTRCode)	{ m_strTRCode = strTRCode;	};
	CString		GetTRCode()						{ return m_strTRCode;	};
	void		SetDiffRealSym(CString strSym)	{ m_strDiffRealSym = strSym;	};
	CString		GetDiffRealSym()				{ return m_strDiffRealSym;	};

	void		SetGrid_O(GRID_O& stGridOut)	{ memcpy(&m_stGridOut, &stGridOut, sizeof(GRID_O));	};
	void		GetGrid_O(GRID_O& stGridOut)	{ memcpy(&stGridOut, &m_stGridOut, sizeof(GRID_O));	};

	int			CampareCode(CString strCode)	{ return m_strCode.Compare(strCode);	};
	int			CampareKey(CString strKey)		{ return m_strKey.Compare(strKey);	};
	int			CampareTR(CString strTR)		{ return m_strTRCode.Compare(strTR);	};
	int			CampareRQ(CString strRQ)		{ return m_strRQ.Compare(strRQ);	};

	void		RemoveAllPacket()				{ m_arrPacket.RemoveAll();	};

	void		AddPacket(CString strPacket, int nVer=7);
	void		AddPacket(ST_CHART_PACKET &stPack)	{ m_arrPacket.Add(stPack);	};

	ST_CHART_PACKET		GetPacket(int nIndex)	{ return m_arrPacket.GetAt(nIndex);	};
	CString		GetPacketToString(int nIndex);

	void		SetPacket(int nIndex, ST_CHART_PACKET &stPack);

	int			GetPacketSize()	{ return m_arrPacket.GetSize();	};

	BOOL		IsReal();
};

typedef	CMap<int, int, CChartProp, CChartProp&>			CChartPropIntMap;
typedef	CMap<CString, LPCTSTR, CChartProp, CChartProp&>	CChartPropMap;
typedef	CArray<CChartProp, CChartProp&>					CChartPropArr;

#pragma	pack()

#endif // !defined(AFX_CHARTPROP_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
