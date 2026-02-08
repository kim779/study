// DrawingTimeData.cpp: implementation of the CDrawingTimeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawingTimeData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CDrawingTimeData

CDrawingTimeData& CDrawingTimeData::operator=(const CDrawingTimeData& data)
{
	if(this == &data)
		return *this;
	
	m_nIndex = data.GetIndex();
	m_strTimeData = data.GetTimeData();
	m_nDate = data.m_nDate;
	m_nTime = data.m_nTime;

	return *this;
}

CDrawingTimeData::CDrawingTimeData(const CDrawingTimeData& data):
	m_nIndex(data.GetIndex()),
	m_strTimeData(data.GetTimeData()),
	m_nDate(data.m_nDate),
	m_nTime(data.m_nTime)
{
}

CDrawingTimeData::CDrawingTimeData(const int nIndex, const CString& strTimeData):
	m_nIndex(nIndex),
	m_strTimeData(strTimeData),
	m_nDate(-1),
	m_nTime(-1)
{
}

CDrawingTimeData::CDrawingTimeData(const int nIndex, const int nDate, const int nTime):
	m_nIndex(nIndex),
	m_strTimeData(""),
	m_nDate(nDate),
	m_nTime(nTime)
{
}

// public ===========================================================
void CDrawingTimeData::SetIndex(const int nIndex)
{
	m_nIndex = nIndex;
}

void CDrawingTimeData::SetTimeData(const CString& strTimeData)
{
	m_strTimeData = strTimeData;
}

// ------------------------------------------------------------------
bool CDrawingTimeData::IsExactData() const
{
	if(m_nIndex < 0)
		return false;

	return (!m_strTimeData.IsEmpty() || m_nTime != -1);
}

int CDrawingTimeData::GetIndex() const
{
	return m_nIndex;
}

CString CDrawingTimeData::GetTimeData() const
{
	return m_strTimeData;
}

//////////////////////////////////////////////////////////////////////
// class CDrawingHourData

CDrawingHourData::~CDrawingHourData()
{
	DeleteAll();
}

// public ============================================================
bool CDrawingHourData::AddDrawingTimeData(const CDrawingTimeData& drawingTimeData)
{
	if(!drawingTimeData.IsExactData())
		return false;

	m_timeDataList.AddTail(new CDrawingTimeData(drawingTimeData));
	return true;
}

bool CDrawingHourData::AddDrawingTimeData(const int nIndex, const CString& strTimeData)
{
	if(nIndex < 0 || strTimeData.IsEmpty())
		return false;

	m_timeDataList.AddTail(new CDrawingTimeData(nIndex, strTimeData));
	return true;
}

bool CDrawingHourData::AddDrawingTimeData(const int nIndex, const int nDate, const int nTime)
{
	if(nIndex < 0 || nTime < 0) return false;

	m_timeDataList.AddTail(new CDrawingTimeData(nIndex, nDate, nTime));
	return true;
}

bool CDrawingHourData::DeleteAll()
{
	if(m_timeDataList.GetCount() <= 0)
		return false;

	POSITION pos = m_timeDataList.GetHeadPosition();
	while(pos != NULL){
		delete m_timeDataList.GetNext(pos);
	}
	m_timeDataList.RemoveAll();
	return true;
}

// -------------------------------------------------------------------
CList<CDrawingTimeData*, CDrawingTimeData*>* CDrawingHourData::GetTimeDataList()
{
	return &m_timeDataList;
}

int CDrawingHourData::GetTimeDataCount() const
{
	return m_timeDataList.GetCount();
}

POSITION CDrawingHourData::GetHeadTimeDataPosition() const
{
	return m_timeDataList.GetHeadPosition();
}

POSITION CDrawingHourData::GetTailTimeDataPosition() const
{
	return m_timeDataList.GetTailPosition();
}

CDrawingTimeData* CDrawingHourData::GetHeadTimeData() const
{
	return GetTimeData(m_timeDataList.GetHeadPosition());
}

