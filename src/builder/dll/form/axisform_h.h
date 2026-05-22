

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for axisform.odl:
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


#ifndef __axisform_h_h__
#define __axisform_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IAxisForm_FWD_DEFINED__
#define __IAxisForm_FWD_DEFINED__
typedef interface IAxisForm IAxisForm;

#endif 	/* __IAxisForm_FWD_DEFINED__ */


#ifndef __AxisForm_FWD_DEFINED__
#define __AxisForm_FWD_DEFINED__

#ifdef __cplusplus
typedef class AxisForm AxisForm;
#else
typedef struct AxisForm AxisForm;
#endif /* __cplusplus */

#endif 	/* __AxisForm_FWD_DEFINED__ */


#ifndef __IfmAvi_FWD_DEFINED__
#define __IfmAvi_FWD_DEFINED__
typedef interface IfmAvi IfmAvi;

#endif 	/* __IfmAvi_FWD_DEFINED__ */


#ifndef __fmAvi_FWD_DEFINED__
#define __fmAvi_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmAvi fmAvi;
#else
typedef struct fmAvi fmAvi;
#endif /* __cplusplus */

#endif 	/* __fmAvi_FWD_DEFINED__ */


#ifndef __IfmBase_FWD_DEFINED__
#define __IfmBase_FWD_DEFINED__
typedef interface IfmBase IfmBase;

#endif 	/* __IfmBase_FWD_DEFINED__ */


#ifndef __fmBase_FWD_DEFINED__
#define __fmBase_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmBase fmBase;
#else
typedef struct fmBase fmBase;
#endif /* __cplusplus */

#endif 	/* __fmBase_FWD_DEFINED__ */


#ifndef __IfmBox_FWD_DEFINED__
#define __IfmBox_FWD_DEFINED__
typedef interface IfmBox IfmBox;

#endif 	/* __IfmBox_FWD_DEFINED__ */


#ifndef __fmBox_FWD_DEFINED__
#define __fmBox_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmBox fmBox;
#else
typedef struct fmBox fmBox;
#endif /* __cplusplus */

#endif 	/* __fmBox_FWD_DEFINED__ */


#ifndef __IfmBrowser_FWD_DEFINED__
#define __IfmBrowser_FWD_DEFINED__
typedef interface IfmBrowser IfmBrowser;

#endif 	/* __IfmBrowser_FWD_DEFINED__ */


#ifndef __fmBrowser_FWD_DEFINED__
#define __fmBrowser_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmBrowser fmBrowser;
#else
typedef struct fmBrowser fmBrowser;
#endif /* __cplusplus */

#endif 	/* __fmBrowser_FWD_DEFINED__ */


#ifndef __IfmButton_FWD_DEFINED__
#define __IfmButton_FWD_DEFINED__
typedef interface IfmButton IfmButton;

#endif 	/* __IfmButton_FWD_DEFINED__ */


#ifndef __fmButton_FWD_DEFINED__
#define __fmButton_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmButton fmButton;
#else
typedef struct fmButton fmButton;
#endif /* __cplusplus */

#endif 	/* __fmButton_FWD_DEFINED__ */


#ifndef __IfmCheck_FWD_DEFINED__
#define __IfmCheck_FWD_DEFINED__
typedef interface IfmCheck IfmCheck;

#endif 	/* __IfmCheck_FWD_DEFINED__ */


#ifndef __fmCheck_FWD_DEFINED__
#define __fmCheck_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmCheck fmCheck;
#else
typedef struct fmCheck fmCheck;
#endif /* __cplusplus */

#endif 	/* __fmCheck_FWD_DEFINED__ */


#ifndef __IfmCombo_FWD_DEFINED__
#define __IfmCombo_FWD_DEFINED__
typedef interface IfmCombo IfmCombo;

#endif 	/* __IfmCombo_FWD_DEFINED__ */


#ifndef __fmCombo_FWD_DEFINED__
#define __fmCombo_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmCombo fmCombo;
#else
typedef struct fmCombo fmCombo;
#endif /* __cplusplus */

#endif 	/* __fmCombo_FWD_DEFINED__ */


#ifndef __IfmEdit_FWD_DEFINED__
#define __IfmEdit_FWD_DEFINED__
typedef interface IfmEdit IfmEdit;

#endif 	/* __IfmEdit_FWD_DEFINED__ */


#ifndef __fmEdit_FWD_DEFINED__
#define __fmEdit_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmEdit fmEdit;
#else
typedef struct fmEdit fmEdit;
#endif /* __cplusplus */

#endif 	/* __fmEdit_FWD_DEFINED__ */


#ifndef __IfmEditEx_FWD_DEFINED__
#define __IfmEditEx_FWD_DEFINED__
typedef interface IfmEditEx IfmEditEx;

#endif 	/* __IfmEditEx_FWD_DEFINED__ */


#ifndef __fmEditEx_FWD_DEFINED__
#define __fmEditEx_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmEditEx fmEditEx;
#else
typedef struct fmEditEx fmEditEx;
#endif /* __cplusplus */

#endif 	/* __fmEditEx_FWD_DEFINED__ */


