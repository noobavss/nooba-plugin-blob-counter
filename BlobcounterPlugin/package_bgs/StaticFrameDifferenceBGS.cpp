#include "StaticFrameDifferenceBGS.h"

StaticFrameDifferenceBGS::StaticFrameDifferenceBGS() : firstTime(true), enableThreshold(true), threshold(15), showOutput(true)
{
  std::cout << "StaticFrameDifferenceBGS()" << std::endl;
}

StaticFrameDifferenceBGS::~StaticFrameDifferenceBGS()
{
  std::cout << "~StaticFrameDifferenceBGS()" << std::endl;
}

void StaticFrameDifferenceBGS::process(const cv::Mat &img_input, cv::Mat &img_output)
{
  if(img_input.empty())
    return;

  if(img_background.empty())
    img_input.copyTo(img_background);
  
  loadConfig();

  if(firstTime)
    saveConfig();

  cv::absdiff(img_input, img_background, img_foreground);

  if(img_foreground.channels() == 3)
    cv::cvtColor(img_foreground, img_foreground, CV_BGR2GRAY);

  if(enableThreshold)
    cv::threshold(img_foreground, img_foreground, threshold, 255, cv::THRESH_BINARY);


	cv::dilate(img_foreground,img_foreground,NULL,cv::Point(-1,-1),2);
	cv::dilate(img_foreground,img_foreground,NULL,cv::Point(-1,-1),2);
	cv::dilate(img_foreground,img_foreground,NULL,cv::Point(-1,-1),2);
	cv::dilate(img_foreground,img_foreground,NULL,cv::Point(-1,-1),2);

  if(showOutput)
    cv::imshow("Static Frame Difference", img_foreground);

  img_foreground.copyTo(img_output);

  firstTime = false;
}

void StaticFrameDifferenceBGS::saveConfig()
{
    QDir dir(QDir::home());
    if(!dir.exists("NoobaVSS")){
        dir.mkdir("NoobaVSS");
    }
    dir.cd("NoobaVSS");
    if(!dir.exists("config")){
        dir.mkdir("config");
    }
    dir.cd("config");

  CvFileStorage* fs = cvOpenFileStorage(dir.absoluteFilePath("StaticFrameDifferenceBGS.xml").toLocal8Bit(), 0, CV_STORAGE_WRITE);

  cvWriteInt(fs, "enableThreshold", enableThreshold);
  cvWriteInt(fs, "threshold", threshold);
  cvWriteInt(fs, "showOutput", showOutput);

  cvReleaseFileStorage(&fs);
}

void StaticFrameDifferenceBGS::loadConfig()
{

    QDir dir(QDir::home());
    if(!dir.exists("NoobaVSS")){
        dir.mkdir("NoobaVSS");
    }
    dir.cd("NoobaVSS");
    if(!dir.exists("config")){
        dir.mkdir("config");
    }
    dir.cd("config");
    CvFileStorage* fs = cvOpenFileStorage(dir.absoluteFilePath("StaticFrameDifferenceBGS.xml").toLocal8Bit(), 0, CV_STORAGE_READ);

    enableThreshold = cvReadIntByName(fs, 0, "enableThreshold", true);
    threshold = cvReadIntByName(fs, 0, "threshold", 15);
    showOutput = cvReadIntByName(fs, 0, "showOutput", false);

    cvReleaseFileStorage(&fs);
}
