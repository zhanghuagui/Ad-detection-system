#include "hpp/videoShotCut.h"

videoShotCut::videoShotCut()
{

}

videoShotCut::~videoShotCut()
{

}

IplImage* videoShotCut::get_edge(IplImage *srcFrame)
{
    IplImage * canny=cvCreateImage(cvGetSize(srcFrame),IPL_DEPTH_8U, 1);
    IplImage * gray =cvCreateImage(cvGetSize(srcFrame),IPL_DEPTH_8U, 1);

    cvCvtColor(srcFrame, gray, CV_BGR2GRAY);
    CvMat* gMat = cvCreateMat(cvGetSize(srcFrame).height,cvGetSize(srcFrame).width,CV_8UC1);
    cvConvert(gray,gMat);

    double low, high;
    int check = get_canny_threshold(gMat, &low, &high);

    if(check == -1)
    {
        printf("Error:can not get canny threshold!/n");
        cvReleaseImage(&gray);
        cvReleaseMat(&gMat);
        return NULL;
    }
    else
    {
        cvCanny(gray, canny, low*4, high*4);

        cvReleaseImage(&gray);
        cvReleaseMat(&gMat);

        return canny;
    }
}

int videoShotCut::get_canny_threshold(CvMat* mat, double * low, double *high)
{
    CvMat* dx = cvCreateMat(mat->height, mat->width,CV_16SC1);
    CvMat* dy = cvCreateMat(mat->height, mat->width, CV_16SC1);
    cvSobel(mat, dx, 1, 0, 3);
    cvSobel(mat, dy, 0, 1, 3);
    int check= adaptive_find_threshold(dx, dy, low, high);

    if(check == -1)
    {
        printf("Error:can not get adaptive find threshold!/n");
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

int videoShotCut::adaptive_find_threshold(CvMat* dx, CvMat* dy,double *low, double *high)
{
    CvSize size;
    IplImage * image = 0;
    int i=0,j=0;
    CvHistogram *hist;
    int hist_size = 255;
    float range_0[]={0, 256};
    float * ranges[]={range_0};
    double PercentOfPixelsNotEdges = 0.7;
    size = cvGetSize(dx);
    image = cvCreateImage(size, IPL_DEPTH_32F, 1);

    float maxv =0;
    for(i=0; i<size.height; i++)
    {
        const short * _dx = (short* )(dx->data.ptr + dx->step * i);
        const short * _dy = (short* )(dy->data.ptr + dx->step * i);
        float* _image = (float *)(image->imageData+ image->widthStep*i);
        for(j=0;j<size.width;j++)
        {
            _image[j]=(float)(abs(_dx[j]) + abs(_dy[j]));
            maxv = maxv < _image[j] ? _image[j]:maxv;
        }
    }

    if(maxv == 0)
    {
        *low = 70;
        *high = 80;
        cvReleaseImage(&image);
        return 1;
    }

    range_0[1]=maxv;
    hist_size = (int)(hist_size>maxv ? maxv:hist_size);

    hist = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
    cvCalcHist(&image, hist, 0, NULL);
    int total = (int )(size.height * size.width * PercentOfPixelsNotEdges);
    float sum =0;
    int icount = hist->mat.dim[0].size;

    float *h = (float *)cvPtr1D(hist->bins, 0);
    for(i=0;i<icount;i++)
    {
        sum +=h[i];
        if(sum > total)
        {
            break;
        }
    }

    *high = (i+1) * maxv / hist_size;
    *low = *high *0.4;
    cvReleaseImage(&image);
    cvReleaseHist(&hist);
    return 1;
}

double videoShotCut::calculate_average_v(IplImage *srcFrame)
{
    if(srcFrame)
    {
        IplImage* h_plane = cvCreateImage(cvGetSize(srcFrame), 8,1);
        IplImage* s_plane = cvCreateImage(cvGetSize(srcFrame), 8,1);
        IplImage* v_plane = cvCreateImage(cvGetSize(srcFrame), 8,1);

        IplImage* hsv=cvCreateImage(cvGetSize(srcFrame), 8, 3);

        cvCvtColor(srcFrame, hsv, CV_BGR2HSV);
        cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane,0);

        CvScalar t;
        t=cvAvg(v_plane);

        cvReleaseImage(&h_plane);
        cvReleaseImage(&s_plane);
        cvReleaseImage(&v_plane);
        cvReleaseImage(&hsv);
        return t.val[0];
    }
    else
    {
        return NULL;
    }
}

double videoShotCut::get_ECR(IplImage *canny1, IplImage *canny2)
{
    int height = canny1->height;
    int width = canny1->width;
    char * edgmap1 = canny1->imageData;
    int sum1=0, sum2=0;
    char* edgmap2 = canny2->imageData;
    int factor = INIT_WINDOW;
    double x_in =0, x_out = 0;
    int m=0, n=0;
    int i=0,j=0;

    for(i=factor/2; i<height-factor/2; ++i)
    {
        for(j=factor/2;j<width-factor/2;++j)
        {
            int tindex = i*width-1*j;
            if(edgmap1[tindex]!=0)
            {
                bool flag=false;
                ++sum1;
                for(m=-factor/2;m<factor/2;++m)
                {
                    for(n=-factor/2;n<factor/2;++n)
                    {
                        if(edgmap2[tindex + m*width +n]!=0)
                        {
                            flag = true;
                            break;
                        }

                    }
                }
                if(flag == false)
                {
                    ++x_out;
                }
            }

            if(edgmap2[tindex]!=0)
            {
                ++sum2;
                bool flag = false;
                for(m=-factor/2;m<factor/2; ++m)
                {
                    for(n=-factor/2;n<factor/2;++n)
                    {
                        if(edgmap1[tindex + m*width + n]!=0)
                        {
                            flag = true;
                            break;
                        }
                    }
                }
                if(flag==false)
                {
                    ++x_in;
                }
            }
        }
    }

    x_in = x_in/(double )(sum2+ 1);
    x_out= x_out/(double )(sum1+1);

    return x_in>x_out?x_in:x_out;
}

CvHistogram * videoShotCut::Calculate_Histogram_HV(IplImage * src)
{
    if(src)
    {
        IplImage* h_plane = cvCreateImage(cvGetSize(src),8,1);
        IplImage* s_plane = cvCreateImage(cvGetSize(src),8,1);
        IplImage* v_plane = cvCreateImage(cvGetSize(src),8,1);

        IplImage* planes[]={h_plane,v_plane};
        IplImage* hsv = cvCreateImage(cvGetSize(src),8,3);

        int h_bins= 32, s_bins= 4, v_bins= 8;

        int hist_size[]= {h_bins, v_bins};
        float h_ranges[]= {0,180};
        float v_ranges[]= {0,255};
        float * ranges[]={h_ranges,v_ranges};

        CvHistogram * hist =0;

        float max_value =0;

        cvCvtColor(src,hsv,CV_BGR2HSV);
        cvCvtPixToPlane(hsv, h_plane, s_plane, v_plane, 0);
        hist=cvCreateHist(2, hist_size, CV_HIST_ARRAY,ranges,1);
        cvCalcHist(planes, hist, 0, 0);
        cvGetMinMaxHistValue(hist, 0, &max_value, 0, 0);
        cvReleaseImage(&h_plane);
        cvReleaseImage(&s_plane);
        cvReleaseImage(&v_plane);
        cvReleaseImage(&hsv);
        return hist;
    }
    else
    {
        printf("Error:calculate histgrom fault!/n");
    }
}

float videoShotCut::compare_hist_image(IplImage *srcFrame1, IplImage *srcFrame2, int modo)
{
    CvHistogram *hist1=0,*hist2=0;
    hist1 = Calculate_Histogram_HV(srcFrame1);
    hist2 = Calculate_Histogram_HV(srcFrame2);
    float valor;
    valor = cvCompareHist(hist1, hist2, modo);
    cvReleaseHist(&hist1);
    cvReleaseHist(&hist2);

    return valor;
}

bool videoShotCut::shot_similar(IplImage *preSrc, IplImage *curSrc)
{
    IplImage* curFrame = NULL;
    IplImage* preFrame = NULL;

    IplImage* curCanny = NULL;
    IplImage* preCanny = NULL;

    double pre_v_sum = 0;
    double cur_v_sum = 0;

    double ECR = 0;

    double hist_similar = 0;

    curFrame = cvCreateImage(cvSize(curSrc->width/2,curSrc->height/2),curSrc->depth, curSrc->nChannels);
    preFrame = cvCreateImage(cvSize(curSrc->width/2,curSrc->height/2),curSrc->depth, curSrc->nChannels);

    cvResize(preSrc,preFrame);
    cvResize(curSrc,curFrame);

    preCanny = get_edge(preFrame);
    pre_v_sum = calculate_average_v(preFrame);

    curCanny = get_edge(curFrame);
    cur_v_sum = calculate_average_v(curFrame);

    ECR = get_ECR(preCanny, curCanny);

    hist_similar=compare_hist_image(curFrame, preFrame, 0);


    if( ((((1-ECR)*0.5 + hist_similar*0.5)<0.85)||((1-ECR)<0.75))&&(hist_similar<0.98))
    {
        cvReleaseImage(&curFrame);
        cvReleaseImage(&preFrame);
        return true;
    }
    else
    {
        cvReleaseImage(&curFrame);
        cvReleaseImage(&preFrame);
        return false;
    }

}




















