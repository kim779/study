

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_WebView.odl:
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


#ifndef __CX_WebView_h_h__
#define __CX_WebView_h_h__

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

#ifndef __IMainWnd_FWD_DEFINED__
#define __IMainWnd_FWD_DEFINED__
typedef interface IMainWnd IMainWnd;

#endif 	/* __IMainWnd_FWD_DEFINED__ */


#ifndef __MainWnd_FWD_DEFINED__
#define __MainWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class MainWnd MainWnd;
#else
typedef struct MainWnd MainWnd;
#endif /* __cplusplus */

#endif 	/* __MainWnd_FWD_DEFINED__ */


#ifndef __IIEWnd_FWD_DEFINED__
#define __IIEWnd_FWD_DEFINED__
typedef interface IIEWnd IIEWnd;

#endif 	/* __IIEWnd_FWD_DEFINED__ */


#ifndef __IEWnd_FWD_DEFINED__
#define __IEWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class IEWnd IEWnd;
#else
typedef struct IEWnd IEWnd;
#endif /* __cplusplus */

#endif 	/* __IEWnd_FWD_DEFINED__ */


#ifndef __IEdgeWnd_FWD_DEFINED__
#define __IEdgeWnd_FWD_DEFINED__
typedef interface IEdgeWnd IEdgeWnd;

#endif 	/* __IEdgeWnd_FWD_DEFINED__ */


#ifndef __EdgeWnd_FWD_DEFINED__
#define __EdgeWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class EdgeWnd EdgeWnd;
#else
typedef struct EdgeWnd EdgeWnd;
#endif /* __cplusplus */

#endif 	/* __EdgeWnd_FWD_DEFINED__ */


#ifndef __IChromeWnd_FWD_DEFINED__
#define __IChromeWnd_FWD_DEFINED__
typedef interface IChromeWnd IChromeWnd;

#endif 	/* __IChromeWnd_FWD_DEFINED__ */


#ifndef __ChromeWnd_FWD_DEFINED__
#define __ChromeWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class ChromeWnd ChromeWnd;
#else
typedef struct ChromeWnd ChromeWnd;
#endif /* __cplusplus */

#endif 	/* __ChromeWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_WebView_LIBRARY_DEFINED__
#define __CX_WebView_LIBRARY_DEFINED__

/* library CX_WebView */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_WebView,0x027A6920,0x5C7A,0x4BCC,0x88,0xA4,0x88,0x0C,0x15,0x3C,0x05,0x2D);

#ifndef __IMainWnd_DISPINTERFACE_DEFINED__
#define __IMainWnd_DISPINTERFACE_DEFINED__

/* dispinterface IMainWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMainWnd,0x67801C5C,0xEC43,0x4CDC,0x9D,0xBD,0x52,0x45,0xE0,0x2B,0x72,0x82);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("67801C5C-EC43-4CDC-9DBD-5245E02B7282")
    IMainWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMainWndVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMainWnd * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMainWnd * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMainWnd * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMainWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMainWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMainWnd * This,
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
    } IMainWndVtbl;

    interface IMainWnd
    {
        CONST_VTBL struct IMainWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMainWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMainWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMainWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMainWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMainWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMainWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMainWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMainWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MainWnd,0xBF36433D,0xF775,0x4BF6,0x85,0xA5,0xB1,0xBC,0xB6,0x44,0x9D,0x34);

#ifdef __cplusplus

class DECLSPEC_UUID("BF36433D-F775-4BF6-85A5-B1BCB6449D34")
MainWnd;
#endif

#ifndef __IIEWnd_DISPINTERFACE_DEFINED__
#define __IIEWnd_DISPINTERFACE_DEFINED__

/* dispinterface IIEWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IIEWnd,0xB8018BF3,0x6CFA,0x460A,0xBA,0x46,0x59,0x1A,0x5C,0x5D,0x28,0xE8);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B8018BF3-6CFA-460A-BA46-591A5C5D28E8")
    IIEWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IIEWndVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIEWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIEWnd * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIEWnd * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIEWnd * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIEWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIEWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIEWnd * This,
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
    } IIEWndVtbl;

    interface IIEWnd
    {
        CONST_VTBL struct IIEWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIEWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIEWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIEWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIEWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIEWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIEWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIEWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IIEWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_IEWnd,0xABF789C7,0x0C42,0x4936,0x88,0x09,0x45,0x4D,0xA2,0x3B,0x2A,0xC4);

#ifdef __cplusplus

class DECLSPEC_UUID("ABF789C7-0C42-4936-8809-454DA23B2AC4")
IEWnd;
#endif

#ifndef __IEdgeWnd_DISPINTERFACE_DEFINED__
#define __IEdgeWnd_DISPINTERFACE_DEFINED__

/* dispinterface IEdgeWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IEdgeWnd,0xFA4AE7FB,0x92A9,0x4452,0x9C,0x50,0xF0,0x8E,0xDA,0xB6,0xE9,0x85);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FA4AE7FB-92A9-4452-9C50-F08EDAB6E985")
    IEdgeWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IEdgeWndVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEdgeWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEdgeWnd * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEdgeWnd * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEdgeWnd * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEdgeWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEdgeWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEdgeWnd * This,
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
    } IEdgeWndVtbl;

    interface IEdgeWnd
    {
        CONST_VTBL struct IEdgeWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEdgeWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IEdgeWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IEdgeWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IEdgeWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IEdgeWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IEdgeWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IEdgeWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IEdgeWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_EdgeWnd,0x9C4BDD26,0xFECD,0x4137,0x84,0xCF,0x49,0x3C,0x49,0x70,0xEE,0x04);

#ifdef __cplusplus

class DECLSPEC_UUID("9C4BDD26-FECD-4137-84CF-493C4970EE04")
EdgeWnd;
#endif

#ifndef __IChromeWnd_DISPINTERFACE_DEFINED__
#define __IChromeWnd_DISPINTERFACE_DEFINED__

/* dispinterface IChromeWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IChromeWnd,0xA2ABAE8D,0xE68E,0x4A66,0xB4,0xD4,0x92,0x71,0x1A,0xF4,0x32,0xC7);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A2ABAE8D-E68E-4A66-B4D4-92711AF432C7")
    IChromeWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IChromeWndVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IChromeWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IChromeWnd * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IChromeWnd * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IChromeWnd * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IChromeWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IChromeWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IChromeWnd * This,
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
    } IChromeWndVtbl;

    interface IChromeWnd
    {
        CONST_VTBL struct IChromeWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IChromeWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IChromeWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IChromeWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IChromeWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IChromeWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IChromeWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IChromeWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IChromeWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_ChromeWnd,0x7775396C,0x4DBB,0x4E20,0x8B,0x93,0x0C,0x6D,0x55,0x33,0x35,0xD8);

#ifdef __cplusplus

class DECLSPEC_UUID("7775396C-4DBB-4E20-8B93-0C6D553335D8")
ChromeWnd;
#endif
#endif /* __CX_WebView_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


