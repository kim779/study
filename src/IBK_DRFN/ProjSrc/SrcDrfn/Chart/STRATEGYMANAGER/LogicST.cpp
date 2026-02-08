// LogicST.cpp: implementation of the CLogicST class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StrategyManager.h"
#include "LogicST.h"
#include "../Common_ST/STConfigDef.h"
#include "../Common_ST/DealDataItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogicST::CLogicST()
{
	m_bInitDataItem = FALSE;
	m_pDealDataItem = NULL;
	m_pIDateItem = NULL;
	m_bTrue = 0;
	m_lPos = 0;	
	m_bIsFilled = FALSE;
	m_chTimeType = NULL;
	m_strTimeNum = "00";
	m_pDrawInfo = NULL;

	m_dStartTimeInLastPeriod = 0;
	m_dEndTimeInLastPeriod = 0;
	m_posStartDealInLastPeriod = NULL;
	m_posEndDealInLastPeriod = NULL;
	m_posPassedDeal = NULL;

	m_pLoaderOfSTLoader = NULL;
}

CLogicST::~CLogicST()
{
	UnloadSTLoader();
}

void CLogicST::UnloadSTLoader()
{
	if(m_pLoaderOfSTLoader)
	{
		m_pLoaderOfSTLoader->Unload();
		delete m_pLoaderOfSTLoader;
		m_pLoaderOfSTLoader = NULL;
	}
}

// 20060705 함수인자 제거 : ,HWND hSocket/*,long pDBMgr*/
//BOOL CLogicST::Init(LPCTSTR lpSDNamePath,  long lPos, int nVlaue, LPCTSTR lpTime, LPCTSTR lpSTFilePath,HWND hParent, HWND hSocket/*,long pDBMgr*/)
//Parent 부모 윈도우: Manager에서 전략 로드할시..
BOOL CLogicST::Init(LPCTSTR lpSDNamePath,  long lPos, int nVlaue, LPCTSTR lpTime, LPCTSTR lpSTFilePath,HWND hParent, long pChartItem)
{
	m_strSDName = lpSDNamePath;
	m_lPos = lPos;
	m_bTrue = nVlaue;
	m_strSTFilePath = lpSTFilePath;	
	m_strTimeTypeNum = lpTime;
	CString strOption;
	strOption.Format("%s=%s",LOAD_MODE,MODE_ACTION);

	// 20060705 함수인자 제거 : ,hSocket/*,pDBMgr*/
	//long lRetValue = m_pLoaderOfSTLoader->Load(lpSDNamePath,hParent,hSocket/*,pDBMgr*/,(long)this,lpSTFilePath,lPos,strOption);
	if(m_pLoaderOfSTLoader)
		delete m_pLoaderOfSTLoader;

	char aCurPath[MAX_PATH], aDllPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, aCurPath);

	// "SYSTEMDLLPATH"의 셋팅은 멀티차트의 TabSystemTradingEx의 OnInitDialog에서 한다.
	if(::GetEnvironmentVariable("SYSTEMDLLPATH", aDllPath, MAX_PATH)>0)
		::SetCurrentDirectory(aDllPath);
	
	m_pLoaderOfSTLoader = new CDllSTLoader;
	long lRetValue = m_pLoaderOfSTLoader->Load(lpSDNamePath,hParent,(long)this,lpSTFilePath,lPos,strOption,pChartItem);

	::SetCurrentDirectory(aCurPath);
	if(lRetValue==-1)
	{
		::AfxMessageBox(m_pLoaderOfSTLoader->GetErrorMessage());
		return FALSE;
	}
	return TRUE;
}

CString CLogicST::GetPath()
{
	return m_strSDName;
}

