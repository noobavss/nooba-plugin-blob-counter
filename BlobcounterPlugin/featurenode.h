#ifndef FEATURENODE_H
#define FEATURENODE_H

#include <QObject>
#include <QList>
#include "detectedevent.h"

class FeatureNode : public QObject
{
    Q_OBJECT
public:
    explicit FeatureNode(QObject* parent = 0);

    virtual void processEvents(const QList<DetectedEvent> event) = 0;
public slots:
    void captureEvent(QList<DetectedEvent> captured_event){ processEvents(captured_event);}
signals:
    void generateEvent(QList<DetectedEvent> generated_event);

};


#endif // FEATURENODE_H
