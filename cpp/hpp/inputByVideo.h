#ifndef INPUTBYVIDEO_H_INCLUDED
#define INPUTBYVIDEO_H_INCLUDED
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libswscale/swscale.h>
}


#include "baseDataStructure.h"

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

#ifndef INT64_MIN
#define INT64_MIN   (-0x7fffffffffffffffffLL-1)
#endif

#ifndef INT64_MAX
#define INT64_MAX   INT64_C(9223372036854775807)
#endif


class videoCapture
{
public:
    videoCapture();
    ~videoCapture();

    void init();
    bool load_video(const char *filename);
    void close_video();
    bool grab_frame_from_video();

    IplImage* retrieve_frame_from_video(int);
    IplImage* query_frame_from_video();

    int64_t get_frame_dts();
    double get_frame_time();


   // void output_frame_shotlist(vector<shot_info > &shotlist);

private:

    int64_t picture_pts;
    int64_t picture_dts;
    int key_frame;
    int64_t offset;
    AVStream *video_st;

    AVFormatContext* ic;
    int  video_stream;
    AVFrame* picture;
    AVFrame   rgb_picture;
    AVPacket  packet;
    IplImage  frame;

    struct SwsContext * img_convert_ctx;
};


#endif // INPUTBYVIDEO_H_INCLUDED





















