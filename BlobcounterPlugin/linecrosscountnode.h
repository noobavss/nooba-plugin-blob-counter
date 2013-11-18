#ifndef LINECROSSCOUNTNODE_H
#define LINECROSSCOUNTNODE_H

#include <featurenode.h>
#include <QList>
#include <QString>

class LineCrossCountNode : public FeatureNode
{

public:
    explicit LineCrossCountNode(FeatureNode* parent = 0);
    ~LineCrossCountNode();
    void processEvents(const QList<DetectedEvent> event);

private:
    enum BlobRegion
    {
      BLOB_REGION_UNKNOWN = 0,
      BLOB_REGION_ONE  = 1,
      BLOB_REGION_TWO  = 2
    };

    int r1_to_r2_count;
    int r2_to_r1_count;
    int max_threshold_r1_to_r2;  //Not used yet
    int max_threshold_r2_to_r1; //Not used yet

};

#endif // LINECROSSCOUNTNODE_H
