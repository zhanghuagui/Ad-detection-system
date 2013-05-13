#ifndef RCONFIGFILE_H_INCLUDED
#define RCONFIGFILE_H_INCLUDED

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <fstream>

using namespace std;

typedef map<string ,string ,less<string> > strMap;
typedef strMap::iterator strMapIt;

const char *const MIDDLESTRING = "_____***_____";

struct analyzeini
{
    string strsect;
    strMap *pmap;

    analyzeini(strMap &strMap):pmap(&strMap){}

    void operator()(const string &strini)
    {
        int first=strini.find('[');
        int last=strini.rfind(']');
        if(first!=string::npos&&last!=string::npos&&first!=last+1)
        {
            strsect=strini.substr(first+1,last-first-1);
            return ;
        }
        if(strsect.empty())
           return ;
////////////////////////////////////////////////////////////////////////////////////////
        if((first=strini.find('='))==string::npos)
           return ;
        string strmp1=strini.substr(0,first);
        string strmp2=strini.substr(first+1,string::npos);

        first=strmp1.find_first_not_of(" \t");
        last =strmp1.find_last_not_of(" \t");
        if(first==string::npos||last==string::npos)
           return ;
        string strkey=strmp1.substr(first,last-first+1);

        first=strmp2.find_first_not_of(" \t");
        if(((last=strmp2.find("\t#",first))!=-1)||
          ((last=strmp2.find(" #",first))!=-1)||
          ((last=strmp2.find("\t//",first))!=-1)||
          ((last=strmp2.find(" //",first))!=-1))
        {
            strmp2=strmp2.substr(0,last-first);
        }
        last=strmp2.find_last_not_of(" \t");
        if(first==string::npos||last==string::npos)
          return ;
        string value=strmp2.substr(first,last-first+1);

        string mapkey=strsect + MIDDLESTRING;
        mapkey+=strkey;

        (*pmap)[mapkey]=value;

        return;
    }

};

class IniFile
{
public:
     IniFile(){};
     ~IniFile(){};
     bool open(const char* ini_file_path)
     {
         return do_open(ini_file_path);
     }

     string read(const char* psect,const char* pkey)
     {
         string mapkey = psect;
         mapkey +=MIDDLESTRING;
         mapkey +=pkey;
         strMapIt it=c_inimap.find(mapkey);
         if(it==c_inimap.end())
            return "";
        else
            return it->second;
     }
protected:
      bool do_open(const char* ini_file_path)
      {
          ifstream filein(ini_file_path);
          if(!filein.is_open())
             return false;
          vector<string > strvect;
          while(!filein.eof())
          {
              string inbuf;
              getline(filein,inbuf,'\n');
              strvect.push_back(inbuf);
          }
          if(strvect.empty())
             return false;
          for_each(strvect.begin(),strvect.end(),analyzeini(c_inimap));

          return !c_inimap.empty();
      }
    strMap c_inimap;
};


#endif // RCONFIGFILE_H_INCLUDED



