CDrawingTimeData* CDrawingHourData::GetTimeData(const int nIndex) const
{
	return GetTimeData(m_timeDataList.FindIndex(nIndex));
}

CDrawingTimeData* CDrawingHourData::GetTimeData(const POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_timeDataList.GetAt(pos);
}

CDrawingTimeData* CDrawingHourData::GetNext_TimeData(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_timeDataList.GetNext(pos);
}

int CDrawingHourData::GetHeadTimeData_Index() const
{
	CDrawingTimeData* pDrawingTimeData = GetTimeData(0);
	if(pDrawingTimeData == NULL)
		return -1;

	return pDrawingTimeData->GetIndex();
}

CString CDrawingHourData::GetHeadTimeData_Data() const
{
	CDrawingTimeData* pDrawingTimeData = GetTimeData(0);
	if(pDrawingTimeData == NULL)
		return "-1";//tour2k

	return pDrawingTimeData->GetTimeData();
}


//////////////////////////////////////////////////////////////////////
// class CDrawingDateData

CDrawingDateData::CDrawingDateData(const CString& strDateData):
	m_strDateData(strDateData),
	m_nDate(-1),
	m_nDay(-1)
{
}

CDrawingDateData::CDrawingDateData(const CString& strDateData, const CDrawingTimeData& drawingTimeData):
	m_strDateData(strDateData),
	m_nDate(-1),
	m_nDay(-1)
{
	AddDrawingHourData(drawingTimeData);
}

CDrawingDateData::CDrawingDateData(const int nDate, const int nDay, const CDrawingTimeData& drawingTimeData):
	m_nDate(nDate),
	m_nDay(nDay)
{
	AddDrawingHourData(drawingTimeData);
}

CDrawingDateData::~CDrawingDateData()
{
	DeleteAll();
}

// public ============================================================
bool CDrawingDateData::AddDrawingHourData(const CDrawingTimeData& drawingTimeData)
{
	if(!drawingTimeData.IsExactData())
		return false;

	CDrawingHourData* pDrawingHourData = new CDrawingHourData();
	pDrawingHourData->AddDrawingTimeData(drawingTimeData);
	m_hourDataList.AddTail(pDrawingHourData);
	return true;
}

bool CDrawingDateData::AddDrawingHourData(const int nIndex, const CString& strTimeData)
{
	if(nIndex < 0 || strTimeData.IsEmpty())
		return false;

	CDrawingHourData* pDrawingHourData = new CDrawingHourData();
	pDrawingHourData->AddDrawingTimeData(nIndex, strTimeData);
	m_hourDataList.AddTail(pDrawingHourData);
	return true;
}

bool CDrawingDateData::AddDrawingHourData(const int nIndex, int nDate, int nTime)
{
	if(nIndex < 0 || nTime < 0) return false;

	CDrawingHourData* pDrawingHourData = new CDrawingHourData();
	pDrawingHourData->AddDrawingTimeData(nIndex, nDate, nTime);
	m_hourDataList.AddTail(pDrawingHourData);
	return true;
}

bool CDrawingDateData::DeleteAll()
{
	if(m_hourDataList.GetCount() <= 0)
		return false;

	POSITION pos = m_hourDataList.GetHeadPosition();
	while(pos != NULL){
		delete m_hourDataList.GetNext(pos);
	}
	m_hourDataList.RemoveAll();
	return true;
}

// -------------------------------------------------------------------
void CDrawingDateData::SetDateData(const CString& strDateData)
{
	m_strDateData = strDateData;
}

// -------------------------------------------------------------------
CString CDrawingDateData::GetDateData() const
{
	return m_strDateData;
}

CList<CDrawingHourData*, CDrawingHourData*>* CDrawingDateData::GetHourDataList()
{
	return &m_hourDataList;
}

int CDrawingDateData::GetHourDataCount() const
{
	return m_hourDataList.GetCount();
}

