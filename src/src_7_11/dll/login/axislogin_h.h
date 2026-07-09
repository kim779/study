

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jul 06 09:17:12 2017
 */
/* Compiler settings for axislogin.odl:
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


#ifndef __axislogin_h_h__
#define __axislogin_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILogin_FWD_DEFINED__
#define __ILogin_FWD_DEFINED__
typedef interface ILogin ILogin;
#endif 	/* __ILogin_FWD_DEFINED__ */


#ifndef __Login_FWD_DEFINED__
#define __Login_FWD_DEFINED__

#ifdef __cplusplus
typedef class Login Login;
#else
typedef struct Login Login;
#endif /* __cplusplus */

#endif 	/* __Login_FWD_DEFINED__ */


#ifndef __ILedger_FWD_DEFINED__
#define __ILedger_FWD_DEFINED__
typedef interface ILedger ILedger;
#endif 	/* __ILedger_FWD_DEFINED__ */


#ifndef __Ledger_FWD_DEFINED__
#define __Ledger_FWD_DEFINED__

#ifdef __cplusplus
typedef class Ledger Ledger;
#else
typedef struct Ledger Ledger;
#endif /* __cplusplus */

#endif 	/* __Ledger_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Axislogin_LIBRARY_DEFINED__
#define __Axislogin_LIBRARY_DEFINED__

/* library Axislogin */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Axislogin,0x808A531C,0xB00D,0x4873,0xB6,0xC5,0x3D,0x68,0x06,0xA1,0xC7,0x60);

#ifndef __ILogin_DISPINTERFACE_DEFINED__
#define __ILogin_DISPINTERFACE_DEFINED__

/* dispinterface ILogin */
/* [uuid] */ 


DEFINE_GUID(DIID_ILogin,0xDEE758DA,0x505A,0x48FF,0x87,0xCD,0x4E,0x4C,0xD8,0x22,0x66,0xA8);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DEE758DA-505A-48FF-87CD-4E4CD82266A8")
    ILogin : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ILoginVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogin * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILogin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILogin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILogin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILogin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ILoginVtbl;

    interface ILogin
    {
        CONST_VTBL struct ILoginVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogin_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogin_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogin_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogin_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILogin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILogin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILogin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ILogin_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Login,0x9ED68054,0xA19B,0x4DA7,0x93,0xBC,0x72,0x36,0xC9,0xA9,0x3A,0x82);

#ifdef __cplusplus

class DECLSPEC_UUID("9ED68054-A19B-4DA7-93BC-7236C9A93A82")
Login;
#endif

#ifndef __ILedger_DISPINTERFACE_DEFINED__
#define __ILedger_DISPINTERFACE_DEFINED__

/* dispinterface ILedger */
/* [uuid] */ 


DEFINE_GUID(DIID_ILedger,0x3BF10F40,0xFDE9,0x4A44,0xBA,0x55,0x56,0xDE,0x5F,0xAE,0xBA,0xC9);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3BF10F40-FDE9-4A44-BA55-56DE5FAEBAC9")
    ILedger : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ILedgerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILedger * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILedger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILedger * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILedger * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILedger * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILedger * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILedger * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ILedgerVtbl;

    interface ILedger
    {
        CONST_VTBL struct ILedgerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILedger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILedger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILedger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILedger_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILedger_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILedger_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILedger_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ILedger_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Ledger,0x11ECD400,0x3667,0x4266,0x9C,0xE7,0xB1,0xB2,0x0A,0xB0,0xA2,0x7D);

#ifdef __cplusplus

class DECLSPEC_UUID("11ECD400-3667-4266-9CE7-B1B20AB0A27D")
Ledger;
#endif
#endif /* __Axislogin_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


