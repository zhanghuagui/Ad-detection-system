#include "hpp/inputByVideo.h"


videoCapture::videoCapture()
{
    init();
}

videoCapture::~videoCapture()
{
    close_video();
}

void videoCapture::init()
{
    ic =0;
    video_stream = -1;
    video_st = 0;
    key_frame = 0;
    picture=0;
    picture_pts = 0;
    picture_dts = 0;
    memset(&rgb_picture, 0, sizeof(rgb_picture));
    memset(&frame, 0, sizeof(frame));
    packet.data = NULL;
    img_convert_ctx =0;
    offset =0;
}

void videoCapture::close_video()
{
    if(picture)
    {
        av_free(picture);
    }

    sws_freeContext(img_convert_ctx);

    if(video_st)
    {
        avcodec_close(video_st->codec);
        video_st = NULL;
    }

    if(ic)
    {
        av_close_input_file(ic);
        ic = NULL;
    }

    if(rgb_picture.data[0])
    {
        cvFree(&rgb_picture.data[0]);
    }

    if(packet.data)
    {
        av_free_packet(&packet);
    }

    init();
}

bool videoCapture::load_video(const char * filename)
{
    unsigned i=0;
    bool valid = false;
    close_video();

    int err=av_open_input_file(&ic, filename, NULL,0, NULL);

    if(err<0)
    {
        printf("ERROR:can not open the video file!\n");
        goto exit_func;
    }

    err = av_find_stream_info(ic);

    if(err<0)
    {
        printf("ERROR:could not find codec parameters!\n");
        goto exit_func;
    }

    for(i=0; i < ic->nb_streams; i++)
    {
        AVCodecContext *enc = ic->streams[i]->codec;

        if(CODEC_TYPE_VIDEO == enc->codec_type && video_stream < 0)
        {
            AVCodec * codec = avcodec_find_decoder(enc->codec_id);
            if(!codec || avcodec_open(enc, codec)<0)
            {
                goto exit_func;
            }

            video_stream = i;
            video_st = ic->streams[i];
            picture = avcodec_alloc_frame();
            rgb_picture.data[0] = (uint8_t *)cvAlloc(avpicture_get_size(PIX_FMT_BGR24,enc->width, enc->height));
            avpicture_fill((AVPicture*)&rgb_picture, rgb_picture.data[0],PIX_FMT_BGR24, enc->width, enc->height);
            cvInitImageHeader(&frame, cvSize(enc->width,enc->height), 8,3,0,4);
            cvSetData(&frame, rgb_picture.data[0],rgb_picture.linesize[0]);
            break;
        }
    }

    img_convert_ctx = sws_getContext(video_st->codec->width,
                                     video_st->codec->height,
                                     video_st->codec->pix_fmt,
                                     video_st->codec->width,
                                     video_st->codec->height,
                                     PIX_FMT_BGR24,
                                     SWS_BICUBIC,
                                     NULL,NULL,NULL);
    if(video_stream >=0) valid = true;

exit_func :
    if(!valid)
    {
        close_video();
    }
   return  valid;
}

bool videoCapture::grab_frame_from_video()
{
    bool valid = false;
    static bool bFirstTime = true;
    int got_picture;

    if(bFirstTime)
    {
        bFirstTime = false;
        packet.data = NULL;
    }

    if(!ic || !video_st)
    {
        return false;
    }

    if(packet.data !=NULL)
    {
        av_free_packet(&packet);
    }



    while(!valid && (av_read_frame(ic,&packet)>=0))
    {
        if(packet.stream_index !=video_stream)
        {
            av_free_packet(&packet);
            continue;
        }

        avcodec_decode_video(video_st->codec,
                             picture,&got_picture,
                             packet.data,packet.size);

        if(got_picture)
        {
            if(picture->key_frame==1)
            {
                key_frame=1;
            }
            else
            {
                key_frame=0;
            }

            offset = packet.pos;
            picture_pts = packet.pts;
            picture_dts = packet.dts;
            valid = true;
        }
    }

  /*   if(valid)
    {
        printf("valid is true!\n");
    }
    else
    {
        printf("valid is false!\n");
    }*/

    return valid;
}

IplImage* videoCapture::retrieve_frame_from_video(int)
{
    if(!video_st||!picture->data[0])
    {
        return 0;
    }

    sws_scale(img_convert_ctx, picture->data,
              picture->linesize,0,
              video_st->codec->height,
              rgb_picture.data,rgb_picture.linesize);
    return &frame;
}

IplImage* videoCapture::query_frame_from_video()
{
  /*  if(grab_frame_from_video())
    {
        printf("grab frame from video successful!\n");
    }
    else
    {
        printf("grab frame from video fail!\n");
    }*/
    return grab_frame_from_video()? retrieve_frame_from_video(0) : 0;
}

int64_t videoCapture::get_frame_dts()
{
    return picture_dts;
}

double videoCapture::get_frame_time()
{
    return 0;
}

/*void videoCapture::output_frame_shotlist(vector<shot_info >& shotlist)
{
    if(!load_video())
        {
            printf("load video error!\n");
            return ;
        }
        IplImage * curFrame=NULL;
        cvNamedWindow("testWindow",CV_WINDOW_AUTOSIZE);
        while(1)
        {
            curFrame=query_frame_from_video();
            if(!curFrame)
            {
                break;
            }
            cvShowImage("testWindow",curFrame);
        }
        cvReleaseImage(&curFrame);
        cvDestroyWindow("testWindow");
}*/






















