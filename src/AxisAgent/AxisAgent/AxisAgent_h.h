

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for AxisAgent.idl:
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


#ifndef __AxisAgent_h_h__
#define __AxisAgent_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAxisAgent_FWD_DEFINED__
#define __IAxisAgent_FWD_DEFINED__
typedef interface IAxisAgent IAxisAgent;

#endif 	/* __IAxisAgent_FWD_DEFINED__ */


#ifndef __AxisAgent_FWD_DEFINED__
#define __AxisAgent_FWD_DEFINED__

#ifdef __cplusplus
typedef class AxisAgent AxisAgent;
#else
typedef struct AxisAgent AxisAgent;
#endif /* __cplusplus */

#endif 	/* __AxisAgent_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __AxisAgent_LIBRARY_DEFINED__
#define __AxisAgent_LIBRARY_DEFINED__

/* library AxisAgent */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_AxisAgent;

#ifndef __IAxisAgent_DISPINTERFACE_DEFINED__
#define __IAxisAgent_DISPINTERFACE_DEFINED__

/* dispinterface IAxisAgent */
/* [uuid] */ 


EXTERN_C const IID DIID_IAxisAgent;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("dfd75f25-bcd8-4c1f-b686-b305d73fba67")
    IAxisAgent : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IAxisAgentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAxisAgent * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAxisAgent * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAxisAgent * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAxisAgent * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAxisAgent * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAxisAgent * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAxisAgent * This,
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
    } IAxisAgentVtbl;

    interface IAxisAgent
    {
        CONST_VTBL struct IAxisAgentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAxisAgent_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAxisAgent_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAxisAgent_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAxisAgent_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAxisAgent_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAxisAgent_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAxisAgent_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IAxisAgent_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_AxisAgent;

#ifdef __cplusplus

class DECLSPEC_UUID("f67dc37b-5139-450a-9a14-d0bad2e406bf")
AxisAgent;
#endif
#endif /* __AxisAgent_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


