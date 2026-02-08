 // PatternMaker.cpp : implementation file
//

#include "stdafx.h"
#include "PatternMaker.h"

#include "./Include_Chart/Dll_Load/IMetaTable.h"
#include "./Include_Chart/Conversion.h"			// for CDataConversion
#include "./Include_PCTR/RealUpdate.h"
#include "./Include_PCTR/IMasterDataMng2.h"
#include "../resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct __tagGData{
	char date[4];
	char time[4];
	char jmcode[6];
	char mmgb[1];
	char price[6];
}GData;

typedef struct __tagChartDate
{
	int	   nIndex;
	double dTime;
}ChartDate;

/////////////////////////////////////////////////////////////////////////////
// CPatternMaker
typedef LPCSTR (* FpGetUnitPrice2)(LPSTR, LPSTR);
FpGetUnitPrice2 pGetUnitPrice2 = NULL;

// (2009/9/12 - Seung-Won, Bae) for OHLC title
CString	CPatternMaker::m_strTitleO;
CString	CPatternMaker::m_strTitleH;
CString	CPatternMaker::m_strTitleL;
CString	CPatternMaker::m_strTitleC;

CPatternMaker::CPatternMaker()
{
	// (2009/9/12 - Seung-Won, Bae) for OHLC title
	if( m_strTitleO.IsEmpty())
	{
		m_strTitleO.LoadString( IDS_OHLC_TITLE_O);
		m_strTitleH.LoadString( IDS_OHLC_TITLE_H);
		m_strTitleL.LoadString( IDS_OHLC_TITLE_L);
		m_strTitleC.LoadString( IDS_OHLC_TITLE_C);
	}

	// (2008/10/26 - Seung-Won, Bae) Draw DO/DH/DL/DC
	m_bDrawOHLC = TRUE;
	m_clrO = RGB( 0, 128, 255);
	m_clrH = RGB( 255, 0, 0);
	m_clrL = RGB( 0, 0, 255);
	m_clrC = RGB( 255, 128, 0);

	m_strCode			= _T("");
	
	m_pPatternEnvData	= NULL;	// 패턴 환경설정 데이터
	
	m_bLog				= FALSE;	// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	m_bReverse			= FALSE;	// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	m_bSpecialChart		= FALSE;
	m_bDrawChegyulLine	= FALSE;

	m_dViewMin			= 0;		// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	m_dViewMax			= 0;		// View내에 대상 Graph된 View의 Maximum Scale 값이다.
		
	m_pnHighLowLinePen	.CreatePen( PS_SOLID,	2, RGB( 128, 0, 0));	// 주요고점 저점 연결라인 색상
	m_pnMaPen			.CreatePen(	PS_SOLID,	2, RGB( 255, 0, 0));	// 이동평균 색상
	m_pnHighLinePen		.CreatePen(	PS_SOLID,	1, RGB( 0, 0, 255));	// 주요고점 라인 색상
	m_pnLowLinePen		.CreatePen(	PS_SOLID,	1, RGB( 255, 0, 0));	// 주요저점 라인 색상
	
	Init_GuideLineValue();
//	CreateLinePen();	

	ReadOneLineForSamgungDemo();

	m_hwndChartItem = NULL;
	m_hSelChartItem = NULL;

	m_bWonChart = TRUE;

	//{{ 통신(소켓) 처리 모듈
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	if(pGateMng)
	{
		int nID = 2;
		m_pITrCommManager = (IAUTrCommManager*)pGateMng->GetInterface(nID);
		m_TRComm = m_pITrCommManager->AdviseTrComm();
		m_TRComm->ChangeCallback(&m_xTrCommSite);
	}
	//}}

	// (2008/9/9 - Seung-Won, Bae) Request MA Data
	m_nCodeGubun = 0;
}

CPatternMaker::~CPatternMaker()
{
	if(m_pITrCommManager && m_TRComm)
		m_pITrCommManager->UnAdviseTrComm(m_TRComm);

	// 라인 삭제
	DeleteLinePen();

	// 주요고점 저점 연결라인 색상
	m_pnHighLowLinePen.DeleteObject();

	// 이동평균 색상 
	m_pnMaPen.DeleteObject();

	// 주요고점 라인 색상
	m_pnHighLinePen.DeleteObject();

	// 주요저점 라인 색상
	m_pnLowLinePen.DeleteObject();


	// 주요고점, 주요저점 데이터 삭제
	DeleteHighLowPtData();

	// 이동평균 데이터 삭제
	DeleteMAData();

	// 저항, 지지선 데이터 삭제
	DeleteRegistNSupportData();

	// 체결 데이터 삭제
	DeleteCheChartData();

	m_strRQ = "DEFAULT";
}

// (2004.06.10, 배승원) Drawing 시점에 수직선을 출력한다.
void CPatternMaker::OnDraw( CDC *p_pDC)
{
	// 패턴 환경설정 데이터가 없으면 리턴
	//if ( !m_pPatternEnvData) return;

	// (2004.09.15, 배승원) 특수차트인 경우에는 보이지 않도록 한다. (역시계곡선/삼선전환도/P&F)
	if( m_pIChartOCX->IsOnSpecialChart())
	{
		m_bSpecialChart		= TRUE;
		return;
	}
	else
		m_bSpecialChart		= FALSE;

	// Drawing이 가능한 상황인지 확인한다.
	if( !m_pIChartManager || !m_pIChartOCX || !m_pIPacketManager) return;

	// (2008/11/3 - Seung-Won, Bae) Do not support the Buy/Sell Data.
	if( !m_dTEprice) return;

	// (2004.07.05, 배승원) Time Mark의 값이 다른 종목에서도 유효하도록, 
	//		Packet의 Type을 관리하여, 변경된 경우가 아니면 계속 Draw하도록 한다.
	//IPacket *pIPacket = m_pIPacketManager->GetPacket( "자료일자");
	// IPacket *pIPacket = m_pIPacketManager->GetPacket( m_strRQ, "자료일자");
	IPacket *pIPacket = m_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_DATE_TIME));

	if( !pIPacket) return;

	try
	{
		// 차트의 기본정보 --------------------------------------------------------------------->>
		// 현 Data View의 Start/End Index를 구한다.
		int nStartIndex, nEndIndex;
		//if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) throw this;
		if( !m_pIChartManager->GetStartEndDrawIdx_Cur(m_strRQ, nStartIndex, nEndIndex)) throw this;

		// 가격차트가 포함된 Block의 Index를 구한다.
		int nR, nC, nV, nG;
		CRect		rctBlockRegion, rctGraphRegion;	
		ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, m_strRQ);
		if( szGraphName.IsEmpty()) return;
		IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
		if( pIBlock)
		{
			// 2.2.1 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
			m_bLog = pIBlock->IsLogScale();
			m_bReverse = pIBlock->IsVScaleInvert( nV);
			// 2.2.0 View의 Min/Max 값을 구한다.
			pIBlock->GetVScaleGroupMinMax(nV, m_dViewMin, m_dViewMax);
	   		pIBlock->GetGraphRegions(rctGraphRegion, rctBlockRegion );
			rctGraphRegion = pIBlock->GetGraphDrwingRegion();		
			pIBlock->Release();
			pIBlock = NULL;
		}
