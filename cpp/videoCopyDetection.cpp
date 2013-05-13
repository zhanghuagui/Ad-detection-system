#include "hpp/videoCopyDetection.h"

videoCD::videoCD(vector<VCDFeature > vcdfeature)
{
    vcdFeature = vcdfeature;
}

videoCD::~videoCD()
{

}

int videoCD::hsv_bin(double h, double s, double v)
{
    int hh, ss, vv;
    if(h >= 316 || h < 20)
        hh = 0;
    else if(h < 40)
        hh = 1;
        else if(h < 75)
            hh = 2;
            else if(h < 155)
                hh = 3;
                else if(h < 190)
                    hh = 4;
                    else if(h < 270)
                        hh = 5;
                        else if(h < 295)
                            hh = 6;
                            else hh = 7;

    if(s >= 0 && s <= 0.2)
        ss = 0;
    else if(s <= 0.7)
        ss = 1;
        else ss = 2;
    if(v >= 0 && v <= 0.2)
        vv = 0;
    else if(v <= 0.7)
        vv = 1;
        else vv = 2;
    int result = 9*hh + 3*ss + vv;

    return result;
}

void  videoCD::get_frame_vcdfeature(IplImage* src, int width, int height, VCDFeature & vcd_feature)
{
    int w_size = width/11;
    int h_size = height/11;

    int sizetemp = w_size*h_size;
    int a = sizetemp*9;
    int d = sizetemp*15;
    int c = sizetemp*25;

    int w_id, h_id;
    int block_size[3][3] = { {a, d, a}, {d, c, d}, {a, d, a} };
    double h, s, v;
    int hsv;

    IplImage * hsv_image=cvCreateImage(cvGetSize(src),src->depth,src->nChannels);
    IplImage * h_plane = cvCreateImage(cvGetSize(src),src->depth,1);
    IplImage * s_plane = cvCreateImage(cvGetSize(src),src->depth,1);
    IplImage * v_plane = cvCreateImage(cvGetSize(src),src->depth,1);

    cvCvtColor(src,hsv_image,CV_BGR2HSV);
    cvCvtPixToPlane(hsv_image, h_plane, s_plane, v_plane, 0);

    memset(vcd_feature.feature , 0 , 3*3*72*sizeof(double) );
    int i=0,j=0;
    for(i=0; i<h_size*11; ++i)
    {
        h_id = i/h_size;
        if(h_id <= 2) h_id = 0;
        else if(h_id <= 7) h_id = 1;
            else h_id = 2;

        for(j=0; j<w_size*11; ++j)
        {
            h = (double)h_plane->imageData[i*width+j];
            s = (double)s_plane->imageData[i*width+j+1];
            v = (double)v_plane->imageData[i*width+j+2];

            hsv = hsv_bin(h, s, v);
            w_id = j/w_size;

            if(w_id <= 2) w_id = 0;
            else if(w_id <= 7) w_id = 1;
                else w_id = 2;

            vcd_feature.feature[w_id][h_id][hsv] += 1;
        }
    }

    // get the feature of each block
    for(int i=0; i!=3; ++i)
        for(int j=0; j!=3; ++j)
                for(int k=0; k!=72; ++k)
                    vcd_feature.feature[i][j][k] /= (double)block_size[i][j];

    cvReleaseImage(&hsv_image);
    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);

}

bool  videoCD::compare_feature(VCDFeature feature_src, VCDFeature feature_dst)
{
    double dif[3][3];
    double weight[3][3] = { {0.075, 0.125, 0.075}, {0.125, 0.2, 0.125}, {0.075, 0.125, 0.075} };
    double sum = 0;
    bool is_similar = false;
    int i=0,j=0,k=0;

    for(i=0 ; i!=3 ; ++i)
    {
        for(j=0 ; j!=3 ; ++j)
        {
            dif[i][j] = 0;
            for(k=0; k!=72; ++k)
                dif[i][j] += (feature_src.feature[i][j][k] - feature_dst.feature[i][j][k])*(feature_src.feature[i][j][k] - feature_dst.feature[i][j][k]);

            dif[i][j] = sqrt(dif[i][j]);
            sum += dif[i][j]*weight[i][j];
        }
    }
    if(sum < COMPARE_THRESHOLD)
        is_similar = true;

    return is_similar;
}

void   videoCD::output_VCD_imageinfo(image_info &imageInfo)
{
        int i=0;
        get_frame_vcdfeature(imageInfo.image, imageInfo.image->width, imageInfo.image->height, imageInfo.vcdFeature);
        for(i=0;i<vcdFeature.size();i++)
        {
            if(compare_feature(imageInfo.vcdFeature,vcdFeature[i]))
            {
                imageInfo.isVCD=true;
                continue ;
            }
        }
}

























