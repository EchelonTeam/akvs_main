#ifndef AKVSOPTMANAGER_H
#define AKVSOPTMANAGER_H

#include "optManager.h"




class AKVSOptManager
{
public:
    QString strDescription;
    struct main_options
    {
        int verboseLevel;
        AKVS_OPTMANAGER::eObject inputSource;
        AKVS_OPTMANAGER::eObject outputSource;
        QString input,output,log;
    } sOptions;
    static AKVSOptManager* _instancePtr;
public:
    OptManager* pManager;


    /*! This class is a singleton class and so the constructor is private, because of that an object
    cannot be created via new instead it will be returned by this method.

    The first time that this method is called the \em appFullName needs to be provided
    for all subsequent calls it is not needed because the instance will just be returned.
    */
public:
    static OptManager * getOptMgr(QString appFullName="");
    static AKVSOptManager * getAKVSOptMgr(QString appFullName="");

    QString parse(int argc, char *argv[], bool quiet=false);
private:
    AKVSOptManager(QString appFullName="");

};




#endif // AKVSOPTMANAGER_H
