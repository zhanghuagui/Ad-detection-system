#ifndef FILEOPERATION_H_INCLUDED
#define FILEOPERATION_H_INCLUDED

#include "baseDataStructure.h"

#include <iostream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

using namespace std;

class fileOP
{
public:
    fileOP();
    fileOP(const string folderpath, vector <shot_info> shots);
    ~fileOP();

    void get_path_from_folder();
    void get_path_from_folder(const string folderpath, vector<string>& filepath,vector<string >& filename);

    void image_to_shotlist();
    void image_to_shotlist(vector<string >filepath,vector<shot_info >& shots);

    vector<string> output_filepath();

    vector<string> output_filename();

    vector<shot_info> output_shotlist();



private:
    vector<string > filePath;
    vector<string > fileName;
    vector<shot_info> shotList;
    string folderPath;
};


#endif // FILEOPERATION_H_INCLUDED
