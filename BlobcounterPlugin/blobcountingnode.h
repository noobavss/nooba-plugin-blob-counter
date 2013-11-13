#ifndef BLOBCOUNTINGNODE_H
#define BLOBCOUNTINGNODE_H

#include <QList>
#include <QStringList>
#include <QDebug>
#include <QHash>
#include <QPoint>

#include <featurenode.h>

class BlobCountingNode : public FeatureNode
{
public:
    explicit BlobCountingNode(FeatureNode* parent = 0);
    ~BlobCountingNode();

    void processEvents(const QList<DetectedEvent> event);

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

#endif // BLOBCOUNTINGNODE_H
