#include "enginehandler.h"
#include "stdio.h"


void EngineHandler::onJoinChannelSuccess(const char *channel, uid_t uid, int elapsed)
{
    fprintf(stderr, "user(%u) join channel(%s) success.\n", uid, channel);
}
void EngineHandler::onRejoinChannelSuccess(const char *channel, uid_t uid, int elapsed)
{
    fprintf(stderr, "user(%u) rejoin channel(%s) success.\n", uid, channel);
}
void EngineHandler::onWarning(int warn, const char *msg)
{
    fprintf(stderr, "engine warning(%d): %s.\n", warn, msg);
}
void EngineHandler::onError(int err, const char *msg)
{
    fprintf(stderr, "engine error(%d): %s.\n", err, msg);
}
void EngineHandler::onUserJoined(uid_t uid, int elapsed)
{
    fprintf(stderr, "user(%u) joined.\n", uid);
}
void EngineHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
    fprintf(stderr, "user(%u) offline.\n", uid);
}
void EngineHandler::onConnectionLost()
{
    fprintf(stderr, "vos connection lost.\n");
}
bool EngineHandler::onCustomizedSei(const void **content, int *length)
{
    return true;
}
