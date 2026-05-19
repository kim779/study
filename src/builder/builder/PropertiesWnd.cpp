
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "axisBuilder.h"

#include "h/mainvar.h"
#include "awTool/awObjectLoad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Properties String
//	Caption | Control Type | Value1 | Value2 | form apply
//	Caption		: Property Head Caption
//	Control Type	: 1 - Edit Control,
//			  2 - Edit + Button
//			  3 - Combo
//			  4 - Button
//			  1001 - font dialog
//			  1002 - color dialog
//			  1003 - File Dialog
//			  1004 - attributes Dialog (grid/table 공용)
//			  1005 - Combo Dialog
//			  1007 - Cell Edit Format
//			  1008 - map option dialog
//			  1009 - column edit dialog
//			  1010 - public Dialog
//			  1011 - cell combo Dialog
//			  1012 - control setup dialog
//			  1013 - user tab page dialog
//			  1014 - inputkey combo
//			  1015 - Edit Format
//			  1016 - CoRGB
//
//	Value1		: Edit일때 4자리 속성 지정 XXXX 첫번째 0:Normal, 1:Disable, 2: Numeric 
//			  2~4번째 자리는 Limit Text
//			  Combo일때 콤보 리스트 $로 시작하는건 정해진 keyword이며, 목록은 "data-text;"로 나타낸다.
//	Value2		: combo에서 선택된 값처럼 폼에 들어갈 값을 지정해준다.
//			  Button 클릭시에 실행될 Dialog Number
//	form apply	: 폼에 적용될 keyword
//////////////////////////////////////////////////////////////////////

//#define	FM_RADIO	100
//#define	FM_CHECK	101
//#define	FM_GRIDCELL	102
//#define	FM_TABLEROW	103
//#define	FM_TABSEL	104
//#define	FM_UTABSEL	105
#define	fmGRIDCELL	102
#define	fmGRIDEXCELL	106
#define	fmTABLEROW	103
#define	fmTABSEL	104
#define	fmUTABSEL	105
#define	fmCHILDMAP	-1
#define	fmTEMPLATE	-2
#define	fmPROCEDURES	-3
#define	fmLAYOUT	-4


/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar

