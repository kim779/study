// PropertiesBar.cpp: implementation of the CPropertiesBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axiswork.h"
#include "mainvar.h"
#include "PropertiesBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
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
#define	FM_RADIO	100
#define	FM_CHECK	101
#define	FM_GRIDCELL	102
#define	FM_TABLEROW	103
#define	FM_TABSEL	104
#define	FM_UTABSEL	105
#define	FM_FORM		-1
#define	FM_TEMPLATE	-2
#define	FM_PROCEDURES	-3

CPropertiesBar::CPropertiesBar()
{
	m_pFormItem = NULL;
	m_mapH      = NULL;
	m_propInfoArr.SetAt(FM_SYS,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tKind|3|1-yyyy/mm/dd;2-hh:mm:ss;3-Term;||type\tAlignment|3|$alignment||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tVisible|3|$bool|0x0002|properties|Choose Visibility\tTransparent|3|$bool|0x0008|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_LABEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str\tVerticalText|3|$bool|0x0040|properties\tAlignment|3|$alignment||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tTransparent|3|$bool|0x0008|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_BOX,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tKind|3|0-Line;1-Style3D;2-2Color;||type\tThick|1|||size\tBoxColor(H)|2|0|1002|pRGB|Set horizontal border line color\tBoxColorVert|2|0|1002|tRGB|Set vertical border line color\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_GROUP,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str|Enter Group Name\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tForeColor|2|0|1002|tRGB|Specify letter color\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_PANEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tType|3|0-3DBorder;1-Line;2-None;||type\tBorder|3|0-None;1-Raised;2-Sunken;3-Bump;4-Etched;||borders\tKind|3|0-Text;1-Bmp;2-Both;|0x0010;0x0080|properties\tImage|2||1003|str2\tImageType|3|0-Center;5-Tile;6-Stretch;||alignImg\tText|1||1|str\tVerticalText|3|$bool|0x0040|properties\tAlignment|3|$align||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tLineColor|2||1002|bRGB|Specify line color\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tHoverImage|3|$bool|0x0020|properties\tTransparent|3|$bool|0x0008|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_BUTTON,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tType|3|0-Text;1-Bmp;2-Both;|0x0010;0x0080|properties\tImage|2||1003|str2\tImageType|3|0-Center;5-Tile;6-Stretch;||alignImg\tText|1||1|str\tVerticalText|3|$bool|0x0040|properties\tAlignment|3|$align||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tKind|3|1-Normal;4-UserDefine;||type\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||Akeys\tTabOrder|1|1||index|Set tab order\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tEnable|3|$bool|0x0001|properties|Choose control usability \tHoverImage|3|$bool|0x0020|properties\tFocus|3|$bool|0x0004|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_EDIT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tDataKind|3|5-Digit;1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||type\tMode|3|1-Input;2-Both;||iok\tLength|1|||size|Set the length of character(s)\tInitialValue|1|||str\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tTabOrder|1|1||index|Set tab order\tTabSkip|3|$bool|0x02000000|attr\tSpin|3|0-None;1-Normal;100-Kospi;110-Future;111-Spread;120-Call Option;130-Put Option;140-Kospi Option;200-Kosdaq;210-KQ Future;220-KQ Call Option;230-KQ Put Option;||vals0\tSpinDelta|1|||Akeys\tEncrypt|3|$bool|0x00400000|attr\tEncryptKey|1|||str2\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tEnable|3|$bool|0x0001|properties|Choose control usability \tShowFormat|3|$bool|0x0100|properties|Choose format option\tNoFloating|3|$bool|0x10000000|attr|Inputing Decimal Point is not allowed\tFloatDigit|1|||offs\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||attr2\tAutoHScroll|3|$bool|0x00010000|attr\tCodeList|3|$bool|0x0040|properties\tAutoForm|3|$bool|0x0080|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tEditFormat|4||1015|xxx\tAttributes|4||1004|xxx");
	m_propInfoArr.SetAt(FM_COMBO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tSelectIndex|1|||checked\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||type\tMode|3|0-None;1-Input;2-Both;||iok\tDataLength|1|||size|Set the length of combo data\tVisibleCount|1|||Hkeys\tDataCount|2|1|1005|Lkeys|Edit Combo Contents and display the number of contents.\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||onEdit\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tTabOrder|1|1||index|Set tab order\tTabSkip|3|$bool|0x02000000|attr\tHintType|3|$hint||isHint\tHintText|1|||onHint\tAccount|3|$bool|0x0010|properties|Set account combo option\tVisible|3|$bool|0x0002|properties|Choose Visibility\tEnable|3|$bool|0x0001|properties|Choose control usability \tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tEditFormat|4||1015|xxx\tAttributes|4||1004|xxx");
	m_propInfoArr.SetAt(FM_OUT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||type\tLength|1|||size\tAlignment|3|$align||align\tLineType|3|0-3DBorder;1-Line;2-None;||iok\tLineColor|2||1002|bRGB|Specify line color\tBorder3D|3|0-None;1-Raised;2-Sunken;3-Bump;4-Etched;||borders\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tTabOrder|1|1||index|Set tab order\tHintType|3|$hint||isHint\tHintText|1|||onHint\tDominoName|1|||auxs\tReference|1|||refs\tFloatDigit|1|||offs\tVisible|3|$bool|0x0002|properties|Choose Visibility\tEnable|3|$bool|0x0001|properties|Choose control usability \tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tSum(OnAlert)|3|$bool|0x0010|properties\tEditFormat|4||1015|xxx\tEditCORGB|4||1016|xxx\tAttributes|4||1004|xxx");
	m_propInfoArr.SetAt(FM_MEMO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||type\tMode|3|1-Input;2-Both;3-Output;||iok\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tTabOrder|1|1||index|Set tab order.\tBorder|3|$bool||borders\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tEnable|3|$bool|0x0001|properties|Choose control usability.\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_GRID,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tRows|1|||rows\tColumns|1||1|cols\tFixedColumn#|1|||type|Set the fixed number of column.\tFlexibleRows|3|$bool|0x00000002|attr2|Set the number of variable rows.\tFlexibleMode|3|0-None;1-Expand;|0x00080000|attr2|Set the mode of variable rows.\tScrollValidRows|3|$bool|0x00400000|attr2\tAutoSize|3|$bool|0x80000000|attr2|Automatic Setting option for Row Height?\tVisibleRows|1|||Hkeys|Set the number of visible rows when AutoSize is set true.\tCellHeight|1|||Lkeys|Set the height of cell when AutoSize is set false.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tDataAppend|3|0-Bottom;1-Top;|0x00000010|attr2|Set Data inserted direction.\tLineColor|2||1002|bRGB|Set the grid line color.\tRowColor|3|$bool|0x00000080|attr2|Choose 1st and 2nd Row Color option?.\t1stRowColor|2||1002|spRGB|Specify color for odd numbered row?\t2ndRowColor|2||1002|stRGB|Specify color for even numbered row?\tTabOrder|1|1||index|Set tab order.\tVariant|1|||str\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tFocus|3|0-None;1-CellBox;2-RowBox;3-RowBar;4-Record;||isEdit|Set focus option.\tFocusColor|2||1002|onEdit|Specify focus color\tFocusText|3|$bool|0x00800000|attr2|Keep ForeColor on focus\tKeyFocus|3|$bool|0x00004000|attr2\tMerge|3|$bool|0x00020000|attr2|Set merge option for the grid header.\tSelectionAlways|3|$bool|0x00040000|attr2|Set selection Always option.\tNoEffect3D|3|$bool|0x00000040|attr2|Set no 3 dimension effects option.\tNoHead|3|$bool|0x00000020|attr2|Set no grid header option.\tNoVerLine|3|$bool|0x00000100|attr2|Set no vertical line option.\tNoHorLine|3|$bool|0x00000200|attr2|Set no horizontal line option.\tNoResizeWidth|3|$bool|0x00100000|attr2\tAutoWidth|3|$bool|0x00200000|attr2\tModifyFlag|3|$bool|0x00010000|attr2\t\tTabLeave|3|$bool|0x00000040|attr\t" \
					"Drop|3|$bool|0x00100000|attr|Set data drop option.\tColumnReedit|3|$bool|0x00400000|attr|Set editing of grid column option.\tPageScroll|3|$bool|0x00000400|attr2|Set page scroll option.\tResizable|3|$bool|0x00008000|attr2\tHeadInformation|3|$bool|0x00000800|attr2|Set grid header information transmission option.\tTable|3|$bool|0x00001000|table\tThumb|3|$bool|0x00002000|attr2\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tColumns...|4||1009|xxx");
	m_propInfoArr.SetAt(FM_TABLE,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tCount|1||1|rows\tAutoSize|3|$bool|0x80000000|attr2\tCellHeight|1|||Akeys\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tStyle|3|0-Combine;1-Head;2-Data;||tableHead\tLineColor|2||1002|bRGB\tHeadWidth|1|||allcellwidth\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tFocus|3|$bool|0x0004|properties\tNoVerLine|3|$bool|0x00000100|attr2\tNoHorLine|3|$bool|0x00000200|attr2\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tRows...|4||1009|xxx");
	m_propInfoArr.SetAt(FM_TREE,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tLength|1|||size\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tEnable|3|$bool|0x0001|properties|Choose control usability.\tHasLines|3|$bool|0x0020|properties\tMultiSelection|3|$bool|0x0004|properties\tFolder|3|$bool|0x00400000|attr\tClick&&Send|3|$bool|0x00000010|attr\tDrop|3|$bool|0x00100000|attr\tDrag|3|$bool|0x00080000|attr\tIgnoreDomino|3|$bool|0x00200000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_AVI,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tFile|2||1003|str2\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_GRAPH,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tLength(Day)|1|||size\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tAttributes|4||1006|xxx");
	m_propInfoArr.SetAt(FM_OBJECT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tMapName|1|0008|1|str\tTabOrder|1|1||index|Set tab order.\tFixedSize|3|$bool||fixSize\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tResizable|3|$bool|0x00008000|attr2\tIgnoreDomino|3|$bool|0x00200000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_TAB,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tSelectIndex|1|||checked|Set the initial page of the tab.\tTabHeight|1|||size|Set tab's height.\tTabCount|1||1|Akeys|Set the number of tab pages.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tType|3|0-Border3D;1-None;2-Oblique;4-Rect;||type\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tFocusBackColor|2||1002|spRGB\tFocusForeColor|2||1002|stRGB\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tIgnoreDomino|3|$bool|0x00200000|attr\tDrop|3|$bool|0x00100000|attr\tDrag|3|$bool|0x00080000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_UTAB,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tSelectIndex|1|||checked|Set the initial page of the user tab.\tTabCount|1||1|Akeys\tPosition|3|1-left;2-right;3-top;4-bottom;||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tType|3|0-Border3D;1-None;2-Oblique;4-Rect;||type\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tFocusBackColor|2||1002|spRGB\tFocusForeColor|2||1002|stRGB\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tFocus|3|$bool|0x0004|properties\tFileLoad|3|$bool|0x00400000|attr\tIgnoreDomino|3|$bool|0x00200000|attr\tDrop|3|$bool|0x00100000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_SHEET,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tDataLength|1|||size\tVisibleCount|1|||Akeys\tAlignment|3|$align||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tType|3|0-Border3D;1-None;2-Oblique;4-Rect;||type\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tFolder|3|$bool|0x00400000|attr\tClick&&Send|3|$bool|0x00000010|attr\tDrop|3|$bool|0x00100000|attr\tIgnoreDomino|3|$bool|0x00200000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_BROWSER,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tMode|3|0-None;3-Output;||iok\tStartPage|1|||str\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tEnable|3|$bool|0x0001|properties|Choose control usability.\tShowTool|3|$bool|0x0004|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_CONTROL, "Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tControl|2|1|1012|str\tMode|3|0-NOP;1-Input;2-Both;3-Output;||iok\tLength|1|||size|Set the length of character(s).\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tSelfFlash|3|$bool|0x00008000|attr\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tProtect|3|$bool|0x00000020|attr|Set protect mode option.\tGridData|3|$bool|0x00400000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");

	m_propInfoArr.SetAt(FM_RADIO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tGroupName|1|||auxs\tText|1||1|str\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tMode|3|0-None;1-Input;2-Both;||iok\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tEnable|3|$bool|0x0001|properties|Choose control usability.\tChecked|3|$bool|0x0004|properties\tIgnoreDomino|3|$bool|0x00200000|attr\tTransparent|3|$bool|0x0008|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_CHECK,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tMode|3|0-None;1-Input;2-Both;||iok\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tEnable|3|$bool|0x0001|properties|Choose control usability.\tChecked|3|$bool|0x0004|properties\tIgnoreDomino|3|$bool|0x00200000|attr\tTransparent|3|$bool|0x0008|properties\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_GRIDCELL, "Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tMode|3|0-None;1-Input;3-Output;2-Both;||celliok\tType|3|0-Normal;1-Check;2-Combo;3-Button;9-Graph;||cellkind\tCheckBoxText|1|||cellstr2\tExclusiveCheck|3|$bool|0x0004|cellproperties\tComboCount|2|1|1011|cellkeys|Edit Combo Contents and display the number of contents.\tButtonType|3|0-Text;1-Bmp;2-Both;|0x0010;0x0080|cellproperties\tButtonText|1|0256||celltxt\tButtonImage|2||1003|cellstr3\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||cellvals2\tLength|1|||cellsize\tWidth|1|||cellwidth\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tImageAlignment|3|0-Center;5-Tile;6-Stretch;||cellalignI\tHeadBackColor|2||1002|cellhpRGB\tHeadForeColor|2||1002|cellhtRGB\tDataBackColor|2||1002|cellpRGB\tDataForeColor|2||1002|celltRGB\tHintType|3|$hint||cellisHint\tHintText|1|||cellonHint\tDomino|1|||cellauxs\tReference|1|||cellrefs\tVisible|3|$bool|0x0002|cellproperties\tSort|3|$bool|0x0020|cellproperties\tShowFormat|3|$bool|0x0100|cellproperties|Choose format option.\tFloatDigit|1|||cellvals\tDate|3|61440-None;0-Auto;4096-YYYYMM;8192-YYMM;12288-MMDD;||cellattr2\tIgnoreFloating0|3|$bool|0x00400000|cellattr\tColumnSuppress|3|$bool|0x0040|cellproperties\tEdit|4||1007|xxx\tAttributes|4||1004|xxx");
	m_propInfoArr.SetAt(FM_TABLEROW, "Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tLength|1|||cellsize\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2||1002|cellhpRGB\tHeadForeColor|2||1002|cellhtRGB\tDataBackColor|2||1002|cellpRGB\tDataForeColor|2||1002|celltRGB\tHintType|3|$hint||cellisHint\tHintText|1|||cellonHint\tFloatDigit|1|||cellvals\tDomino|1|||cellauxs\tReference|1|||cellrefs\tVisible|3|$bool|0x0002|cellproperties\tEdit|4||1007|xxx\tAttributes|4||1004|xxx");
	m_propInfoArr.SetAt(FM_TABSEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tHeadText|1||1|tabHead\tSelectIndex|1|||checked|Set the initial page of the user tab.\tTabHeight|1|||size\tTabCount|1|||Akeys\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tType|3|0-Border3D;1-None;2-Oblique;4-Rect;||type\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tFocusBackColor|2||1002|spRGB|Specify background color on the selected page.\tFocusForeColor|2||1002|stRGB|Specify letter color on the selected page.\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tIgnoreDomino|3|$bool|0x00200000|attr\tDrop|3|$bool|0x00100000|attr\tDrag|3|$bool|0x00080000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_propInfoArr.SetAt(FM_UTABSEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tHeadText|1||1|tabHead\tHeadData|1|||tabData\tHeadVisible|3|$bool||tabVisible\tSelectIndex|1|||checked|Set the initial page of the user tab.\tTabCount|1|||Akeys\tPosition|3|1-left;2-right;3-top;4-bottom;||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tType|3|0-Border3D;1-None;2-Oblique;4-Rect;||type\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tFocusBackColor|2||1002|spRGB|Specify background color on the selected page.\tFocusForeColor|2||1002|stRGB|Specify letter color on the selected page.\tVisible|3|$bool|0x0002|properties|Choose Visibility.\tFocus|3|$bool|0x0004|properties\tFileLoad|3|$bool|0x00400000|attr\tIgnoreDomino|3|$bool|0x00200000|attr\tDrop|3|$bool|0x00100000|attr\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?\tPageEdit|4||1013|xxx");
	m_propInfoArr.SetAt(FM_FORM,	"Title|1|||mapcaption|Enter title of map.\tWidth|1|||mapwidth|Enter the width of the map.\tHeight|1|||mapheight|Enter the height of the map.\tFont|2|1|1001|mapfname|Specify map's Font name.\tFontSize|1|||mapfpoint|Set map's Font size.\tFontStyle|3|$fstyle||mapfstyle|Choose map's Font style.\tVersion|1|||mapvers|Enter version of the map.\tName|1|0008||mapname|Enter the name of the map.\tGroup|3|||mapGroup|Choose map's group\tDescription|1|0020||mapdesc|Enter the description of the map.\tViewType|3|4-Fixed;5-FixedEx;2-Resizable;0-Normal;1-Scroll;3-Static;||maptype|Set the view type option.\tMethod|3|0-Form;1-Menu;2-Send;||mapmapM\tTxCode|1|0008||maptrxC\tTxHead|1|0032||maptrxH\tRepeat|1|||repeatV\tPublic|2|1|1010|publicR\tHelp|1|||maphelpN\tBackGround|3|0-None;1-Color;2-Image;||mapimageK\tBackColor|2||1002|mapBkClr\tImage|2||1003|mapimageV\tOptions|4||1008|xxx");
	m_propInfoArr.SetAt(FM_TEMPLATE, "Width|1|||mapwidth|Set the template's width.\tHeight|1|||mapheight|Set the template's height.\tFont|2|1|1001|mapfname|Specify template's Font name.\tFontSize|1|||mapfpoint|Set template's Font size.\tFontStyle|3|$fstyle||mapfstyle|Choose template's Font style.\tTemplateName|1|0008|1|mapname|Enter the name of the template.\tMapGroup|3|||mapGroup\tDescription|1|0020||mapdesc\t#end\t#start\tPROCEDURES\tTitle|1|||mapcaption\tVersion|1|||mapvers\tName|1|0008|1|mapname\tGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
	m_propInfoArr.SetAt(FM_PROCEDURES, "Title|1|||mapcaption\tVersion|1|||mapvers\tName|1|0008|1|mapname\tGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
}

CPropertiesBar::~CPropertiesBar()
{
	m_propInfoArr.RemoveAll();
	if (m_pFormItem)
		delete m_pFormItem;
}

BEGIN_MESSAGE_MAP(CPropertiesBar, CSizingControlBarG)
	//{{AFX_MSG_MAP(CPropertiesBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_CBN_SELENDOK(ID_PRO_CTLLISTCBO, OnSelEndOk)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

int CPropertiesBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadPropertyFile();
	
	if (!m_ctrllistCBO.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL |
		CBS_DROPDOWNLIST|CBS_SORT// | CBS_AUTOHSCROLL | CBS_HASSTRINGS | CBS_SORT
		,CRect(0,10,200,200), this, ID_PRO_CTLLISTCBO ))
		return -1;
	if (!m_listGrid.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | WS_TABSTOP, CRect(0, 0, 0, 0), this, 110))
		return -1;

	if (!m_descStatic.Create("", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_BORDER, CRect(0), this))
		return -1;
	m_descStatic.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	
	/*if (!m_font.CreateStockObject(DEFAULT_GUI_FONT))
		if (!m_font.CreatePointFont(80, "MS Sans Serif"))
			return -1;*/
	if (!m_font.CreatePointFont(80, "FixedSys"))
		return -1;
	
	m_listGrid.ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	m_descStatic.SetFont(&m_font);
	m_ctrllistCBO.SetFont(&m_font);

	POSITION pos = m_propInfoArr.GetStartPosition();
	while(pos)
	{
		int key;
		CString value;

		m_propInfoArr.GetNextAssoc(pos, key, value);
		m_listGrid.setHeadInfo(key, value);
	}

	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_SELECTIONBAR);
	imglist.Create(17, 17, ILC_COLOR24|ILC_MASK, 26, 1);
	imglist.Add(&bitmap, RGB(0, 255, 255));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_MAP));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_TEMPLATE));
	imglist.Add(AfxGetApp()->LoadIcon(IDI_PROCEDURES));
	imglist.SetBkColor(CLR_NONE);
	m_ctrllistCBO.SetImageList(&imglist);
	imglist.Detach();
	bitmap.Detach();
	m_ctrllistCBO.SetFont(GetFont());
	

	return 0;
}

void CPropertiesBar::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBarG::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);

	rc.DeflateRect(3, 5, 3, 0);
	rc.bottom = rc.top + 220;

	m_ctrllistCBO.MoveWindow(rc);
	GetClientRect(rc);

	rc.DeflateRect(3, 30, 3, 40);
	m_listGrid.MoveWindow(rc);
	m_listGrid.RedrawWindow();

	GetClientRect(rc);
	rc.DeflateRect(3, 30, 3, 0);
	rc.top = rc.bottom - 38;
	m_descStatic.MoveWindow(rc);
	m_descStatic.RedrawWindow();
}

