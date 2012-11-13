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
    cout << QObject::tr("(log):Sozdanie otcheta po FO i visiaschim FO.").toStdString().c_str()<< endl;  // (log):�������� ������ �� �� � ������� ��.
    //cout << "Making func and flow func object report..." << endl;
//����������

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
        // ���������
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
    cout << QObject::tr("Sozdanie otcheta.").toStdString().c_str()<< endl; // �������� ������
    if( FilePrint_func_object( vs1, vs_flow, pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"1.", pa->strOutputDirectory, 50000, 50000, *(pa->lsProcessedFiles) )==-1)return -1;
    if( FilePrint_flowing_func_object( vs_flow, vs1, pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"2.",pa->strOutputDirectory, 50000, 50000, *(pa->lsProcessedFiles) )==-1)
    {
        return -1;
    }
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;  // ��������� �� ���.

    return 1;
}

HRET FilePrint_info_object( sReportInfo* pa)
{

    QTime delta_time;
    delta_time.start();

    cout << QObject::tr("(log):Sozdanie otcheta informacionnih obiektov.").toStdString().c_str() << endl; // �������� ������ �������������� ��������.
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
    //cout << "�������� ������" << endl;
    if( FilePrint_info_object( vs2, vs2, pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"3.", pa->strOutputDirectory, 50000, 50000, *( pa->lsProcessedFiles) )==-1)
    {
        return -1;
    }
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl; // ��������� �� ���.

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
    //>>>>>>>>>>>>>>>>>>>>>����� HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //����������� ����� ������

    int numstrok= vs.size(); //����� ��������� � �������
    int num_html_file; //����� html ������ ����������� ��� ������ �����
    int num_file_for_vs= vs.size()/mod_for_html_file+1;
    num_html_file= numstrok/mod_for_html_file+1; // ������� html ������ ����� ���������
    //�������� numfile ����� ���� �����

    GroupReportFile gfiles( num_html_file);
    gfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.html", path.c_str(), lsProcessedFiles);
    gfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );
    gfiles.WriteToAllFile( QString( getCSS().c_str() ) );
    gfiles.WriteToAllFile( QString( "</HEAD>\n<BODY>") );

    //��������� html �����

    zagolovok_html_gfile( gfiles, QObject::tr("OTCHET PO FYNKCIONALNIM OBIEKTAM").toStdString().c_str(), project_name, version); // ����� �� �������������� ��������


    // ������� �������� ������� �������
    double pokritie= (vs.size() != 0) ? (100.0*vs2.size())/vs.size() : 0 ;
    QString pokr= (pokritie > 0) ? QString::number( qRound( pokritie ) ):
                  QObject::tr("mejdy 1% i 0").toStdString().c_str(); // ����� 1% � 0

    gfiles.WriteToAllFile( QString( html_breaket( "h5", "", string()+QObject::tr("Procientnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str()+pokr.toStdString()+"%").c_str()) ); // ���������� ����������� ������� ������� � ������ ����� �������:



    silki_na_sleduiuchiyg(gfiles);



    gfiles.WriteToAllFile(QString( html_breaket( "H3", "ALIGN=\"center\"",QObject::tr("Perechen fynkcionalnih obiektov").toStdString().c_str()).c_str())); // �������� �������������� ��������
    gfiles.WriteToAllFile(QString( html_breaket( "TABLE", "class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",
                                   html_tr(
                                       html_th( QObject::tr("Chislo fynkcionalnih obiektov").toStdString().c_str())+ // ����� �������������� ��������
                                       html_th( inttostring( vs.size()) )
                                   )
                                               ).c_str()
                                 ));
    gfiles.WriteToAllFile(QString("<BR>"));



    gfiles.WriteToAllFile(QString( "<TABLE class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1>"));
    gfiles.WriteToAllFile(QString( html_tr(
                                       html_th("�")+
                                       html_th(QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str())+ // ��� ��������������� �������
                                       html_th(QObject::tr("Fail raspolojenia FO").toStdString().c_str()) // ���� ������������ ��
                                   ).c_str()));


    //����� ���������� ������� � html �����
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

                                      html_th(QObject::tr("Chislo fynkcionalnih obiektov:").toStdString().c_str())+ // ����� �������������� ��������:
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


    //����� ����� �������� � html � txt ����� ��� ��� ������ ����� �������
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
//��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������

    silki_na_sleduiuchiyg(gfiles);



    gfiles.WriteToAllFile(QString("</BODY></HTML>"));

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>����� ������ HTML <<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>����� � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<
    int num_txt_file; //����� txt ������ ����������� ��� ������ �����
    num_txt_file= numstrok/mod_for_txt_file+1;  //������� txt ������ ����� ���������
    //�������� numfile ����� ���� �����
    //������� ��� txt ������ � �������.

    GroupReportFile gtextfiles( num_txt_file );
    gtextfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.txt", path.c_str(), lsProcessedFiles);





    //����� ��������� � txt �����
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================PERECHEN FYNKCHIONALNIH OBIEKTOV==========================================================================================\n").toStdString().c_str())); // =====================�������� �������������� ��������==========================================================================================\n
    gtextfiles.WriteToAllFile(QString((QObject::tr("Proiekt: ").toStdString().c_str() + project_name+"\n").c_str() )); // ������:
    gtextfiles.WriteToAllFile(QString(version.c_str())+"\n");

    for(int i=0; i< gtextfiles.Size(); i++)
        gtextfiles.GetFile( i) << QObject::tr("Imia faila: ").toStdString().c_str() << gtextfiles.GetFileName( i).toStdString() << endl; // ��� �����:

    gtextfiles.WriteToAllFile(QString(QObject::tr("Chislo fynkcionalnih obiektov: ").toStdString().c_str() + QString::number(vs.size()) +"\n") ); //����� �������������� ��������:
    for(int i=0; i< gtextfiles.Size(); i++)
        gtextfiles.GetFile( i) << QObject::tr("Fail ").toStdString().c_str()<< QString::number(i+1).toStdString() << QObject::tr(" is ").toStdString().c_str()<< QString::number(num_txt_file ).toStdString() << "\n"; // ����  ��

    // ��� ������� ������� ������� � �����.
    gtextfiles.WriteToAllFile( QString()+QObject::tr("Procentnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str()+pokr+"%\n"); // ���������� ����������� ������� ������� � ������ ����� �������:
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================SPISOK FYNKCIONALNIH OBIEKTOV============================================================================================\n").toStdString().c_str())); // =====================������ �������������� ��������============================================================================================\n

    for(int i=0; i< gtextfiles.Size(); i++)
        gtextfiles.GetFile( i)<< left << setw( NUMBER_STR_LENGTH) << "�" << "   " << left<< setw( NAME_STR_LENTH_FOR_ELEMENT) << QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str() << "   " << setw( NAME_STR_LENTH_FOR_ELEMENT) << left << QObject::tr("Pyt' faila, soderjaschego FO").toStdString().c_str() << "   " << endl; // ��� ��������������� �������

    gtextfiles.WriteToAllFile(QString("===============================================================================================================================================\n"));



    //����� ���������� ������� � txt �����
    for( int i=0; i<num_txt_file; i++)
    {

        int k;
        for( int j=0; j<mod_for_txt_file; j++)
        {
            k= j+i*mod_for_txt_file;
            if( size_t( k)>= vs.size())
            {
                gtextfiles.GetFile( i) << "==============================================================================================================================================="<<std::endl;
                gtextfiles.GetFile( i) << QObject::tr("Chislo fynkchionalnih obiektov: ").toStdString().c_str()<<vs.size()<<"\n"; // ����� �������������� ��������:
                break;
            }

            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k ].s.id), "args", &ss1);
            gtextfiles.GetFile( i) << left << setw( NUMBER_STR_LENGTH) << QString::number(j+i*mod_for_txt_file+1).toStdString() << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << ( vs[k].name+ss1) << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << vs.at( k).file << endl;
        }
    }
//����������� ������� ��� txt ������
    //��������� ��� txt �����

//>>>>>>>>>>>>>>>>>>>>> ����� ������ � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}


// ������� ������� � ���� ��� ���������� vs
// vs                - ������ �������� ������ ����� �������� ��������������� ������� �������
// project_name		 - ��� �������
// base_file_name    - ��� ������ �� ����� ��������������� � ����� ��� �������� txt � html ����������� ����������!!!!!
// �������� ���� ���� "��� ����" ������� ���� � ���� ������� ������ � ���������� "txt" � "html"
// mod_for_html_file - ���������� ������� � html �����
// mod_for_txt_file  - ���������� ������� � txt �����
int FilePrint_info_object(vector< NameScope > &vs,
                          vector< NameScope > &vs2,
                          string project_name ,string version,
                          string base_file_name,string path,
                          int mod_for_html_file,int mod_for_txt_file ,list <string> & lsProcessedFiles)
{

    //>>>>>>>>>>>>>>>>>>>>>����� HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //����������� ����� ������
    int numstrok=vs.size(); //����� ��������� � �������
    int num_html_file; //����� html ������ ����������� ��� ������ �����
    int num_file_for_vs=vs.size()/mod_for_html_file+1;
    num_html_file= numstrok/mod_for_html_file+1; // ������� html ������ ����� ���������

    GroupReportFile gfiles( num_html_file);
    gfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.html", path.c_str(), lsProcessedFiles);
    gfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );

    //���������� ��������� � ������ html ����
    gfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n") );
    gfiles.WriteToAllFile( QString(getCSS().c_str()) );
    gfiles.WriteToAllFile( QString("</HEAD>\n<BODY>"));

    //��������� ��������� HTML ������
    zagolovok_html_gfile( gfiles, QObject::tr("OTCHET PO INFORMACIONNIM OBIEKTAM").toStdString().c_str(), project_name, version); // ����� �� �������������� ��������

    silki_na_sleduiuchiyg(gfiles);

    gfiles.WriteToAllFile( QString(html_breaket("H3","ALIGN=\"center\"",QObject::tr("Perechen informacionnih obiektov").toStdString().c_str()).c_str())); // �������� �������������� ��������
    gfiles.WriteToAllFile( QString(html_breaket("TABLE","class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",
                                   html_tr(
                                       html_th(QObject::tr("Chislo informacionnih obiektov").toStdString().c_str())+ // ����� �������������� ��������
                                       html_th(inttostring(vs.size() ))
                                   )
                                               ).c_str()
                                  ));
    gfiles.WriteToAllFile( QString("<BR>"));

    //��������� � html ���� ����� ������ ����� � �������

    gfiles.WriteToAllFile( QString("<TABLE class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1>"));
    gfiles.WriteToAllFile( QString(html_tr(
                                       html_th("�")+
                                       html_th(QObject::tr("Imia informacionnogo obiekta").toStdString().c_str())+ // ��� ��������������� �������
                                       html_th(QObject::tr("Fail raspolojenia IO").toStdString().c_str()) // ���� ������������ ��
                                   ).c_str()));

    //����� ���������� ������� � html �����
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
                                        html_th(QObject::tr("Chislo informacionnih obiektov: ").toStdString().c_str())+ //����� �������������� ��������:
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
    //����� ��������� ����� htlm ����������
    gfiles.WriteToAllFile( QString( "</BODY></HTML>") );


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>����� ������ HTML <<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>����� � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<
    int num_txt_file; //����� txt ������ ����������� ��� ������ �����
    num_txt_file= numstrok/mod_for_txt_file+1;  //������� txt ������ ����� ���������

    GroupReportFile gtextfiles( num_txt_file );
    gtextfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.txt", path.c_str(), lsProcessedFiles);




    //����� ��������� � txt �����
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================PERECHEN INFORMACIONNIH OBIEKTOV==========================================================================================\n").toStdString().c_str())); // =====================�������� �������������� ��������==========================================================================================\n
    gtextfiles.WriteToAllFile(QString((QObject::tr("Proekt: ").toStdString().c_str() + project_name).c_str() )); // ������:
    gtextfiles.WriteToAllFile(QString(version.c_str())+"\n");


    for( int i=0; i < gtextfiles.Size(); i++)
        gtextfiles.GetFile(i) << QObject::tr("Imia faila: ").toStdString().c_str() << gtextfiles.GetFileName(i).toStdString() << endl; // ��� �����:

    gtextfiles.WriteToAllFile( QString( QObject::tr("Chislo informacionnih obiektov: ").toStdString().c_str() + QString::number(vs.size()) +"\n")); // ����� �������������� ��������:

    for( int i=0; i < gtextfiles.Size(); i++)
        gtextfiles.GetFile( i ) <<QObject::tr("Fail ").toStdString().c_str() << i+1 << QObject::tr(" is ").toStdString().c_str() << num_txt_file  << endl; // ����   ��
    gtextfiles.WriteToAllFile(QString( QObject::tr("=====================SPISOK INFORMACIONNIH OBIEKTOV============================================================================================\n").toStdString().c_str())); // =====================������ �������������� ��������============================================================================================\n

    for( int i=0; i < gtextfiles.Size(); i++)
        gtextfiles.GetFile( i ) << left << setw( NUMBER_STR_LENGTH) << "�" << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << QObject::tr( "Imia informacionnogo obiekta" ).toStdString().c_str() << "   " << setw( NAME_STR_LENTH_FOR_ELEMENT) << left << QObject::tr("Pyt' faila, soderjaschego IO").toStdString().c_str() << "   " << endl; // ���� �����, ����������� ��

    gtextfiles.WriteToAllFile( QString("===============================================================================================================================================\n"));

    //����� ���������� ������� � txt �����
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
                gtextfiles.GetFile( i) << QObject::tr("Chislo informacionnih obiektov: ").toStdString().c_str() << vs.size() << "\n"; // ����� �������������� ��������:
                break;
            }
            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k].s.id), "args", &ss1);
            gtextfiles.GetFile( i) << left << setw( NUMBER_STR_LENGTH) << QString::number( j+i*mod_for_txt_file+1 ).toStdString()  << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << vs[ k].name << ss1 << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << vs[ k].file << endl;
        }
    }