/*		if( !m_pIChartManager->GetBlockIndexOfGraph( "가격차트", nR, nC)) throw this;
		
		// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
		m_pIChartManager->GetVerticalScaleConversionType( nR, nC, m_bLog, m_bReverse);

		// View의 Min/Max 값을 구한다.
		m_pIChartManager->GetMinMaxDataInViewWithGraph( "가격차트", m_dViewMin, m_dViewMax);

		rctBlockRegion = m_pIChartManager->GetNoFrameGraphRegion( nR, nC);		// Frame 영역
		rctGraphRegion = m_pIChartManager->GetDrawingGraphRegion( nR, nC);		// Graph 영역
*/				
		int		nDataOnPage = nEndIndex - nStartIndex + 1;	// 현 Data View의 Data 갯수
		double	dCandleWidth = double( rctGraphRegion.Width()) / double( nDataOnPage);	// 봉의 폭을 구한다
		// <<--------------------------------------------------------------------------------------
		CPen *pOldPen = NULL; 
		int nYHigh = 0; 

		//#########################################################################################
		//체결 기준선 및 체결시점에 표시한다.
		//#########################################################################################
		if(m_bDrawChegyulLine)
		{
//{{ test 컴파일 에러 = 막음
			CString strPacketType="YYYYMMDD";   
//			pIPacket->GetPacketTypeString( strPacketType);
//			OutputDebugString(strPacketType);
//	}}

			//CList<double,double> *clistDate;
			//clistDate = NULL;
			//clistDate = pIPacket->GetDataList();

			//int			nDataSequence = clistDate->GetCount();
			int			nDataSequence = pIPacket->GetDataCount();
			// 일봉/주봉/월봉 인 경우 ====================================
			OutputDebugString("체결 타입 " + strPacketType);
			if( strPacketType.Left(4) == "YYYY")
			{
				

//				double dValue[] = { 999.00,
//					842.00,
//					564.00,
//					653.00,
//					765.00
//				};

				
				int nDataCnt= 0;
				int nXCrossPt = 0;
				CString sTemp, sDate, sCode, sMMGb, sPrice;
				int nSize  = m_ArrayFileChart.GetSize();
				//POSITION posDate = clistDate->GetTailPosition();
				for(int i=0;i<nSize;i++)
				{
					sTemp = m_ArrayFileChart.GetAt(i);
					if(sTemp.GetLength())
					{
						OutputDebugString(sTemp);
						GData *pData = (GData*)(LPTSTR)(LPCTSTR)sTemp;

						//종목코드 비교 
						sCode.Format("%6.6s", pData->jmcode);
						if(sCode != m_strCode) continue;
						
						

						//날짜를 구한다.
						sDate = sTemp.Left(4);
						double dReadDate = atof(sDate);

					
						CString sDate;
						double dDate;
						pIPacket->GetData(i, dDate);
						sDate.Format("%f", dDate);
						double dLastDate = atof(sDate.Mid(4,4));
						if(dReadDate == dLastDate )
						{
							nDataCnt++;
						
							if(nDataCnt == 1)//일봉은 한번만 그린다.
							{
								nDataSequence--;
								pOldPen = p_pDC->SelectObject(&m_penChegyul);
								// 시간 기준선을 위에서 아래로 그린다.
								double dDataOffset = nDataSequence - nStartIndex;
								 nXCrossPt = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
								
								p_pDC->MoveTo( nXCrossPt, rctGraphRegion.top);
								p_pDC->LineTo( nXCrossPt, rctGraphRegion.bottom);

								p_pDC->SelectObject(pOldPen);			
								pOldPen = NULL;
							}

							//체결 지점을 구한다.
							sPrice.Format("%6.6s", pData->price);
							double dValue = atof(sPrice);

							nYHigh = m_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
							rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

							sMMGb.Format("%1.1s", pData->mmgb);
							if(sMMGb == "2")
							{
								pOldPen = p_pDC->SelectObject(&m_penSell);
							}
							else if(sMMGb == "1")
							{
								pOldPen = p_pDC->SelectObject(&m_penBuy);
							}

							p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
							p_pDC->LineTo( nXCrossPt + 5, nYHigh);

							p_pDC->SelectObject(pOldPen);			
							pOldPen = NULL;
						}
					}
				}

			
				// 체결라인 그린다. -----------------------------------------------------------------(2-1)
				/*for(int i = 0; i < 5; i++)
				{					
					if((i == 0) ||
					   (i == 1) ||
					   (i == 4) )
					{
						pOldPen = p_pDC->SelectObject(&m_penSell);
					}
					else if((i == 3) ||
					   (i == 2) )
					{
						pOldPen = p_pDC->SelectObject(&m_penBuy);
					}
					
					int nTick = 0;
					if( m_dViewMax > 100000)
						nTick = 5000;
					else if( m_dViewMax > 10000)
						nTick = 500;
					else if( m_dViewMax > 1000)
						nTick = 50;

					double dValue = m_dTHprice - (nTick * i);
					CString sTempTT;
					sTempTT.Format("체결 단가 %f", dValue);
					OutputDebugString(sTempTT);
					// Y좌표를 구한다					
					nYHigh = p_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
						rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
								
					//체결 지점을 그린다.
					if(dValue > m_dTLprice)
					{
						p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
						p_pDC->LineTo( nXCrossPt + 5, nYHigh);

									
					}
					p_pDC->SelectObject(pOldPen);
					pOldPen = NULL;
				}*/
			}
			else if( (strPacketType.Left( 4) == "MMDD") ||//틱 단위 / 분봉인 경우
					 (strPacketType.Left( 4) == "HHMM") ) 
			{
				//################################################################################
				//##삼성 시연용
				IPacket *pIPacketHigh, *pIPacketLow;
				pIPacketHigh = pIPacketLow= NULL;		
//				pIPacketHigh	= m_pIPacketManager->GetPacket(m_strRQ, "고가");
				pIPacketHigh	= m_pIPacketManager->GetPacket(m_strRQ, _MTEXT( C0_HIGH));
// 				pIPacketLow		= m_pIPacketManager->GetPacket(m_strRQ, "저가");
 				pIPacketLow		= m_pIPacketManager->GetPacket(m_strRQ, _MTEXT( C0_LOW));
				if ( !pIPacketHigh || !pIPacketLow ) return;

				//CList<double, double>  *clistHigh, *clistLow;			
				//clistHigh = clistLow = NULL;
					
				//clistHigh	= pIPacketHigh->GetDataList();
				//clistLow	= pIPacketLow->GetDataList();			
				

				int nTotalDataCnt	= pIPacketHigh->GetDataCount();

				double *pdDate		= new double[nTotalDataCnt];	// 시간
				double *pdHigh		= new double[nTotalDataCnt];	// 고가
				double *pdLow		= new double[nTotalDataCnt];	// 저가				

				for(int nNum=0; nNum< nTotalDataCnt; nNum++)
				{
					pIPacket->GetData(nNum, pdDate[nNum]);
					pIPacketHigh->GetData(nNum, pdHigh[nNum]);
					pIPacketLow->GetData(nNum, pdLow[nNum]);
				}
//				POSITION	posDate, posHigh, posLow;
//				
//				for(posDate = clistDate->GetHeadPosition(),
//					posHigh  = clistHigh->GetHeadPosition(),
//					posLow  = clistLow->GetHeadPosition(); 
//					posDate && posHigh && posLow;)
//				{
//					if (posDate == NULL)	continue;
//					
//					pdDate[nNum]	= (double)clistDate->GetNext(posDate);
//					pdHigh[nNum]	= (double)clistHigh->GetNext(posHigh);
//					pdLow[nNum]		= (double)clistLow->GetNext(posLow);
//					nNum++;
//				}			
				
//				if( (m_strCode == "016360") ||
//					(m_strCode == "006400")
//				)
//				{	
					
					CString sTemp, sDate, sCode, sMMGb, sPrice;
					int nSize  = m_ArrayFileChart.GetSize();
					for(int i=0;i<nSize;i++)
					{
						sTemp = m_ArrayFileChart.GetAt(i);
						if(sTemp.GetLength())
						{
							OutputDebugString(sTemp);
							GData *pData = (GData*)(LPTSTR)(LPCTSTR)sTemp;

							//종목코드 비교 
							sCode.Format("%6.6s", pData->jmcode);
							if(sCode != m_strCode) continue;								
							
							//날짜.
							sDate = sTemp.Left(8);
							double dReadDate = atof(sDate);
							int nNewDataSequence = 0;
							nNewDataSequence = nDataSequence;
							while(nNewDataSequence)
							{	
								nNewDataSequence--;
								if(dReadDate > pdDate[nNewDataSequence])
								{
									//상하 기준선 
									pOldPen = p_pDC->SelectObject(&m_penChegyul);

									double dDataOffset = nNewDataSequence - nStartIndex;				
									int nXCrossPt = GetXPositoin( dDataOffset + 1, dCandleWidth, rctGraphRegion);
									if(nXCrossPt < rctGraphRegion.right)
									{
										p_pDC->MoveTo( nXCrossPt, rctGraphRegion.top);
										p_pDC->LineTo( nXCrossPt, rctGraphRegion.bottom);
									}

									p_pDC->SelectObject(pOldPen);			
									pOldPen = NULL;
									
									//체결 지점을 구한다.
									sPrice.Format("%6.6s", pData->price);
									double dValue = atof(sPrice);
									nYHigh = m_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
									rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);

									sMMGb.Format("%1.1s", pData->mmgb);
									if(sMMGb == "2")
									{
										pOldPen = p_pDC->SelectObject(&m_penSell);
									}
									else if(sMMGb == "1")
									{
										pOldPen = p_pDC->SelectObject(&m_penBuy);
									}

									p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
									p_pDC->LineTo( nXCrossPt + 5, nYHigh);

									p_pDC->SelectObject(pOldPen);			
									pOldPen = NULL;
									
									break;
								}
							}
						}							
					}
					
				//}
				/*
				else
				{
					//################################################################################
					//체결 기준선을 그린다.===================================================
						
					int nIndexArray[30] = { 0,0,1,0,1,0,0,1,0,1,
											0,1,1,0,0,1,0,0,1,0,
											0,0,1,0,1,0,0,1,0,0};

					
					for(int i = 0; i < 30 ; i++)
					{   	
						nDataSequence--;	
						// 시간 기준선을 위에서 아래로 그린다.======================================
						
										
						double dDataOffset = nDataSequence - nStartIndex;				
						int nXCrossPt = GetXPositoin( dDataOffset, dCandleWidth, rctGraphRegion);
						
						if(nIndexArray[i] == 0)
							continue;

						double dHigh = 0;
						double dLow	 = 0;	

						dHigh = pdHigh[nNum - i - 1];
						dLow  = pdLow [nNum - i - 1];	

						pOldPen = p_pDC->SelectObject(&m_penChegyul);	
						
						CString sTempTT2;						
						sTempTT2.Format("고가 : %f, 저가 : %f 체결 단가 %f ", dHigh, dLow);
						OutputDebugString(sTempTT2);
						
						p_pDC->MoveTo( nXCrossPt, rctGraphRegion.top);
						p_pDC->LineTo( nXCrossPt, rctGraphRegion.bottom);						
						
						p_pDC->SelectObject(pOldPen);			
						pOldPen = NULL;

						//체결 기준선을 그린다.=====================================================

						// 체결라인 그린다. -----------------------------------------------------------------(2-1)
						for(int j = 0; j < 5; j++)
						{					
							if((j == 0) ||
							   (j == 1) ||
							   (j == 4) )
							{
								pOldPen = p_pDC->SelectObject(&m_penSell);
							}
							else if((j == 3) ||
							   (j == 2) )
							{
								pOldPen = p_pDC->SelectObject(&m_penBuy);
							}
							
							int nTick = 0;
							if( m_dViewMax > 100000)
								nTick = 500;
							else if( m_dViewMax > 10000)
								nTick = 100;
							else if( m_dViewMax > 1000)
								nTick = 10;

									

							double dValue = dHigh - (nTick * j);
							CString sTempTT;						
							sTempTT.Format("고가 : %f, 저가 : %f 체결 단가 %f ", dHigh, dLow, dValue);
							OutputDebugString(sTempTT);
							// Y좌표를 구한다					
							nYHigh = p_pIChartOCX->ConvertDataToYPosition(dValue, m_dViewMax, m_dViewMin, 
								rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
										
							//체결 지점을 그린다.
							if( (dValue > dLow) && (dHigh != dLow))
							{
								p_pDC->MoveTo( nXCrossPt - 5, nYHigh);
								p_pDC->LineTo( nXCrossPt + 5, nYHigh);										
							}
							p_pDC->SelectObject(pOldPen);
							pOldPen = NULL;
						}
					} 
				}
//====================================================================================================*/		
				delete pdDate;	// 날짜 
				delete pdHigh;	// 고가
				delete pdLow;	// 저가
				//clistChartDate
			}
		}

		// (2008/10/26 - Seung-Won, Bae) Draw DO/DH/DL/DC
		if( m_bDrawOHLC)
		{
			// (2008/11/4 - Seung-Won, Bae) Check Graph area hight.
			CSize sizHight = p_pDC->GetTextExtent( "시\r\n고\r\n저\r\n종");
			if( sizHight.cx < rctBlockRegion.Height())
			{
				IPacket *pIPacketH = m_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_HIGH));
				IPacket *pIPacketL = m_pIPacketManager->GetPacket( m_strRQ, _MTEXT( C0_LOW));
				if( pIPacketH && pIPacketL)
				{
					ILPCSTR szType = pIPacketH->GetType();

					CString strO, strH, strL, strC;
					CDataConversion dataCNV( m_pIChartOCX->GetOcxHwnd(), m_pIChartOCX->GetChartMode());
					if( 0 < m_dTSprice)
					{
						strO = dataCNV.DoubleToString( m_dTSprice);
						strO = dataCNV.GetStringFormatTrans( strO, CString( szType));
					}
					if( 0 < m_dTHprice)
					{
						strH = dataCNV.DoubleToString( m_dTHprice);
						strH = dataCNV.GetStringFormatTrans( strH, CString( szType));
					}
					if( 0 < m_dTLprice)
					{
						strL = dataCNV.DoubleToString( m_dTLprice);
						strL = dataCNV.GetStringFormatTrans( strL, CString( szType));
					}
					if( 0 < m_dTEprice)
					{
						strC = dataCNV.DoubleToString( m_dTEprice);
						strC = dataCNV.GetStringFormatTrans( strC, CString( szType));
					}

					int nOLen = strO.GetLength();
					int nHLen = strH.GetLength();
					int nLLen = strL.GetLength();
					int nCLen = strC.GetLength();

					int nMaxLen = nOLen;
					if( nMaxLen < nHLen) nMaxLen = nHLen;
					if( nMaxLen < nLLen) nMaxLen = nLLen;
					if( nMaxLen < nCLen) nMaxLen = nCLen;

					strO = m_strTitleO + CString( ' ', nMaxLen - nOLen + 1) + strO;
					strH = m_strTitleH + CString( ' ', nMaxLen - nHLen + 1) + strH;
					strL = m_strTitleL + CString( ' ', nMaxLen - nLLen + 1) + strL;
					strC = m_strTitleC + CString( ' ', nMaxLen - nCLen + 1) + strC;

					// 1. Check Last data of view to check drawing side.
					double dHigh = 0, dLow = 0;
					pIPacketH->GetData( nEndIndex - 1, dHigh);	// -1 for Virtual Candle.
					pIPacketL->GetData( nEndIndex - 1, dLow);
					double dCenter = ( dHigh + dLow) / 2;
					double dViewCenter = ( m_dViewMax + m_dViewMin) / 2;
					BOOL bUpside = ( dCenter < dViewCenter);

					// 2. Set Base Position
					CPoint ptRightTop( rctGraphRegion.right, rctGraphRegion.top + 1);
					if( !bUpside)
						ptRightTop.y = rctGraphRegion.bottom - p_pDC->GetTextExtent("0").cy * 4;

					// 3. Draw Text
					p_pDC->SetBkMode(TRANSPARENT);
					CSize size = p_pDC->GetTextExtent( strO);
					// 20081106 이주표 >>
					if(m_dYEprice > m_dTSprice)
						m_clrO = m_clrL;
					else
						m_clrO = m_clrH;
					// 20081106 이주표 <<
					p_pDC->SetTextColor( m_clrO);
					p_pDC->TextOut( ptRightTop.x - size.cx, ptRightTop.y, strO);
					ptRightTop.y += size.cy;
					// 20081106 이주표 >>
					if(m_dYEprice > m_dTHprice)
						m_clrH = m_clrL;
					else
						m_clrH = m_clrH;
					// 20081106 이주표 <<
					p_pDC->SetTextColor( m_clrH);
					p_pDC->TextOut( ptRightTop.x - size.cx, ptRightTop.y, strH);
					ptRightTop.y += size.cy;
					// 20081106 이주표 >>
					if(m_dYEprice > m_dTLprice)
						m_clrL = m_clrL;
					else
						m_clrL = m_clrH;
					// 20081106 이주표 <<
					p_pDC->SetTextColor( m_clrL);
					p_pDC->TextOut( ptRightTop.x - size.cx, ptRightTop.y, strL);
					ptRightTop.y += size.cy;
					// 20081106 이주표 >>
					if(m_dYEprice > m_dTEprice)
						m_clrC = m_clrL;
					else
						m_clrC = m_clrH;
					// 20081106 이주표 <<
					p_pDC->SetTextColor( m_clrC);
					p_pDC->TextOut( ptRightTop.x - size.cx, ptRightTop.y, strC);
					ptRightTop.y += size.cy;
				}
				if( pIPacketH) pIPacketH->Release();
				if( pIPacketL) pIPacketL->Release();
			}
		}

		if( (g_clConfigSet.m_bNotDrawLine) || !m_bWonChart)
		{
			pIPacket->Release();
			return;
		}

