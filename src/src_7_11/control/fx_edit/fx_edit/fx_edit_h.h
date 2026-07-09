

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jul 06 09:17:28 2017
 */
/* Compiler settings for fx_edit.idl:
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


#ifndef __fx_edit_h_h__
#define __fx_edit_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

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



#ifndef __fx_edit_LIBRARY_DEFINED__
#define __fx_edit_LIBRARY_DEFINED__

/* library fx_edit */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_fx_edit;

#ifndef __IControlwnd_DISPINTERFACE_DEFINED__
#define __IControlwnd_DISPINTERFACE_DEFINED__

/* dispinterface IControlwnd */
/* [uuid] */ 


EXTERN_C const IID DIID_IControlwnd;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7733226F-155D-48E3-88D0-8CBBF5C5DECD")
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

class DECLSPEC_UUID("A6C84D7A-D570-45CD-94FF-22378904E088")
Controlwnd;
#endif
#endif /* __fx_edit_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