#ifndef __IfmGrid_FWD_DEFINED__
#define __IfmGrid_FWD_DEFINED__
typedef interface IfmGrid IfmGrid;

#endif 	/* __IfmGrid_FWD_DEFINED__ */


#ifndef __fmGrid_FWD_DEFINED__
#define __fmGrid_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmGrid fmGrid;
#else
typedef struct fmGrid fmGrid;
#endif /* __cplusplus */

#endif 	/* __fmGrid_FWD_DEFINED__ */


#ifndef __IfmGroup_FWD_DEFINED__
#define __IfmGroup_FWD_DEFINED__
typedef interface IfmGroup IfmGroup;

#endif 	/* __IfmGroup_FWD_DEFINED__ */


#ifndef __fmGroup_FWD_DEFINED__
#define __fmGroup_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmGroup fmGroup;
#else
typedef struct fmGroup fmGroup;
#endif /* __cplusplus */

#endif 	/* __fmGroup_FWD_DEFINED__ */


#ifndef __IfmLabel_FWD_DEFINED__
#define __IfmLabel_FWD_DEFINED__
typedef interface IfmLabel IfmLabel;

#endif 	/* __IfmLabel_FWD_DEFINED__ */


#ifndef __fmLabel_FWD_DEFINED__
#define __fmLabel_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmLabel fmLabel;
#else
typedef struct fmLabel fmLabel;
#endif /* __cplusplus */

#endif 	/* __fmLabel_FWD_DEFINED__ */


#ifndef __IfmMemo_FWD_DEFINED__
#define __IfmMemo_FWD_DEFINED__
typedef interface IfmMemo IfmMemo;

#endif 	/* __IfmMemo_FWD_DEFINED__ */


#ifndef __fmMemo_FWD_DEFINED__
#define __fmMemo_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmMemo fmMemo;
#else
typedef struct fmMemo fmMemo;
#endif /* __cplusplus */

#endif 	/* __fmMemo_FWD_DEFINED__ */


#ifndef __IfmObject_FWD_DEFINED__
#define __IfmObject_FWD_DEFINED__
typedef interface IfmObject IfmObject;

#endif 	/* __IfmObject_FWD_DEFINED__ */


#ifndef __fmObject_FWD_DEFINED__
#define __fmObject_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmObject fmObject;
#else
typedef struct fmObject fmObject;
#endif /* __cplusplus */

#endif 	/* __fmObject_FWD_DEFINED__ */


#ifndef __IfmOut_FWD_DEFINED__
#define __IfmOut_FWD_DEFINED__
typedef interface IfmOut IfmOut;

#endif 	/* __IfmOut_FWD_DEFINED__ */


#ifndef __fmOut_FWD_DEFINED__
#define __fmOut_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmOut fmOut;
#else
typedef struct fmOut fmOut;
#endif /* __cplusplus */

#endif 	/* __fmOut_FWD_DEFINED__ */


#ifndef __IfmPanel_FWD_DEFINED__
#define __IfmPanel_FWD_DEFINED__
typedef interface IfmPanel IfmPanel;

#endif 	/* __IfmPanel_FWD_DEFINED__ */


#ifndef __fmPanel_FWD_DEFINED__
#define __fmPanel_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmPanel fmPanel;
#else
typedef struct fmPanel fmPanel;
#endif /* __cplusplus */

#endif 	/* __fmPanel_FWD_DEFINED__ */


#ifndef __IfmRadio_FWD_DEFINED__
#define __IfmRadio_FWD_DEFINED__
typedef interface IfmRadio IfmRadio;

#endif 	/* __IfmRadio_FWD_DEFINED__ */


#ifndef __fmRadio_FWD_DEFINED__
#define __fmRadio_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmRadio fmRadio;
#else
typedef struct fmRadio fmRadio;
#endif /* __cplusplus */

#endif 	/* __fmRadio_FWD_DEFINED__ */


#ifndef __IfmSheet_FWD_DEFINED__
#define __IfmSheet_FWD_DEFINED__
typedef interface IfmSheet IfmSheet;

#endif 	/* __IfmSheet_FWD_DEFINED__ */


#ifndef __fmSheet_FWD_DEFINED__
#define __fmSheet_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmSheet fmSheet;
#else
typedef struct fmSheet fmSheet;
#endif /* __cplusplus */

#endif 	/* __fmSheet_FWD_DEFINED__ */


#ifndef __IfmSysm_FWD_DEFINED__
#define __IfmSysm_FWD_DEFINED__
typedef interface IfmSysm IfmSysm;

#endif 	/* __IfmSysm_FWD_DEFINED__ */


#ifndef __fmSysm_FWD_DEFINED__
#define __fmSysm_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmSysm fmSysm;
#else
typedef struct fmSysm fmSysm;
#endif /* __cplusplus */

#endif 	/* __fmSysm_FWD_DEFINED__ */


#ifndef __IfmTab_FWD_DEFINED__
#define __IfmTab_FWD_DEFINED__
typedef interface IfmTab IfmTab;

#endif 	/* __IfmTab_FWD_DEFINED__ */