//Load된 전략 파일에 따라 옵션을 적용 하여 Excute 시킨다. Excute는 전략로드포인터를 통해 실행된다.
long CLogicST::Run(LPCTSTR lpDataCode, long lDataCnt, LPCTSTR lpEndDate,LPCTSTR lpOption)
{
	CString strQuoteInputData;
	CString strEndDate;

	if(m_strTimeTypeNum.GetLength()>2)
	{
		CString strTimeType = m_strTimeTypeNum.Right(2);
		m_strTimeNum = m_strTimeTypeNum.Left(m_strTimeTypeNum.GetLength()-2);
		if(strTimeType==TIME_STRTICK)
		{
			m_chTimeType = TIME_TICK;
		}
		else
		{
			m_chTimeType = TIME_MIN;
		}
	}
	else
	{
//		if(m_strTimeTypeNum==TIME_STRMONTH)
//		{
//			m_chTimeType = TIME_MONTH;
//		}
//		else if(m_strTimeTypeNum==TIME_STRWEEK)
//		{
//			m_chTimeType = TIME_WEEK;
//		}
//		else //if(m_strTimeTypeNum=="일")
//		{
			m_chTimeType = TIME_DAY;
//		}
	}

	strEndDate = lpEndDate;
	//옵션세팅 
	strQuoteInputData.Format("%s=%s,%s=%s,%s=%d,%s=%c,%s=%s"
		,EXECUTE_SYMBOL
		,lpDataCode
		,EXECUTE_ENDDATE
		,strEndDate
		,EXECUTE_ALLCOUNT
		,lDataCnt
		,EXECUTE_TYPE
		,m_chTimeType
		,EXECUTE_TMNUMBER
		,m_strTimeNum);
	if(lpOption!=NULL)
	{
		strQuoteInputData = strQuoteInputData + "," + lpOption;
	}
	
	return m_pLoaderOfSTLoader->Execute(strQuoteInputData);//전략파일을 로드한 로드포인터다.
}

void CLogicST::SetConfig(int nUseExt, int nType, int nBongCount)
{
	// 이전, 이내, 연속 관련 설정 읽기
	m_nUseExt = nUseExt;
	m_nType = nType;
	m_nBongCount = nBongCount;
}

void CLogicST::CheckStrategySignal()
{
	// 이전, 이내, 연속 설정관련 값 확인
	// 설정값 FALSE이면 리턴
	if(m_nUseExt == 0 || m_pIDateItem == NULL)
		return;

	int nDateItemSize = m_pIDateItem->GetSize();
	m_arrayDeal_Src.RemoveAll();
	m_arrayDeal_Des.RemoveAll();
	m_arrayDeal_Src.SetSize(nDateItemSize);
	m_arrayDeal_Des.SetSize(nDateItemSize);

	int nSignalCount = GetStrategySignal();
	for(int nCurIndx = 0; nCurIndx < nDateItemSize; ++nCurIndx)
	{
		if(m_nType == 1 /* 이내 */)
		{
			int nIndx = nCurIndx - (m_nBongCount - 1);
			if(nIndx >= 0)		
				m_arrayDeal_Des[nCurIndx] = m_arrayDeal_Src[nIndx];		
		}
		else if(m_nType == 2 /* 이전 */)
		{
			int nIndx = nCurIndx - m_nBongCount;
			if(nIndx >= 0)
				m_arrayDeal_Des[nCurIndx] = m_arrayDeal_Src[nIndx];		
		}
		else if(m_nType == 3 /* 연속 */)
		{
			int nSeriesCount = m_nBongCount;
			int nIndx = nCurIndx;
			if(nIndx >= nSeriesCount - 1)
			{
				double dSignal = 0.;
				while(nSeriesCount)
				{
					if((dSignal = m_arrayDeal_Src[nIndx--]) == 0)
						break;

					nSeriesCount--;
				}
				
				if(dSignal != 0. && nSeriesCount == 0)
				{
					m_arrayDeal_Des[nCurIndx] = dSignal;
				}
			}
		}
	}

	SetStrategySignal(nSignalCount);
}


int CLogicST::GetStrategySignal()
{
	POSITION pos = m_pDealDataItem->GetHeadPosition();
	CDealData* pDealData = NULL;
	
	int nIndx = -1;
	long lDealValue = 0;
	
	int nSignalCount = 0;
	while(pos)
	{
		if(pDealData = m_pDealDataItem->GetNext(pos))
		{
			nIndx = pDealData->m_nIndex;
			lDealValue = pDealData->m_lDealValue;
			
			m_arrayDeal_Src.SetAt(nIndx, lDealValue);
			nSignalCount++;
		}
	}
	
	return nSignalCount;
}

void CLogicST::SetStrategySignal(int nSignalCount)
{
	POSITION pos = m_pDealDataItem->GetHeadPosition();
	CDealData* pDealData = NULL;

	int nDateItemSize = m_pIDateItem->GetSize();
	int nCount = 0;
	for(int nIndx = 0; nIndx < nDateItemSize; ++nIndx)
	{
		if(m_arrayDeal_Des[nIndx] == 0)
			continue;
		
		// 기존 DealData를 Shift
		if(pDealData = m_pDealDataItem->GetNext(pos))
		{
			pDealData->m_nIndex = nIndx;
			pDealData->m_lDealValue = (long)m_arrayDeal_Des[nIndx];
			nCount++;
		}
		// DealData가 추가되어야 할 경우
		else
		{
			m_pDealDataItem->SetData(nIndx, (long)m_arrayDeal_Des[nIndx]);
		}
	}
	
	// DealData의 갯수가 줄어든 경우
	if(nSignalCount > nCount)
	{
		int nDelCount = nSignalCount - nCount;
		while(nDelCount--)
		{
			m_pDealDataItem->RemoveTail();
		}
	}
}