int CPropertiesBar::AddComboString(CString sItem, int data)
{
	int idx = 0;
	AddCBOString(sItem, data);
	//m_ctrllistCBO.SetItemData(idx = m_ctrllistCBO.AddString(sItem), data);
	return idx;
}

void CPropertiesBar::ReplaceComboString(CString sItem, int data)
{
	for (int ii = 0; ii < m_ctrllistCBO.GetCount(); ii++)
	{
		if ((int)m_ctrllistCBO.GetItemData(ii) == data)
		{
			m_ctrllistCBO.DeleteString(ii);
			m_ctrllistCBO.SetCurSel(AddComboString(sItem, data));
			break;
		}
	}
}

void CPropertiesBar::ResetCombo(int type /*= 1*/)
{
	m_ctrllistCBO.ResetContent();
	if (type == 1)
		AddCBOString("_form_", -1);
		//m_ctrllistCBO.SetItemData(m_ctrllistCBO.AddString("_form_"), -1);
	else
		m_listGrid.ResetGrid();
}

void CPropertiesBar::SelectControl(int selctl)
{
	if (selctl == -2)
	{
		CStringArray	formArr;
		CString info;

		for (int ii = m_pFormItem->getFormCount() - 1; ii >= 0; ii--)
		{
			if (m_pFormItem->getFormPoint(ii)->block)
			{
				formArr.Add(getPropertyStr(ii));
				m_pFormItem->getFormPoint(ii)->bCommon = true;
			}
			else
				m_pFormItem->getFormPoint(ii)->bCommon = false;
		}

		info = compositInArr(formArr);

		if (!info.IsEmpty())
		{
			m_listGrid.setHeadInfo(2000, info);
			changePro(2000, info);
		}

		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);
		return;
	}

	int ii = 0;
	for (ii = m_ctrllistCBO.GetCount() - 1; ii >= 0; ii--)
	{
		if ((int)m_ctrllistCBO.GetItemData(ii) == selctl)
		{
			m_ctrllistCBO.SetCurSel(ii);
			int idx = m_ctrllistCBO.GetItemData(m_ctrllistCBO.GetCurSel());
		
			if (m_mapH->formR && selctl >= 0)
			{
				if (m_pFormItem->getFormPoint(idx)->group)
				{
					CString info = getPropertyStr(idx);
					if (!info.IsEmpty())
					{
						m_listGrid.setHeadInfo(2000, info);
						changePro(2000, info);
					}
				}
				else
					changePro(idx, getPropertyStr(idx));
			}
			else if (selctl == -1)
				changePro(idx, getPropertyStr(-1));
			break;
		}
	}
	if (ii == -1)
	{
		m_listGrid.EraseCtrls(FALSE);
		m_listGrid.ResetGrid();
		m_listGrid.RedrawGrid();
	}
	AfxGetMainWnd()->PostMessage(WM_USER, ID_USR_SETFOCUSCHILD, 0);
}