#ifndef __fmTab_FWD_DEFINED__
#define __fmTab_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmTab fmTab;
#else
typedef struct fmTab fmTab;
#endif /* __cplusplus */

#endif 	/* __fmTab_FWD_DEFINED__ */


#ifndef __IfmTable_FWD_DEFINED__
#define __IfmTable_FWD_DEFINED__
typedef interface IfmTable IfmTable;

#endif 	/* __IfmTable_FWD_DEFINED__ */


#ifndef __fmTable_FWD_DEFINED__
#define __fmTable_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmTable fmTable;
#else
typedef struct fmTable fmTable;
#endif /* __cplusplus */

#endif 	/* __fmTable_FWD_DEFINED__ */


#ifndef __IfmTreeView_FWD_DEFINED__
#define __IfmTreeView_FWD_DEFINED__
typedef interface IfmTreeView IfmTreeView;

#endif 	/* __IfmTreeView_FWD_DEFINED__ */


#ifndef __fmTreeView_FWD_DEFINED__
#define __fmTreeView_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmTreeView fmTreeView;
#else
typedef struct fmTreeView fmTreeView;
#endif /* __cplusplus */

#endif 	/* __fmTreeView_FWD_DEFINED__ */


#ifndef __IfmUserTab_FWD_DEFINED__
#define __IfmUserTab_FWD_DEFINED__
typedef interface IfmUserTab IfmUserTab;

#endif 	/* __IfmUserTab_FWD_DEFINED__ */


#ifndef __fmUserTab_FWD_DEFINED__
#define __fmUserTab_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmUserTab fmUserTab;
#else
typedef struct fmUserTab fmUserTab;
#endif /* __cplusplus */

#endif 	/* __fmUserTab_FWD_DEFINED__ */


#ifndef __IfmCtrl_FWD_DEFINED__
#define __IfmCtrl_FWD_DEFINED__
typedef interface IfmCtrl IfmCtrl;

#endif 	/* __IfmCtrl_FWD_DEFINED__ */


#ifndef __fmCtrl_FWD_DEFINED__
#define __fmCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class fmCtrl fmCtrl;
#else
typedef struct fmCtrl fmCtrl;
#endif /* __cplusplus */

#endif 	/* __fmCtrl_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Axisform_LIBRARY_DEFINED__
#define __Axisform_LIBRARY_DEFINED__

/* library Axisform */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Axisform,0xFB34F483,0x60AC,0x4585,0xA2,0xD8,0x71,0x1C,0xC8,0x9D,0x90,0x49);

#ifndef __IAxisForm_DISPINTERFACE_DEFINED__
#define __IAxisForm_DISPINTERFACE_DEFINED__

/* dispinterface IAxisForm */
/* [uuid] */ 


DEFINE_GUID(DIID_IAxisForm,0x1AED3C4B,0x5A8A,0x48F3,0x82,0x1D,0xB0,0xE0,0xC6,0xC8,0x2D,0x3F);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("1AED3C4B-5A8A-48F3-821D-B0E0C6C82D3F")
    IAxisForm : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IAxisFormVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAxisForm * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAxisForm * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAxisForm * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAxisForm * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAxisForm * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAxisForm * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAxisForm * This,
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
    } IAxisFormVtbl;

    interface IAxisForm
    {
        CONST_VTBL struct IAxisFormVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAxisForm_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IAxisForm_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IAxisForm_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IAxisForm_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IAxisForm_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IAxisForm_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IAxisForm_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IAxisForm_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_AxisForm,0x3E389F8A,0x3C8B,0x4E82,0xB5,0xB3,0x8B,0x98,0x9B,0xC8,0x78,0x5F);

#ifdef __cplusplus

class DECLSPEC_UUID("3E389F8A-3C8B-4E82-B5B3-8B989BC8785F")
AxisForm;
#endif

#ifndef __IfmAvi_DISPINTERFACE_DEFINED__
#define __IfmAvi_DISPINTERFACE_DEFINED__

/* dispinterface IfmAvi */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmAvi,0xC7240887,0x09C7,0x472F,0xAA,0xB4,0x51,0x98,0xC2,0x0B,0x1A,0x01);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C7240887-09C7-472F-AAB4-5198C20B1A01")
    IfmAvi : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmAviVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmAvi * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmAvi * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmAvi * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmAvi * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmAvi * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmAvi * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmAvi * This,
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
    } IfmAviVtbl;

    interface IfmAvi
    {
        CONST_VTBL struct IfmAviVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmAvi_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmAvi_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmAvi_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmAvi_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmAvi_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmAvi_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmAvi_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmAvi_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmAvi,0xE5C415C3,0xDC41,0x4015,0x92,0x08,0x7D,0x85,0xB9,0x01,0x26,0xC7);

#ifdef __cplusplus

class DECLSPEC_UUID("E5C415C3-DC41-4015-9208-7D85B90126C7")
fmAvi;
#endif

#ifndef __IfmBase_DISPINTERFACE_DEFINED__
#define __IfmBase_DISPINTERFACE_DEFINED__

