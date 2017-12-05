#include "avengine.h"
#include "base/safe_log.h"
//#include "base/time_util.h"
#include <map>



#if 0

namespace agora {
namespace ServerSDK2 {

using namespace rtc;

enum CODEC_TYPE {
    _C_INVALID = -1,
    _VC_VP8 = 0,
    _VC_H264,
    _VC_H265,
    _VC_MPEG1,

    _VC_YUV = 99,
    _AC_AAC = 100,
    _AC_OPUS,
    _AC_PCM,
};

static std::map<int, CODEC_TYPE> EngCodecMap = {
    {0, _VC_VP8},  //kVideoCodecVP8
    {1, _VC_H264}, //kVideoCodecH264
    {6, _VC_VP8},  //kVideoCodecEvp
    {7, _VC_H264}, //kVideoCodecE264
};

class AVEngine::PeerStream : public AgoraRTC::ICMFile {
//    typedef std::shared_ptr<AVData> AVDataPtr;
//    typedef std::deque<AVDataPtr> AVDataQue;
public:
    PeerStream(AVEngine *handler, uid_t uid)
    /*: disp_(AVDispatcher(uid, handler))*/ {
        handler_ = handler;
        uid_ = uid;
    }
    ~PeerStream() {
        ;
    }

    virtual int startAudioRecord() { return 0; }
    virtual int startVideoRecord() { return 0; }
    virtual int stopAudioRecord() { return 0; }
    virtual int stopVideoRecord() { return 0; }
    virtual int setVideoRotation(int rotation) { return 0; }

    virtual int onDecodeVideo(uint32_t video_ts, uint8_t payload_type,
        uint8_t *buffer, uint32_t length, AgoraRTC::VFrameInfo info) override;

    virtual int onEncodeVideo(uint32_t video_ts, uint8_t payload_type,
        uint8_t *buffer, uint32_t length, AgoraRTC::VFrameInfo info) override;

    virtual int onDecodeAudio(uint32_t audio_ts, uint8_t payload_type, uint32_t channels,
        uint8_t *buffer, uint32_t length) override;

    virtual int onEncodeAudio(uint32_t audio_ts, uint8_t payload_type, uint32_t channels,
        uint8_t *buffer, uint32_t length) override;