//전일 데이타 시작 ======================================================================================

		if(g_clConfigSet.m_bChkYSprice) //전일 시가
		{	
			OutputDebugString("전일시가 그린다");
			CString sTemp, strBeforeOpenPrice;
			strBeforeOpenPrice.LoadString(IDS_BEFOREOPENPRICE);
			// sTemp.Format("전일시가 HWND %d", m_hwndChartItem);
			sTemp.Format("%s HWND %d", strBeforeOpenPrice, m_hwndChartItem);
			OutputDebugString(sTemp);

			// sTemp.Format("전일시가 R:%d, G:%d, B:%d",	GetRValue(g_clConfigSet.m_clrYSprice),
			sTemp.Format("%s R:%d, G:%d, B:%d",		strBeforeOpenPrice,
													GetRValue(g_clConfigSet.m_clrYSprice),
													GetGValue(g_clConfigSet.m_clrYSprice),
													GetBValue(g_clConfigSet.m_clrYSprice));
			OutputDebugString(sTemp);
			

			nYHigh = 0;
			pOldPen = p_pDC->SelectObject(&m_penYSprice);			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dYSprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}


		if(g_clConfigSet.m_bChkYHprice) //전일 고가
		{	
			OutputDebugString("전일고가 그린다");
			nYHigh = 0;					
			pOldPen = p_pDC->SelectObject(&m_penYHprice);
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dYHprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkYLprice) //전일 저가
		{	
			OutputDebugString("전일저가 그린다");
			nYHigh = 0;			
			pOldPen = p_pDC->SelectObject(&m_penYLprice);
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dYLprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
		
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkYEprice) //전일 종가
		{	
			OutputDebugString("전일종가 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penYEprice);
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dYEprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}	
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}


		if(g_clConfigSet.m_bChkYSHL) //전일 (시 + 고 + 저 ) /2
		{	
			OutputDebugString("전일(시 + 고 + 저 ) /2 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penYSHL);
			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dYSHL, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
	
//전일 데이타 종료 ======================================================================================
		
//당일 데이타 시작 ======================================================================================

		if(g_clConfigSet.m_bChkTSprice) //당일 시가
		{	
			OutputDebugString("당일시가 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penTSprice);

			
		
				// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTSprice, m_dViewMax, m_dViewMin, 
								rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
		
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTHprice) //당일 고가
		{	
			OutputDebugString("당일고가 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penTHprice);
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTHprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTLprice) //당일 저가
		{	
			OutputDebugString("당일저가 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penTLprice);
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTLprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
		
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTEprice) //당일 종가
		{	
			OutputDebugString("당일종가 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penTEprice);
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTEprice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}	
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkTHL) //당일 (고 + 저 ) /2
		{	
			OutputDebugString("당일(고 + 저 ) /2 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penTHL);
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTHL, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		
		if(g_clConfigSet.m_bChkTSHL) //당일 (시 + 고 + 저 ) /2
		{	
			OutputDebugString("당일(시 + 고 + 저 ) /2 그린다");
			nYHigh = 0;
			pOldPen = p_pDC->SelectObject(&m_penTSHL);
			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTSHL, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		
		if(g_clConfigSet.m_bChkTHLE) //당일 (시 + 고 + 저 ) /4
		{	
			OutputDebugString("당일 {고 + 저  + (종 * 2 )}/4 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penTHLE);
			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dTHLE, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		
		if(g_clConfigSet.m_bChkHighLimit) //상한가
		{	
			OutputDebugString("상한가 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penHighLimit);
			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dHighLimit, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}


		if(g_clConfigSet.m_bChkLowLimit) //하한가
		{	
			OutputDebugString("하한가 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penLowLimit);
			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dLowLimit, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		
//당일 데이타 종료 ======================================================================================

//PIVOT 시작  ======================================================================================
		

		//피봇 2차 저항 
		if(g_clConfigSet.m_bChkPivot2Rst) //피봇 2차 저항 
		{	
			OutputDebugString("피봇 2차 저항선 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivot2Rst);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPivot2Rst, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//피봇 1차 저항 
		if(g_clConfigSet.m_bChkPivot1Rst) //피봇 1차 저항 
		{	
			OutputDebugString("피봇 1차 저항선 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivot1Rst);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPivot1Rst, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//Pivot 기준선 
		if(g_clConfigSet.m_bChkPivotGLine) //피봇 기준선
		{	
			OutputDebugString("피봇 기준선 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivotGLine);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPivotGLine, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//피봇 1차 지지
		if(g_clConfigSet.m_bChkPivot1GG) //피봇 1차 지지
		{	
			OutputDebugString("피봇 1차 지지선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPivot1GG);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPivot1GG, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		//피봇 2차 지지
		if(g_clConfigSet.m_bChkPivot2GG) //피봇 2차 지지
		{	
			OutputDebugString("피봇 2차 지지선 그린다");
			nYHigh = 0;		
			pOldPen = p_pDC->SelectObject(&m_penPivot2GG);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPivot2GG, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
//PIVOT 종료  ======================================================================================

//Demark 시작  ======================================================================================
		if(g_clConfigSet.m_bChkDMKPreHPrice) //Demark 예상고가
		{	
			OutputDebugString("Demark 예상고가선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penDMKPreHPrice);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dDMKPreHPrice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkDMKGLine) //Demark 기준선
		{	
			OutputDebugString("Demark 기준선 그린다");
			nYHigh = 0;
			pOldPen = p_pDC->SelectObject(&m_penDMKGLine);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dDMKGLine, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(g_clConfigSet.m_bChkDMKPreLPrice) //Demark 예상저가
		{	
			OutputDebugString("Demark 예상저가선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penDMKPreLPrice);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dDMKPreLPrice, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
//Demark 종료  ======================================================================================

//이동평균 시작  ======================================================================================

		if(m_dMAPeriod1 > 0 && g_clConfigSet.m_bChk2Pyung1) //이동평균 1번째 항목
		{	
			OutputDebugString(m_strCode + "  이동평균 1번째 항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung1);		
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod1, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" 이평 1값 : %f, 체크 : %d ", m_dMAPeriod1, g_clConfigSet.m_bChk2Pyung1);
			OutputDebugString( m_strCode + sTemp);
		}
		
		OutputDebugString("                  ");

		if(m_dMAPeriod2 > 0 && g_clConfigSet.m_bChk2Pyung2) //이동평균 2번째 항목
		{	
			OutputDebugString(m_strCode + "  이동평균 2번째 항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung2);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod2, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" 이평 2값 : %f, 체크 : %d ", m_dMAPeriod2, g_clConfigSet.m_bChk2Pyung2);
			OutputDebugString( m_strCode + sTemp);
		}

		OutputDebugString("                  ");

		if(m_dMAPeriod3 > 0 && g_clConfigSet.m_bChk2Pyung3) //이동평균 3번째 항목
		{	
			OutputDebugString(m_strCode + "  이동평균 3번째 항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung3);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod3, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" 이평 3값 : %f, 체크 : %d ", m_dMAPeriod3, g_clConfigSet.m_bChk2Pyung3);
			OutputDebugString( m_strCode + sTemp);
		}

	
		OutputDebugString("                  ");

		if(m_dMAPeriod4 > 0 && g_clConfigSet.m_bChk2Pyung4) //이동평균 4번째 항목
		{	
			OutputDebugString(m_strCode + "  이동평균 4번째 항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_pen2Pyung4);			
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dMAPeriod4, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}
		else
		{
			CString sTemp;
			sTemp.Format(" 이평 4값 : %f, 체크 : %d ", m_dMAPeriod4, g_clConfigSet.m_bChk2Pyung4);
			OutputDebugString( m_strCode + sTemp);
		}

		OutputDebugString("                  ");
		
//이동평균 종료  ======================================================================================


//평균매입단가 시작  ======================================================================================
		
		if(m_dPmmDanga > 0 && g_clConfigSet.m_bChkPmmDanga) //평균 매입단가.
		{	
			OutputDebugString("평균매입단가  항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPmmDanga);		
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPmmDanga, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(m_dPmmPDanga > 0 && g_clConfigSet.m_bChkPmmPDanga) //평균 매입단가 기준 +% 선
		{	
			OutputDebugString("평균 매입단가 기준 +% 항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPmmPDanga);		
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPmmPDanga, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}

		if(m_dPmmMDanga > 0 && g_clConfigSet.m_bChkPmmMDanga) //평균 매입단가.
		{	
			OutputDebugString("평균 매입단가 기준 -%  항목선 그린다");
			nYHigh = 0;	
			pOldPen = p_pDC->SelectObject(&m_penPmmMDanga);		
			
			
			// Y좌표를 구한다
			nYHigh = m_pIChartOCX->ConvertDataToYPosition(	m_dPmmMDanga, m_dViewMax, m_dViewMin, 
				rctGraphRegion.top, rctGraphRegion.bottom, m_bLog, m_bReverse);
			
			if ( nYHigh >= rctGraphRegion.top && nYHigh <= rctGraphRegion.bottom )
			{
				p_pDC->MoveTo( rctGraphRegion.left, nYHigh);
				p_pDC->LineTo( rctGraphRegion.right, nYHigh);	
			}
			
			p_pDC->SelectObject(pOldPen);			
			pOldPen = NULL;
		}	
	}
	catch( CPatternMaker *)
	{
		pIPacket->Release();
		return;
	}
	
	pIPacket->Release();
}

void CPatternMaker::DeleteHighLowPtData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryHighLowPtData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryHighLowPtData.GetAt(i);
		delete pObj;
	}

	m_aryHighLowPtData.RemoveAll();
}

void CPatternMaker::DeleteMAData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryMAData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryMAData.GetAt(i);
		delete pObj;
	}

	m_aryMAData.RemoveAll();
}

void CPatternMaker::DeleteRegistNSupportData()
{
	CObject *pObj=NULL;
	for(int i=0; i < m_aryRegistNSupportData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryRegistNSupportData.GetAt(i);
		delete pObj;
	}

	m_aryRegistNSupportData.RemoveAll();
}


int CPatternMaker::GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion)
{
	return m_pIChartManager->GetXPosition( m_strRQ, ( int)dDataOffset, 0);
}

double CPatternMaker::GetGradient(double dXPos1, double dYPos1, double dXPos2, double dYPos2)
{
	double dGradient=0;

	if( (dXPos2 - dXPos1) != 0)
	{
		dGradient = (dYPos2 - dYPos1) / (dXPos2 - dXPos1);
	}

	return dGradient;
}


void CPatternMaker::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	// 패턴환경설정 데이터
	if ( pPatternEnvData )
		m_pPatternEnvData = pPatternEnvData;
}


void CPatternMaker::CreateLinePen()
{
	//===========================================================================================
	m_penYSprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYSprice	, g_clConfigSet.m_clrYSprice		);//전일가격 시가				펜
	m_penYHprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYHprice	, g_clConfigSet.m_clrYHprice		);//전일가격 고가				펜
	m_penYLprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYLprice	, g_clConfigSet.m_clrYLprice		);//전일가격 저가				펜
	m_penYEprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYEprice	, g_clConfigSet.m_clrYEprice		);//전일가격 종가				펜
	m_penYSHL			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineYSHL		, g_clConfigSet.m_clrYSHL			);//전일가격 (시+고+저)/3		펜
	m_penTSprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTSprice	, g_clConfigSet.m_clrTSprice		);//당일가격 시가				펜
	m_penTHprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTHprice	, g_clConfigSet.m_clrTHprice		);//당일가격 고가				펜
	m_penTLprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTLprice	, g_clConfigSet.m_clrTLprice		);//당일가격 저가				펜
	m_penTEprice		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTEprice	, g_clConfigSet.m_clrTEprice		);//당일가격 종가				펜
	m_penTHL			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTHL		, g_clConfigSet.m_clrTHL			);//당일가격 (고+저)/2			펜
	m_penTSHL			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTSHL		, g_clConfigSet.m_clrTSHL			);//당일가격 (시+고+저)/3		펜
	m_penTHLE			.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineTHLE		, g_clConfigSet.m_clrTHLE			);//당일가격 (고+저+(종*2))/4	펜	
	m_penHighLimit		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineHighLimit	, g_clConfigSet.m_clrHighLimit		);//상한가						펜
	m_penLowLimit		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineLowLimit	, g_clConfigSet.m_clrLowLimit		);//하한가						펜	
	m_penPivot2Rst		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot2Rst	, g_clConfigSet.m_clrPivot2Rst		);// 피봇 2차 저항				펜 
	m_penPivot1Rst		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot1Rst	, g_clConfigSet.m_clrPivot1Rst		);// 피봇 1차 저항				펜
	m_penPivotGLine		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivotGLine	, g_clConfigSet.m_clrPivotGLine		);// 피봇 기준선				펜
	m_penPivot1GG		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot1GG	, g_clConfigSet.m_clrPivot1GG		);// 피봇 1차 지지				펜
	m_penPivot2GG		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePivot2GG	, g_clConfigSet.m_clrPivot2GG		);// 피봇 2차 지지				펜	
	m_penDMKPreHPrice	.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineDMKPreHPrice,g_clConfigSet.m_clrDMKPreHPrice	);// Demark 예상고가			펜
	m_penDMKGLine		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineDMKGLine	, g_clConfigSet.m_clrDMKGLine		);// Demark 기준선				펜
	m_penDMKPreLPrice	.CreatePen(PS_SOLID,	g_clConfigSet.m_nLineDMKPreLPrice,g_clConfigSet.m_clrDMKPreLPrice	);// Demark 예상저가			펜	
	m_pen2Pyung1		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung1	, g_clConfigSet.m_clr2Pyung1		);// 이동평균 첫번째 입력값		펜
	m_pen2Pyung2		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung2	, g_clConfigSet.m_clr2Pyung2		);// 이동평균 두번째 입력값		펜
	m_pen2Pyung3		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung3	, g_clConfigSet.m_clr2Pyung3		);// 이동평균 세번째 입력값		펜
	m_pen2Pyung4		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLine2Pyung4	, g_clConfigSet.m_clr2Pyung4		);// 이동평균 네번째 입력값		펜	
	m_penPmmDanga		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePmmDanga	, g_clConfigSet.m_clrPmmDanga		);// 평균매입 단가				펜
	m_penPmmPDanga		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePmmPDanga	, g_clConfigSet.m_clrPmmPDanga		);// 평균매입 단가 +n%			펜
	m_penPmmMDanga		.CreatePen(PS_SOLID,	g_clConfigSet.m_nLinePmmMDanga	, g_clConfigSet.m_clrPmmMDanga		);// 평균매입 단가 -n%			펜

	m_penChegyul		.CreatePen(PS_SOLID,	1/*펜 굵기 */	, RGB(205	,	173	,	254	)									);		// 체결 시간표시	펜 
	m_penSell			.CreatePen(PS_SOLID,	1/*펜 굵기 */	, RGB(38	,	189	,	202	)									);		// 매도 체결 표시	펜
	m_penBuy			.CreatePen(PS_SOLID,	1/*펜 굵기 */	, RGB(235	,	97	,	152	)									);		// 매수 체결 표시	펜

	
}

