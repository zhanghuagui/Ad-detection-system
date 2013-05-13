#ifndef VIDEOSHOTCUT_H_INCLUDED
#define VIDEOSHOTCUT_H_INCLUDED

#include "baseDataStructure.h"
#include <highgui.h>
#include <cv.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <iostream>

#define INIT_WINDOW  15
class videoShotCut
{
public:
    videoShotCut();
    ~videoShotCut();

    IplImage *get_edge(IplImage *srcFrame);
    int get_canny_threshold(CvMat* mat, double * low, double *high);
    int adaptive_find_threshold(CvMat* dx, CvMat* dy,double *low, double *high);

    double calculate_average_v(IplImage *srcFrame);
    double get_ECR(IplImage *canny1, IplImage *canny2);
    CvHistogram * Calculate_Histogram_HV(IplImage * src);
    float compare_hist_image(IplImage *srcFrame1, IplImage *srcFrame2,int modo);

    bool shot_similar(IplImage *src1, IplImage *src2);


};


#endif // VIDEOSHOTCUT_H_INCLUDED
