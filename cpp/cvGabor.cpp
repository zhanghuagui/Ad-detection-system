#include "hpp/cvGabor.h"

CvGabor::CvGabor()
{

}

CvGabor::~CvGabor()
{
    cvReleaseMat(&ImagMat);
    cvReleaseMat(&RealMat);
}

CvGabor::CvGabor(int iMu, int iNu)
{
    double dSigma= 2*PI;
    SpaFrequency= sqrt(2.0);
    Init(iMu, iNu,dSigma,SpaFrequency);
}

CvGabor::CvGabor(int iMu, int iNu, double dSigma)
{
    SpaFrequency=sqrt(2.0);
    Init(iMu,iNu,dSigma,SpaFrequency);
}

CvGabor::CvGabor(int iMu ,int iNu, double dSigma, double spafrequencry)
{
    Init(iMu,iNu,dSigma,spafrequencry);
}

CvGabor::CvGabor(double arcdirection,int iNu)
{
    Sigma=2*PI;
    SpaFrequency=sqrt(2.0);
    Init(arcdirection,iNu,Sigma,SpaFrequency);
}

CvGabor::CvGabor(double arcdirection, int iNu, double dSigma)
{
    SpaFrequency=sqrt(2.0);
    Init(arcdirection,iNu,dSigma,SpaFrequency);
}

CvGabor::CvGabor(double arcdirection, int iNu, double dSigma, double spafrequency)
{
    Init(arcdirection,iNu,dSigma,spafrequency);
}

void CvGabor::Init(int iMu, int iNu, double dSigma, double dF)
{
    bInitialised= false;
    bKernel = false;

    Sigma = dSigma;
    SpaFrequency=dF;

    Kmax= PI/2;
    K=Kmax/pow(SpaFrequency,(double)iNu);
    ArcDirection=PI*iMu/8;
    bInitialised= true;
    Width = mask_width();
    RealMat=cvCreateMat(Width,Width,CV_32FC1);
    ImagMat=cvCreateMat(Width,Width,CV_32FC1);
    create_kernel();
}

void CvGabor::Init(double dPhi, int iNu, double dSigma ,double dF)
{
    bInitialised = false;
    bKernel = false;
    Sigma= dSigma;
    SpaFrequency=dF;

    Kmax=PI/2;
    K=Kmax/pow(SpaFrequency,(double)iNu);
    ArcDirection= dPhi;
    bInitialised = true;
    Width= mask_width();
    RealMat=cvCreateMat(Width,Width,CV_32FC1);
    ImagMat=cvCreateMat(Width,Width,CV_32FC1);
    create_kernel();
}

bool CvGabor::IsInit()
{
    return bInitialised;
}

long CvGabor::mask_width()
{
    long lWidth;
    if(IsInit()==false)
    {
        perror("ERROR:The object has not been initilised in mask_width()!\n");
        return 0;
    }
    else
    {
        double dModSigma = Sigma/K;
        double dWidth = (int)(dModSigma*6 +1);
        if(fmod(dWidth,2.0)==0.0)
           dWidth++;
        lWidth = (long)dWidth;

        return lWidth;
    }
}

void CvGabor::create_kernel()//create the gabor kernel
{
    if(IsInit()==false)
    {
        perror("ERROR:The object has not been initilised in create_kernel!\n");
        return ;
    }
    else
    {
        CvMat *mReal, *mImag;
        mReal = cvCreateMat(Width,Width,CV_32FC1);
        mImag = cvCreateMat(Width,Width,CV_32FC1);

        int x, y;
        double dReal;
        double dImag;
        double dTemp1,dTemp2,dTemp3;
        int i=0,j=0;
        for(i=0; i<Width; i++)
        {
            for(j=0;j<Width;j++)
            {
                x= i-(Width-1)/2;
                y= j-(Width-1)/2;
                dTemp1=(pow(K,2)/pow(Sigma,2))*exp(-(pow((double)x,2)+pow((double)y,2))*pow(K,2)/(2*pow(Sigma,2)));
                dTemp2=cos(K*cos(ArcDirection)*x+K*sin(ArcDirection)*y)-exp(-(pow(Sigma,2)/2));
                dTemp3=sin(K*cos(ArcDirection)*x+K*sin(ArcDirection)*y);
                dReal = dTemp1*dTemp2;
                dImag = dTemp1*dTemp3;

                cvSetReal2D((CvMat*)mReal,i,j,dReal);
                cvSetReal2D((CvMat*)mImag,i,j,dImag);
            }
        }
        bKernel = true;
        cvCopy(mReal,RealMat,NULL);
        cvCopy(mImag,ImagMat,NULL);
        cvReleaseMat(&mReal);
        cvReleaseMat(&mImag);
    }
}

