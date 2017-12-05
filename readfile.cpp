#include "readfile.h"

#include <iostream>
#include <fstream>
#include <cstring>

#define APPID "APPID="
#define UID "UID="
#define WIDTH "WIDTH="
#define HEIGHT "HEIGHT="
#define FPS "FPS="
#define BITRATE "BITRATE="
#define CHANNEL "CHANNEL="
#define LIVE "LIVE="


ReadConfig::ReadConfig(std::string& filepath) :m_appid(""), m_uid(""), m_width(""),m_height(""),m_fps(""),m_bitrate(""),
    m_live("0"),m_channel("")
{
  m_filepath = filepath;
}
ReadConfig::~ReadConfig()
{

}

void ReadConfig::readText()
{
  std::string line = "";

  std::ifstream file;
  file.open(m_filepath);
  if (file.is_open())
  {
    while (getline(file, line))
    {
      if(!strncmp(line.c_str(), APPID, strlen(APPID)))
      {
        std::string appid = "";
        size_t appid_pos = line.find("=");
        appid = line.substr(appid_pos + 1,strlen(line.c_str())- appid_pos + 1);
        setAppID(appid);
      }
      else if (!strncmp(line.c_str(), UID,strlen(UID)))
      {
        std::string uid = "";
        size_t uid_pos = line.find("=");
        uid = line.substr(uid_pos + 1, strlen(line.c_str()) - uid_pos + 1);
        setUID(uid);
      }
      else if (!strncmp(line.c_str(), WIDTH,strlen(WIDTH)))
      {
        std::string width = "";
        size_t width_pos = line.find("=");
        width = line.substr(width_pos + 1, strlen(line.c_str()) - width_pos + 1);
        setWidth(width);
      }
      else if (!strncmp(line.c_str(), HEIGHT,strlen(HEIGHT)))
      {
        std::string height = "";
        size_t height_pos = line.find("=");
        height = line.substr(height_pos + 1, strlen(line.c_str()) - height_pos + 1);
      }
      else if (!strncmp(line.c_str(), FPS,strlen(FPS)))
      {

        std::string fps = "";
        size_t fps_pos = line.find("=");
        fps = line.substr(fps_pos + 1, strlen(line.c_str()) - fps_pos + 1);
        setFPS(fps);
      }
      else if (!strncmp(line.c_str(), BITRATE,strlen(BITRATE)))
      {
        std::string bitrate = "";
        size_t bitrate_pos = line.find("=");
        bitrate = line.substr(bitrate_pos + 1, strlen(line.c_str()) - bitrate_pos + 1);
        setBitrate(bitrate);
      }

      else if (!strncmp(line.c_str(), CHANNEL,strlen(CHANNEL)))
      {
        std::string channel = "";
        size_t channel_pos = line.find("=");
        channel = line.substr(channel_pos + 1, strlen(line.c_str()) - channel_pos + 1);
        setChannel(channel);
      }
      else if (!strncmp(line.c_str(), LIVE,strlen(LIVE)))
      {
        std::string live = "";
        size_t live_pos = line.find("=");
        live = line.substr(live_pos + 1, strlen(line.c_str()) - live_pos + 1);
        setLive(live);
      }
    }
  }

}


