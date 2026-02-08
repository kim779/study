// NumericalInquiryDataQueryer.cpp: implementation of the CNumericalInquiryDataQueryer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NumericalInquiryDataQueryer.h"

#include "../Include_Chart/Conversion.h"								// for CDataConversion
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_AddIn_133101/I133101/_IPacketListManager.h"		// for IPacketListManager
#include "../Include_AddIn_133101/I133101/_IStringList.h"				// for IStringList

#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumericalInquiryDataQueryer::CNumericalInquiryDataQueryer( IChartOCX *p_pIChartOCX)
{
	// (2006/6/24 - Seung-Won, Bae) Manage the OCX Interfaces
	m_pIChartOCX				= p_pIChartOCX;
	m_pIChartManager			= NULL;
	m_pIDefaultPacketManager	= NULL;
	m_pIPacketListManager		= NULL;
	if( m_pIChartOCX)
	{
		m_pIChartOCX->AddRef();
		m_pIChartManager			= p_pIChartOCX->GetIChartManager();
		m_pIDefaultPacketManager	= p_pIChartOCX->GetIDefaultPacketManager();
		m_pIPacketListManager		= p_pIChartOCX->GetIPacketListManager();
	}

	// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in DongBu
	m_bWithHorizontalLine = TRUE;
	m_hOcxWnd = ( m_pIChartOCX ? m_pIChartOCX->GetOcxHwnd() : NULL);

	// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
	m_eChartMode = ( CChartInfo::CHART_MODE)m_pIChartOCX->GetChartMode();
}

CNumericalInquiryDataQueryer::~CNumericalInquiryDataQueryer()
{
	// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
	ClearInquiryDataList();
	
	// (2006/6/24 - Seung-Won, Bae) Manage the OCX Interfaces
	if( m_pIChartOCX)				m_pIChartOCX->Release();
	if( m_pIChartManager)			m_pIChartManager->Release();
	if( m_pIDefaultPacketManager)	m_pIDefaultPacketManager->Release();
	if( m_pIPacketListManager)		m_pIPacketListManager->Release();
}


//////////////////////////////////////////////////////////////////////
// (2006/6/24 - Seung-Won, Bae) Overations
//////////////////////////////////////////////////////////////////////
// XScale Text 툴팁표시 - ojtaso (20080429)
BOOL CNumericalInquiryDataQueryer::ParsingInquiryHorzScaleTextData(CString szRQ, CPoint point, CString& strScaleText)
{
	CInquiryItem *plistData = NULL;
	if(!szRQ.IsEmpty())
	{
		POSITION psnViewData = NULL;
		psnViewData = m_pIChartManager->GetSubGraphViewData_First(szRQ);
		if( !psnViewData) return FALSE;

		ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
		CString strHorzScaleName( szHorzScaleName);

		IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strHorzScaleName);
		if( pPacket == NULL) return FALSE;

		int nDataIndex = -1;

		POSITION pos = NULL;
		double dData = 0;

		if( m_pIPacketListManager->IsBasePacketString())
		{
			nDataIndex = FindInquiryDataIndex(point, psnViewData);

			if( nDataIndex < 0)
			{
				m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
				return FALSE;
			}

			ILPCSTR szData = NULL;
			pPacket->GetData( nDataIndex, szData);
			if( !szData.IsEmpty()) strScaleText = szData;
		}
		else
		{
			nDataIndex = m_pIChartManager->FindViewDataIndex_Pt(szRQ, point.x);

			if( nDataIndex < 0)
			{
				m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
				return FALSE;
			}
			
			dData = 0.0;
			if( !pPacket->GetData( nDataIndex, dData))
				pPacket->GetDataPastandFuture( nDataIndex, dData);

			if(dData == 0.0)
			{
				pPacket->Release();
				return FALSE;
			}

			// 자료일자 중복 표시 수정 - ojtaso (20080125)
			if(strHorzScaleName.Find("자료일자") < 0)
				strScaleText = CDataConversion::DoubleToString(dData);
			else 
			{
				// 마우스위치한곳의 Block Index를 가져온다.
				int nRow, nColumn;
				if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn))
				{
					m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
					return FALSE;
				}

				// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
				ILPCSTR szDateCompart, szTimeCompart;
				m_pIChartManager->GetHorzScaleCompart( nRow, nColumn, szDateCompart, szTimeCompart);
				CString strCompartDate( szDateCompart);
				CString strCompartTime( szTimeCompart);

				ILPCSTR szPacketType = pPacket->GetType();
				CString strPacketType( szPacketType);
				strScaleText = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(CDataConversion::IntToString((int)dData), strPacketType, strCompartDate, strCompartTime);
			}

		}
	
		pPacket->Release();
	}

	return TRUE;
}

// 1. Get the Data
/*-------------------------------------------------------------------------------
 - Function    :  ParsingInquiryData
 - Created at  :  2005-03-22   09:23
 - Author      :  최종찬
 - Parameters  :  point - 해당 마우스 포인터.
				  bToolTip - 툴팁에서 데이타를 조회할 경우.(Data조회필터링함수를 뛰어넘어야 한다.)
 - Return Value:  BOOL - 파싱성공, FALSE - 실패.
 - Description :  point에 위치한 수치조회데이타를 구하여 list컨테이너에 담아 리턴한다.
				  ※ _LISTInquiryData 수치조회데이타'리스트'를 담을 컨테이너.(h파일 typedef 참조)
				  ※ pair<CString, CString>은  <타이틀, 데이타>로 구성.
				  ※ list< pair<CStirng, CString> >는 <타이틀, 데이타>'리스트'로 구성 (list하나가 자료일자 하나와 매칭됨)
				  ※ list< list<.......> >는 수치조회데이타'리스트'의 '리스트'로써, 해당일전후 5일간의 조회데이타 list임.	(Multi Line Dlg용.)
 - Update	   :  (05/09/05) 하나의 캔들안에서 마우스가 움직일때는 여타의 수치조회데이타는 변경할 필요가 없으나
							수평대값은 변경해주어야 한다. 작업처리함.
 -------------------------------------------------------------------------------*/
