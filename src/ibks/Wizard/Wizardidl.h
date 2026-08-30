

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for Wizard.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0626 
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


#ifndef __Wizardidl_h__
#define __Wizardidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if _CONTROL_FLOW_GUARD_XFG
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
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


/* interface __MIDL_itf_Wizard_0000_0000 */
/* [local] */ 

#pragma external_header(push)
#pragma external_header(pop)
#pragma external_header(push)
#pragma external_header(push)
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma external_header(push)
#pragma warning(push)
#pragma warning(disable:4001) 
#pragma once
#pragma warning(pop)
#pragma external_header(pop)
#pragma warning(pop)
#pragma external_header(pop)
#pragma region Desktop Family
#pragma endregion
#pragma external_header(pop)


extern RPC_IF_HANDLE __MIDL_itf_Wizard_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_Wizard_0000_0000_v0_0_s_ifspec;


#ifndef __WizardLib_LIBRARY_DEFINED__
#define __WizardLib_LIBRARY_DEFINED__

/* library WizardLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_WizardLib;

#ifndef ___DWizard_DISPINTERFACE_DEFINED__
#define ___DWizard_DISPINTERFACE_DEFINED__

/* dispinterface _DWizard */
/* [uuid] */ 


EXTERN_C const IID DIID__DWizard;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("72d9dc6c-90be-4e43-9e19-48b968830ae7")
    _DWizard : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWizardVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWizard * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWizard * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWizard * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWizard * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWizard * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWizard * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWizard * This,
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
/* [uuid] */ 


EXTERN_C const IID DIID__DWizardEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("bf500ab3-0b43-43e8-adc4-f944d226da5f")
    _DWizardEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DWizardEventsVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DWizardEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DWizardEvents * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DWizardEvents * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DWizardEvents * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DWizardEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DWizardEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DWizardEvents * This,
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


EXTERN_C const CLSID CLSID_Wizard;

#ifdef __cplusplus

class DECLSPEC_UUID("fe2fc2d9-055b-4095-8806-c98367187868")
Wizard;
#endif

#ifndef __IxScreen_DISPINTERFACE_DEFINED__
#define __IxScreen_DISPINTERFACE_DEFINED__

/* dispinterface IxScreen */
/* [uuid] */ 


EXTERN_C const IID DIID_IxScreen;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("89CFEA19-83BB-4DB3-BA70-D509794C1091")
    IxScreen : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IxScreenVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IxScreen * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IxScreen * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IxScreen * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IxScreen * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IxScreen * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IxScreen * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IxScreen * This,
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


EXTERN_C const CLSID CLSID_xScreen;

#ifdef __cplusplus

class DECLSPEC_UUID("8B184965-2CFD-4134-9D99-59DDEA5E0FA5")
xScreen;
#endif

#ifndef __IxSystem_DISPINTERFACE_DEFINED__
#define __IxSystem_DISPINTERFACE_DEFINED__

/* dispinterface IxSystem */
/* [uuid] */ 


EXTERN_C const IID DIID_IxSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("557CAD32-82F6-4BA8-A55E-DDF8D378DCBE")
    IxSystem : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IxSystemVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IxSystem * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IxSystem * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IxSystem * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IxSystem * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IxSystem * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IxSystem * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IxSystem * This,
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


EXTERN_C const CLSID CLSID_xSystem;

#ifdef __cplusplus

class DECLSPEC_UUID("774C1579-3E4A-485F-9EBC-AD47BCA54B60")
xSystem;
#endif
#endif /* __WizardLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


