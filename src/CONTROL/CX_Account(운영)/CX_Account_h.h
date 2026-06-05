

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_Account.odl:
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


#ifndef __CX_Account_h_h__
#define __CX_Account_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAccountCtrl_FWD_DEFINED__
#define __IAccountCtrl_FWD_DEFINED__
typedef interface IAccountCtrl IAccountCtrl;

#endif 	/* __IAccountCtrl_FWD_DEFINED__ */


#ifndef __CAccountCtrl_FWD_DEFINED__
#define __CAccountCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class CAccountCtrl CAccountCtrl;
#else
typedef struct CAccountCtrl CAccountCtrl;
#endif /* __cplusplus */

#endif 	/* __CAccountCtrl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_Account_LIBRARY_DEFINED__
#define __CX_Account_LIBRARY_DEFINED__

/* library CX_Account */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_Account,0x4C9186A1,0x82C8,0x4EA6,0xB1,0x05,0xEA,0xB3,0x1A,0x23,0xA2,0xE6);

#ifndef __IAccountCtrl_DISPINTERFACE_DEFINED__
#define __IAccountCtrl_DISPINTERFACE_DEFINED__

/* dispinterface IAccountCtrl */
/* [uuid] */ 


DEFINE_GUID(DIID_IAccountCtrl,0x06ED8B27,0x3CFC,0x4A5E,0xB6,0xD9,0x52,0x6F,0xAD,0xC7,0x18,0xA7);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("06ED8B27-3CFC-4A5E-B6D9-526FADC718A7")
    IAccountCtrl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IAccountCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAccountCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAccountCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAccountCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAccountCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAccountCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAccountCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAccountCtrl * This,
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
    } IAccountCtrlVtbl;

    interface IAccountCtrl
    {
        CONST_VTBL struct IAccountCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAccountCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAccountCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAccountCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAccountCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAccountCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAccountCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAccountCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IAccountCtrl_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_CAccountCtrl,0x4BC93945,0xB263,0x45d1,0xA9,0x3D,0x64,0xCF,0x84,0x73,0x81,0x0D);

#ifdef __cplusplus

class DECLSPEC_UUID("4BC93945-B263-45d1-A93D-64CF8473810D")
CAccountCtrl;
#endif
#endif /* __CX_Account_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


