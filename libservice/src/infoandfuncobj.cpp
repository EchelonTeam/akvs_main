/*
 * infoandfuncobj.cpp
 *
 *  Created on: 17.12.2008
 *      Author: javamain
 */
#include "libservice.h"
#include "common_libservice.h"
#include "groupfiles.h"
#include <iomanip>
#include <QTime>
#include <QMap>
#include <qmath.h>
#include <math.h>
#include <QtGlobal>
bool sort_func(NameScope  a, NameScope  b)
{
    int cmp = a.file.compare( b.file ) ;

    if( cmp > 0 )
    {
        return false;
    }

    if( cmp < 0 )
    {
        return true;
    }

    if( cmp == 0 )
    {
        return  a.start_pos  >  b.start_pos;
    }

}

HRET FilePrint_func_object(sReportInfo* pa)
{
    QTime delta_time;
    delta_time.start();
    cout << QObject::tr("(log):Sozdanie otcheta po FO i visiaschim FO.").toStdString().c_str()<< endl;  // (log):Создание отчета по ФО и висящим ФО.
    //cout << "Making func and flow func object report..." << endl;
//обновилось

    QORMList< sScopeFull> allscope ( EXE_SCOPE );
    QORMList< sScopeFull>::iterator it = allscope.begin();

    vector< NameScope> vs1;
    vector< NameScope> vs_flow;
    NameScope d;
    int num=0;

    for( int i=0; it != allscope.end(); ++it, i++ )
    {
        num++;
        d.s=*it;
        d.name=(*it).getQualifiedName2();
        d.file=(*it).file_path;
        //d.start_pos=(*it).pos_x1;

        vs1.push_back( d ) ;
        // сортируем
        //sort(vs1.begin(), vs1.end(), sort_func);

        //cout << i++ << endl;
        switch( ak_vs_db::get_scope_metadata(0, "chxInvertFlowFunc" ))
        {
        case true:
            if( !(*it).isFlowing())
                vs_flow.push_back(d) ;
            break;
        case false:
            if( (*it).isFlowing())
                vs_flow.push_back(d);
            break;
        }
    }

    //cout << "make report" << endl;
    cout << QObject::tr("Sozdanie otcheta.").toStdString().c_str()<< endl; // Создание отчета
    if( FilePrint_func_object( vs1, vs_flow, pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"1.", pa->strOutputDirectory, 50000, 50000, *(pa->lsProcessedFiles) )==-1)return -1;
    if( FilePrint_flowing_func_object( vs_flow, vs1, pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"2.",pa->strOutputDirectory, 50000, 50000, *(pa->lsProcessedFiles) )==-1)
    {
        return -1;
    }
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;  // Выполнено за сек.

    return 1;
}

HRET FilePrint_info_object( sReportInfo* pa)
{

    QTime delta_time;
    delta_time.start();

    cout << QObject::tr("(log):Sozdanie otcheta informacionnih obiektov.").toStdString().c_str() << endl; // Создание отчета информационных объектов.
            //"Making info object report..." << endl;

    vector< NameScope > vs2;
    QORMList< sScopeFull> tmp2 ( INFO_SCOPE);
    QORMList< sScopeFull>::iterator it= tmp2.begin();
    NameScope d;
    for(int i=0; it!=tmp2.end(); ++it, i++)
    {
        //cout << i << endl;
        d.name= ( *it).getQualifiedName2();
        d.s= ( *it);
        d.file= ( *it).file_path;
        vs2.push_back( d);
    }
    //cout << "создание отчета" << endl;
    if( FilePrint_info_object( vs2, vs2, pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"3.", pa->strOutputDirectory, 50000, 50000, *( pa->lsProcessedFiles) )==-1)
    {
        return -1;
    }
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl; // Выполнено за сек.

    return 1;
}


