#ifndef GROUPFILES_H
#define GROUPFILES_H

#include <QList>
#include <QString>
#include <fstream>
#include <iostream>
#include <list>


using namespace std;
namespace alex
{
/*!
        \a GroupReportFile класс по созданию группы файлов для отчетов
  */

class GroupReportFile
{
public:
    // конструктор
    GroupReportFile();

    // закрыть все файлы и удалить выделенное пространство
    ~GroupReportFile();

    // создать группу файлов
    GroupReportFile( int num_file);

    int OpenFiles(QString files_name, QString path);
    int OpenFiles(QString files_name, QString path, list<string> & lsProcessedFiles);
    // количество файлов
    int Size();

    // получить дикриптор определенного файла
    std::ofstream &GetFile(int i);

    // записать во все файлы данную строку
    int WriteToAllFile(QString str);

    // получить имя для \a num файла
    QString GetFileName( int num);

    // получить путь к файлу \a num
    QString GetFilePath( int num);
private:
    /**
          массив файлов
      */

    QList< std::ofstream * > files;
    QList< QString> files_name;
    QList< QString> files_path;
    /**
          количество файлов
      */
    int num_file;
};


};








#endif // GROUPFILES_H
