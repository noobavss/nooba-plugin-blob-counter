#ifndef LINECROSSINGNODE_H
#define LINECROSSINGNODE_H

#include <QList>
#include <QStringList>
#include <QDebug>
#include <QHash>
#include <QPoint>

#include <featurenode.h>

class LineCrossingNode : public FeatureNode
{
public:
    enum BlobRegion
    {
      BLOB_REGION_UNKNOWN = 0,
      BLOB_REGION_ONE  = 1,
      BLOB_REGION_TWO  = 2
    };

    explicit LineCrossingNode(FeatureNode* parent = 0);
    ~LineCrossingNode();

    void processEvents(const QList<DetectedEvent> event);
    QList<DetectedEvent> processEventsLocal(const QList<DetectedEvent> event);
    inline QPoint getPoint1(){return point1;}
    inline QPoint getPoint2(){return point2;}
    inline void setPoint1_x(int point1_x){point1.setX(point1_x);}
    inline void setPoint1_y(int point1_y){point1.setY(point1_y);}
    inline void setPoint2_x(int point2_x){point2.setX(point2_x);}
    inline void setPoint2_y(int point2_y){point2.setY(point2_y);}

    BlobRegion getBlobRegion(QPoint centroid);

private:
    QPoint point1;
    QPoint point2;




    QHash<QString,DetectedEvent> previousEvents;
};

#endif // LINECROSSINGNODE_H
