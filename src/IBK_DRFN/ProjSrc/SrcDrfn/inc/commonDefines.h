#define FUT_KEY_NUM    '1'	
#define CALL_KEY_NUM   '2'	
#define PUT_KEY_NUM    '3'	
#define SP_KEY_NUM     '4'	

#define FUT_KEY_ALPHA  'A'	
#define CALL_KEY_ALPHA 'B'	
#define PUT_KEY_ALPHA  'C'	
#define SP_KEY_ALPHA   'D'

#define IS_FUT_CODE_1(x)				  ( x == '1' || x == 'A' )
#define IS_CALL_CODE_1(x)				  ( x == '2' || x == 'B' )
#define IS_PUT_CODE_1(x)				  ( x == '3' || x == 'C' )
#define IS_SPREAD_CODE_1(x)				  ( x == '4' || x == 'D' )

#define IS_FUT_CODE_3(x)				  ( x == "101" || x == "A01" )
#define IS_CALL_CODE_3(x)				  ( x == "201" || x == "B01" )
#define IS_PUT_CODE_3(x)				  ( x == "301" || x == "C01" )
#define IS_SPREAD_CODE_3(x)				  ( x == "401" || x == "D01" )
#define IS_3GBOND_CODE_3(x)	  			  ( x == "165" || x == "A65" ) // 3년 국채
#define IS_5GBOND_CODE_3(x)	  			  ( x == "166" || x == "A66" ) // 5년 국채
#define IS_10GBOND_CODE_3(x)     		  ( x == "167" || x == "A67" ) // 10년 국채
#define IS_DOLLAR_CODE_3(x)	              ( x == "175" || x == "A75" ) // 미국 달러
#define IS_DOLLAR_CALL_CODE_3(x)		  ( x == "275" || x == "B75" ) // 미국 달러 옵션
#define IS_YEN_CODE_3(x)				  ( x == "176" || x == "A76" ) // 엔
#define IS_EURO_CODE_3(x)				  ( x == "177" || x == "A77" ) // 유로
#define IS_LEAN_HOGS_FUT_3(x)			  ( x == "186" || x == "A86" ) // 돈육
#define IS_OLDGOLD_CODE_3(x)			  ( x == "185" || x == "A85" ) // 금
#define IS_MINI_GOLD_FUT_3(x)			  ( x == "187" || x == "A87" ) // 미니금
#define IS_START_CODE_3(x)				  ( x == "103" || x == "A03" ) // 스타트 선물