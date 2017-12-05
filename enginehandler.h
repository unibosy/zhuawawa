#ifndef ENGINEHANDLER_H
#define ENGINEHANDLER_H

#include "internal.h"

using namespace agora::rtc;

class EngineHandler : public IRtcEngineEventHandlerEx
{
public:
    // inherited from IRtcEngineEventHandlerEx
    virtual void onJoinChannelSuccess(const char *channel, uid_t uid, int elapsed) override;
    virtual void onRejoinChannelSuccess(const char *channel, uid_t uid, int elapsed) override;
    virtual void onWarning(int warn, const char *msg) override;
    virtual void onError(int err, const char *msg) override;
    virtual void onUserJoined(uid_t uid, int elapsed) override;
    virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason) override;
    virtual void onConnectionLost() override;
    virtual bool onCustomizedSei(const void **content, int *length) override;
};


#endif // ENGINEHANDLER_H
