

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Certify.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
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


#ifndef __Certifyidl_h__
#define __Certifyidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if _CONTROL_FLOW_GUARD_XFG
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef ___DCertify_FWD_DEFINED__
#define ___DCertify_FWD_DEFINED__
typedef interface _DCertify _DCertify;

#endif 	/* ___DCertify_FWD_DEFINED__ */


#ifndef ___DCertifyEvents_FWD_DEFINED__
#define ___DCertifyEvents_FWD_DEFINED__
typedef interface _DCertifyEvents _DCertifyEvents;

#endif 	/* ___DCertifyEvents_FWD_DEFINED__ */


#ifndef __Certify_FWD_DEFINED__
#define __Certify_FWD_DEFINED__

#ifdef __cplusplus
typedef class Certify Certify;
#else
typedef struct Certify Certify;
#endif /* __cplusplus */

#endif 	/* __Certify_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_Certify_0000_0000 */
/* [local] */ 

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


extern RPC_IF_HANDLE __MIDL_itf_Certify_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Certify_0000_0000_v0_0_s_ifspec;


#ifndef __CertifyLib_LIBRARY_DEFINED__
#define __CertifyLib_LIBRARY_DEFINED__

/* library CertifyLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_CertifyLib;

#ifndef ___DCertify_DISPINTERFACE_DEFINED__
#define ___DCertify_DISPINTERFACE_DEFINED__

/* dispinterface _DCertify */
/* [uuid] */ 


EXTERN_C const IID DIID__DCertify;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("45b73aba-cdd7-4cb0-a9f3-d5386e3cfacd")
    _DCertify : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCertifyVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCertify * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCertify * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCertify * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCertify * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCertify * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCertify * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCertify * This,
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
    } _DCertifyVtbl;

    interface _DCertify
    {
        CONST_VTBL struct _DCertifyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCertify_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCertify_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCertify_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCertify_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCertify_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCertify_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCertify_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCertify_DISPINTERFACE_DEFINED__ */


#ifndef ___DCertifyEvents_DISPINTERFACE_DEFINED__
#define ___DCertifyEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DCertifyEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DCertifyEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("605e9aa3-dfb7-4ab1-bd82-cda32da4f50a")
    _DCertifyEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DCertifyEventsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DCertifyEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DCertifyEvents * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DCertifyEvents * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DCertifyEvents * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DCertifyEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DCertifyEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DCertifyEvents * This,
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
    } _DCertifyEventsVtbl;

    interface _DCertifyEvents
    {
        CONST_VTBL struct _DCertifyEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DCertifyEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DCertifyEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DCertifyEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DCertifyEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DCertifyEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DCertifyEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DCertifyEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DCertifyEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Certify;

#ifdef __cplusplus

class DECLSPEC_UUID("3277f0b4-b8be-42aa-9b58-2fa88886ac64")
Certify;
#endif
#endif /* __CertifyLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