int FilePrint_func_object(
    vector< NameScope > &vs,
    vector< NameScope > &vs2,
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    int mod_for_txt_file,
    list< string> &lsProcessedFiles)
{
    using namespace alex;
    //>>>>>>>>>>>>>>>>>>>>>вывод HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //определение числа файлов

    int numstrok= vs.size(); //число элементов в векторе
    int num_html_file; //число html файлов необходимых для вывода строк
    int num_file_for_vs= vs.size()/mod_for_html_file+1;
    num_html_file= numstrok/mod_for_html_file+1; // столько html файлов будем создавать
    //создадим numfile число имен файла

    GroupReportFile gfiles( num_html_file);
    gfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.html", path.c_str(), lsProcessedFiles);
    gfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );
    gfiles.WriteToAllFile( QString( getCSS().c_str() ) );
    gfiles.WriteToAllFile( QString( "</HEAD>\n<BODY>") );

    //открываем html файлы

    zagolovok_html_gfile( gfiles, QObject::tr("OTCHET PO FYNKCIONALNIM OBIEKTAM").toStdString().c_str(), project_name, version); // ОТЧЕТ ПО ФУНКЦИОНАЛЬНЫМ ОБЪЕКТАМ


    // считаем покрытие висящих функций
    double pokritie= (vs.size() != 0) ? (100.0*vs2.size())/vs.size() : 0 ;
    QString pokr= (pokritie > 0) ? QString::number( qRound( pokritie ) ):
                  QObject::tr("mejdy 1% i 0").toStdString().c_str(); // между 1% и 0

    gfiles.WriteToAllFile( QString( html_breaket( "h5", "", string()+QObject::tr("Procientnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str()+pokr.toStdString()+"%").c_str()) ); // Процентное соотношение висящих функций к общему числу функций:



    silki_na_sleduiuchiyg(gfiles);



    gfiles.WriteToAllFile(QString( html_breaket( "H3", "ALIGN=\"center\"",QObject::tr("Perechen fynkcionalnih obiektov").toStdString().c_str()).c_str())); // Перечень функциональных объектов
    gfiles.WriteToAllFile(QString( html_breaket( "TABLE", "class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",
                                   html_tr(
                                       html_th( QObject::tr("Chislo fynkcionalnih obiektov").toStdString().c_str())+ // Число функциональных объектов
                                       html_th( inttostring( vs.size()) )
                                   )
                                               ).c_str()
                                 ));
    gfiles.WriteToAllFile(QString("<BR>"));



    gfiles.WriteToAllFile(QString( "<TABLE class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1>"));
    gfiles.WriteToAllFile(QString( html_tr(
                                       html_th("№")+
                                       html_th(QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str())+ // Имя функционального объекта
                                       html_th(QObject::tr("Fail raspolojenia FO").toStdString().c_str()) // Файл расположения ФО
                                   ).c_str()));


    //пишем содержимое вектора в html файлы
    for(int i=0; i<gfiles.Size(); i++)
    {
        //char a[10];
        int k=0;
        for(int j=0; j<mod_for_html_file; j++)
        {
            k=j+i*mod_for_html_file;
            if( size_t( k ) >= vs.size() )
            {
                gfiles.GetFile(i) << html_tr(
                                      html_th("")+

                                      html_th(QObject::tr("Chislo fynkcionalnih obiektov:").toStdString().c_str())+ // Число функциональных объектов:
                                      html_th(inttostring(vs.size()))
                                  )<<endl;
                break;
            }
            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k ].s.id ) , "args" , &ss1);

            gfiles.GetFile( i ) << html_tr(
                                    html_th( QString::number( k+1 ).toStdString().c_str())+
                                    html_td( vs.at( k ).name+ss1)+
                                    html_td( vs.at( k ).file )
                                )<<endl;
        }
    }

    gfiles.WriteToAllFile(QString("</TABLE><br>\n"));


    //здесь можно вставить в html и txt файлы все что угодно после таблицы
    //вставляем ссылку в каждый htlm файл на предыдущий файл и на последующий
//вставляем ссылку в каждый htlm файл на предыдущий файл и на последующий
    //вставляем ссылку в каждый htlm файл на предыдущий файл и на последующий

    silki_na_sleduiuchiyg(gfiles);



    gfiles.WriteToAllFile(QString("</BODY></HTML>"));

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>конец вывода HTML <<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>вывод в TXT файл <<<<<<<<<<<<<<<<<<<<<<<<<<<
    int num_txt_file; //число txt файлов необходимых для вывода строк
    num_txt_file= numstrok/mod_for_txt_file+1;  //столько txt файлов будем создавать
    //создадим numfile число имен файла
    //зададим имя txt файлов в строках.

    GroupReportFile gtextfiles( num_txt_file );
    gtextfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.txt", path.c_str(), lsProcessedFiles);





    //пишем заголовки в txt файлы
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================PERECHEN FYNKCHIONALNIH OBIEKTOV==========================================================================================\n").toStdString().c_str())); // =====================ПЕРЕЧЕНЬ ФУНКЦИОНАЛЬНЫХ ОБЪЕКТОВ==========================================================================================\n
    gtextfiles.WriteToAllFile(QString((QObject::tr("Proiekt: ").toStdString().c_str() + project_name+"\n").c_str() )); // Проект:
    gtextfiles.WriteToAllFile(QString(version.c_str())+"\n");

    for(int i=0; i< gtextfiles.Size(); i++)
        gtextfiles.GetFile( i) << QObject::tr("Imia faila: ").toStdString().c_str() << gtextfiles.GetFileName( i).toStdString() << endl; // Имя файла:

    gtextfiles.WriteToAllFile(QString(QObject::tr("Chislo fynkcionalnih obiektov: ").toStdString().c_str() + QString::number(vs.size()) +"\n") ); //Число функциональных объектов:
    for(int i=0; i< gtextfiles.Size(); i++)
        gtextfiles.GetFile( i) << QObject::tr("Fail ").toStdString().c_str()<< QString::number(i+1).toStdString() << QObject::tr(" is ").toStdString().c_str()<< QString::number(num_txt_file ).toStdString() << "\n"; // Файл  из

    // тут вставим процент висящих к общим.
    gtextfiles.WriteToAllFile( QString()+QObject::tr("Procentnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str()+pokr+"%\n"); // Процентное соотношение висящих функций к общему числу функций:
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================SPISOK FYNKCIONALNIH OBIEKTOV============================================================================================\n").toStdString().c_str())); // =====================СПИСОК ФУНКЦИОНАЛЬНЫХ ОБЪЕКТОВ============================================================================================\n

    for(int i=0; i< gtextfiles.Size(); i++)
        gtextfiles.GetFile( i)<< left << setw( NUMBER_STR_LENGTH) << "№" << "   " << left<< setw( NAME_STR_LENTH_FOR_ELEMENT) << QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str() << "   " << setw( NAME_STR_LENTH_FOR_ELEMENT) << left << QObject::tr("Pyt' faila, soderjaschego FO").toStdString().c_str() << "   " << endl; // Имя функционального объекта

    gtextfiles.WriteToAllFile(QString("===============================================================================================================================================\n"));



    //пишем содержимое вектора в txt файлы
    for( int i=0; i<num_txt_file; i++)
    {

        int k;
        for( int j=0; j<mod_for_txt_file; j++)
        {
            k= j+i*mod_for_txt_file;
            if( size_t( k)>= vs.size())
            {
                gtextfiles.GetFile( i) << "==============================================================================================================================================="<<std::endl;
                gtextfiles.GetFile( i) << QObject::tr("Chislo fynkchionalnih obiektov: ").toStdString().c_str()<<vs.size()<<"\n"; // Число функциональных объектов:
                break;
            }

            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k ].s.id), "args", &ss1);
            gtextfiles.GetFile( i) << left << setw( NUMBER_STR_LENGTH) << QString::number(j+i*mod_for_txt_file+1).toStdString() << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << ( vs[k].name+ss1) << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << vs.at( k).file << endl;
        }
    }