/* dispinterface IfmBase */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmBase,0x6E6307E3,0x9BA5,0x489A,0xB7,0xA2,0xB6,0xF4,0xAE,0x0F,0x37,0x52);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6E6307E3-9BA5-489A-B7A2-B6F4AE0F3752")
    IfmBase : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmBaseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmBase * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmBase * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmBase * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmBase * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmBase * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmBase * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmBase * This,
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
    } IfmBaseVtbl;

    interface IfmBase
    {
        CONST_VTBL struct IfmBaseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmBase_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmBase_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmBase_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmBase_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmBase_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmBase_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmBase_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmBase_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmBase,0x1B43B685,0x9F9B,0x446E,0xA7,0x9A,0xA1,0x35,0xA9,0xCB,0xBB,0x22);

#ifdef __cplusplus

class DECLSPEC_UUID("1B43B685-9F9B-446E-A79A-A135A9CBBB22")
fmBase;
#endif

#ifndef __IfmBox_DISPINTERFACE_DEFINED__
#define __IfmBox_DISPINTERFACE_DEFINED__

/* dispinterface IfmBox */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmBox,0xD012FFFE,0x4679,0x4802,0xB5,0x8A,0xDD,0x30,0x14,0xA0,0x71,0x86);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D012FFFE-4679-4802-B58A-DD3014A07186")
    IfmBox : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmBoxVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmBox * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmBox * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmBox * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmBox * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmBox * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmBox * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmBox * This,
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
    } IfmBoxVtbl;

    interface IfmBox
    {
        CONST_VTBL struct IfmBoxVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmBox_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmBox_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmBox_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmBox_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmBox_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmBox_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmBox_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmBox_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmBox,0xA3406029,0x876E,0x4D7E,0x97,0x1E,0xCF,0x4A,0x3C,0x6A,0xBE,0x74);

#ifdef __cplusplus

class DECLSPEC_UUID("A3406029-876E-4D7E-971E-CF4A3C6ABE74")
fmBox;
#endif

#ifndef __IfmBrowser_DISPINTERFACE_DEFINED__
#define __IfmBrowser_DISPINTERFACE_DEFINED__

/* dispinterface IfmBrowser */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmBrowser,0x3BEC7DED,0x7805,0x4A75,0x90,0x68,0x1F,0x9D,0xC0,0x80,0xD8,0x3E);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("3BEC7DED-7805-4A75-9068-1F9DC080D83E")
    IfmBrowser : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmBrowserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmBrowser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmBrowser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmBrowser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmBrowser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmBrowser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmBrowser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmBrowser * This,
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
    } IfmBrowserVtbl;

    interface IfmBrowser
    {
        CONST_VTBL struct IfmBrowserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmBrowser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmBrowser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmBrowser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmBrowser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmBrowser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmBrowser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmBrowser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmBrowser_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmBrowser,0x2389DA5C,0x3CC1,0x4860,0xAA,0xA1,0x1E,0x99,0x77,0x1A,0x1A,0xC2);

#ifdef __cplusplus

class DECLSPEC_UUID("2389DA5C-3CC1-4860-AAA1-1E99771A1AC2")
fmBrowser;
#endif

#ifndef __IfmButton_DISPINTERFACE_DEFINED__
#define __IfmButton_DISPINTERFACE_DEFINED__

/* dispinterface IfmButton */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmButton,0x6ED765AC,0x16A4,0x4038,0xA1,0x81,0x7D,0x5D,0x0C,0x00,0xFD,0x3A);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6ED765AC-16A4-4038-A181-7D5D0C00FD3A")
    IfmButton : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmButtonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmButton * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmButton * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmButton * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmButton * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmButton * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmButton * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmButton * This,
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
    } IfmButtonVtbl;

    interface IfmButton
    {
        CONST_VTBL struct IfmButtonVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmButton_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmButton_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmButton_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmButton_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmButton_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmButton_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmButton_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmButton_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmButton,0xD3D48E2D,0xED6F,0x4BE8,0xAB,0x9A,0x0E,0x8E,0x35,0xD3,0x1F,0x36);

#ifdef __cplusplus

class DECLSPEC_UUID("D3D48E2D-ED6F-4BE8-AB9A-0E8E35D31F36")
fmButton;
#endif

#ifndef __IfmCheck_DISPINTERFACE_DEFINED__
#define __IfmCheck_DISPINTERFACE_DEFINED__

/* dispinterface IfmCheck */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmCheck,0x7E2C96E8,0xFBD9,0x4332,0x83,0x1A,0x8F,0xC5,0x0D,0x65,0x55,0x4B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7E2C96E8-FBD9-4332-831A-8FC50D65554B")
    IfmCheck : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmCheckVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmCheck * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmCheck * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmCheck * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmCheck * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmCheck * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmCheck * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmCheck * This,
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
    } IfmCheckVtbl;

    interface IfmCheck
    {
        CONST_VTBL struct IfmCheckVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmCheck_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmCheck_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmCheck_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmCheck_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmCheck_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmCheck_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmCheck_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmCheck_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmCheck,0xDE98FF5A,0xC342,0x472F,0x8C,0x00,0x19,0x39,0x92,0xCD,0x37,0xF1);

