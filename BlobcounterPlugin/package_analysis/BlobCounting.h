
#pragma once
//#include <boost/config/user.hpp>
//#include <boost/lexical_cast.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDir>

#include "../package_tracking/cvblob/cvblob.h"

enum LaneOrientation
{
  LO_NONE       = 0,
  LO_HORIZONTAL = 1,
  LO_VERTICAL   = 2
};

enum BlobPosition
{
  VP_NONE = 0,
  VP_A  = 1,
  VP_B  = 2
};

class BlobCounting
{
private:
  bool firstTime;
  bool showOutput;
  int key;
  cv::Mat img_input;
  cvb::CvTracks tracks;
  std::map<cvb::CvID, std::vector<CvPoint2D64f> > points;
  LaneOrientation laneOrientation;
  std::map<cvb::CvID, BlobPosition> positions;
  long countAB;
  long countBA;
  int img_w;
  int img_h;
  int showAB;
  char string[100];

public:
  BlobCounting();
  ~BlobCounting();

  void setInput(const cv::Mat &i);
  void setTracks(const cvb::CvTracks &t);
  void process();

  inline void setFrameIndex(int index){frameIndex = index;}
private:
  BlobPosition getBlobPosition(const CvPoint2D64f centroid);
  QFile file;
  
	
  int frameIndex;

  void saveConfig();
  void loadConfig();
};