POSITION CDrawingDateData::GetHeadHourDataPosition() const
{
	return m_hourDataList.GetHeadPosition();
}

POSITION CDrawingDateData::GetTailHourDataPosition() const
{
	return m_hourDataList.GetTailPosition();
}

CDrawingHourData* CDrawingDateData::GetHourData(const int nIndex) const
{
	return GetHourData(m_hourDataList.FindIndex(nIndex));
}

CDrawingHourData* CDrawingDateData::GetHourData(const POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_hourDataList.GetAt(pos);
}

CDrawingHourData* CDrawingDateData::GetNext_HourData(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_hourDataList.GetNext(pos);
}

CDrawingTimeData* CDrawingDateData::GetTimeData(const int nHourIndex, const int nTimeIndex) const
{
	CDrawingHourData* pDrawingHourData = GetHourData(nHourIndex);
	if(pDrawingHourData == NULL)
		return NULL;
	return pDrawingHourData->GetTimeData(nTimeIndex);
}

int CDrawingDateData::GetHeadTimeData_Index() const
{
	CDrawingHourData* pDrawingHourData = GetHourData(0);
	if(pDrawingHourData == NULL)
		return -1;

	return pDrawingHourData->GetHeadTimeData_Index();
}

CString CDrawingDateData::GetHeadTimeData_Data() const
{
	CDrawingHourData* pDrawingHourData = GetHourData(0);
	if(pDrawingHourData == NULL)
		return "-1";//tour2k

	return pDrawingHourData->GetHeadTimeData_Data();
}


//////////////////////////////////////////////////////////////////////
// class CDrawingDateDataList

CDrawingDateDataList::CDrawingDateDataList() :
	m_bIsDailyDrawing(true)
{
}

CDrawingDateDataList::~CDrawingDateDataList()
{
	DeleteAll();
}

// public ============================================================
bool CDrawingDateDataList::AddDrawingDateData(CDrawingDateData* pDrawingDateData)
{
	if(pDrawingDateData == NULL)
		return false;

	m_drawingDateDataList.AddTail(pDrawingDateData);
	return true;
}

bool CDrawingDateDataList::DeleteAll()
{
	if(m_drawingDateDataList.GetCount() <= 0)
		return false;

	POSITION pos = m_drawingDateDataList.GetHeadPosition();
	while(pos != NULL){
		delete m_drawingDateDataList.GetNext(pos);
	}
	m_drawingDateDataList.RemoveAll();
	return true;
}

// ----------------------------------------------------------------------------
void CDrawingDateDataList::SetDailyDrawing(const bool bIsDailyDrawing)
{
	m_bIsDailyDrawing = bIsDailyDrawing;
}

// ----------------------------------------------------------------------------
bool CDrawingDateDataList::IsDailyDrawing() const
{
	return m_bIsDailyDrawing;
}

CList<CDrawingDateData*, CDrawingDateData*>* CDrawingDateDataList::GetDrawingDateDataList()
{
	return &m_drawingDateDataList;
}

int CDrawingDateDataList::GetDateDataCount() const
{
	return m_drawingDateDataList.GetCount();
}

POSITION CDrawingDateDataList::GetHeadDateDataPosition() const
{
	return m_drawingDateDataList.GetHeadPosition();
}

POSITION CDrawingDateDataList::GetTailDateDataPosition() const
{
	return m_drawingDateDataList.GetTailPosition();
}

CDrawingDateData* CDrawingDateDataList::GetDateData(const int nIndex) const
{
	return GetDateData(m_drawingDateDataList.FindIndex(nIndex));
}

CDrawingDateData* CDrawingDateDataList::GetDateData(const POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_drawingDateDataList.GetAt(pos);
}

CDrawingDateData* CDrawingDateDataList::GetNext_DateData(POSITION& pos) const
{
	if(pos == NULL)
		return NULL;
	return m_drawingDateDataList.GetNext(pos);
}
