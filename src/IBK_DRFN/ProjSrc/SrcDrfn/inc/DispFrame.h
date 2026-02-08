#if !defined(AFX_DISP_FRAME__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
#define AFX_DISP_FRAME__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CDispFrame wrapper class

class CDispFrame : public COleDispatchDriver
{
public:
	CDispFrame();

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
		IIx_GetDataSaveType=0,
		IIx_GetFrameInfoFile,
		IIx_GetFontSize,
		IIx_GetFrameScrNum,
		IIx_ChangeFrameSize, 
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);
//}} Dispid자동처리
	
public:
// Attributes

	
// Operations
	short GetDataSaveType() ;
	CString GetFrameInfoFile() ;
	short GetFontSize();
	CString GetFrameScrNum() ;
	void ChangeFrameSize(short nFlag, short nParam1, short nParam2) ;
};

#endif // !defined(AFX_DISP_FRAME__BY_JUNOK_LEE_0C23_4391_BC8C_6051B31F5D27__INCLUDED_)
