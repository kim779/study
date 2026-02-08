#if !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)
#define AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_

// #include "../include/DllLoadDef.h"
#include "./WndObj.h"
//////////////////////////////////////////////////////////////////////////
// fnInit

typedef struct {
	//	char	m_aParam[MAXDLLPARAM][1024+1];	// 파라미터
	long	m_dwReserved1;
} STODINIT_IN;
#define		SZSTODINIT_IN		sizeof(STODINIT_IN)
#define		INIT_STODINIT_IN(pst)		pst->m_dwReserved1=NULL;


typedef struct {
	void		*m_pDllKey;
	IPlatformObj *m_pPlatformObj;
} STODINIT_OUT;
#define		SZSTODINIT_OUT		sizeof(STODINIT_OUT)
#define		INIT_STODINIT_OUT(pst)		pst->m_pDllKey=NULL;	\
	pst->m_pPlatformObj=NULL;

//////////////////////////////////////////////////////////////////////////
// fnLoad

typedef struct {
	STODINIT_OUT *m_pInitOut;
	HWND	m_hParent;
	CRect	m_Rect;		// 영역(Rect)
	UINT	m_nID;		// 생성할 컨트롤의 ID
} STODCREATEINFO_IN;
#define		SZSTODCREATEINFO_IN		sizeof(STODCREATEINFO_IN)

#define		INIT_STODCREATEINFO_IN(pstIn)		pstIn->m_hParent=NULL;	\
	pstIn->m_nID=0;

typedef struct {
	STODINIT_OUT m_InitOut;

	IAWndObj	*m_pWndObj;		// IAWndObj* 포인트
	// STDLLDISPATCH m_stDllDisp;	// 
	IDispatch*	m_pDispatch;	// 
} STODCREATEINFO_OUT;

#define		INIT_STODCREATEINFO_OUT(pstOut)		pstOut->m_pWndObj = NULL;	\
	pstOut->m_pDispatch = NULL;

//////////////////////////////////////////////////////////////////////////
// Dll Export Function

typedef HRESULT		(WINAPI * FPInit) (STODINIT_IN*, STODINIT_OUT*);
typedef HRESULT		(WINAPI * FPLoad) (STODCREATEINFO_IN*, STODCREATEINFO_OUT*);
typedef HRESULT		(WINAPI * FPUnLoad) (void* dwDllKey);
typedef HRESULT		(WINAPI * FPFormResize) (void* dwDllKey, int w, int h);
typedef HRESULT		(WINAPI * FPShowTabPage) (int bPage);
typedef HRESULT		(WINAPI * FPSetData)(void* dwDllKey, int nType, long wData, long lData);

//////////////////////////////////////////////////////////////////////////

#ifndef _SafeDelete_
#define		SafeDelete(xxVal)				if(xxVal) delete xxVal; xxVal=NULL
#endif

#define FN_SET_RESIZE 1 // 고정 0, 유동 1 : 컴포넌트 리사이징 유무 

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_DLLLOADSTRUCT_H__689FC5AC_9596_11D2_AC26_00104B9E52FF__INCLUDED_)

