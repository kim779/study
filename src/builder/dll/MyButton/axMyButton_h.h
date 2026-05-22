

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for axMyButton.idl:
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


#ifndef __axMyButton_h_h__
#define __axMyButton_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IfmMyButton_FWD_DEFINED__
#define __IfmMyButton_FWD_DEFINED__
typedef interface IfmMyButton IfmMyButton;

#endif 	/* __IfmMyButton_FWD_DEFINED__ */


#ifndef __fmMyButton_FWD_DEFINED__
#define __fmMyButton_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmMyButton fmMyButton;
#else
typedef struct fmMyButton fmMyButton;
#endif /* __cplusplus */

#endif 	/* __fmMyButton_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __axMyButton_LIBRARY_DEFINED__
#define __axMyButton_LIBRARY_DEFINED__

/* library axMyButton */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_axMyButton;

#ifndef __IfmMyButton_DISPINTERFACE_DEFINED__
#define __IfmMyButton_DISPINTERFACE_DEFINED__

/* dispinterface IfmMyButton */
/* [uuid] */ 


EXTERN_C const IID DIID_IfmMyButton;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E990B576-C6EA-431E-8F32-6AC17882C10E")
    IfmMyButton : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmMyButtonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmMyButton * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmMyButton * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmMyButton * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmMyButton * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmMyButton * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmMyButton * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmMyButton * This,
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
    } IfmMyButtonVtbl;

    interface IfmMyButton
    {
        CONST_VTBL struct IfmMyButtonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmMyButton_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmMyButton_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmMyButton_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmMyButton_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmMyButton_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmMyButton_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmMyButton_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmMyButton_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_fmMyButton;

#ifdef __cplusplus

class DECLSPEC_UUID("94654E5E-5EEF-4FF9-AFCC-7EB79AC5CDAD")
fmMyButton;
#endif
#endif /* __axMyButton_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


