

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for IB201600.odl:
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


#ifndef __IB201600_h_h__
#define __IB201600_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IToolWnd_FWD_DEFINED__
#define __IToolWnd_FWD_DEFINED__
typedef interface IToolWnd IToolWnd;

#endif 	/* __IToolWnd_FWD_DEFINED__ */


#ifndef __ToolWnd_FWD_DEFINED__
#define __ToolWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class ToolWnd ToolWnd;
#else
typedef struct ToolWnd ToolWnd;
#endif /* __cplusplus */

#endif 	/* __ToolWnd_FWD_DEFINED__ */


#ifndef __IViewWnd_FWD_DEFINED__
#define __IViewWnd_FWD_DEFINED__
typedef interface IViewWnd IViewWnd;

#endif 	/* __IViewWnd_FWD_DEFINED__ */


#ifndef __ViewWnd_FWD_DEFINED__
#define __ViewWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class ViewWnd ViewWnd;
#else
typedef struct ViewWnd ViewWnd;
#endif /* __cplusplus */

#endif 	/* __ViewWnd_FWD_DEFINED__ */


#ifndef __ISiseWnd_FWD_DEFINED__
#define __ISiseWnd_FWD_DEFINED__
typedef interface ISiseWnd ISiseWnd;

#endif 	/* __ISiseWnd_FWD_DEFINED__ */


#ifndef __SiseWnd_FWD_DEFINED__
#define __SiseWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class SiseWnd SiseWnd;
#else
typedef struct SiseWnd SiseWnd;
#endif /* __cplusplus */

#endif 	/* __SiseWnd_FWD_DEFINED__ */


#ifndef __IMatrixWnd_FWD_DEFINED__
#define __IMatrixWnd_FWD_DEFINED__
typedef interface IMatrixWnd IMatrixWnd;

#endif 	/* __IMatrixWnd_FWD_DEFINED__ */


#ifndef __MatrixWnd_FWD_DEFINED__
#define __MatrixWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class MatrixWnd MatrixWnd;
#else
typedef struct MatrixWnd MatrixWnd;
#endif /* __cplusplus */

#endif 	/* __MatrixWnd_FWD_DEFINED__ */


#ifndef __IHogaWnd_FWD_DEFINED__
#define __IHogaWnd_FWD_DEFINED__
typedef interface IHogaWnd IHogaWnd;

#endif 	/* __IHogaWnd_FWD_DEFINED__ */


#ifndef __HogaWnd_FWD_DEFINED__
#define __HogaWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class HogaWnd HogaWnd;
#else
typedef struct HogaWnd HogaWnd;
#endif /* __cplusplus */

#endif 	/* __HogaWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __IB201600_LIBRARY_DEFINED__
#define __IB201600_LIBRARY_DEFINED__

/* library IB201600 */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_IB201600,0x473DFF18,0x2C95,0x42D1,0x8D,0xB3,0x3F,0x33,0x02,0x61,0xC5,0x83);

#ifndef __IToolWnd_DISPINTERFACE_DEFINED__
#define __IToolWnd_DISPINTERFACE_DEFINED__