//заканчиваем таблицы для txt файлов
    //закрываем все txt файлы

//>>>>>>>>>>>>>>>>>>>>> конец вывода в TXT файл <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}


// функция выводит в файл все содержание vs
// vs                - вектор содержит массив строк названий информационного объекта объекта
// project_name		 - имя проекта
// base_file_name    - имя отчета он будет индексироваться и будте два варианта txt и html обязательно английское!!!!!
// название идет типа "мой файл" функция сама к ниму добавит индекс и расшерения "txt" и "html"
// mod_for_html_file - количество строчек в html файле
// mod_for_txt_file  - количество строчек в txt файле
int FilePrint_info_object(vector< NameScope > &vs,
                          vector< NameScope > &vs2,
                          string project_name ,string version,
                          string base_file_name,string path,
                          int mod_for_html_file,int mod_for_txt_file ,list <string> & lsProcessedFiles)
{

    //>>>>>>>>>>>>>>>>>>>>>вывод HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //определение числа файлов
    int numstrok=vs.size(); //число элементов в векторе
    int num_html_file; //число html файлов необходимых для вывода строк
    int num_file_for_vs=vs.size()/mod_for_html_file+1;
    num_html_file= numstrok/mod_for_html_file+1; // столько html файлов будем создавать

    GroupReportFile gfiles( num_html_file);
    gfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.html", path.c_str(), lsProcessedFiles);
    gfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );

    //записываем заголовок в каждый html файл
    gfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );
    gfiles.WriteToAllFile( QString(getCSS().c_str()) );
    gfiles.WriteToAllFile( QString("</HEAD>\n<BODY>"));

    //вставляем заголовок HTML файлов
    zagolovok_html_gfile( gfiles, QObject::tr("OTCHET PO INFORMACIONNIM OBIEKTAM").toStdString().c_str(), project_name, version); // ОТЧЕТ ПО ИНФОРМАЦИОННЫМ ОБЪЕКТАМ

    silki_na_sleduiuchiyg(gfiles);

    gfiles.WriteToAllFile( QString(html_breaket("H3","ALIGN=\"center\"",QObject::tr("Perechen informacionnih obiektov").toStdString().c_str()).c_str())); // Перечень информационных объектов
    gfiles.WriteToAllFile( QString(html_breaket("TABLE","class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",
                                   html_tr(
                                       html_th(QObject::tr("Chislo informacionnih obiektov").toStdString().c_str())+ // Число информационных объектов
                                       html_th(inttostring(vs.size() ))
                                   )
                                               ).c_str()
                                  ));
    gfiles.WriteToAllFile( QString("<BR>"));

    //вставляем в html файл текст первых строк в таблицы

    gfiles.WriteToAllFile( QString("<TABLE class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1>"));
    gfiles.WriteToAllFile( QString(html_tr(
                                       html_th("№")+
                                       html_th(QObject::tr("Imia informacionnogo obiekta").toStdString().c_str())+ // Имя информационного объекта
                                       html_th(QObject::tr("Fail raspolojenia IO").toStdString().c_str()) // Файл расположения ИО
                                   ).c_str()));

    //пишем содержимое вектора в html файлы
    for( int i=0; i < gfiles.Size() ; i++)
    {
        int k=0;
        for( int j=0; j<mod_for_html_file; j++)
        {
            k=j+i*mod_for_html_file;
            if(size_t(k)>=vs.size())
            {
                gfiles.GetFile( i)  << html_tr(
                                        html_th("")+
                                        html_th(QObject::tr("Chislo informacionnih obiektov: ").toStdString().c_str())+ //Число информационных объектов:
                                        html_th(inttostring(vs.size()))
                                    );
                gfiles.GetFile( i) << endl;

                break;
            }
            gfiles.GetFile( i ) << html_tr(
                                    html_th( QString::number( k+1 ).toStdString())+
                                    html_td( vs[ k ].name)+
                                    html_td( vs[ k ].file)
                                );

        }
    }

    gfiles.WriteToAllFile(QString("</TABLE><br>\n"));

    silki_na_sleduiuchiyg(gfiles);
    //здесь вставляем концы htlm документов
    gfiles.WriteToAllFile( QString( "</BODY></HTML>") );


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>конец вывода HTML <<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>вывод в TXT файл <<<<<<<<<<<<<<<<<<<<<<<<<<<
    int num_txt_file; //число txt файлов необходимых для вывода строк
    num_txt_file= numstrok/mod_for_txt_file+1;  //столько txt файлов будем создавать

    GroupReportFile gtextfiles( num_txt_file );
    gtextfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.txt", path.c_str(), lsProcessedFiles);




    //пишем заголовки в txt файлы
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================PERECHEN INFORMACIONNIH OBIEKTOV==========================================================================================\n").toStdString().c_str())); // =====================ПЕРЕЧЕНЬ ИНФОРМАЦИОННЫХ ОБЪЕКТОВ==========================================================================================\n
    gtextfiles.WriteToAllFile(QString((QObject::tr("Proekt: ").toStdString().c_str() + project_name).c_str() )); // Проект:
    gtextfiles.WriteToAllFile(QString(version.c_str())+"\n");


    for( int i=0; i < gtextfiles.Size(); i++)
        gtextfiles.GetFile(i) << QObject::tr("Imia faila: ").toStdString().c_str() << gtextfiles.GetFileName(i).toStdString() << endl; // Имя файла:

    gtextfiles.WriteToAllFile( QString( QObject::tr("Chislo informacionnih obiektov: ").toStdString().c_str() + QString::number(vs.size()) +"\n")); // Число информационных объектов:

    for( int i=0; i < gtextfiles.Size(); i++)
        gtextfiles.GetFile( i ) <<QObject::tr("Fail ").toStdString().c_str() << i+1 << QObject::tr(" is ").toStdString().c_str() << num_txt_file  << endl; // Файл   из
    gtextfiles.WriteToAllFile(QString( QObject::tr("=====================SPISOK INFORMACIONNIH OBIEKTOV============================================================================================\n").toStdString().c_str())); // =====================СПИСОК ИНФОРМАЦИОННЫХ ОБЪЕКТОВ============================================================================================\n

    for( int i=0; i < gtextfiles.Size(); i++)
        gtextfiles.GetFile( i ) << left << setw( NUMBER_STR_LENGTH) << "№" << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << QObject::tr( "Imia informacionnogo obiekta" ).toStdString().c_str() << "   " << setw( NAME_STR_LENTH_FOR_ELEMENT) << left << QObject::tr("Pyt' faila, soderjaschego IO").toStdString().c_str() << "   " << endl; // Путь файла, содержащего ИО

    gtextfiles.WriteToAllFile( QString("===============================================================================================================================================\n"));

    //пишем содержимое вектора в txt файлы
    for( int i=0; i < gtextfiles.Size() ; i++)
    {
        //char a[ 10];
        int k;
        for( int j=0; j<mod_for_txt_file; j++)
        {
            k= j+i*mod_for_txt_file;
            if( size_t( k) >= vs.size())
            {
                gtextfiles.GetFile( i) << "===============================================================================================================================================" << endl;
                gtextfiles.GetFile( i) << QObject::tr("Chislo informacionnih obiektov: ").toStdString().c_str() << vs.size() << "\n"; // Число информационных объектов:
                break;
            }
            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k].s.id), "args", &ss1);
            gtextfiles.GetFile( i) << left << setw( NUMBER_STR_LENGTH) << QString::number( j+i*mod_for_txt_file+1 ).toStdString()  << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << vs[ k].name << ss1 << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << vs[ k].file << endl;
        }
    }