void CPatternMaker::DeleteLinePen()
{
	//================================================================	
	m_penYSprice		.DeleteObject();
	m_penYHprice		.DeleteObject();
	m_penYLprice		.DeleteObject();
	m_penYEprice		.DeleteObject();
	m_penYSHL			.DeleteObject();
	m_penTSprice		.DeleteObject();
	m_penTHprice		.DeleteObject();
	m_penTLprice		.DeleteObject();
	m_penTEprice		.DeleteObject();
	m_penTHL			.DeleteObject();
	m_penTSHL			.DeleteObject();
	m_penTHLE			.DeleteObject();
	m_penHighLimit		.DeleteObject();
	m_penLowLimit		.DeleteObject();
	m_penPivot2Rst		.DeleteObject();
	m_penPivot1Rst		.DeleteObject();
	m_penPivotGLine		.DeleteObject();
	m_penPivot1GG		.DeleteObject();
	m_penPivot2GG		.DeleteObject();
	m_penDMKPreHPrice	.DeleteObject();
	m_penDMKGLine		.DeleteObject();
	m_penDMKPreLPrice	.DeleteObject();
	m_pen2Pyung1		.DeleteObject();
	m_pen2Pyung2		.DeleteObject();
	m_pen2Pyung3		.DeleteObject();
	m_pen2Pyung4		.DeleteObject();
	m_penPmmDanga		.DeleteObject();
	m_penPmmPDanga		.DeleteObject();
	m_penPmmMDanga		.DeleteObject();
	m_penChegyul		.DeleteObject();		// 체결 시간표시	펜 
	m_penSell			.DeleteObject();		// 매도 체결 표시	펜
	m_penBuy			.DeleteObject();		// 매수 체결 표시	펜
	//================================================================	
}

