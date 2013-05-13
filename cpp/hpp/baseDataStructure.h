#ifndef BASEDATASTRUCTURE_H_INCLUDED
#define BASEDATASTRUCTURE_H_INCLUDED


#include <vector>
#include <string>


#include <highgui.h>
#include <cv.h>



using namespace std;


typedef struct VCDFeature
{
    double feature[3][3][72];
}VCDFeature;

typedef struct shot_info
{
    IplImage *image;
    IplImage *gray;
    IplImage *canny;

    bool isVCD;
    bool isFMPI;

    int64_t startDTS;
    double startTime;
    int64_t endDTS;
    double endTime;

    vector<double > FMPIFeature;
    VCDFeature  vcdFeature;
}shot_info;

typedef struct image_info
{
    IplImage * image;
    IplImage * gray;
    IplImage * canny;

    bool isVCD ;
    bool isFMPI;

    int64_t dts;
    double time;

    vector<double > FMPIFeature;
    VCDFeature  vcdFeature;
}image_info;



typedef struct VCD_SD
{
    double time;
    bool isVCD;
    bool isFMPI;
}VCD_SD;











#endif // BASEDATASTRUCTURE_H_INCLUDED





















