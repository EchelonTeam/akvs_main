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
        \a GroupReportFile ����� �� �������� ������ ������ ��� �������
  */

class GroupReportFile
{
public:
    // �����������
    GroupReportFile();

    // ������� ��� ����� � ������� ���������� ������������
    ~GroupReportFile();

    // ������� ������ ������
    GroupReportFile( int num_file);

    int OpenFiles(QString files_name, QString path);
    int OpenFiles(QString files_name, QString path, list<string> & lsProcessedFiles);
    // ���������� ������
    int Size();

    // �������� ��������� ������������� �����
    std::ofstream &GetFile(int i);

    // �������� �� ��� ����� ������ ������
    int WriteToAllFile(QString str);

    // �������� ��� ��� \a num �����
    QString GetFileName( int num);

    // �������� ���� � ����� \a num
    QString GetFilePath( int num);
private:
    /**
          ������ ������
      */

    QList< std::ofstream * > files;
    QList< QString> files_name;
    QList< QString> files_path;
    /**
          ���������� ������
      */
    int num_file;
};


};








#endif // GROUPFILES_H
