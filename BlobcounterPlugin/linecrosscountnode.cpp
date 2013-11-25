#include "linecrosscountnode.h"

#include <QList>
#include <QString>
#include <QStringList>
#include <QDebug>

LineCrossCountNode::LineCrossCountNode(FeatureNode *parent):
    FeatureNode(parent),
    r1_to_r2_count(0),
    r2_to_r1_count(0),
    max_threshold_r1_to_r2(0),
    max_threshold_r2_to_r1(-1)
{
}


LineCrossCountNode::~LineCrossCountNode(){

}

void LineCrossCountNode::processEvents(const QList<DetectedEvent> event)
{
    //This is output event
    QList<DetectedEvent> countchange;

    countchange = processEventsLocal(event);
    if(!countchange.isEmpty()){
        emit generateEvent(countchange);
    }

}

QList<DetectedEvent> LineCrossCountNode::processEventsLocal(const QList<DetectedEvent> event)
{
    //This is output event
    QList<DetectedEvent> countchange;
    foreach(DetectedEvent linecrossevent, event){

        QList<QString> message = linecrossevent.getMessage().split(",");
        QString region1 = message.at(2).section("->",0,0);
        if(region1 == "R1"){
            r1_to_r2_count++;
            countchange.append(DetectedEvent("countchange",QString("%1,%2,R1->R2,%3").arg(message.at(0)).arg(message.at(1)).arg(r1_to_r2_count),1.0));

        }
        else{
            r2_to_r1_count++;
            countchange.append(DetectedEvent("countchange",QString("%1,%2,R2->R1,%3").arg(message.at(0)).arg(message.at(1)).arg(r2_to_r1_count),1.0));

        }
    }
    return countchange;
}
