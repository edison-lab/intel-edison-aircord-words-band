#include <iostream>
#include <stdio.h>
#include <stdlib.h>

bool isRunning(std::string processName)
{
  FILE *fid;
  std::string ipCommand = "ps | grep " + processName + " | grep -v grep";
  char commandres[80] = {0};
  
  if((fid = popen (ipCommand.c_str(), "r")) == NULL)
  {
    Serial.write("error.");
  }
  
  std::string ret;
  while(fgets(commandres, 80, fid) != NULL) 
  {
    pclose(fid);
    ret = std::string(commandres);
  }
  
  pclose(fid);

  if(ret.length() != 0)
    return true;
  else
    return false;

}

std::string getHostname()
{
  FILE *fid;
  std::string ipCommand = "cat /etc/hostname";
  char commandres[80] = {0};
  
  if((fid = popen (ipCommand.c_str(), "r")) == NULL)
  {
    Serial.write("error.");
    return "";
  }
  
  std::string ret;
  while(fgets(commandres, 80, fid) != NULL) 
  {

    std::string str(commandres);
    if (str[str.size()-1] == '\n') str.erase(str.size()-1);
    if (str[str.size()-1] == '\r') str.erase(str.size()-1);

    ret = str;
  }
  pclose(fid);

  return ret;
}

std::string getIpAddress()
{
  FILE *fid;
  std::string ipCommand = "ifconfig wlan0 | grep inet | sed 's/.*inet[^6][^0-9]*\\([0-9.]*\\)[^0-9]*.*/\\1/'";
  char commandres[80] = {0};
  
  if((fid = popen (ipCommand.c_str(), "r")) == NULL)
  {
    Serial.write("error.");
    return "";
  }
  std::string ret;
  while(fgets(commandres, 80, fid) != NULL) 
  {
    std::string str(commandres);
    if (str[str.size()-1] == '\n') str.erase(str.size()-1);
    if (str[str.size()-1] == '\r') str.erase(str.size()-1);
    ret = str;
  }
  
  pclose(fid);
  return ret;
} // getIpAddress

std::string getSSID()
{
  FILE *fid;
  std::string ssidCommand = "iwconfig wlan0 | grep ESSID: | sed 's/.*ESSID:\"\\(.*\\)\"/\\1/'";
  char commandres[80] = {0};
  
  if((fid = popen (ssidCommand.c_str(), "r")) == NULL)
  {
    Serial.write("error.");
    return "";
  }
  
  std::string ret;
  while(fgets(commandres, 80, fid) != NULL)
  {
    ret = std::string(commandres);
  }
  
  pclose(fid);
  return  ret;
}