void CLogicST::ReceiveFromSTDataManager(LPARAM lParam)
{
	if(!m_bInitDataItem)
	{
		CIDataItem *pDataItem = NULL;		
		m_pDrawInfo = (ST_DRAWINFO*)lParam;
		CPtrArray* pPtrArraySDataItem = m_pDrawInfo->pPtrArraySDataItem;
		if(pPtrArraySDataItem->GetSize())
		{
			m_pIDateItem = (CISDataItem *)pPtrArraySDataItem->GetAt(0);
		}

		m_pDealDataItem = (CDealDataItem*)m_pDrawInfo->pDealDataItem;

		// 이전, 이내, 연속 체크
		CheckStrategySignal();

		m_bInitDataItem = TRUE;
	}
	m_bIsFilled = TRUE;
}

BOOL CLogicST::IsFilled()
{
	return m_bIsFilled;
}

void CLogicST::InitFilledSet()
{
	m_bIsFilled = FALSE;
}

long CLogicST::GetTimeTypeNum()
{
	return (atol(&m_chTimeType)*100+atol(m_strTimeNum));
}

CDealData* CLogicST::GetValue(LPCTSTR lpTime, char chType,LPCTSTR lpCurrentTime,int nPos, LPCTSTR lpCurrentTimeType)
{
	long lRetValue = 0;
	CDealData* pDealData = NULL;
	if(m_strTimeTypeNum==lpCurrentTimeType)
	{
		POSITION posCurrent;
		POSITION posStartDealData = NULL;
		if(m_posPassedDeal)
		{
			posStartDealData = m_posPassedDeal;
		}
		else
		{
			posStartDealData = m_pDealDataItem->GetHeadPosition();
		}
		
		while(posStartDealData)
		{
			posCurrent = posStartDealData;
			pDealData = m_pDealDataItem->GetNext(posStartDealData);
			//예외
			if(pDealData->m_nIndex>nPos)
			{
				pDealData = NULL;
				break;
			}
			if(pDealData->m_nIndex==nPos)
			{
				lRetValue = pDealData->m_lDealValue;
				break;
			}

			m_posPassedDeal = posCurrent;
		}
	}
	else
	{
		CString strDate;
		int nDateItemSize = m_pIDateItem->GetSize();
		double dTimeValue = 0;
		CString strCurrentTime = lpCurrentTime;
		double dYY = atof(strCurrentTime.Left(2));
		double dYYMMDD = atof(strCurrentTime.Left(6));
		double dYYMM = atof(strCurrentTime.Left(4));
		// 월	YYYYMM00
		// 주	YYYYMMDD
		// 일	YYYYMMDD

		// 분	MMDDHHMM
		// 틱	DDHHMMSS
		//	YYMMDDHHMMSS
		switch(chType)
		{
		case TIME_MIN:// MMDDHHMM => YYMMDDHHMMSS
			{
				dTimeValue = atof(lpTime)*100 + dYY*10000000000;
			}
			break;
		case TIME_TICK:// DDHHMMSS => YYMMDDHHMMSS
			{
				dTimeValue = atof(lpTime) + dYYMM*100000000;
			}
			break;
		default:// YYMMDD => YYMMDDHHMMSS
			{
				dTimeValue = atof(lpTime)*1000000;
			}
			break;
		}

	/////////////////////////////////
		
		POSITION posStartDealData = NULL;
		if(m_posPassedDeal)
		{
			if(m_dStartTimeInLastPeriod<=dTimeValue&&m_dEndTimeInLastPeriod>=dTimeValue)
			{
				posStartDealData = m_posStartDealInLastPeriod;
			}
			else
			{
				posStartDealData = m_posPassedDeal;
				m_pDealDataItem->GetNext(posStartDealData);
			}
		}
		else
		{
			posStartDealData = m_pDealDataItem->GetHeadPosition();
		}
		

	//////////////////////////
		double dTimeValueInside = 0;
		int nIndexOfDate = 0;
		BOOL bFirstLooping = TRUE;
		CDealData* pDealDataNext = NULL;
		POSITION posCurrent;
		while(posStartDealData)
		{
			posCurrent = posStartDealData;
			pDealDataNext = m_pDealDataItem->GetNext(posStartDealData);
			nIndexOfDate = pDealDataNext->m_nIndex;
			strDate = m_pIDateItem->GetAt(nIndexOfDate);		
			switch(m_chTimeType)
			{
			case TIME_MIN:
				{
					dTimeValueInside = atof(strDate)*100 + dYY*10000000000;
				}
				break;
			case TIME_TICK:
				{
					dTimeValueInside = atof(strDate) + dYYMM*100000000;
				}
				break;
			default:
				{
					dTimeValueInside = atof(strDate)*1000000;
				}
				break;
			}
			if(dTimeValueInside>dTimeValue)
			{			
				break;
			}
			pDealData = pDealDataNext;
			if(bFirstLooping)
			{
				m_dStartTimeInLastPeriod = dTimeValueInside;
				m_posStartDealInLastPeriod = posCurrent;
				bFirstLooping = FALSE;
			}
			m_dEndTimeInLastPeriod = dTimeValueInside;
			m_posEndDealInLastPeriod = posCurrent;
			
			if(pDealData->m_lDealValue>0&&pDealData->m_lDealValue!=lRetValue)
			{
				if(lRetValue==0)
				{
					lRetValue = pDealData->m_lDealValue;
				}
				else if(	(pDealData->m_lDealValue&VALBUY)&&(lRetValue&VALSELL)
							||(pDealData->m_lDealValue&VALSELL)&&(lRetValue&VALBUY)	)
				{
					lRetValue = 0;
				}
				else if(pDealData->m_lDealValue&VALEXITLONG&&lRetValue&VALENTRYSELL)
				{
					lRetValue = VALBUY;
				}
				else if(pDealData->m_lDealValue&VALEXITSHORT&&lRetValue&VALENTRYBUY)
				{
					lRetValue = VALSELL;
				}
			}
			m_posPassedDeal = posCurrent;
		}
	}//else


	if(!m_bTrue&&lRetValue)
	{
		if(lRetValue&VALEXITLONG)	
		{
			lRetValue &= ~VALEXITLONG;
			lRetValue |= VALEXITSHORT;			
		}
		else if(lRetValue&VALEXITSHORT)	
		{
			lRetValue &= ~VALEXITSHORT;
			lRetValue |= VALEXITLONG;
		}
		if(lRetValue&VALENTRYSELL)	
		{
			lRetValue &= ~VALENTRYSELL;
			lRetValue |= VALENTRYBUY;
		}
		else if(lRetValue&VALENTRYBUY)	
		{
			lRetValue &= ~VALENTRYBUY;
			lRetValue |= VALENTRYSELL;
		}
	}
	if(pDealData&&lRetValue)
	{
		m_DealData = *pDealData;
		m_DealData.m_lDealValue = lRetValue;	
	}
	else
	{		
		m_DealData.Clear();
	}
	//KHD : Test 2006.08.23VALENTRYSELL : 현재 강제로...데이터 나오게..
	//m_DealData.m_lDealValue = VALEXITSHORT;
	//m_DealData.m_lDealValue|= VALENTRYBUY;
	return &m_DealData;
}