void CPropertiesBar::OnSelEndOk()
{
	if (m_ctrllistCBO.GetCurSel() != CB_ERR)
	{
		int idx = m_ctrllistCBO.GetItemData(m_ctrllistCBO.GetCurSel());
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOCHILD, MAKELPARAM(idx, SC_PROPERTIESBAR));
		CString info;
		if (m_propInfoArr.Lookup((idx >= 0?m_pFormItem->getFormPoint(idx)->kind:-1), info))
		{
			if (idx == -1)
			{
				if (m_mapH->mapK == MK_TEMPLATE)
					m_propInfoArr.Lookup(FM_TEMPLATE, info);
				else if (m_mapH->mapK == MK_PROCEDURES)
					m_propInfoArr.Lookup(FM_PROCEDURES, info);
				else
					m_propInfoArr.Lookup(idx, info);
			}
			changePro(idx, info);
		}
	}
}

void CPropertiesBar::Initialize(struct _mapH* mapH)
{
	//m_formR = formR;
	if (m_pFormItem)
		delete m_pFormItem;
	m_pFormItem = new CFormItem(mapH);
	m_mapH = mapH;
	m_listGrid.Initialize(mapH);
}

void CPropertiesBar::changePro(int idx, CString info)
{
	int headNum = (idx < 0?idx:getPropertyInt(idx, true));

	if (idx < 0)
	{
		if (m_mapH->mapK == MK_NORM)
			headNum = -1;
		else if (m_mapH->mapK == MK_TEMPLATE)
			headNum = -2;
		else headNum = -3;
	}
	m_listGrid.EraseCtrls(FALSE);	// SDI focus 때문에 FALSE수정
	m_listGrid.setIndex((idx == 2000?-2:idx));
	m_listGrid.ResetGrid();
	m_listGrid.setHeadInfo(headNum);
	m_listGrid.LoadData((idx == 2000?-2:idx), 0);
	m_listGrid.RedrawGrid();
}