//>>>>>>>>>>>>>>>>>>>>> ����� ������ � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
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
    //>>>>>>>>>>>>>>>>>>>>>����� HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //����������� ����� ������
    int numstrok= vs.size(); //����� ��������� � �������
    int num_html_file; //����� html ������ ����������� ��� ������ �����
    int num_file_for_vs= vs.size()/mod_for_html_file+1;
    num_html_file= numstrok/mod_for_html_file+1; // ������� html ������ ����� ���������
    //�������� numfile ����� ���� �����

    GroupReportFile ghtmlfiles( num_html_file );
    ghtmlfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.html", path.c_str(), lsProcessedFiles);

    ghtmlfiles.WriteToAllFile( QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n"));
    ghtmlfiles.WriteToAllFile( QString(getCSS().c_str()));
    ghtmlfiles.WriteToAllFile( QString("</HEAD>\n<BODY>"));
    //��������� ��������� HTML ������
    zagolovok_html_gfile( ghtmlfiles, QObject::tr("OTCHET PO IZBITOCHNIM FYNKCIONALNIM OBIEKTAM").toStdString().c_str(), project_name, version); // ����� �� ���������� �������������� ��������


    double pokritie=( vs2.size() != 0)?( 100.0*vs.size())/ vs2.size(): 0;
    QString pokr= ( pokritie >0)? QString::number( qRound( pokritie)):
                  QObject::tr("mejdy 1% i 0").toStdString().c_str(); // ����� 1% � 0

    ghtmlfiles.WriteToAllFile(QString( html_breaket( "h5", "", string()+QObject::tr("Procientnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str()+pokr.toStdString()+"%\n").c_str()) ); //���������� ����������� ������� ������� � ������ ����� �������:
    silki_na_sleduiuchiyg(ghtmlfiles);
    //��������� � html ���� ����� ��������� �������
    ghtmlfiles.WriteToAllFile(QString ( html_breaket( "H3","ALIGN=\"center\"",QObject::tr("Perechen' visiaschih fynkcionalnih obiektov").toStdString()).c_str()) );
    ghtmlfiles.WriteToAllFile(QString (html_breaket( "TABLE","class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1",
                                       html_tr(
                                           html_th( QObject::tr("Chislo visiaschih fynkcionalnih jbiektov").toStdString().c_str())+ // ����� ������� �������������� ��������
                                           html_th( inttostring(vs.size()) )
                                       )
                                                   ).c_str()) );
    ghtmlfiles.WriteToAllFile("<BR>\n");
    ghtmlfiles.WriteToAllFile( QString("<TABLE class=\"mytable\" WIGHT=\"100%\" ALIGN=\"center\" border=1>"));
    ghtmlfiles.WriteToAllFile( QString(html_tr(
                                           html_th("�")+
                                           html_th(QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str())+ // ��� ��������������� �������
                                           html_th(QObject::tr("Fail raspolojenia FO").toStdString().c_str()) // ���� ������������ ��
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
                                            html_th( QObject::tr("Chislo visiaschih fynkcionalnih obiektov: " ).toStdString().c_str())+ // ����� ������� �������������� ��������:
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
    //����� ��������� ����� html ����������
    ghtmlfiles.WriteToAllFile( QString("</BODY></HTML>"));
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>����� ������ HTML <<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>����� � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<
    int num_txt_file; //����� txt ������ ����������� ��� ������ �����
    num_txt_file= numstrok/mod_for_txt_file+1;  //������� txt ������ ����� ���������
    //�������� numfile ����� ���� �����
    GroupReportFile gtextfiles( num_txt_file );
    gtextfiles.OpenFiles( QString( base_file_name.c_str() ) + "%1.txt", path.c_str(), lsProcessedFiles);
    gtextfiles.WriteToAllFile(QString(QObject::tr("=====================PERECHEN IZBITOCHNIH FYNKCIONALNIH OBIEKTOV==========================================================================================\n").toStdString().c_str())); //=====================�������� ���������� �������������� ��������==========================================================================================\n
    gtextfiles.WriteToAllFile(QString(QObject::tr("Proiekt: ").toStdString().c_str()) + project_name.c_str() + "\n"); // ������:
    gtextfiles.WriteToAllFile(QString(version.c_str())+"\n");
    for(int i=0; i < gtextfiles.Size();  i++)
        gtextfiles.GetFile( i) << QObject::tr("Imia faila: ").toStdString().c_str() << gtextfiles.GetFileName(i).toStdString() << std::endl; // ��� �����:
    gtextfiles.WriteToAllFile(QString( QObject::tr("Chislo visiaschih fynkcii: ").toStdString().c_str() + QString::number(vs.size())+"\n")); // ����� ������� �������:
    for(int i=0; i < gtextfiles.Size();  i++)
        gtextfiles.GetFile( i) << QObject::tr("Fail ").toStdString().c_str() << i+1 << QObject::tr(" is ").toStdString().c_str() << num_txt_file << std::endl; // ����  ��
    gtextfiles.WriteToAllFile(QString(QObject::tr("Procientnoe sootnoschenie visiaschih fynkcii k obschemy chisly fynkcii: ").toStdString().c_str())+pokr+"%\n");
    gtextfiles.WriteToAllFile(QString( QObject::tr("=====================SPISOK VISIASCHIH FYNKCIONALNIH OBIEKTOV============================================================================================\n" ).toStdString().c_str())); // =====================������ ������� �������������� ��������============================================================================================\n
    for(int i=0; i < gtextfiles.Size();  i++)
        gtextfiles.GetFile( i) << left << setw( NUMBER_STR_LENGTH) << "�" << "   " << left << setw( NAME_STR_LENTH_FOR_ELEMENT) << QObject::tr("Imia fynkcionalnogo obiekta").toStdString().c_str() << "   " << setw( NAME_STR_LENTH_FOR_ELEMENT)<<std::left<<QObject::tr("Pyt' faila, soderjaschego FO").toStdString().c_str()<<"   "<<std::endl; // ���� �����, ����������� ��
    gtextfiles.WriteToAllFile(QString( "====================================================================================================================================================\n"));
    //����� ���������� ������� � txt �����
    for( int i=0; i < gtextfiles.Size() ; i++)
    {
        int k;
        for( int j=0; j < mod_for_txt_file; j++)
        {
            k= j+i*mod_for_txt_file;
            if( size_t( k ) >= vs.size() )
            {
                gtextfiles.GetFile( i) << "===================================================================================================================================================="<<std::endl;
                gtextfiles.GetFile( i) << QObject::tr("Chislo visiaschih fynkchionalnih obiektov: ").toStdString().c_str() << vs.size() << "\n"; // ����� ������� �������������� ��������:
                break;
            }
            string ss1;
            ak_vs_db::get_scope_metadata ( ( vs[ k].s.id), "args", &ss1);
            gtextfiles.GetFile( i) << std::left << std::setw( NUMBER_STR_LENGTH) << QString::number(j+i*mod_for_txt_file+1).toStdString() << "   " << std::left << std::setw( NAME_STR_LENTH_FOR_ELEMENT) << ( vs[ k].name+ss1).c_str() << "   " << std::left << std::setw( NAME_STR_LENTH_FOR_ELEMENT) << vs[ k].file.c_str() << endl;
        }
    }
//>>>>>>>>>>>>>>>>>>>>> ����� ������ � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}
