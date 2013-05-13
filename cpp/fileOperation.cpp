#include "hpp/fileOperation.h"

fileOP::fileOP()
{

}

fileOP::fileOP(const string folderpath,vector<shot_info> shots)
{
    folderPath=folderpath;
    shotList=shots;
}

fileOP::~fileOP()
{

}

void fileOP::get_path_from_folder()
{
    DIR *dp;
    struct dirent *dirp;

    if((dp=opendir(folderPath.c_str()))==NULL)
    {
        cout<<"Error ("<<errno<<") opening "<<folderPath<<endl;
        exit(0);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        string subDir=folderPath;
        if(strcmp(".",dirp->d_name)==0||strcmp("..",dirp->d_name)==0)
        {
        }
        else
        {
            string subDir1=string(dirp->d_name);
            fileName.push_back(subDir1);
            subDir=subDir+subDir1;
            filePath.push_back(subDir);
        }
    }

    closedir(dp);

}

void fileOP::get_path_from_folder(const string folderpath, vector<string >& filepath, vector<string >& filename)
{
    DIR *dp;

    struct dirent *dirp;

    if((dp=opendir(folderpath.c_str()))==NULL)
    {
        cout<<"Error ("<<errno<<") opening "<<folderPath<<endl;
        exit(0);
    }

    while((dirp=readdir(dp))!=NULL)
    {
        string subDir=folderpath;
        if(strcmp(".",dirp->d_name)==0||strcmp("..",dirp->d_name)==0)
        {
        }
        else
        {
            string subDir1=string(dirp->d_name);
            filename.push_back(subDir1);
            subDir=subDir+subDir1;
            filepath.push_back(subDir);
        //    cout<<"floderPath:"<<folderpath<<endl;
        //   cout<<"subDir:"<<subDir<<endl;
        //   cout<<"subDir1:"<<subDir1<<endl;
        }
    }

    closedir(dp);

}

void fileOP::image_to_shotlist()
{
    shot_info shotinfo;
    int pathSize = filePath.size();
    int i=0;
    for(i=0;i<pathSize; i++)
    {
        IplImage* src = cvLoadImage(filePath.at(i).c_str());
        IplImage* gray= cvCreateImage(cvGetSize(src),src->depth,1);
        cvCvtColor(src,gray,CV_BGR2GRAY);
        shotinfo.image=src;
        shotinfo.gray= gray;
        shotList.push_back(shotinfo);

        cvReleaseImage(&src);
        cvReleaseImage(&gray);
    }
    return ;
}

void fileOP::image_to_shotlist(vector<string > filepath, vector<shot_info >& shots)
{
    shot_info shotinfo;
    int pathSize = filepath.size();
    int i=0;
    for(i=0;i<pathSize;i++)
    {
        IplImage* src=cvLoadImage(filepath.at(i).c_str());
        IplImage* gray=cvCreateImage(cvGetSize(src),src->depth,1);
        cvCvtColor(src,gray,CV_BGR2GRAY);
        shotinfo.image=src;
        shotinfo.gray = gray;
        shots.push_back(shotinfo);

        cvReleaseImage(&src);
        cvReleaseImage(&gray);
    }
    return ;
}

vector<string > fileOP::output_filename()
{
    get_path_from_folder();
    return fileName;
}

vector<string > fileOP::output_filepath()
{
    get_path_from_folder();
    return filePath;
}

vector<shot_info > fileOP::output_shotlist()
{
    get_path_from_folder();
    image_to_shotlist();
    return shotList;
}