    virtual int onDecodeVideoSEI(const char *info, int len) override;
private:
    AVEngine *handler_;
    uid_t uid_;
    //AVDispatcher disp_;
};

int AVEngine::PeerStream::onDecodeVideo(unsigned int video_ts, unsigned char payload_type, \
    unsigned char* buffer, unsigned int length, AgoraRTC::VFrameInfo info)
{
#if 0
    (void)video_ts;
    (void)payload_type;

    assert(uid_ != 0);

    //SAFE_LOG(INFO) << "onDecodeVideo: length = " << length << " frame_type: " << info.frame_type;

    CODEC_TYPE cType;
    if((payload_type & 0xFC) == 40 || payload_type == 107 || payload_type == 127)
        cType = _VC_H264;
    else{
        cType = _VC_VP8;
    }
    AVPictureType pType;
    if(info.frame_type == 2)
        pType = AV_PICTURE_TYPE_I;
    else if(info.frame_type == 1)
        pType = AV_PICTURE_TYPE_B;
    else if(info.frame_type == 0)
        pType = AV_PICTURE_TYPE_P;
    else
        pType = AV_PICTURE_TYPE_NONE;
    disp_.DispatchVideo(cType, buffer, length, pType);
#endif
    return 0;
}


int AVEngine::PeerStream::onEncodeVideo(uint32_t video_ts, uint8_t payload_type,
    uint8_t *buffer, uint32_t length, AgoraRTC::VFrameInfo info)
{
    assert(uid_ == 0);

    return 0;
}

int AVEngine::PeerStream::onDecodeAudio(unsigned int audio_ts, unsigned char payload_type, \
    unsigned int channels, unsigned char* buffer, unsigned int length)
{
    (void)audio_ts;
    (void)payload_type;

    assert(uid_ != 0);

    //SAFE_LOG(INFO) << "onDecodeAudio: length = " << length << " channels: " << channels;
    //disp_.DispatchAudio(_AC_PCM, channels, buffer, length);

    return 0;
}


//Mix audio output
int AVEngine::PeerStream::onEncodeAudio(uint32_t audio_ts, uint8_t payload_type, uint32_t channels,
    uint8_t *buffer, uint32_t length)
{
    assert(uid_ == 0);

    return 0;
}

int AVEngine::PeerStream::onDecodeVideoSEI(const char *info, int len)
{
    return 0;
}

AVEngine::AVEngine(rtc::IRtcEngineEventHandlerEx *externHandler) /*: AVIO(0),*/:applite_(nullptr)
{
    externHandler_ = externHandler;
#if 0
    swr_ = NULL;
    fifo_ = NULL;

    audio_frame_ = NULL;
    fifo_frame_ = NULL;
    sws_ = NULL;
    video_frame_ = NULL;
#endif
    channels_ = 0;
    last_pts_ = 0;


    width_ = 0;
    height_ = 0;
    //fps_ = 0;
    //now_ts_ = base::tick_ms();
    ts_valid_ = false;
}

AVEngine::~AVEngine()
{
    SAFE_LOG(INFO) << "Leaving channel and ready to cleanup";

    if (applite_) {
        applite_->leaveChannel();
        applite_->release(true);
        applite_ = nullptr;
    }
#if 0
    Push(NULL);

    if(swr_) {
        swr_free(&swr_);
        swr_ = NULL;
    }
    if(fifo_) {
        av_audio_fifo_free(fifo_);
        fifo_ = NULL;
    }
    if(audio_frame_) {
        av_frame_free(&audio_frame_);
        audio_frame_ = NULL;
    }
    if(fifo_frame_) {
        av_frame_free(&fifo_frame_);
        fifo_frame_ = NULL;
    }
    while(!audioQue_.empty())
        audioQue_.pop_front();

    if(sws_) {
        sws_freeContext(sws_);
    }
    if(video_frame_) {
        av_free(video_frame_->data[0]);
        av_frame_free(&video_frame_);
        video_frame_ = NULL;
    }
    while(!videoQue_.empty())
        videoQue_.pop_front();
#endif
}

int AVEngine::Init(AVEngineConfig &conf)
{
    //Create Engine Runtime
    applite_ = dynamic_cast<rtc::IRtcEngineEx*>(createAgoraRtcEngine());
    if (applite_ == NULL) {
        SAFE_LOG(FATAL) << "Failed to create an Agora Rtc Engine!";
        return -1;
    }

    const char *rtc_version = applite_->getSourceVersion();
    const char *eng_version = getChatEngineSourceVersion();
    LOG(INFO, "SDK version: %s; chat engine: %s", rtc_version, eng_version);

    RegisterICMFileObserver(this);
    registerVideoSource(this);

    rtc::RtcEngineContextEx context;
    context.eventHandler = this;
    context.isExHandler = true;
    context.appId = conf.appid.c_str();
    context.dataDir = conf.logPath;
    context.context = NULL;

    applite_->initializeEx(context);
    applite_->setLogCallback(true);
    applite_->setClientRole(conf.role, NULL);
    if(conf.role == rtc::CLIENT_ROLE_BROADCASTER) {
        applite_->setParameters("{\"rtc.client_type\": 2}"); //pstn
    }else if(conf.role == rtc::CLIENT_ROLE_AUDIENCE) {
        applite_->setParameters("{\"rtc.client_type\": 3}"); //mute some heart beat packets
    }
    applite_->setChannelProfile(conf.profile);
    applite_->enableVideo();
    applite_->enableAudio();

    rtc::RtcEngineParameters parameter(*applite_);

    agora::rtc::AParameter msp(*applite_);
    msp->setBool("che.audio.external_device", true);
    msp->setInt("che.audio.audioChannels", 1);
    msp->setBool("che.audio.server_mode", true);
    msp->setBool("che.audio.external.to.apm", false);
    if(conf.role == rtc::CLIENT_ROLE_AUDIENCE) {
        parameter.muteLocalAudioStream(true);
        parameter.muteLocalVideoStream(true);
        msp->setBool("che.video.server_mode", false);
    } else if (conf.role == rtc::CLIENT_ROLE_BROADCASTER){
        msp->setBool("che.video.server_mode", true);
        msp->setBool("che.video.recording", true); //do not decode in engine
        msp->setInt("che.video.maxVideoFrameRate", 60);
        msp->setInt("che.video.h264Profile", 100); //high
        msp->setInt("che.video.startVideoBitRate", 100);
        msp->setBool("rtc.video.prefer_frame_rate", true);
        msp->setBool("che.video.prefer_frame_rate", true);
        registerAudioFrameObserver(this);
    }
    //msp->setInt("che.video.local.camera_index", 1024);
    msp->setInt("che.video.local.camera_index", 1025);

    msp->setInt("rtc.connection_lost_period", 30000);
    msp->setBool("rtc.transport_quality_indication", true);
    msp->setUInt("rtc.listener_min_jitter", 3000);

    parameter.enableDualStreamMode(conf.dual);
    std::string logFile = conf.logPath + "mediasdk.log";
    parameter.setLogFile(logFile.c_str());
    //set avio log
    //LogDir = conf.logPath;

    if (applite_->joinChannel(conf.vendor_key.c_str(), conf.channel.c_str(), NULL, 0) < 0) {
        SAFE_LOG(ERROR) << "Failed to create the channel " << conf.channel;
        return -2;
    }
    //must be set after joinChannel, or don't work
    msp->setInt("che.audio.audioSampleRate", 48000);
    return 0;
}

void AVEngine::onJoinChannelSuccess(const char *channel, uid_t uid, int elapsed)
{
    SAFE_LOG(INFO) << uid << " logined successfully in " << channel
    << ", elapsed: " << elapsed << " ms";

    if(externHandler_)
        externHandler_->onJoinChannelSuccess(channel, uid, elapsed);
}

void AVEngine::onRejoinChannelSuccess(const char *channel, uid_t uid, int elapsed) {
    SAFE_LOG(INFO) << uid << " rejoin to channel: " << channel << ", time offset "
    << elapsed << " ms";
}

void AVEngine::onWarning(int warn, const char *msg) {
    SAFE_LOG(WARN) << "code: " << warn << ", " << msg;

    if(externHandler_)
        externHandler_->onWarning(warn, msg);
}

void AVEngine::onError(int rescode, const char *msg) {
    switch (rescode) {
      default:
        LOG(INFO, "Error in mediasdk: %d, %s", rescode, msg);
        break;
    }

    if(externHandler_)
        externHandler_->onError(rescode, msg);
}

void AVEngine::onUserJoined(uid_t uid, int elapsed) {
    SAFE_LOG(INFO) << "offset " << elapsed << " ms: " << uid
    << " joined the channel";

    if(externHandler_)
        externHandler_->onUserJoined(uid, elapsed);
}

void AVEngine::onUserOffline(uid_t uid, rtc::USER_OFFLINE_REASON_TYPE reason) {
    const char *detail = reason == USER_OFFLINE_QUIT ? "quit" : "dropped";
    SAFE_LOG(INFO) << "User " << uid  << detail;

    if(externHandler_)
        externHandler_->onUserOffline(uid, reason);
}

void AVEngine::onConnectionLost()
{
    SAFE_LOG(WARN) << "onConnectionLost, aborting.";

    if(externHandler_)
        externHandler_->onConnectionLost();
}

bool AVEngine::onCustomizedSei(const void **content, int *length)
{
    SAFE_LOG(INFO) << "onCustomizedSei";

    if(externHandler_)
        return externHandler_->onCustomizedSei(content, length);

    return false;
}

AgoraRTC::ICMFile* AVEngine::GetICMFileObject(unsigned int uid)
{
    std::lock_guard<std::mutex> auto_lock(peer_lock_);
    auto it = peers_.find(uid);
    if (it == peers_.end()) {
      it = peers_.insert(std::make_pair(uid, PeerStreamPtr(new PeerStream(this, uid)))).first;
    }

    return it->second.get();
}

int AVEngine::InsertRawAudioPacket(unsigned int uid,
    const unsigned char *payload, unsigned short size, int type,
    unsigned int ts, unsigned short seq)
{
    return 0;
}

bool AVEngine::onRecordAudioFrame(AudioFrame& audioFrame)
{
#if 0
    if(!ts_valid_)
        now_ts_ = base::tick_ms();
    else
        now_ts_ += 10;

    //printf("now ms = %ld\n", base::tick_ms());

    //push video
    for(;;) {
        VideoFramePtr pVideoFrame;
        std::lock_guard<std::mutex> auto_lock(videoQueLock_);
        if(videoQue_.empty()) {
            break;
        }

        pVideoFrame = videoQue_.front();
        if(now_ts_ >= pVideoFrame->pts) {
            mFuncOutputYuvEx(mReceiver, (void *)pVideoFrame->data.data(), pVideoFrame->width, pVideoFrame->height, \
                0, 0, 0, 0, 0, pVideoFrame->pts, 1);
            videoQue_.pop_front();
        } else {
            break;
        }
    }

    //push audio
    for(;;) {
        AudioFramePtr pAudioFrame;
        std::lock_guard<std::mutex> auto_lock(audioQueLock_);
        if(audioQue_.empty()) {
            break;
        }

        pAudioFrame = audioQue_.front();
        if(now_ts_ >= pAudioFrame->pts) {
            memcpy(audioFrame.buffer, pAudioFrame->data.data(), pAudioFrame->data.size());
            audioQue_.pop_front();
        } else {
            break;
        }
    }
#endif
    return true;
}

bool AVEngine::onPlaybackAudioFrame(AudioFrame& audioFrame)
{
    (void)audioFrame;
    return true;
}

bool AVEngine::onMixedAudioFrame(AudioFrame& audioFrame)
{
    (void)audioFrame;
    return true;
}

bool AVEngine::onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame)
{
    (void)uid;
    (void)audioFrame;
    return true;
}

void AVEngine::setVideoProfile(int width, int height, int fps, int kbps, int gop)
{
    rtc::RtcEngineParameters parameter(*applite_);
    parameter.muteLocalVideoStream(true);
    applite_->setVideoProfileEx(width, height, fps, kbps);
    {
        std::lock_guard<std::mutex> auto_lock(videoQueLock_);
        while(!videoQue_.empty())
            videoQue_.pop_front();
    }
    parameter.muteLocalVideoStream(false);
}

void AVEngine::setAudioProfile(int sampleRate, int channels \
/*rtc::AUDIO_PROFILE_TYPE profile, rtc::AUDIO_SCENARIO_TYPE scenario*/)
{
    /*
    if(sampleRate != 8000 && sampleRate != 16000 && sampleRate != 32000 \
      && sampleRate != 48000) {
        SAFE_LOG(ERROR) << "Not Support sampleRate: " << sampleRate;
        return;
    }
    */
    rtc::RtcEngineParameters parameter(*applite_);
    parameter.muteLocalAudioStream(true);
    parameter.setRecordingAudioFrameParameters(sampleRate, channels, \
        RAW_AUDIO_FRAME_OP_MODE_WRITE_ONLY, channels * sampleRate/100);
    parameter.setPlaybackAudioFrameParameters(sampleRate, channels, \
        RAW_AUDIO_FRAME_OP_MODE_READ_ONLY, channels * sampleRate/100);
    //applite_->setAudioProfile(profile, scenario);
    //clean all buffer frame

    {
        std::lock_guard<std::mutex> auto_lock(audioQueLock_);
        while(!audioQue_.empty())
            audioQue_.pop_front();
    }
    parameter.muteLocalAudioStream(false);
}

inline void AVEngine::_validTime(int64_t pts)
{
#if 0
    if(!ts_valid_) {
        ts_valid_ = true;
        now_ts_ = pts - (base::tick_ms() - now_ts_);
    }
#endif
}
#if 0
void AVEngine::pushAVData(AVData *data)
{
    _validTime(data->frame->pts);
    if(data->codecType == _VC_YUV) {
        if(width_ != data->frame->width || height_ != data->frame->height) {
            width_  = data->frame->width;
            height_ = data->frame->height;

            //if(data->frame->format != AV_PIX_FMT_YUV420P || data->frame->linesize[0] != width_ * height_) {
                video_frame_ = av_frame_alloc();
                video_frame_->width = width_;//640;
                video_frame_->height = height_;//360;
                av_image_alloc(video_frame_->data, video_frame_->linesize, video_frame_->width, \
                    video_frame_->height, AV_PIX_FMT_YUV420P, 1);

                sws_ = sws_getContext(width_, height_, (AVPixelFormat)data->frame->format, \
                    video_frame_->width, video_frame_->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
            //}
            setVideoProfile(video_frame_->width, video_frame_->height, 15, 1500, 0);
        }
        AVFrame *in_frame = data->frame;
        if(sws_) {
            sws_scale(sws_, (const uint8_t * const *)data->frame->data, data->frame->linesize,
                0, data->frame->height, video_frame_->data, video_frame_->linesize);
            in_frame = video_frame_;
        }
        in_frame->pts = data->frame->pts;

        VideoFramePtr pVideoFrame(new _VideoFrame);
        pVideoFrame->width = in_frame->width;
        pVideoFrame->height = in_frame->height;
        pVideoFrame->pts = in_frame->pts;
        pVideoFrame->data.assign((const char*)in_frame->data[0], (size_t)(in_frame->width * in_frame->height * 3 / 2));
        {
            std::lock_guard<std::mutex> auto_lock(videoQueLock_);
            videoQue_.push_back(pVideoFrame);
        }
    } else if(data->codecType == _AC_PCM) {
        if(channels_ != data->frame->channels) {
            if(data->frame->channels <= 2)
                channels_ = data->frame->channels;
            else
                channels_ = 2;

            if(data->frame->sample_rate != AUDIO_SAMPLE_RATE || data->frame->format != AV_SAMPLE_FMT_S16) {
                //ServerSDK2_LOG(INFO) << "sid: " << data->sid << " audio format is different from engine support format, use resampler.";
                swr_ = swr_alloc();
                av_opt_set_int(swr_, "in_channel_layout",  data->frame->channel_layout, 0);
                av_opt_set_int(swr_, "out_channel_layout", (channels_ == 1)?AV_CH_LAYOUT_MONO:AV_CH_LAYOUT_STEREO,  0);
                av_opt_set_int(swr_, "in_sample_rate",     data->frame->sample_rate, 0);
                av_opt_set_int(swr_, "out_sample_rate",    AUDIO_SAMPLE_RATE, 0);
                av_opt_set_sample_fmt(swr_, "in_sample_fmt",  (AVSampleFormat)data->frame->format, 0);
                av_opt_set_sample_fmt(swr_, "out_sample_fmt", AV_SAMPLE_FMT_S16,  0);
                swr_init(swr_);

                audio_frame_ = av_frame_alloc();
                audio_frame_->format = AV_SAMPLE_FMT_S16;
                audio_frame_->channel_layout = (channels_ == 1)?AV_CH_LAYOUT_MONO:AV_CH_LAYOUT_STEREO;
                audio_frame_->sample_rate = AUDIO_SAMPLE_RATE;
                audio_frame_->nb_samples = av_rescale_rnd(swr_get_delay(swr_, data->frame->sample_rate) + \
                    data->frame->nb_samples, AUDIO_SAMPLE_RATE, data->frame->sample_rate, AV_ROUND_UP);
                av_frame_get_buffer(audio_frame_, 0);
            }
            if((swr_ && audio_frame_->nb_samples != AUDIO_NB_SAMPLES) || data->frame->nb_samples != AUDIO_NB_SAMPLES) {
                fifo_ = av_audio_fifo_alloc(AV_SAMPLE_FMT_S16, channels_, 1);

                fifo_frame_ = av_frame_alloc();
                fifo_frame_->format = AV_SAMPLE_FMT_S16;
                fifo_frame_->channel_layout = (channels_ == 1)?AV_CH_LAYOUT_MONO:AV_CH_LAYOUT_STEREO;
                fifo_frame_->sample_rate = AUDIO_SAMPLE_RATE;
                fifo_frame_->nb_samples = AUDIO_NB_SAMPLES;
                av_frame_get_buffer(fifo_frame_, 0);

                //if(swr_)
                //    duration_ = audio_frame_->nb_samples * 1000 / audio_frame_->sample_rate;
                //else
                    duration_ = data->frame->nb_samples * 1000 / data->frame->sample_rate;
            }
            setAudioProfile(AUDIO_SAMPLE_RATE, channels_);
        }

        //proc
        AVFrame *in_frame = data->frame;
        if(swr_) {
            /* convert to destination format */
            int ret = swr_convert(swr_, audio_frame_->data, audio_frame_->nb_samples,
                (const uint8_t **)data->frame->data, data->frame->nb_samples);
            if(ret < 0)
                return;
            in_frame = audio_frame_;
        }
        if(fifo_) {
            av_audio_fifo_realloc(fifo_, av_audio_fifo_size(fifo_) + in_frame->nb_samples);
            av_audio_fifo_write(fifo_, (void **)in_frame->data, in_frame->nb_samples);
            if(last_pts_ == 0)
                last_pts_ = data->frame->pts;

            while(av_audio_fifo_size(fifo_) >= AUDIO_NB_SAMPLES) {
                av_audio_fifo_read(fifo_, (void **)fifo_frame_->data, fifo_frame_->nb_samples);

                AudioFramePtr pAudioFrame(new _AudioFrame);
                pAudioFrame->sample_rate = AUDIO_SAMPLE_RATE;
                pAudioFrame->channels = channels_;
                pAudioFrame->nb_samples = AUDIO_NB_SAMPLES;
                pAudioFrame->pts = last_pts_;
                pAudioFrame->data.assign((const char*)fifo_frame_->data[0], (size_t)(AUDIO_NB_SAMPLES * channels_ * 2));
                {
                    std::lock_guard<std::mutex> auto_lock(audioQueLock_);
                    audioQue_.push_back(pAudioFrame);
                }
                /* caculate next frame pts */
                last_pts_ += duration_ * AUDIO_NB_SAMPLES/data->frame->nb_samples;
            }

        } else {
            AudioFramePtr pAudioFrame(new _AudioFrame);
            pAudioFrame->sample_rate = AUDIO_SAMPLE_RATE;
            pAudioFrame->channels = channels_;
            pAudioFrame->nb_samples = AUDIO_NB_SAMPLES;
            pAudioFrame->pts = data->frame->pts;
            pAudioFrame->data.assign((const char*)in_frame->data[0], (size_t)(AUDIO_NB_SAMPLES * channels_ * 2));
            {
                std::lock_guard<std::mutex> auto_lock(audioQueLock_);
                audioQue_.push_back(pAudioFrame);
            }
        }
    }
}

#endif

/*
this buffer must be sure s16 pcm format, and contain 10ms samples
*/
void AVEngine::pushPCMFrame(void *buffer, int sampleRate, int channels, int nbSamples, int64_t pts)
{
    if(sampleRate/100 != nbSamples){
        return;
    }
    _validTime(pts);

    AudioFramePtr pAudioFrame(new _AudioFrame);
    pAudioFrame->sample_rate = sampleRate;
    pAudioFrame->channels = channels;
    pAudioFrame->nb_samples = nbSamples;
    pAudioFrame->pts = pts;
    pAudioFrame->data.assign((const char*)buffer, (size_t)(nbSamples * channels * 2));
    {
        std::lock_guard<std::mutex> auto_lock(audioQueLock_);
        audioQue_.push_back(pAudioFrame);
    }
}

void AVEngine::pushYUVFrame(void *buffer, int width, int height, int64_t pts)
{
    if(!buffer) {
        SAFE_LOG(INFO) << "NULL YUVFrame";
        return;
    }
    _validTime(pts);

    VideoFramePtr pVideoFrame(new _VideoFrame);
    pVideoFrame->width = width;
    pVideoFrame->height = height;
    pVideoFrame->pts = pts;
    pVideoFrame->data.assign((const char*)buffer, (size_t)(width * height * 3 / 2));
    {
        std::lock_guard<std::mutex> auto_lock(videoQueLock_);
        videoQue_.push_back(pVideoFrame);
    }
}

//PROC_RET AVEngine::Proc(StreamInfo *info, AVData *inData, AVData *outData)
//{
//    //stream 0 push to channel
//    if(inData->sid == 0) {
//        pushAVData(inData);
//        return _ERR_FAIL;
//    }

//    //or dispatch out
//    outData->Ref(inData);
//    return _ERR_SUCCESS;
//}

}}

#endif
