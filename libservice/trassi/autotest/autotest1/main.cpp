#include <QtCore/QCoreApplication>

#include <matrix_report.h>
#include <iostream>
#include <libdb_interface.h>
#include <data_orm.h>
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <libservice.h>
using namespace std;
using namespace ak_vs_db;
int main(int argc, char *argv[])
{
    if( argc<3)
    {
        cout<< "program.exe <dir1> <dir2>"<< endl;
        cout<< "<dir1> - directory for control matrix"<< endl;
        cout<< "<dir2> - directory for information matrix"<< endl;
        return  1;
    }

    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale(QLocale::Russian,QLocale::RussianFederation).name());
    a.installTranslator(&qtTranslator);

    //соедин€емс€ с базой
    db_connect (1);
    //выбираем кластер
    db_set_current_cluster(0);
    //сделать список линков

    cout<<"debug matrix"<<endl;
    list< string> ls;

    /*
        matrix_cont_legend( QString( argv[ 1]), 1000, 100, ls);
        matrix_cont( QString( argv[ 1]), 1000 ,100, ls);
        matrix_info( QString( argv[ 2]), 1000 ,100, ls);
        matrix_info_legend( QString( argv[ 2]), 1000, 100, ls);
    */
    //list< string>  lsProcessedFiles;

    TraceReportf_f_new(
        string( "ѕроектное им€"),   // им€ проекта
        string( "1.0"),             // им€ версии
        string( "report"),          // начала имени файла
        string( "c:\\test"),        // путь
        100,                        // модуль дл€ файлов
        ls);
    cout<<"end matrix"<<endl;

    db_set_current_cluster(NULL);
    db_dconnect();

    return -1;
}
