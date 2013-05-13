#ifndef VIDEOCOPYDETECTION_H_INCLUDED
#define VIDEOCOPYDETECTION_H_INCLUDED

#include "baseDataStructure.h"
#include <vector>
#include <string>

#include <highgui.h>
#include <cv.h>

#define COMPARE_THRESHOLD 0.1

class videoCD
{
public:
    videoCD(vector<VCDFeature > vcdfeature);
    ~videoCD();
    void  get_frame_vcdfeature(IplImage* src, int width, int height, VCDFeature & vcd_feature);
    int   hsv_bin(double h, double s, double v);
    bool  compare_feature(VCDFeature feature_src, VCDFeature feature_dst);

    void  output_VCD_imageinfo(image_info &imageInfo);

private:
    vector<VCDFeature > vcdFeature;
};


#endif // VIDEOCOPYDETECTION_H_INCLUDED
