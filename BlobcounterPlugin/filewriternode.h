#ifndef FILEWRITERNODE_H
#define FILEWRITERNODE_H

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <detectedevent.h>
#include <featurenode.h>

class FileWriterNode : public FeatureNode
{
public:
    explicit FileWriterNode(FeatureNode* parent = 0);
    ~FileWriterNode();

    void processEvents(const QList<DetectedEvent> event);

    void closeFile(void);
    bool openFile(QString filename);

private:
    QFile file;
    QTextStream out_stream;


};

#endif // FILEWRITERNODE_H
