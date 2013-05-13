#include "hpp/databaseOperation.h"

dbOperation::dbOperation(const string dbip,const string dbuser, const string dbpwd,const string dbname)
{
    dbIP = dbip.c_str();
    dbUSER = dbuser.c_str();
    dbPWD = dbpwd.c_str();
    dbNAME = dbname.c_str();
}

dbOperation::~dbOperation()
{

}

vector<VCDFeature >  dbOperation::query_vcdfeature_from_db()
{
    MYSQL mysql;
    MYSQL_RES *res=NULL;
    MYSQL_ROW row=NULL;

    string sTemp,rowTemp;
    int i=0;
    int iTemp=0, count=0, a=0, b=0, c=0, d=0;

    int test_cout=0;

    VCDFeature vcdfeature;

    mysql_init(&mysql);
    if(!mysql_real_connect(&mysql,dbIP,dbUSER,dbPWD,dbNAME,0 , NULL,0))
    {
        printf("Connect to the feature db error!\n");
        exit(0);
    }

    string query1="select feature from frame_feature ";
    if(mysql_real_query(&mysql, query1.c_str(), (unsigned)query1.size()))
    {
        printf("Query VCD feature from the database error!\n");
        exit(0) ;
    }

    res=mysql_store_result(&mysql);
    while(row = mysql_fetch_row(res))
    {
        test_cout++;
       // printf("test_cout=%d\n",test_cout);
        rowTemp = row[0];
        for(i=0;i<rowTemp.size();i++)
        {
            if(rowTemp[i]!=' ')
            {
                sTemp.push_back(rowTemp[i]);
            }
            else
            {
                iTemp = atoi(sTemp.c_str());
                c=count%72;
                d=count/72;
                a=d/3;
                b=d%3;
                if(a>2||a<0||b>2||b<0||c>71||c<0)
                {
                    printf("Error:read the feature from database fault!\n");
                    exit(0);
                }
                vcdfeature.feature[a][b][c]=(double)iTemp/(double)100000;
                sTemp.clear();
                ++count;
            }
        }
        count=0;
        a=0;
        b=0;
        c=0;
        d=0;
       vcdFeature.push_back(vcdfeature);
    }
    mysql_free_result(res);
    mysql_close(&mysql);

    return vcdFeature;
}