void CPatternMaker::Init_GuideLineValue()
{		
	m_bDraw2Pyung1	= FALSE;			// 이동평균 1번째 그릴것 인지..
	m_bDraw2Pyung2	= FALSE;			// 이동평균 2번째 그릴것 인지..
	m_bDraw2Pyung3	= FALSE;			// 이동평균 3번째 그릴것 인지..
	m_bDraw2Pyung4	= FALSE;			// 이동평균 4번째 그릴것 인지..

	m_dYSprice		= 0 ;	//전일가격 시가
	m_dYHprice		= 0 ;	//전일가격 고가
	m_dYLprice		= 0 ;	//전일가격 저가
	m_dYEprice		= 0 ;	//전일가격 종가
	m_dYSHL			= 0 ;	//전일가격 (시+고+저)/3
	m_dTSprice		= 0 ;	//당일가격 시가
	m_dTHprice		= 0 ;	//당일가격 고가
	m_dTLprice		= 0 ;	//당일가격 저가
	m_dTEprice		= 0 ;	//당일가격 종가
	m_dTHL			= 0 ;	//당일가격 (고+저)/2
	m_dTSHL			= 0 ;	//당일가격 (시+고+저)/3
	m_dTHLE			= 0 ;	//당일가격 (고+저+(종*2))/4
	m_dHighLimit	= 0 ;	//상한가
	m_dLowLimit		= 0 ;	//하한가
	m_dPivot2Rst	= 0 ;	// 피봇 2차 저항
	m_dPivot1Rst	= 0 ;	// 피봇 1차 저항
	m_dPivotGLine	= 0 ;	// 피봇 기준선
	m_dPivot1GG		= 0 ;	// 피봇 1차 지지
	m_dPivot2GG		= 0 ;	// 피봇 2차 지지
	m_dDMKPreHPrice = 0 ;	// Demark 예상고가
	m_dDMKGLine		= 0 ;	// Demark 기준선
	m_dDMKPreLPrice = 0 ;	// Demark 예상저가

	m_dMAPeriod1	= 0 ;	//이동평균 1번째 값
	m_dMAPeriod2	= 0 ;	//이동평균 2번째 값
	m_dMAPeriod3	= 0 ;	//이동평균 3번째 값
	m_dMAPeriod4	= 0 ;	//이동평균 4번째 값

	m_dMASum1		= 0 ;
	m_dMASum2		= 0 ;
	m_dMASum3		= 0 ;
	m_dMASum4		= 0 ;

	m_dPmmDanga		= 0 ;	//평균매입단가
	m_dPmmPDanga	= 0 ;	//단가 + n%
	m_dPmmMDanga	= 0 ;	//단가 - n%	
}


