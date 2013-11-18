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
    explicit LineCrossingNode(FeatureNode* parent = 0);
    ~LineCrossingNode();

    void processEvents(const QList<DetectedEvent> event);
    inline QPoint getPoint1(){return point1;}
    inline QPoint getPoint2(){return point2;}

private:
    QPoint point1;
    QPoint point2;


    enum BlobRegion
    {
      BLOB_REGION_UNKNOWN = 0,
      BLOB_REGION_ONE  = 1,
      BLOB_REGION_TWO  = 2
    };

    QHash<QString,DetectedEvent> previousEvents;
    BlobRegion getBlobRegion(QPoint centroid);
};

#endif // LINECROSSINGNODE_H
