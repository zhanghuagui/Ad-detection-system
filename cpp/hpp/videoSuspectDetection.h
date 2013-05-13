#ifndef VIDEOSUSPECTDETECTION_H_INCLUDED
#define VIDEOSUSPECTDETECTION_H_INCLUDED


#include <highgui.h>
#include <cv.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>

#include "baseDataStructure.h"
#include "svm.h"


class videoSD
{
public:
    videoSD(const string scaleFilePath,const string modelPath);
    ~videoSD();

    void initial_scale_model(const string scaleFilePath);

    void load_FMPI_model(const string modelPath );

    void predict_FMPI_image(image_info & imageInfo);

    void svm_scale(vector<double >& feature);
    double output_scale_data(int index, double value);

    double predict_result(vector<double >&  feature);

    void output_SD_imageinfo(image_info &imageInfo);

private:
    vector<double > featureMin;
    vector<double > featureMax;
    double lower;
    double upper;
    struct svm_model * FMPIModel;
};


#endif // VIDEOSUSPECTDETECTION_H_INCLUDED