//>>>>>>>>>>>>>>>>>>>>> конец вывода в TXT файл <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}



int FilePrint_flowing_func_object( vector< NameScope> &vs,
                                   vector< NameScope> &vs2,
                                   string project_name,
                                   string version,
                                   string base_file_name,
                                   string path,
                                   int mod_for_html_file,
                                   int mod_for_txt_file,
                                   list < string> & lsProcessedFiles
                                 )
{
    //>>>>>>>>>>>>>>>>>>>>>вывод HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //определение числа файлов
    int numstrok= vs.size(); //число элементов в векторе
    int num_html_file; //число html файлов необходимых для вывода строк
    int num_file_for_vs= vs.size()/mod_for_html_file+1;
    num_html_file= numstrok/mod_for_html_file+1; // столько html файлов будем создавать
    //создадим numfile число имен файла

    GroupReportFile ghtmlfiles( num_html_file );
    ghtmlfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.html", path.c_str(), lsProcessedFiles);

    ghtmlfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n"));
    ghtmlfiles.WriteToAllFile( QString(getCSS().c_str()));
    ghtmlfiles.WriteToAllFile( QString("</HEAD>\n<BODY>"));
    //вставляем заголовок HTML файлов
    zagolovok_html_gfile( ghtmlfiles, QObject::tr("OTCHET PO IZBITOCHNIM FYNKCIONALNIM OBIEKTAM").toStdString().c_str(), project_name, version); // ОТЧЕТ ПО ИЗБЫТОЧНЫМ ФУНКЦИОНАЛЬНЫМ ОБЪЕКТАМ


    double pokritie=( vs2.size() != 0)?( 100.0*vs.size())/ vs2.size(): 0;
    QString pokr= ( pokritie >0)? QString::number( qRound( pokritie)):
                  QObject::tr("mejdy 1% i 0").toStdString().c_str(); // между 1% и 0

    ghtmlfiles.WriteToAllFile(QString( html_breaket( "h5", "", string()+QObject::tr("Procientnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str()+pokr.toStdString()+"%\n").c_str()) ); //Процентное соотношение висящих функций к общему числу функций:
    silki_na_sleduiuchiyg(ghtmlfiles);
    //вставляем в html файл текст заголовка таблицы
    ghtmlfiles.WriteToAllFile(QString ( html_breaket( "H3","ALIGN=\"center\"",QObject::tr("Perechen' visiaschih fynkcionalnih obiektov").toStdString()).c_str()) );
    ghtmlfiles.WriteToAllFile(QString (html_breaket( "TABLE","class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",
                                       html_tr(
                                           html_th( QObject::tr("Chislo visiaschih fynkcionalnih jbiektov").toStdString().c_str())+ // Число висящих функциональных объектов
                                           html_th( inttostring(vs.size()) )
                                       )
                                                   ).c_str()) );
    ghtmlfiles.WriteToAllFile("<BR>\n");
    ghtmlfiles.WriteToAllFile( QString("<TABLE class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1>"));
    ghtmlfiles.WriteToAllFile( QString(html_tr(
                                           html_th("№")+
                                           html_th(QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str())+ // Имя функционального объекта
                                           html_th(QObject::tr("Fail raspolojenia FO").toStdString().c_str()) // Файл расположения ФО
                                       ).c_str()));
    ghtmlfiles.WriteToAllFile( QString("\n"));

    for( int i=0; i < ghtmlfiles.Size() ; i++)
    {

        int k=0;
        for( int j=0; j < mod_for_html_file; j++)
        {
            k=j+i*mod_for_html_file;
            if( size_t( k) >= vs.size())
            {
                ghtmlfiles.GetFile( i ) << html_tr(
                                            html_th( "" )+
                                            html_th( QObject::tr("Chislo visiaschih fynkcionalnih obiektov: " ).toStdString().c_str())+ // Число висящих функциональных объектов:
                                            html_th( inttostring( vs.size() ) )
                                        );
                ghtmlfiles.GetFile( i) << endl;
                break;
            }
            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k].s.id), "args", &ss1);
            ghtmlfiles.GetFile( i)<<html_tr(
                                      html_th( inttostring( k+1))+
                                      html_td( vs[ k].name+ss1)+
                                      html_td( vs[ k].file)
                                  );
            ghtmlfiles.GetFile( i) << endl;
        }
    }
    ghtmlfiles.WriteToAllFile(QString("</TABLE><br>\n"));
    silki_na_sleduiuchiyg( ghtmlfiles);
    //здесь вставляем концы html документов
    ghtmlfiles.WriteToAllFile( QString("</BODY></HTML>"));
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>конец вывода HTML <<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>вывод в TXT файл <<<<<<<<<<<<<<<<<<<<<<<<<<<
    int num_txt_file; //число txt файлов необходимых для вывода строк
    num_txt_file= numstrok/mod_for_txt_file+1;  //столько txt файлов будем создавать
    //создадим numfile число имен файла
    GroupReportFile gtextfiles( num_txt_file );
    gtextfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.txt", path.c_str(), lsProcessedFiles);
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================PERECHEN IZBITOCHNIH FYNKCIONALNIH OBIEKTOV==========================================================================================\n").toStdString().c_str())); //=====================ПЕРЕЧЕНЬ ИЗБЫТОЧНЫХ ФУНКЦИОНАЛЬНЫХ ОБЪЕКТОВ==========================================================================================\n
    gtextfiles.WriteToAllFile(QString(QObject::tr("Proiekt: ").toStdString().c_str()) + project_name.c_str() + "\n"); // Проект:
    gtextfiles.WriteToAllFile(QString(version.c_str())+"\n");
    for(int i=0; i < gtextfiles.Size();  i++)
        gtextfiles.GetFile( i) << QObject::tr("Imia faila: ").toStdString().c_str() << gtextfiles.GetFileName(i).toStdString() << std::endl; // Имя файла:
    gtextfiles.WriteToAllFile(QString( QObject::tr("Chislo visiaschih fynkcii: ").toStdString().c_str() + QString::number(vs.size())+"\n")); // Число висящих функций:
    for(int i=0; i < gtextfiles.Size();  i++)
        gtextfiles.GetFile( i) << QObject::tr("Fail ").toStdString().c_str() << i+1 << QObject::tr(" is ").toStdString().c_str() << num_txt_file << std::endl; // Файл  из
    gtextfiles.WriteToAllFile(QString(QObject::tr("Procientnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str())+pokr+"%\n");
    gtextfiles.WriteToAllFile(QString( QObject::tr("=====================SPISOK VISIASCHIH FYNKCIONALNIH OBIEKTOV============================================================================================\n" ).toStdString().c_str())); // =====================СПИСОК ВИСЯЩИХ ФУНКЦИОНАЛЬНЫХ ОБЪЕКТОВ============================================================================================\n
    for(int i=0; i < gtextfiles.Size();  i++)
        gtextfiles.GetFile( i) << left << setw( NUMBER_STR_LENGTH) << "№" << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str() << "   " << setw( NAME_STR_LENTH_FOR_ELEMENT)<<std::left<<QObject::tr("Pyt' faila, soderjaschego FO").toStdString().c_str()<<"   "<<std::endl; // Путь файла, содержащего ФО
    gtextfiles.WriteToAllFile(QString( "====================================================================================================================================================\n"));
    //пишем содержимое вектора в txt файлы
    for( int i=0; i < gtextfiles.Size() ; i++)
    {
        int k;
        for( int j=0; j < mod_for_txt_file; j++)
        {
            k= j+i*mod_for_txt_file;
            if( size_t( k ) >= vs.size() )
            {
                gtextfiles.GetFile( i) << "===================================================================================================================================================="<<std::endl;
                gtextfiles.GetFile( i) << QObject::tr("Chislo visiaschih fynkchionalnih obiektov: ").toStdString().c_str() << vs.size() << "\n"; // Число висящих функциональных объектов:
                break;
            }
            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k].s.id), "args", &ss1);
            gtextfiles.GetFile( i) << std::left << std::setw( NUMBER_STR_LENGTH) << QString::number(j+i*mod_for_txt_file+1).toStdString() << "   " << std::left << std::setw( NAME_STR_LENTH_FOR_ELEMENT) << ( vs[ k].name+ss1).c_str() << "   " << std::left << std::setw( NAME_STR_LENTH_FOR_ELEMENT) << vs[ k].file.c_str() << endl;
        }
    }
//>>>>>>>>>>>>>>>>>>>>> конец вывода в TXT файл <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}