BOOL CLogicST::Stop()
{
	BOOL bRetValue = m_pLoaderOfSTLoader->Stop();
	if(bRetValue)
	{
		m_bInitDataItem	= FALSE;
	}	
	return bRetValue;
}


//@060922전략Real관련수정
long CLogicST::Update(long lPos, BOOL bIsUpdateData)
{
	long lRetValue = m_pLoaderOfSTLoader->Update(lPos, bIsUpdateData);
	return lRetValue;
}


BOOL CLogicST::Pause()
{
	return m_pLoaderOfSTLoader->Pause();
}

CLogicST *CLogicST::GetLogicST(long lIndex)
{
	if(!lIndex)	return this;
	return NULL;
}

CString CLogicST::GetDateValue(long lIndex)
{
	if(m_pIDateItem)
	{
		return m_pIDateItem->GetAt(lIndex);
	}
	return "";
}

long CLogicST::GetDataCount()
{
	if(m_pIDateItem)
	{
		return m_pIDateItem->GetSize();
	}
	return 0;		
}

long CLogicST::GetPtrST()
{
	return (long)m_pDrawInfo;
}


CPtrArray* CLogicST::GetPArrayHistoryData()
{
	if(m_pDrawInfo)
	{
		return m_pDrawInfo->pPtrArrayHistoryDataItem;
	}
	return NULL;
}


CString CLogicST::GetInputs(long lIndex)
{
	return m_pLoaderOfSTLoader->GetInput(lIndex);
}