CString CPropertiesBar::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}


CString CPropertiesBar::getPropertyStr(int idx, bool bGroupCheck)
{
	CString info;
	
	if (idx >= 0)
	{
		m_propInfoArr.Lookup(m_pFormItem->getFormPoint(idx)->kind, info);
		if (m_pFormItem->getFormPoint(idx)->kind == FM_BUTTON)
		{
			if (m_pFormItem->getFormPoint(idx)->type == BTN_RADIO)
				m_propInfoArr.Lookup(FM_RADIO, info);
			else if (m_pFormItem->getFormPoint(idx)->type == BTN_CHECK)
				m_propInfoArr.Lookup(FM_CHECK, info);
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID)
		{
			if (m_pFormItem->getFormPoint(idx)->auxR != (char *)0 &&
				((struct _repR *)m_pFormItem->getFormPoint(idx)->auxR)->selected >= 0)
				m_propInfoArr.Lookup(FM_GRIDCELL, info);
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_TABLE)
		{
			if (m_pFormItem->getFormPoint(idx)->auxR != (char *)0 &&
				((struct _repR *)m_pFormItem->getFormPoint(idx)->auxR)->selected >= 0)
				m_propInfoArr.Lookup(FM_TABLEROW, info);
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_TAB)
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
				m_propInfoArr.Lookup(FM_TABSEL, info);
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_UTAB)
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
				m_propInfoArr.Lookup(FM_UTABSEL, info);
		}
		if (m_pFormItem->getFormPoint(idx)->group && bGroupCheck)
		{
			CStringArray	formArr;
			struct _formR *nGroup = m_pFormItem->getFormPoint(idx)->group;

			for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				m_pFormItem->getFormPoint(ii)->bCommon = false;
				if (m_pFormItem->getFormPoint(ii)->group == nGroup)
				{
					formArr.Add(getPropertyStr(ii, false));
					m_pFormItem->getFormPoint(ii)->bCommon = true;
				}
			}
			
			info = compositInArr(formArr);
		}
	}
	else if (idx == -1)
	{
		if (m_mapH->mapK == MK_TEMPLATE)
			m_propInfoArr.Lookup(FM_TEMPLATE, info);
		else if (m_mapH->mapK == MK_PROCEDURES)
			m_propInfoArr.Lookup(FM_PROCEDURES, info);
		else
			m_propInfoArr.Lookup(idx, info);
	}
	return info;
}

