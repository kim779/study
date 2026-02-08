// ISTController.h: interface for the ISTController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTCONTROLLERD_H__75B03A52_3933_4AD6_AF22_9F4DB37584F8__INCLUDED_)
#define AFX_ISTCONTROLLERD_H__75B03A52_3933_4AD6_AF22_9F4DB37584F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Common_ST/IDataItem.h"
#include "../Common_ST/ISDataItem.h"
#include "../Common_ST/ITempMemMng.h"

#define UMSG_ENDRECEIVINGFROMSERVER		WM_USER+100	// Finished job about receiving data from server
#define UMSG_ENDREALTIMEDATAFROMSERVER	WM_USER+101	// Finished job about real time data from server
#define UMSG_EXECUTEVIRTUALLY			WM_USER+102	// Set Environment File


#define CISTControllerD CISTControllerD0001

// 20060705 클래스선언 제거
//class CDBMgr;
//STController에 정의 되어 있다.
class CISTControllerD0001  
{
public:
	virtual ~CISTControllerD0001() {};

	// 20060705 함수인자 제거 : ,HWND hSocket, CDBMgr* pDBMgr
	//virtual BOOL CreateApQurier(CWnd *pWnd,HWND hSocket, CDBMgr* pDBMgr) = 0;
	virtual BOOL CreateApQurier(CWnd *pWnd) = 0;

	virtual BOOL Destory() = 0;
	virtual void SetReceiver(HWND hReceiver, long lKey) = 0;
	virtual short Execute(LPCTSTR lpInputData) = 0;
	virtual long Stop() = 0;
	virtual long Pause() = 0;
	virtual long Update(long lPos, BOOL bIsUpdateData) = 0;
	virtual long Command(LPCTSTR lpCommand, LPCTSTR lpOption) = 0;
	virtual CString GetPath() =0;
	virtual CString GetSfgPath() =0;
	

	virtual BOOL SetSubInputsFrom(long lIndex, double dValue) = 0;
	virtual BOOL SetCurrentPos(long lPos) = 0;
	virtual BOOL IfCondition(BOOL bIfConditon) = 0;
	virtual void SubIn(long lIndex) = 0;
	virtual void SubOut() = 0;
	virtual void IfIn() = 0;
	virtual void IfOut() = 0;
	virtual void ForIn() = 0;
	virtual void ForOut() = 0;
	virtual void BracketIn() = 0;
	virtual void BracketOut() = 0;
	virtual BOOL DrawGraph(BOOL bIsInRealTime) = 0;
	virtual void SetPath(LPCTSTR lpPath) = 0;


	// Data Info
	virtual long BarIndex() = 0;			// 봉의 번호. 최초시점 0
	virtual BOOL IsFirstInThisMem() = 0;
	virtual long GetCntOfAllApData() = 0;	
	virtual BOOL IsUpdateData() = 0;

	// Quote Data
	virtual double GetRawData(long lDataIndex, double dReverseIndex) = 0;
	virtual CString GetRawSData(long lDataIndex, double dReverseIndex) = 0;
	virtual CIDataItem* GetRawData(long lDataIndex) = 0;
	virtual CISDataItem* GetRawSData(long lDataIndex) = 0;

	// Temp Buffer
	virtual void SetValueInBuffer(long lIndex,double dValue) = 0;
	virtual double GetValueInBuffer(long lIndex) = 0;

	// Array
	virtual BOOL SetArray(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue, long lArrayCnt) = 0;
	virtual double GetArrayHolderData(long lHolderIndex, double dIndex) = 0;
	virtual void SetArrayHolderData(long lHolderIndex,double dIndex, double dValue) = 0;	
	virtual CIDataItem* GetArrayHolderDataItem(long lHolderIndex) = 0;
	
	/// Function Data
	virtual void SetFuncData(double dIndex, double dValue) = 0;
	virtual double GetFuncData(double dIndex) = 0;

