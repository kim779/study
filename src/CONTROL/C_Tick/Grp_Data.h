// Grp_Data.h: interface for the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include <afxtempl.h>

class CGrp_Data  
{
public:
	CGrp_Data(class CGrpWnd *pGrpWnd, int dKind, int dKey, int dIndex);
	virtual ~CGrp_Data();

public:
	int	AttachGraphData(char* data, int count, int mgap);
	bool	UpdateRTM();
	struct _cgTick* GetGraphData(size_t index);
	int	GetDataGap() { return m_dGap; }
	size_t	GetDataCount() { return _vDataQue.size(); }
	static int	CharToInt(const char* cnvB, int cnvL);

public:
	int	m_dKind{};
	int	m_dKey{};
	int	m_dIndex{};
	int	m_dGap{};
	int	m_szFrame{};			// Frame Size (1 Que Size)

private:
	inline	double	CharToDouble(const char* cnvB, int cnvL);

private:
	class CGrpWnd *m_pGrpWnd{};
	std::vector<std::shared_ptr<struct _cgTick>>	_vDataQue;
};