//<<[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
//BOOL CNumericalInquiryDataQueryer::ParsingInquiryData(CString szRQ, CPoint point, bool bToolTip, BOOL bPairTick)
BOOL CNumericalInquiryDataQueryer::ParsingInquiryData(CString szRQ, CPoint point, bool bToolTip/*=FALSE*/, BOOL bPairTick/*=FALSE*/, BOOL bViewAll/*=TRUE*/)
//>>
{
	m_nMaxDecimal = 0;	// 수치조회데이타 소숫점 최대 자릿수 초기화

	// 절차.
	// 1. MainBlock에서 ViewData를 가져오고... 거기서 MousePoint가 위치한 곳의
	// 2. RowViewData만 추출한다.
	// 3. MousePoint가 위치한곳의 데이타인덱스를 구한다.
	// 4. 데이타인덱스가 이전에 조회한 데이타인덱스와 같을 경우, 굳이 다시 조회해올 필요가 없다.
	// 5. 1차적으로 '자료일자'의 데이타만을 추출해서 Container에 삽입한다.
	// 6. 그리고 반복구문으로 RowViewData에서 SubGraphViewData를 가져와서 Container에 삽입.
	// 7. 마지막으로 '가격차트'대에 MousePoint가 위치해있으면 수평대값을 구해서와서 Container에 삽입.

	CInquiryItem *plistData = NULL;
	CInquiryItem *plistPrevData = NULL;
	if(!szRQ.IsEmpty())
	{
		// ViewData를 가져온다. (여기서 패킷명을 얻어온다.)
		POSITION psnViewData = NULL;
		psnViewData = m_pIChartManager->GetSubGraphViewData_First(szRQ);
		if( !psnViewData) return FALSE;

		// 마우스위치한곳의 Block Index를 가져온다.
		int nRow, nColumn;
		if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// 데이타인덱스 구해오기.
		// 인덱스 구해오기에 필요한 RowViewData를 가져온다.
	/*	if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, nRow, nColumn)) return FALSE;*/
		if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, 0, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
		CString strHorzScaleName( szHorzScaleName);

		// 인덱스 구함.	
		// XScale : 영역의 시작/끝 인덱스 구하기 - vntsorl(20070327)
	//	int nDataIndex = FindInquiryDataIndex(point, psnViewData);
		int nDataIndex = 0;
		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		BOOL bInverseLine = ( strHorzScaleName == _MTEXT( C0_INVERSETIMELINE_VOLUME));
		if( bInverseLine) nDataIndex = m_pIChartManager->GetDataIndexOfInverseLine( point);
		// 문자형 데이타인지 : xScaleManager - ojtaso (20070703)
		else if( m_pIPacketListManager->IsBasePacketString())
			nDataIndex = FindInquiryDataIndex(point, psnViewData);
		else nDataIndex = m_pIChartManager->FindViewDataIndex_Pt(szRQ, point.x);

		if( nDataIndex < 0)
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// 종찬(05/09/05) MouseMove범위가 하나의 캔들속이라해도 수평대값은 변해야한다.
		// 새롭게 데이타를 넣어야 하니까 컨테이너를 일단 비운다.
		// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
		// (2008/9/14 - Seung-Won, Bae) for Pair Tick
		if( !bPairTick)
		{
			ClearInquiryDataList();
			plistData = new CInquiryItem;
			plistPrevData = new CInquiryItem;
		}
		else
		{
			if( m_listInquiryData.IsEmpty()) m_listInquiryData.AddTail( new CInquiryItem);
			if( m_listPrevInquiryData.IsEmpty()) m_listPrevInquiryData.AddTail( new CInquiryItem);
			plistData = m_listInquiryData.GetHead();
			plistPrevData = m_listPrevInquiryData.GetHead();

			// (2008/11/9 - Seung-Won, Bae) Delete Horizontal Value.
			if( !plistData->IsEmpty()) delete plistData->RemoveTail();	// 수평대
			if( !plistData->IsEmpty()) delete plistData->RemoveTail();	// 공백
		}
		
		// 실질적으로 데이타를 파싱하여 Container에 적재한다.
		BOOL bHorzScaleData = FALSE;
		bool bScaleData_Date = false;
		if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, 0, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		if( !bInverseLine)
		{
			// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
			// (2008/9/14 - Seung-Won, Bae) for Pair Tick
			if( !bPairTick)	bHorzScaleData = SetHorzScaleDataToContainer(szRQ, strHorzScaleName, nDataIndex, plistData, nRow, nColumn);
			else			bHorzScaleData = TRUE;
			SetHorzScaleDataToContainer(szRQ, strHorzScaleName, max(0,nDataIndex-1), plistPrevData, nRow, nColumn);
			// 스케일데이타가 '자료일자'일 경우는 1줄짜리 수치조회창에서는 필터링을 한다.
			if( strHorzScaleName == _MTEXT( C0_DATE_TIME)) bScaleData_Date = true;
		}

		int nNextPacketIndex = -1;
		CString strTitle, strPacketNames;
		bool bSetTitle = false;

		// (2008/10/7 - Seung-Won, Bae) Show Info for only One Block.
		CRect rctGraphRegion;
		IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
		if( pIBlock)
		{
			CRect rctNoFrameAndMargin;
			pIBlock->GetGraphRegions( rctGraphRegion, rctNoFrameAndMargin);
			pIBlock->Release();
		}

		BOOL bNoGraph = TRUE;
		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		if( bHorzScaleData || bInverseLine) while( psnViewData)
		{
			//>>[A00000441]풍선도움말 보기시 전체 조회 옵션 추가
			if( bViewAll == FALSE )
			{
				// (2008/10/7 - Seung-Won, Bae) Show Info for only One Block.
				CRect rctSubGraphRegion = m_pIChartManager->GetSubGraphViewData_GraphRegion( psnViewData);
				CRect rctUnionRegion;
				if( !rctUnionRegion.IntersectRect( rctGraphRegion, rctSubGraphRegion))
				{
					m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
					continue;
				}
			}	//<< 
			//<< 

			bNoGraph = FALSE;
			ILPCSTR szTitle, szPacketNames, szRQ;
			m_pIChartManager->GetSubGraphViewData_TitleAndPacketNameAndRQString( psnViewData, szTitle, szPacketNames, szRQ);
			strTitle = szTitle;
			strPacketNames = szPacketNames;

			// 데이타를 컨테이너에 적재.
			strTitle.TrimRight(' ');
			if( strTitle.IsEmpty())
			{
				nNextPacketIndex = strPacketNames.Find( ',');
				if( nNextPacketIndex < 0)	strTitle = strPacketNames;
				else						strTitle = strPacketNames.Left( nNextPacketIndex);
			}

			if(bSetTitle == false)
			{
				SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);

				//110116alzioyes:툴팁 거래량에 등락추가.
				if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0 || strTitle == _MTEXT( C0_VOLUME))
					SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
				
				bSetTitle = true;
				m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
				continue;
			}
			SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);

			//110116alzioyes:툴팁 거래량에 등락추가.
			if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0 || strTitle == _MTEXT( C0_VOLUME))
				SetInquiryDataToContainer( CString( szRQ), strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
			
			m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
		}
		bSetTitle = false;

		if( bNoGraph)
		{
			while( !m_listInquiryData.IsEmpty())
			{
				plistData = m_listInquiryData.RemoveHead();
				while( !plistData->IsEmpty()) delete plistData->RemoveHead();
				delete plistData;
				plistData = NULL;
			}
			return FALSE;
		}

		// 가격차트에 마우스포인터가 위치해 있을 경우는 '수평대'값을 추가시켜준다.
		// 수치조회창이 일반윈도우타입일때만 실행한다.
		// (2006/2/18 - Seung-Won, Bae) Do not support the Horizontal Line in DongBu
		if( m_bWithHorizontalLine && nDataIndex != -1)
		{ 
			ML_SET_LANGUAGE_RES();
			
			CString strHorzTitle;
			strHorzTitle.LoadString(IDS_HORIZONTALZONE);
			CString strHorzData = _T("");	// 데이타

			int nR, nC, nV, nG;
			ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, szRQ);
			if( !szGraphName.IsEmpty() && nR == nRow && nC == nColumn)
			{
				IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
				if( pIBlock)
				{
					double dMin, dMax;
					if( pIBlock->GetVScaleGroupMinMax( nV, dMin, dMax))
					{
						POSITION psnViewData = m_pIChartManager->GetSubGraphViewData_First( NULL);
						if( psnViewData)
						{
							if( m_pIChartManager->GetSubGraphViewData_Index( psnViewData, nRow, nColumn))
							{
								CRect rectGraphRegion = m_pIChartManager->GetSubGraphViewData_GraphRegion( psnViewData);

								// (2008/2/26 - Seung-Won, Bae) Get invert type form Price Chart VScale
								BOOL bLog = pIBlock->IsLogScale();
								BOOL bReverse = pIBlock->IsVScaleInvert( nV);

								double dHorScale = m_pIChartOCX->ConvertYPositionToData( point.y, rectGraphRegion.top, rectGraphRegion.bottom, dMax, dMin, bLog, bReverse);

								IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, _MTEXT( C0_CLOSE));
								if( pPacket)
								{
									ILPCSTR szPacketType = pPacket->GetType();
									CString strPacketType( szPacketType);
									strHorzData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( CDataConversion::DoubleToString( dHorScale), strPacketType);
									pPacket->Release();
								}
								else
								{
									strHorzData = CDataConversion::GetStringThousandFormat(CDataConversion::DoubleToString(dHorScale));
								}

								if(strHorzTitle != _T("") && strHorzData != _T(""))
								{
									// 빈줄 하나 넣고 수평대 가격을 넣는다.
									
									plistData->AddTail( new CRowData( "", ""));
									plistData->AddTail( new CRowData( strHorzTitle, strHorzData));
								}
							}

							m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
						}
					}
					pIBlock->Release();
				}
			}
		}//end of if(nDataIndex != -1)
	}
	else
	{
		// 전체 ViewData를 가져옴
		POSITION psnViewData = NULL;
		psnViewData = m_pIChartManager->GetSubGraphViewData_First("");
		if( !psnViewData) return FALSE;

		int nRow, nColumn;
		if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		// 데이터 인덱스를 얻어올 수 있는지 확인
		if( !m_pIChartManager->GetSubGraphViewData_Index( psnViewData, 0, nColumn))
		{
			m_pIChartManager->GetSubGraphViewData_Close( psnViewData);
			return FALSE;
		}

		ILPCSTR szHorzScaleName = m_pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
		CString strHorzScaleName( szHorzScaleName);
		m_pIChartManager->GetSubGraphViewData_Close( psnViewData);

		// 컨테이너 초기화
		// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
		ClearInquiryDataList();

		// 실질적으로 데이타를 파싱하여 Container에 적재한다.
		BOOL bHorzScaleData = FALSE;
		bool bScaleData_Date = false;

		// 컨테이너 생성
		plistData = new CInquiryItem;
		plistPrevData = new CInquiryItem;

		// (2007/4/30 - Seung-Won, Bae) Skip InverseLine Horizontal Scale Info.
		int nDataIndex = 0;
		BOOL bInverseLine = ( strHorzScaleName == _MTEXT( C0_INVERSETIMELINE_VOLUME));
		// 스케일데이타가 '자료일자'일 경우는 1줄짜리 수치조회창에서는 필터링을 한다.
		if( strHorzScaleName == _MTEXT( C0_DATE_TIME)) bScaleData_Date = true;

		CString strRQFromAr;
		LPCTSTR lpcszCode = NULL;
		LPCTSTR lpcszName = NULL;
		bool bSetTitle = false;
		// 첫 데이터 얻기
		int nNextPacketIndex = -1;
		CString strTitle, strPacketNames, strSubGraphRQ;

		IStringList *pislRQs = m_pIPacketListManager->GetAllRQ();
		if( pislRQs)
		{
			pislRQs->AddHead( "DEFAULT");

			ILPCSTR szRQ = NULL;
			POSITION psn = pislRQs->GetHeadPosition();
			while( psn)
			{
				//2007.07.05 by LYH 자료일자 얻음(첫번째 RQ에 인덱스가 없으면 다음 찾음
				pislRQs->GetNext( psn, szRQ);
				strRQFromAr = szRQ;
				if( bInverseLine) nDataIndex = m_pIChartManager->GetDataIndexOfInverseLine( point);
				else nDataIndex = m_pIChartManager->FindViewDataIndex_Pt(strRQFromAr, point.x);
				if( nDataIndex < 0) continue;

				// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
				bHorzScaleData = SetHorzScaleDataToContainer(strRQFromAr, strHorzScaleName, nDataIndex, plistData, nRow, nColumn);
				SetHorzScaleDataToContainer(strRQFromAr, strHorzScaleName, max(0,nDataIndex-1), plistPrevData, nRow, nColumn);
				break;
			}

			psn = pislRQs->GetHeadPosition();
			while( psn)
			{
				pislRQs->GetNext( psn, szRQ);
				strRQFromAr = szRQ;

				psnViewData = m_pIChartManager->GetSubGraphViewData_First("");
				m_pIPacketListManager->GetItemCode( strRQFromAr, lpcszCode, lpcszName);

				nNextPacketIndex = -1;
				if( bHorzScaleData || bInverseLine) while( psnViewData)
				{
					ILPCSTR szTitle, szPacketNames, szGraphRQ;
					m_pIChartManager->GetSubGraphViewData_TitleAndPacketNameAndRQString( psnViewData, szTitle, szPacketNames, szGraphRQ);
					strTitle = szTitle;
					strPacketNames = szPacketNames;
					strSubGraphRQ = szGraphRQ;

					// 같은 RQ에 대해서만 툴팁 데이터를 모으기 위함
					if( strRQFromAr != szGraphRQ)
					{
						m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
						continue;
					}

					//2007.07.05 by LYH RQ별로 데이터 인덱스 따로 얻어옴
					if( bInverseLine) nDataIndex = m_pIChartManager->GetDataIndexOfInverseLine( point);
					else nDataIndex = m_pIChartManager->FindViewDataIndex_Pt( strRQFromAr, point.x);
				
					if(nDataIndex>0)
					{
						// 데이타를 컨테이너에 적재.
						strTitle.TrimRight(' ');
						if( strTitle.IsEmpty())
						{
							nNextPacketIndex = strPacketNames.Find( ',');
							if( nNextPacketIndex < 0)	strTitle = strPacketNames;
							else						strTitle = strPacketNames.Left( nNextPacketIndex);
						}

						if(bSetTitle == false)
						{
							SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);
							if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0)
								SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
							
							bSetTitle = true;
							m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
							continue;
						}
						SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, nDataIndex, plistData, bToolTip, bScaleData_Date, bSetTitle);
						if(strTitle == _MTEXT( C3_PRICE) || strPacketNames.Find(_MTEXT( C0_CLOSE)) >= 0)
							SetInquiryDataToContainer(strSubGraphRQ, strTitle, strPacketNames, max(0,nDataIndex-1), plistPrevData, bToolTip, bScaleData_Date);
					}
					m_pIChartManager->GetSubGraphViewData_NextInColumn( psnViewData);
				}
				bSetTitle = false;
			}

			pislRQs->Release();
		}
	}
 
	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( !bPairTick)
	{
		m_listInquiryData.AddTail( plistData);	
		m_listPrevInquiryData.AddTail( plistPrevData);
	}
	return TRUE;
}

