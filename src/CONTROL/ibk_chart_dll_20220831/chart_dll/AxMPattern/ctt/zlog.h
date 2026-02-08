#ifndef _VLOG_H
#define _VLOG_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


static char	log_fname[150];
static char	vproc_name[100];
static char	*def_log = "/proj/csb/dat/tmp/vlog.log";
static int	set_fg = 0;
static int	set_lfg = 0;
static int	set_title = 1;

#ifdef	LONG_BUFF
#define BUF_LEN	1024 * 65
#else
#define BUF_LEN	1024 * 16
#endif

#ifdef	_MTIME
#define TFORM_LEN	15
#define TFORM_SPT	6
#else
#define TFORM_LEN	8
#define TFORM_SPT	6
#endif
#define  MinLen(x,y) ((x > y) ? y : x )

#define LOGTY_WW	1
#define LOGTY_YYYYMD	2
#define LOGTY_YYMD	3
#define LOGTY_MD	4
#define TRACE	vlog(">>>> Trace File[ %s ] Line( %d )",__FILE__,__LINE__)

static int vlog(char* fmt,...);

static setlog(char *pname,char *lname)
{
	set_fg = 1;
	if(strlen(pname) > 0)
		set_lfg = 1;
	strcpy(vproc_name,pname);
	strcpy(log_fname,lname);
	if(memcmp(pname,"NOTITLE",7) == 0)
		set_title = 0;
}

static set_vfname(char *pname,char *fnam,int ymd)
{
	time_t	today;
	struct	tm *tm;
	
	today = time(0);
	tm = localtime(&today);

	switch(ymd)
	{
	case LOGTY_WW :
		sprintf(log_fname,"%s%02d.log",fnam,tm->tm_wday);
		break;
	case LOGTY_YYYYMD :
		sprintf(log_fname,"%s%04d%02d%02d.log",fnam,
			tm->tm_year +1900, tm->tm_mon+1, tm->tm_mday);
		break;
	case LOGTY_YYMD :
		sprintf(log_fname,"%s%02d%02d%02d.log",fnam,
			tm->tm_year % 100, tm->tm_mon+1, tm->tm_mday);
		break;
	case LOGTY_MD :
		sprintf(log_fname,"%s%02d%02d.log",fnam,
						tm->tm_mon+1, tm->tm_mday);
		break;
	default:
		return 0;
	}
	set_fg = 1;
	if(strlen(pname) > 0)
		set_lfg = 1;
	strcpy(vproc_name,pname);
}


static FILE* log_open()
{
	struct	stat	st;
	struct	tm	tm1, tm2;
	int		rc, time1, time2;
	time_t		tt1, tt2;
	char	*path;

	if(strcmp(log_fname,"DISPLAY") == 0)
		return stdout;

	if(set_fg == 1)
		path = log_fname;
	else
		path = def_log;

	rc = stat (path, &st);
	if (rc < 0)
		return fopen(path, "w");
	tt1 = st.st_ctime;
	localtime_r(&tt1,&tm1);
	time1 = (tm1.tm_mon+1) * 100 + tm1.tm_mday;
	tt2 = time(0);
	localtime_r(&tt2,&tm2);
	time2 = (tm2.tm_mon+1) * 100 + tm2.tm_mday;

	if (time1 == time2)
		return fopen(path, "a");
	else
		return fopen(path, "w");

	return fopen(path, "a");
}

