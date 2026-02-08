

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for CX_InfoMgr.odl:
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


#ifndef __CX_InfoMgr_h_h__
#define __CX_InfoMgr_h_h__

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


#ifndef __IInfoWnd_FWD_DEFINED__
#define __IInfoWnd_FWD_DEFINED__
typedef interface IInfoWnd IInfoWnd;

#endif 	/* __IInfoWnd_FWD_DEFINED__ */


#ifndef __InfoWnd_FWD_DEFINED__
#define __InfoWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class InfoWnd InfoWnd;
#else
typedef struct InfoWnd InfoWnd;
#endif /* __cplusplus */

#endif 	/* __InfoWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_InfoMgr_LIBRARY_DEFINED__
#define __CX_InfoMgr_LIBRARY_DEFINED__

/* library CX_InfoMgr */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_InfoMgr,0x41254F9C,0x4856,0x48C7,0xBC,0x03,0xEF,0x96,0x0C,0x3D,0x11,0x3F);

#ifndef __IControlWnd_DISPINTERFACE_DEFINED__
#define __IControlWnd_DISPINTERFACE_DEFINED__

/* dispinterface IControlWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IControlWnd,0x7ECC238F,0xF8E1,0x4D3D,0xB6,0x86,0x66,0x33,0xD7,0x1C,0xAA,0x7F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7ECC238F-F8E1-4D3D-B686-6633D71CAA7F")
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


DEFINE_GUID(CLSID_ControlWnd,0x464E51F9,0x5EBB,0x4D55,0x9B,0xC9,0x05,0x4E,0x2A,0x2F,0x4F,0x7F);

#ifdef __cplusplus

class DECLSPEC_UUID("464E51F9-5EBB-4D55-9BC9-054E2A2F4F7F")
ControlWnd;
#endif

#ifndef __IInfoWnd_DISPINTERFACE_DEFINED__
#define __IInfoWnd_DISPINTERFACE_DEFINED__

/* dispinterface IInfoWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IInfoWnd,0x8148CB88,0x1196,0x41FE,0x9B,0x83,0x7A,0xB9,0x93,0x4D,0x07,0xDA);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8148CB88-1196-41FE-9B83-7AB9934D07DA")
    IInfoWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IInfoWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IInfoWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IInfoWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IInfoWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IInfoWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IInfoWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IInfoWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IInfoWnd * This,
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
    } IInfoWndVtbl;

    interface IInfoWnd
    {
        CONST_VTBL struct IInfoWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IInfoWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IInfoWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IInfoWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IInfoWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IInfoWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IInfoWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IInfoWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IInfoWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_InfoWnd,0x19D982F7,0x8F7E,0x4EA3,0x95,0x9E,0x02,0x95,0xC8,0x35,0x65,0xAC);

#ifdef __cplusplus

class DECLSPEC_UUID("19D982F7-8F7E-4EA3-959E-0295C83565AC")
InfoWnd;
#endif
#endif /* __CX_InfoMgr_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