CInquiryItem *CNumericalInquiryDataQueryer::GetHead( void)
{
	if( m_listInquiryData.IsEmpty()) return NULL;
	return m_listInquiryData.GetHead();
}

// 수치조회, 툴팁에 등락률 추가 표시 - ojtaso (20080710)
CInquiryItem *CNumericalInquiryDataQueryer::GetPrevHead( void)
{
	if( m_listPrevInquiryData.IsEmpty()) return NULL;
	return m_listPrevInquiryData.GetHead();
}

/*-------------------------------------------------------------------------------
 - Function    :  GetInquiryDataMaxLength
 - Created at  :  2005-03-31   09:50
 - Author      :  최종찬
 - Parameters  :  pListData               - [in] 수치조회데이타
 -                nFirstColumnMaxLength  - [out] 첫번째 칼럼의 최대길이.
 -                nSecondColumnMaxLength - [out] 두번째 칼럼의 최대길이.
 - Return Value:  bool - true: 함수호출성공, false: 함수호출실패.
 - Description :  수치조회툴팁을 문자열에 맞게 구성하기 위해 최대 문자열길이를 구한다.
 -------------------------------------------------------------------------------*/
BOOL CNumericalInquiryDataQueryer::GetInquiryDataMaxLength( CInquiryItem *pListData, int& nFirstColumnMaxLength, int& nSecondColumnMaxLength)
{
	CRowData *pRowData = NULL;
	POSITION posRow = pListData->GetHeadPosition();
	while( posRow)
	{
		pRowData = pListData->GetNext( posRow);
		
		if(nFirstColumnMaxLength < pRowData->m_strName.GetLength())
			nFirstColumnMaxLength = pRowData->m_strName.GetLength();
		
		if(nSecondColumnMaxLength < pRowData->m_strData.GetLength())
			nSecondColumnMaxLength = pRowData->m_strData.GetLength();
	}
	return TRUE;
}



