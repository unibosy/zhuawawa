#ifndef AVENGINE_H
#define AVENGINE_H

#include "internal.h"
#include <string>
#include <utility>
#include <unordered_map>
#include <memory>
#include <deque>
#include <mutex>
//#include "AVDispatcher.h"
using namespace std;




#if 0

namespace agora {
namespace ServerSDK2 {

struct AVEngineConfig {
    std::string appid;
    std::string vendor_key;
    std::string channel; //channel name
    std::string logPath;
    bool dual;
    rtc::CLIENT_ROLE_TYPE role;
    rtc::CHANNEL_PROFILE_TYPE profile;
};

class AVEngine :
    public rtc::IRtcEngineEventHandlerEx,
    public AgoraRTC::ICMFileObserver,
    public media::IAudioFrameObserver,
    public VideoSource
//    public AVIO
{
#define AUDIO_SAMPLE_RATE 48000
#define AUDIO_NB_SAMPLES 480
    class PeerStream;
    friend class PeerStream;

    class _VideoFrame {
    public:
        int width;
        int height;
        int64_t pts;
        std::string data;
    };
    class _AudioFrame {
    public:
        int sample_rate;
        int channels;
        int nb_samples;
        int64_t pts;
        std::string data;
    };

    typedef std::unique_ptr<PeerStream> PeerStreamPtr;
    typedef std::shared_ptr<_VideoFrame> VideoFramePtr;
    typedef std::shared_ptr<_AudioFrame> AudioFramePtr;
    typedef std::deque<VideoFramePtr> VideoFrameQue;
    typedef std::deque<AudioFramePtr> AudioFrameQue;
public:
    AVEngine(rtc::IRtcEngineEventHandlerEx *externHandler);
    ~AVEngine();

    // inherited from IRtcEngineEventHandlerEx
    virtual void onJoinChannelSuccess(const char *channel, uid_t uid, int elapsed) override;
    virtual void onRejoinChannelSuccess(const char *channel, uid_t uid, int elapsed) override;
    virtual void onWarning(int warn, const char *msg) override;
    virtual void onError(int err, const char *msg) override;
    virtual void onUserJoined(uid_t uid, int elapsed) override;
    virtual void onUserOffline(uid_t uid, rtc::USER_OFFLINE_REASON_TYPE reason) override;
    //virtual void onRtcStats(const rtc::RtcStats &stats) override;
    //virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed) override;
    virtual void onConnectionLost() override;
    //virtual void onUserMuteVideo(uid_t uid, bool muted) override;
    //virtual void onLogEvent(int level, const char *msg, int length) override;
    virtual bool onCustomizedSei(const void **content, int *length) override;
    //virtual void onAudioTransportQuality(uid_t uid, unsigned int kbps,
    //    unsigned short delay, unsigned short lost) override;
    //virtual void onVideoTransportQuality(uid_t uid, unsigned int kbps,
    //    unsigned short delay, unsigned short lost) override;
    // Inherited from ICMFileObserver
    virtual AgoraRTC::ICMFile* GetICMFileObject(unsigned int uid) override;
    virtual int InsertRawAudioPacket(unsigned int uid,
        const unsigned char *payload, unsigned short size, int type,
        unsigned int ts, unsigned short seq) override;
    /*
    // Inherited from IVideoFrameObserver
    virtual bool onCaptureVideoFrame(VideoFrame& videoFrame) override;
    virtual bool onRenderVideoFrame(unsigned int uid, VideoFrame& videoFrame) override;
    */
    // Inherited from IAudioFrameObserver
    virtual bool onRecordAudioFrame(AudioFrame& audioFrame) override;
    virtual bool onPlaybackAudioFrame(AudioFrame& audioFrame) override;
    virtual bool onMixedAudioFrame(AudioFrame& audioFrame) override;
    virtual bool onPlaybackAudioFrameBeforeMixing(unsigned int uid, AudioFrame& audioFrame) override;

    //virtual PROC_RET Proc(StreamInfo *info, AVData *inData, AVData *outData) override;

    int Init(AVEngineConfig &conf);
    // Call befor Init
    void setVideoProfile(int width, int height, int fps, int kbps, int gop);
    void setAudioProfile(int sampleRate, int channels/*, rtc::AUDIO_PROFILE_TYPE profile, rtc::AUDIO_SCENARIO_TYPE scenario*/);
    void pushYUVFrame(void *buffer, int width, int height, int64_t pts);
    void pushPCMFrame(void *buffer, int sampleRate, int channels, int nbSamples, int64_t pts);
    //void pushAVData(AVData *data);
private:
    inline void _validTime(int64_t pts);
    rtc::IRtcEngineEx *applite_;
    std::mutex peer_lock_;
    std::unordered_map<uid_t, PeerStreamPtr> peers_;
    //audio
#if 0
    SwrContext *swr_;
    AVAudioFifo *fifo_;
    AVFrame *audio_frame_;
    AVFrame *fifo_frame_;
#endif
    int channels_;
    int duration_;
    int64_t last_pts_;
    std::mutex audioQueLock_;
    AudioFrameQue audioQue_;
    //video
#if 0
    SwsContext *sws_;
    AVFrame *video_frame_;
#endif
    int width_;
    int height_;
    //int fps_;
    std::mutex videoQueLock_;
    VideoFrameQue videoQue_;

    std::atomic<int64_t> now_ts_;
    std::atomic<bool> ts_valid_;

    rtc::IRtcEngineEventHandlerEx *externHandler_;
};



}}

#endif


#endif // AVENGINE_H
