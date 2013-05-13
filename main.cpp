
#include "cpp/hpp/FMPI.h"
#include "cpp/hpp/baseDataStructure.h"
#include "cpp/hpp/databaseOperation.h"
#include "cpp/hpp/inputByVideo.h"
#include "cpp/hpp/rConfigFile.h"
#include "cpp/hpp/videoCopyDetection.h"
#include "cpp/hpp/videoShotCut.h"
#include "cpp/hpp/videoSuspectDetection.h"
#include "cpp/hpp/fileOperation.h"
#include "cpp/hpp/xml.h"
#include "cpp/hpp/date.h"

#include <iostream>
using namespace std;

string server;
string username;
string password;
string videoFeatureDB;

string scalepath;
string modelpath;

string folderpath;

int read_config()
{
    IniFile readConf;
    if(!readConf.open("config.ini"))
    {
        return -1;
    }
    server=readConf.read("DB","SERVER");
    username=readConf.read("DB","USERNAME");
    password=readConf.read("DB","PASSWORD");
    videoFeatureDB = readConf.read("DB","VCD_DATABASE");

    scalepath=readConf.read("SVM","SCALEPATH");
    modelpath=readConf.read("SVM","MODELPATH");

    folderpath=readConf.read("PATH","FOLDERPATH");
    return 1;
}

int main()
{
  av_register_all();
///////load test video//////////////////////////////////////////////////////////////////////
  videoCapture video;
 ///////////////////////////////////////////////////////////////////////////////////////
  image_info  imageInfo;
  image_info imageInfo1;
//////////////////////////////////////////////////////////////////////////////////////////
  VCD_SD vcd_sd;
  vector<VCD_SD> vcd_sd_list;

  if(read_config()==-1)
  {
      printf("Error:read config file failed!\n");
      exit(0);
  }
//////load feature from database/////////////////////////////////////////////////////////////////////////////////
  vector<VCDFeature> vcdFeature;
  dbOperation getVCDFeatureFromDB(server,username,password,videoFeatureDB);
  vcdFeature = getVCDFeatureFromDB.query_vcdfeature_from_db();
/////////////////////////////////////////////////////////////////////////////////////////
  videoCD copyDetection(vcdFeature);
///////////////////////////////////////////////////////////////////////////////////////
  FMPI fmpi;
/////////load suspectDectection scale and model//////////////////////////////////////////////////////////////////////////////
  videoSD videosd(scalepath,modelpath);
//////////////////////////////////////////////////////////////////////////////////////
  videoShotCut videoshotcut;
////////////////////////////////////////////////////////////////////////////////////////
  //get video path from folder
  fileOP fileop;
  vector<string> filepath;
  vector<string> filename;
  fileop.get_path_from_folder(folderpath,filepath,filename);

//////////////////////////////////////////////////////////////////////////////////

    IplImage * curFrame=NULL;
    IplImage * gray = NULL;

    cvNamedWindow("curWindow",CV_WINDOW_AUTOSIZE);
   // cvNamedWindow("grayWindow",CV_WINDOW_AUTOSIZE);
/////////////////////////////////////////////////////////////////////////

   unsigned int video_num=0;
   unsigned int frame_num=0;//25
  // unsigned int time_num=0;//18000

   while(video_num < filepath.size())
   {
       if(!video.load_video(filepath.at(video_num).c_str()))
       {
          printf("Error:Load video error!\n");
          video_num++;
          continue;
       }

/////////////////////////////////////////////////////////////////////////
       bool isFirstImageOfShot=true;
       curFrame=video.query_frame_from_video();
       gray =cvCreateImage(cvGetSize(curFrame),curFrame->depth,1);

       while(true)
       {
          // double st = cvGetTickCount();

          curFrame=video.query_frame_from_video();
          if(!curFrame)
          {
             video_num++;
             //filename.at(video_num)
             //Write XML
             vcd_sd_list.clear();
             break;
          }

          frame_num++;
          if(frame_num>=10)
          {
            cvShowImage("curWindow",curFrame);
            imageInfo.image =curFrame;
    //      gray =cvCreateImage(cvGetSize(curFrame),curFrame->depth,1);
            cvCvtColor(curFrame, gray, CV_BGR2GRAY);
        //  cvShowImage("grayWindow",gray);

            imageInfo.gray = gray;
            imageInfo.dts = video.get_frame_dts();
            imageInfo.time = video.get_frame_time();
/*
          if(isFirstImageOfShot)
          {
              copyDetection.output_VCD_imageinfo(imageInfo);

              fmpi.output_FMPI_imageinfo(imageInfo);

              videosd.output_SD_imageinfo(imageInfo);
              imageInfo1=imageInfo;
              isFirstImageOfShot=false;

          }
          else
          {
              if(videoshotcut.shot_similar(imageInfo1.image,imageInfo.image))
              {
                  imageInfo.isVCD=imageInfo1.isVCD;
                  imageInfo.isFMPI=imageInfo1.isFMPI;
                  imageInfo1=imageInfo;
              }
              else
              {
                  copyDetection.output_VCD_imageinfo(imageInfo);
                  fmpi.output_FMPI_imageinfo(imageInfo);
                  videosd.output_SD_imageinfo(imageInfo);
                  isFirstImageOfShot=true;
              }
           }
*/

             copyDetection.output_VCD_imageinfo(imageInfo);

            // fmpi.output_FMPI_imageinfo(imageInfo);//get FMPI feature

           //  videosd.output_SD_imageinfo(imageInfo);//predict

            // imageInfo1=imageInfo;

             vcd_sd.time=imageInfo.time;
             vcd_sd.isVCD= imageInfo.isVCD;
             vcd_sd.isFMPI=imageInfo.isFMPI;
             vcd_sd_list.push_back(vcd_sd);

            // double et = cvGetTickCount();

            // cout<<"time:"<<(et-st)/(cvGetTickFrequency()*1000000)<<endl;

             frame_num=0;

             cvWaitKey(2);
          }
         }
        cvReleaseImage(&gray);
   }
    cvReleaseImage(&curFrame);
    cvDestroyWindow("curWindow");
   // cvDestroyWindow("grayWindow");

    return 0;

}
