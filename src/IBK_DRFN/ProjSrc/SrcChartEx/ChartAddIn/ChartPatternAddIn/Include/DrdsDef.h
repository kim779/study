
// ********************************
// Drds.cfg 와 동일한 구조체 사용
// 실시간으로 내려오는 데이터 등록
//*********************************



// DRDS COMMAND "S31" 현재가
typedef struct
{
	char	shcode   		[ 7];			//종목코드              
    char    time     		[ 6];			//시간(HH:MM:SS)        
    char    sign     		[ 1];			//등락부호              
    char    change   		[ 9];			//등락폭                
    char    price    		[ 9];			//현재가                
    char    chgrate  		[ 7];			//등락률                
    char    open     		[ 9];			//전장시가              
    char    high     		[ 9];			//고가                  
    char    low      		[ 9];			//저가                  
    char    offerho  		[ 9];			//매도호가              
    char    bidho    		[ 9];			//매수호가              
    char    volume   		[10];			//누적거래량            
    char    cvolume  		[10];			//변동거래량            
    char    value    		[12];			//거래대금              
    char    maegb    		[ 1];			//매매구분              
    char    status   		[ 1];			//장구분                
    char    cbgb     		[ 1];			//CB구분                
    char    stop     		[ 1];			//STOP                  
    char    outvol   		[ 9];			//시간외거래량          
    char    outval   		[12];			//시간외거래대금        
    char    outrepvol		[ 9];			//시간외대량매매        
    char    outrepval		[12];			//시간외대량매매대금    
} STPriceData;		
#define SZ_PriceData		sizeof(STPriceData)


// DRDS COMMAND "S31" 시간 데이터(데이터 1개)
typedef struct
{
	char    garbage    		[ 3];			//Garbage    
    char    time    		[ 6];			//시간                
  
} ST_OneTimeData;		
#define SZ_OneTimeData		sizeof(ST_OneTimeData)


// DRDS COMMAND "S31" 수량 데이터(데이터 1개)
typedef struct
{
	char    garbage    		[ 3];			//Garbage    
    char    volume    		[ 10];			//수량
  
} ST_OneVolumeData;		
#define SZ_OneVolumeData	sizeof(ST_OneVolumeData)


// DRDS COMMAND "S31" 현재가 또는 시가, 고가, 저가(데이터 1개)
typedef struct
{
	char    garbage    		[ 3];			//Garbage    
    char    price    		[ 9];			//현재가, 시가, 고가, 저가
  
} ST_OnePriceData;		
#define SZ_OnePriceData		sizeof(ST_OnePriceData)