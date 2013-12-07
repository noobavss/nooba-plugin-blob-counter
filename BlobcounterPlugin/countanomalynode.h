#ifndef COUNTANOMALYNODE_H
#define COUNTANOMALYNODE_H

#include <featurenode.h>
#include <QList>
#include <detectedevent.h>
#include <QQueue>

class CountAnomalyNode: public FeatureNode
{
public:
    enum COUNTING_DIRECTION{
        R1_to_R2 = true,
        R2_to_R1 = false
    };

    explicit CountAnomalyNode(FeatureNode* parent = 0);
    ~CountAnomalyNode();
    void processEvents(const QList<DetectedEvent> event);
    QList<DetectedEvent> processEventsLocal(const QList<DetectedEvent> event);

    int getMax_of_count_range() const;
    void setMax_of_count_range(int value);

    int getMin_of_count_range() const;
    void setMin_of_count_range(int value);

    int getTime_window() const;
    void setTime_window(int frames);

    bool getAnomal_range() const;
    void setAnomal_range(bool range);

    void setDirection(COUNTING_DIRECTION counting_direction);
private:
    bool anomal_range;  //in true, out false
    int time_window;
    int min_of_count_range;
    int max_of_count_range;
    COUNTING_DIRECTION direction; //R1 to R2 => true
                    //R2 to R1 => false
    QQueue<DetectedEvent> previousEvents;


};

#endif // COUNTANOMALYNODE_H
