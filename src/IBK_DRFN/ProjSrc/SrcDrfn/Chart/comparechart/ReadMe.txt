========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : CompareChart
========================================================================


AppWizard has created this CompareChart DLL for you.  This DLL not only
demonstrates the basics of using the Microsoft Foundation classes but
is also a starting point for writing your DLL.

This file contains a summary of what you will find in each of the files that
make up your CompareChart DLL.

CompareChart.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

CompareChart.h
	This is the main header file for the DLL.  It declares the
	CMultiChartApp class.

CompareChart.cpp
	This is the main DLL source file.  It contains the class CMultiChartApp.
	It also contains the OLE entry points required of inproc servers.

CompareChart.odl
    This file contains the Object Description Language source code for the
    type library of your DLL.

CompareChart.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

CompareChart.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\CompareChart.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.

CompareChart.def
    This file contains information about the DLL that must be
    provided to run with Microsoft Windows.  It defines parameters
    such as the name and description of the DLL.  It also exports
	functions from the DLL.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named CompareChart.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

/////////////////////////////////////////////////////////////////////////////
[주요작업]
@080520-001 CodeInput삭제
@080520-002 조회단위 조정
@080520-003 테스트작업 
@080520-004 일/주/월등의 변경만 하도록 처리하는 루틴.

