

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for cx_test.odl:
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


#ifndef __cx_test_h_h__
#define __cx_test_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMapWnd_FWD_DEFINED__
#define __IMapWnd_FWD_DEFINED__
typedef interface IMapWnd IMapWnd;

#endif 	/* __IMapWnd_FWD_DEFINED__ */


#ifndef __MapWnd_FWD_DEFINED__
#define __MapWnd_FWD_DEFINED__

#ifdef __cplusplus
typedef class MapWnd MapWnd;
#else
typedef struct MapWnd MapWnd;
#endif /* __cplusplus */

#endif 	/* __MapWnd_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Cx_test_LIBRARY_DEFINED__
#define __Cx_test_LIBRARY_DEFINED__

/* library Cx_test */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Cx_test,0xD454AE7D,0xB76F,0x4433,0xAC,0x98,0x7A,0x9A,0x50,0xF5,0x64,0x47);

#ifndef __IMapWnd_DISPINTERFACE_DEFINED__
#define __IMapWnd_DISPINTERFACE_DEFINED__

/* dispinterface IMapWnd */
/* [uuid] */ 


DEFINE_GUID(DIID_IMapWnd,0x867CD1F0,0x86C2,0x4642,0xA6,0x5D,0x90,0x36,0x29,0x8C,0x45,0x97);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("867CD1F0-86C2-4642-A65D-9036298C4597")
    IMapWnd : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IMapWndVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMapWnd * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMapWnd * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMapWnd * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMapWnd * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMapWnd * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMapWnd * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMapWnd * This,
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
    } IMapWndVtbl;

    interface IMapWnd
    {
        CONST_VTBL struct IMapWndVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMapWnd_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMapWnd_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMapWnd_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMapWnd_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMapWnd_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMapWnd_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMapWnd_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IMapWnd_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_MapWnd,0x8791636A,0xAE06,0x4AB4,0xBD,0xB9,0xC7,0xBD,0x98,0xA4,0x86,0x08);

#ifdef __cplusplus

class DECLSPEC_UUID("8791636A-AE06-4AB4-BDB9-C7BD98A48608")
MapWnd;
#endif
#endif /* __Cx_test_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


