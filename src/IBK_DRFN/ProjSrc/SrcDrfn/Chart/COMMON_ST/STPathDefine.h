
#ifndef __STPATHDEFINE_H__
#define __STPATHDEFINE_H__

// 파일경로 매크로 추가
// 기존의 STControllerDef.h 에 추가하지 않은 이유는
// 수정할 수 없는 DLL(STDllOperator.dll)에서 참조하고 있기 때문


// 	#define SDSYSTEMFILE		"\\DATA\\STDATA\\STSYSTEM\\*.SD"
// 	#define SDSYSTEMFILE_D		"\\DATA\\STDATA\\STSYSTEMD\\*.SD"
// 
// 	#define SDUSERFILE			"\\DATA\\STDATA\\STUSER\\*.SD"
// 	#define SDUSERFILE_D		"\\DATA\\STDATA\\STUSERD\\*.SD"
// 
// 	#define STUSERFILE			"\\DATA\\STDATA\\STUSER\\*.ST"
// 	#define STUSERFILE_D		"\\DATA\\STDATA\\STUSERD\\*.ST"
// 
// 
// 	//#define STSD					"Data\\STDATA\\SD"
// 	//#define STSD_D				"Data\\STDATA\\SDD"
// 	//#define STSYSTEM				"Data\\STDATA\\STSystem"
// 	//#define STSYSTEM_D			"Data\\STDATA\\STSystemD"
// 	//#define STUSER				"User\\STDATA\\SD"
// 	//#define STUSER_D				"User\\STDATA\\SDD"
// 	#define PATH_DATA_STDATA_SD				"data\\stdata\\sd"
// 	#define PATH_DATA_STDATA_SD_DEBUG		"data\\stdata\\sdd"
// 	#define PATH_DATA_STDATA_STSYSTEM		"data\\stdata\\stsystem"
// 	#define PATH_DATA_STDATA_STSYSTEM_DEBUG	"data\\stdata\\stsystemd"
// 	#define PATH_USER_STDATA_SD				"usr\\stdata\\sd"
// 	#define PATH_USER_STDATA_SD_DEBUG		"usr\\stdata\\sdd"
// 
// 	#define STSTDIR				"Data\\STDATA\\ST"
// 
// 	//#define STFOLDER_SD			"STDATA\\SD"
// 	//#define STFOLDER_SDD		"STDATA\\SDD"
// 	//#define STFOLDER_ST			"STDATA\\ST"
// 	//#define STFOLDER_STSYSTEM	"STDATA\\STSystem"
// 	//#define STFOLDER_STSYSTEMD	"STDATA\\STSystemD"
// 	#define PATH_STDATA_SD				"stdata\\sd"
// 	#define PATH_STDATA_SD_DEBUG		"stdata\\sdd"
// 	#define PATH_STDATA_ST				"stdata\\st"
// 	#define PATH_STDATA_STSYSTEM		"stdata\\stsystem"
// 	#define PATH_STDATA_STSYSTEM_DEBUG	"stdata\\stsystemd"
// 
// 
// 	#define STSYSTEM_FOLDER		"\\STDATA\\STSYSTEM"
// 	#define STSYSTEMD_FOLDER	"\\STDATA\\STSYSTEMD"
// 	#define STUSER_FOLDER		"\\STDATA\\STUSER"
// 	#define STUSERD_FOLDER		"\\STDATA\\STUSERD"
// 
// 	#define STSYSTEM_STFILE		"\\STDATA\\STSYSTEM\\*.ST"
// 	#define STSYSTEMD_STFILE	"\\STDATA\\STSYSTEMD\\*.ST"
// 	#define STUSER_STFILE		"\\STDATA\\STUSER\\*.ST"
// 	#define STUSERD_STFILE		"\\STDATA\\STUSERD\\*.ST"
// 
// 	//{{ ST Macro
// 	#define	STDATA_SD_DEBUG				"\\STDATA\\SDD"
// 	#define	STDATA_SD_RELEASE			"\\STDATA\\SD"
// 	#define	STDATA_ST					"\\STDATA\\ST"
// 	#define	STDATA_ST_USERCOMBINE		"\\STDATA\\ST\\사용자조합전략"
// 	//}} ST Macro

//-->@Solomon
#define SDSYSTEMFILE		"\\Icss\\STDATA\\STSYSTEM\\*.SD"
#define SDSYSTEMFILE_D		"\\Icss\\STDATA\\STSYSTEMD\\*.SD"

#define SDUSERFILE			"\\Icss\\STDATA\\STUSER\\*.SD"
#define SDUSERFILE_D		"\\Icss\\STDATA\\STUSERD\\*.SD"

#define STUSERFILE			"\\Icss\\STDATA\\STUSER\\*.ST"
#define STUSERFILE_D		"\\Icss\\STDATA\\STUSERD\\*.ST"

#define PATH_DATA_STDATA_SD				"Icss\\stdata\\sd"
#define PATH_DATA_STDATA_SD_DEBUG		"Icss\\stdata\\sdd"
#define PATH_DATA_STDATA_STSYSTEM		"Icss\\stdata\\stsystem"
#define PATH_DATA_STDATA_STSYSTEM_DEBUG	"Icss\\stdata\\stsystemd"
#define PATH_USER_STDATA_SD				"usr\\stdata\\sd"
#define PATH_USER_STDATA_SD_DEBUG		"usr\\stdata\\sdd"

#define STSTDIR				"Icss\\STDATA\\ST"

#define PATH_STDATA_SD				"stdata\\sd"
#define PATH_STDATA_SD_DEBUG		"stdata\\sdd"
#define PATH_STDATA_ST				"stdata\\st"
#define PATH_STDATA_STSYSTEM		"stdata\\stsystem"
#define PATH_STDATA_STSYSTEM_DEBUG	"stdata\\stsystemd"


#define STSYSTEM_FOLDER		"\\STDATA\\STSYSTEM"
#define STSYSTEMD_FOLDER	"\\STDATA\\STSYSTEMD"
#define STUSER_FOLDER		"\\STDATA\\STUSER"
#define STUSERD_FOLDER		"\\STDATA\\STUSERD"

#define STSYSTEM_STFILE		"\\STDATA\\STSYSTEM\\*.ST"
#define STSYSTEMD_STFILE	"\\STDATA\\STSYSTEMD\\*.ST"
#define STUSER_STFILE		"\\STDATA\\STUSER\\*.ST"
#define STUSERD_STFILE		"\\STDATA\\STUSERD\\*.ST"

//{{ ST Macro
#define	STDATA_SD_DEBUG				"\\STDATA\\SDD"
#define	STDATA_SD_RELEASE			"\\STDATA\\SD"
#define	STDATA_ST					"\\STDATA\\ST"
#define	STDATA_ST_USERCOMBINE		"\\STDATA\\ST\\사용자조합전략"
//}} ST Macro

//<--@Solomon
#endif // __STPATHDEFINE_H__
