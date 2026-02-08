// IGraphInfo.h: interface for the IGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
#define AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
#include "_AddInVer.h"	// for AddIn Version Define

interface IGraphInfo : public IUnknown
{
protected:
	virtual ~IGraphInfo() {}

		
public:
	// 종찬(05/07/14) 지표 계산에 필요한 조건값을 제공한다.
	virtual CList<double, double>* GetCommonCalculateVarList() = 0;
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName) = 0;
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex) = 0;

	// 종찬(05/07/15) 계산에 쓰이는 패킷명을 제공한다.
	virtual LPCTSTR GetCommonCalculatePacketNames() = 0;
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex) = 0;

	// 서브그래프의 인덱스를 리턴한다.
	virtual int GetSubGraphIndex(const char* pcszSubGraphName) = 0;
	
	// 서브그래프의 Show/Hide 여부를 리턴한다. (bSubGraphShow - true: show, false: hide)
	virtual bool IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow) = 0;
	virtual bool IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow) = 0;
};

#endif // !defined(AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
