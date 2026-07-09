

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jul 06 09:17:12 2017
 */
/* Compiler settings for fx_codectrl.idl:
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


#ifndef __fx_codectrl_h_h__
#define __fx_codectrl_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IfxCodeCtrl_FWD_DEFINED__
#define __IfxCodeCtrl_FWD_DEFINED__
typedef interface IfxCodeCtrl IfxCodeCtrl;
#endif 	/* __IfxCodeCtrl_FWD_DEFINED__ */


#ifndef __fxCodeCtrl_FWD_DEFINED__
#define __fxCodeCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class fxCodeCtrl fxCodeCtrl;
#else
typedef struct fxCodeCtrl fxCodeCtrl;
#endif /* __cplusplus */

#endif 	/* __fxCodeCtrl_FWD_DEFINED__ */


#ifndef __IControlwnd_FWD_DEFINED__
#define __IControlwnd_FWD_DEFINED__
typedef interface IControlwnd IControlwnd;
#endif 	/* __IControlwnd_FWD_DEFINED__ */


#ifndef __Controlwnd_FWD_DEFINED__
#define __Controlwnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class Controlwnd Controlwnd;
#else
typedef struct Controlwnd Controlwnd;
#endif /* __cplusplus */

#endif 	/* __Controlwnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __fx_codectrl_LIBRARY_DEFINED__
#define __fx_codectrl_LIBRARY_DEFINED__

/* library fx_codectrl */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_fx_codectrl;

#ifndef __IfxCodeCtrl_DISPINTERFACE_DEFINED__
#define __IfxCodeCtrl_DISPINTERFACE_DEFINED__

/* dispinterface IfxCodeCtrl */
/* [uuid] */ 


EXTERN_C const IID DIID_IfxCodeCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("656726DE-5FD2-4929-8F97-F57464D0AD50")
    IfxCodeCtrl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfxCodeCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfxCodeCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfxCodeCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfxCodeCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfxCodeCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfxCodeCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfxCodeCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfxCodeCtrl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IfxCodeCtrlVtbl;

    interface IfxCodeCtrl
    {
        CONST_VTBL struct IfxCodeCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfxCodeCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfxCodeCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfxCodeCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfxCodeCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfxCodeCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfxCodeCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfxCodeCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfxCodeCtrl_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_fxCodeCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("779AEC8F-2D8A-4FD9-B5CA-E9B01B4DA1BE")
fxCodeCtrl;
#endif

#ifndef __IControlwnd_DISPINTERFACE_DEFINED__
#define __IControlwnd_DISPINTERFACE_DEFINED__

/* dispinterface IControlwnd */
/* [uuid] */ 


EXTERN_C const IID DIID_IControlwnd;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3EBBD33B-2F57-4ACA-8C33-D82219DCE4F4")
    IControlwnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IControlwndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IControlwnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IControlwnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IControlwnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IControlwnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IControlwnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IControlwnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IControlwnd * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IControlwndVtbl;

    interface IControlwnd
    {
        CONST_VTBL struct IControlwndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IControlwnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IControlwnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IControlwnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IControlwnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IControlwnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IControlwnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IControlwnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IControlwnd_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Controlwnd;

#ifdef __cplusplus

class DECLSPEC_UUID("765AC45F-31E4-43E4-A1C7-33CF26C740F2")
Controlwnd;
#endif
#endif /* __fx_codectrl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