void CPatternMaker::RecvRealCurPrice(CString sEnd)
{	
	m_dTEprice = atof( sEnd);
	
	// (2008/9/5 - Seung-Won, Bae) Manage TR Code for Future/Option
	switch( m_nCodeGubun)
	{
		case 2:
		case 4:
		case 5:
		case 6:
				m_dTEprice /= 100.;
				break;
	}
	
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//당일가격 (고+저+(종*2))/4	

	m_dMAPeriod1 = (m_dTEprice + m_dMASum1) / ((double)g_clConfigSet.m_nDay2Pyung1);
	m_dMAPeriod2 = (m_dTEprice + m_dMASum2) / ((double)g_clConfigSet.m_nDay2Pyung2);
	m_dMAPeriod3 = (m_dTEprice + m_dMASum3) / ((double)g_clConfigSet.m_nDay2Pyung3);
	m_dMAPeriod4 = (m_dTEprice + m_dMASum4) / ((double)g_clConfigSet.m_nDay2Pyung4);
}

void CPatternMaker::RecvRealHigh(CString sHigh)
{	
	m_dTHprice = atof(sHigh);
	
	m_dTHL			= (m_dTHprice + m_dTLprice) / 2;					//당일가격 (고+저)/2
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//당일가격 (시+고+저)/3
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//당일가격 (고+저+(종*2))/4	
}

void CPatternMaker::RecvRealOpen(CString sOpen)
{	
	m_dTSprice = atof(sOpen);
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//당일가격 (시+고+저)/3
}

void CPatternMaker::RecvRealLow(CString sLow)
{	
	m_dTLprice = atof(sLow );
	
	m_dTHL			= (m_dTHprice + m_dTLprice) / 2;					//당일가격 (고+저)/2
	m_dTSHL			= (m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//당일가격 (시+고+저)/3
	m_dTHLE			= (m_dTHprice + m_dTLprice + (m_dTEprice * 2)) / 4;	//당일가격 (고+저+(종*2))/4	
}

void CPatternMaker::RecvRealJango(CString sMMDanga)
{
	if(sMMDanga.GetLength())
		m_dPmmDanga = atof(sMMDanga);
	else
	{		
		m_dPmmDanga		= 0;
		m_dPmmPDanga	= 0;
		m_dPmmMDanga	= 0;

		return;
	}

	if(m_dPmmDanga)
	{		
		m_dPmmPDanga = m_dPmmDanga + (abs(g_clConfigSet.m_nPmmPDanga) * m_dPmmDanga / 100);
		m_dPmmMDanga = m_dPmmDanga - (abs(g_clConfigSet.m_nPmmMDanga) * m_dPmmDanga / 100);
	}
}

//이동 평균 값
void CPatternMaker::SetMAInfo(D10418_O *pD10418)
{
	memcpy(&m_MAvgInfo, pD10418, sizeof(D10418_O));
	m_strCode.Format("%*.*s", sizeof(pD10418->sShcode), sizeof(pD10418->sShcode), pD10418->sShcode);
	m_strCode.Replace(" ", "");

	CString sTemp, sCurPrice;
	sCurPrice.Format("%*.*s", sizeof(pD10418->sCurPrice), sizeof(pD10418->sCurPrice), pD10418->sCurPrice);
	double dCurPrice = atof(sCurPrice); 
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung1)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum1), sizeof(pD10418->sSum1), pD10418->sSum1);
		m_dMASum1 = atof(sTemp);	
		m_dMAPeriod1 = (dCurPrice + m_dMASum1) / ((double)g_clConfigSet.m_nDay2Pyung1);
		sTemp.Format("SetMAInfo - 1 - Sum : %f, Avg : %f ", m_dMASum1, m_dMAPeriod1);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod1 = 0;
		m_dMASum1 = 0;
		sTemp.Format("SetMAInfo - 1 - Sum : %f, Avg : %f ", m_dMASum1, m_dMAPeriod1);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	OutputDebugString("----------------------------------------------------------");
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung2)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum2), sizeof(pD10418->sSum2), pD10418->sSum2);
		m_dMASum2 = atof(sTemp);	
		m_dMAPeriod2 = (dCurPrice + m_dMASum2) / ((double)g_clConfigSet.m_nDay2Pyung2);
		sTemp.Format("SetMAInfo - 2 - Sum : %f, Avg : %f ", m_dMASum2, m_dMAPeriod2);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod2 = 0;
		m_dMASum2 = 0;
		sTemp.Format("SetMAInfo - 2 - Sum : %f, Avg : %f ", m_dMASum2, m_dMAPeriod2);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	OutputDebugString("----------------------------------------------------------");
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung3)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum3), sizeof(pD10418->sSum3), pD10418->sSum3);
		m_dMASum3 = atof(sTemp);	
		m_dMAPeriod3 = (dCurPrice + m_dMASum3) / ((double)g_clConfigSet.m_nDay2Pyung3);
		sTemp.Format("SetMAInfo - 3 - Sum : %f, Avg : %f ", m_dMASum3, m_dMAPeriod3);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod3 = 0;
		m_dMASum3 = 0;
		sTemp.Format("SetMAInfo - 3 - Sum : %f, Avg : %f ", m_dMASum3, m_dMAPeriod3);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	OutputDebugString("----------------------------------------------------------");
//============================================================================================
	if(g_clConfigSet.m_bChk2Pyung4)
	{
		sTemp.Format("%*.*s", sizeof(pD10418->sSum4), sizeof(pD10418->sSum4), pD10418->sSum4);
		m_dMASum4 = atof(sTemp);	
		m_dMAPeriod4 = (dCurPrice + m_dMASum4) / ((double)g_clConfigSet.m_nDay2Pyung4);
		sTemp.Format("SetMAInfo - 4 - Sum : %f, Avg : %f ", m_dMASum4, m_dMAPeriod4);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
	else
	{
		m_dMAPeriod4 = 0;
		m_dMASum4 = 0;
		sTemp.Format("SetMAInfo - 4 - Sum : %f, Avg : %f ", m_dMASum4, m_dMAPeriod4);
		OutputDebugString(m_strCode + sTemp + "\n\n");
	}
}

void CPatternMaker::GetPMMDanga()
{
	if(m_strCode.GetLength() < 6) 
	{
		m_dPmmDanga		= 0;
		m_dPmmPDanga	= 0;
		m_dPmmMDanga	= 0;

		return;
	}

	if( g_clConfigSet.m_bChkPmmDanga	||
		g_clConfigSet.m_bChkPmmPDanga	||
		g_clConfigSet.m_bChkPmmMDanga 
		)
	{
		HINSTANCE hCommDll = ::LoadLibrary(_T("Comm.DLL"));
		if(hCommDll)
		{					
			//Acct(11) + Name(20) + Pass(8)  : 전체 계좌 리스트 
			pGetUnitPrice2 = (FpGetUnitPrice2)::GetProcAddress(hCommDll, "EX_GetUnitPrice");
			if(pGetUnitPrice2) 
			{
				LPCSTR szUintPrice = pGetUnitPrice2((LPTSTR)(LPCTSTR)g_clConfigSet.m_strAcct, (LPTSTR)(LPCTSTR)m_strCode);
				CString strPmmDanga;
				strPmmDanga.Format("%9.9s", szUintPrice);
				RecvRealJango(strPmmDanga);				
			}
			
			FreeLibrary(hCommDll);
		}
	}
}

void CPatternMaker::SetCode(CString strCode)
{
	m_strCode = strCode;
}

void CPatternMaker::SetChegyulList(D48112_OSUB *pD48112_OSub)
{
	DeleteCheChartData();

	CString sTemp;
	sTemp.Format("%*.*s", sizeof(pD48112_OSub->grdCnt), sizeof(pD48112_OSub->grdCnt), pD48112_OSub->grdCnt);
	int nCnt = atoi(sTemp); //전체 체결 리스트 갯수.
	//if(nCnt > 0) m_bDrawChegyulLine = TRUE;
	m_bDrawChegyulLine = TRUE;//삼성 데모용
	for(int i = 0 ; i < nCnt ; i++)
	{		
		D48112_OG* pChegyulData = (D48112_OG*)((pD48112_OSub->grid) + i);
		CCheChartData* pCheChartData = NULL;
		pCheChartData = new CCheChartData;
		if(pCheChartData)
		{
			sTemp.Format("%*.*s", sizeof(pChegyulData->jmcode), sizeof(pChegyulData->jmcode), pChegyulData->jmcode);
			sTemp.TrimLeft(); sTemp.TrimRight(); sTemp.Remove(' '); sTemp.Remove('A');
			pCheChartData->m_strJmCode = sTemp; //종목코드

			sTemp.Format("%*.*s", sizeof(pChegyulData->chetime), sizeof(pChegyulData->chetime), pChegyulData->chetime);
			pCheChartData->m_strChetime = sTemp; //체결 날짜.

			sTemp.Format("%*.*s", sizeof(pChegyulData->chedanga), sizeof(pChegyulData->chedanga), pChegyulData->chedanga);
			pCheChartData->m_dChedanga = atof(sTemp); //체결 단가.

			sTemp.Format("%*.*s", sizeof(pChegyulData->mesumedogb), sizeof(pChegyulData->mesumedogb), pChegyulData->mesumedogb);
			pCheChartData->m_nMesuMedoGb = atoi(sTemp);//매도매수구분


			m_aryCheChartData.Add(pCheChartData);
		}		
	}
}

