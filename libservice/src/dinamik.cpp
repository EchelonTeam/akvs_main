/*
 * dinamik.cpp
 *
 *  Created on: 16.12.2008
 *      Author: javamain
 */
#include "libservice.h"
#include "groupfiles.h"
#include <sqladapter.h>



using namespace alex;
using namespace std;

HRET DinamicReport(sReportInfo* pa)
{
    DinamicReport(pa->strProjectName,ak_vs_db::get_program_name(),pa->strBaseFileName+"5.",pa->strOutputDirectory,20000,*(pa->lsProcessedFiles));
    DinamicReport_vetvi(pa->strProjectName,ak_vs_db::get_program_name(),pa->strBaseFileName+"5.1.",pa->strOutputDirectory,20000,*(pa->lsProcessedFiles));
    return 1;
}



// расчитать покрытие отработанных дин. функций ко всем функциям

string pokritie()
{
    sqlAdapterForInProb dlnk_tmp;
    // подсчитаем число функций
    int num_func = dlnk_tmp.functionCount();
    // опдсчитаем число динамических функций
    int num_din_func = dlnk_tmp.Size();

    return num_func != 0
           ? QString::number(100 * num_din_func / num_func).toStdString()
           : "0";
}


int DinamicReport(
    string project_name,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles)
{
    cout << "Din report start" << endl;
    int numstrok= NumFunctionInDb();
    sqlAdapterForInProb tmp_prob_ad ;
    int num_html_file= tmp_prob_ad.Size() / mod_for_html_file + 1; // столько html файлов будем создавать

    GroupReportFile gfiles( num_html_file);
    QString file_name = QString( base_file_name.c_str())  + "%1.html";
    gfiles.OpenFiles( file_name, QString(path.c_str()));

    for(int i=0; i < gfiles.Size(); i++)
    {
        lsProcessedFiles.push_back(gfiles.GetFilePath(i).toStdString());
    }

    gfiles.WriteToAllFile( "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n");
    gfiles.WriteToAllFile( getCSS().c_str() );
    gfiles.WriteToAllFile( "</HEAD>\n<BODY>" );
    zagolovok_html_gfile( gfiles, QObject::tr("OTCHET PO DINAMICHESKOMY ANALIZY(FYNKCIA-FYNKCIA)").toStdString().c_str(), project_name, version); // ОТЧЕТ ПО ДИНАМИЧЕСКОМУ АНАЛИЗУ (ФУНКЦИЯ-ФУНКЦИЯ)
    silki_na_sleduiuchiyg( gfiles);
    gfiles.WriteToAllFile( html_breaket( "H5", "", string( QObject::tr("Pokritie fynkcii pri dinamicheskom analize: ").toStdString().c_str()) + pokritie() + "%").c_str() ); // Покрытие функций при динамическом анализе:
    gfiles.WriteToAllFile( "<br><table class=\"content\" border=1 align=\"center\"> \n");

    gfiles.WriteToAllFile(
        html_tr(
            html_th("№") +
            html_th(QObject::tr("Fynkcii").toStdString().c_str()) + // Функции
            html_th(QObject::tr("Fail").toStdString().c_str()) +    // Файл
            html_th(QObject::tr("Stroka").toStdString().c_str()) +  // Строка
            html_th(QObject::tr("Fynkcii").toStdString().c_str()) + // Функции
            html_th(QObject::tr("Fail").toStdString().c_str()) +    // Файл
            html_th(QObject::tr("Stroka").toStdString().c_str()) +  // Строка
            html_th(QObject::tr("Pokritie v dinamicheskom analize").toStdString().c_str()) // Покрытие в динамическом анализе
        ).c_str()
    );
    gfiles.WriteToAllFile( "\n");

    sqlAdapterForInProb dlnk;
    for( int i=0, t=0; i < num_html_file; i++)
    {

        for( int j = 0, k=0; (j < mod_for_html_file) && dlnk.next()  ; j++, t++)
        {
            k= j + i*mod_for_html_file;

            // вызод если что
            if( k >=  numstrok )
            {
                break;
            }

//            if( dlnk.IsProbGone() == 0)
//            {
//                gfiles.GetFile( i) << "<tr class=\"myeen\">";
//            }

//            if( dlnk.IsProbGone() == 1)
//            {
//                gfiles.GetFile( i) << "<tr class=\"myred\">";
//            }

            gfiles.GetFile( i) << ( html_th( inttostring( t+1))+
                                    html_td( dlnk.name1().toStdString() ) +
                                    html_td( dlnk.path1().toStdString() ) +
                                    html_td( dlnk.pos1() != -1 ? QString::number( dlnk.pos1()).toStdString() : "~" ) +
                                    html_td( dlnk.name2().toStdString() ) +
                                    html_td( dlnk.path2().toStdString() ) +
                                    html_td( dlnk.pos2() != -1 ? QString::number( dlnk.pos2()).toStdString() : "~" ) +
                                    html_breaket( "TD","ALIGN=\"center\"", dlnk.isUsedLink(dlnk.id2()) ? "+" : "-" )
                                  ).c_str();
            gfiles.GetFile( i) << "</TR>" << endl;
        }
    }

    gfiles.WriteToAllFile( "</TABLE><br>\n");
    silki_na_sleduiuchiyg( gfiles );
    gfiles.WriteToAllFile( "</BODY></HTML>");

    return 0;
}