/* dispinterface IToolWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IToolWnd,0x4ED2ADAD,0x0014,0x4441,0xBE,0xF6,0x6B,0xCE,0xC7,0xBC,0x6C,0x32);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4ED2ADAD-0014-4441-BEF6-6BCEC7BC6C32")
    IToolWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IToolWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IToolWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IToolWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IToolWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IToolWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IToolWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IToolWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IToolWnd * This,
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
    } IToolWndVtbl;

    interface IToolWnd
    {
        CONST_VTBL struct IToolWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IToolWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IToolWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IToolWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IToolWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IToolWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IToolWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IToolWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IToolWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_ToolWnd,0x6CFD36F2,0x38C4,0x4AF8,0x84,0x8A,0xD2,0x31,0x1B,0x74,0x7F,0xBE);

#ifdef __cplusplus

class DECLSPEC_UUID("6CFD36F2-38C4-4AF8-848A-D2311B747FBE")
ToolWnd;
#endif

#ifndef __IViewWnd_DISPINTERFACE_DEFINED__
#define __IViewWnd_DISPINTERFACE_DEFINED__

/* dispinterface IViewWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IViewWnd,0x0436CD91,0x84D3,0x41BC,0x97,0x7C,0xDC,0x0C,0x7E,0x87,0x59,0xB6);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0436CD91-84D3-41BC-977C-DC0C7E8759B6")
    IViewWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IViewWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IViewWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IViewWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IViewWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IViewWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IViewWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IViewWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IViewWnd * This,
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
    } IViewWndVtbl;

    interface IViewWnd
    {
        CONST_VTBL struct IViewWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IViewWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IViewWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IViewWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IViewWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IViewWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IViewWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IViewWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IViewWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_ViewWnd,0x7BDFD516,0xFEE5,0x4EEA,0x8F,0x36,0x37,0xE6,0x67,0x01,0x22,0x9E);

#ifdef __cplusplus

class DECLSPEC_UUID("7BDFD516-FEE5-4EEA-8F36-37E66701229E")
ViewWnd;
#endif

#ifndef __ISiseWnd_DISPINTERFACE_DEFINED__
#define __ISiseWnd_DISPINTERFACE_DEFINED__

/* dispinterface ISiseWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_ISiseWnd,0x98CBF053,0xC703,0x4615,0xAB,0xA0,0x89,0xC5,0x94,0x3F,0x7C,0xBA);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("98CBF053-C703-4615-ABA0-89C5943F7CBA")
    ISiseWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ISiseWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISiseWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISiseWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISiseWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISiseWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISiseWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISiseWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISiseWnd * This,
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
    } ISiseWndVtbl;

    interface ISiseWnd
    {
        CONST_VTBL struct ISiseWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISiseWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ISiseWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ISiseWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ISiseWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ISiseWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ISiseWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ISiseWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ISiseWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_SiseWnd,0x7B3FBE3B,0xD247,0x43EE,0xB1,0xCC,0x48,0x90,0x5B,0x5A,0x8F,0x34);

#ifdef __cplusplus

class DECLSPEC_UUID("7B3FBE3B-D247-43EE-B1CC-48905B5A8F34")
SiseWnd;
#endif

#ifndef __IMatrixWnd_DISPINTERFACE_DEFINED__
#define __IMatrixWnd_DISPINTERFACE_DEFINED__

/* dispinterface IMatrixWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMatrixWnd,0xA76C7CE4,0x07DF,0x4E22,0xB5,0xED,0xC0,0xEA,0x4E,0xCB,0x71,0xFB);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A76C7CE4-07DF-4E22-B5ED-C0EA4ECB71FB")
    IMatrixWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMatrixWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMatrixWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMatrixWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMatrixWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMatrixWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMatrixWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMatrixWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMatrixWnd * This,
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
    } IMatrixWndVtbl;

    interface IMatrixWnd
    {
        CONST_VTBL struct IMatrixWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMatrixWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMatrixWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMatrixWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMatrixWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMatrixWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMatrixWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMatrixWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMatrixWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MatrixWnd,0xF5748DD6,0x26BF,0x4D79,0xA9,0x20,0xB3,0xF8,0xAD,0xC9,0x30,0x7E);

#ifdef __cplusplus

class DECLSPEC_UUID("F5748DD6-26BF-4D79-A920-B3F8ADC9307E")
MatrixWnd;
#endif

#ifndef __IHogaWnd_DISPINTERFACE_DEFINED__
#define __IHogaWnd_DISPINTERFACE_DEFINED__

/* dispinterface IHogaWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IHogaWnd,0xD3314442,0x9B73,0x48C6,0x8B,0x91,0x7F,0x4F,0xAA,0x7D,0x3C,0xF0);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D3314442-9B73-48C6-8B91-7F4FAA7D3CF0")
    IHogaWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IHogaWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IHogaWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IHogaWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IHogaWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IHogaWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IHogaWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IHogaWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IHogaWnd * This,
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
    } IHogaWndVtbl;

    interface IHogaWnd
    {
        CONST_VTBL struct IHogaWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHogaWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IHogaWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IHogaWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IHogaWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IHogaWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IHogaWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IHogaWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IHogaWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_HogaWnd,0x9BF209B4,0xB57A,0x4E8B,0xBC,0x93,0x71,0xFA,0x55,0x9A,0x6C,0xD8);

#ifdef __cplusplus

class DECLSPEC_UUID("9BF209B4-B57A-4E8B-BC93-71FA559A6CD8")
HogaWnd;
#endif
#endif /* __IB201600_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