	/// Inputs
	virtual BOOL SetInputs(long lIndex, LPCTSTR lpName, int nType, double dInitValue) = 0;
	virtual void SetInputHolderData(long lHolderIndex,double dIndex, double dValue) = 0;
	virtual double GetInputHolderData(long lHolderIndex, double dIndex) = 0;
	virtual CIDataItem* GetInputHolderDataItem(long lHolderIndex) = 0;
	virtual CString GetInputs(long lIndex) = 0;
	virtual double GetInputData(LPCTSTR lpName, double dIndex) = 0;
	virtual double GetInputDataInRoot(LPCTSTR lpName, double dIndex) = 0;

	// Variables
	virtual BOOL SetVariables(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue) = 0;
	virtual void SetVarHolderData(long lHolderIndex,double dIndex, double dValue) = 0;	
	virtual double GetVarHolderData(long lHolderIndex, double dIndex) = 0;
	virtual CIDataItem* GetVarHolderDataItem(long lHolderIndex) = 0;

	// Outer Function
	virtual BOOL DeclareSubFunc(long lHolderIndex, LPCTSTR lpFuncName) = 0;
	virtual double SubFunc(long lSTActor, long lIndexFunc, long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 ) = 0;

	// Inner Function
	virtual BOOL DeclareInnerFunc(long lType, long lHolderIndex, LPCTSTR lpFuncName) = 0;
	virtual double InnerFunc(long lType, long lIndexFunc, long lIndex, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varAgr10) = 0;
	virtual CString GetDeclaredInnerFuncInfo() = 0;

	// PlotFunc
	virtual BOOL SetPlotDataHolder(long lHolderIndex, LPCTSTR lpName) = 0;
	virtual double GetPlotData(long lHolderIndex, double dIndex) = 0;
	virtual void SetPlotData(long lHolderIndex,double dIndex, double dValue) = 0;	
	virtual void AddCommentAtPlotData(long lHolderIndex,LPCTSTR lpTitle, LPCTSTR lpComment) = 0;
	virtual CString GetCommentAtPlotData(long lHolderIndex,LPCTSTR lpTitle) = 0;
	virtual CIDataItem* GetPlotHolderDataItem(long lHolderIndex) = 0;
	// Config
	virtual void SetConfig() = 0;
	virtual void SetConfigOnWizard(CRect* pRect) = 0;
	virtual void WriteConfig() = 0;
	virtual void DeleteConfig() = 0;
	virtual CString GetUserID() = 0;
	virtual CString GetPassword() = 0;
	virtual long GetExpiredDate() = 0;
	virtual long GetSourceType() = 0;
	virtual BOOL GetUseUserID() = 0;
	virtual BOOL GetUsePassword() = 0;
	virtual BOOL GetUseExpiredDate() = 0;
	virtual CString GetSTNamePath() = 0;
	virtual int GetSTPos() = 0;
	virtual DWORD GetPrivateProfileStringInST(
		  LPCTSTR lpAppName
		, LPCTSTR lpKeyName
		, LPCTSTR lpDefault
		, LPTSTR lpReturnedString
		, DWORD nSize) = 0;

	// Deal Position
	virtual void SetDealPosData(double dValue, long lType, LPCTSTR lpDealName,double dContracts, double dPrice, LPCTSTR lpExitEntryName) = 0;	
	virtual void AddCommentAtDealPosData(LPCTSTR lpTitle,LPCTSTR lpComment) = 0;
	virtual CString GetCommentAtDealPosData(LPCTSTR lpTitle) = 0;

	// Bull Bear
	virtual BOOL SetBullBearDataHolder(LPCTSTR lpName) = 0;
	virtual double GetBullBearData(double dIndex) = 0;
	virtual void SetBullBearData(double dIndex, double dValue) = 0;		
	virtual BOOL IsHoldingBullBearAreaData() = 0;
	virtual void AddCommentAtBullBearData(LPCTSTR lpTitle,LPCTSTR lpComment) = 0;
	virtual CString GetCommentAtBullBearData(LPCTSTR lpTitle) = 0;

	// TempMng
	virtual CITempMemMng *GetITempMemMng() = 0;
	virtual LPCTSTR GetError() = 0;
};

#endif // !defined(AFX_ISTCONTROLLERD_H__75B03A52_3933_4AD6_AF22_9F4DB37584F8__INCLUDED_)