int CPropertiesBar::getPropertyInt(int idx, bool bGroupCheck)
{
	if (idx >= 0)
	{
		if (idx == 2000)
			return 2000;
		if (m_pFormItem->getFormPoint(idx)->kind == FM_BUTTON)
		{
			if (m_pFormItem->getFormPoint(idx)->type == BTN_RADIO)
				return FM_RADIO;
			else if (m_pFormItem->getFormPoint(idx)->type == BTN_CHECK)
				return FM_CHECK;
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_GRID)
		{
			if (m_pFormItem->getFormPoint(idx)->auxR != (char *)0 &&
				((struct _repR *)m_pFormItem->getFormPoint(idx)->auxR)->selected >= 0)
				return FM_GRIDCELL;
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_TABLE)
		{
			if (m_pFormItem->getFormPoint(idx)->auxR != (char *)0 &&
				((struct _repR *)m_pFormItem->getFormPoint(idx)->auxR)->selected >= 0)
				return FM_TABLEROW;
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_TAB)
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
				return FM_TABSEL;
		}
		else if (m_pFormItem->getFormPoint(idx)->kind == FM_UTAB)
		{
			if (m_pFormItem->getFormPoint(idx)->keys)
				return FM_UTABSEL;
		}
		if (m_pFormItem->getFormPoint(idx)->group && bGroupCheck)
		{
			CStringArray	formArr;
			struct _formR *nGroup = m_pFormItem->getFormPoint(idx)->group;

			for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
			{
				m_pFormItem->getFormPoint(ii)->bCommon = false;
				if (m_pFormItem->getFormPoint(ii)->group == nGroup)
				{
					formArr.Add(getPropertyStr(ii, false));
					m_pFormItem->getFormPoint(ii)->bCommon = true;
				}
			}
			m_listGrid.setHeadInfo(2000, compositInArr(formArr));
			return 2000;
		}
		return m_pFormItem->getFormPoint(idx)->kind;
	}
	else if (idx == -1)
	{
		if (m_mapH->mapK == MK_TEMPLATE)
			return FM_TEMPLATE;
		else if (m_mapH->mapK == MK_PROCEDURES)
			return FM_PROCEDURES;
		else
			return idx;
	}
	return -2;
}

