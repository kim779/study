

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Mar 23 15:55:28 2017
 */
/* Compiler settings for Wizard.odl:
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


#ifndef __Wizard_h_h__
#define __Wizard_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DWizard_FWD_DEFINED__
#define ___DWizard_FWD_DEFINED__
typedef interface _DWizard _DWizard;
#endif 	/* ___DWizard_FWD_DEFINED__ */


#ifndef ___DWizardEvents_FWD_DEFINED__
#define ___DWizardEvents_FWD_DEFINED__
typedef interface _DWizardEvents _DWizardEvents;
#endif 	/* ___DWizardEvents_FWD_DEFINED__ */


#ifndef __Wizard_FWD_DEFINED__
#define __Wizard_FWD_DEFINED__

#ifdef __cplusplus
typedef class Wizard Wizard;
#else
typedef struct Wizard Wizard;
#endif /* __cplusplus */

#endif 	/* __Wizard_FWD_DEFINED__ */


#ifndef __IxScreen_FWD_DEFINED__
#define __IxScreen_FWD_DEFINED__
typedef interface IxScreen IxScreen;
#endif 	/* __IxScreen_FWD_DEFINED__ */


#ifndef __xScreen_FWD_DEFINED__
#define __xScreen_FWD_DEFINED__

#ifdef __cplusplus
typedef class xScreen xScreen;
#else
typedef struct xScreen xScreen;
#endif /* __cplusplus */

#endif 	/* __xScreen_FWD_DEFINED__ */


#ifndef __IxSystem_FWD_DEFINED__
#define __IxSystem_FWD_DEFINED__
typedef interface IxSystem IxSystem;
#endif 	/* __IxSystem_FWD_DEFINED__ */


#ifndef __xSystem_FWD_DEFINED__
#define __xSystem_FWD_DEFINED__

#ifdef __cplusplus
typedef class xSystem xSystem;
#else
typedef struct xSystem xSystem;
#endif /* __cplusplus */

#endif 	/* __xSystem_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __WIZARDLib_LIBRARY_DEFINED__
#define __WIZARDLib_LIBRARY_DEFINED__

/* library WIZARDLib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_WIZARDLib,0x95840AF6,0xB82B,0x42DE,0x94,0xC9,0x20,0x39,0x36,0x76,0x89,0xCB);

#ifndef ___DWizard_DISPINTERFACE_DEFINED__
#define ___DWizard_DISPINTERFACE_DEFINED__

/* dispinterface _DWizard */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DWizard,0x540246C5,0x8AA5,0x46DE,0xB4,0xCB,0xE3,0xF2,0x91,0xD6,0x95,0xC3);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("540246C5-8AA5-46DE-B4CB-E3F291D695C3")
    _DWizard : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWizardVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWizard * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWizard * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWizard * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWizard * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWizard * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWizard * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWizard * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWizardVtbl;

    interface _DWizard
    {
        CONST_VTBL struct _DWizardVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWizard_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWizard_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWizard_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWizard_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWizard_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWizard_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWizard_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWizard_DISPINTERFACE_DEFINED__ */


#ifndef ___DWizardEvents_DISPINTERFACE_DEFINED__
#define ___DWizardEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DWizardEvents */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DWizardEvents,0xA2DD4943,0xF0C3,0x4AB0,0x85,0xC6,0xC5,0xD4,0xE5,0x33,0x0A,0x01);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A2DD4943-F0C3-4AB0-85C6-C5D4E5330A01")
    _DWizardEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWizardEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWizardEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWizardEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWizardEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWizardEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWizardEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWizardEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWizardEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DWizardEventsVtbl;

    interface _DWizardEvents
    {
        CONST_VTBL struct _DWizardEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DWizardEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DWizardEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DWizardEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DWizardEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DWizardEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DWizardEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DWizardEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DWizardEvents_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Wizard,0xB2816A99,0x87C0,0x4EC2,0xB9,0x7F,0x38,0x75,0xB8,0x6A,0xF2,0xC7);

#ifdef __cplusplus

class DECLSPEC_UUID("B2816A99-87C0-4EC2-B97F-3875B86AF2C7")
Wizard;
#endif

#ifndef __IxScreen_DISPINTERFACE_DEFINED__
#define __IxScreen_DISPINTERFACE_DEFINED__

/* dispinterface IxScreen */
/* [uuid] */ 


DEFINE_GUID(DIID_IxScreen,0x892925C0,0x9BED,0x4662,0x99,0x32,0xDB,0x3E,0x6E,0xCB,0x32,0x9B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("892925C0-9BED-4662-9932-DB3E6ECB329B")
    IxScreen : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IxScreenVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IxScreen * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IxScreen * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IxScreen * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IxScreen * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IxScreen * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IxScreen * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IxScreen * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IxScreenVtbl;

    interface IxScreen
    {
        CONST_VTBL struct IxScreenVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IxScreen_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IxScreen_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IxScreen_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IxScreen_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IxScreen_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IxScreen_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IxScreen_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IxScreen_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_xScreen,0x63ED2000,0x869B,0x44C9,0xB4,0x3E,0x2D,0x37,0x67,0x58,0xC5,0x8F);

#ifdef __cplusplus

class DECLSPEC_UUID("63ED2000-869B-44C9-B43E-2D376758C58F")
xScreen;
#endif

#ifndef __IxSystem_DISPINTERFACE_DEFINED__
#define __IxSystem_DISPINTERFACE_DEFINED__

/* dispinterface IxSystem */
/* [uuid] */ 


DEFINE_GUID(DIID_IxSystem,0x55F722FF,0x9F53,0x411D,0xB7,0xBA,0x86,0x33,0xBD,0x30,0x08,0x85);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("55F722FF-9F53-411D-B7BA-8633BD300885")
    IxSystem : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IxSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IxSystem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IxSystem * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IxSystem * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IxSystem * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IxSystem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IxSystem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IxSystem * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IxSystemVtbl;

    interface IxSystem
    {
        CONST_VTBL struct IxSystemVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IxSystem_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IxSystem_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IxSystem_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IxSystem_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IxSystem_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IxSystem_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IxSystem_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IxSystem_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_xSystem,0x9B4CF2DB,0x3D25,0x4DFF,0xB8,0xFA,0xF6,0x34,0x09,0x26,0x84,0xA3);

#ifdef __cplusplus

class DECLSPEC_UUID("9B4CF2DB-3D25-4DFF-B8FA-F634092684A3")
xSystem;
#endif
#endif /* __WIZARDLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


