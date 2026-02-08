// ChartExcelDataImportAddInImp.cpp: implementation of the CChartExcelDataImportAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "ChartExcelDataImportAddInImp.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Chart/Conversion.h"							// for CDataConversion
#include "../Include_Addin_134221/I134221/_IHelpMessageMap.h"		// for IHelpMessageMap

#include "BasicExcelVC6.h"
using namespace YExcel;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartExcelDataImportAddInImp::CChartExcelDataImportAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2008/3/3 - Seung-Won, Bae) for Multi Language
	m_hOcxWnd = m_pChartCWnd->GetSafeHwnd();
}

CChartExcelDataImportAddInImp::~CChartExcelDataImportAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartExcelDataImportAddInImp::m_strAddInItemName = "EXCEL_IMPORT";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartExcelDataImportAddInImp)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnCmdMsg)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartExcelDataImportAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	return FALSE;
#if 0	//Tool ID가 정의가 되면 푼다.	
	if( nID != ID_DLV_SHOW_DATA_LIST_VIEWER) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		CCmdUI *pCmdUI = ( CCmdUI *)pExtra;
		if( !pCmdUI) return FALSE;
		pCmdUI->Enable( TRUE);
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_DATALIST_REPORT, 1);
		return TRUE;
	}
#endif
	return TRUE;
}

void CChartExcelDataImportAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( CToolOptionInfo::T_EXCEL_IMPORT != p_nToolType) return;

	if(!p_nCmdOption) return;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LoadExcel();	

	return;
}

BOOL CChartExcelDataImportAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(  p_nCommandType  != 1) return FALSE;

	return LoadExcel();
}

