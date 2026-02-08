// GrpData.h: interface for the CGrpData class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>

class CGrpData  
{
public:
	CGrpData(class CGrpWnd *pGrpWnd);
	virtual ~CGrpData();

public:
	int	AttachGraphData(int index, bool bIndex, int count, int discard, char *pData, CString code, CString siga, CString jgga);
	bool	UpdateRTM(int index, CString val, CString ctim);
	struct _cgMJ* GetGraphData(int index);
	int	GetDataGap() { return m_dGap; }
	size_t	GetDataCount() { return _vDataQue.size(); }
	void	Recalculate();
	static int	CharToInt(const char* cnvB, int cnvL);
	static double	CharToDouble(const char* cnvB, int cnvL);
	static int	GetToken(char *pData, char *token);

public:
	CString	m_pName[MAX_JISU];
	CString	m_pCode[MAX_JISU];
	CString	m_pSymbol[MAX_JISU];	// R_CURR
	int	m_pDigit[MAX_JISU]{};
	int	m_pJgga[MAX_JISU]{};
	int	m_pDsga[MAX_JISU]{};
	int	m_dIndex{};
	int	m_dGap{};
	int	m_szFrame{};			// Frame Size (1 Que Size)

private:
	int	GetCompareVal(int index);
	bool	CreateDataQue(int count);
	int	GetInsertIndex(int time);

private:
	class CGrpWnd *m_pGrpWnd{};
	//CArray	<char*, char*>				m_DataQue;	// Data Array
	std::vector<std::unique_ptr<struct _cgMJ>>	_vDataQue;

	int	m_startTime{};
};