void CPropertiesBar::SetKeyIn(char nChar)
{
	m_listGrid.SetKeyIn(nChar);
}

long CPropertiesBar::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 1: // change property
		changePro(lParam, getPropertyStr(lParam));
		break;
	case 2: // desc change
		m_descStatic.SetWindowText(m_listGrid.getDesc(HIWORD(lParam)));
		break;
	}
	return 0;
}

void CPropertiesBar::killFocus()
{
	m_listGrid.EraseCtrls(false);
}

CString CPropertiesBar::compositInArr(CStringArray &formArr)
{
	CString info, tmp;
	info.Empty();
	if (formArr.GetSize() == 0)
		return "";
	tmp = formArr.GetAt(0);
	while(!tmp.IsEmpty())
	{
		CString tmp2 = Parser(tmp, "\t");
		if (tmp2.Find("Name|1|||name") >= 0)
			continue;
		for (int ii = 1; ii < formArr.GetSize(); ii++)
		{
			CString sPro = formArr.GetAt(ii);
			if (sPro.Find(tmp2) == -1)
			{
				tmp2.Empty();
				break;
			}
		}
		if (!tmp2.IsEmpty())
		{
			info += tmp2;
			info += '\t';
		}
	}
	formArr.RemoveAll();
	return info;
}

