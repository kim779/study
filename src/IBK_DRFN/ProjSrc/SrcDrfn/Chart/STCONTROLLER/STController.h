// STController.h: interface for the CSTController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STCONTROLLER_H__07AC573E_B416_46D1_802D_0221E005A4CB__INCLUDED_)
#define AFX_STCONTROLLER_H__07AC573E_B416_46D1_802D_0221E005A4CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "../Include_ST/ISTController.h"
//#include "../Include_ST/ISTControllerD.h"
#include "../Include_ST/InnerFunction.h"
#include "../Common_ST/DealDataItem.h"

#include "STControllerApp.h"
#include "APDataItem.h"
#include "LogicMaker.h"


class CApQurier;
class CSTDataItem;
class CMemManager;
class CLogicItem;
class CSTController : public CISTController
{
public:	
	virtual ~CSTController();

	// 20060705 함수인자 제거 : , HWND hSocket, CDBMgr* pDBMgr
	//BOOL CreateApQurier(CWnd *pWnd, HWND hSocket, CDBMgr* pDBMgr);
	BOOL CreateApQurier(CWnd *pWnd);

	BOOL Destory();
	void SetReceiver(HWND hReceiver, long lKey);
	short Execute(LPCTSTR lpInputData);
	long Stop();
	long Pause();
	long Update(long lPos, BOOL bIsUpdateData);
	long Command(LPCTSTR lpCommand, LPCTSTR lpOption);
	
	BOOL SetSubInputsFrom(long lIndex, double dValue);
	BOOL SetCurrentPos(long lPos);
	BOOL IfCondition(BOOL bIfConditon);
	void SubIn(long lIndex);
	void SubOut();
	void IfIn();
	void IfOut();
	void ForIn();
	void ForOut();
	void BracketIn();
	void BracketOut();
	BOOL DrawGraph(BOOL bIsInRealTime);
	BOOL PrepareGraph();
	void SetPath(LPCTSTR lpPath);
	void SetChartItem(long pChartItem);//KHD
	long GetChartItem() { return m_pChartItem; }

	// Data Info
	long BarIndex();
	BOOL IsFirstInThisMem();
	long GetCntOfAllApData() { return m_lCntOfAllApData; }
	BOOL IsUpdateData() { return m_bIsUpdateData; }

	// raw Data
	double GetRawData(long lDataIndex, double dReverseIndex);
	CString GetRawSData(long lDataIndex, double dReverseIndex);
	CIDataItem* GetRawData(long lDataIndex);
	CISDataItem* GetRawSData(long lDataIndex);

	// Temp Buffer
	void SetValueInBuffer(long lIndex,double dValue);
	double GetValueInBuffer(long lIndex);

	/// Function Data
	void SetFuncData(double dIndex, double dValue);
	double GetFuncData(double dIndex);

	/// Inputs
	BOOL SetInputs(long lIndex, LPCTSTR lpName, int nType, double dInitValue);
	void SetInputHolderData(long lHolderIndex,double dIndex, double dValue);
	double GetInputHolderData(long lHolderIndex, double dIndex);
	double GetInputDataInRoot(LPCTSTR lpName, double dIndex);

	// Variables
	BOOL SetVariables(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue);
	void SetVarHolderData(long lHolderIndex,double dIndex, double dValue);	
	double GetVarHolderData(long lHolderIndex, double dIndex);
	CIDataItem* GetVarHolderDataItem(long lHolderIndex);
	
	// Array
	BOOL SetArray(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue, long lArrayCnt);
	double GetArrayHolderData(long lHolderIndex, double dIndex);
	void SetArrayHolderData(long lHolderIndex,double dIndex, double dValue);	
	
	// Outer Function
	BOOL DeclareSubFunc(long lHolderIndex, LPCTSTR lpFuncName);
	double SubFunc(long lSTActor, long lIndexFunc, long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 );
	
	// Inner Function
	BOOL DeclareInnerFunc(long lType, long lHolderIndex, LPCTSTR lpFuncName);
	double InnerFunc(long lType, long lIndexFunc,long lIndex, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);

	void SetConfig(){}
	void SetConfigOnWizard(CRect* pRect){}
	void SetUserID(LPCTSTR lpUserID);
	void SetPassword(LPCTSTR lpPassword);
	void SetExpiredDate(long lExpriedDate);
	long GetSourceType();
	void SetSourceType(long lSourceType);
	BOOL GetUseUserID();
	BOOL GetUsePassword();
	BOOL GetUseExpiredDate();
	void SetUseUserID(BOOL bUse);
	void SetUsePassword(BOOL bUse);
	void SetUseExpiredDate(BOOL bUse);
	void SetSTName(LPCTSTR lpSTName, int nSTPos);
	void SetAdditonalOption(LPCTSTR lpAdditionalOption);	
	void SetpStringArrayInputs(CStringArray *pStringArrayInputs);
	
	LPCTSTR GetError();
	void ResetLastSignal_ForReal(long lCurrentPos);

