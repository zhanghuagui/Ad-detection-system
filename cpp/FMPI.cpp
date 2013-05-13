#include "hpp/FMPI.h"

FMPI::FMPI()
{

}

FMPI::~FMPI()
{

}


void FMPI::get_FMPI_feature(vector<double > &feature, IplImage* src, IplImage* srcGray)
{
    get_edg_statistic(srcGray,feature);

    double gabor[4][16];
    get_gabor_logo_feature(srcGray,gabor);

    vector<double > hsvHistBin;
    image_2_HSV_hist(src, hsvHistBin);

    feature_formate(feature, gabor, hsvHistBin);

}

void FMPI::get_edg_statistic(IplImage*srcGray, vector<double >& edgStatistic)
{
    IplImage *des= canny_edg_detection(srcGray);

    int width = des->width;
    int height = des->height;
    int blockW = des->width/BLOCK;
    int blockH = des->height/BLOCK;

    char* edgMap = des->imageData;
    int i=0, j=0, k=0,l=0;

    for(i=0; i< BLOCK; i++)
    {
        for(j=0;j<BLOCK; j++)
        {
            edgStatistic.push_back(0);
            for(k=i*blockH; k<(i+1)*blockH; k++)
            {
                for(l=j*blockW; l<(j+1)*blockW; l++)
                {
                    int tindex = k*width +l;
                    if(edgMap[tindex] !=0)
                    {
                        ++edgStatistic[i*BLOCK+j];
                    }
                }
            }
            edgStatistic[i*BLOCK+i] = edgStatistic[i*BLOCK+j]/(blockH*blockW);
        }
    }

    cvReleaseImage(&des);
}

IplImage* FMPI::canny_edg_detection(IplImage *srcGray)
                                                                                                    {
    IplImage* canny = cvCreateImage(cvGetSize(srcGray),IPL_DEPTH_8U, 1);

    CvMat* grayMat = cvCreateMat(cvGetSize(srcGray).height, cvGetSize(srcGray).width, CV_8UC1);
    cvConvert(srcGray, grayMat);
    double low, high;

    int check = get_canny_threshold(grayMat, &low, &high);

    if(check==-1)
    {
        printf("Error:can not get canny threshold!/n");
        cvReleaseMat(&grayMat);
        return NULL;
    }
    else
    {
        cvCanny(srcGray, canny, low*4, high*3);
        cvReleaseMat(&grayMat);
        return canny;
    }
}

int FMPI::get_canny_threshold(CvMat* mat,double *low, double *high)
{
    CvMat* dx = cvCreateMat(mat->height, mat->width, CV_16SC1);
    CvMat* dy = cvCreateMat(mat->height, mat->width, CV_16SC1);

    cvSobel(mat, dx, 1, 0, 3);
    cvSobel(mat, dy, 0, 1, 3);

    int check = adaptive_find_threshold(dx, dy, low, high);
    if(check == -1)
    {
        printf("Error:can not adaptive find threshold!/n");
        cvReleaseMat(&dx);
        cvReleaseMat(&dy);
        return -1;
    }
    else
    {
        cvReleaseMat(&dx);
        cvReleaseMat(&dy);
        return 1;
    }

}

int FMPI::adaptive_find_threshold(CvMat *dx,CvMat *dy, double *low, double *high)
{
    CvSize size;
    IplImage *image = NULL;
    size = cvGetSize(dx);
    image = cvCreateImage(size, IPL_DEPTH_32F, 1);
    int i = 0, j = 0;
    CvHistogram *hist;
    int histSize = 255;
    float range_0[] = {0, 255};
    float * ranges[] = {range_0};
    double percentOfPixelsNotEdges = 0.7;


    float maxV = 0;
    for(i=0; i<size.height; i++)
    {
        const short* _dx = (short *) (dx->data.ptr + dx->step * i);
        const short* _dy = (short *) (dy->data.ptr + dy->step * i);
        float* _image = (float* ) (image->imageData + image->widthStep*i);
        for(j=0; j<size.width; j++)
        {
            _image[j]=(float)(abs(_dx[j])+ abs(_dy[j]));
            maxV = maxV <_image[j]?_image[j] : maxV;
        }
    }
    if(maxV == 0)
    {
        cvReleaseImage(&image);
        *high = 60;
        *low = 80;
        return 1;
    }
    range_0[1]= maxV;
    histSize = (int)(histSize>maxV ? maxV : histSize);

    hist = cvCreateHist(1,&histSize, CV_HIST_ARRAY, ranges, 1);
    cvCalcHist(&image, hist, 0, NULL);
    int total = (int) (size.height * size.width * percentOfPixelsNotEdges);
    float sum = 0;
    int  iCount = hist->mat.dim[0].size;

    float *h = (float *)cvPtr1D(hist->bins, 0);
    for(i=0; i<iCount; i++)
    {
        sum +=h[i];
        if(sum > total)
          break;
    }

    *high = (i+1) * maxV /histSize +0.001;
    *low = *high *0.4;

    cvReleaseImage(&image);
    cvReleaseHist(&hist);

    return 1;
}