//////////////////////////////////////////////////////////////////////
// (2006/6/24 - Seung-Won, Bae) Library Functions
//////////////////////////////////////////////////////////////////////

/*-------------------------------------------------------------------------------
 - Function    :  FindInquiryDataIndex
 - Created at  :  2005-03-29   10:29
 - Author      :  최종찬
 - Parameters  :  point        - 마우스 포인트
 -                pRowViewData - 해당 RowViewData의 그래프영역을 구하기 위함.
 - Return Value:  UINT 마우스가 위치한 곳의 데이타 인덱스.
 - Description :  현재 마우스가 위치한 곳의 데이타인덱스를 구해온다. 
				  ※구해올 참조식
				  → 마우스위치한곳의 데이타갯수(인덱스) : 한페이지의 데이타갯수 = 마우스.x : 그래프의 가로영역
 -------------------------------------------------------------------------------*/
UINT CNumericalInquiryDataQueryer::FindInquiryDataIndex( CPoint p_ptPoint, POSITION p_psnViewData)
{
// (2006/7/10 - Seung-Won, Bae) Use new Logic for Data Index of Mouse Point (TimeMarker version. BaeSeungWon.)
/*
	// 0. 참조식에 따라서 세가지를 구해야 한다. 한페이지의 데이타갯수, 마우스포인트의 X값, 
	//	  그리고 그래프의 가로영역이다.

	// 0.1. 한페이지의 데이타 갯수를 구해온다.
	int nStartIndex, nEndIndex;
	if( !m_pIChartManager->GetDataIndexRangeInView( nStartIndex, nEndIndex)) return -1;
	int nPageDataCount = nEndIndex - nStartIndex + 1;
	if( nPageDataCount <= 0) return -1;

	// 0.2. 마우스 포인트의 X값. 
	// --> 함수파라미터로 이미 제공!

	// 0.3. RowViewData의 그래프영역의 Width값을 가져오기 위해 그래프영역값 구해온다.
	CRect rectGraphRegion;
	m_pIChartManager->GetSubGraphViewData_GraphRegion( p_psnViewData, rectGraphRegion);
	//sy 2005.11.01.
	// graph region 왼쪽에 있을 경우 -> 왼쪽의 첫data
	// graph region 오른쪽에 있을 경우 -> 오른쪽의 첫data
	if(rectGraphRegion.left >= point.x)
		return nStartIndex;
	else if(rectGraphRegion.right <= point.x)
		return GetIndexInRightMargin( nEndIndex);
	//sy end
	
	// Index구해오기. 함수주석의 참조식 참조.
	int nIndex = int((double)((point.x - rectGraphRegion.left) * nPageDataCount) / (double)rectGraphRegion.Width());
	// 요상한넘이 나오면 에러처리해주는 쎈쓰!
	if(nIndex < 0 || nIndex > nEndIndex)
		return -1;

	//sy 2005.11.01.
	// nIndex는 단순히 현재 보이는 그래프영역에서의 인덱스이므로 
	// 실제 데이타에서의 인덱스는 Start Index를 더해줘야 한다.
	return GetIndexInRightMargin( nStartIndex + nIndex);
*/

	// 1.1 주어진 Point에 해당하는 Block의 Graph Region을 구한다.
	// (2004.06.14, 배승원) Block 영역의 Height에, Graph의 Width 영역을 Input 영역으로 확인한다.
	CRect rctGraphRegion = m_pIChartManager->GetSubGraphViewData_GraphRegion( p_psnViewData);

	// 1.2 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return -1;

	//if( p_ptPoint.x <= rctGraphRegion.left || rctGraphRegion.right <= p_ptPoint.x) return -1;
	if( p_ptPoint.x <= rctGraphRegion.left) return nStartIndex;
	if(rctGraphRegion.right <= p_ptPoint.x) return nEndIndex;	

	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
	return nStartIndex + ( ( p_ptPoint.x - rctGraphRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctGraphRegion.Width();
}

// (2006/7/10 - Seung-Won, Bae) Use new Logic for Data Index of Mouse Point (TimeMarker version. BaeSeungWon.)
/*
//sy 2005.11.01.
int CNumericalInquiryDataQueryer::GetIndexInRightMargin(const int nIndex) const
{
	if(nIndex <= 0)
		return nIndex;

	// 오른쪽 여백이 있을 경우
	int nRightMargin = 0;
	nRightMargin = m_pIChartOCX->GetBongCountMargin();
	if(nRightMargin <= 0)
		return nIndex;

	int nMaxRange;
	if( !m_pIChartOCX->GetMaxScrollRange( nMaxRange)) nMaxRange = 0;
	int nIndexMax = nMaxRange - nRightMargin;
	if(nIndex < nIndexMax)
		return nIndex;

	return (nIndexMax - 1);
}
//sy end
*/

/*-------------------------------------------------------------------------------
 - Function    :  SetHorzScaleDataToContainer
 - Created at  :  2005-03-24   14:53
 - Author      :  최종찬
 - Parameters  :  strHorzScaleName   - 스케일명.
 -                nDataIndex - 데이타 인덱스
 -				  listData - 
 - Return Value:  BOOL - TRUE: 성공, FALSE: 실패.
 - Description :  스케일데이타를 컨테이너에 적재한다.
 -------------------------------------------------------------------------------*/
// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
BOOL CNumericalInquiryDataQueryer::SetHorzScaleDataToContainer(CString szRQ, const CString& strHorzScaleName, int nDataIndex, CInquiryItem *listData, const int nRow, const int nColumn)
{
	IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strHorzScaleName);
	if( pPacket == NULL) return FALSE;

	POSITION pos = NULL;
	CString strScaleData = _T("");	// Scale Data.
	double dData = 0;

	ILPCSTR szPacketType = pPacket->GetType();
	CString strPacketType = szPacketType;
	
	// 2011.01.25 by SYS >> 분차트 YYYY(년) : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	int nHourType = 0;
	CScaleBaseData::HORZSCALEDRAWERTYPE eDateUnit = (CScaleBaseData::HORZSCALEDRAWERTYPE)pPacket->GetDateUnitType();
	if (strPacketType == _T("YYYYMMDDHHMMSS"))
	{
		if( CScaleBaseData::HORZ_TIME_ONE	== eDateUnit || 
			CScaleBaseData::HORZ_TIME_TWO	== eDateUnit ||
			CScaleBaseData::HORZ_TICK		== eDateUnit ||
			CScaleBaseData::HORZ_TIME_SECOND== eDateUnit)
		{
			if (strPacketType.Find("HH") >= 0)
				nHourType = 1;
		}
	}
	// 2011.01.25 by SYS <<
	
	ILPCSTR szScaleData;
	if( strPacketType == _MTEXT( C6_CHARACTER))
	{
		pPacket->GetData( nDataIndex, szScaleData);
		strScaleData = szScaleData;
	}
	else
	{
		dData = 0.0;
		if( !pPacket->GetData( nDataIndex, dData))
			pPacket->GetDataPastandFuture( nDataIndex, dData);
		if( dData == 0.0)
		{
			pPacket->Release();
			return FALSE;
		}

		if(strHorzScaleName.Compare(_MTEXT( C0_DATE_TIME)) != 0)
			strScaleData = CDataConversion::DoubleToString(dData);
		else 
		{
			// 수치조회창의 자료일자 Format, X Scale 설정값에 따라 처리 - ojtaso (20070827)
			ILPCSTR szCompartDate;
			ILPCSTR szCompartTime;
			m_pIChartManager->GetHorzScaleCompart( nRow, nColumn, szCompartDate, szCompartTime);
			CString strCompartDate( szCompartDate);
			CString strCompartTime( szCompartTime);
			
			//ILPCSTR szPacketType = pPacket->GetType();
			//CString strPacketType( szPacketType);
			
			CString strData = CDataConversion::DoubleToString(dData);

			// 2011.01.25 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
			if (strPacketType == _T("YYYYMMDDHHMMSS"))
			{
				// nDataUnit (0:nontype, 1:일, 2:주, 3:월, 4:분, 5:틱, 6:Text)
				CString strFormat = _T("YYYYMMDD");	// 디폴트포맷
				switch(eDateUnit)
				{
				case CScaleBaseData::HORZ_DAILY:		// 일
				case CScaleBaseData::HORZ_WEEKLEY:		// 주 
					strFormat = _T("YYYYMMDD");		
					break;
				case CScaleBaseData::HORZ_MONTHLY:		// 월 
					strFormat = _T("YYYYMM");		
					break;
				case CScaleBaseData::HORZ_TIME_ONE:		// 분
				case CScaleBaseData::HORZ_TIME_TWO:
					strFormat = _T("MMDDHHMM");
					break;
				case CScaleBaseData::HORZ_TICK:			// 틱 
				case CScaleBaseData::HORZ_TIME_SECOND:
					strFormat = _T("DDHHMMSS");
					break;
				}
				strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strFormat, strCompartDate, strCompartTime);
			}
			else
				strScaleData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans(strData, strPacketType, strCompartDate, strCompartTime);
			// 2011.01.25 by SYS <<
		}
	}

	CString strScaleName;
	if(strHorzScaleName.Compare(_MTEXT( C0_DATE_TIME)) != 0)
	{
		strScaleName = _MTOL0( strHorzScaleName);
	}
	else
	{
		ML_SET_LANGUAGE_RES();

		// 2011.01.25 by SYS >> 분차트 YYYY(년) : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
		// ILPCSTR szPacketType = pPacket->GetType();
		if (strPacketType == _T("YYYYMMDDHHMMSS"))
		{
			if (nHourType == 1)
				strScaleName.LoadString(IDS_TIME);
			else
				strScaleName.LoadString(IDS_DATE);
		}
		else
		{
			if( strPacketType.Find("HH") >= 0)	
				strScaleName.LoadString(IDS_TIME);
			else
				strScaleName.LoadString(IDS_DATE);
		}
		// 2011.01.25 by SYS <<
	}

	listData->AddTail( new CRowData( strScaleName, strScaleData));

	pPacket->Release();
	return TRUE;
}



