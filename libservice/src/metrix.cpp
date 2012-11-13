
#include "libservice.h"
#include "htmlfunction.h"
#include "metrix.h"

#include <QString>
#include <QList>
#include <iostream>
#include <QFile>
#include <QTextStream>


struct Metrix_obj
{
    Metrix_obj():name(""), result(""), metrix("") {}
    Metrix_obj(QString n, QString r, QString m):name(n), result(r), metrix(m) {}
    Metrix_obj(char * n, char * r, char * m):name(n), result(r), metrix(m) {}
    QString name;
    QString result;
    QString metrix;
};

QString text_tmp_str;
void f_text_row(const Metrix_obj& obj)
{
    text_tmp_str+=alex::html_tr(
                      alex::html_td( obj.name.toStdString().c_str())+
                      alex::html_td( obj.result.toStdString().c_str())+
                      alex::html_td( obj.metrix.toStdString().c_str())
                  ).c_str();
}

void MetrixReport( sReportInfo* pa)
{
    // открываем файл
    string name= pa->strBaseFileName+".html";
    QString path=(pa->strOutputDirectory + "\\" + name ).c_str();
    pa->lsProcessedFiles->push_back( path.toStdString() );

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file);

    QList< Metrix_obj> metrix;
    FilesInformation fi;


    QString str_num= QObject::tr("scht."   ).toStdString().c_str();
    QString str_str= QObject::tr("strok"   ).toStdString().c_str();
    QString str_byte=QObject::tr("bait"    ).toStdString().c_str();
    QString str_symb=QObject::tr("simvolov").toStdString().c_str();

    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo failov v proekte").toStdString().c_str(),  QString::number( statistic::GetFileNum()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo naidennih potencialno opasnih konstrykcii").toStdString().c_str(), QString::number(statistic::GetNumPOK()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo failov s potencialno opasnimi ronstrykciami").toStdString().c_str(), QString::number(statistic::GetNumFileWithPOK()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Srednee kolichestvo obiektov v faile").toStdString().c_str(),QString::number(statistic::GetAvgNumScopeInAllFile()), str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo FO v proekte (E)").toStdString().c_str(), QString::number( statistic::GetNumFunc()),str_num ));
    int cont_link=0;
    int info_link=0;
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo sviazei FO po ypravleniy (C)").toStdString().c_str(),QString::number(cont_link=statistic::GetNumContLink()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo sviazei FO po informacii (I)").toStdString().c_str(),QString::number(info_link=statistic::GetNumInfoLink()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo sviazei FO (V=C+I)").toStdString().c_str(),QString::number(cont_link+info_link),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo \“visiaschih\” FO v proekte").toStdString().c_str(), QString::number( statistic::GetNumFlowingFunc()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo IO v proekte").toStdString().c_str(), QString::number( statistic::GetNumInfoObj()),str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo vetvei koda v proekte").toStdString().c_str(), QString::number( statistic::GetNumCodeVetveiInProject()),str_num ));

    //metrix.push_back( Metrix_obj("Количество операторов кода",QString::number(0)));

    metrix.push_back( Metrix_obj(QObject::tr("Ciklomaticheskaia slojnost koda po fynkciam (po Makkeiby, V-E)").toStdString().c_str(),QString::number(statistic::CiclomaticComplexityMakkeyb())," " ));
    metrix.push_back( Metrix_obj(QObject::tr("Razmer samoi dlinnoi stroki").toStdString().c_str(), QString::number( fi.max_file_char_in_line), str_symb ));
    metrix.push_back( Metrix_obj(QObject::tr("Srelnii razmer faila").toStdString().c_str(), QString::number( fi.avg_file_line), str_str ));
    metrix.push_back( Metrix_obj(QObject::tr("Razmer samogo bolshogo faila").toStdString().c_str(), QString::number( fi.max_file_size)+"/"+QString::number( fi.max_lines ), str_byte+"/"+str_str ));
    metrix.push_back( Metrix_obj(QObject::tr("Razmer samogo malenkogo faila").toStdString().c_str(), QString::number( fi.min_file_size ), str_byte ));
    metrix.push_back( Metrix_obj(QObject::tr("Srednii rasmer faila").toStdString().c_str(), QString::number( fi.avg_file_size), str_byte ));
    metrix.push_back( Metrix_obj(QObject::tr("Obiem ishodnih tekstov").toStdString().c_str(), QString::number( fi.size_all_file_in_project), str_byte));
    metrix.push_back( Metrix_obj(QObject::tr("Kolichestvo strok v proekte").toStdString().c_str(), QString::number( fi.num_all_lines_in_project), str_num));

    //metrix.push_back( Metrix_obj("Размер всего проекта", QString::number(fi.size_all_file_in_project)));
//    metrix.push_back( Metrix_obj("Среднее количество объектов в файле",QString::number(statistic::GetAvgNumScopeInAllFile()), str_num ));
    metrix.push_back( Metrix_obj(QObject::tr("Iaziki, ispolzovannie v proekte:").toStdString().c_str(), statistic::GetNamesOfUsingLang(), " " ));

//  metrix.push_back( Metrix_obj("Загруженнсоть файлов объектами",QString::number(0)) );

    // распечатываем метрики в лог работы
    for(int i=0; i < metrix.size(); i++)
    {
        cout << metrix.value( i).name.toStdString() << " ";
        cout << metrix.value( i).result.toStdString() << " ";
        cout << metrix.value( i).metrix.toStdString() << endl;
    }

    text_tmp_str=alex::html_tr(
                     alex::html_th(QObject::tr("Metriki").toStdString().c_str())+
                     alex::html_th(QObject::tr("Znachenie").toStdString().c_str())+
                     alex::html_th(QObject::tr("Edinici izmerenia").toStdString().c_str())
                 ).c_str();
    for_each(metrix.begin(), metrix.end(), f_text_row);

    out << QString(alex::html_breaket("html","",
                                      getCSS() +
                                      alex::html_breaket("head","", alex::html_breaket("title","",QObject::tr("Metriki proekta").toStdString().c_str()).c_str())+
                                      alex::html_breaket("body","",

                                              make_zagolovok(QObject::tr("Metriki proekta").toStdString().c_str(), pa->strProjectName, ak_vs_db::get_program_name(), name)+
                                              alex::html_breaket("BR")+
                                              alex::html_breaket("table","class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",text_tmp_str.toStdString().c_str())
                                                        )).c_str())  << "\n";

    file.close();
}
