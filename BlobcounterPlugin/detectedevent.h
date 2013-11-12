#ifndef DETECTEDEVENT_H
#define DETECTEDEVENT_H

#include <QObject>
#include <QString>
#include <QMetaType>

class DetectedEvent : public QObject
{
    Q_OBJECT
public:
    DetectedEvent(QObject *parent = 0);             // constructor
    DetectedEvent(const DetectedEvent &other,QObject *parent = 0);  // copy constructor
    DetectedEvent& operator =(const DetectedEvent &other); // assignment operator
    ~DetectedEvent();

    DetectedEvent(const QString event_id,const QString event_msg, const float conf,QObject *parent = 0) :
         QObject(parent),event_identifier(event_id),event_message(event_msg),confidence(conf){}

    QString getIdentifier() const { return event_identifier;}
    QString getMessage() const { return event_message;}
    float getConfidence() const { return confidence;}

    void setIdentifier(const QString event_id) { event_identifier = event_id;}
    void setMessage(const QString event_msg)  { event_message = event_msg;}
    void setConfidence(const float conf) { confidence = conf;}


signals:

public slots:

private:
    QString event_identifier;
    QString event_message;
    float confidence;
};
Q_DECLARE_METATYPE(DetectedEvent)

#endif // DETECTEDEVENT_H
