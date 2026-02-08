#if !defined(AFX_DISP_PRIMLIB__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
#define AFX_DISP_PRIMLIB__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CDispPrimLib wrapper class

class CDispPrimLib : public COleDispatchDriver
{
public:
	CDispPrimLib();

//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable
//		IIx_Rows=0,

		// Method
		IIx_IsUserDefinedMode,
		IIx_ReLoadOtherMap,
		IIx_SetMessage,
		IIx_MapSwitch,
		IIx_InsertSplit,
		IIx_DeleteSplit,
		IIx_TabToggle,
		IIx_DefaultSetUp,
		IIx_SetActiveTab,
		IIx_ChangeInputRangeResize,
		IIx_ToggleWindow,
		IIx_SplitResizeOption,
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);
//}} Dispid자동처리
	
public:
// Attributes

	
// Operations
	short IsUserDefinedMode(LPCTSTR strMode, short nConID) ;
	void ReLoadOtherMap(LPCTSTR szPath) ;
	void SetMessage(LPCTSTR nId) ;
	void MapSwitch(LPCTSTR strLR) ;
	void InsertSplit(LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPublic, LPCTSTR szGroup, LPCTSTR szSameXfmResize = _T("")) ;
	void DeleteSplit(LPCTSTR szPath) ;
	void TabToggle(LPCTSTR strUpDown) ;
	void DefaultSetUp(LPCTSTR szLR, LPCTSTR szUpDown, LPCTSTR szHoga, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPath3, LPCTSTR szExt1, LPCTSTR szExt2) ; 
	void SetActiveTab(LPCTSTR szTabID) ;
	void ChangeInputRangeResize(LPCTSTR szChange) ;
	void ToggleWindow(LPCTSTR strID, LPCTSTR strDIR, LPCTSTR strSUBDIR) ;
	void SplitResizeOption(LPCTSTR strID, LPCTSTR RightDIR, LPCTSTR BottomDIR) ;
};

#endif // !defined(AFX_DISP_PRIMLIB__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)