#ifdef __cplusplus

class DECLSPEC_UUID("DE98FF5A-C342-472F-8C00-193992CD37F1")
fmCheck;
#endif

#ifndef __IfmCombo_DISPINTERFACE_DEFINED__
#define __IfmCombo_DISPINTERFACE_DEFINED__

/* dispinterface IfmCombo */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmCombo,0x27F81344,0x9295,0x457A,0xB5,0xFD,0x10,0xE3,0x4D,0x2D,0xE2,0x5C);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("27F81344-9295-457A-B5FD-10E34D2DE25C")
    IfmCombo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmComboVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmCombo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmCombo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmCombo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmCombo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmCombo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmCombo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmCombo * This,
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
    } IfmComboVtbl;

    interface IfmCombo
    {
        CONST_VTBL struct IfmComboVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmCombo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmCombo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmCombo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmCombo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmCombo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmCombo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmCombo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmCombo_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmCombo,0xB8FBE3AE,0xB5EF,0x45A7,0x91,0xB7,0x66,0x7D,0x49,0x2B,0x30,0x34);

#ifdef __cplusplus

class DECLSPEC_UUID("B8FBE3AE-B5EF-45A7-91B7-667D492B3034")
fmCombo;
#endif

#ifndef __IfmEdit_DISPINTERFACE_DEFINED__
#define __IfmEdit_DISPINTERFACE_DEFINED__

/* dispinterface IfmEdit */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmEdit,0xCD6C18AB,0xF621,0x4BFB,0xA3,0x7D,0x6D,0x2D,0x80,0xD3,0x1B,0xAA);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CD6C18AB-F621-4BFB-A37D-6D2D80D31BAA")
    IfmEdit : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmEditVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmEdit * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmEdit * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmEdit * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmEdit * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmEdit * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmEdit * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmEdit * This,
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
    } IfmEditVtbl;

    interface IfmEdit
    {
        CONST_VTBL struct IfmEditVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmEdit_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmEdit_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmEdit_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmEdit_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmEdit_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmEdit_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmEdit_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmEdit_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmEdit,0xC0DA2002,0xE373,0x4CC3,0x8D,0x00,0xD6,0x34,0x23,0xFE,0xD2,0xC6);

#ifdef __cplusplus

class DECLSPEC_UUID("C0DA2002-E373-4CC3-8D00-D63423FED2C6")
fmEdit;
#endif

#ifndef __IfmEditEx_DISPINTERFACE_DEFINED__
#define __IfmEditEx_DISPINTERFACE_DEFINED__

/* dispinterface IfmEditEx */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmEditEx,0xCA3A7032,0x2513,0x44B9,0x85,0xBB,0xE1,0x00,0x6A,0x5F,0xC4,0xBE);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CA3A7032-2513-44B9-85BB-E1006A5FC4BE")
    IfmEditEx : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmEditExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmEditEx * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmEditEx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmEditEx * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmEditEx * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmEditEx * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmEditEx * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmEditEx * This,
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
    } IfmEditExVtbl;

    interface IfmEditEx
    {
        CONST_VTBL struct IfmEditExVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmEditEx_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmEditEx_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmEditEx_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmEditEx_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmEditEx_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmEditEx_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmEditEx_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmEditEx_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmEditEx,0x06E81FC9,0x0C6D,0x406F,0xA7,0xB0,0x61,0xD9,0x9B,0xCC,0x94,0xE5);

#ifdef __cplusplus

class DECLSPEC_UUID("06E81FC9-0C6D-406F-A7B0-61D99BCC94E5")
fmEditEx;
#endif

#ifndef __IfmGrid_DISPINTERFACE_DEFINED__
#define __IfmGrid_DISPINTERFACE_DEFINED__

/* dispinterface IfmGrid */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmGrid,0xED8E198C,0x161B,0x401A,0x87,0xD0,0x2E,0x46,0x27,0xE2,0xEF,0x96);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("ED8E198C-161B-401A-87D0-2E4627E2EF96")
    IfmGrid : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmGridVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmGrid * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmGrid * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmGrid * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmGrid * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmGrid * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmGrid * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmGrid * This,
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
    } IfmGridVtbl;

    interface IfmGrid
    {
        CONST_VTBL struct IfmGridVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmGrid_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmGrid_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmGrid_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmGrid_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmGrid_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmGrid_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmGrid_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmGrid_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmGrid,0x931B6CC4,0xF888,0x4720,0x84,0xDE,0x0B,0x48,0x2B,0x76,0x70,0xF0);

#ifdef __cplusplus

class DECLSPEC_UUID("931B6CC4-F888-4720-84DE-0B482B7670F0")
fmGrid;
#endif

#ifndef __IfmGroup_DISPINTERFACE_DEFINED__
#define __IfmGroup_DISPINTERFACE_DEFINED__

