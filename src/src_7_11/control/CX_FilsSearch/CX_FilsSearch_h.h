

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Jun 07 21:28:41 2017
 */
/* Compiler settings for CX_FilsSearch.odl:
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


#ifndef __CX_FilsSearch_h_h__
#define __CX_FilsSearch_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IControlwnd_FWD_DEFINED__
#define __IControlwnd_FWD_DEFINED__
typedef interface IControlwnd IControlwnd;
#endif 	/* __IControlwnd_FWD_DEFINED__ */


#ifndef __Controlwnd_FWD_DEFINED__
#define __Controlwnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class Controlwnd Controlwnd;
#else
typedef struct Controlwnd Controlwnd;
#endif /* __cplusplus */

#endif 	/* __Controlwnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CX_FilsSearch_LIBRARY_DEFINED__
#define __CX_FilsSearch_LIBRARY_DEFINED__

/* library CX_FilsSearch */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CX_FilsSearch,0x6DF008A3,0xB973,0x4804,0x89,0x26,0xA6,0x25,0xD5,0x1E,0x55,0xE2);

#ifndef __IControlwnd_DISPINTERFACE_DEFINED__
#define __IControlwnd_DISPINTERFACE_DEFINED__

/* dispinterface IControlwnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IControlwnd,0xFC6CC42A,0xA48C,0x4551,0x8C,0x49,0x33,0x98,0x36,0xC5,0x05,0x2F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FC6CC42A-A48C-4551-8C49-339836C5052F")
    IControlwnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IControlwndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IControlwnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IControlwnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IControlwnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IControlwnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IControlwnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IControlwnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IControlwnd * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IControlwndVtbl;

    interface IControlwnd
    {
        CONST_VTBL struct IControlwndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IControlwnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IControlwnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IControlwnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IControlwnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IControlwnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IControlwnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IControlwnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IControlwnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Controlwnd,0xC0FE15C4,0x5887,0x43A8,0x93,0x5A,0x07,0x3B,0x0B,0x65,0xF1,0x3C);

#ifdef __cplusplus

class DECLSPEC_UUID("C0FE15C4-5887-43A8-935A-073B0B65F13C")
Controlwnd;
#endif
#endif /* __CX_FilsSearch_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