void CPatternMaker::DeleteCheChartData()
{
	m_bDrawChegyulLine = FALSE;
	CObject *pObj=NULL;
	for(int i=0; i < m_aryCheChartData.GetSize(); i++)
	{
		pObj = (CObject *) m_aryCheChartData.GetAt(i);
		delete pObj;
	}

	m_aryCheChartData.RemoveAll();
}



void CPatternMaker::ReadOneLineForSamgungDemo()
{
	OutputDebugString("Read...file");

	FILE* pfile=fopen("chart.txt","r");
	if(pfile)
	{
	   m_ArrayFileChart.RemoveAll();

	   char szBuf[256];
	   memset(szBuf, 0x00, sizeof(GData));
	   while(fgets(szBuf,256,pfile))
	   {
			
			CString sTemp;
			sTemp.Format("%s", szBuf);
			sTemp.TrimLeft(); sTemp.TrimRight(); sTemp.Remove(' ');
			if(sTemp.GetLength())
			{				
				OutputDebugString(sTemp);
				GData *pData = (GData*)(LPTSTR)(LPCTSTR)sTemp;			
				m_ArrayFileChart.Add(sTemp);
			}
	   }
	   fclose(pfile);
	}
}

//{{ 통신(소켓) 처리 모듈
#define DR_MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;

HRESULT CPatternMaker::CTrCommSite::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDataLen)
{
	if(dwTRDataLen==0)
		return 0;
	DR_MYMETHOD_PROLOGUE(CPatternMaker, TrCommSite)
	
	if( !strcmp( szTR, "70000") || !strcmp( szTR, "80000") || !strcmp( szTR, "30000"))
		pThis->ReceiveSiseData(aTRData, dwTRDataLen);
	// (2008/9/9 - Seung-Won, Bae) Request MA Data
	else if( !strcmp( szTR, "70001") || !strcmp( szTR, "80001") || !strcmp( szTR, "30001"))
		pThis->ReceiveMaData( aTRData, dwTRDataLen);

	return S_OK;
}

HRESULT CPatternMaker::CTrCommSite::StatusChange(int nState)
{
	return  S_OK;
}

HRESULT CPatternMaker::CTrCommSite::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	// (2008/9/4 - Seung-Won, Bae) Support Real Data
	DR_MYMETHOD_PROLOGUE(CPatternMaker, TrCommSite)

	CRealUpdateData* cReal = (CRealUpdateData*)pRealData;
	if( !cReal) return 0L;

	CString strPrice = cReal->GetDataString2( DI_PRICE);// 현재가
	if( !strPrice.IsEmpty()) pThis->RecvRealCurPrice( strPrice);

	return 1;
}

void CPatternMaker::RequestSiseData()
{
	CString sCode = m_strCode;
	if(sCode.GetLength()<1)
		return;

	IGateManager* pGateMng = NULL;
	AfxGetIGateManager( pGateMng);
	IMasterDataManager2* pDataManager = NULL;
	if(pGateMng) pDataManager = ( IMasterDataManager2 *)pGateMng->GetInterface(51);

	// (2008/9/9 - Seung-Won, Bae) Request MA Data
	m_nCodeGubun = atoi( pDataManager->GetCodeInfo2("", 2, m_strCode));
	if( 0 == m_nCodeGubun)
	{
		m_nCodeGubun = atoi( m_strCode.Left(1));
		m_strCode = m_strCode.Mid( 1);
	}

	if( m_nCodeGubun == 1 || m_nCodeGubun == 3)
	{
		m_TRComm->SetTR("70000");
		m_TRComm->AddMainKey(DI_CODE, m_strCode, C_DT_STRING, 6);
		m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// 업데이트 타입
		m_TRComm->AddDataKey(3004);//전일시가
		m_TRComm->AddDataKey(3005);//전일고가
		m_TRComm->AddDataKey(3006);//전일저가
		m_TRComm->AddDataKey(3003);//전일종가
		m_TRComm->AddDataKey(15009);//시가
		m_TRComm->AddDataKey(15010);//고가
		m_TRComm->AddDataKey(15011);//저가
		m_TRComm->AddDataKey(15001);//현재가
		m_TRComm->AddDataKey(16117);//상한가
		m_TRComm->AddDataKey(16118);//하한가
	}
	else if( m_nCodeGubun == 5 || m_nCodeGubun == 6)
	{
		m_TRComm->SetTR("80000");
		m_TRComm->AddMainKey(DI_CODE, m_strCode, C_DT_STRING, 8);
		m_TRComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// 업데이트 타입
		m_TRComm->AddDataKey(3004);//전일시가
		m_TRComm->AddDataKey(3005);//전일고가
		m_TRComm->AddDataKey(3006);//전일저가
		m_TRComm->AddDataKey(3003);//전일종가
		m_TRComm->AddDataKey(15009);//시가
		m_TRComm->AddDataKey(15010);//고가
		m_TRComm->AddDataKey(15011);//저가
		m_TRComm->AddDataKey(15001);//현재가
		m_TRComm->AddDataKey(16117);//상한가
		m_TRComm->AddDataKey(16118);//하한가
	}
	else if( m_nCodeGubun == 2 || m_nCodeGubun == 4)
	{
		m_TRComm->SetTR("30000");
		m_TRComm->AddMainKey( DI_CODE, m_strCode, C_DT_USHORT, 2);
		m_TRComm->SetForceMid( m_nCodeGubun);
		m_TRComm->SetRealUpdateType( 2);			//  RT_UPDATE	2	// 업데이트 타입
		m_TRComm->AddDataKey(3004);//전일시가
		m_TRComm->AddDataKey(3005);//전일고가
		m_TRComm->AddDataKey(3006);//전일저가
		m_TRComm->AddDataKey(3003);//전일종가
		m_TRComm->AddDataKey(15009);//시가
		m_TRComm->AddDataKey(15010);//고가
		m_TRComm->AddDataKey(15011);//저가
		m_TRComm->AddDataKey(15001);//현재가
	}
	else return;

	m_TRComm->Send2Server("", 0, FALSE);				
}	

