#pragma once


#include <string>

class ReadConfig
{
public:
  ReadConfig(std::string& filepath);
  ~ReadConfig();
public:
  void readText();
public:
  void setAppID(std::string& appid){m_appid = appid;}
  std::string getAppId(){return m_appid;}

  void setChannel(std::string& channel) {m_channel = channel;}
  std::string getChannel(){return m_channel;}

  void setUID(std::string& uid){m_uid = uid;}
  std::string getUID(){return m_uid;}

  void setLive(std::string& live) {m_live = live;}
  std::string getLive(){return m_live;}


  void setWidth(std::string& width){m_width = width;}
  std::string getWidth(){return m_width;}

  void setHeight(std::string& height){m_height = height;}
  std::string getHeight(){return m_height;}

  void setFPS(std::string& fps){m_fps = fps;}
  std::string getFPS(){return m_fps;}

  void setBitrate(std::string& bitrate){m_bitrate = bitrate;}
  std::string getBitrate(){return m_bitrate;}

  void setChannelKey(std::string& channelKey){m_channelKey = channelKey;}
  std::string getChannelKey(){return m_channelKey;}

  void setCameraId(std::string& cameraId){m_cameraId = cameraId;}
  std::string getCameraId(){return m_cameraId;}

private:
  std::string m_appid;
  std::string m_channel;
  std::string m_uid;
  std::string m_live;
  std::string m_width;
  std::string m_height;
  std::string m_fps;
  std::string m_bitrate;
  std::string m_filepath;
  std::string m_channelKey;
  std::string m_cameraId;
};
