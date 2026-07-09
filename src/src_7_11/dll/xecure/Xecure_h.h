

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun May 07 12:04:16 2017
 */
/* Compiler settings for Xecure.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __Xecure_h_h__
#define __Xecure_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DXecure_FWD_DEFINED__
#define ___DXecure_FWD_DEFINED__
typedef interface _DXecure _DXecure;
#endif 	/* ___DXecure_FWD_DEFINED__ */


#ifndef ___DXecureEvents_FWD_DEFINED__
#define ___DXecureEvents_FWD_DEFINED__
typedef interface _DXecureEvents _DXecureEvents;
#endif 	/* ___DXecureEvents_FWD_DEFINED__ */


#ifndef __Xecure_FWD_DEFINED__
#define __Xecure_FWD_DEFINED__

#ifdef __cplusplus
typedef class Xecure Xecure;
#else
typedef struct Xecure Xecure;
#endif /* __cplusplus */

#endif 	/* __Xecure_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __XECURELib_LIBRARY_DEFINED__
#define __XECURELib_LIBRARY_DEFINED__

/* library XECURELib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_XECURELib,0x83E9D393,0xFA66,0x4DAD,0x83,0xD0,0x6C,0x80,0x53,0x86,0x1A,0x70);

#ifndef ___DXecure_DISPINTERFACE_DEFINED__
#define ___DXecure_DISPINTERFACE_DEFINED__

/* dispinterface _DXecure */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DXecure,0x66AC43F4,0x2CED,0x46F1,0xB6,0x47,0x8D,0x05,0xE7,0xB6,0x6B,0x2F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("66AC43F4-2CED-46F1-B647-8D05E7B66B2F")
    _DXecure : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DXecureVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DXecure * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DXecure * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DXecure * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DXecure * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DXecure * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DXecure * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DXecure * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DXecureVtbl;

    interface _DXecure
    {
        CONST_VTBL struct _DXecureVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DXecure_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DXecure_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DXecure_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DXecure_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DXecure_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DXecure_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DXecure_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DXecure_DISPINTERFACE_DEFINED__ */


#ifndef ___DXecureEvents_DISPINTERFACE_DEFINED__
#define ___DXecureEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DXecureEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DXecureEvents,0x66AC43F5,0x2CED,0x46F1,0xB6,0x47,0x8D,0x05,0xE7,0xB6,0x6B,0x2F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("66AC43F5-2CED-46F1-B647-8D05E7B66B2F")
    _DXecureEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DXecureEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DXecureEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DXecureEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DXecureEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DXecureEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DXecureEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DXecureEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DXecureEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DXecureEventsVtbl;

    interface _DXecureEvents
    {
        CONST_VTBL struct _DXecureEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DXecureEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DXecureEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DXecureEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DXecureEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DXecureEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DXecureEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DXecureEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DXecureEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Xecure,0x66AC43F3,0x2CED,0x46F1,0xB6,0x47,0x8D,0x05,0xE7,0xB6,0x6B,0x2F);

#ifdef __cplusplus

class DECLSPEC_UUID("66AC43F3-2CED-46F1-B647-8D05E7B66B2F")
Xecure;
#endif
#endif /* __XECURELib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