/* dispinterface IfmGroup */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmGroup,0x0AE7F991,0x14C7,0x43C3,0x99,0xB1,0x46,0x75,0xDA,0xC7,0x48,0x2B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0AE7F991-14C7-43C3-99B1-4675DAC7482B")
    IfmGroup : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmGroupVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmGroup * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmGroup * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmGroup * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmGroup * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmGroup * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmGroup * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmGroup * This,
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
    } IfmGroupVtbl;

    interface IfmGroup
    {
        CONST_VTBL struct IfmGroupVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmGroup_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmGroup_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmGroup_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmGroup_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmGroup_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmGroup_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmGroup_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmGroup_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmGroup,0xDE6BF64F,0x33D7,0x45BA,0x80,0x31,0x10,0x30,0x99,0x81,0x84,0x0D);

#ifdef __cplusplus

class DECLSPEC_UUID("DE6BF64F-33D7-45BA-8031-10309981840D")
fmGroup;
#endif

#ifndef __IfmLabel_DISPINTERFACE_DEFINED__
#define __IfmLabel_DISPINTERFACE_DEFINED__

/* dispinterface IfmLabel */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmLabel,0x4846F8F7,0x758E,0x4CF7,0x87,0x0F,0xD1,0xAE,0x21,0x8F,0x75,0xB1);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4846F8F7-758E-4CF7-870F-D1AE218F75B1")
    IfmLabel : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmLabelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmLabel * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmLabel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmLabel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmLabel * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmLabel * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmLabel * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmLabel * This,
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
    } IfmLabelVtbl;

    interface IfmLabel
    {
        CONST_VTBL struct IfmLabelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmLabel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmLabel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmLabel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmLabel_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmLabel_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmLabel_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmLabel_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmLabel_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmLabel,0x59AA397A,0xB4E7,0x4305,0x97,0x8F,0xD4,0x12,0xFC,0x0F,0x5E,0x41);

#ifdef __cplusplus

class DECLSPEC_UUID("59AA397A-B4E7-4305-978F-D412FC0F5E41")
fmLabel;
#endif

#ifndef __IfmMemo_DISPINTERFACE_DEFINED__
#define __IfmMemo_DISPINTERFACE_DEFINED__

/* dispinterface IfmMemo */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmMemo,0x8B328127,0x73D1,0x4510,0xA6,0x76,0xB2,0xF5,0x6B,0x24,0xC3,0x51);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("8B328127-73D1-4510-A676-B2F56B24C351")
    IfmMemo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmMemoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmMemo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmMemo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmMemo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmMemo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmMemo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmMemo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmMemo * This,
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
    } IfmMemoVtbl;

    interface IfmMemo
    {
        CONST_VTBL struct IfmMemoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmMemo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmMemo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmMemo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmMemo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmMemo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmMemo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmMemo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmMemo_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmMemo,0x70A38E58,0xD3A5,0x4741,0x80,0x61,0xD5,0x76,0x1E,0xEB,0xC2,0x5B);

#ifdef __cplusplus

class DECLSPEC_UUID("70A38E58-D3A5-4741-8061-D5761EEBC25B")
fmMemo;
#endif

#ifndef __IfmObject_DISPINTERFACE_DEFINED__
#define __IfmObject_DISPINTERFACE_DEFINED__

/* dispinterface IfmObject */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmObject,0x45FDC0D0,0xA48C,0x403C,0xAD,0x72,0xE3,0x83,0xFC,0xCD,0x78,0x4B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("45FDC0D0-A48C-403C-AD72-E383FCCD784B")
    IfmObject : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmObject * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmObject * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmObject * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmObject * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmObject * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmObject * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmObject * This,
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
    } IfmObjectVtbl;

    interface IfmObject
    {
        CONST_VTBL struct IfmObjectVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmObject_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmObject_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmObject_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmObject_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmObject_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmObject_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmObject_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmObject_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmObject,0xF806408F,0x3F0E,0x477D,0x99,0x34,0x81,0x80,0xD1,0xF0,0x3E,0xBF);

#ifdef __cplusplus

class DECLSPEC_UUID("F806408F-3F0E-477D-9934-8180D1F03EBF")
fmObject;
#endif

#ifndef __IfmOut_DISPINTERFACE_DEFINED__
#define __IfmOut_DISPINTERFACE_DEFINED__

/* dispinterface IfmOut */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmOut,0xCCBF17FF,0xB2D8,0x4122,0xA2,0xD1,0x80,0xFD,0xD7,0xB5,0xBA,0x8C);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("CCBF17FF-B2D8-4122-A2D1-80FDD7B5BA8C")
    IfmOut : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmOutVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmOut * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmOut * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmOut * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmOut * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmOut * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmOut * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmOut * This,
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
    } IfmOutVtbl;

    interface IfmOut
    {
        CONST_VTBL struct IfmOutVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmOut_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmOut_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmOut_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmOut_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmOut_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmOut_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmOut_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmOut_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmOut,0xA8996EFA,0xF4B6,0x4570,0xBD,0x8F,0xBF,0xD7,0xB3,0x0B,0xE8,0x18);

#ifdef __cplusplus

class DECLSPEC_UUID("A8996EFA-F4B6-4570-BD8F-BFD7B30BE818")
fmOut;
#endif

