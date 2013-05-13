
#include "hpp/date.h"
//读取当前系统的日期
void gpGetCurDate(char * pszDate)
{
	time_t l=time(NULL);
	struct tm *ptm;
	time_t t=(time_t)l;

	ptm=localtime(&t);
	if (ptm==NULL)
	{
		time_t t;
		time(&t);
		ptm=localtime(&t);
	}
	sprintf(pszDate,"%04d-%02d-%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday);

}

void gpGetCurTime(char* pszTime)
{
	time_t l=time(NULL);
	struct tm *ptm;
	time_t t=(time_t)l;

	ptm=localtime(&t);
	if (ptm==NULL)
	{
		time_t t;
		time(&t);
		ptm=localtime(&t);
	}
	sprintf(pszTime,"%02d:%02d:%02d",ptm->tm_hour,ptm->tm_min,ptm->tm_sec);


}

//读取当前的StrDate(YYYY-MM-DD HH:MI:SS)
void gpGetCurStrDate(char *pszDate)
{
	time_t t=time(NULL);
	gpTimeT2StrDate(pszDate,t);
}

//time_t -> Date(YYYY-MM-DD HH:MI:SS)
void gpTimeT2StrDate(char *pszDate,long l)
{
    struct tm *ptm;
    time_t t=(time_t)l;

    ptm=localtime(&t);
    if (ptm==NULL)
    {
        time_t t;
        time(&t);
        ptm=localtime(&t);
    }
    sprintf(pszDate,"%04d-%02d-%02d %02d:%02d:%02d",ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
}



//Date(YYYY-MM-DD HH:MI:SS) or Date(YY-MM-DD HH:MI:SS) -> struct tm
void gpStrDate2Tm(const char *pszDate,struct tm *ptm)
{
    if (pszDate==NULL) return;
    if (ptm==NULL) return;
    if (*pszDate==0)
    {
        time_t t=0;
        struct tm *ptmTmp=localtime(&t);
        memcpy(ptm,ptmTmp,sizeof(struct tm));
        return;
    }
    int nYear=atoi(pszDate);
    if (nYear<100) nYear+=2000;

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nMonth=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nDay=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nHour=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nMinute=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nSecond=atoi(pszDate);

    ptm->tm_year=nYear-1900;
    ptm->tm_mon=nMonth-1;
    ptm->tm_mday=nDay;
    ptm->tm_hour=nHour;
    ptm->tm_min=nMinute;
    ptm->tm_sec=nSecond;
}

//Date(YYYY-MM-DD HH:MI:SS) or Date(YY-MM-DD HH:MI:SS) -> time_t
long gpStrDate2TimeT(const char *pszDate)
{
    if (*pszDate==0) return 0;

    int nYear=atoi(pszDate);
    if (nYear<100) nYear+=2000;

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nMonth=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nDay=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nHour=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nMinute=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nSecond=atoi(pszDate);

    struct tm tm;
    tm.tm_year=nYear-1900;
    tm.tm_mon=nMonth-1;
    tm.tm_mday=nDay;
    tm.tm_hour=nHour;
    tm.tm_min=nMinute;
    tm.tm_sec=nSecond;
    return mktime(&tm);
}

//IntDate(YYMMDDHHMI) -> struct tm
void gpIntDate2Tm(int nDate,struct tm *ptm)
{
    memset(ptm,0,sizeof(struct tm));
    ptm->tm_min=nDate % 100;
    nDate/=100;

    ptm->tm_hour=nDate % 100;
    nDate/=100;

    ptm->tm_mday=nDate % 100;
    nDate/=100;

    ptm->tm_mon=(nDate % 100)-1;
    nDate/=100;

    ptm->tm_year=(nDate % 100)+100;
    nDate/=100;
}

//IntDate(YYMMDDHHMI) -> Date(YYYY-MM-DD HH:MI:SS)
void gpIntDate2StrDate(char *pszDate,int nDate)
{
    int nMinute=nDate % 100;
    nDate/=100;

    int nHour=nDate % 100;
    nDate/=100;

    int nDay=nDate % 100;
    nDate/=100;

    int nMonth=nDate % 100;
    nDate/=100;

    int nYear=nDate % 100;
    nDate/=100;

    if (nYear<100) nYear+=2000;

    sprintf(pszDate,"%04d-%02d-%02d %02d:%02d:00",nYear,nMonth,nDay,nHour,nMinute);
}

//IntDate(YYMMDDHHMI) -> time_t
time_t gpIntDate2TimeT(int nDate)
{
    struct tm tm;
    memset(&tm,0,sizeof(tm));
    tm.tm_min=nDate % 100;
    nDate/=100;

    tm.tm_hour=nDate % 100;
    nDate/=100;

    tm.tm_mday=nDate % 100;
    nDate/=100;

    tm.tm_mon=(nDate % 100)-1;
    nDate/=100;

    tm.tm_year=(nDate % 100)+100;
    nDate/=100;

    return mktime(&tm);
}

//Date(YYYY-MM-DD HH:MI:SS) or Date(YY-MM-DD HH:MI:SS) -> IntDate(YYMMDDHHMI)
int gpStrDate2IntDate(char *pszDate)
{
    int nYear=atoi(pszDate);
    if (nYear<100) nYear+=2000;

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nMonth=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nDay=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nHour=atoi(pszDate);

    while (isdigit(*pszDate)) pszDate++;
    while (*pszDate && !isdigit(*pszDate)) pszDate++;
    int nMinute=atoi(pszDate);

    return nMinute+nHour*100+nDay*10000+nMonth*1000000+(nYear % 100)*100000000;
}

//Date(Ddd Mmm Date hh:mm:ss yyyy) -> time_t
int gpAscTime2TimeT(const char *pszAscTime)
{
    struct tm tm;
    memset(&tm,0,sizeof(tm));

    const char *s=pszAscTime+4;

    //Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec
    tm.tm_mon=0;
    if (strncmp(s,"Jan",3)==0) tm.tm_mon=1;
    else if (strncmp(s,"Feb",3)==0) tm.tm_mon=2;
    else if (strncmp(s,"Mar",3)==0) tm.tm_mon=3;
    else if (strncmp(s,"Apr",3)==0) tm.tm_mon=4;
    else if (strncmp(s,"May",3)==0) tm.tm_mon=5;
    else if (strncmp(s,"Jun",3)==0) tm.tm_mon=6;
    else if (strncmp(s,"Jul",3)==0) tm.tm_mon=7;
    else if (strncmp(s,"Aug",3)==0) tm.tm_mon=8;
    else if (strncmp(s,"Sep",3)==0) tm.tm_mon=9;
    else if (strncmp(s,"Oct",3)==0) tm.tm_mon=10;
    else if (strncmp(s,"Nov",3)==0) tm.tm_mon=11;
    else if (strncmp(s,"Dec",3)==0) tm.tm_mon=12;
    tm.tm_mon--;

    s+=4;
    tm.tm_mday=atoi(s);
    s+=3;
    tm.tm_hour=atoi(s);
    s+=3;
    tm.tm_min=atoi(s);
    s+=3;
    tm.tm_sec=atoi(s);
    s+=3;
    tm.tm_year=atoi(s)-1900;

    return mktime(&tm);
}

//--------------------------------------------------------------------
//在原有的Date中加上多少秒，传进来的dwDate必须是YYMMDDHHMI
//--------------------------------------------------------------------
unsigned int gpAddDate(unsigned int dwDate,unsigned int dwSeconds)
{
    struct tm tm;
    gpIntDate2Tm(dwDate,&tm);
    tm.tm_yday=0;
    tm.tm_wday=0;
    tm.tm_isdst=0;
    time_t t=mktime(&tm);
    t+=dwSeconds;
    struct tm *ptm=localtime(&t);
    return ((ptm->tm_year+1900-1999)<<24) + ((ptm->tm_mon+1)<<18) + ((ptm->tm_mday)<<12) + ((ptm->tm_hour)<<6) + (ptm->tm_min);
}

//--------------------------------------------------------------------
//返回的dwDate是YYMMDDHHMI的
//--------------------------------------------------------------------
unsigned int gpGetCurDate()
{
    char szTmp[40];
    gpGetCurStrDate(szTmp);
    return gpStrDate2IntDate(szTmp);
}
