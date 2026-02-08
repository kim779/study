// IndicatorConfigurationData.h: interface for the CIndicatorConfigurationData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORCONFIGURATIONDATA_H__77298508_80CC_4DBB_BD5C_F4E9130FC9FA__INCLUDED_)
#define AFX_INDICATORCONFIGURATIONDATA_H__77298508_80CC_4DBB_BD5C_F4E9130FC9FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainBlock;
class CIndicatorInfo;

/*class CIndicatorConfigurationData
{
public:
	enum INDICONFIG_RETURN
	{
		LOAD_ERROR = -1,
		CANCEL = 0,
		OK,
		GRAPH_DELETE
	};

	CIndicatorConfigurationData(CWnd* pParent, CPacketList* pPacketList, CIndicatorList* pIndicatorList, 
		const bool bIsDeleteAll, const CString& functionName, const CString& subGraphName, const CString& graphData, 
		const bool bIsSignal, const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType, 
		const bool bIsExistence_PriceMA, const bool bIsAllHiding_PriceMA)
	{
		m_pParent = pParent;
		m_pPacketList = pPacketList;
		m_pIndicatorList = pIndicatorList;
		m_bIsDeleteAll = bIsDeleteAll;
		m_strFunctionName = functionName;
		m_strSubGraphName = subGraphName;
		m_strGraphData = graphData;
		m_bIsSignal = bIsSignal;
		m_bIsVertScaleLogType = bIsVertScaleLogType;
		m_bIsVertScaleInvertType = bIsVertScaleInvertType;
		m_bExistence_PriceMA = bIsExistence_PriceMA;
		m_bAllHiding_PriceMA = bIsAllHiding_PriceMA;
	};

	CIndicatorConfigurationData(CWnd* pParent, CPacketList* pPacketList, CIndicatorList* pIndicatorList, 
		const bool bIsDeleteAll, const CString& functionName, const CString& subGraphName, const CString& graphData, 
		const bool bIsSignal, const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType)
	{
		m_pParent = pParent;
		m_pPacketList = pPacketList;
		m_pIndicatorList = pIndicatorList;
		m_bIsDeleteAll = bIsDeleteAll;
		m_strFunctionName = functionName;
		m_strSubGraphName = subGraphName;
		m_strGraphData = graphData;
		m_bIsSignal = bIsSignal;
		m_bIsVertScaleLogType = bIsVertScaleLogType;
		m_bIsVertScaleInvertType = bIsVertScaleInvertType;
		m_bExistence_PriceMA = false;
		m_bAllHiding_PriceMA = false;
	};

	void SetExistence_PriceMA(const bool bIsExistence) { m_bExistence_PriceMA = bIsExistence; };
	void SetAllHiding_PriceMA(const bool bIsHiding) { m_bAllHiding_PriceMA = bIsHiding; };

	CWnd* GetParent() const { return m_pParent; };
	CPacketList* GetPacketList() const { return m_pPacketList; };
	CIndicatorList* GetIndicatorList() const { return m_pIndicatorList; };
	CIndicatorList* GetIndicatorList() { return m_pIndicatorList; };
	CString GetFunctionName() const { return m_strFunctionName; };
	CString GetSubGraphName() const { return m_strSubGraphName; };
	CString GetGraphData() const { return m_strGraphData; };
	bool IsDeleteAll() const { return m_bIsDeleteAll; };
	bool IsSignal() const { return m_bIsSignal; };
	bool IsVertScaleLogType() const { return m_bIsVertScaleLogType; };
	bool IsVertScaleInvertType() const { return m_bIsVertScaleInvertType; };
	bool IsExistence_PriceMA() const { return m_bExistence_PriceMA; };
	bool IsAllHiding_PriceMA() const { return m_bAllHiding_PriceMA; };

private:
	CWnd* m_pParent;
	CPacketList* m_pPacketList;
	CIndicatorList* m_pIndicatorList;
	bool m_bIsDeleteAll;
	CString m_strGraphData;
	CString m_strFunctionName;
	CString m_strSubGraphName;
	bool m_bIsSignal;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
	bool m_bExistence_PriceMA;
	bool m_bAllHiding_PriceMA;
};


class CIndicatorConfigurationChangedData
{
public:
	CIndicatorConfigurationChangedData()
	{
		m_bIsSignal = false;
		m_bIsVertScaleLogType = false;
		m_bAllHiding_PriceMA = false;
		m_bCalculateGraph = true;
		m_bInitializePacket = false;
	};

	void SetPacketNames(const CString& packetNames){ m_strPacketNames = packetNames; };
	void SetSubGraphData(const CString& subGraphData){ m_strSubGraphData = subGraphData; };
	void SetSignal(const bool bIsSignal){ m_bIsSignal = bIsSignal; };
	void SetVertScaleLogType(const bool bIsVertScaleLogType){ m_bIsVertScaleLogType = bIsVertScaleLogType; };
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType){ m_bIsVertScaleInvertType = bIsVertScaleInvertType; };
	void SetAllHiding_PriceMA(const bool bIsHiding){ m_bAllHiding_PriceMA = bIsHiding; };
	void SetCalculteGraph(const bool bCalGraph){ m_bCalculateGraph = bCalGraph; };
	void SetInitializePacket(const bool bInitializePacket){ m_bInitializePacket = bInitializePacket; };

	CString GetPacketNames() const{ return m_strPacketNames; };
	CString GetSubGraphData() const{ return m_strSubGraphData; };
	bool IsSignal() const{ return m_bIsSignal; };
	bool IsVertScaleLogType() const{ return m_bIsVertScaleLogType; };
	bool IsVertScaleInvertType() const{ return m_bIsVertScaleInvertType; };
	bool IsAllHiding_PriceMA() const{ return m_bAllHiding_PriceMA; };
	bool IsCalculteGraph() const{ return m_bCalculateGraph; };
	bool IsInitializePacket() const{ return m_bInitializePacket; };

private:
	CString m_strPacketNames;
	CString m_strSubGraphData;
	bool m_bCalculateGraph;
	bool m_bIsSignal;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
	bool m_bAllHiding_PriceMA;
	bool m_bInitializePacket;
};
*/