#ifndef __IfmPanel_DISPINTERFACE_DEFINED__
#define __IfmPanel_DISPINTERFACE_DEFINED__

/* dispinterface IfmPanel */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmPanel,0xF7303843,0x6D61,0x43A1,0xBE,0xB7,0xC0,0x3A,0x14,0x1B,0xC8,0x4B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F7303843-6D61-43A1-BEB7-C03A141BC84B")
    IfmPanel : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmPanelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmPanel * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmPanel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmPanel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmPanel * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmPanel * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmPanel * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmPanel * This,
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
    } IfmPanelVtbl;

    interface IfmPanel
    {
        CONST_VTBL struct IfmPanelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmPanel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmPanel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmPanel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmPanel_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmPanel_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmPanel_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmPanel_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmPanel_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmPanel,0x9003C204,0x7A44,0x4402,0x9A,0x9D,0xBA,0xC8,0xBE,0x04,0x90,0x2C);

#ifdef __cplusplus

class DECLSPEC_UUID("9003C204-7A44-4402-9A9D-BAC8BE04902C")
fmPanel;
#endif

#ifndef __IfmRadio_DISPINTERFACE_DEFINED__
#define __IfmRadio_DISPINTERFACE_DEFINED__

/* dispinterface IfmRadio */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmRadio,0x233923D8,0xD9A5,0x4F28,0xAF,0xE2,0xAB,0xB4,0x81,0x96,0x03,0xBD);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("233923D8-D9A5-4F28-AFE2-ABB4819603BD")
    IfmRadio : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmRadioVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmRadio * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmRadio * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmRadio * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmRadio * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmRadio * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmRadio * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmRadio * This,
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
    } IfmRadioVtbl;

    interface IfmRadio
    {
        CONST_VTBL struct IfmRadioVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmRadio_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmRadio_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmRadio_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmRadio_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmRadio_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmRadio_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmRadio_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmRadio_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmRadio,0x24327C60,0xD2E2,0x4173,0xB9,0x7E,0x4F,0x70,0xB6,0x51,0xA6,0xCE);

#ifdef __cplusplus

class DECLSPEC_UUID("24327C60-D2E2-4173-B97E-4F70B651A6CE")
fmRadio;
#endif

#ifndef __IfmSheet_DISPINTERFACE_DEFINED__
#define __IfmSheet_DISPINTERFACE_DEFINED__

/* dispinterface IfmSheet */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmSheet,0x59E54F8E,0xA9E5,0x4799,0x8F,0x09,0x79,0xFE,0x7B,0x3F,0x99,0x9A);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("59E54F8E-A9E5-4799-8F09-79FE7B3F999A")
    IfmSheet : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmSheetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmSheet * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmSheet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmSheet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmSheet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmSheet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmSheet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmSheet * This,
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
    } IfmSheetVtbl;

    interface IfmSheet
    {
        CONST_VTBL struct IfmSheetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmSheet_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmSheet_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmSheet_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmSheet_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmSheet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmSheet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmSheet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmSheet_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmSheet,0xB2D6D863,0x086A,0x4E32,0x9B,0x11,0x30,0xC0,0xB6,0x67,0x5D,0x24);

#ifdef __cplusplus

class DECLSPEC_UUID("B2D6D863-086A-4E32-9B11-30C0B6675D24")
fmSheet;
#endif

#ifndef __IfmSysm_DISPINTERFACE_DEFINED__
#define __IfmSysm_DISPINTERFACE_DEFINED__

/* dispinterface IfmSysm */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmSysm,0x156A22B2,0xE279,0x4A7E,0xAC,0x31,0x95,0xF0,0xE3,0x0D,0xBB,0x1B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("156A22B2-E279-4A7E-AC31-95F0E30DBB1B")
    IfmSysm : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmSysmVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmSysm * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmSysm * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmSysm * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmSysm * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmSysm * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmSysm * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmSysm * This,
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
    } IfmSysmVtbl;

    interface IfmSysm
    {
        CONST_VTBL struct IfmSysmVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmSysm_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmSysm_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmSysm_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmSysm_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmSysm_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmSysm_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmSysm_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmSysm_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmSysm,0x5187066F,0x8B3C,0x41FA,0x88,0x20,0x73,0x4E,0x84,0x3D,0x90,0x47);

#ifdef __cplusplus

class DECLSPEC_UUID("5187066F-8B3C-41FA-8820-734E843D9047")
fmSysm;
#endif

#ifndef __IfmTab_DISPINTERFACE_DEFINED__
#define __IfmTab_DISPINTERFACE_DEFINED__

/* dispinterface IfmTab */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmTab,0x299E30A5,0x4A51,0x412C,0xB8,0x3A,0xDE,0x20,0x95,0xDB,0x2B,0xE6);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("299E30A5-4A51-412C-B83A-DE2095DB2BE6")
    IfmTab : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmTabVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmTab * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmTab * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmTab * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmTab * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmTab * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmTab * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmTab * This,
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
    } IfmTabVtbl;

    interface IfmTab
    {
        CONST_VTBL struct IfmTabVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmTab_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmTab_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmTab_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmTab_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmTab_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmTab_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmTab_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmTab_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmTab,0xFC57D917,0x0FD2,0x41C0,0x8A,0x5C,0x4D,0xF8,0xB0,0x2E,0x0D,0x21);

