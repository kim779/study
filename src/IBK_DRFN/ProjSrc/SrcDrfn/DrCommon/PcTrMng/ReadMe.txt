========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : PcTrMng
========================================================================


AppWizard has created this PcTrMng DLL for you.  This DLL not only
demonstrates the basics of using the Microsoft Foundation classes but
is also a starting point for writing your DLL.

This file contains a summary of what you will find in each of the files that
make up your PcTrMng DLL.

PcTrMng.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

PcTrMng.h
	This is the main header file for the DLL.  It declares the
	CPcTrMngApp class.

PcTrMng.cpp
	This is the main DLL source file.  It contains the class CPcTrMngApp.


PcTrMng.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

PcTrMng.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\PcTrMng.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.

PcTrMng.def
    This file contains information about the DLL that must be
    provided to run with Microsoft Windows.  It defines parameters
    such as the name and description of the DLL.  It also exports
	functions from the DLL.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named PcTrMng.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////

메인에서 직접 넘어온다.



/*
	// IDataObject
	BEGIN_INTERFACE_PART(DataObject, IDataObject)
		INIT_INTERFACE_PART(COleControl, DataObject)
		STDMETHOD(GetData)(LPFORMATETC, LPSTGMEDIUM);
		STDMETHOD(GetDataHere)(LPFORMATETC, LPSTGMEDIUM);
		STDMETHOD(QueryGetData)(LPFORMATETC);
		STDMETHOD(GetCanonicalFormatEtc)(LPFORMATETC, LPFORMATETC);
		STDMETHOD(SetData)(LPFORMATETC, LPSTGMEDIUM, BOOL);
		STDMETHOD(EnumFormatEtc)(DWORD, LPENUMFORMATETC*);
		STDMETHOD(DAdvise)(LPFORMATETC, DWORD, LPADVISESINK, LPDWORD);
		STDMETHOD(DUnadvise)(DWORD);
		STDMETHOD(EnumDAdvise)(LPENUMSTATDATA*);
	END_INTERFACE_PART_STATIC(DataObject)
*/


/* -----------------------------------------------------------------------------
01. function명      : {}
02. 작  성  자      : {}
03. 작 성 일 자     : {}
04. function기능    : {}
05. 입력인자        : {}
06. 출력인자        : {}
07. return 값       : {}
08. REMARKS 사항    : {}
----------------------------------------------------------------------------- */
