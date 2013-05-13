#ifndef CVGABOR_H_INCLUDED
#define CVGABOR_H_INCLUDED

#include <highgui.h>
#include <cv.h>

#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>

#include "baseDataStructure.h"

#define PI 3.14159265
#define CV_GABOR_REAL   1
#define CV_GABOR_IMAG   2
#define CV_GABOR_MAG    3
#define CV_GABOR_PHASE  4

class CvGabor{
public:
     CvGabor();
     ~CvGabor();

     CvGabor(int iMu, int iNu);
     CvGabor(int iMu,int iNu,double dSigma);
     CvGabor(int iMu,int iNu,double dSigma,double dF);
     CvGabor(double dPhi,int iNu);
     CvGabor(double dPhi,int iNu,double dSigma);
     CvGabor(double dPhi,int iNu,double dSigma,double dF);

     bool IsInit();
     long mask_width();

     void Init(int iMu,int iNu,double dSigma,double dF);
     void Init(double dPhi,int iNu,double dSigma,double dF);

     void conv_img_a(IplImage *src, IplImage *dst, int Type);

     void split_statistic(IplImage *des, double n[]);
     void gabor_logo_detection(IplImage *src, double res[][16]);

protected:
     double Sigma;
     double ArcDirection;//Phi
     double SpaFrequency;//F
     CvMat *ImagMat;
     CvMat *RealMat;
     bool bInitialised;
     double K;
     double Kmax;
     long Width;//the width of window
     bool bKernel;

private:
     void create_kernel();

};


#endif // CVGABOR_H_INCLUDED





















