

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for IBKSConnector.odl:
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


#ifndef __IBKSConnector_h_h__
#define __IBKSConnector_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DIBKSConnector_FWD_DEFINED__
#define ___DIBKSConnector_FWD_DEFINED__
typedef interface _DIBKSConnector _DIBKSConnector;

#endif 	/* ___DIBKSConnector_FWD_DEFINED__ */


#ifndef ___DIBKSConnectorEvents_FWD_DEFINED__
#define ___DIBKSConnectorEvents_FWD_DEFINED__
typedef interface _DIBKSConnectorEvents _DIBKSConnectorEvents;

#endif 	/* ___DIBKSConnectorEvents_FWD_DEFINED__ */


#ifndef __IBKSConnector_FWD_DEFINED__
#define __IBKSConnector_FWD_DEFINED__

#ifdef __cplusplus
typedef class IBKSConnector IBKSConnector;
#else
typedef struct IBKSConnector IBKSConnector;
#endif /* __cplusplus */

#endif 	/* __IBKSConnector_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IBKSCONNECTORLib_LIBRARY_DEFINED__
#define __IBKSCONNECTORLib_LIBRARY_DEFINED__

/* library IBKSCONNECTORLib */
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

DEFINE_GUID(LIBID_IBKSCONNECTORLib,0x90685967,0x8F5E,0x491A,0x8C,0xCA,0x42,0x20,0x47,0x9A,0xC1,0x89);

#ifndef ___DIBKSConnector_DISPINTERFACE_DEFINED__
#define ___DIBKSConnector_DISPINTERFACE_DEFINED__

/* dispinterface _DIBKSConnector */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DIBKSConnector,0xC66F0D7B,0x235C,0x409A,0x90,0x18,0x05,0x87,0xD1,0x09,0x4C,0x20);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C66F0D7B-235C-409A-9018-0587D1094C20")
    _DIBKSConnector : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIBKSConnectorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIBKSConnector * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIBKSConnector * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIBKSConnector * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIBKSConnector * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIBKSConnector * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIBKSConnector * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIBKSConnector * This,
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
    } _DIBKSConnectorVtbl;

    interface _DIBKSConnector
    {
        CONST_VTBL struct _DIBKSConnectorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIBKSConnector_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DIBKSConnector_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DIBKSConnector_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DIBKSConnector_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DIBKSConnector_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DIBKSConnector_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DIBKSConnector_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIBKSConnector_DISPINTERFACE_DEFINED__ */


#ifndef ___DIBKSConnectorEvents_DISPINTERFACE_DEFINED__
#define ___DIBKSConnectorEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DIBKSConnectorEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DIBKSConnectorEvents,0x55F8C481,0x7091,0x4ACA,0x8E,0x9C,0xA5,0xBA,0x13,0x94,0x43,0x06);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("55F8C481-7091-4ACA-8E9C-A5BA13944306")
    _DIBKSConnectorEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DIBKSConnectorEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DIBKSConnectorEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DIBKSConnectorEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DIBKSConnectorEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DIBKSConnectorEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DIBKSConnectorEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DIBKSConnectorEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DIBKSConnectorEvents * This,
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
    } _DIBKSConnectorEventsVtbl;

    interface _DIBKSConnectorEvents
    {
        CONST_VTBL struct _DIBKSConnectorEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DIBKSConnectorEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DIBKSConnectorEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DIBKSConnectorEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DIBKSConnectorEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DIBKSConnectorEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DIBKSConnectorEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DIBKSConnectorEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DIBKSConnectorEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_IBKSConnector,0xCDADD338,0xC7AB,0x4977,0xB6,0x5D,0x8E,0x98,0x8B,0x59,0x58,0xE3);

#ifdef __cplusplus

class DECLSPEC_UUID("CDADD338-C7AB-4977-B65D-8E988B5958E3")
IBKSConnector;
#endif
#endif /* __IBKSCONNECTORLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


