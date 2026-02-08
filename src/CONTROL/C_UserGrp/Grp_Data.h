// Grp_Data.h: interface for the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>
struct	_ItemInfo
{
	CString	name;		// 종목명 혹은 데이타명
	CString	RtsCode;	// 실시간 코드
	CString	RtsSymbol;	// 실시간 심볼
	int	RtsSign=0;	// 실시간 대비부호 사용여부
	int	digit=0;	// 자리수
	double	power=0;	// 10digit승
	int	region=0;	// 영역
	int	pivot=0;	// 기준값
};
#define	sz_ItemInfo	sizeof(_ItemInfo)

enum {RM_NO = 0, RM_REMOVE, RM_MINUS};
enum {RTS_NO=0, RTS_INSERT, RTS_LAST};

class CGrp_Data  
{
public:
	CGrp_Data(class CGrpWnd *pGrpWnd, int dIndex, int dGap);
	virtual ~CGrp_Data();

public:
	int	AttachGraphData(char* data, int nHead, int nData, int iSize, int dSize);
	int	RealTimeData(int index, CString sTime, CString sVal, bool bInsert);
	int	RealTimeItemData(int index, CString sVal);
	struct _cgNValx* GetGraphData(int index);
	int	GetDataCount() { return _vData.size(); }
	static int	CharToInt(const char* cnvB, int cnvL);
	static double	CharToDouble(const char* cnvB, int cnvL);
	void	GetNValue(struct _cgNValx *gNVal, CString str, int startIndex, int nCount);
	void	InitInfo(int nHead);
public:
	std::vector<std::unique_ptr<_ItemInfo>>	m_pInfo;
	int		m_nInfo{};
	bool		m_bRealTime{};
	CArray <CString, CString>	m_arItemRts;	// Item Rts Symbol Array

private:
	class CGrpWnd* m_pGrpWnd{};
	std::map<int, std::shared_ptr<struct _cgNValx>> _mData;
	std::vector<std::shared_ptr<struct _cgNValx>>	_vData;
	int	m_szFrame{};		// 1Frame Size (1 Que Size)
	int	m_dIndex{};
	int	m_dGap{};
	int	m_dCount{};		// val 개수

};