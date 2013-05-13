#ifndef DATABASEOPERATION_H_INCLUDED
#define DATABASEOPERATION_H_INCLUDED

#include "baseDataStructure.h"

#include <string>
#include <vector>
#include <mysql.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;



class dbOperation
{
public:
    dbOperation(string dbip,string dbuser, string dbpwd, string dbname);
    ~dbOperation();

    vector<VCDFeature > query_vcdfeature_from_db();



private:
    const char * dbIP;
    const char * dbUSER;
    const char * dbPWD;
    const char * dbNAME;

    vector<VCDFeature > vcdFeature;


};


#endif // DATABASEOPERATION_H_INCLUDED
