

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for BrowserCtrl.odl:
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


#ifndef __BrowserCtrl_h_h__
#define __BrowserCtrl_h_h__

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



#ifndef __BrowserCtrl_LIBRARY_DEFINED__
#define __BrowserCtrl_LIBRARY_DEFINED__

/* library BrowserCtrl */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_BrowserCtrl,0xF42873CD,0x4F31,0x40A7,0xBA,0xDB,0x61,0xCA,0xD7,0x93,0xC9,0xCD);

#ifndef __IControlWnd_DISPINTERFACE_DEFINED__
#define __IControlWnd_DISPINTERFACE_DEFINED__

/* dispinterface IControlWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IControlWnd,0x58553D20,0xF769,0x4AB6,0xAF,0x78,0x3C,0xAA,0x7E,0x11,0x50,0x74);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("58553D20-F769-4AB6-AF78-3CAA7E115074")
    IControlWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IControlWndVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IControlWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IControlWnd * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IControlWnd * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IControlWnd * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IControlWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IControlWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
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


DEFINE_GUID(CLSID_ControlWnd,0xFA9F9D53,0x2456,0x4680,0xB4,0x20,0xDA,0xE8,0x67,0xCD,0xFE,0xF7);

#ifdef __cplusplus

class DECLSPEC_UUID("FA9F9D53-2456-4680-B420-DAE867CDFEF7")
ControlWnd;
#endif
#endif /* __BrowserCtrl_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


