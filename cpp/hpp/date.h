#ifndef _DATA_H
#define _DATA_H

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//读取当前的日期
void gpGetCurDate(char *pszDate);

//读取当前的时间
void gpGetCurTime(char *pszTime);

//读取当前的StrDate(YYYY-MM-DD HH:MI:SS)
void gpGetCurStrDate(char *pszDate);

//time_t -> Date(YYYY-MM-DD HH:MI:SS)
void gpTimeT2StrDate(char *pszDate,long l);

//Date(YYYY-MM-DD HH:MI:SS) or Date(YY-MM-DD HH:MI:SS) -> struct tm
void gpStrDate2Tm(const char *pszDate,struct tm *ptm);

//Date(YYYY-MM-DD HH:MI:SS) or Date(YY-MM-DD HH:MI:SS) -> time_t
long gpStrDate2TimeT(const char *pszDate);

//IntDate(YYMMDDHHMI) -> struct tm
void gpIntDate2Tm(int nDate,struct tm *ptm);

//IntDate(YYMMDDHHMI) -> Date(YYYY-MM-DD HH:MI:SS)
void gpIntDate2StrDate(char *pszDate,int nDate);

//IntDate(YYMMDDHHMI) -> time_t
time_t gpIntDate2TimeT(int nDate);

//Date(YYYY-MM-DD HH:MI:SS) or Date(YY-MM-DD HH:MI:SS) -> IntDate(YYMMDDHHMI)
int gpStrDate2IntDate(char *pszDate);

//Date(Ddd Mmm Date hh:mm:ss yyyy) -> time_t
int gpAscTime2TimeT(const char *pszAscTime);

//--------------------------------------------------------------------
//在原有的Date中加上多少秒，传进来的dwDate必须是YYMMDDHHMI
//--------------------------------------------------------------------
unsigned int gpAddDate(unsigned int dwDate,unsigned int dwSeconds);

//--------------------------------------------------------------------
//返回的dwDate是YYMMDDHHMI的
//--------------------------------------------------------------------
unsigned int gpGetCurDate();


#endif