// DrawingTimeData.h: interface for the CDrawingTimeData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGTIMEDATA_H__1CBDE97C_069F_41EA_9F7F_F53BFBAEA66B__INCLUDED_)
#define AFX_DRAWINGTIMEDATA_H__1CBDE97C_069F_41EA_9F7F_F53BFBAEA66B__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDrawingTimeData
{
public:
	CDrawingTimeData& operator=(const CDrawingTimeData& data);

public:
	CDrawingTimeData(const CDrawingTimeData& data);
	CDrawingTimeData(const int nIndex = -1, const CString& strTimeData = "");
	CDrawingTimeData(const int nIndex, const int nDate, const int nTime);

	void SetIndex(const int nIndex);
	void SetTimeData(const CString& strTimeData);

	bool IsExactData() const;
	int GetIndex() const;
	CString GetTimeData() const;

public:
	int m_nDate;
	int m_nTime;

private:
	int m_nIndex;
	CString m_strTimeData;
};


class CDrawingHourData
{
public:
	~CDrawingHourData();

	bool AddDrawingTimeData(const CDrawingTimeData& drawingTimeData);
	bool AddDrawingTimeData(const int nIndex, const CString& strTimeData);
	bool AddDrawingTimeData(const int nIndex, const int nDate, const int nTime);
	bool DeleteAll();

	CList<CDrawingTimeData*, CDrawingTimeData*>* GetTimeDataList();
	int GetTimeDataCount() const;
	POSITION GetHeadTimeDataPosition() const;
	POSITION GetTailTimeDataPosition() const;
	CDrawingTimeData* GetHeadTimeData() const;
	CDrawingTimeData* GetTimeData(const int nIndex) const;
	CDrawingTimeData* GetTimeData(const POSITION& pos) const;
	CDrawingTimeData* GetNext_TimeData(POSITION& pos) const;
	int GetHeadTimeData_Index() const;
	CString GetHeadTimeData_Data() const;

private:
	CList<CDrawingTimeData*, CDrawingTimeData*> m_timeDataList;
};


class CDrawingDateData
{
public:
	CDrawingDateData(const CString& strDateData);
	CDrawingDateData(const CString& strDateData, const CDrawingTimeData& drawingTimeData);
	CDrawingDateData(const int nDate, const int nDay, const CDrawingTimeData& drawingTimeData);
	~CDrawingDateData();

	bool AddDrawingHourData(const CDrawingTimeData& drawingTimeData);
	bool AddDrawingHourData(const int nIndex, const CString& strTimeData);
	bool AddDrawingHourData(const int nIndex, int nDate, int nTime);	
	bool DeleteAll();

	void SetDateData(const CString& strDateData);

	CString GetDateData() const;
	CList<CDrawingHourData*, CDrawingHourData*>* GetHourDataList();
	int GetHourDataCount() const;
	POSITION GetHeadHourDataPosition() const;
	POSITION GetTailHourDataPosition() const;
	CDrawingHourData* GetHourData(const int nIndex) const;
	CDrawingHourData* GetHourData(const POSITION& pos) const;
	CDrawingHourData* GetNext_HourData(POSITION& pos) const;
	CDrawingTimeData* GetTimeData(const int nHourIndex, const int nTimeIndex) const;
	int GetHeadTimeData_Index() const;
	CString GetHeadTimeData_Data() const;

public:
	int m_nDate;
	int m_nDay;

private:
	CString m_strDateData;
	CList<CDrawingHourData*, CDrawingHourData*> m_hourDataList;
};


class CDrawingDateDataList
{
public:
	CDrawingDateDataList();
	~CDrawingDateDataList();

	bool AddDrawingDateData(CDrawingDateData* pDrawingDateData);
	bool DeleteAll();

	void SetDailyDrawing(const bool bIsDailyDrawing);

	bool IsDailyDrawing() const;
	CList<CDrawingDateData*, CDrawingDateData*>* GetDrawingDateDataList();
	int GetDateDataCount() const;
	POSITION GetHeadDateDataPosition() const;
	POSITION GetTailDateDataPosition() const;
	CDrawingDateData* GetDateData(const int nIndex) const;
	CDrawingDateData* GetDateData(const POSITION& pos) const;
	CDrawingDateData* GetNext_DateData(POSITION& pos) const;

private:
	bool m_bIsDailyDrawing;
	CList<CDrawingDateData*, CDrawingDateData*> m_drawingDateDataList;
};

#endif // !defined(AFX_DRAWINGTIMEDATA_H__1CBDE97C_069F_41EA_9F7F_F53BFBAEA66B__INCLUDED_)
