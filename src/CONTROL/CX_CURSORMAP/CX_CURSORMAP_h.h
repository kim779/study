

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_CURSORMAP.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __CX_CURSORMAP_h_h__
#define __CX_CURSORMAP_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DEXTCTRL_FWD_DEFINED__
#define ___DEXTCTRL_FWD_DEFINED__
typedef interface _DEXTCTRL _DEXTCTRL;

#endif 	/* ___DEXTCTRL_FWD_DEFINED__ */


#ifndef ___DEXTCTRLEvents_FWD_DEFINED__
#define ___DEXTCTRLEvents_FWD_DEFINED__
typedef interface _DEXTCTRLEvents _DEXTCTRLEvents;

#endif 	/* ___DEXTCTRLEvents_FWD_DEFINED__ */


#ifndef __CX_CURSORMAP_FWD_DEFINED__
#define __CX_CURSORMAP_FWD_DEFINED__

#ifdef __cplusplus
typedef class CX_CURSORMAP CX_CURSORMAP;
#else
typedef struct CX_CURSORMAP CX_CURSORMAP;
#endif /* __cplusplus */

#endif 	/* __CX_CURSORMAP_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_CURSORMAPLib_LIBRARY_DEFINED__
#define __CX_CURSORMAPLib_LIBRARY_DEFINED__

/* library CX_CURSORMAPLib */
/* [control][helpstring][helpfile][version][uuid] */ 

#pragma external_header(push)
#pragma external_header(pop)
#pragma external_header(push)
#pragma external_header(push)
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma external_header(push)
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)
#pragma external_header(pop)
#pragma warning(pop)
#pragma external_header(pop)
#pragma region Desktop Family
#pragma endregion
#pragma external_header(pop)

DEFINE_GUID(LIBID_CX_CURSORMAPLib,0x24408BC4,0xDEF4,0x4AF8,0x83,0xDB,0xCB,0x36,0x6E,0x86,0x69,0x24);

#ifndef ___DEXTCTRL_DISPINTERFACE_DEFINED__
#define ___DEXTCTRL_DISPINTERFACE_DEFINED__

/* dispinterface _DEXTCTRL */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DEXTCTRL,0x5ADC603A,0xF412,0x41B4,0x99,0x64,0xF4,0xBD,0x0A,0x15,0x3C,0x92);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5ADC603A-F412-41B4-9964-F4BD0A153C92")
    _DEXTCTRL : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DEXTCTRLVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DEXTCTRL * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DEXTCTRL * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DEXTCTRL * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DEXTCTRL * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DEXTCTRL * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DEXTCTRL * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DEXTCTRL * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DEXTCTRLVtbl;

    interface _DEXTCTRL
    {
        CONST_VTBL struct _DEXTCTRLVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DEXTCTRL_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DEXTCTRL_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DEXTCTRL_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DEXTCTRL_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DEXTCTRL_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DEXTCTRL_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DEXTCTRL_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DEXTCTRL_DISPINTERFACE_DEFINED__ */


#ifndef ___DEXTCTRLEvents_DISPINTERFACE_DEFINED__
#define ___DEXTCTRLEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DEXTCTRLEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DEXTCTRLEvents,0xC324A902,0x68A9,0x42BE,0x94,0x2C,0x7E,0x66,0x8F,0x1D,0x32,0x5E);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C324A902-68A9-42BE-942C-7E668F1D325E")
    _DEXTCTRLEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DEXTCTRLEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DEXTCTRLEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DEXTCTRLEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DEXTCTRLEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DEXTCTRLEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DEXTCTRLEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DEXTCTRLEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DEXTCTRLEvents * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _DEXTCTRLEventsVtbl;

    interface _DEXTCTRLEvents
    {
        CONST_VTBL struct _DEXTCTRLEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DEXTCTRLEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DEXTCTRLEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DEXTCTRLEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DEXTCTRLEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DEXTCTRLEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DEXTCTRLEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DEXTCTRLEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DEXTCTRLEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_CX_CURSORMAP,0xAA0F54A8,0x970E,0x451A,0x97,0x3E,0xB7,0x4D,0x98,0xF9,0xCF,0x16);

#ifdef __cplusplus

class DECLSPEC_UUID("AA0F54A8-970E-451A-973E-B74D98F9CF16")
CX_CURSORMAP;
#endif
#endif /* __CX_CURSORMAPLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


