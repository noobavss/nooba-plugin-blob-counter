#include "linecrossingnode.h"

LineCrossingNode::LineCrossingNode(FeatureNode *parent) :
    FeatureNode(parent),
    point1(QPoint(500.0,0.0)),
    point2(QPoint(500.0,700.0))
{
}

LineCrossingNode::~LineCrossingNode()
{

}


void LineCrossingNode::processEvents(const QList<DetectedEvent> event)
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
            if(previousEvents.contains(newMessage.at(1))){
                //Then we have two instances, so can check for a change in area.
                //expected to be newMessage.at(1) as 1 which are the id of a blob
                DetectedEvent oldEvent = previousEvents.value(newMessage.at(1));
                QList<QString> oldMessage = oldEvent.getMessage().split(",");

                BlobRegion newRegion = getBlobRegion(QPoint(newMessage.at(2).toFloat(),newMessage.at(3).toFloat()));
                BlobRegion oldRegion = getBlobRegion(QPoint(oldMessage.at(2).toFloat(),oldMessage.at(3).toFloat()));

                if(newRegion != oldRegion){
                    if(newRegion == BLOB_REGION_ONE && oldRegion == BLOB_REGION_TWO){
                        corssAreaEvent.append(DetectedEvent("crossed",QString("%1,%2,%3").arg(newMessage.at(0)).arg(newMessage.at(1)).arg("R1->R2"),1.0));
                    }
                    else{
                        corssAreaEvent.append(DetectedEvent("crossed",QString("%1,%2,%3").arg(newMessage.at(0)).arg(newMessage.at(1)).arg("R2->R1"),1.0));
                    }
                }
            }
            else{
                //corssAreaEvent.append(DetectedEvent("crossed",QString("%1,%2,%3").arg(newMessage.at(0)).arg(newMessage.at(1)).arg("notImplementedYet"),1.0));

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

LineCrossingNode::BlobRegion LineCrossingNode::getBlobRegion(QPoint centroid)
{
  BlobRegion blobRegion = BLOB_REGION_UNKNOWN;

  //qDebug() <<"p1.x " << point1.x() << " p2.x " << point2.x() << "centroid.x " << centroid.x();

  //Page 1017  Introduction to algorithms
  //Thomson H. Cormen
  //Computational GeomatryClockwise / Counterclockwise measure
  //p0 = point1 p1 = pont2 p2 = centroid
  float crossproduct = (point2.x() - point1.x()) * (centroid.y() - - point1.y()) - (centroid.x() - point1.x()) * (point2.y() - point1.y());


  //qDebug() <<"crossproduct " << crossproduct;
  if(crossproduct > 0.0){
      blobRegion = BLOB_REGION_ONE;
  }
  else{
      blobRegion = BLOB_REGION_TWO;
  }
  return blobRegion;
}
