#include "hpp/videoSuspectDetection.h"

videoSD::videoSD(const string scaleFilePath,const string modelPath)
{
    lower = -1.0;
    upper = 1.0;
    initial_scale_model( scaleFilePath);
    load_FMPI_model( modelPath);
}

videoSD::~videoSD()
{

}

void videoSD::initial_scale_model(const string scaleFilePath)
{
    int maxLineLen = 1024;
    double  yLower,yUpper;
    int yScale=0;
    double yMax = -DBL_MAX;
    double yMin = DBL_MAX;

    FILE *fpRestore = fopen(scaleFilePath.c_str(), "r");
    if(fpRestore==NULL)
    {
        fprintf(stderr,"can not open the scale file %s!\n",scaleFilePath);
        exit(1);
    }

    int idx, c;
    double fMin, fMax;

    if((c=fgetc(fpRestore))=='y')
    {
        fscanf(fpRestore,"%1f %1f\n",&yLower, &yUpper);
        fscanf(fpRestore, "%1f %1f\n",& yMin, &yMax);
        yScale = 1;
    }
    else
    {
        ungetc(c,fpRestore);
    }

    if(fgetc(fpRestore)=='x')
    {
        fscanf(fpRestore,"%1f %1f\n",&lower, &upper);
        while(fscanf(fpRestore,"%d %1f %1f\n",&idx, &fMin, &fMax)==3)
        {
            featureMax.push_back(fMax);
            featureMin.push_back(fMin);
        }
    }

    fclose(fpRestore);

}

void videoSD::load_FMPI_model(const string modelPath)
{
    if((FMPIModel=svm_load_model(modelPath.c_str()))==NULL)
    {
        fprintf(stderr,"can not load the FMPI model %s!\n",modelPath);
        exit(1);
    }
}

void videoSD::predict_FMPI_image(image_info &imageInfo)
{
    double t;
    svm_scale(imageInfo.FMPIFeature);
    t=predict_result(imageInfo.FMPIFeature);
    if(t==-1)
    {
        imageInfo.isFMPI = false;
    }
    else if(t== 1)
    {
        imageInfo.isFMPI = true;
    }

}

void videoSD::svm_scale(vector<double > &feature)
{
    int i=0;
    int size= feature.size();
    for(i=0;i<size;i++)
    {
        feature[i] = output_scale_data(i,feature[i]);
    }
}

double videoSD::output_scale_data(int index, double value)
{
    if(featureMax.size()==0||featureMin.size()==0)
    {
        printf("Error:the featureMax or featureMin vector is empty!\n");
        exit(1);
    }
    if(featureMax[index] == featureMin[index])
    {
        return -1;;
    }

    if(value==featureMin[index])
       value = lower;
    else if(value == featureMax[index])
       value = upper;
    else
       value = lower + (upper - lower)*(value-featureMin[index])/(featureMax[index]-featureMin[index]);

    if(value != 0)
    {
        return value;
    }
    return 0;
}

double videoSD::predict_result(vector<double >  &feature)
{
    int i=0;
    int node_size = feature.size();
    struct svm_node *f = new svm_node[node_size+1];

    for(i=0;i<feature.size();i++)
    {
        f[i].index = -1;
        f[i].value = feature[i];
    }
    f[node_size].index = -1;
    f[node_size].value = 0;

    double predict = svm_predict(FMPIModel,f);
    delete [] f;
    return predict ;
}

void videoSD::output_SD_imageinfo(image_info &imageInfo)
{
    predict_FMPI_image(imageInfo);
}