void CPropertiesBar::setGroup(CString sGroup)
{
	CString info;
	if (m_propInfoArr.Lookup(FM_FORM, info))
	{
		int pos = pos = info.Find("||mapGroup");
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			m_propInfoArr.SetAt(FM_FORM, info);
			m_listGrid.setHeadInfo(FM_FORM, info);
		}
	}
	if (m_propInfoArr.Lookup(FM_TEMPLATE, info))
	{
		int pos = pos = info.Find("||mapGroup");
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			m_propInfoArr.SetAt(FM_TEMPLATE, info);
			m_listGrid.setHeadInfo(FM_TEMPLATE, info);
		}
	}

	if (m_propInfoArr.Lookup(FM_PROCEDURES, info))
	{
		int pos = pos = info.Find("||mapGroup");
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			m_propInfoArr.SetAt(FM_PROCEDURES, info);
			m_listGrid.setHeadInfo(FM_PROCEDURES, info);
		}
	}
}

void CPropertiesBar::Refresh(int row)
{
	//m_listGrid.refreshData();
	m_listGrid.LoadData(m_listGrid.m_index, 0, row);
	if (row == -1)
	{
		m_listGrid.m_bInvalidate = TRUE;
		m_listGrid.Invalidate();
	}
	else
		m_listGrid.InvalidateRect2(CPoint(0, row));
}

void CPropertiesBar::LoadPropertyFile()
{
	CString path, data = "";
	CFileFind finder;
	char buf[4097];
	path = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	path += "\\tab\\properties.dat";

	if (finder.FindFile(path))
	{
		TRY
		{
			CFile file(path, CFile::modeRead);
			UINT nBytesRead;
			do
			{
				nBytesRead = file.Read(buf, 4096);
				if (nBytesRead)
					data += CString(buf, nBytesRead);
			}while((int)nBytesRead);
			file.Close();
		}CATCH(CFileException, e )
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH
	}
	if (!data.IsEmpty())
		parseProperties(data);
}