CPropertiesWnd::CPropertiesWnd()
{
	m_mapH      = NULL;

	//m_infoMapHTS.SetAt(FM_SYS,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tKind|3|1-yyyy/mm/dd;2-hh:mm:ss;3-Term;||type\tAlignment|3|$alignment||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fontStyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tTransparent|3|$bool|0x00100000|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_infoMapHTS.SetAt(fmLABEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|strings\tVerticalText|3|$bool|0x00000080|styles\tAlignment|3|$alignment||alignment\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tTransparent|3|$bool|0x00100000|styles");
	m_infoMapHTS.SetAt(fmBOX,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tKind|3|0-Line;1-Style3D;||charType\tThick|1|||size\tBoxColor(H)|2|0|1002|paintColor|Set horizontal border line color\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility");
	m_infoMapHTS.SetAt(fmGROUP,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|strings|Enter Group Name\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tForeColor|2|0|1002|textColor|Specify letter color\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility");
	m_infoMapHTS.SetAt(fmIMVIEW,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tType|3|0-Line;1-3DBorder;2-None;||charType\tBorder|3|0-None;1-Raised;2-Sunken;3-Bump;4-Etched;5-Line;||borderStyle\tKind|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|styles\tImage|2||1003|images\tImageType|3|0-Center;6-Tile;5-Stretch;||alignmentImage\tText|1||1|strings\tVerticalText|3|$bool|0x00000080|styles\tAlignment|3|$alignment||alignment\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tLineColor|2||1002|borderColor|Specify line color\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tHoverImage|3|$bool|0x00000020|styles\tTransparent|3|$bool|0x00100000|styles");
	m_infoMapHTS.SetAt(fmBUTTON,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tType|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|styles\tImage|2||1003|images\tImageType|3|0-Center;6-Tile;5-Stretch;||alignmentImage\tText|1||1|strings\tVerticalText|3|$bool|0x00000080|styles\tAlignment|3|$alignment||alignment\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||keyParam\tTabOrder|1|1||index|Set tab order\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tEnable|3|$bool|0x00000001|styles|Choose control usability \tHoverImage|3|$bool|0x00000020|styles\tFocus|3|$bool|0x00000004|styles");
	m_infoMapHTS.SetAt(fmEDIT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;5-Digit;||charType\tMode|3|1-Input;3-Both;||ioType\tLength|1|||size|Set the length of character(s)\tInitialValue|1|||strings\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tTabOrder|1|1||index|Set tab order\tTabSkip|3|$bool|0x00002000|attributes\tSpin|3|0-None;1-Normal;100-Kospi;110-Future;111-Spread;120-Call Option;130-Put Option;140-Kospi Option;200-Kosdaq;210-KQ Future;220-KQ Call Option;230-KQ Put Option;||keyParam\tSpinDelta|1|||AkeyValues\tEncrypt|3|$bool|0x00010000|attributes\tEncryptKey|1|||keyNames\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tEnable|3|$bool|0x00000001|styles|Choose control usability \tShowFormat|3|$bool|0x00000040|styles|Choose format option\tNoFloating|3|$bool|0x00004000|attributes|Inputing Decimal Point is not allowed\tFloatDigit|1|||offsets\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||auxAttributes\tRTS Symbol|1|||rts|rts symbol.\tAutoHScroll|3|$bool|0x20000000|attributes\tCodeList|3|$bool|0x00000080|styles\tAutoForm|3|$bool|0x00010000|styles\tEditFormat|4||1015|xxx\tAttributes|4||1004|xxx");
	m_infoMapHTS.SetAt(fmCOMBO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tSelectIndex|1|||checked\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||charType\tMode|3|0-Free;1-Input;3-Both;||ioType\tDataLength|1|||size|Set the length of combo data\tVisibleCount|1|||HkeyValues\tDataCount|2|1|1005|LkeyValues|Edit Combo Contents and display the number of contents.\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||auxValues\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tTabOrder|1|1||index|Set tab order\tTabSkip|3|$bool|0x00002000|attributes\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tAccount|3|$bool|0x00000008|styles|Set account combo option\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tEnable|3|$bool|0x00000001|styles|Choose control usability \tEditFormat|4||1015|xxx\tAttributes|4||1004|xxx");
	m_infoMapHTS.SetAt(fmOUTPUT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||charType\tLength|1|||size\tAlignment|3|$alignment||alignment\tLineType|3|0-Line;1-3DBorder;2-None;||ioType\tLineColor|2||1002|borderColor|Specify line color\tBorder3D|3|0-None;1-Raised;2-Sunken;3-Bump;4-Etched;5-Line;||borderStyle\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tTabOrder|1|1||index|Set tab order\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tDominoName|1|||keyNames\tReference|1|||referenceName\tFloatDigit|1|||offsets\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tEnable|3|$bool|0x00000001|styles|Choose control usability \tRTS Symbol|1|||rts|rts symbol.\tSum(OnAlert)|3|$bool|0x00004000|styles\tEditFormat|4||1015|xxx\tEditCORGB|4||1016|xxx\tAttributes|4||1004|xxx");
	m_infoMapHTS.SetAt(fmMEMO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tMode|3|1-Input;2-Output;3-Both;||ioType\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color.\tTabOrder|1|1||index|Set tab order.\tBorder|3|$bool||borderStyle\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tEnable|3|$bool|0x00000001|styles|Choose control usability.");
	m_infoMapHTS.SetAt(fmGRID,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tRows|1|||rows\tColumns|1||1|cols\tFixedColumn#|1|||fixedColumn|Set the fixed number of column.\tFlexibleRows|3|$bool|0x00010000|auxAttributes|Set the number of variable rows.\tFlexibleMode|3|0-None;1-Expand;|0x01000000|auxAttributes|Set the mode of variable rows.\tScrollValidRows|3|$bool|0x00001000|auxAttributes\tAutoSize|3|$bool|0x80000000|auxAttributes|Automatic Setting option for Row Height?\tVisibleRows|1|||HkeyValues|Set the number of visible rows when AutoSize is set true.\tCellHeight|1|||LkeyValues|Set the height of cell when AutoSize is set false.\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tDataAppend|3|0-Bottom;1-Top;|0x00000008|auxAttributes|Set Data inserted direction.\tLineColor|2||1002|borderColor|Set the grid line color.\tRowColor|3|$bool|0x00000020|auxAttributes|Choose 1st and 2nd Row Color option?.\tRowsOfColor|1|||rowLines|Specify rows for each row color\t1stRowColor|2||1002|paintColor2|Specify color for odd numbered row?\t2ndRowColor|2||1002|paintColor1|Specify color for even numbered row?\tTabOrder|1|1||index|Set tab order.\tVariant|1|||strings\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tFocus|3|0-None;1-CellBox;2-RowBox;3-RowBar;4-Record;||whatEdit|Set focus option.\tFocusColor|2||1002|auxValues|Specify focus color\tFocusText|3|$bool|0x08000000|auxAttributes|Keep ForeColor on focus\tKeyFocus|3|$bool|0x00200000|auxAttributes\tMerge|3|$bool|0x00000400|auxAttributes|Set merge option for the grid header.\tSelectionAlways|3|$bool|0x00800000|auxAttributes|Set selection Always option.\tNoEffect3D|3|$bool|0x00020000|auxAttributes|Set no 3 dimension effects option.\tNoHead|3|$bool|0x00000010|auxAttributes|Set no grid header option.\tNoVerLine|3|$bool|0x00000040|auxAttributes|Set no vertical line option.\tNoHorLine|3|$bool|0x00000080|auxAttributes|Set no horizontal line option.\tNoResizeWidth|3|$bool|0x02000000|auxAttributes\tAutoWidth|3|$bool|0x00000800|auxAttributes\tModifyFlag|3|$bool|0x00400000|auxAttributes\tTabLeave|3|$bool|0x00000010|attributes\tDrop|3|$bool|0x80000000|attributes|Set data drop option.\tColumnReedit|3|$bool|0x00010000|attributes|Set editing of grid column option.\tPageScroll|3|$bool|0x00040000|auxAttributes|Set page scroll option.\tResizable|3|$bool|0x00000200|auxAttributes\tHeadInformation|3|$bool|0x00000100|auxAttributes|Set grid header information transmission option.\tTable|3|$bool|0x00080000|table\tThumb|3|$bool|0x00100000|auxAttributes\tColumns...|4||1009|xxx");
	m_infoMapHTS.SetAt(fmGRIDEX,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tRows|1|||rows\tColumns|1||1|cols\tFixedColumn#|1|||fixedColumn|Set the fixed number of column.\tFlexibleRows|3|$bool|0x00010000|auxAttributes|Set the number of variable rows.\tFlexibleMode|3|0-None;1-Expand;|0x01000000|auxAttributes|Set the mode of variable rows.\tScrollValidRows|3|$bool|0x00001000|auxAttributes\tAutoSize|3|$bool|0x80000000|auxAttributes|Automatic Setting option for Row Height?\tVisibleRows|1|||HkeyValues|Set the number of visible rows when AutoSize is set true.\tCellHeight|1|||LkeyValues|Set the height of cell when AutoSize is set false.\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tDataAppend|3|0-Bottom;1-Top;|0x00000008|auxAttributes|Set Data inserted direction.\tLineColor|2||1002|borderColor|Set the grid line color.\tRowColor|3|$bool|0x00000020|auxAttributes|Choose 1st and 2nd Row Color option?.\tRowsOfColor|1|||rowLines|Specify rows for each row color\t1stRowColor|2||1002|paintColor2|Specify color for odd numbered row?\t2ndRowColor|2||1002|paintColor1|Specify color for even numbered row?\tTabOrder|1|1||index|Set tab order.\tVariant|1|||strings\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tFocus|3|0-None;1-CellBox;2-RowBox;3-RowBar;4-Record;||whatEdit|Set focus option.\tFocusColor|2||1002|auxValues|Specify focus color\tFocusText|3|$bool|0x08000000|auxAttributes|Keep ForeColor on focus\tKeyFocus|3|$bool|0x00200000|auxAttributes\tMerge|3|$bool|0x00000400|auxAttributes|Set merge option for the grid header.\tSelectionAlways|3|$bool|0x00800000|auxAttributes|Set selection Always option.\tNoEffect3D|3|$bool|0x00020000|auxAttributes|Set no 3 dimension effects option.\tNoHead|3|$bool|0x00000010|auxAttributes|Set no grid header option.\tNoVerLine|3|$bool|0x00000040|auxAttributes|Set no vertical line option.\tNoHorLine|3|$bool|0x00000080|auxAttributes|Set no horizontal line option.\tNoResizeWidth|3|$bool|0x02000000|auxAttributes\tAutoWidth|3|$bool|0x00000800|auxAttributes\tModifyFlag|3|$bool|0x00400000|auxAttributes\tTabLeave|3|$bool|0x00000010|attributes\tDrop|3|$bool|0x80000000|attributes|Set data drop option.\tColumnReedit|3|$bool|0x00010000|attributes|Set editing of grid column option.\tPageScroll|3|$bool|0x00040000|auxAttributes|Set page scroll option.\tResizable|3|$bool|0x00000200|auxAttributes\tHeadInformation|3|$bool|0x00000100|auxAttributes|Set grid header information transmission option.\tTable|3|$bool|0x00080000|table\tThumb|3|$bool|0x00100000|auxAttributes\tColumns...|4||1009|xxx");
	m_infoMapHTS.SetAt(fmTABLE,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tCount|1||1|rows\tAutoSize|3|$bool|0x80000000|auxAttributes\tCellHeight|1|||AkeyValues\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tStyle|3|0-Combine;1-Head;2-Data;||tableHead\tLineColor|2||1002|borderColor\tHeadWidth|1|||allcellwidth\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tFocus|3|$bool|0x00000004|styles\tNoVerLine|3|$bool|0x00000040|auxAttributes\tNoHorLine|3|$bool|0x00000080|auxAttributes\tRows...|4||1009|xxx");
	m_infoMapHTS.SetAt(fmTREE,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tLength|1|||size\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color.\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tEnable|3|$bool|0x00000001|styles|Choose control usability.\tHasLines|3|$bool|0x00000020|styles\tMultiSelection|3|$bool|0x00000004|styles\tFolder|3|$bool|0x00010000|attributes\tClick&&Send|3|$bool|0x00800000|attributes\tDrag|3|$bool|0x40000000|attributes\tIgnoreDomino|3|$bool|0x00001000|attributes");
	m_infoMapHTS.SetAt(fmFORM,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tMapName|1|0008|1|strings\tTabOrder|1|1||index|Set tab order.\tFixedSize|3|$bool||fixSize\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tResizable|3|$bool|0x00000200|auxAttributes\tIgnoreDomino|3|$bool|0x00001000|attributes");
	m_infoMapHTS.SetAt(fmTAB,	"Name|1|||name|Enter Symbol Name./tLeft|1|||left|Set left location./tTop|1|||top|Set top location./tWidth|1|||width|Set Width./tHeight|1|||height|Set Height./tSelectIndex|1|||checked|Set the initial page of the user tab./tTabCount|1||1|AkeyValues/tPosition|3|1-left;2-right;3-top;4-bottom;||alignment/tFont|2|1|1001|fontName|Choose Font Name./tFontSize|1|||fontPoint|Set Font Size./tFontStyle|3|$fontStyle||fontStyle|Choose Font Style./tImage|2||1003|images/tBackColor|2||1002|paintColor|Specify Background Color./tForeColor|2||1002|textColor|Specify letter color./tFocusBackColor|2||1002|paintColor2/tFocusForeColor|2||1002|paintColor/tHintType|3|$typeHint||typeHint/tHintText|1|||hintText/tVisible|3|$bool|0x00000002|styles|Choose Visibility./tFocus|3|$bool|0x00000004|styles/tFileLoad|3|$bool|0x00010000|attributes/tIgnoreDomino|3|$bool|0x00001000|attributes/tPageEdit|4||1013|xxx");
	m_infoMapHTS.SetAt(fmBROWSER,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tMode|3|0-None;2-Output;||ioType\tStartPage|1|||strings\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tEnable|3|$bool|0x00000001|styles|Choose control usability.\tShowTool|3|$bool|0x00000004|styles");
	m_infoMapHTS.SetAt(fmOBJECT,	"Title|1|||mapcaption|Enter title of map.\tWidth|1|||mapwidth|Enter the width of the map.\tHeight|1|||mapheight|Enter the height of the map.\tFont|2|1|1001|mapfname|Specify map's Font name.\tFontSize|1|||mapfpoint|Set map's Font size.\tFontStyle|3|$fontStyle||mapfstyle|Choose map's Font style.\tVersion|1|||mapvers|Enter version of the map.\tName|1|0008||mapname|Enter the name of the map.\tGroup|3|||mapGroup|Choose map's group\tDescription|1|0020||mapdesc|Enter the description of the map.\tViewType|3|4-Fixed;5-FixedEx;2-Resizable;0-Normal;1-Scroll;3-Static;||maptype|Set the view charType option.\tMethod|3|0-Form;1-Menu;2-Send;||mapmapM\tTxCode|1|0008||maptrxC\tTxHead|1|0032||maptrxH\tRepeat|1|||repeatV\tPublic|2|1|1010|publicR\tRTS Symbol|1|||rts|rts symbol.\tHelp|1|||maphelpN\tBackGround|3|0-None;1-Color;2-Image;||mapimageK\tBackColor|2||1002|mapBkClr\tImage|2||1003|mapimageV\tOptions|4||1008|xxx");

	m_infoMapHTS.SetAt(fmRADIO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tGroupName|1|||keyNames\tText|1||1|strings\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tMode|3|0-Free;1-Input;3-Both;||ioType\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color.\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tEnable|3|$bool|0x00000001|styles|Choose control usability.\tChecked|3|$bool|0x00000004|styles\tIgnoreDomino|3|$bool|0x00001000|attributes\tTransparent|3|$bool|0x00100000|styles");
	m_infoMapHTS.SetAt(fmCHECK,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|strings\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tMode|3|0-Free;1-Input;3-Both;||ioType\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color.\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tEnable|3|$bool|0x00000001|styles|Choose control usability.\tChecked|3|$bool|0x00000004|styles\tIgnoreDomino|3|$bool|0x00001000|attributes\tTransparent|3|$bool|0x00100000|styles");
	m_infoMapHTS.SetAt(fmGRIDCELL,	"Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||cellcharType\tMode|3|0-Free;1-Input;2-Output;3-Both;||cellioType\tType|3|0-Normal;1-Check;2-Combo;3-Button;9-Graph;||cellkind\tCheckBoxText|1|||cellstrings\tExclusiveCheck|3|$bool|0x00000004|cellstyles\tComboCount|2|1|1011|entryCombo|Edit Combo Contents and display the number of contents.\tButtonType|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|cellstyles\tButtonText|1|0256||celldisplays\tButtonImage|2||1003|cellimages\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||cellkeyParam\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellheadText\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignmentHead\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignmentText\tImageAlignment|3|0-Center;5-Stretch;6-Tile;||cellalignmentImage\tHeadBackColor|2||1002|cellheadPaintColor\tHeadForeColor|2||1002|cellheadTextColor\tDataBackColor|2||1002|cellpaintColor\tDataForeColor|2||1002|celltextColor\tHintType|3|$typeHint||celltypeHint\tHintText|1|||cellhintText\tDomino|1|||celldominoName\tReference|1|||cellreferenceName\tVisible|3|$bool|0x00000002|cellstyles\tSort|3|$bool|0x00000020|cellstyles\tShowFormat|3|$bool|0x00000040|cellstyles|Choose format option.\tFloatDigit|1|||celloffsets\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||cellauxAttributes\tIgnoreFloating0|3|$bool|0x00010000|cellattributes\tRTS Symbol|1|||cellrts|rts symbol.\tColumnSuppress|3|$bool|0x00000080|cellstyles\tEdit|4||1007|xxx\tAttributes|4||1004|xxx");
	m_infoMapHTS.SetAt(fmGRIDEXCELL,"Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||cellcharType\tMode|3|0-Free;1-Input;2-Output;3-Both;||cellioType\tType|3|0-Normal;1-Check;2-Combo;3-Button;9-Graph;||cellkind\tCheckBoxText|1|||cellstrings\tExclusiveCheck|3|$bool|0x00000004|cellstyles\tComboCount|2|1|1011|entryCombo|Edit Combo Contents and display the number of contents.\tButtonType|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|cellstyles\tButtonText|1|0256||celldisplays\tButtonImage|2||1003|cellimages\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||cellkeyParam\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellheadText\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignmentHead\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignmentText\tImageAlignment|3|0-Center;5-Stretch;6-Tile;||cellalignmentImage\tHeadBackColor|2||1002|cellheadPaintColor\tHeadForeColor|2||1002|cellheadTextColor\tDataBackColor|2||1002|cellpaintColor\tDataForeColor|2||1002|celltextColor\tHintType|3|$typeHint||celltypeHint\tHintText|1|||cellhintText\tDomino|1|||celldominoName\tReference|1|||cellreferenceName\tVisible|3|$bool|0x00000002|cellstyles\tSort|3|$bool|0x00000020|cellstyles\tShowFormat|3|$bool|0x00000040|cellstyles|Choose format option.\tFloatDigit|1|||celloffsets\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||cellauxAttributes\tIgnoreFloating0|3|$bool|0x00010000|cellattributes\tRTS Symbol|1|||cellrts|rts symbol.\tColumnSuppress|3|$bool|0x00000080|cellstyles\tEdit|4||1007|xxx\tAttributes|4||1004|xxx\tSubTotal|3|$bool|0x00008000|cellstyles\tTotal|3|$bool|0x00800000|cellstyles\tformula|1|||cellExFormula");
	m_infoMapHTS.SetAt(fmTABLEROW,	"Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||cellcharType\tLength|1|||cellsize\tHead|1||1|cellheadText\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignmentHead\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignmentText\tHeadBackColor|2||1002|cellheadPaintColor\tHeadForeColor|2||1002|cellheadTextColor\tDataBackColor|2||1002|cellpaintColor\tDataForeColor|2||1002|celltextColor\tHintType|3|$typeHint||celltypeHint\tHintText|1|||cellhintText\tFloatDigit|1|||celloffsets\tDomino|1|||celldominoName\tReference|1|||cellreferenceName\tRTS Symbol|1|||cellrts|rts symbol.\tVisible|3|$bool|0x00000002|cellstyles\tEdit|4||1007|xxx\tAttributes|4||1004|xxx");
	m_infoMapHTS.SetAt(fmTABSEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tHeadText|1||1|tabdisplays\tHeadData|1|||tabstrings\tHeadVisible|3|$bool||tabVisible\tSelectIndex|1|||checked|Set the initial page of the user tab.\tTabCount|1|||AkeyValues\tPosition|3|1-left;2-right;3-top;4-bottom;||alignment\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tImage|2||1003|images\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color.\tFocusBackColor|2||1002|paintColor2|Specify background color on the selected page.\tFocusForeColor|2||1002|paintColor1|Specify letter color on the selected page.\tVisible|3|$bool|0x00000002|styles|Choose Visibility.\tFocus|3|$bool|0x00000004|styles\tFileLoad|3|$bool|0x00010000|attributes\tIgnoreDomino|3|$bool|0x00001000|attributes\tPageEdit|4||1013|xxx");
	m_infoMapHTS.SetAt(fmCHILDMAP, "Title|1|||mapcaption|Enter title of map.\tWidth|1|||mapwidth|Enter the width of the map.\tHeight|1|||mapheight|Enter the height of the map.\tFont|2|1|1001|mapfname|Specify map's Font name.\tFontSize|1|||mapfpoint|Set map's Font size.\tFontStyle|3|$fontStyle||mapfstyle|Choose map's Font style.\tVersion|1|||mapvers|Enter version of the map.\tName|1|0008||mapname|Enter the name of the map.\tGroup|3|||mapGroup|Choose map's group\tDescription|1|0020||mapdesc|Enter the description of the map.\tViewType|3|4-Fixed;5-FixedEx;2-Resizable;0-Normal;1-Scroll;3-Static;||maptype|Set the view type option.\tTxCode|1|0008||maptrxC\tTxHead|1|0032||maptrxH\tRepeat|1|||repeatV\tPublic|2|1|1010|publicR\tHelp|1|||maphelpN\tBackGround|3|0-None;1-Color;2-Image;||mapimageK\tBackColor|2||1002|mapBkClr\tImage|2||1003|mapimageV\tOptions|4||1008|xxx");
	m_infoMapHTS.SetAt(fmTEMPLATE, "Width|1|||mapwidth|Set the template's width.\tHeight|1|||mapheight|Set the template's height.\tFont|2|1|1001|mapfname|Specify template's Font name.\tFontSize|1|||mapfpoint|Set template's Font size.\tFontStyle|3|$fontStyle||mapfstyle|Choose template's Font style.\tTemplateName|1|0008|1|mapname|Enter the name of the template.\tMapGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
	m_infoMapHTS.SetAt(fmPROCEDURES, "Title|1|||mapcaption\tVersion|1|||mapvers\tName|1|0008|1|mapname\tGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
	// Layout
	m_infoMapHTS.SetAt(fmLAYOUT, "Left|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tAvailable|3|$bool|0x00000002|styles|Choose available.");
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_infoMapMTS.SetAt(fmCHILDMAP, "Title|1|||mapcaption|Enter title of map.\tWidth|1|||mapwidth|Enter the width of the map.\tHeight|1|||mapheight|Enter the height of the map.\tFont|2|1|1001|mapfname|Specify map's Font name.\tFontSize|1|||mapfpoint|Set map's Font size.\tFontStyle|3|$fontStyle||mapfstyle|Choose map's Font style.\tVersion|1|||mapvers|Enter version of the map.\tName|1|0008||mapname|Enter the name of the map.\tGroup|3|||mapGroup|Choose map's group\tDescription|1|0020||mapdesc|Enter the description of the map.\tViewType|3|4-Fixed;5-FixedEx;2-Resizable;0-Normal;1-Scroll;3-Static;||maptype|Set the view type option.\tTxCode|1|0008||maptrxC\tTxHead|1|0032||maptrxH\tRepeat|1|||repeatV\tPublic|2|1|1010|publicR\tHelp|1|||maphelpN\tBackGround|3|0-None;1-Color;2-Image;||mapimageK\tBackColor|2||1002|mapBkClr\tImage|2||1003|mapimageV\tOptions|4||1008|xxx\tController|1|||controller|form controller\tAlpha|1|||alpha|Set alpha");
	m_infoMapMTS.SetAt(fmTEMPLATE, "Width|1|||mapwidth|Set the template's width.\tHeight|1|||mapheight|Set the template's height.\tFont|2|1|1001|mapfname|Specify template's Font name.\tFontSize|1|||mapfpoint|Set template's Font size.\tFontStyle|3|$fontStyle||mapfstyle|Choose template's Font style.\tTemplateName|1|0008|1|mapname|Enter the name of the template.\tMapGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
	m_infoMapMTS.SetAt(fmPROCEDURES, "Title|1|||mapcaption\tVersion|1|||mapvers\tName|1|0008|1|mapname\tGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
	m_infoMapMTS.SetAt(fmLAYOUT, "Left|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tAvailable|3|$bool|0x00000002|styles|Choose available.");
}

CPropertiesWnd::~CPropertiesWnd()
{
	m_infoMapHTS.RemoveAll();
	m_infoMapMTS.RemoveAll();
	m_infoMapCTMOBJ.RemoveAll();
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_CBN_SELENDOK(ID_PRO_COMBO, OnSelCombo)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 메시지 처리기

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	loadPropertyFile(MEDIA_HTS, _T("\\tab\\properties.dat"));
	loadPropertyFile(MEDIA_MTS, _T("\\tab\\properties_m.dat"));

	CRect	rectDummy;

	rectDummy.SetRectEmpty();

	// 콤보 상자를 만듭니다.
	if (!m_wndCtrlCombo.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, CRect(0,10,200,200), this, ID_PRO_COMBO))
	{
		TRACE0(_T("속성 콤보 상자를 만들지 못했습니다. \n"));
		return -1;
	}

	CImageList	ilist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_CONTROL);
	ilist.Create(17, 17, ILC_COLOR24|ILC_MASK, 26, 1);
	ilist.Add(&bitmap, RGB(0, 255, 255));
	ilist.Add(AfxGetApp()->LoadIcon(IDI_MAP));
	ilist.Add(AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	ilist.Add(AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	ilist.SetBkColor(CLR_NONE);
	m_wndCtrlCombo.SetImageList(&ilist);
	ilist.Detach();
	bitmap.Detach();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, ID_PRO_GRID))
	{
		TRACE0(_T("속성 표를 만들지 못했습니다. \n"));
		return -1;
	}

	setPropListFont();
	initPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(IDB_PROPERTIES_HC, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 정보 세팅

	adjustLayout();

	// Layout
	m_bLayout = false;

	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	adjustLayout();
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}                                                                                                                                  

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	setPropListFont();
}

void CPropertiesWnd::SetCmtObjProp(DWORD idx, CString strProp)
{
	//m_infoMapHTS.SetAt(fmOBJECT, strProp);
	m_wndPropList.SetPropInfo(idx, strProp, true);
	m_infoMapCTMOBJ.SetAt(idx, strProp);
}

void CPropertiesWnd::OnSelCombo()
{
	if (m_mapH->forms.IsEmpty())
		return;

	if (m_wndCtrlCombo.GetCurSel() != CB_ERR)
	{
		CString info;
		int	idx = (int)m_wndCtrlCombo.GetItemData(m_wndCtrlCombo.GetCurSel());
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOCHILD, MAKELPARAM(idx, SC_PROPERTIESBAR));

		CMap<int, int, CString, CString>* m_infoMap;
		if (m_mapH->media == MEDIA_HTS)
			m_infoMap = &m_infoMapHTS;
		else
			m_infoMap = &m_infoMapMTS;
		if (m_infoMap->Lookup((idx >= 0 ? ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->kind : -1), info))
		{
			if (idx == -1)
			{
				if (m_mapH->mapK == MK_TEMPLATE)
					m_infoMap->Lookup(fmTEMPLATE, info);
				else if (m_mapH->mapK == MK_PROCEDURES)
					m_infoMap->Lookup(fmPROCEDURES, info);
				else
					m_infoMap->Lookup(idx, info);
			}
			changeProperty(idx, info);
		}
	}
}

LRESULT CPropertiesWnd::OnMessage(WPARAM wp, LPARAM lp)
{
	switch (wp)
	{
	case 1: // change property
		changeProperty((int)lp, getPropertyStr((int)lp));
		break;

	case 2: // desc change
	//	m_descStatic.SetWindowText(m_wndPropList.getDesc(HIWORD(lParam)));
		break;
	}
	return 0;
}

void CPropertiesWnd::Initialize(class mapForm* mapForm)
{
	m_mapH = mapForm;

	m_wndPropList.Initialize(mapForm);
}

void CPropertiesWnd::EnableProperty(BOOL bEnable)
{
	m_wndPropList.EnableWindow(bEnable);
	m_wndCtrlCombo.EnableWindow(bEnable);
}

int CPropertiesWnd::AddControl(CString sStr, DWORD data)
{
	int	nImage = 0;
	if ((int)data >= 0 && !(m_mapH->forms.IsEmpty()) && m_mapH->forms.GetCount() > (int)data)
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(data))->getProp();
		nImage = form->kind;
	}
	else if ((int)data < 0 && m_mapH)
		nImage = 23 + m_mapH->mapK;

	COMBOBOXEXITEM cbExItem;
	cbExItem.mask    = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_LPARAM|CBEIF_SELECTEDIMAGE;
	cbExItem.iItem   = 0;
	cbExItem.pszText = (LPTSTR)(LPCTSTR)sStr;
	cbExItem.iImage  = nImage-10;
	cbExItem.iSelectedImage  = nImage-10;
	cbExItem.lParam  = data;
	return m_wndCtrlCombo.InsertItem(&cbExItem);
}

void CPropertiesWnd::ReplaceControl(CString sItem, int data)
{
	for (int ii = 0; ii < m_wndCtrlCombo.GetCount(); ii++)
	{
		if ((int)m_wndCtrlCombo.GetItemData(ii) == data)
		{
			m_wndCtrlCombo.DeleteString(ii);
			m_wndCtrlCombo.SetCurSel(AddControl(sItem, data));
			break;
		}
	}
}

void CPropertiesWnd::SelectControl(int selctl, bool bLayout)
{
	m_bLayout = bLayout;

	if (selctl == -2)
	{
		CStringArray	formArr;
		CString info;

		CformProp* form = NULL;
		for (int ii = m_mapH->forms.GetCount() - 1; ii >= 0; ii--)
		{
			//(((CformProp *)((CObjectLoad*)m_mapH->forms.GetAt(m_index))->getProp())->scpN.IsEmpty())
			form = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp(); // (CformProp *)m_mapH->forms.GetAt(ii);
			if (form->block)
			{
				formArr.Add(getPropertyStr(ii));
				form->bCommon = true;
			}
			else
				form->bCommon = false;
		}

		info = compositeInArr(formArr);

		if (!info.IsEmpty())
		{
			m_wndPropList.SetPropInfo(2000, info);
			changeProperty(2000, info);
			int idx = AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_OBJSEL, 0);
			if (idx >= 0)
			{
				for (int ii = m_wndCtrlCombo.GetCount() - 1; ii >= 0; ii--)
				{
					if ((int)m_wndCtrlCombo.GetItemData(ii) == idx)
					{
						m_wndCtrlCombo.SetCurSel(ii);
					}
				}
			}
		}

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);
		return;
	}

	int	ii;

	for (ii = m_wndCtrlCombo.GetCount() - 1; ii >= 0; ii--)
	{
		if ((int)m_wndCtrlCombo.GetItemData(ii) == selctl)
		{
			m_wndCtrlCombo.SetCurSel(ii);
			int idx = (int)m_wndCtrlCombo.GetItemData(m_wndCtrlCombo.GetCurSel());
		
			if (!m_mapH->forms.IsEmpty() && selctl >= 0)
			{
				if (((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp()->group)
				{
					CString info = getPropertyStr(idx);
					if (!info.IsEmpty())
					{
						m_wndPropList.SetPropInfo(2000, info);
						changeProperty(2000, info);
					}
				}
				else
					changeProperty(idx, getPropertyStr(idx));
			}
			else if (selctl == -1)
			{
				setPropList();
				changeProperty(idx, getPropertyStr(-1));
			}
			break;
		}
	}
	if (ii == -1)
		m_wndPropList.Clear();

	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);
}

void CPropertiesWnd::ResetControl(int type)
{
	m_wndCtrlCombo.ResetContent();
	if (type == 1)
		AddControl(_T("_form_"), -1);
	else
		m_wndPropList.Clear();
}

void CPropertiesWnd::SetGroup(CString sGroup)
{
	SetData(MEDIA_HTS, sGroup);
	SetData(MEDIA_MTS, sGroup);
}

void CPropertiesWnd::SetData(char media, CString sGroup)
{
	CString	info;
	int	pos;

	CMap<int, int, CString, CString>* infoMap;

	if (media == MEDIA_HTS)
		infoMap = &m_infoMapHTS;
	else
		infoMap = &m_infoMapMTS;

	if (infoMap->Lookup(fmCHILDMAP, info))
	{
		pos = info.Find(_T("||mapGroup"));
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			infoMap->SetAt(fmCHILDMAP, info);
			m_wndPropList.SetPropInfo(fmCHILDMAP, info);
		}
	}

	if (infoMap->Lookup(fmTEMPLATE, info))
	{
		pos = info.Find(_T("||mapGroup"));
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			infoMap->SetAt(fmTEMPLATE, info);
			m_wndPropList.SetPropInfo(fmTEMPLATE, info);
		}
	}

	if (infoMap->Lookup(fmPROCEDURES, info))
	{
		pos = info.Find(_T("||mapGroup"));
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			infoMap->SetAt(fmPROCEDURES, info);
			m_wndPropList.SetPropInfo(fmPROCEDURES, info);
		}
	}
}

void CPropertiesWnd::Refresh(int row)
{
	m_wndPropList.Refresh(row);
	/*m_wndPropList.LoadData(m_wndPropList.m_index, 0, row);
	if (row == -1)
	{
		m_wndPropList.m_bInvalidate = TRUE;
		m_wndPropList.Invalidate();
	}
	else
		m_wndPropList.InvalidateRect2(CPoint(0, row));
		*/
}

void CPropertiesWnd::adjustLayout()
{
	if (GetSafeHwnd() == NULL)
		return;

	CRect	rectClient,rectCombo;

	GetClientRect(rectClient);
	m_wndCtrlCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndCtrlCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.top+200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::initPropList()
{
	int	key;
	CString	text;

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMap<int, int, CString, CString>* infoMap;
	//if (m_mapH->media == MEDIA_HTS)
		infoMap = &m_infoMapHTS;
	//else
	//	infoMap = &m_infoMapMTS;

	for (POSITION pos = infoMap->GetStartPosition(); pos; )
	{
		infoMap->GetNextAssoc(pos, key, text);
		m_wndPropList.SetPropInfo(key, text);
	}
}

void CPropertiesWnd::setPropList()
{
	int	key;
	CString	text;

	CMap<int, int, CString, CString>* infoMap;
	if (m_mapH->media == MEDIA_HTS)
		infoMap = &m_infoMapHTS;
	else
		infoMap = &m_infoMapMTS;

	for (POSITION pos = infoMap->GetStartPosition(); pos;)
	{
		infoMap->GetNextAssoc(pos, key, text);
		m_wndPropList.SetPropInfo(key, text);
	}
}

void CPropertiesWnd::loadPropertyFile(BYTE media, CString path)
{
	CString	text;
	CFile	file;
	char*	wb;

	text = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	text += path;

	if (file.Open(text, CFile::modeRead | CFile::typeBinary))
	{
		int	len, readN;

		len = (int)file.GetLength();
		wb = new char[len+1];
		ZeroMemory(wb, len+1);
		readN = file.Read(wb, len);
		file.Close();
		if (readN != len)
		{
			delete[] wb;
			return;
		}

		text = CString(wb, len);
		if (!text.IsEmpty())
			parseProperties(media, text);
		delete[] wb;
	}
}

void CPropertiesWnd::parseProperties(BYTE media, CString data)
{
	CMapStringToPtr mapFormID;

	mapFormID.SetAt(_T("LABEL"),	(void*)fmLABEL);
	mapFormID.SetAt(_T("BOX"),	(void*)fmBOX);
	mapFormID.SetAt(_T("GROUP"),	(void*)fmGROUP);
	mapFormID.SetAt(_T("PANEL"),	(void*)fmIMVIEW);
	mapFormID.SetAt(_T("BUTTON"),	(void*)fmBUTTON);
	mapFormID.SetAt(_T("EDIT"),	(void*)fmEDIT);
	mapFormID.SetAt(_T("COMBO"),	(void*)fmCOMBO);
	mapFormID.SetAt(_T("OUT"),	(void*)fmOUTPUT);
	mapFormID.SetAt(_T("MEMO"),	(void*)fmMEMO);
	mapFormID.SetAt(_T("GRID"),	(void*)fmGRID);
	mapFormID.SetAt(_T("GRIDEX"),	(void*)fmGRIDEX);
	mapFormID.SetAt(_T("TABLE"),	(void*)fmTABLE);
	mapFormID.SetAt(_T("TREEVIEW"),	(void*)fmTREE);
	mapFormID.SetAt(_T("OBJECT"),	(void*)fmFORM);
	mapFormID.SetAt(_T("USRTAB"),	(void*)fmTAB);
	mapFormID.SetAt(_T("BROWSER"),	(void*)fmBROWSER);
	mapFormID.SetAt(_T("CONTROL"),	(void*)fmOBJECT);
	mapFormID.SetAt(_T("RADIO"),	(void*)fmRADIO);
	mapFormID.SetAt(_T("CHECK"),	(void*)fmCHECK);
	mapFormID.SetAt(_T("CELL"),	(void*)fmGRIDCELL);
	mapFormID.SetAt(_T("CELLEX"),	(void*)fmGRIDEXCELL);
	mapFormID.SetAt(_T("ROW"),	(void*)fmTABLEROW);
	mapFormID.SetAt(_T("TAB SELECTED"),	(void*)fmTABSEL);
	mapFormID.SetAt(_T("CHILDMAP"),		(void*)fmCHILDMAP);
	mapFormID.SetAt(_T("TEMPLATE"),		(void*)fmTEMPLATE);
	mapFormID.SetAt(_T("PROCEDURES"),	(void*)fmPROCEDURES);
	mapFormID.SetAt(_T("LAYOUT"),	(void*)fmLAYOUT);

	bool	bStartIn = false;
	void*	rtValue;
	CString	data_pro, token;

	data.Replace(_T("\r\n"), _T("\n"));
	while (!data.IsEmpty())
	{
		token = parse(data, _T("\n"));
		if (!token.CompareNoCase(_T("#start")))
		{
			bStartIn = false;
			token = parse(data, _T("\n"));
			if (mapFormID.Lookup(token, rtValue))
			{
				bStartIn = true;
				data_pro = _T("");
				token = parse(data, _T("\n"));
			}
		}

		if (!token.CompareNoCase(_T("#end")))
		{
			if (media == MEDIA_HTS)
				m_infoMapHTS.SetAt((long)rtValue, data_pro);
			else
				m_infoMapMTS.SetAt((long)rtValue, data_pro);
			bStartIn = false;
		}

		if (bStartIn)
		{
			data_pro += token;
			data_pro += _T("\t");
		}
	}
	mapFormID.RemoveAll();
}

void CPropertiesWnd::setPropListFont()
{
	::DeleteObject(m_font.Detach());

	LOGFONT	lf;
	NONCLIENTMETRICS info;

	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_font.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_font);
	m_wndCtrlCombo.SetFont(&m_font);
}

int CPropertiesWnd::getPropertyInt(int idx, bool bGroupCheck)
{
	if (idx >= 0)
	{
		if (idx == 2000)
			return idx;
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();
		switch (form->kind)
		{
		case fmBUTTON:
			break;
		case fmCHECK:
			break;
		case fmRADIO:
			break;
		case fmGRID:
			if (form->cellProperties != NULL && form->CellSelected >= 0)
				return fmGRIDCELL;
			break;
		case fmGRIDEX:
			if (form->cellProperties != NULL && form->CellSelected >= 0)
				return fmGRIDEXCELL;
			break;

		case fmTABLE:
			if (form->cellProperties != NULL && form->CellSelected >= 0)
				return fmTABLEROW;
			break;

		case fmTAB:
			if (form->keyValues)
				return fmTABSEL;
			break;

		default:
			break;
		}

		if (form->group && bGroupCheck)
		{
			CStringArray	formArr;
			CformProp*	nGroup = form->group;
			CformProp* form = NULL;

			for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
			{
				form = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
				form->bCommon = false;
				if (form->group == nGroup)
				{
					formArr.Add(getPropertyStr(ii, false));
					form->bCommon = true;
				}
			}
			m_wndPropList.SetPropInfo(2000, compositeInArr(formArr));
			return 2000;
		}

		return form->kind;
	}

	if (idx == -1)
	{
		if (m_mapH->mapK == MK_TEMPLATE)
			return fmTEMPLATE;
		else if (m_mapH->mapK == MK_PROCEDURES)
			return fmPROCEDURES;
		else
			return idx;
	}
	return -2;
}

CString CPropertiesWnd::getPropertyStr(int idx, bool bGroupCheck)
{
	CString info;
	
	if (idx >= 0)
	{
		CformProp* form = ((CObjectLoad*)m_mapH->forms.GetAt(idx))->getProp();

		CMap<int, int, CString, CString>* m_infoMap;
		if (m_mapH->media == MEDIA_HTS)
			m_infoMap = &m_infoMapHTS;
		else
			m_infoMap = &m_infoMapMTS;

		m_infoMap->Lookup(form->kind, info);
		switch (form->kind)
		{
		case fmBUTTON:
			break;
		case fmRADIO:
			break;
		case fmCHECK:
			break;

		case fmGRID:
			if (form->cellProperties != NULL && form->CellSelected >= 0)
				m_infoMap->Lookup(fmGRIDCELL, info);
			break;
		case fmGRIDEX:
			if (form->cellProperties != NULL && form->CellSelected >= 0)
				m_infoMap->Lookup(fmGRIDEXCELL, info);
			break;

		case fmTABLE:
			if (form->cellProperties != NULL && form->CellSelected >= 0)
				m_infoMap->Lookup(fmTABLEROW, info);
			break;

		case fmTAB:
			if (form->keyValues)
				m_infoMap->Lookup(fmTABSEL, info);
			break;
		case fmOBJECT:
			m_infoMapCTMOBJ.Lookup(form->iCtmIdx, info);
			break;
		}

		if (form->group && bGroupCheck)
		{
			CStringArray	formArr;
			CformProp*	nGroup = form->group;
			CformProp* form = NULL;
			for (int ii = 0; ii < m_mapH->forms.GetCount(); ii++)
			{
				form = ((CObjectLoad*)m_mapH->forms.GetAt(ii))->getProp();
				form->bCommon = false;
				if (form->group == nGroup)
				{
					formArr.Add(getPropertyStr(ii, false));
					form->bCommon = true;
				}
			}
			
			info = compositeInArr(formArr);
		}
	}
	else if (idx == -1)
	{
		CMap<int, int, CString, CString>* m_infoMap;
		if (m_mapH->media == MEDIA_HTS)
			m_infoMap = &m_infoMapHTS;
		else
			m_infoMap = &m_infoMapMTS;

		if (m_mapH->mapK == MK_TEMPLATE)
			m_infoMap->Lookup(fmTEMPLATE, info);
		else if (m_mapH->mapK == MK_PROCEDURES)
			m_infoMap->Lookup(fmPROCEDURES, info);
		else
			m_infoMap->Lookup(idx, info);
	}
	return info;
}

void CPropertiesWnd::changeProperty(int idx, CString info)
{
	int	headNum;

	if (!m_bLayout)
		headNum = (idx < 0 ? idx : getPropertyInt(idx, true));
	else
		headNum = fmLAYOUT;

	if (idx < 0)
	{
		if (m_mapH->mapK == MK_NORM)
			headNum = -1;
		else if (m_mapH->mapK == MK_TEMPLATE)
			headNum = -2;
		else
			headNum = -3;
	}

	m_wndPropList.SetIndex(idx == 2000 ? -2 : idx);
	m_wndPropList.SetProperties(headNum, idx == 2000 ? -2 : idx);
//	m_wndPropList.LoadData((idx == 2000?-2:idx), 0);
}

CString CPropertiesWnd::compositeInArr(CStringArray &formArr)
{
	if (formArr.GetSize() <= 0)
		return _T("");

	CString	info, tmps, tmpx, pros;

	info = _T("");;
	tmps = formArr.GetAt(0);
	while (!tmps.IsEmpty())
	{
		tmpx = parse(tmps, _T("\t"));
		if (tmpx.Find(_T("Name|1|||name")) >= 0)
			continue;

		for (int ii = 1; ii < formArr.GetSize(); ii++)
		{
			pros = formArr.GetAt(ii);
			if (pros.Find(tmpx) == -1)
			{
				tmpx = _T("");
				break;
			}
		}

		if (!tmpx.IsEmpty())
		{
			info += tmpx;
			info += _T('\t');
		}
	}
	formArr.RemoveAll();
	return info;
}

CString CPropertiesWnd::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);
	
	if (pos == -1)
	{
		tmps = src;
		src  = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src  = src.Mid(pos + sub.GetLength());
	return tmps;
}