void CvGabor::conv_img_a(IplImage *src, IplImage *dst, int Type)
{
    double ve, re, im;

    int width=src->width;
    int height= src->height;
    CvMat *mat=cvCreateMat(src->width,src->height,CV_32FC1);

    int i=0, j=0;
    for(i=0;i<width;i++)
    {
        for(j=0;j<height;j++)
        {
            ve=cvGetReal2D((IplImage *)src,j,i);
            cvSetReal2D((CvMat*)mat,i,j,ve);
        }
    }

    CvMat *rmat=cvCreateMat(width,height,CV_32FC1);
    CvMat *imat=cvCreateMat(width,height,CV_32FC1);

    CvMat *kernel=cvCreateMat(Width,Width,CV_32FC1);
    switch (Type)
    {
        case CV_GABOR_REAL:
           cvCopy((CvMat *)RealMat, (CvMat *)kernel, NULL);
           cvFilter2D((CvMat *)mat, (CvMat *)mat, (CvMat *)kernel, cvPoint((Width-1)/2,(Width-1)/2));
           break;
        case CV_GABOR_IMAG:
           cvCopy((CvMat*)ImagMat, (CvMat *)kernel,NULL);
           cvFilter2D((CvMat*)mat, (CvMat*)mat,(CvMat*)kernel, cvPoint((Width-1)/2,(Width-1)/2));
           break;
        case CV_GABOR_MAG:
           cvCopy((CvMat*)RealMat, (CvMat*)kernel, NULL);
           cvFilter2D((CvMat*)mat, (CvMat*)rmat, (CvMat*)kernel, cvPoint((Width-1)/2,(Width-1)/2));

           cvCopy((CvMat*)RealMat, (CvMat*)kernel, NULL);
           cvFilter2D((CvMat*)mat,(CvMat*)imat,(CvMat*)kernel, cvPoint((Width-1)/2,(Width-1)/2));

           for(i=0;i<width;i++)
           {
               for(j=0;j<height;j++)
               {
                   re=cvGetReal2D((CvMat*)rmat,i,j);
                   im=cvGetReal2D((CvMat*)imat,i,j);
                   ve=sqrt(re*re + im*im);
                   cvSetReal2D((CvMat *)mat, i, j, ve);
               }
           }
           break;
        case CV_GABOR_PHASE:
           break;
    }

    if(dst->depth == IPL_DEPTH_8U)
    {
        cvNormalize((CvMat*)mat, (CvMat*)mat, 0, 255, CV_MINMAX, NULL);
        for(i=0; i<width; i++)
        {
            for(j=0; j<height; j++)
            {
                ve=cvGetReal2D((CvMat *)mat, i, j);
                ve=cvRound(ve);
                cvSetReal2D((IplImage*)dst, j,i,ve);
            }
        }
    }

    if(dst->depth==IPL_DEPTH_32F)
    {
        for(i=0;i<width;i++)
        {
            for(j=0;j<height;j++)
            {
                ve=cvGetReal2D((CvMat*)mat,i,j);
                cvSetReal2D((IplImage *)dst, j,i,ve);
            }
        }
    }

    cvReleaseMat(&kernel);
    cvReleaseMat(&imat);
    cvReleaseMat(&rmat);
    cvReleaseMat(&mat);
}

void CvGabor::split_statistic(IplImage *des, double n[16])
{
    int bin_x,bin_y;

    bin_x=des->width/4;
    bin_y=des->height/4;

    IplImage *temp=cvCreateImage(cvSize(bin_x,bin_y),des->depth,des->nChannels);
    double svg=0.0;
    int tem_n=0;
    int i=0,j=0;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            cvSetImageROI(des,cvRect(j*bin_x, i*bin_y,bin_x,bin_y));
            cvCopy(des, temp);
            cvResetImageROI(des);
            svg=cvAvg(temp,NULL).val[0];

            n[tem_n++]=svg;
            cvZero(temp);
        }
    }
    cvReleaseImage(&temp);
}

void CvGabor::gabor_logo_detection(IplImage *source ,double res[][16])
{
    IplImage *des=cvCreateImage(cvGetSize(source),source->depth, source->nChannels);

    int imu,inu;
    int i=0;
    for(i=0;i<4;i++)
    {
        imu=2*i;
        inu=1;
        SpaFrequency=sqrt(2.0);
        Init(imu,inu,2*PI,SpaFrequency);
        conv_img_a(source,des,1);
        cvAdaptiveThreshold(des,des,255,CV_ADAPTIVE_THRESH_GAUSSIAN_C,CV_THRESH_BINARY,3,5);

        split_statistic(des,res[i]);
    }
    cvReleaseImage(&des);
}






















