int DinamicReport_vetvi(
    string project_name,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles)
{

    int num_html_file= sqlAdapterForDinVetvi::getSize()/ mod_for_html_file + 1; // столько html файлов будем создавать

    GroupReportFile gfiles( num_html_file);
    QString file_name=QString( base_file_name.c_str() ) + "%1.html";
    gfiles.OpenFiles( file_name, path.c_str());

    for( int i=0; i<gfiles.Size(); i++)
    {
        lsProcessedFiles.push_back( gfiles.GetFilePath( i).toStdString() );
    }

    gfiles.WriteToAllFile( QString( "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );
    gfiles.WriteToAllFile( QString( getCSS().c_str() ) );
    gfiles.WriteToAllFile( QString( "</HEAD>\n<BODY>") );
    zagolovok_html_gfile(  gfiles, QObject::tr("OTCHET PO DINAMICHESKOMY ANALIZY(VETVI)").toStdString().c_str(), project_name, version); // ОТЧЕТ ПО ДИНАМИЧЕСКОМУ АНАЛИЗУ(ВЕТВИ)
    silki_na_sleduiuchiyg( gfiles);
    sqlAdapterForDinVetvi dlnk_tmp;
    gfiles.WriteToAllFile( "<br><table class=\"content\" border=1 align=\"center\"> \n");
    gfiles.WriteToAllFile(
        html_tr(
            html_th( "№") +
            html_th( QObject::tr("Nazvanie obiekta").toStdString().c_str()) +   // Название объекта
            html_th( QObject::tr("Fail").toStdString().c_str()) +               // Файл
            html_th( QObject::tr("Stroka").toStdString().c_str())               // Строка
        ).c_str()
    );

    gfiles.WriteToAllFile( "\n");

    for( int i=0, t=0; i < gfiles.Size() ; i++)
    {
        for( int j=0,  k=0;  j < mod_for_html_file && dlnk_tmp.next() ; j++, t++)
        {
            k= j + i*mod_for_html_file;
            gfiles.GetFile( i) << "<tr class=\"myeen\">"
                               << (html_th( inttostring( t+1)) +
                                   html_td( dlnk_tmp.getFullName().toStdString() )+
                                   html_td( dlnk_tmp.getFileName().toStdString() ) +
                                   html_td( QString::number(dlnk_tmp.getStartPos()).toAscii())
                                  ).c_str()
                               ;
            gfiles.GetFile( i) << "</TR>" << endl;
        }
    }

    gfiles.WriteToAllFile( "</TABLE><br>\n" );
    silki_na_sleduiuchiyg( gfiles );
    gfiles.WriteToAllFile( "</BODY></HTML>" );


    return 0;
}

