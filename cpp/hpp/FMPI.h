#ifndef FMPI_H_INCLUDED
#define FMPI_H_INCLUDED

#include <highgui.h>
#include <cv.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <fstream>

#include "baseDataStructure.h"
#include "cvGabor.h"

#define  BLOCK  4
#define  HBIN_NUM  4

#define  HSV_BIN_NUM  18

using namespace std;

class FMPI
{
public:

   FMPI();
   ~FMPI();
   void get_FMPI_feature(vector<double > &feature, IplImage* src, IplImage* srcGray);

   void get_edg_statistic(IplImage*srcGray, vector<double >& edgStatistic);
   IplImage *canny_edg_detection(IplImage *srcGray);
   int get_canny_threshold(CvMat* mat,double *low, double *high);
   int adaptive_find_threshold(CvMat *dx,CvMat *dy, double *low, double *high);

   void get_gabor_logo_feature(IplImage* srcGray, double gaborStatistic[][16]);

   void image_2_HSV_hist(IplImage* src, vector<double>& histStatistic);
   CvHistogram* calculate_histogram(IplImage * src);
   void statistic_hist(CvHistogram* hist,vector<double>& histStatistic);

   void feature_formate(vector<double>& edgFeature,double gaborFeature[4][16],vector<double>& hsvHistBin);

   void  output_FMPI_imageinfo(image_info &imageInfo);

   void write_feature_to_file(vector<vector<double> >& FMPIfeature, string trainFilePath);

private:


};

#endif // FMPI_H_INCLUDED




















