//////////////////////////////////////////////////////////////////////////
// 엑셀 파일에서 데이터를 불러오는 경우는 숫자형의 데이터만을 불러 온다.
// 즉 문자형의 데이터는 지원하지 않는다.
// 날짜데이터는 엑셀에서의 셀 포맷이 날짜형인 것만을 지원한다.
// 차트의 설정이 되어 있는 경우만 지원한다. 즉 보조메시지 등 이미 설정이 되어 있는 경우에만 차트를 그릴 수 있다.
// 보조메시지가 없는 빈 차트의 경우 데이터는 입력이 되나, 화면에 표시하기 위해서는 사용자는 기간 초기화 등의 작업을 해야한다.
BOOL CChartExcelDataImportAddInImp::LoadExcel()
{
	CString strFilter = "*.xls";

	CFileDialog dlg(TRUE, "*.xls", "*.xls", OFN_FILEMUSTEXIST, strFilter);
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		BasicExcel e;

		if(!e.Load(strFileName)) return FALSE;
		//BasicExcelWorksheet* sheet1 = e.GetWorksheet("Sheet1");
		size_t iIndex = 0;
		BasicExcelWorksheet* sheet1 = e.GetWorksheet(iIndex);
		if (sheet1)
		{
			// import start
			//////////////////////////////////////////////////////////////////////////
			// 보조 메시지 얻기
			IHelpMessageMap *SubMsgMap = m_pIDefaultPacketManager->GetHelpMessageMap();
			CString strSubMsg;
			ILPCSTR strKey, strValue;
			POSITION pos  = SubMsgMap->GetStartPosition();
			while (pos)
			{
				SubMsgMap->GetNextAssoc(pos, strKey, strValue);
				strSubMsg.Insert(0,CString(strKey) + "=" + CString(strValue) + "@");
			}
			SubMsgMap->Release();
			SubMsgMap = NULL;
			//////////////////////////////////////////////////////////////////////////
			if(strSubMsg == "")	// 보조메시지가 없는 경우 데이터만 입력한다.
			{
				size_t maxRows = sheet1->GetTotalRows();
				size_t maxCols = sheet1->GetTotalCols();
				BasicExcelCell* CellPacketName;
				CString strPacketName;

				// data import
				for(int i=0;i<maxCols;i++)
				{
					// Packet 명 얻기
					strPacketName = "";
					CellPacketName = sheet1->Cell(0,i);
					strPacketName = CellPacketName->GetWString();
					
					IPacket *pIPacket = NULL;
					// if(strPacketName.Find("일자") >= 0)
					CString strDate;
					strDate.LoadString(IDS_DATE);
					if(strPacketName.Find(strDate) >= 0)
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("자료일자");
						//strPacketName = "자료일자";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_DATE_TIME));
						strPacketName = _MTEXT(C0_DATE_TIME);
					}
					// else if(strPacketName.Find("거래량") >= 0)	
					else if(strPacketName.Find( _LTEXT3( C3_VOLUME)) >= 0)	
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("기본거래량");
						//strPacketName = "기본거래량";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_VOLUME));
						strPacketName = _MTEXT(C0_VOLUME);
					}
					else
					{
						strPacketName = _LTOM0( strPacketName);
						pIPacket = m_pIDefaultPacketManager->GetPacket( strPacketName);
					}

					if(!pIPacket)
					{
					//	CString strMsg;
					//	strMsg.Format("[%s]패킷이 설정되어있지 않습니다.\n 패킷 설정 또는 엑셀 파일의 항목을 확인하시기 바랍니다.", strPacketName);
					//	AfxMessageBox(strMsg);
						//continue;
						return TRUE;
					}

					pIPacket->ClearData(true);
					ILPCSTR szPacketType = pIPacket->GetType();
					CString strPacketType( szPacketType);
					if( strPacketType == _MTEXT( C6_CHARACTER))
					{
						CString strMsg;
						// strMsg = "텍스트 데이터는 임포트할 수 없습니다.";
						strMsg.LoadString(IDS_TEXTIMPORT);
						AfxMessageBox(strMsg);
					}
					else
					{
						CTimeSpan t(0, 0, 0, 0 );
						//for(int j=1; j<maxRows; j++)
						for(int j=maxRows-1; j>0; j--)
						{
							// if(strPacketName == "자료일자")	_MTEXT(C0_DATE_TIME)
							if(strPacketName == _MTEXT(C0_DATE_TIME))
							{
								//////////////////////////////////////////////////////////////////////////
								// 아래의 조건은 경우에 따리 바뀔 수 있다.
								if( ( strPacketType == "YYYYMMDD") || ( strPacketType == "YYMMDD") ||
									( strPacketType == "YYYYMM") || ( strPacketType == "YYYYMMDD") ||
									( strPacketType == "YYMM") || ( strPacketType == "MMDD") )
									pIPacket->AppendData(sheet1->Cell(j,i)->GetDate());
								else
									pIPacket->AppendData(sheet1->Cell(j,i)->GetTime());
							}
							else
								pIPacket->AppendData(sheet1->Cell(j,i)->GetDouble());
						}					
					}
					pIPacket->Release();
				}
			}
			else
			{
				m_pIChartOCX->SetBinTrHelpMsg(strSubMsg);	// SetBinTr을 위한 보조 메시지 세팅 
				size_t maxRows = sheet1->GetTotalRows();
				size_t maxCols = sheet1->GetTotalCols();
				BasicExcelCell* CellPacketName;
				CString strPacketName;
				
				double* p_dBuf;
				
				//////////////////////////////////////////////////////////////////////////
				// data import
				for(int i=0;i<maxCols;i++)
				{
					p_dBuf = new double[maxRows-1];
					memset(p_dBuf,0,sizeof(double)*(maxRows-1));
					
					// Packet 명 얻기
					strPacketName = "";
					CellPacketName = sheet1->Cell(0,i);
					strPacketName = CellPacketName->GetWString();
					
					IPacket *pIPacket = NULL;
					CString strDate;
					strDate.LoadString(IDS_DATE);
					// if(strPacketName.Find("일자") >= 0)
					if(strPacketName.Find(strDate) >= 0)
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("자료일자");
						//strPacketName = "자료일자";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_DATE_TIME));
						strPacketName = _MTEXT(C0_DATE_TIME);
					}
					// else if(strPacketName.Find("거래량") >= 0)
					else if(strPacketName.Find( _LTEXT3( C3_VOLUME)) >= 0)
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("기본거래량");
						//strPacketName = "기본거래량";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_VOLUME));
						strPacketName = _MTEXT(C0_VOLUME);
					}
					else
					{
						strPacketName = _LTOM0( strPacketName);
						pIPacket = m_pIDefaultPacketManager->GetPacket( strPacketName);
					}
					
					if(!pIPacket)
					{
						//	CString strMsg;
						//	strMsg.Format("[%s]패킷이 설정되어있지 않습니다.\n 패킷 설정 또는 엑셀 파일의 항목을 확인하시기 바랍니다.", strPacketName);
						//	AfxMessageBox(strMsg);
						continue;
						//return TRUE;
					}
					
					pIPacket->ClearData(true);
					ILPCSTR szPacketType = pIPacket->GetType();
					CString strPacketType( szPacketType);
					if( strPacketType == _MTEXT(C6_CHARACTER))
					{
						CString strMsg;
						// strMsg = "텍스트 데이터는 임포트할 수 없습니다.";
						strMsg.LoadString( IDS_TEXTIMPORT);
						AfxMessageBox(strMsg);
						
					}
					else
					{
						CTimeSpan t(0, 0, 0, 0 );
						for(int j=0; j<maxRows-1; j++)
						{
							// if(strPacketName == "자료일자")
							if( strPacketName == _MTEXT(C0_DATE_TIME))
							{
								//////////////////////////////////////////////////////////////////////////
								// 아래의 조건은 경우에 따리 바뀔 수 있다.
								if( ( strPacketType == "YYYYMMDD") || ( strPacketType == "YYMMDD") ||
									( strPacketType == "YYYYMM") || ( strPacketType == "YYYYMMDD") ||
									( strPacketType == "YYMM") || ( strPacketType == "MMDD"))
									p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetDate();
								else
									p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetTime();
							}
							else
							// 20081013 박동관 8진법/32진법을 10진법으로 읽어온다. >>
							{
								CString strOutput;			// 변환된 값이 저장될 변수
								CString strInput = sheet1->Cell(j+1,i)->GetString();

								// (2008/10/19 - Seung-Won, Bae) Use double for Volume
								if( strInput.IsEmpty()) p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetDouble();
								else
								{
									// 패킷을 얻어온다.
									pIPacket = m_pIDefaultPacketManager->GetPacket(strPacketName);	
									if(pIPacket)
									{
										// 10진법으로 변환한다.
										ILPCSTR szPacketType2 = pIPacket->GetType();
										CString strPacketType2( szPacketType2);
										BOOL bResult = CDataConversion::GetDataToDec( strInput, strPacketType2, strOutput);
										// 문자열을 double로 변환한다.
										if( bResult)	p_dBuf[maxRows-2-j] = atof(strOutput);
										// 저장된 데이터가 32진법이 아닐때는 0으로 넣는다.
										else			p_dBuf[maxRows-2-j] = 0;
									}
									else				p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetDouble();
								}
							}
							// 20081013 박동관 <<
						}					
					}
					m_pIChartOCX->SetBinTrDoubleData(strPacketName, maxRows-1, p_dBuf);
					delete []p_dBuf;
					pIPacket->Release();
				}
				
				// import end
				m_pIChartOCX->SetBinTrEnd();
			}
		}
	}

	return TRUE;
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CChartExcelDataImportAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	OnToolCommand( CToolOptionInfo::T_DATALIST_REPORT, 0);
	OnToolCommand( CToolOptionInfo::T_DATALIST_REPORT, 1);
}
