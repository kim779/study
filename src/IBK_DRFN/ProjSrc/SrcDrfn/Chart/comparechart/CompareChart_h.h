

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CompareChart.odl:
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


#ifndef __CompareChart_h_h__
#define __CompareChart_h_h__

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

#ifndef __IWndMapView_FWD_DEFINED__
#define __IWndMapView_FWD_DEFINED__
typedef interface IWndMapView IWndMapView;

#endif 	/* __IWndMapView_FWD_DEFINED__ */


#ifndef __WndMapView_FWD_DEFINED__
#define __WndMapView_FWD_DEFINED__

#ifdef __cplusplus
typedef class WndMapView WndMapView;
#else
typedef struct WndMapView WndMapView;
#endif /* __cplusplus */

#endif 	/* __WndMapView_FWD_DEFINED__ */


#ifndef __IStdDialog_FWD_DEFINED__
#define __IStdDialog_FWD_DEFINED__
typedef interface IStdDialog IStdDialog;

#endif 	/* __IStdDialog_FWD_DEFINED__ */


#ifndef __StdDialog_FWD_DEFINED__
#define __StdDialog_FWD_DEFINED__

#ifdef __cplusplus
typedef class StdDialog StdDialog;
#else
typedef struct StdDialog StdDialog;
#endif /* __cplusplus */

#endif 	/* __StdDialog_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CompareChart_LIBRARY_DEFINED__
#define __CompareChart_LIBRARY_DEFINED__

/* library CompareChart */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CompareChart,0x932D4874,0x2B3C,0x4A37,0xBA,0x46,0x0E,0x78,0x55,0x2D,0xDC,0xC1);

#ifndef __IWndMapView_DISPINTERFACE_DEFINED__
#define __IWndMapView_DISPINTERFACE_DEFINED__

/* dispinterface IWndMapView */
/* [uuid] */ 


DEFINE_GUID(DIID_IWndMapView,0x340A11D9,0xAAE9,0x4C33,0xAD,0x56,0x6E,0x7E,0xBD,0x48,0x60,0x31);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("340A11D9-AAE9-4C33-AD56-6E7EBD486031")
    IWndMapView : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IWndMapViewVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWndMapView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWndMapView * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWndMapView * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWndMapView * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWndMapView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWndMapView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWndMapView * This,
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
    } IWndMapViewVtbl;

    interface IWndMapView
    {
        CONST_VTBL struct IWndMapViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWndMapView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWndMapView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWndMapView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWndMapView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWndMapView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWndMapView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWndMapView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IWndMapView_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_WndMapView,0xDAE59411,0xEF4A,0x49C3,0x9B,0x86,0xAD,0x3A,0x2B,0x5A,0xA1,0x0F);

#ifdef __cplusplus

class DECLSPEC_UUID("DAE59411-EF4A-49C3-9B86-AD3A2B5AA10F")
WndMapView;
#endif

#ifndef __IStdDialog_DISPINTERFACE_DEFINED__
#define __IStdDialog_DISPINTERFACE_DEFINED__

/* dispinterface IStdDialog */
/* [uuid] */ 


DEFINE_GUID(DIID_IStdDialog,0x506C0A8C,0xCBC2,0x4d27,0xBD,0x67,0xD4,0x53,0x1A,0xB6,0xF5,0xA8);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("506C0A8C-CBC2-4d27-BD67-D4531AB6F5A8")
    IStdDialog : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IStdDialogVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IStdDialog * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IStdDialog * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IStdDialog * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IStdDialog * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IStdDialog * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IStdDialog * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IStdDialog * This,
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
    } IStdDialogVtbl;

    interface IStdDialog
    {
        CONST_VTBL struct IStdDialogVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IStdDialog_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IStdDialog_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IStdDialog_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IStdDialog_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IStdDialog_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IStdDialog_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IStdDialog_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IStdDialog_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_StdDialog,0x15FC7B68,0xFB3A,0x4b5d,0xA6,0x2D,0xF1,0xBB,0x96,0xF9,0x2D,0x27);

#ifdef __cplusplus

class DECLSPEC_UUID("15FC7B68-FB3A-4b5d-A62D-F1BB96F92D27")
StdDialog;
#endif
#endif /* __CompareChart_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