#ifdef __cplusplus

class DECLSPEC_UUID("FC57D917-0FD2-41C0-8A5C-4DF8B02E0D21")
fmTab;
#endif

#ifndef __IfmTable_DISPINTERFACE_DEFINED__
#define __IfmTable_DISPINTERFACE_DEFINED__

/* dispinterface IfmTable */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmTable,0xFC28F962,0x0228,0x436F,0x86,0x59,0xB3,0x3D,0x87,0xF2,0x56,0x90);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("FC28F962-0228-436F-8659-B33D87F25690")
    IfmTable : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmTableVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmTable * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmTable * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmTable * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmTable * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmTable * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmTable * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmTable * This,
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
    } IfmTableVtbl;

    interface IfmTable
    {
        CONST_VTBL struct IfmTableVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmTable_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmTable_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmTable_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmTable_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmTable_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmTable_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmTable_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmTable_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmTable,0x64C80050,0x9551,0x4C7D,0xB8,0xA3,0x33,0xCB,0x38,0xAB,0x81,0xCE);

#ifdef __cplusplus

class DECLSPEC_UUID("64C80050-9551-4C7D-B8A3-33CB38AB81CE")
fmTable;
#endif

#ifndef __IfmTreeView_DISPINTERFACE_DEFINED__
#define __IfmTreeView_DISPINTERFACE_DEFINED__

/* dispinterface IfmTreeView */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmTreeView,0x890032BC,0x3F92,0x434A,0xB5,0x53,0x83,0x53,0xCC,0xD9,0x6F,0xBD);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("890032BC-3F92-434A-B553-8353CCD96FBD")
    IfmTreeView : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmTreeViewVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmTreeView * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmTreeView * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmTreeView * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmTreeView * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmTreeView * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmTreeView * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmTreeView * This,
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
    } IfmTreeViewVtbl;

    interface IfmTreeView
    {
        CONST_VTBL struct IfmTreeViewVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmTreeView_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmTreeView_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmTreeView_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmTreeView_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmTreeView_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmTreeView_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmTreeView_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmTreeView_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmTreeView,0x30C8755B,0xCDE0,0x4491,0xB6,0xC7,0xAE,0xB5,0xB2,0x08,0x4E,0x2C);

#ifdef __cplusplus

class DECLSPEC_UUID("30C8755B-CDE0-4491-B6C7-AEB5B2084E2C")
fmTreeView;
#endif

#ifndef __IfmUserTab_DISPINTERFACE_DEFINED__
#define __IfmUserTab_DISPINTERFACE_DEFINED__

/* dispinterface IfmUserTab */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmUserTab,0x7BDDAA17,0x02D4,0x405B,0xAF,0xF2,0xA1,0xBD,0x45,0x0A,0xDA,0x24);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("7BDDAA17-02D4-405B-AFF2-A1BD450ADA24")
    IfmUserTab : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmUserTabVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmUserTab * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmUserTab * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmUserTab * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmUserTab * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmUserTab * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmUserTab * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmUserTab * This,
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
    } IfmUserTabVtbl;

    interface IfmUserTab
    {
        CONST_VTBL struct IfmUserTabVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmUserTab_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmUserTab_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmUserTab_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmUserTab_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmUserTab_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmUserTab_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmUserTab_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmUserTab_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmUserTab,0xADA5BB3E,0x584E,0x4EFE,0x9F,0x52,0xC7,0x1F,0xDD,0x54,0x6A,0xDA);

#ifdef __cplusplus

class DECLSPEC_UUID("ADA5BB3E-584E-4EFE-9F52-C71FDD546ADA")
fmUserTab;
#endif

#ifndef __IfmCtrl_DISPINTERFACE_DEFINED__
#define __IfmCtrl_DISPINTERFACE_DEFINED__

/* dispinterface IfmCtrl */
/* [uuid] */ 


DEFINE_GUID(DIID_IfmCtrl,0xB625BCF2,0x8767,0x4EE7,0xA4,0x57,0x77,0xB6,0xBF,0x36,0x3B,0xB7);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B625BCF2-8767-4EE7-A457-77B6BF363BB7")
    IfmCtrl : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IfmCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IfmCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IfmCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IfmCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IfmCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IfmCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IfmCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IfmCtrl * This,
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
    } IfmCtrlVtbl;

    interface IfmCtrl
    {
        CONST_VTBL struct IfmCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IfmCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IfmCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IfmCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IfmCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IfmCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IfmCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IfmCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IfmCtrl_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_fmCtrl,0x3FA1F829,0x3E10,0x4D45,0xAB,0x3B,0x3D,0xDA,0x97,0xAA,0xD7,0x63);

#ifdef __cplusplus

class DECLSPEC_UUID("3FA1F829-3E10-4D45-AB3B-3DDA97AAD763")
fmCtrl;
#endif
#endif /* __Axisform_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


