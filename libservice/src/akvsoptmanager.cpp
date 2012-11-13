#include "akvsoptmanager.h"

#include <QStringList>

namespace AKVS_OPTMANAGER
{

//CONSOLE=0,CONSOLE_ERR, FILE, FILELIST, DIRECTORY,DATABASE
QStringList lObjects;


void verbose(const OptSwitch * opt)
{
    QStringList arg = opt->argList();
    int level;
    bool ok = true;
    if (arg.isEmpty())
        level = 1;
    else
        level = arg[0].toInt(&ok,10);
    if (!ok)
        level = 1;
    AKVSOptManager::getAKVSOptMgr()->sOptions.verboseLevel = level;
}

void log(const OptSwitch * opt)
{
    QStringList arg = opt->argList();
    if (!arg.isEmpty())
    {
        AKVSOptManager::getAKVSOptMgr()->sOptions.log = arg[0];
    }
}

void input(const OptSwitch * opt)
{
    QStringList arg = opt->argList();
    if (!arg.isEmpty())
    {
        int i=lObjects.indexOf(arg[0]);
        if (i!=-1) AKVSOptManager::getAKVSOptMgr()->sOptions.inputSource=(eObject)i;
        AKVSOptManager::getAKVSOptMgr()->sOptions.input = arg[1];
    }
}

void output(const OptSwitch * opt)
{
    QStringList arg = opt->argList();
    if (!arg.isEmpty())
    {
        int i=lObjects.indexOf(arg[0]);
        if (i!=-1) AKVSOptManager::getAKVSOptMgr()->sOptions.outputSource=(eObject)i;
        AKVSOptManager::getAKVSOptMgr()->sOptions.output = arg[1];
    }
}

};

using namespace AKVS_OPTMANAGER;

AKVSOptManager* AKVSOptManager::_instancePtr=0;

AKVSOptManager::AKVSOptManager(QString ProgName)
{
    lObjects << "console"
             << "console_error"
             << "file"
             << "list"
             << "directory"
             << "db" ;

    sOptions.verboseLevel=0;
    sOptions.inputSource=(eObject)0;
    sOptions.outputSource=(eObject)0;
    pManager = getOptMgr(ProgName);
    QString detail;
    OptManager * om = OptManager::getOptMgr(ProgName);
    //om->addAppArgs("FILE");
    detail = QObject::tr("This program is a test program for the \"qgetopt\" library. It will accept the -d or the -d command line options and requires the name of a file to be passed in.");
    om->addDescription(detail);
    detail = QObject::tr("This is the level of detail that will be written, by default to the console but it can also be written to the file. 0 no detail. 1 the program argument passed in. 2 the options and their value. Add 4 to any of the previous levels and the details will also be written to the file.");
    om->addSwitch("verbous",verbose, 'v', "[LEVEL]")->addDescription(detail);
    detail = QObject::tr("Path to the tre file");
    om->addSwitch("log",log, 'l',"tre-file")->addDescription(detail);

}

/*! This class is a singleton class and so the constructor is private, because of that an object
    cannot be created via new instead it will be returned by this method.

    The first time that this method is called the \em appFullName needs to be provided
    for all subsequent calls it is not needed because the instance will just be returned.
 */
OptManager * AKVSOptManager::getOptMgr(QString appFullName)
{
    return OptManager::getOptMgr(appFullName);
}

AKVSOptManager * AKVSOptManager::getAKVSOptMgr(QString appFullName)
{
    if (_instancePtr==0)
    {
        _instancePtr = new AKVSOptManager(appFullName);
    }
    return _instancePtr;
}

QString AKVSOptManager::parse(int argc, char *argv[], bool quiet)
{
    return pManager->parse(argc,argv,quiet);
}
