#include "blobcountingnode.h"

BlobCountingNode::BlobCountingNode(FeatureNode *parent) :
    FeatureNode(parent)
{
}

BlobCountingNode::~BlobCountingNode()
{

}


void BlobCountingNode::processEvents(const QList<DetectedEvent> event)
{


    //This should recieve blob events only.

    //This is output event
    QList<DetectedEvent> corssAreaEvent;


    //previousEvents hash map consists of last received events regarding each blob tag.
    //if this is empty, we can't compare the area which was earlier, so no crossings.
    if(!previousEvents.isEmpty()){

        foreach(DetectedEvent newEvent, event){
            //We are checking is there any previous matching blob for each new blob.
            //If so,
            QList<QString> newMessage = newEvent.getMessage().split(",");
            if(!previousEvents.contains(newMessage.at(1))){
                //Then we have two instances, so can check for a change in area.
                //expected to be newMessage.at(1) as 1 which are the id of a blob
    //            DetectedEvent oldEvent = previousEvents.value(newMessage.at(1));
    //            QList<QString> oldMessage = oldEvent.getMessage().split(",");
                corssAreaEvent.append(DetectedEvent("crossed",QString("%1,%2,%3").arg(newMessage.at(0)).arg(newMessage.at(1)).arg("notImplementedYet"),1.0));

            }
        }
    }

    //Insert new events anyway to the hash map.
    //If already exists the key, this will replace the previous version
    foreach(DetectedEvent e, event){
        QList<QString> message = e.getMessage().split(",");
        previousEvents.insert(message.at(1),e);
    }
    if(!corssAreaEvent.isEmpty()){
        emit generateEvent(corssAreaEvent);
    }
}