void FMPI::get_gabor_logo_feature(IplImage* srcGray, double gaborStatistic[][16])
{
    CvGabor  getGaborFeature;
    getGaborFeature.gabor_logo_detection(srcGray, gaborStatistic);
}

void FMPI::image_2_HSV_hist(IplImage* src, vector<double>& histStatistic)
{
    CvHistogram* hist = NULL;
    hist = calculate_histogram(src);
    if(hist)
    {
        statistic_hist(hist, histStatistic);
    }
    else
    {
        printf("Error:calculate hist error!\n");
        return ;
    }
}

CvHistogram* FMPI::calculate_histogram(IplImage * src)
{
    if(src)
    {
        IplImage* h_plane = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U,1);
        IplImage* s_plane = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U,1);
        IplImage* v_plane = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U,1);
        IplImage* planes[] = {h_plane, s_plane, v_plane};
        IplImage* hsv = cvCreateImage(cvGetSize(src),IPL_DEPTH_8U, 3);

        int h_bins=HSV_BIN_NUM;
        int hist_size[]={h_bins};
        float h_ranges[]={0,180};
        float* ranges[]={h_ranges};

        CvHistogram * hist =0;
        float max_value= 0;
        cvCvtColor(src, hsv, CV_BGR2HSV);
        cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);

        hist=cvCreateHist(1,hist_size,CV_HIST_ARRAY, ranges, 1);

        cvCalcHist(&h_plane,hist, 0,0);
        cvNormalizeHist(hist, 1.0);

        cvReleaseImage(&h_plane);
        cvReleaseImage(&s_plane);
        cvReleaseImage(&v_plane);
        cvReleaseImage(&hsv);
        return hist;

    }
    else
        return NULL;
}

void FMPI::statistic_hist(CvHistogram* hist,vector<double>& histStatistic)
{
    const int m= HSV_BIN_NUM;
    double f=0.0;
    int i=0;
    for(i=0;i<m ;i++)
    {
        f=cvQueryHistValue_1D(hist,i);
        histStatistic.push_back(f);
    }

    sort(histStatistic.begin(),histStatistic.end());
    reverse(histStatistic.begin(),histStatistic.end());
}

void FMPI::feature_formate(vector<double>& edgFeature,double gaborFeature[4][16],vector<double>& hsvHistBin)
{
    int i=0,j=0;
    for(i=0;i<4;i++)
    {
        for(j=0;j<16;j++)
        {
            edgFeature.push_back(gaborFeature[i][j]);
        }
    }

    for(i=0;i<HSV_BIN_NUM;i++)
    {
        edgFeature.push_back(hsvHistBin.at(i));
    }
}

void FMPI::write_feature_to_file(vector<vector<double> >& FMPIfeature, string trainFilePath)
{
    int i=0,j=0;
    int featureSize;
    string label = "-1";
    ofstream fileOut;
    fileOut.open(trainFilePath.c_str());
    if(fileOut.is_open())
    {
        featureSize=FMPIfeature.size();
        for(i=0;i<featureSize;i++)
        {
            int index=1;
            vector<double> feature=FMPIfeature.at(i);
            fileOut << label <<" ";

            for(j=0;j<feature.size();j++)
            {
                fileOut<<index++ << ":"<<feature[j]<<" ";
            }
            fileOut<<endl;
        }

    }
    else
    {
        printf("ERROR:can not open train.txt file!\n ");
        return ;
    }
}

void FMPI::output_FMPI_imageinfo(image_info &imageInfo)
{

    get_FMPI_feature(imageInfo.FMPIFeature, imageInfo.image, imageInfo.gray);
}


