static int log_topen()
{
	struct	stat	st;
	struct	tm	tm1, tm2;
	int		rc, time1, time2;
	time_t		tt1, tt2;
	char	*path;


	if(set_fg == 1)
		path = log_fname;
	else
		path = def_log;

	rc = stat (path, &st);
	if (rc < 0)
		return open(path,O_WRONLY | O_CREAT | O_APPEND, 0644);
	tt1 = st.st_ctime;
	localtime_r(&tt1,&tm1);
	time1 = (tm1.tm_mon+1) * 100 + tm1.tm_mday;
	tt2 = time(0);
	localtime_r(&tt2,&tm2);
	time2 = (tm2.tm_mon+1) * 100 + tm2.tm_mday;

	if (time1 == time2)
	{
		return open(path,O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		unlink(path);
		return open(path,O_WRONLY | O_CREAT | O_APPEND, 0644);
	}

}


static today_check (path)
char	*path;
{
	struct	stat	st;
	struct	tm	tm1, tm2;
	int		rc, time1, time2;
	time_t		tt1, tt2;

	rc = stat (path, &st);
	if (rc < 0)
		return (1);

	tt1 = st.st_ctime;
	localtime_r(&tt1,&tm1);
	time1 = (tm1.tm_mon+1) * 100 + tm1.tm_mday;

	tt2 = time(0);
	localtime_r(&tt2,&tm2);
	time2 = (tm2.tm_mon+1) * 100 + tm2.tm_mday;

	if (time1 == time2)
		return (0);
	else
		return (1);
}


static int vlog(char* fmt,...)
{
#if (_VLOG == 1)
	FILE*	file;
	char 	time_buf[30];
	time_t	curtime;
	struct	tm	tp;
	struct  timeval tval;
	va_list  pvar;

	va_start(pvar, fmt);
	
	if(!(file = log_open()))
		return 0;

	gettimeofday(&tval, NULL);
	curtime = tval.tv_sec;
	/*tval.tv_usec /= 10000;*/

	localtime_r(&curtime,&tp);
	if(set_title == 1)
	{
		if(set_lfg == 1)
			fprintf(file,"[%s]",vproc_name );
	
		sprintf(time_buf,"%02d/%02d %02d:%02d:%02d-%06d", tp.tm_mon+1, 
			tp.tm_mday,tp.tm_hour, tp.tm_min, tp.tm_sec,tval.tv_usec );
		fprintf(file,"[%.*s]",TFORM_LEN, time_buf + TFORM_SPT );
	}
	vfprintf(file,fmt,pvar);
	fprintf(file,"\n");
	if(file != stdout) fclose(file);
	va_end(pvar);
#endif
	return 1;
}



static int vnlog(char* title,char* buf,int len)
{
#if (_VLOG == 1)
	FILE*	file;
	char 	time_buf[30];
	time_t	curtime;
	struct	tm	tp;
	char	temp_buf[BUF_LEN + 1];
	struct  timeval tval;
	int i;

	if( len > BUF_LEN) len = BUF_LEN;
	if(!(file = log_open()))
		return 0;

	gettimeofday(&tval, NULL);
	curtime = tval.tv_sec;
	/*tval.tv_usec /= 10000;*/

	localtime_r(&curtime,&tp);
	if(set_title == 1)
	{
		if(set_lfg == 1)
			fprintf(file,"[%s]",vproc_name );
		sprintf(time_buf,"%02d/%02d %02d:%02d:%02d-%06d",
				tp.tm_mon+1, tp.tm_mday,tp.tm_hour, tp.tm_min,
				tp.tm_sec,tval.tv_usec );
		fprintf(file,"[%.*s]",TFORM_LEN,time_buf + TFORM_SPT );
	}
	for(i=0;i < len; i++){
	if(buf[i] == (char)NULL) temp_buf[i] = '@'; 
	else temp_buf[i] = buf[i];
	}
	temp_buf[len] = (char)NULL;
	fprintf(file," %s [%s]\n",title,temp_buf );
	if(file != stdout) fclose(file);
#endif
	return 1;
}


static int vhlog(char* title,char* buf,int len)
{
#if (_VLOG == 1)
	FILE*	file;
	char	time_buf[30];
	time_t	curtime;
	struct	tm tp;
	char	temp_buf[BUF_LEN + 1];
	struct	timeval tval;
	int		ii, jj, kk;


	if( len > BUF_LEN) len = BUF_LEN;
	if(!(file = log_open()))
		return 0;

	gettimeofday(&tval, NULL);
	curtime = tval.tv_sec;
	/*tval.tv_usec /= 10000;*/

	localtime_r(&curtime,&tp);
	if(set_title == 1)
	{
		if(set_lfg == 1)
			fprintf(file,"[%s]",vproc_name );
		sprintf(time_buf,"%02d/%02d %02d:%02d:%02d-%06d",
				tp.tm_mon+1, tp.tm_mday,
	  			tp.tm_hour, tp.tm_min, tp.tm_sec,tval.tv_usec );
		fprintf(file,"[%.*s] %s len(%d)\n",TFORM_LEN,time_buf  + TFORM_SPT,title, len);
	}

	for (ii = 0; ii < len; )
	{
		kk = ii;
		fprintf(file, "%04d   ", ii);
		for (jj = 0; jj < 15 && ii < len; jj++, ii++)
			fprintf(file, "%02X ", buf[ii] & 0xff);
		for (; jj < 15; jj++)
			fprintf(file, "   ", buf[ii]);
		fprintf(file, "  ");
		for (ii = kk, jj = 0; jj < 15 && ii < len; jj++, ii++)
		{
			if (buf[ii] < ' ' || buf[ii] >= 0x80)
				fprintf(file, ".");
			else	fprintf(file, "%c", buf[ii]);
		}
		fprintf(file, "\n");
	}
	if (len > 0)
		fprintf(file, "\n");


	fflush(file);
	if(file != stdout) fclose(file);
#endif
	return 1;
}



static int vtlog(char* fmt,...)
{
#if (_VLOG == 1)
	int	file, len;
	char 	time_buf[30];
	time_t	curtime;
	struct	tm	tp;
	struct  timeval tval;
	va_list  pvar;
	char	temp_buf[BUF_LEN + 1];

	va_start(pvar, fmt);
	
	if((file = log_topen()) < 0)
		return 0;

	gettimeofday(&tval, NULL);
	curtime = tval.tv_sec;
	/*tval.tv_usec /= 10000;*/

	localtime_r(&curtime,&tp);
	if(set_title == 1)
	{
		if(set_lfg == 1)
		{
			sprintf(temp_buf,"[%s]",vproc_name );
			write(file,temp_buf,strlen(temp_buf));
		}
		sprintf(time_buf,"%02d/%02d %02d:%02d:%02d-%06d", tp.tm_mon+1, 
			tp.tm_mday,tp.tm_hour, tp.tm_min, tp.tm_sec,tval.tv_usec );
		sprintf(temp_buf,"[%.*s]",TFORM_LEN,time_buf + TFORM_SPT );
		write(file,temp_buf,strlen(temp_buf));
	}
	len = vsprintf(temp_buf,fmt,pvar);
	if(len > 0)
		write(file,temp_buf,len);
	write(file,"\n",1);
	close(file);
	va_end(pvar);
#endif
	return 1;
}

static int vtnlog(char* title,char* buf,int len)
{
#if (_VLOG == 1)
	int	file;
	char 	time_buf[30];
	time_t	curtime;
	struct	tm	tp;
	char	temp_buf[BUF_LEN + 1];
	struct  timeval tval;
	int i;

	if( len > BUF_LEN) len = BUF_LEN;
	if((file = log_topen()) < 0)
		return 0;

	gettimeofday(&tval, NULL);
	curtime = tval.tv_sec;
	/*tval.tv_usec /= 10000;*/

	localtime_r(&curtime,&tp);
	if(set_title == 1)
	{
		if(set_lfg == 1)
		{
			sprintf(temp_buf,"[%s]",vproc_name );
			write(file,temp_buf,strlen(temp_buf));
		}
		sprintf(time_buf,"%02d/%02d %02d:%02d:%02d-%06d",
				tp.tm_mon+1, tp.tm_mday,tp.tm_hour, tp.tm_min,
				tp.tm_sec,tval.tv_usec );
	
		sprintf(temp_buf,"[%.*s]",TFORM_LEN,time_buf + TFORM_SPT );
		write(file,temp_buf,strlen(temp_buf));
	}

	for(i=0;i < len; i++){
	if(buf[i] == (char)NULL) temp_buf[i] = '@'; 
	else temp_buf[i] = buf[i];
	}
	temp_buf[len] = (char)NULL;
	write(file," ",1);
	write(file,title,strlen(title));
	write(file," [",2);
	write(file,temp_buf,strlen(temp_buf));
	write(file,"]\n",2);
	close(file);
#endif
	return 1;
}



static int vthlog(char* title,char* buf,int len)
{
#if (_VLOG == 1)
	int	file;
	char	time_buf[30];
	time_t	curtime;
	struct	tm tp;
	char	temp_buf[BUF_LEN + 1];
	struct	timeval tval;
	int		ii, jj, kk;
	char	line_buff[1024];


	if( len > BUF_LEN) len = BUF_LEN;
	if(!(file = log_topen()))
		return 0;

	gettimeofday(&tval, NULL);
	curtime = tval.tv_sec;
	/*tval.tv_usec /= 10000;*/

	localtime_r(&curtime,&tp);
	if(set_title == 1)
	{
		if(set_lfg == 1)
		{
			sprintf(temp_buf,"[%s]",vproc_name );
			write(file,temp_buf,strlen(temp_buf));
		}
	
		sprintf(time_buf,"%02d/%02d %02d:%02d:%02d-%06d",
				tp.tm_mon+1, tp.tm_mday,
	  			tp.tm_hour, tp.tm_min, tp.tm_sec,tval.tv_usec );
		sprintf(temp_buf,"[%.*s] %s len(%d)\n",TFORM_LEN,time_buf  + TFORM_SPT,title, len);
		write(file,temp_buf,strlen(temp_buf));
	}
	
	for (ii = 0; ii < len; )
	{
		kk = ii;
		sprintf(line_buff,"%04d   ", ii);
		write(file,line_buff,strlen(line_buff));

		for (jj = 0; jj < 15 && ii < len; jj++, ii++)
		{
			sprintf(line_buff,"%02X ", buf[ii] & 0xff);
			write(file,line_buff,strlen(line_buff));
		}
		for (; jj < 15; jj++)
		{
			write(file,"   ",strlen("   "));
		}
		write(file,"  ",strlen("  "));

		for (ii = kk, jj = 0; jj < 15 && ii < len; jj++, ii++)
		{
			if (buf[ii] < ' ' || buf[ii] >= 0x80)
				write(file, ".",1);
			else
				write(file, &buf[ii],1);
		}
		write(file, "\n",1);
	}
	if (len > 0)
		write(file, "\n",1);
	close(file);
#endif
	return 1;
}

#endif
