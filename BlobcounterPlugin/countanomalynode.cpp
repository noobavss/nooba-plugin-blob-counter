#include "countanomalynode.h"
#include "QStringList"
#include "QDebug"

CountAnomalyNode::CountAnomalyNode(FeatureNode *parent) :
    FeatureNode(parent),
    anomal_range(true),
    time_window(300),
    min_of_count_range(0),
    max_of_count_range(5)
{
}

CountAnomalyNode::~CountAnomalyNode(){

}

void CountAnomalyNode::processEvents(const QList<DetectedEvent> event)
{
    //This is output event
    QList<DetectedEvent> countanomaly;

    countanomaly = processEventsLocal(event);
    if(!countanomaly.isEmpty()){
        emit generateEvent(countanomaly);
    }

}

QList<DetectedEvent> CountAnomalyNode::processEventsLocal(const QList<DetectedEvent> event)
{
    //This is output event
    QList<DetectedEvent> countanomaly;
    int current_frame_id;
    int r1_to_r2_count_old;
    int r2_to_r1_count_old;
    int r1_to_r2_count_new;
    int r2_to_r1_count_new;

    if(event.isEmpty()){
        return countanomaly;
    }

    qDebug() << event.at(0).getIdentifier() << event.at(0).getMessage();

    //This function should receive count change events only.
    //Example: Debug MSG:  "countchange 348,3,R1->R2,2 1"
    if(event.at(0).getIdentifier() != "countchange"){
        return countanomaly;
    }

    //Parsing frame id
    current_frame_id = event.at(0).getMessage().section(",",0,0).toInt();

    qDebug() << "Current Frame ID: "<< current_frame_id;


//    Insert new events anyway to the queue
    foreach(DetectedEvent e, event){
         if(e.getMessage().section(",",2,2) == "R1->R2"){
            qDebug() << "Going R1->R2";

            //Add New event to the queue
            previousEventsR1_to_R2.enqueue(e);

            //Remove older events from the queues, and take the first of the time period.
            while(true){
                DetectedEvent e = previousEventsR1_to_R2.head();
                int frame_id = e.getMessage().section(",",0,0).toInt();
                if(current_frame_id - frame_id > time_window){
                    previousEventsR1_to_R2.dequeue();
                    if(previousEventsR1_to_R2.isEmpty()){
                        return countanomaly;
                    }
                }
                else{
                    //e is the last related event received till
                    //the beginning of the time period.
                    r1_to_r2_count_old = e.getMessage().section(",",3,3).toInt();
                    qDebug() << "r1_to_r2_count_old: "<< r1_to_r2_count_old;

                    break;
                }
            }
            r1_to_r2_count_new = e.getMessage().section(",",3,3).toInt();
            qDebug() << "r1_to_r2_count_new: "<< r1_to_r2_count_new;

            //This means we have all required numbers.
            //So logic
            if(anomal_range == true){
                //Being within the range is anomal
               if(r1_to_r2_count_new - r1_to_r2_count_old < max_of_count_range
                       && r1_to_r2_count_new - r1_to_r2_count_old > min_of_count_range){
                   countanomaly.append(DetectedEvent("<FONT COLOR='#ff0000'>OutOfPhase", "of the flow R1->R2",1.0));
               }
            }else{
                //Being outside the range is anomal
               if(r1_to_r2_count_new - r1_to_r2_count_old >= max_of_count_range
                       && r1_to_r2_count_new - r1_to_r2_count_old <= min_of_count_range){
                   countanomaly.append(DetectedEvent("<FONT COLOR='#ff0000'>OutOfPhase", "of the flow R1->R2",1.0));
               }
            }

         }
         else if(e.getMessage().section(",",2,2) == "R2->R1"){
             qDebug() << "Going R2->R1";

             //Add New event to the queue
             previousEventsR2_to_R1.enqueue(e);

             //Remove older events from the queues, and take the first of the time period.
             while(true){
                 DetectedEvent e = previousEventsR2_to_R1.head();
                 int frame_id = e.getMessage().section(",",0,0).toInt();
                 if(current_frame_id - frame_id > time_window){
                     previousEventsR2_to_R1.dequeue();
                     if(previousEventsR2_to_R1.isEmpty()){
                         return countanomaly;
                     }
                 }
                 else{
                     //e is the last related event received till
                     //the beginning of the time period.
                     r2_to_r1_count_old = e.getMessage().section(",",3,3).toInt();
                     qDebug() << "r2_to_r1_count_old: "<< r2_to_r1_count_old;

                     break;
                 }
             }
             r2_to_r1_count_new = e.getMessage().section(",",3,3).toInt();
             qDebug() << "r2_to_r1_count_new: "<< r2_to_r1_count_new;

             //This means we have all required numbers.
             //So logic
             if(anomal_range == true){
                 //Being within the range is anomal
                if(r2_to_r1_count_new - r2_to_r1_count_old < max_of_count_range
                        && r2_to_r1_count_new - r2_to_r1_count_old > min_of_count_range){
                    countanomaly.append(DetectedEvent("<FONT COLOR='#ff0000'>OutOfPhase", "of the flow R2->R1",1.0));
                }
             }else{
                 //Being outside the range is anomal
                if(r2_to_r1_count_new - r2_to_r1_count_old >= max_of_count_range
                        || r2_to_r1_count_new - r2_to_r1_count_old <= min_of_count_range){
                    countanomaly.append(DetectedEvent("<FONT COLOR='#ff0000'>OutOfPhase", "of the flow R2->R1",1.0));
                }
             }
         }



    }
    return countanomaly;
}

int CountAnomalyNode::getMax_of_count_range() const
{
    return max_of_count_range;
}

void CountAnomalyNode::setMax_of_count_range(int value)
{
    max_of_count_range = value;
}
int CountAnomalyNode::getMin_of_count_range() const
{
    return min_of_count_range;
}

void CountAnomalyNode::setMin_of_count_range(int value)
{
    min_of_count_range = value;
}

int CountAnomalyNode::getTime_window() const
{
    return time_window;
}
bool CountAnomalyNode::getAnomal_range() const
{
    return anomal_range;
}

void CountAnomalyNode::setAnomal_range(bool value)
{
    anomal_range = value;
}

void CountAnomalyNode::setTime_window(int value)
{
    time_window = value;
}