void CPropertiesBar::parseProperties(CString data)
{
	CMapStringToPtr mapFormID;
	mapFormID.SetAt("SYSTEM", (void*)FM_SYS);
	mapFormID.SetAt("LABEL", (void*)FM_LABEL);
	mapFormID.SetAt("BOX", (void*)FM_BOX);
	mapFormID.SetAt("GROUP", (void*)FM_GROUP);
	mapFormID.SetAt("PANEL", (void*)FM_PANEL);
	mapFormID.SetAt("BUTTON", (void*)FM_BUTTON);
	mapFormID.SetAt("EDIT", (void*)FM_EDIT);
	mapFormID.SetAt("COMBO", (void*)FM_COMBO);
	mapFormID.SetAt("OUT", (void*)FM_OUT);
	mapFormID.SetAt("MEMO", (void*)FM_MEMO);
	mapFormID.SetAt("GRID", (void*)FM_GRID);
	mapFormID.SetAt("TABLE", (void*)FM_TABLE);
	mapFormID.SetAt("TREE", (void*)FM_TREE);
	mapFormID.SetAt("AVI", (void*)FM_AVI);
	mapFormID.SetAt("GRAPH", (void*)FM_GRAPH);
	mapFormID.SetAt("OBJECT", (void*)FM_OBJECT);
	mapFormID.SetAt("TAB", (void*)FM_TAB);
	mapFormID.SetAt("UTAB", (void*)FM_UTAB);
	mapFormID.SetAt("SHEET", (void*)FM_SHEET);
	mapFormID.SetAt("BROWSER", (void*)FM_BROWSER);
	mapFormID.SetAt("CONTROL", (void*)FM_CONTROL);
	mapFormID.SetAt("RADIO", (void*)FM_RADIO);
	mapFormID.SetAt("CHECK", (void*)FM_CHECK);
	mapFormID.SetAt("CELL", (void*)FM_GRIDCELL);
	mapFormID.SetAt("ROW", (void*)FM_TABLEROW);
	mapFormID.SetAt("TAB SELECTED", (void*)FM_TABSEL);
	mapFormID.SetAt("UTAB SELECTED", (void*)FM_UTABSEL);
	mapFormID.SetAt("FORM", (void*)FM_FORM);
	mapFormID.SetAt("TEMPLATE", (void*)FM_TEMPLATE);
	mapFormID.SetAt("PROCEDURES", (void*)FM_PROCEDURES);

	bool bStartIn = false;
	void *rtValue;
	CString data_pro;
	data.Replace("\r\n", "\n");
	while(!data.IsEmpty())
	{
		CString token = Parser(data, "\n");
		if (!token.CompareNoCase("#start"))
		{
			bStartIn = false;
			token = Parser(data, "\n");
			if (mapFormID.Lookup(token, rtValue))
			{
				bStartIn = true;
				data_pro.Empty();
				token = Parser(data, "\n");
			}
		}
		if (!token.CompareNoCase("#end"))
		{
			m_propInfoArr.SetAt((long)rtValue, data_pro);			
			bStartIn = false;
		}

		if (bStartIn)
		{
			data_pro += token;
			data_pro += "\t";
		}
	}
	mapFormID.RemoveAll();
}

void CPropertiesBar::EnableProperty(bool bEnable)
{
	m_listGrid.EnableWindow(bEnable);
	m_ctrllistCBO.EnableWindow(bEnable);
}

int CPropertiesBar::AddCBOString(CString sStr, DWORD data)
{
	int nImage = 0;
	if ((int)data >= 0 && m_pFormItem && m_pFormItem->getFormCount() > (int)data)
	{
		nImage = m_pFormItem->getFormPoint(data)->kind;
		if (nImage == FM_BUTTON)
		{
			if (m_pFormItem->getFormPoint(data)->type == BTN_RADIO)
				nImage = 21;
			else if (m_pFormItem->getFormPoint(data)->type == BTN_CHECK)
				nImage = 22;
		}
		else if (nImage >= FM_GRAPH)
			nImage--;
	}
	else if ((int)data < 0 && m_mapH)
		nImage = 23 + m_mapH->mapK;

	COMBOBOXEXITEM cbExItem;
	cbExItem.mask    = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_LPARAM|CBEIF_SELECTEDIMAGE;
	cbExItem.iItem   = 0;
	cbExItem.pszText = (LPTSTR)(LPCTSTR)sStr;
	cbExItem.iImage  = nImage;
	cbExItem.iSelectedImage  = nImage;
	cbExItem.lParam  = data;
	int nIdx = m_ctrllistCBO.InsertItem(&cbExItem);

	return nIdx;
}