/*-------------------------------------------------------------------------------
 - Function    :  SetInquiryDataToContainer
 - Created at  :  2005-03-24   14:22
 - Author      :  최종찬
 - Description :  조회데이타를 컨테이너에 삽입하는 실질적인 부분.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDataQueryer::SetInquiryDataToContainer(CString szRQ, const CString& strTitle, const CString& strPacketNames, int nDataIndex, CInquiryItem *listData, bool bToolTip, bool bScaleData_Date, bool bSetTitle)
{
	CString strRealPacketNames = GetRealPacketNames(strTitle, strPacketNames);
	int nPacketCount = CDataConversion::GetStringDataCount(strRealPacketNames, _T(","));

	CString strName, strData;
	if(nPacketCount == 1)
	{
		if(bSetTitle == false)
		{
			LPCTSTR lpcszCode = NULL;
			LPCTSTR lpcszName = NULL;
			m_pIPacketListManager->GetItemCode( szRQ, lpcszCode, lpcszName);

			if(lpcszName && strlen(lpcszName) > 0)
			{
				ML_SET_LANGUAGE_RES();

				CString strIssueName;
				strIssueName.LoadString(IDS_ISSUENAME);

				listData->AddTail( new CRowData( "", ""));
				listData->AddTail( new CRowData( strIssueName, lpcszName));
			}
		}

		strName = CDataConversion::GetStringData(strRealPacketNames, _T(","));
		IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strName);
		if( !strTitle.IsEmpty()) strName = strTitle;

		if( strName !=  _MTEXT( C2_OVERHANGING_SUPPLY) && pPacket && (pPacket->GetIndexOfRealDataStart() <= nDataIndex))
		{
			//////////////////////////////////////////////////////////////////////////
			CString strMetaText(strName);
			CString strUIText;
			if(strMetaText == _MTEXT( C0_DATE_TIME) || strMetaText == _MTEXT( C0_OPEN) || 
				strMetaText == _MTEXT( C0_HIGH) || strMetaText == _MTEXT( C0_LOW) ||
				strMetaText == _MTEXT( C0_CLOSE))
				strUIText = _MTOL0(strMetaText);
			else
			{
				strUIText = _MTOL1_N( strMetaText);
				if( strUIText.IsEmpty()) strUIText = _MTOL3(strMetaText);
			}
			strName = strUIText;
			//////////////////////////////////////////////////////////////////////////

			ILPCSTR szPacketType = pPacket->GetType();
			if( CString( szPacketType) == _MTEXT( C6_CHARACTER))
			{
				ILPCSTR szData;
				if( pPacket->GetData( nDataIndex, szData))
					listData->AddTail( new CRowData( strName, szData));
			}
			else
			{
				double dData = 0.0;
				if( pPacket->GetData( nDataIndex, dData))
				{
					ILPCSTR szPacketType = pPacket->GetType();
					CString strType = szPacketType;
					int nLength = strType.GetLength();
					int nDecimalPt =  strType.Find(".") + 1;
					int nDecimal;
				
					if(nDecimalPt == 0)
						nDecimal = 0;
					else
						nDecimal = nLength - nDecimalPt;

					// 최대 소수점 자릿수 기억해두기.(나중에 수치조회창 표현할때 소수점자릿수 맞추기위함)
					if(nDecimal > m_nMaxDecimal)
						m_nMaxDecimal = nDecimal;

					// 20080910 JS.Kim		8진법/32진법 처리
					//strData = CDataConversion::DoubleToString(dData, nDecimal);
					strData = CDataConversion::DoubleToString(dData);

					// 2011.01.20 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
					if (strType == _T("YYYYMMDDHHMMSS"))
					{
						int nDataUnit = pPacket->GetDateUnitType();
						strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans2( strData, nDataUnit);
					}
					else
						strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strType);
					// 2011.01.20 by SYS <<

					listData->AddTail( new CRowData( strName, strData));
				}
			}
		}//end of else...

		if( pPacket) pPacket->Release();

	}//end of if(nPacketCount...
	else
	{
		if(bSetTitle == false)
		{
			LPCTSTR lpcszCode = NULL;
			LPCTSTR lpcszName = NULL;
			m_pIPacketListManager->GetItemCode(szRQ, lpcszCode, lpcszName);

			if(lpcszName && strlen(lpcszName) > 0)
			{
				ML_SET_LANGUAGE_RES();

				CString strIssueName;
				strIssueName.LoadString(IDS_ISSUENAME);

				listData->AddTail( new CRowData( "", ""));
				listData->AddTail( new CRowData( strIssueName, lpcszName));
			}
		}

		while(strRealPacketNames.IsEmpty() == false)
		{
			strName = CDataConversion::GetStringData(strRealPacketNames, _T(","));

			IPacket* pPacket = m_pIPacketListManager->GetPacket(szRQ, strName);
			if( !pPacket) return;
			if( pPacket->GetIndexOfRealDataStart() <= nDataIndex)
			{
				// (2008/3/21 - Seung-Won, Bae) Check Dynamic Packet Name
				// 1. Rainbow Chart
				int nPrefixLen = 0;
				if( !strncmp( strName, _MTEXT( C0_RAINBOW_PACKET), nPrefixLen = strlen( _MTEXT( C0_RAINBOW_PACKET))))
					strName = _LTEXT0( C0_RAINBOW_PACKET) + strName.Mid( nPrefixLen);
				else
				// 2. Box Chart
				if( !strncmp( strName, _MTEXT( C2_BOX_CHART), nPrefixLen = strlen( _MTEXT( C2_BOX_CHART))))
						strName = CString( _LTEXT2( C2_BOX_CHART)) + "_" + _MTOL0( strName.Mid( nPrefixLen));
				else
				// 3. Others
					strName = _MTOL0( strName);

				ILPCSTR szPacketType = pPacket->GetType();
				if( CString( szPacketType) == _MTEXT( C6_CHARACTER))
				{
					ILPCSTR szData = NULL;
					if( pPacket->GetData( nDataIndex, szData))
						listData->AddTail( new CRowData( strName, szData));
				}
				else
				{
					double dData = 0.0;
					if( pPacket->GetData( nDataIndex, dData))
					{
						CString strPacketType = szPacketType;
						ILPCSTR szPacketType = pPacket->GetType();
						int nLength = strPacketType.GetLength();
						int nDecimalPt = strPacketType.Find(".") + 1;
						int nDecimal;
					
						if(nDecimalPt == 0)	nDecimal = 0;
						else				nDecimal = nLength - nDecimalPt;

						// 최대 소숫점자리수 기억해두기.(나중에 수치조회창 표현할때 소수점자릿수 맞추기위함)
						if(nDecimal > m_nMaxDecimal) m_nMaxDecimal = nDecimal;

						// 20080910 JS.Kim		8진법/32진법 처리
						//strData = CDataConversion::DoubleToString(dData, nDecimal);
						strData = CDataConversion::DoubleToString(dData);

						// 2011.01.20 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
						if (strPacketType == _T("YYYYMMDDHHMMSS"))
						{
							int nDataUnit = pPacket->GetDateUnitType();
							strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans2( strData, nDataUnit);
						}
						else
							strData = CDataConversion( m_hOcxWnd, m_eChartMode).GetStringFormatTrans( strData, strPacketType);
						// 2011.01.20 by SYS <<

						listData->AddTail( new CRowData( strName, strData));
					}
				}
			}

			pPacket->Release();
		}//end of while(...
	}//end of else
}

/*-------------------------------------------------------------------------------
 - Function    :  GetRealPacketNames
 - Created at  :  2005-03-24   14:08
 - Author      :  최종찬
 - Parameters  :  strTitle       - "가격"이고..
 -                strPacketNames - "시가,고가,저가,종가"인 경우...
 - Return Value:  CString - 새롭게 바꾼 패킷명.
 - Description :  1틱일경우는 검사하여 새로운 패킷명을 리턴해준다.
 -------------------------------------------------------------------------------*/
