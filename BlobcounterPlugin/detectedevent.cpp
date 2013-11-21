#include "detectedevent.h"

DetectedEvent::DetectedEvent(QObject *parent)
    :QObject(parent)
{
}

DetectedEvent::DetectedEvent(const DetectedEvent &other)
{
    event_identifier = other.getIdentifier();
    event_message = other.getMessage();
    confidence = other.getConfidence();
}

DetectedEvent::~DetectedEvent()
{
}

DetectedEvent & DetectedEvent::operator =(const DetectedEvent &rhs)
{
    if(&rhs == this)
        return *this;

    event_identifier = rhs.getIdentifier();
    event_message = rhs.getMessage();
    confidence = rhs.getConfidence();
    return *this;
}
