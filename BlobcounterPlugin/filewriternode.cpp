#include "filewriternode.h"

FileWriterNode::FileWriterNode(FeatureNode *parent) :
    FeatureNode(parent)
{
}

FileWriterNode::~FileWriterNode()
{

    if(file.isOpen()){
        file.flush();
        file.close();
    }
}

void FileWriterNode::processEvents(const QList<DetectedEvent> event)
{
    foreach(DetectedEvent e, event){
        //qDebug() << "===================================" << e.getIdentifier() << " " << e.getMessage() << " " << e.getConfidence();
        out_stream << e.getIdentifier() << "," << e.getMessage() << "\n";
        out_stream.flush();
    }
}

bool FileWriterNode::openFile(QString filename){

    if(file.isOpen()){
        file.flush();
        file.close();
    }
    file.setFileName(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Cannot create/open file named " << filename << "! Abort.";
        return false;
    }
    out_stream.setDevice(&file);
    return true;
}

void FileWriterNode::closeFile(void){
    file.flush();
    file.close();
}