CString CNumericalInquiryDataQueryer::GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const
{
	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ",")
		strRealPacketNames += ",";

	// title = "가격", packetName = "시가,고가,저가,종가", "1틱"일 경우 
	// -> packetName : "종가"
	if(strTitle == _MTEXT( C3_PRICE) && strPacketNames.Find(_MTEXT( C0_CLOSE)) > 0)
	{
		if( m_pIDefaultPacketManager->GetBaseTimeDataType() == CScaleBaseData::HORZ_TICK && m_pIDefaultPacketManager->GetBaseTimeUnit() == 1)
		{
			strRealPacketNames = _MTEXT( C0_CLOSE);
			strRealPacketNames += ",";
		}
	}

	return strRealPacketNames;
}

// (2008/11/9 - Seung-Won, Bae) Support the Data Clear Routine
void CNumericalInquiryDataQueryer::ClearInquiryDataList( void)
{
	// (2006/6/24 - Seung-Won, Bae) Manage the Data with Group (?)
	CInquiryItem *plistData = NULL;
	while( !m_listInquiryData.IsEmpty())
	{
		plistData = m_listInquiryData.RemoveHead();
		while( !plistData->IsEmpty()) delete plistData->RemoveHead();
		delete plistData;
	}
	while( !m_listPrevInquiryData.IsEmpty())
	{
		plistData = m_listPrevInquiryData.RemoveHead();
		while( !plistData->IsEmpty()) delete plistData->RemoveHead();
		delete plistData;
	}
}