class CIndicatorConfigurationData
{
public:
	enum INDICONFIG_RETURN
	{
		LOAD_ERROR = -1,
		CANCEL = 0,
		OK,
		GRAPH_DELETE
	};

	CIndicatorConfigurationData(CWnd* pParent, CMainBlock* pMainBlock, CIndicatorInfo* pIndicatorInfo, 
		const bool bIsVertScaleLogType, const bool bIsVertScaleInvertType, const bool bIsDeleteAll)
	{
		m_pParent = pParent;
		m_pMainBlock = pMainBlock;
		m_pIndicatorInfo = pIndicatorInfo;
		m_bIsVertScaleLogType = bIsVertScaleLogType;
		m_bIsVertScaleInvertType = bIsVertScaleInvertType;
		m_bIsDeleteAll = bIsDeleteAll;
	};

	CWnd* GetParent() const { return m_pParent; };
	CMainBlock* GetMainBlock() const { return m_pMainBlock; };
	CMainBlock* GetMainBlock() { return m_pMainBlock; };
	CIndicatorInfo* GetIndicatorInfo() const { return m_pIndicatorInfo; };
	CIndicatorInfo* GetIndicatorInfo() { return m_pIndicatorInfo; };
	bool IsVertScaleLogType() const { return m_bIsVertScaleLogType; };
	bool IsVertScaleInvertType() const { return m_bIsVertScaleInvertType; };
	bool IsDeleteAll() const { return m_bIsDeleteAll; };

private:
	CWnd* m_pParent;
	CMainBlock* m_pMainBlock;
	CIndicatorInfo* m_pIndicatorInfo;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
	bool m_bIsDeleteAll;
};


class CIndicatorConfigurationChangedData
{
public:
	CIndicatorConfigurationChangedData()
	{
		m_pIndicatorInfo = NULL;
		m_bCalculateGraph = false;
		m_bIsVertScaleLogType = false;
		m_bIsVertScaleInvertType = false;
	};

	void SetIndicatorInfo(CIndicatorInfo* pIndicatorInfo) { m_pIndicatorInfo = pIndicatorInfo; };
	void SetCalculteGraph(const bool bCalGraph) { m_bCalculateGraph = bCalGraph; };
	void SetVertScaleLogType(const bool bIsVertScaleLogType) { m_bIsVertScaleLogType = bIsVertScaleLogType; };
	void SetVertScaleInvertType(const bool bIsVertScaleInvertType) { m_bIsVertScaleInvertType = bIsVertScaleInvertType; };

	CIndicatorInfo* GetIndicatorInfo() const { return m_pIndicatorInfo; };
	bool IsCalculteGraph() const { return m_bCalculateGraph; };
	bool IsVertScaleLogType() const { return m_bIsVertScaleLogType; };
	bool IsVertScaleInvertType() const { return m_bIsVertScaleInvertType; };

private:
	CIndicatorInfo* m_pIndicatorInfo;
	bool m_bCalculateGraph;
	bool m_bIsVertScaleLogType;
	bool m_bIsVertScaleInvertType;
};


#endif // !defined(AFX_INDICATORCONFIGURATIONDATA_H__77298508_80CC_4DBB_BD5C_F4E9130FC9FA__INCLUDED_)
