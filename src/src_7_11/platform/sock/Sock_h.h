

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Mar 23 15:53:58 2017
 */
/* Compiler settings for Sock.odl:
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


#ifndef __Sock_h_h__
#define __Sock_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DSock_FWD_DEFINED__
#define ___DSock_FWD_DEFINED__
typedef interface _DSock _DSock;
#endif 	/* ___DSock_FWD_DEFINED__ */


#ifndef ___DSockEvents_FWD_DEFINED__
#define ___DSockEvents_FWD_DEFINED__
typedef interface _DSockEvents _DSockEvents;
#endif 	/* ___DSockEvents_FWD_DEFINED__ */


#ifndef __Sock_FWD_DEFINED__
#define __Sock_FWD_DEFINED__

#ifdef __cplusplus
typedef class Sock Sock;
#else
typedef struct Sock Sock;
#endif /* __cplusplus */

#endif 	/* __Sock_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __SOCKLib_LIBRARY_DEFINED__
#define __SOCKLib_LIBRARY_DEFINED__

/* library SOCKLib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_SOCKLib,0x4240D683,0x6BA4,0x4626,0xB0,0xD9,0x27,0xE1,0x3A,0xCB,0xE9,0x44);

#ifndef ___DSock_DISPINTERFACE_DEFINED__
#define ___DSock_DISPINTERFACE_DEFINED__

/* dispinterface _DSock */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DSock,0x425B9540,0x5811,0x4FF2,0xB0,0x54,0x41,0x7D,0x84,0x08,0xBA,0xA5);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("425B9540-5811-4FF2-B054-417D8408BAA5")
    _DSock : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSockVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSock * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSock * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSock * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSock * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSock * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSock * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSock * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSockVtbl;

    interface _DSock
    {
        CONST_VTBL struct _DSockVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSock_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSock_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSock_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSock_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSock_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSock_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSock_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSock_DISPINTERFACE_DEFINED__ */


#ifndef ___DSockEvents_DISPINTERFACE_DEFINED__
#define ___DSockEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DSockEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DSockEvents,0xDF8B333E,0x3E0B,0x4CCD,0x94,0x0D,0x32,0x8E,0x22,0xD4,0x19,0xF4);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DF8B333E-3E0B-4CCD-940D-328E22D419F4")
    _DSockEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DSockEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DSockEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DSockEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DSockEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DSockEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DSockEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DSockEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DSockEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DSockEventsVtbl;

    interface _DSockEvents
    {
        CONST_VTBL struct _DSockEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DSockEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DSockEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DSockEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DSockEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DSockEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DSockEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DSockEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DSockEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Sock,0xC8D8A27C,0xB3E4,0x4998,0x97,0xB6,0x2E,0xFF,0x95,0x67,0xB3,0xF9);

#ifdef __cplusplus

class DECLSPEC_UUID("C8D8A27C-B3E4-4998-97B6-2EFF9567B3F9")
Sock;
#endif
#endif /* __SOCKLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