	// Bull Bear
	BOOL SetBullBearDataHolder(LPCTSTR lpName);
	double GetBullBearData(double dIndex);
	void SetBullBearData(double dIndex, double dValue);
	BOOL IsHoldingBullBearAreaData() { return (BOOL)m_pDataItemBullBearArea;}
	void AddCommentAtBullBearData(LPCTSTR lpTitle,LPCTSTR lpComment);
	CString GetCommentAtBullBearData(LPCTSTR lpTitle);

	// PlotFunc
	BOOL SetPlotDataHolder(long lHolderIndex, LPCTSTR lpName);
	double GetPlotData(long lHolderIndex, double dIndex);
	void SetPlotData(long lHolderIndex,double dIndex, double dValue);	
	void AddCommentAtPlotData(long lHolderIndex,LPCTSTR lpTitle,LPCTSTR lpComment);
	CString GetCommentAtPlotData(long lHolderIndex,LPCTSTR lpTitle);
	CIDataItem* GetPlotHolderDataItem(long lHolderIndex);

	// Deal Position
	void SetDealPosData(double dValue, long lType, LPCTSTR lpDealName, double dContracts, double dPrice, LPCTSTR lpExitEntryName);	
	void AddCommentAtDealPosData(LPCTSTR lpTitle,LPCTSTR lpComment);
	CString GetCommentAtDealPosData(LPCTSTR lpTitle);

	// TempMem
	CITempMemMng *GetITempMemMng();


protected:	
	CSTController() {}
public:
	CSTController(CSTControllerApp *pTheApp);

	long GetExpiredDate();

	CIDataItem* GetInputHolderDataItem(long lHolderIndex);
	CString GetInputs(long lIndex);
	double GetInputData(LPCTSTR lpName, double dIndex);

	CIDataItem* GetArrayHolderDataItem(long lHolderIndex);
	
	CString GetDeclaredInnerFuncInfo() { return m_strDeclaredInnerFuncInfo;}

	// Config
	void WriteConfig(){}
	void DeleteConfig() {}
	CString GetPassword();
	CString GetUserID();
	DWORD GetPrivateProfileStringInST(
		  LPCTSTR lpAppName
		, LPCTSTR lpKeyName
		, LPCTSTR lpDefault
		, LPTSTR lpReturnedString
		, DWORD nSize);

	virtual BOOL OnReceiveQuoteData();
	void ClearMemManager();
	void InitMemManager();	
	BOOL OnReceiveQuoteDataInRealTime(long lPos, BOOL bIsUpdateData);
	void SetSizeDataItem(long lSize);
	CString GetSfgPath() {return m_strSfgFile;}
	CString GetSTNamePath() { return m_strSTName;}
	int GetSTPos() { return m_nSTPos;}
	
	CString GetPath() { return m_strPath;}
	
protected:	
	BOOL RegisterItemIntoLogicMaker();


	BOOL	m_bIsExternaInputData;
		
	long m_lCntOfAllApData;
	long m_lCurrentPos;
	long m_lCurrentBar;
	CApQurier* m_pAPQurier;
	CAPDataItem* m_pAPDataItem;
	CMemManager* m_pRootMemManager;
//	CMemManager m_RootMemManager;
	CMemManager* m_pCurrentPosMemManager;


	BOOL m_bIsUpdateData;
	long m_lExpiredDate;
	long m_lSourceType;
	CString m_strUserID;
	CString m_strPassword;
	BOOL m_bUseUserID;
	BOOL m_bUsePassword;
	BOOL m_bUseExpiredDate;
	CString m_strSTName;
	int m_nSTPos;

	ST_DRAWINFO m_drawInfo;
	CPtrArray m_ptrArrayPlot;
	CPtrArray m_ptrArraySDataItem;
	CPtrArray m_ptrArrayHistoryDataItem;
	CSTDataItem *m_pDataItemPlot[MAXCNTOFPLOTHOLDER];
	CLogicItem* m_pLogicItemInput[MAXCNTOFINPUTHOLDER];
	CStringArray *m_pStringArrayInputs;
	CStringArray m_StringArrayInputsFromExternal;

//	CSTDataItem *m_pDataItemDealPosition;// Buy, Sell, ExitLong, ExitShort
//	CSTDataItem m_DataItemExitDealContract;
//	CSDataItem m_SDataItemDealName;
	CDealDataItem m_dealDataItem;
	CSTDataItem *m_pDataItemBullBearArea;// PlotBull, PlotBear
	HWND m_hReceiver;
	long m_lKey;
	HWND m_hParent;
	CString m_strPath;
	CString m_strSubFuncPath;
	CSTControllerApp *m_pTheApp;
	CString m_strSfgFile;
	CString m_strDeclaredInnerFuncInfo;

	CLogicMaker m_logicMaker;
	BOOL	m_bOperate;	// flag become true , when this is excuted first.
	BOOL	m_bProcess;	// flag is used, when this is executed or stoped.
	long	m_lLastDealPosition;
	long	m_lLastDealValue;


	double m_ValueInInnerFunc;
	CString m_strError;

	long m_pChartItem;//KHD
};

#endif // !defined(AFX_STCONTROLLER_H__07AC573E_B416_46D1_802D_0221E005A4CB__INCLUDED_)
