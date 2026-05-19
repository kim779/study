
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "axisMBuilder.h"

#include "h/mainvar.h"
#include "amCom/formItem.h"

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

#define	FM_GRIDCELL	102
#define FM_GRIDEXCELL	105
#define	FM_TABLEROW	103
#define	FM_TABSEL	104
#define	FM_FORM		-1
#define	FM_PROCEDURES	-3
#define	FM_LAYOUT	-4


/////////////////////////////////////////////////////////////////////////////
// CPropertiesBar

CPropertiesWnd::CPropertiesWnd()
{
	m_formItem = NULL;
	m_mapH     = NULL;

	m_infoMap.SetAt(FM_LABEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str\tAlignment|3|$alignment||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tLineColor|2||1002|bRGB|Set the line color.\tAlpha|1|2||alpha|Specify Alpha Value.\tBorder|3|0-None;5-Line;||borders\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tTransparent|3|$bool|0x00000008|properties\tImage|2||1003|str2\tVoice|1||1|desc|voice reading\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3");
	m_infoMap.SetAt(FM_BOX,		"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tThick|1|||size\tLineColor|2||1002|pRGB|Specify Background Color.\tAlpha|1|2||alpha|Specify Alpha Value.");
	m_infoMap.SetAt(FM_GROUP,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str|Enter Group Name\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tForeColor|2|0|1002|tRGB|Specify letter color.\tAlpha|1|2||alpha|Specify Alpha Value.\tHintType|3|$hint||isHint\tHintText|1|||onHint\tVisible|3|$bool|0x0002|properties|Choose Visibility\tKeeping|3|$bool|0x00800000|attr|Removing prohibited by User's Workshop?");
	m_infoMap.SetAt(FM_IMAGEVIEW,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tBackColor|2||1002|pRGB|Specify Background Color.\tLineColor|2||1002|bRGB|Set the line color.\tAlpha|1|2||alpha|Specify Alpha Value.\tBorder|3|0-None;5-Line;||borders\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tImage|2||1003|str2\tVoice|1||1|desc|voice reading");
	m_infoMap.SetAt(FM_BUTTON,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tType|3|0-Text;1-Bmp;2-Both;|0x00000010;0x00000080|properties\tImage|2||1003|str2\tIcon Image|2||1003|OptionImg\tText|1||1|str\tAlignment|3|$align||align\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tEnable|3|$bool|0x00000001|properties|Choose control usability\tVoice|1||1|desc|voice reading\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3");
	m_infoMap.SetAt(FM_CHECK,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tEnable|3|$bool|0x00000001|properties|Choose control usability.\tCheck|3|$bool|0x00000004|properties\tImage|2||1003|str2\tIcon Image|2||1003|OptionImg\tOverlayImage|3|$bool|0x00000800|properties|Choose Overlay Image. Default is false.\tVoice|1||1|desc|voice reading\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3");
	m_infoMap.SetAt(FM_RADIO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tText|1||1|str\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tGroup Name|1|||auxs\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tEnable|3|$bool|0x00000001|properties|Choose control usability.\tSelected|3|$bool|0x00000004|properties\tImage|2||1003|str2\tIcon Image|2||1003|OptionImg\tVoice|1||1|desc|voice reading\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3");
	m_infoMap.SetAt(FM_EDIT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tAlignment|3|$alignment||align\tDataKind|3|5-Digit;1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||type\tLineColor|2||1002|bRGB|Set the line color.\tBorder|3|0-None;5-Line;||borders\tLength|1|||size|Set the length of character(s)\tDefaultData|1|||str\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tSelectColor|2||1002|spRGB|Selected color.\tAlpha|1|2||alpha|Specify Alpha Value.\tHintText|1|||onHint\tEncrypt|3|$bool|0x00400000|attr\tEncryptKey|1|||auxs\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tEnable|3|$bool|0x00000001|properties|Choose control usability \tFloatDigit|1|||offs\tImage|2||1003|str2\tRTS Symbol|1|||rts|rts symbol.\tVoice|1||1|desc|voice reading\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tEditFormat|1||editS|Edit Format\tAttributes|4||1004|xxx");
	m_infoMap.SetAt(FM_OUT,		"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tLength|1|||size\tAlignment|3|$align||align\tLineColor|2||1002|bRGB|Set the line color.\tBorder|3|0-None;5-Line;||borders\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tAlpha|1|2||alpha|Specify Alpha Value.\tFloatDigit|1|||offs\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tText Resize|3|$bool|0x00000800|properties|Text resize in rect\tEnable|3|$bool|0x00000001|properties|Choose control usability \tImage|2||1003|str2\tRts Symbol|1|||rts|rts symbol.\tVoice|1||1|desc|voice reading.\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tEdit Format|1||editS|Edit Format\tDomino Name|1|||auxs\tAttributes|4||1004|xxx");
	m_infoMap.SetAt(FM_COMBO,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tSelectIndex|1|||checked\tDataLength|1|||size|Set the length of combo data\tVisibleCount|1|||Hkeys\tDataCount|2|1|1005|Lkeys|Edit Combo Contents and display the number of contents.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color\tSelectColor|2||1002|spRGB|Selected color.\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility\tEnable|3|$bool|0x00000001|properties|Choose control usability \tImage|2||1003|str2\tVoice|1||1|desc|voice reading\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tButtonImage|2||1003|OptionImg|Drop Button Image\tListImage|2||1003|OptionImg2|List Background Image\tSelectedImage|2||1003|OptionImg3|Selected Item Image\tListLineColor|2||1002|stRGB|Specify line color of the list\tListTextColor|2||1002|opRGB|Specify line color of the list");
	m_infoMap.SetAt(FM_GRID,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tRows|1|||rows\tColumns|1||1|cols\tFixedRow|1|||fixedrow|Set the fixed number of row.\tFixedCol|1|||fixedcol|Set the fixed number of column.\tHeadHeight|1|||HeadH|Grid Head Height.\tVisibleRows|1|||Hkeys|Set the number of visible rows when AutoSize is set true.\tCellHeight|1|||Lkeys|Set the height of cell when AutoSize is set false.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tDataAppend|3|0-Bottom;1-Top;|0x00000010|attr2|Set Data inserted direction.\tBackColor|2||1002|pRGB|Set the grid back color.\tLineColor|2||1002|bRGB|Set the grid line color.\tRowColor|3|$bool|0x00000040|attr2|Choose 1st and 2nd Row Color option?.\tRowsOfColor|1|||rowsof|Specify rows for eac\t row color\t1stRowColor|2||1002|spRGB|Specify color for odd numbered row?\t2ndRowColor|2||1002|stRGB|Specify color for even numbered row?\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tFocusColor|2||1002|onEdit|Specify focus color\tNoHead|3|$bool|0x00000020|attr2|Set no grid header option.\tNoVertical Line|3|$bool|0x00000080|attr2\tNoHorizon line|3|$bool|0x00000100|attr2\tRTS Blink|3|$bool|0x00010000|attr2|Set rts blink on/off.\tBlink Color|2||1002|opRGB|Specify blink color on rts.\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tColumns...|4||1009|xxx");
	m_infoMap.SetAt(FM_GRIDEX,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tRows|1|||rows\tColumns|1||1|cols\tFixedRow|1|||fixedrow|Set the fixed number of row.\tFixedCol|1|||fixedcol|Set the fixed number of column.\tHeadHeight|1|||HeadH|Grid Head Height.\tVisibleRows|1|||Hkeys|Set the number of visible rows when AutoSize is set true.\tCellHeight|1|||Lkeys|Set the height of cell when AutoSize is set false.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tDataAppend|3|0-Bottom;1-Top;|0x00000010|attr2|Set Data inserted direction.\tBackColor|2||1002|pRGB|Set the grid back color.\tLineColor|2||1002|bRGB|Set the grid line color.\tRowColor|3|$bool|0x00000040|attr2|Choose 1st and 2nd Row Color option?.\tRowsOfColor|1|||rowsof|Specify rows for eac\t row color\t1stRowColor|2||1002|spRGB|Specify color for odd numbered row?\t2ndRowColor|2||1002|stRGB|Specify color for even numbered row?\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tFocusColor|2||1002|onEdit|Specify focus color\tHeader Option|3|0-None;1-Merge;2-MultiLine;||attr2|Set Header option for the grid.\tNoHead|3|$bool|0x00000020|attr2|Set no grid header option.\tNoVertical Line|3|$bool|0x00000080|attr2\tNoHorizon line|3|$bool|0x00000100|attr2\tDrag Column|3|$bool|0x00008000|attr2\tRTS Blink|3|$bool|0x00010000|attr2|Set rts blink on/off.\tBlink Color|2||1002|opRGB|Specify blink color on rts.\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tColumns...|4||1009|xxx");
	m_infoMap.SetAt(FM_TABLE,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tCount|1||1|rows\tAutoSize|3|$bool|0x00002000|attr2\tCellHeight|1|||Akeys\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tStyle|3|0-Combine;1-Head;2-Data;||tableHead\tLineColor|2||1002|bRGB\tHeadWidth|1|||allcellwidth\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tRTS Blink|3|$bool|0x00010000|attr2|Set rts blink on/off.\tBlink Shape|3|0-Backgorund;1-Box;||BlinkShape|Specify blink background/box on rts.\tBlink Color|2||1002|opRGB|Specify blink color on rts.Define Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tRows...|4||1009|xxx");
	m_infoMap.SetAt(FM_OBJECT,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tFormName|1|0008|1|str\tTabOrder|1|1||index|Set tab order.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.");
	m_infoMap.SetAt(FM_TAB,		"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tHeadText|1||1|tabHead\tHeadData|1|||tabData\tHeadVisible|3|$bool||tabVisible\tSelectIndex|1|||checked|Set the initial page of the user tab.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tFocusBackColor|2||1002|spRGB|Specify background color on the selected page.\tFocusForeColor|2||1002|stRGB|Specify letter color on the selected page.\tAlpha|1|2||alpha|Specify Alpha Value.\tImage|2||1003|str2\tVisible Items|1|||type\tTabKind|3|$fixSize|0x00000040|properties|Tab Fixed.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tTab Option|3|$bool|0x00000800|properties|Choose Option Button Visble. Default is false.\tOption Image|2||1003|OptionImg\tSub Form|1||1|desc|Sub Form Name\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tPageEdit|4||1013|xxx");
	m_infoMap.SetAt(FM_BROWSER,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tMode|3|0-None;3-Output;||iok\tStartPage|1|||str\tVisible|3|$bool|0x00000002|properties|Choose Visibility.");
	m_infoMap.SetAt(FM_CONTROL,	"Name|1|||name|Enter Symbol Name.\tCustom|1|1||str|Custom Object.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tLength|1|||size|Set the length of character(s).\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tAlpha|1|2||alpha|Specify Alpha Value.\tRts Symbol|1|||rts|rts symbol.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tAttributes|4||1004|xxx");

	m_infoMap.SetAt(FM_GRIDCELL,	"Name|1|0030||cellname\tWidth|1|||cellwidth\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2||1002|cellhpRGB\tHeadForeColor|2||1002|cellhtRGB\tDataBackColor|2||1002|cellpRGB\tDataForeColor|2||1002|celltRGB\tAlpha|1|2||alpha|Specify Alpha Value.\tVisible|3|$bool|0x00000002|cellproperties\tSort|3|$bool|0x00000040|cellproperties\tFloatDigit|1|||cellvals\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tLength|1|||cellsize\tHeadImage|2||1003|cellstr4\tRts Symbol|1|||cellrts|rts symbol.\tEdit Format|1||celleditS|celleditS\tDomino Name|1||cellauxs|cellauxs\tText Resize|3|$bool|0x00000800|cellproperties|Text resize in rect\tDefine Margin|3|$bool|0x00000008|cellproperties|Define special margin\tTop Margin|1||1|cellmargin0\tLeft Margin|1||1|cellmargin1\tBottom Margin|1||1|cellmargin2\tRight Margin|1||1|cellmargin3\tAttributes|4||1004|xxx");
	m_infoMap.SetAt(FM_GRIDEXCELL,	"Name|1|0030||cellname\tWidth|1|||cellwidth\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2||1002|cellhpRGB\tHeadForeColor|2||1002|cellhtRGB\tDataBackColor|2||1002|cellpRGB\tDataForeColor|2||1002|celltRGB\tAlpha|1|2||alpha|Specify Alpha Value.\tHeadHeight|1|||cellheadH|Head height in Multiline Grid.\tCellHeight|1|||cellheight|Cell height in Multiline Grid.\tVisible|3|$bool|0x00000002|cellproperties\tSort|3|$bool|0x00000040|cellproperties\tFloatDigit|1|||cellvals\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tMode|3|0-None;1-Input;3-Output;2-Both;||celliok\tType|3|0-Normal;1-Check;2-Combo;3-Button;||cellkind\tCheckBoxText|1|||cellstr2\tExclusiveCheck|3|$bool|0x00000004|cellproperties\tComboCount|2|1|1011|cellkeys|Edit Combo Contents and display the number of contents.\tButtonType|3|0-Text;1-Bmp;2-Both;|0x00000010;0x00000020|cellproperties\tButtonText|1|0256||celltxt\tButtonImage|2||1003|cellstr3\tLength|1|||cellsize\tFont|2|1|1001|cellfname|Choose Font Name.\tFontSize|1|||cellfsize|Set Font Size.\tFontStyle|3|$fstyle||cellfstyle|Choose Font Style.\tHeadImage|2||1003|cellstr4\tRts Symbol|1|||cellrts|rts symbol.\tEdit Format|1||celleditS|celleditS\tDomino Name|1||cellauxs|cellauxs\tText Resize|3|$bool|0x00000800|cellproperties|Text resize in rect\tMerge Column|1||cellmerge|cellmerge\tDrag Row|3|$bool|0x00000400|cellproperties|Drag Row\tDefine Margin|3|$bool|0x00000008|cellproperties|Define special margin\tTop Margin|1||1|cellmargin0\tLeft Margin|1||1|cellmargin1\tBottom Margin|1||1|cellmargin2\tRight Margin|1||1|cellmargin3\tAttributes|4||1004|xxx");
	m_infoMap.SetAt(FM_TABLEROW,	"Name|1|0030||cellname\tDataKind|3|1-Numeric;2-AlphaNumeric;3-MultiByte;4-Mixed;||celltype\tLength|1|||cellsize\tHead|1||1|cellhead\tHeadAlignment|3|0-Center;1-Left;2-Right;||cellalignH\tDataAlignment|3|0-Center;1-Left;2-Right;||cellalignC\tHeadBackColor|2||1002|cellhpRGB\tHeadForeColor|2||1002|cellhtRGB\tAlpha|1|2||alpha|Specify Alpha Value.\tDataBackColor|2||1002|cellpRGB\tDataForeColor|2||1002|celltRGB\tHintText|1|||cellonHint\tFloatDigit|1|||cellvals\tVisible|3|$bool|0x00000002|cellproperties\tRts Symbol|1|||cellrts|rts symbol.\tEdit Format|1||celleditS|celleditS\tDomino Name|1||cellauxs|cellauxs\tAttributes|4||1004|xxx");
	m_infoMap.SetAt(FM_TABSEL,	"Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tHeadText|1||1|tabHead\tHeadData|1|||tabData\tHeadVisible|3|$bool||tabVisible\tSelectIndex|1|||checked|Set the initial page of the user tab.\tFont|2|1|1001|fname|Choose Font Name.\tFontSize|1|||fpoint|Set Font Size.\tFontStyle|3|$fstyle||fstyle|Choose Font Style.\tBackColor|2||1002|pRGB|Specify Background Color.\tForeColor|2||1002|tRGB|Specify letter color.\tFocusBackColor|2||1002|spRGB|Specify background color on the selected page.\tFocusForeColor|2||1002|stRGB|Specify letter color on the selected page.\tAlpha|1|2||alpha|Specify Alpha Value.\tImage|2||1003|str2\tVisible Items|1|||type\tItemSize|3|$fixSize|0x00000040|properties|Tab Fixed.\tVisible|3|$bool|0x00000002|properties|Choose Visibility.\tTab Option|3|$bool|0x00000800|properties|Choose Option Button Visble. Default is false.\tOption Image|2||1003|OptionImg|Select Option Image.\tForm Name|1|||desc|Sub Form Name\tDefine Margin|3|$bool|0x00000008|properties|Define special margin\tTop Margin|1||1|margin0\tLeft Margin|1||1|margin1\tBottom Margin|1||1|margin2\tRight Margin|1||1|margin3\tPageEdit|4||1013|xxx");
	m_infoMap.SetAt(FM_FORM,	"Title|1|||mapcaption|Enter title of map.\tWidth|1|||mapwidth|Enter the width of the map.\tHeight|1|||mapheight|Enter the height of the map.\tFont|2|1|1001|mapfname|Specify map's Font name.\tFontSize|1|||mapfpoint|Set map's Font size.\tFontStyle|3|$fstyle||mapfstyle|Choose map's Font style.\tVersion|1|||mapvers|Enter version of the map.\tName|1|0008||mapname|Enter the name of the map.\tDescription|1|0020||mapdesc|Enter the description of the map.\tTxCode|1|0008||maptrxC\tRepeat|1|||repeatV\tBackColor|2||1002|mapBkClr\tAlpha|1|2||alpha|Specify Alpha Value.\tForm Controller|1|||formCtrl|Form Controller Name.\tForm Type|3|0-normal;1-vscroll;2-hscroll;||formtype|form type\tOptions|4||1008|xxx");
	m_infoMap.SetAt(FM_PROCEDURES,	"Title|1|||mapcaption\tVersion|1|||mapvers\tName|1|0008|1|mapname\tGroup|3|||mapGroup\tDescription|1|0020||mapdesc");
	// Layout
	m_infoMap.SetAt(FM_LAYOUT,	"Left|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tAvailable|3|$bool|0x00000002|properties|Choose available.");
}

CPropertiesWnd::~CPropertiesWnd()
{
	m_infoMap.RemoveAll();
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

	loadPropertyFile();

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

	//bitmap.LoadBitmap(IDB_CONTROL);	
	bitmap.LoadBitmap(IDR_DRAWBAR);
	ilist.Create(20, 20, ILC_COLOR24|ILC_MASK, 26, 1);
	ilist.Add(&bitmap, RGB(192, 192, 192));
	ilist.Add(AfxGetApp()->LoadIcon(IDI_MAP));
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

void CPropertiesWnd::OnSelCombo()
{
	if (m_formItem == NULL)
		return;
	if (m_wndCtrlCombo.GetCurSel() != CB_ERR)
	{
		CString info;
		int	idx = (int)m_wndCtrlCombo.GetItemData(m_wndCtrlCombo.GetCurSel());
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SELCTRLTOCHILD, MAKELPARAM(idx, SC_PROPERTIESBAR));

		if (m_infoMap.Lookup((idx >= 0 ? m_formItem->GetForm(idx)->kind : -1), info))
		{
			if (idx == -1)
			{
				if (m_mapH->mapK == MK_PROCEDURES)
					m_infoMap.Lookup(FM_PROCEDURES, info);
				else
					m_infoMap.Lookup(idx, info);
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

void CPropertiesWnd::Initialize(struct _mapH* mapH, CFormItem* formItem)
{
	m_mapH = mapH;
	m_formItem = formItem;

	m_wndPropList.Initialize(mapH, formItem);
}

void CPropertiesWnd::EnableProperty(BOOL bEnable)
{
	m_wndPropList.EnableWindow(bEnable);
	m_wndCtrlCombo.EnableWindow(bEnable);
}

int CPropertiesWnd::AddControl(CString sStr, DWORD data)
{
	int	nImage = 0;

	if ((int)data >= 0 && m_formItem && m_formItem->GetCount() > (int)data)
		nImage = m_formItem->GetForm(data)->kind;
	else if ((int)data < 0 && m_mapH)
		nImage = 23 + m_mapH->mapK;

	COMBOBOXEXITEM cbExItem;
	cbExItem.mask    = CBEIF_TEXT|CBEIF_IMAGE|CBEIF_LPARAM|CBEIF_SELECTEDIMAGE;
	cbExItem.iItem   = 0;
	cbExItem.pszText = (LPTSTR)(LPCTSTR)sStr;
	cbExItem.iImage  = nImage;
	cbExItem.iSelectedImage  = nImage;
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

		for (int ii = m_formItem->GetCount() - 1; ii >= 0; ii--)
		{
			if (m_formItem->GetForm(ii)->block)
			{
				formArr.Add(getPropertyStr(ii));
				m_formItem->GetForm(ii)->bCommon = true;
			}
			else
				m_formItem->GetForm(ii)->bCommon = false;
		}

		info = compositeInArr(formArr);

		if (!info.IsEmpty())
		{
			m_wndPropList.SetPropInfo(2000, info);
			changeProperty(2000, info);
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
		
			if (m_mapH->formR && selctl >= 0)
			{
				if (m_formItem->GetForm(idx)->group)
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
				changeProperty(idx, getPropertyStr(-1));
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
	CString	info;
	int	pos;

	if (m_infoMap.Lookup(FM_FORM, info))
	{
		pos = info.Find(_T("||mapGroup"));
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			m_infoMap.SetAt(FM_FORM, info);
			m_wndPropList.SetPropInfo(FM_FORM, info);
		}
	}

	if (m_infoMap.Lookup(FM_PROCEDURES, info))
	{
		pos = info.Find(_T("||mapGroup"));
		if (pos >= 0)
		{
			info = info.Left(pos) + sGroup + info.Mid(pos);
			m_infoMap.SetAt(FM_PROCEDURES, info);
			m_wndPropList.SetPropInfo(FM_PROCEDURES, info);
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

	for (POSITION pos = m_infoMap.GetStartPosition(); pos; )
	{
		m_infoMap.GetNextAssoc(pos, key, text);
		m_wndPropList.SetPropInfo(key, text);
	}
}

void CPropertiesWnd::loadPropertyFile()
{
	CString	text;
	CFile	file;
	char*	wb;

	text = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	text += _T("\\tab\\properties.dat");

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
			parseProperties(text);
		delete[] wb;
	}
}

void CPropertiesWnd::parseProperties(CString data)
{
	CMapStringToPtr mapFormID;

	mapFormID.SetAt(_T("LABEL"),		(void*)FM_LABEL);
	mapFormID.SetAt(_T("BOX"),		(void*)FM_BOX);
	mapFormID.SetAt(_T("GROUP"),		(void*)FM_GROUP);
	mapFormID.SetAt(_T("PABNEL"),		(void*)FM_IMAGEVIEW);
	mapFormID.SetAt(_T("BUTTON"),		(void*)FM_BUTTON);
	mapFormID.SetAt(_T("EDIT"),		(void*)FM_EDIT);
	mapFormID.SetAt(_T("COMBO"),		(void*)FM_COMBO);
	mapFormID.SetAt(_T("OUT"),		(void*)FM_OUT);
	mapFormID.SetAt(_T("GRID"),		(void*)FM_GRID);
	mapFormID.SetAt(_T("GRIDEX"),		(void*)FM_GRIDEX);
	mapFormID.SetAt(_T("TABLE"),		(void*)FM_TABLE);
	mapFormID.SetAt(_T("OBJECT"),		(void*)FM_OBJECT);
	mapFormID.SetAt(_T("TAB"),		(void*)FM_TAB);
	mapFormID.SetAt(_T("BROWSER"),		(void*)FM_BROWSER);
	mapFormID.SetAt(_T("CONTROL"),		(void*)FM_CONTROL);
	mapFormID.SetAt(_T("RADIO"),		(void*)FM_RADIO);
	mapFormID.SetAt(_T("CHECK"),		(void*)FM_CHECK);
	mapFormID.SetAt(_T("CELL"),		(void*)FM_GRIDCELL);
	mapFormID.SetAt(_T("CELLEX"),		(void*)FM_GRIDEXCELL);
	mapFormID.SetAt(_T("ROW"),		(void*)FM_TABLEROW);
	mapFormID.SetAt(_T("TAB SELECTED"),	(void*)FM_TABSEL);
	mapFormID.SetAt(_T("FORM"),		(void*)FM_FORM);
	mapFormID.SetAt(_T("PROCEDURES"),	(void*)FM_PROCEDURES);
	mapFormID.SetAt(_T("LAYOUT"),		(void*)FM_LAYOUT);	// Layout Properties Read

	bool	bStartIn = false;
	void*	rtValue  = NULL;
	CString	data_pro, token;

	data.Replace(_T("\r\n"), _T("\n"));
	while (!data.IsEmpty())
	{
		token = parse(data, _T("\n"));
		if (!token.CompareNoCase(_T("#start")))
		{
			bStartIn = false;
			token = parse(data, _T("\n"));
			Msg("");
			Msg("%s", token);
			Msg("");
			if (mapFormID.Lookup(token, rtValue))
			{
				bStartIn = true;
				data_pro = _T("");
				token = parse(data, _T("\n"));
			}
		}

		if (!token.CompareNoCase(_T("#end")))
		{
			m_infoMap.SetAt((long)rtValue, data_pro);			
			bStartIn = false;
		}

		if (bStartIn)
		{
			Msg("%s", token);
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

		switch (m_formItem->GetForm(idx)->kind)
		{
		case FM_GRID:
			if (m_formItem->GetForm(idx)->auxR != (char *)NULL && ((struct _repR *)m_formItem->GetForm(idx)->auxR)->selected >= 0)
				return FM_GRIDCELL;
			break;

		case FM_GRIDEX:
			if (m_formItem->GetForm(idx)->auxR != (char *)NULL && ((struct _repR *)m_formItem->GetForm(idx)->auxR)->selected >= 0)
				return FM_GRIDEXCELL;
			break;

		case FM_TABLE:
			if (m_formItem->GetForm(idx)->auxR != (char *)NULL && ((struct _repR *)m_formItem->GetForm(idx)->auxR)->selected >= 0)
				return FM_TABLEROW;
			break;

		case FM_TAB:
			if (m_formItem->GetForm(idx)->keys)
				return FM_TABSEL;
			break;

		default:
			break;
		}

		if (m_formItem->GetForm(idx)->group && bGroupCheck)
		{
			CStringArray	formArr;
			struct	_formR*	nGroup = m_formItem->GetForm(idx)->group;

			for (int ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				m_formItem->GetForm(ii)->bCommon = false;
				if (m_formItem->GetForm(ii)->group == nGroup)
				{
					formArr.Add(getPropertyStr(ii, false));
					m_formItem->GetForm(ii)->bCommon = true;
				}
			}
			m_wndPropList.SetPropInfo(2000, compositeInArr(formArr));
			return 2000;
		}

		return m_formItem->GetForm(idx)->kind;
	}

	if (idx == -1)
	{
		if (m_mapH->mapK == MK_PROCEDURES)
			return FM_PROCEDURES;
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
		m_infoMap.Lookup(m_formItem->GetForm(idx)->kind, info);
		switch (m_formItem->GetForm(idx)->kind)
		{
		case FM_GRID:
			if (m_formItem->GetForm(idx)->auxR != (char *)NULL && ((struct _repR *)m_formItem->GetForm(idx)->auxR)->selected >= 0)
				m_infoMap.Lookup(FM_GRIDCELL, info);
			break;

		case FM_GRIDEX:
			if (m_formItem->GetForm(idx)->auxR != (char *)NULL && ((struct _repR *)m_formItem->GetForm(idx)->auxR)->selected >= 0)
				m_infoMap.Lookup(FM_GRIDEXCELL, info);
			break;

		case FM_TABLE:
			if (m_formItem->GetForm(idx)->auxR != (char *)NULL && ((struct _repR *)m_formItem->GetForm(idx)->auxR)->selected >= 0)
				m_infoMap.Lookup(FM_TABLEROW, info);
			break;

		case FM_TAB:
			if (m_formItem->GetForm(idx)->keys)
				m_infoMap.Lookup(FM_TABSEL, info);
			break;
		}

		if (m_formItem->GetForm(idx)->group && bGroupCheck)
		{
			CStringArray	formArr;
			struct	_formR*	nGroup = m_formItem->GetForm(idx)->group;

			for (int ii = 0; ii < m_formItem->GetCount(); ii++)
			{
				m_formItem->GetForm(ii)->bCommon = false;
				if (m_formItem->GetForm(ii)->group == nGroup)
				{
					formArr.Add(getPropertyStr(ii, false));
					m_formItem->GetForm(ii)->bCommon = true;
				}
			}
			
			info = compositeInArr(formArr);
		}
	}
	else if (idx == -1)
	{
		if (m_mapH->mapK == MK_PROCEDURES)
			m_infoMap.Lookup(FM_PROCEDURES, info);
		else
			m_infoMap.Lookup(idx, info);
	}
	return info;
}

void CPropertiesWnd::changeProperty(int idx, CString info)
{
	int headNum;

	if (!m_bLayout)
		headNum = (idx < 0 ? idx : getPropertyInt(idx, true));
	else
		headNum = FM_LAYOUT;

	if (idx < 0)
	{
		if (m_mapH->mapK == MK_NORM)
			headNum = -1;
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