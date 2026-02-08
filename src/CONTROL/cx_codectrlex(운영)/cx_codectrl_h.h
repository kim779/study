

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for cx_codectrl.odl:
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


#ifndef __cx_codectrl_h_h__
#define __cx_codectrl_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IControlWnd_FWD_DEFINED__
#define __IControlWnd_FWD_DEFINED__
typedef interface IControlWnd IControlWnd;

#endif 	/* __IControlWnd_FWD_DEFINED__ */


#ifndef __ControlWnd_FWD_DEFINED__
#define __ControlWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class ControlWnd ControlWnd;
#else
typedef struct ControlWnd ControlWnd;
#endif /* __cplusplus */

#endif 	/* __ControlWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __cx_codectrl_LIBRARY_DEFINED__
#define __cx_codectrl_LIBRARY_DEFINED__

/* library cx_codectrl */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_cx_codectrl,0x6D401289,0x737F,0x4155,0x9D,0xA9,0x32,0xF2,0xB5,0x50,0x91,0x61);

#ifndef __IControlWnd_DISPINTERFACE_DEFINED__
#define __IControlWnd_DISPINTERFACE_DEFINED__

/* dispinterface IControlWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IControlWnd,0x794B73A4,0x2083,0x426A,0x8F,0x47,0xA8,0x97,0xE6,0xF9,0x83,0x09);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("794B73A4-2083-426A-8F47-A897E6F98309")
    IControlWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IControlWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IControlWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IControlWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IControlWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IControlWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IControlWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IControlWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IControlWnd * This,
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
    } IControlWndVtbl;

    interface IControlWnd
    {
        CONST_VTBL struct IControlWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IControlWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IControlWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IControlWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IControlWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IControlWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IControlWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IControlWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IControlWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_ControlWnd,0x0ABFCF1D,0x152E,0x426B,0xB9,0x20,0xED,0x48,0x76,0xB0,0x89,0x28);

#ifdef __cplusplus

class DECLSPEC_UUID("0ABFCF1D-152E-426B-B920-ED4876B08928")
ControlWnd;
#endif
#endif /* __cx_codectrl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


