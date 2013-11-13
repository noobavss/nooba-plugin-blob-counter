#ifndef BLOBCOUNTINGNODE_H
#define BLOBCOUNTINGNODE_H

#include <QList>
#include <QStringList>
#include <QDebug>
#include <QHash>
#include <featurenode.h>

class BlobCountingNode : public FeatureNode
{
public:
    explicit BlobCountingNode(FeatureNode* parent = 0);
    ~BlobCountingNode();

    void processEvents(const QList<DetectedEvent> event);

private:
    QHash<QString,DetectedEvent> previousEvents;

};

#endif // BLOBCOUNTINGNODE_H
