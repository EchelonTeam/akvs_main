

#include "groupfiles.h"
#include <QObject>


using namespace alex;
using namespace std;

GroupReportFile::GroupReportFile() {}

// закрыть все файлы и удалить выделенное пространство
GroupReportFile::~GroupReportFile()
{
    for( int i=0; i < this->num_file; i++)
    {
        if( this->files.value( i) != NULL )
        {
            this->files.value( i)->close();
            delete this->files.value( i);
        }
    }
}

// создать группу файлов
GroupReportFile::GroupReportFile( int num_file)
{
    for( int i=0 ; i < num_file; i++)
    {
        this->files.push_back( new std::ofstream);
    }
    this->num_file=num_file;
}

int GroupReportFile::OpenFiles(QString files_name, QString path)
{
    QString file_full_name;
    for( int i=0; i < this->num_file ; i++)
    {
        file_full_name = path+"\\"+files_name.arg( i);
        this->files_name.push_back( files_name.arg( i));
        this->files_path.push_back( file_full_name);
        this->files.value( i)->open( file_full_name.toStdString().c_str());
        if( this->files.value( i)->is_open() == false)
        {
            std::cout << QObject::tr("fail").toStdString().c_str() << file_full_name.toStdString() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl; // файл   не открылся
            return -1;
        }
    }
}


int GroupReportFile::OpenFiles(QString files_name, QString path, list<string> & lsProcessedFiles)
{
    QString file_full_name;
    for( int i=0; i < this->num_file ; i++)
    {
        file_full_name = path+"\\"+files_name.arg( i);
        this->files_name.push_back( files_name.arg( i));
        this->files_path.push_back( file_full_name);
        this->files.value( i)->open( file_full_name.toStdString().c_str() );
        lsProcessedFiles.push_back( file_full_name.toStdString().c_str() );
        if( this->files.value( i)->is_open() == false)
        {
            std::cout << QObject::tr("fail").toStdString().c_str() << file_full_name.toStdString() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl; // файл   не открылся
            return -1;
        }
    }
}
// количество файлов
int GroupReportFile::Size()
{
    return num_file;
}

// получить дикриптор определенного файла
std::ofstream & GroupReportFile::GetFile(int i)
{
    if(i < num_file)
        return *files.value( i);
}

int GroupReportFile::WriteToAllFile( QString str)
{
    for( int i=0; i < this->num_file; i++)
    {
        *files.value( i) << str.toStdString().c_str();
    }
}

QString GroupReportFile::GetFileName( int num)
{
    return this->files_name.value( num );
}

QString GroupReportFile::GetFilePath(int num)
{
    return this->files_path.value( num );
}
