

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for axislogin.odl:
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


DEFINE_GUID(LIBID_Axislogin,0x74E42E36,0xB966,0x4D43,0x99,0x5A,0xB3,0xC4,0xB9,0x62,0xBA,0xD2);

#ifndef __ILogin_DISPINTERFACE_DEFINED__
#define __ILogin_DISPINTERFACE_DEFINED__

/* dispinterface ILogin */
/* [uuid] */ 


DEFINE_GUID(DIID_ILogin,0xEE69E4E4,0x5BF8,0x4CAD,0x9C,0x8E,0x54,0x25,0x0C,0xF3,0x3A,0x1A);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("EE69E4E4-5BF8-4CAD-9C8E-54250CF33A1A")
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
            _COM_Outptr_  void **ppvObject);
        
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


DEFINE_GUID(CLSID_Login,0x8BC6BE27,0xEB22,0x4221,0x95,0x6C,0x8F,0xA5,0xBB,0x32,0x22,0x49);

#ifdef __cplusplus

class DECLSPEC_UUID("8BC6BE27-EB22-4221-956C-8FA5BB322249")
Login;
#endif

#ifndef __ILedger_DISPINTERFACE_DEFINED__
#define __ILedger_DISPINTERFACE_DEFINED__

/* dispinterface ILedger */
/* [uuid] */ 


DEFINE_GUID(DIID_ILedger,0xF26FD729,0xBC91,0x46EF,0xB4,0xD2,0x6A,0xA9,0xFA,0xAE,0xB0,0x17);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F26FD729-BC91-46EF-B4D2-6AA9FAAEB017")
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
            _COM_Outptr_  void **ppvObject);
        
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


DEFINE_GUID(CLSID_Ledger,0x5BEC8BAF,0x65AE,0x4B13,0x95,0x7F,0x08,0x3D,0x36,0x45,0xF8,0x79);

#ifdef __cplusplus

class DECLSPEC_UUID("5BEC8BAF-65AE-4B13-957F-083D3645F879")
Ledger;
#endif
#endif /* __Axislogin_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