void CPatternMaker::ReceiveSiseData(LPVOID	aTRData, long dwTRDataLen)
{
	int nKeyCount = 10;

	CString strOut, strData;
	if( m_nCodeGubun == 1 || m_nCodeGubun == 3)
	{
		int nIdx = 11 + 2 * nKeyCount + 6;

		// 전일가격
		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dYSprice		= atof(strData); 		//전일가격 시가	 

		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dYHprice		= atof(strData); 		//전일가격 고가 

		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dYLprice		= atof(strData); 		//전일가격 고가 

		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dYEprice		= atof(strData); 		//전일가격 고가 

		//당일가격 
		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dTSprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dTHprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dTLprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dTEprice		= atof(strData); 

		// 상한가
		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dHighLimit		= atof(strData); 

		// 하한가
		strData = m_TRComm->ApplyByteOrder2(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
		m_dLowLimit		= atof(strData); 
	}
	else if( m_nCodeGubun == 5 || m_nCodeGubun == 6)	//선물옵션
	{
		int nIdx = 11 + 2 * nKeyCount + 8;

		int nType, nLength, nExp;
		m_TRComm->GetFidInfo( 15001, m_nCodeGubun, nType, nLength, nExp);

		// 전일가격
		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYSprice		= atof(strData); 		//전일가격 시가	 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYHprice		= atof(strData); 		//전일가격 고가 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYLprice		= atof(strData); 		//전일가격 고가 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYEprice		= atof(strData); 		//전일가격 고가 

		//당일가격 
		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTSprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTHprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTLprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTEprice		= atof(strData); 

		// 상한가
		strData = m_TRComm->ApplyByteOrder2(C_DT_SHORT, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dHighLimit		= atof(strData); 

		// 하한가
		strData = m_TRComm->ApplyByteOrder2(C_DT_SHORT, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dLowLimit		= atof(strData); 
	}
	else if( m_nCodeGubun == 2 || m_nCodeGubun == 4)	// 업종
	{
		nKeyCount = 8;
		int nIdx = 11 + 2 * nKeyCount + 2;

		int nType, nLength, nExp;
		m_TRComm->GetFidInfo( 15001, m_nCodeGubun, nType, nLength, nExp);

		// 전일가격
		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYSprice		= atof(strData); 		//전일가격 시가	 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYHprice		= atof(strData); 		//전일가격 고가 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYLprice		= atof(strData); 		//전일가격 고가 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dYEprice		= atof(strData); 		//전일가격 고가 

		//당일가격 
		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTSprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTHprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTLprice		= atof(strData); 

		strData = m_TRComm->ApplyByteOrder2(nType, (char *)aTRData+nIdx, 10, &nIdx, 2);
		m_dTEprice		= atof(strData); 

		// 상한가 / 하한가
		m_dHighLimit	= 0; 
		m_dLowLimit		= 0; 
	}
	else return;

	m_dYSHL			= (m_dYSprice + m_dYHprice + m_dYLprice) / 3;		//전일가격 (시+고+저)/3
	m_dTHL			= ( m_dTHprice + m_dTLprice) / 2;					//당일가격 (고+저)/2
	m_dTSHL			= ( m_dTSprice + m_dTHprice + m_dTLprice) / 3;		//당일가격 (시+고+저)/3
	m_dTHLE			= ( m_dTHprice + m_dTLprice + m_dTEprice * 2) / 4;	//당일가격 (고+저+(종*2))/4

	//Pivot===============================================================================
	m_dPivotGLine = (m_dYEprice + m_dYHprice + m_dYLprice ) / 3;
	m_dPivot2Rst = m_dPivotGLine + m_dYHprice - m_dYLprice;
	m_dPivot1Rst = m_dPivotGLine * 2 - m_dYLprice;
	m_dPivot1GG = m_dPivotGLine * 2 - m_dYHprice;	
	m_dPivot2GG = m_dPivotGLine - m_dYHprice + m_dYLprice;	

	//Demark==============================================================================
	if (m_dYSprice > m_dYEprice)
		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYLprice) / 2;
	else if (m_dYSprice < m_dYEprice)
		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYHprice) / 2;
	else
		m_dDMKGLine = (m_dYHprice + m_dYLprice + m_dYEprice + m_dYEprice) / 2;

	m_dDMKPreHPrice = m_dDMKGLine - m_dYLprice;
	m_dDMKPreLPrice = m_dDMKGLine - m_dYHprice;
	m_dDMKGLine = m_dDMKGLine / 2;

	// (2008/9/9 - Seung-Won, Bae) Request MA Data
	RequestMaData();
}
//}}

// Set Pointer of IChartOCX 
void CPatternMaker::SetIChartOCX( IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager)
{
	m_pIChartManager	= p_pIChartManager;
	m_pIChartOCX		= p_pIChartOCX;
	m_pIPacketManager = p_pIPacketManager;

	DeleteLinePen();
	CreateLinePen();
}

// Refresh Chart for Pattern Data
void CPatternMaker::RefreshChart()
{
	m_pIChartOCX->InvalidateControl();
}

// (2008/9/9 - Seung-Won, Bae) Request MA Data
void CPatternMaker::RequestMaData( void)
{
	CString sCode = m_strCode;
	if(sCode.GetLength()<1)
		return;

	int nMaxRange = g_clConfigSet.m_nDay2Pyung1;
	if( nMaxRange < g_clConfigSet.m_nDay2Pyung2) nMaxRange = g_clConfigSet.m_nDay2Pyung2;
	if( nMaxRange < g_clConfigSet.m_nDay2Pyung3) nMaxRange = g_clConfigSet.m_nDay2Pyung3;
	if( nMaxRange < g_clConfigSet.m_nDay2Pyung4) nMaxRange = g_clConfigSet.m_nDay2Pyung4;
	CString sCount;
	sCount.Format( "%d", nMaxRange);

	if( m_nCodeGubun == 1 || m_nCodeGubun == 3)
	{
		m_TRComm->SetTR( "70001");
		m_TRComm->AddMainKey( DI_CODE, sCode, C_DT_STRING, 6);
		m_TRComm->AddOptionKey( 1, "", 13, 0, 0);
		m_TRComm->AddOptionKey( 1000, sCount, C_DT_SHORT, 2, 3);
		m_TRComm->AddOptionKey( 1999, "53", C_DT_BYTE, 1, 6);
		m_TRComm->AddOptionKey( 1060, "1", C_DT_ULONG, 4, 2);
		m_TRComm->AddDataKey( 30700);	//종가
		m_TRComm->Send2Server("", 0, FALSE);
	}
	else if( m_nCodeGubun == 5 || m_nCodeGubun == 6)
	{
		m_TRComm->SetTR( "80001");
		m_TRComm->AddMainKey( DI_CODE, sCode, C_DT_STRING, 8);
		m_TRComm->AddOptionKey( 1, "", 13, 0, 0);
		m_TRComm->AddOptionKey( 1000, sCount, C_DT_SHORT, 2, 3);
		m_TRComm->AddOptionKey( 1999, "51", C_DT_BYTE, 1, 6);
		m_TRComm->AddOptionKey( 1060, "1", C_DT_ULONG, 4, 2);
		m_TRComm->AddDataKey( 15001);	//종가
		m_TRComm->Send2Server("", 0, FALSE);
	}
	else if( m_nCodeGubun == 2 || m_nCodeGubun == 4)
	{
		m_TRComm->SetTR( "30001");
		m_TRComm->AddMainKey( DI_CODE, m_strCode, C_DT_USHORT, 2);
		m_TRComm->SetForceMid( m_nCodeGubun);
		m_TRComm->AddOptionKey( 1, "", 13, 0, 0);
		m_TRComm->AddOptionKey( 1000, sCount, C_DT_SHORT, 2, 3);
		m_TRComm->AddOptionKey( 1999, "53", C_DT_BYTE, 1, 6);
		m_TRComm->AddOptionKey( 1060, "1", C_DT_ULONG, 4, 2);
		m_TRComm->AddDataKey( 15001);	//종가
		m_TRComm->Send2Server("", 0, FALSE);
	}
}
void CPatternMaker::ReceiveMaData( LPVOID aTRData, long dwTRDataLen)
{
	int nIdx = 11 + 2 * 3 + 2 * 1;

	int nPriceType = C_DT_ULONG, nDotPos = 0, nLength, nExp;
	if( m_nCodeGubun == 1 || m_nCodeGubun == 3)	// 주식
	{
		nIdx += 6;
		nDotPos = 0;
	}
	else if( m_nCodeGubun == 5 || m_nCodeGubun == 6) // 선옵
	{
		nIdx += 8; 
		nDotPos = 2;
	}
	else if( m_nCodeGubun == 2 || m_nCodeGubun == 4) // 업종
	{
		nIdx += 2; 
		nDotPos = 2;
	}
	
	CString strCount = m_TRComm->ApplyByteOrder2(C_DT_SHORT, (char *)aTRData+nIdx, 4, &nIdx);
	nIdx += 5;
	
	int nCount = atoi(strCount);
	m_TRComm->GetFidInfo( 15001, m_nCodeGubun, nPriceType, nLength, nExp);

	CString strData;
	double dToday = 0;
	m_dMASum1 = 0;
	m_dMASum2 = 0;
	m_dMASum3 = 0;
	m_dMASum4 = 0;
	for(int i = 0; i< nCount; i++)
	{
		if( nIdx >= dwTRDataLen)
		{
			nCount = i;
			break;
		}

		strData = m_TRComm->ApplyByteOrder2(nPriceType, (char *)aTRData+nIdx, 10, &nIdx, nDotPos);

		if( 0 == i)
		{
			dToday = atof( strData);
			continue;
		}

		if( i < g_clConfigSet.m_nDay2Pyung1) m_dMASum1 += atof( strData);
		if( i < g_clConfigSet.m_nDay2Pyung2) m_dMASum2 += atof( strData);
		if( i < g_clConfigSet.m_nDay2Pyung3) m_dMASum3 += atof( strData);
		if( i < g_clConfigSet.m_nDay2Pyung4) m_dMASum4 += atof( strData);
	}
	m_dMAPeriod1 = ( m_dMASum1 + dToday) / g_clConfigSet.m_nDay2Pyung1;
	m_dMAPeriod2 = ( m_dMASum2 + dToday) / g_clConfigSet.m_nDay2Pyung2;
	m_dMAPeriod3 = ( m_dMASum3 + dToday) / g_clConfigSet.m_nDay2Pyung3;
	m_dMAPeriod4 = ( m_dMASum4 + dToday) / g_clConfigSet.m_nDay2Pyung4;

	RefreshChart();
}

// (2008/10/26 - Seung-Won, Bae) Draw DO/DH/DL/DC
void CPatternMaker::SetDrawOHLC( BOOL p_bDrawOHLC, COLORREF p_clrUp, COLORREF p_clrDown)
{
	m_bDrawOHLC = p_bDrawOHLC;
	m_clrO = RGB( GetRValue( p_clrDown), GetGValue( p_clrDown) / 2, GetBValue( p_clrDown) / 2);
	m_clrH = p_clrUp;
	m_clrL = p_clrDown;
	m_clrC = RGB( GetRValue( p_clrUp), GetGValue( p_clrUp)/ 2, GetBValue( p_clrUp) / 2);
}
