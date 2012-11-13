/*
 * matrix_report.cpp
 *
 *  Created on: 30.10.2008
 *      Author: javamain
 */

#include "matrix_report.h"
#include "libservice.h"
#include <libdb_interface.h>
#include <QList>
#include <QMap>
#include <QVector>
#include <iostream>
#include <math.h>
#include <QFile>
#include <QSet>
#include <QTextStream>
#include <QTime>
#include <data_orm.h>
#include <QStringList>
#include <iomanip>
#include <QtGlobal>
using namespace alex;
using namespace std;
using namespace ak_vs_db;

HRET Matrix_cont_cluster( sReportInfo* pa)
{
    QORMList < sCluster> tmp;
    int i=0;
    for(QORMList< sCluster>::iterator j = tmp.begin(); j != tmp.end(); j++,i++)
    {
        db_set_current_cluster((*j).id);
        QString sss= QString( pa->strOutputDirectory.c_str())+"\\"+ QString::number((*j).id); 	//формируем директорию для отчета.
        mkdir( sss.toStdString().c_str() );
        if( pa->do_html_matrix)
        {
            Matrix_cont2(pa);
        }
        if( make_cont_Matrix_report( pa->strBaseFileName, sss.toStdString(), pa->strProjectName, ak_vs_db::get_program_name(), *(pa->lsProcessedFiles))== -1) return -1;
    }
    db_set_current_cluster( NULL);
    return 1;
}

// реально используется
HRET Matrix_cont(sReportInfo* pa)
{
    QTime delta_time;

    if(pa->do_html_matrix)
    {

        delta_time.start();
        cout << QObject::tr("(log):Sozdanie otcheta matric sviazei.").toStdString().c_str() << endl;  // (log):Создание отчета матриц связей.
        Matrix_cont2( pa );
        cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek." ).toStdString().c_str()<< endl; // Выполнено за  сек.
    }


    if(pa->do_txt_matrix)
    {
        cout << QObject::tr("Sozdanie tekstovih matric sviazei po ypravleniy..." ).toStdString().c_str()<< endl; // Создание текстовых матриц связей по управлению...
        if( make_cont_Matrix_report( pa->strBaseFileName, pa->strOutputDirectory,pa->strProjectName,ak_vs_db::get_program_name(),*(pa->lsProcessedFiles))==-1)
        {
            return -1;
        }
        cout << QObject::tr("Vipolneno sa " ).toStdString().c_str()<< delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl; // Выполнено за сек.
    }
    return 1;
}

HRET Matrix_info_cluster(sReportInfo* pa)
{
    QORMList < sCluster> tmp;
    int i=0;
    for( QORMList< sCluster>::iterator j = tmp.begin(); j != tmp.end(); j++,i++)
    {
        db_set_current_cluster( ( *j).id);
        QString sss= pa->strOutputDirectory.c_str()+("\\"+ QString::number( (*j).id)) ; 	//формируем директорию для отчета.
        mkdir( sss.toStdString().c_str());
        if( pa->do_html_matrix)
        {
            Matrix_info2( pa);

        }
        if( make_info_Matrix_report( pa->strBaseFileName, sss.toStdString(), pa->strProjectName, ak_vs_db::get_program_name(), *( pa->lsProcessedFiles) )== -1) return -1;
    }
    db_set_current_cluster( NULL);
    return 1;
}

HRET Matrix_info( sReportInfo* pa)
{
    QTime delta_time;
    if( pa->do_html_matrix)
    {
        delta_time.start();
        cout << QObject::tr("(log):Sozdanie otcheta matric sviazei po informacii." ).toStdString().c_str()<< endl; // (log):Создание отчета матриц связей по информации.
        Matrix_info2( pa);
        cout << QObject::tr("Vipolneno sa ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek." ).toStdString().c_str()<< endl; // Выполнено за сек.
    }


    if(pa->do_txt_matrix)
    {
        cout << QObject::tr("Sozdanie otcheta tekstovih matric sviaziei po informacii..." ).toStdString().c_str() << endl; // Создание отчета текстовых матриц связей по информации...
        if( make_info_Matrix_report( pa->strBaseFileName, pa->strOutputDirectory, pa->strProjectName, ak_vs_db::get_program_name(), *( pa->lsProcessedFiles) )== -1)
            return -1;
        cout << QObject::tr("Vipolneno za " ).toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl; // Выполнено за  сек.
    }
    return 1;
}


int Matrix_cont_report_html2( int cx, int cy, string fnames,
                              string pathname,
                              string project_name,
                              string version,
                              list < string> & lsProcessedFiles)
{
    return 0;
    //для y - ка все функции.
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    vector< string> full_name;
    for( int i=0; i< allscope.size(); i++)
    {
        full_name.push_back( it[ i].getQualifiedName());
    }
    int x= allscope.size();
    int y= x;
    //расчет количества html файлов
    int dx= 0;
    int dy= 0;
    if( 0 != x%cx )
    {
        dx= x/cx+1;
    }
    else
    {
        dx= x/cx;
    }
    if( 0 == x)
    {
        dx=1;
    }
    if( 0 != y%cy)
    {
        dy= y/cy+1;
    }
    else
    {
        dy= y/cy;
    }
    if( 0 == y)
    {
        dy=1;
    }
    //зодаем массив аштмл файлов.
    fstream **files=new fstream* [dy];///******************
    for( int i=0; dy > i; i++)
    {
        files[ i]= new fstream [ dx];
    }
    //создать имяна для html файлов
    string **file_name= new string*[ dy];//********************
    for( int i=0; i <dy; i++)
    {
        file_name[ i]= new string [ dx];
    }
    string **fname= new string*[dy];//********************
    for( int i=0; i<dy; i++)
    {
        fname[ i]= new string [ dx];
    }
    //создадим названия для файлов
    for( int j=0; j<dy; j++)
    {
        for( int i=0; i<dx; i++)
        {
            char ax[ 10];
            char ay[ 10];
            char app[ 10];
            itoa( i, ax, 10);
            itoa( j, ay, 10);
            itoa( j*dy+i, app, 10);
            file_name[ j][ i]
            .append( pathname.c_str() )
            .append( "\\")
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
            fname[ j][ i]
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
        }
    }
    //начнем писать в html файлы матрицы
    for( int j=0; j<dy; j++)
    {
        for( int i=0; i<dx; i++)
        {
            //откроем все файлы html отчетов
            files[ j][ i].open( file_name[ j][ i].c_str(), ios::out);
            lsProcessedFiles.push_back( file_name[ j][ i]);
            if( files[ j][ i].is_open()==false)
            {
                cerr << QObject::tr("fail").toStdString().c_str() << file_name[ j][ i].c_str() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl; // файл        не открылся
            }
            //введем заголовок
            files[ j][ i] << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n";
            files[ j][ i] << getCSS().c_str();
            files[ j][ i] << "<script type=\"text/javascript\">\n\
					function printtable(x,startx,starty,dx,dy)\n\
					{if(x.length==0)return;\n\
						var lencol=(\"\"+(startx+x.length)).length;\n\
							document.write(\"<table border=1>\");\n\
						for(var yc=0;yc<x.length+1;yc++)\n\
						{\n\
							if(yc==0)\n\
							{\n\
								document.write(\"<tr>\");\n\
								document.write(\"<th>NN</th>\");\n\
								for(var t=startx;t<startx+x[0].length;t++)\n\
								{	\n\
								var numprob=(lencol-(\"\"+(t+1)).length);\n\
									document.write(\"<th>\");\n\
									for(var g=0;g<numprob;g++){\n\
										document.write(\"&nbsp;\");\n\
									}\n\
									document.write(t+1);\n\
									document.write(\"</th>\");\n\
								}\n\
								document.write(\"</tr>\");\n\
							}\n\
							if(yc!=0)\n\
							{\n\
								document.write(\"<tr>\");\n\
								for(var xc=0;xc<x[0].length+1;xc++)\n\
								{\n\
									if(xc==0)\n\
									{\n\
										document.write(\"<th>\"+(starty+yc)+\"</th>\");\n\
									}\n\
									else\n\
									{\n\
										var tit;\n\
										var clss;\n\
										if(x[yc-1][xc-1]){clss=\" class=\\\"one\\\" \"; tit=\"Связь между \"+namey[yc-1]+\" и \"+namex[xc-1];}\n\
                                        else{clss=\"\";tit=\"Нет связь между \"+namey[yc-1]+\" и \"+namex[xc-1];}\n\
										document.write(\"<td align=center\" +\" title=\\\"\"+tit + \"\\\"\"+ clss+\" >\"+x[yc-1][xc-1]+\"</td>\");\n\
									};\n\
								}\n\
								document.write(\"</tr>\");\n\
							}\n\
						}\n\
						document.write(\"</table>\");\n\
					}\n\
							</script>\n";
            files[ j][ i] << "</HEAD><BODY>" << endl;
            zagolovok_html_file( &files[ j][ i], QObject::tr("MATRICA SVIAZEI PO YPRAVLENIY").toStdString().c_str(), project_name, version, fname[ j][ i], j*dy+i+1, dy*dx); // МАТРИЦА СВЯЗЕЙ ПО УПРАВЛЕНИЮ
            //таблица для таблицы и стрелочек в ней
            files[ j][ i] << "<table  class=ta align=\"center\"> <tr> <td>	\n";
            if( j-1>=0 && i-1>=0)
            {
                files[ j][ i] << strelka( fname[ j-1][ i-1], "..\\up_left.JPG");
            }//влево и вверх
            files[ j][ i] << "</td> <td>";
            if( j-1>=0 && i<dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i], "..\\up.JPG");
            }//вверх
            files[ j][ i] << "</td> <td>";
            if( j-1>=0 && i+1<dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i+1], "..\\up_right.JPG");
            }//вверх и вправо
            files[ j][ i] << " </td> </tr><tr> <td>";
            if( j<dy && i-1>=0)
            {
                files[ j][ i] << strelka( fname[ j][ i-1], "..\\left.JPG");
            }//влево
            files[ j][ i] << "</td> <td>\n";

            //выведем матрицу
            if(ak_vs_db::is_matrix_region_nonzero(CALL_LINK, i*cx, (i+1)*cx, j*cy, (j+1)*cy))
            {
                files[ j][ i] << "<script type=\"text/javascript\">";
                files[ j][ i] << "var starty=" << j*cy << ";\n";
                files[ j][ i] << "var startx=" << i*cx << ";\n";
                files[ j][ i] << "var cy=" << cy << ";\n";
                files[ j][ i] << "var cx=" << cx << ";\n";
                files[ j][ i] << "var namex=[";
                string ss1;
                for( int h= i*cx; h < (i+1)*cx; h++)
                {
                    int i1= it[ h].id;
                    get_scope_metadata( i1, "args", &ss1);
                    string sasa= it[ h].getQualifiedName()+ss1;
                    int k= 0;
                    while( sasa.find( "\"", k) != string::npos)
                    {
                        k=sasa.find( "\"", k);
                        sasa.erase( k, 1);
                        sasa.insert( k, "\\\"");
                        k= k+2;
                    }
                    files[ j][ i] << "\"" << sasa << "\"";
                    if( h != x-1 && h < (i+1)*cx-1 ) files[ j][ i] << ",";
                    if( h == x-1) break;
                }
                files[ j][ i] << "];\n";
                files[ j][ i] << "var namey=[";
                string ss2;
                for( int h= j*cy; h < (j+1)*cy; h++)
                {
                    int i1= it[ h].id;
                    get_scope_metadata( i1, "args", &ss2);
                    string sasa= it[ h].getQualifiedName()+ss2;
                    int k=0;
                    while( sasa.find( "\"", k) != string::npos)
                    {
                        k=sasa.find( "\"", k);
                        sasa.erase( k, 1);
                        sasa.insert( k, "\\\"");
                        k= k+2;
                    }
                    files[ j][ i] << "\"" << sasa << "\"";
                    if( h != y-1 && h < (j+1)*cy-1) files[ j][ i] << ",";
                    if( h == y-1) break;
                }
                files[ j][ i] << "];\n";
                bool test= false;
                string str= "";
                str= "var x=[";
                for( int gy=0; gy < cy; gy++)
                {
                    sScope c;
                    c.id= it[ gy+j*cy].id;
                    QORMList< sScopeConnected> tmp2 ( c, CALL_LINK, i*cx, (i+1)*cx);
                    QORMList< sScopeConnected>::iterator jt = tmp2.begin();
                    str.append( "[");
                    for( int gx=0; gx < cx; gx++)
                    {
                        test= true;
                        str.append( inttostring( jt[ gx+i*cx].is_connected));
                        if( gx+i*cx != x-1 && gx<cx-1)
                        {
                            str.append( ",");
                        }
                        if( gx+i*cx == x-1) break;
                    }
                    str.append( "]");
                    if( gy+j*cy != y-1 && gy < cy-1)
                    {
                        str.append( ",");
                    }
                    if( gy+j*cy == y-1)break;
                }
                if( test)
                {
                    str.append( "];\n printtable(x,startx,starty,cx,cy);");
                }
                files[ j][ i] << str+"</script>";
                str= "";
            }
            else
            {
                files[ j][ i] << html_breaket( "TABLE", "border=1 class=matrix",
                                               html_tr(
                                                   html_th( "№")+
                                                   html_th( QObject::tr("s ").toStdString().c_str()+inttostring(i*cx)+QObject::tr(" po ").toStdString().c_str()+inttostring((i+1)*cx) )
                                               )+
                                               html_tr(
                                                   html_th( QObject::tr("s ").toStdString().c_str()+inttostring(j*cy)+QObject::tr(" po ").toStdString().c_str()+inttostring((j+1)*cy))+
                                                   html_breaket( "TD","align=\"center\"","0")
                                               )
                                             )<<endl;

            }
            files[ j][ i] << "</td> <td>";
            if( j < dy && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j][ i+1],"..\\right.JPG");
            }//вправо

            files[ j][ i] << " </td> </tr> <tr> <td>";
            if( j+1 < dy && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j+1][ i-1], "..\\down_left.JPG");
            }//влево и вниз
            files[ j][ i]<<"</td> <td> \n";
            if( j+1<dy && i < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i], "..\\down.JPG");
            }//вниз
            files[ j][ i] << "</td> <td>";
            if( j+1<dy && i+1<dx)
            {
                files[j][i]<<strelka(fname[j+1][i+1],"..\\down_right.JPG");
            }//вниз и вправо
            files[ j][ i] << "</td> </tr>";
            files[ j][ i] << "</table>";
            files[ j][ i] << "</BODY></HEAD></HTML>";
            files[ j][ i].close();
        }
    }
    //удаляем имена файлов.
    for( int i=0; i<dy; i++)
    {
        delete file_name[ i];
    }
    delete [] file_name;
    //удаляем имена файлов.
    for( int i=0; i<dy; i++)
    {
        delete fname[ i];
    }
    delete [] fname;
    //удаляем объекты файлы.
    for( int i=0; i<dy; i++)
    {
        delete files[i];
    }
    delete [] files;
    return 0;
}


//***todo

class Matrix_Rect
{
public:
    Matrix_Rect():x_start_num( 0),y_start_num( 0),
        size()
    {}
    int x_start_num;
    int y_start_num;
    int size;
    void setInThisRect( int a, int b)
    {
        if(in_this_rect[ a ]==NULL)
        {
            in_this_rect[ a ]=new QList< int>();
        }
        if( !in_this_rect[ a ]->contains( b))
            in_this_rect[ a ]->push_back(b);
    }
    QList< int> * getInThisRect( int a)
    {
        return in_this_rect[ a ];
    }
    QMap< int, QList< int> *> in_this_rect;
};


int matrix_info_legend( QString path, int NUM, int NUM_SCREEN, list< string> & lsProcessedFiles)
{
    QMap< int, int> idtonum;
    QMap< int, int> numtoid;
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    QList< int> all_func;
    QMap< int, QString> full_name;

    int scope_number= allscope.size();
    string arg_fun;
    for( int i=0, size= allscope.size(); i< size; i++)
    {
        idtonum.insert( it[ i].id, i);
        numtoid.insert( i, it[ i].id);
        all_func.push_back( it[ i].id);
        get_scope_metadata ( it[ i].id, "args", &arg_fun);
        full_name[ it[ i].id]= QString( it[ i].getQualifiedName().c_str())+ arg_fun.c_str();
    }
    QMap< int, QSet< int> > mlink;
    QMap< int, QSet< int> > tmp_link_set;
    QORMList< sLink> data (INFORMATION_LINK);
    QMap< int, QList< int> > tmp_key_obj_value_list_func; // мап ключ объект , пересечение -
    QMap< int, QList<int> > tmp_id_func_id_obj;


    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if( 0 != (*j).id  && 0 != (*j).idB)
        {
            tmp_key_obj_value_list_func[ (*j).idB].push_back( (*j).id);   //
            tmp_id_func_id_obj[(*j).id].push_back((*j).idB);
        }
    }

    int a13,b13;
    for( QMap< int, QList< int> >::const_iterator it( tmp_key_obj_value_list_func.constBegin()); it != tmp_key_obj_value_list_func.constEnd(); ++it)
    {
        QList< int> tmp_id;
        for( QList< int>::const_iterator jt=it.value().constBegin(); jt != it.value().constEnd(); ++jt)
        {
            tmp_id.push_back( *jt);

        }
        for( QList< int>::iterator ht=tmp_id.begin(); ht != tmp_id.end(); ++ht)
        {
            a13=*ht;
            for( QList< int>::iterator tt=ht; tt != tmp_id.end(); ++tt)
            {
                b13= *tt;
                mlink[ a13].insert( b13);
                mlink[ b13].insert( a13);
            }
        }
    }

    // на этом закончился подготовительный этап
    ofstream legend_file( (path+"\\"+"Legend.txt").toStdString().c_str() );
    legend_file << QObject::tr("Sviaz fynkcii po informacii").toStdString().c_str() << "\n";
    for( QMap< int, QSet< int> >::const_iterator it( mlink.constBegin()); it != mlink.constEnd(); ++it)
    {
        int id1=it.key();
        int id2;
        int num1,num2;
        int numx_rect,numy_rect;
        for( QSet< int>::const_iterator jt=it.value().constBegin(); jt != it.value().constEnd(); ++jt)
        {
            id2=*jt;
            legend_file << "(" << idtonum[ id1] << ", " << idtonum[ id2] << ")" << "   "<< "(" << id1 << ", " << id2 << ")" << "   " << full_name[ id1].toStdString() << "   " << full_name[ id2].toStdString() << "\n";
        }
    }
    legend_file.close();
    return 1;
}


int getIntersect(QList<int>  &a, QList<int>  &b)
{
    QList<int>::iterator it;
    QList<int>::iterator jt;
    for( it=a.begin(); it != a.end(); it++)
    {

        for(jt=b.begin(); jt != b.end(); jt++)
        {
            if( (*jt)==(*it))
                return (*jt);
        }
    }
    return -1;
}

int matrix_info( QString path, int NUM, int NUM_SCREEN, list< string> & lsProcessedFiles)
{
    QMap< int, int> idtonum;
    QMap< int, int> numtoid;
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    QList< int> all_func;
    QMap< int, QString> full_name;
    QORMList< sScopeFull> tmp2 ( INFO_SCOPE );

    QMap< int, QString> nameofvar;

    //cout << "Take name of var...\n" << endl;
    // информационные скоупы
    QORMList< sScopeFull>::iterator itt= tmp2.begin();
    for( ; itt != tmp2.end() ; ++itt )
    {
        nameofvar[ (*itt).id ] = QString( ( *itt ).name.c_str() );
    }
    //cout << "Take idtoname numtoid and full_func_name...\n" << endl;
    int scope_number=allscope.size();
    for( int i=0; i < scope_number; i++)
    {
        idtonum.insert( it[ i ].id, i );
        numtoid.insert( i, it[ i ].id );
        all_func.push_back( it[ i ].id );
        full_name.insert( it[ i ].id, QString( it[ i ].getQualifiedName2().c_str() ) );
    }

    QMap< int, QSet< int> > mlink;
    QMap< int, QSet< int> > tmp_link_set;
    QORMList< sLink> data ( INFORMATION_LINK );
    QMap< int, QList< int> > tmp_key_obj_value_list_func; // мап ключ объект , пересечение -
    QMap< int, QList< int> > tmp_id_func_id_obj;
    //cout << "Take Link information...\n"<< endl;
    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if( 0 != (*j).id && 0 != (*j).idB )
        {
            tmp_key_obj_value_list_func[ (*j).idB ].push_back( (*j).id );   //
            tmp_id_func_id_obj[ (*j).id ].push_back( (*j).idB );
        }
    }

    //cout << "Take manadged with information Link...\n" << endl;
    int a13, b13;

    for( QMap< int, QList< int> >::const_iterator it= tmp_key_obj_value_list_func.begin();
            it != tmp_key_obj_value_list_func.end(); ++it)
    {
        QList< int> tmp_id;
        for( QList< int>::const_iterator jt=it.value().begin() ; jt!=it.value().end(); ++jt)
        {
            tmp_id.push_back( *jt );
        }
        for( QList< int>::iterator ht = tmp_id.begin(); ht != tmp_id.end(); ++ht )
        {
            a13=*ht;
            for( QList< int>::iterator tt=ht; tt!=tmp_id.end(); ++tt)
            {
                b13=*tt;
                mlink[ a13].insert( b13);
                mlink[ b13].insert( a13);
            }
        }

        if( mlink.size() % 10000 == 0)
        {
            cout << "mlink="<<mlink.size()<< endl;
        }
    }

    //cout << "Make matrix quadrant...\n" << endl;
    // на этом закончился подготовительный этап
    int count=ceil( double( all_func.size())/NUM);
    vector< vector< Matrix_Rect *> > mrv_array;
    for( int i=0; i < count; i++)
    {
        vector< Matrix_Rect*> ab;
        for( int j=0; j<count; j++)
        {
            ab.push_back( new Matrix_Rect());
        }
        mrv_array.push_back( ab);
    }

    for( int i=0; i < count; i++)
    {
        for( int j=0; j < count; j++)
        {
            mrv_array[ i ][ j ]->x_start_num=i+1;
            mrv_array[ i ][ j ]->y_start_num=j+1;
            mrv_array[ i ][ j ]->size = NUM;
        }
    }

    //cout << "Push one to needed quadrant...\n" << endl;
    //разложим все еденицы по необходимым квадратикам
    for( QMap< int, QSet< int> >::const_iterator it(mlink.constBegin()); it != mlink.constEnd(); ++it)
    {
        int id1=it.key();
        int id2;
        int num1,num2;
        int numx_rect,numy_rect;
        for(QSet< int>::const_iterator jt=it.value().constBegin(); jt != it.value().constEnd(); ++jt)
        {
            id2=*jt;
            numx_rect=( int)floor( double( idtonum[ id1])/NUM);
            numy_rect=( int)floor( double( idtonum[ id2])/NUM);
            mrv_array[ numx_rect][ numy_rect]->setInThisRect( id1, id2);// in_this_rect[id1].push_back(id2);
        }
    }
    //файлы настройки
    vector< vector< QString> > file_name_mat;
    for( int i=0; i < count; i++)
    {
        vector< QString> ab;
        for( int j=0; j<count; j++)
        {
            QString aaa;
            ab.push_back( aaa);
        }
        file_name_mat.push_back( ab);
    }
    mkdir( ( path+"\\data").toStdString().c_str());
    for(int i=0; i < count; i++)
    {
        for(int j=0; j < count; j++)
        {
            QString filename( "x(" + QString::number(i)+")" + "y(" + QString::number(j)+").js");
            QFile file( path + "\\data\\" + filename);
            file_name_mat[ i][ j]=filename;
            if ( !file.open( QIODevice::WriteOnly | QIODevice::Text))
                return -1;
            QTextStream out( &file);
            int numx = NUM;
            int numy = NUM;
            if( i==count-1) numx=all_func.size()%NUM;
            if( j==count-1) numy=all_func.size()%NUM;
            out << "var nx=" << QString::number( numx) << ";" << "\n";   // тут надо ввести NUM
            out << "var ny=" << QString::number( numy) << ";" << "\n";
            out << "var dx=" << QString::number( i*NUM) << ";" << "\n";
            out << "var dy=" << QString::number( j*NUM) << ";" << "\n";
            out << "function getX()"<<"\n";
            out << "{"<<"\n";
            out << "var x=[";
            QMap< int, QList< int> *> *pmap=&mrv_array[ i][ j]->in_this_rect;
            for( QMap< int, QList< int> *>::iterator it=pmap->begin(); it!=pmap->end(); ++it)
            {
                QList < int> * plst=it.value();
                for( QList< int>::iterator jt =plst->begin(); jt != plst->end(); ++jt)
                {
                    string name_obj;
                    int obj=getIntersect( tmp_id_func_id_obj[ it.key() ], tmp_id_func_id_obj[ *jt ]);

                    if( obj != -1)
                    {
                        name_obj= nameofvar[obj].toStdString();//get_scope_name_by_id(obj);
                    }

                    if( QString( name_obj.c_str()) != "" )
                    {
                        //out << "[" << QString::number( idtonum[ it.key()]) << "," << QString::number( idtonum[*jt]) << ",\"" << name_obj.c_str() << "\"]";
                        out << "[" << QString::number( idtonum[*jt]) << "," <<  QString::number( idtonum[ it.key()]) << ",\"" << name_obj.c_str() << "\"]";
                        it++;
                        jt++;
                        if( it != pmap->end() ) out << ",";
                        else
                        {
                            if( jt != plst->end() ) out <<",";
                        }
                        it--;
                        jt--;
                    }
                }
            }

            out << "];" << "\n";
            out << "return x;" << "\n";
            out << "}" << "\n";
            file.close();
        }
    }

    //записываем имена в список файлов


    QFile file2(path+"\\data\\control.js");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))

        return -1;
    QTextStream out2( &file2);
    out2 << "var arrayfile=[";
    for( int i=0; i<count; i++)
    {
        if( i!=0) out2 << ",";
        out2 << "[";
        for( int j=0; j<count; j++)
        {
            if( j!=0) out2 << ",";
            out2 << "\"" << file_name_mat[ i][ j]<<"\"";
        }
        out2 << "]";
    }
    out2 << "];" << "\n";

    out2 << "var function_name=[";
    for( int i=0; i < full_name.size(); i++)
    {

        sScopeFull s( numtoid[ i]);
        s.refresh();

        out2 << "[\"" << full_name[ numtoid[i]].replace( QString( "\""), QString( "") ) << "\",\"" <<  QString( s.file_path.c_str()).replace(QString("\\"),QString("/")) << "\"]";
        if( i != ( full_name.size()- 1) ) out2 << ",";
    }
    out2 << "];" << "\n";

    out2 << "var num_x_file=arrayfile.length;  // eiee?anoai oaeeia" << "\n";
    out2 << "var num_y_file=num_x_file; 	      // eiee?anoai oaeeia" << "\n";
    out2 << "var screen_size_x=" << NUM_SCREEN << "; //oaeouee ?acia? ieia X" << "\n";
    out2 << "var screen_size_y=" << NUM_SCREEN << "; //oaeouee ?acia? ieia Y" << "\n";
    out2 << "var file_size_x=" << NUM << ";  // eiee?anoai ooieoee a oaeea" << "\n";
    out2 << "var file_size_y=" << NUM << ";  // eiee?anoai ooieoee a oaeea" << "\n";
    out2 << "var max_screen_in_file_x=Math.ceil(file_size_x/screen_size_x); // eiee?anoai ye?aiia a oaeea"<<"\n";
    out2 << "var max_screen_in_file_y=Math.ceil(file_size_y/screen_size_y); // eiee?anoai ye?aiia a oaeea"<<"\n";
    out2 << "var x_in_file=0;   // eii?aeiaoa a oaeea X" << "\n";
    out2 << "var y_in_file=0;   // eii?aeiaoa a oaeea Y" << "\n";
    out2 << "var tmp_x_pos=num_x_file*screen_size_x;  // eiee?anoai ooieoee" << "\n";
    out2 << "var tmp_y_pos=num_y_file*screen_size_y;  // eiee?anoai ooieoee" << "\n";
    out2 << "var tmp_size=20;   // oaeouee ?acia? o?eooa" << "\n";
    out2 << "var sleep_time=50;  // oaeouey caaa??ea ia?aa ioia?a?aieai iao?eou" << "\n";
    out2 << "function gotojs()" << "\n";
    out2 << "{" << "\n";
    out2 << "	gotojs_file();" << "\n";
    out2 << "}" << "\n";
    out2 << "function gotojs_file()" << "\n";
    out2 << "{" << "\n";
    out2 << "	var new_x=document.getElementById(\"go_to_x_js\").value;" << "\n";
    out2 << "	var new_y=document.getElementById(\"go_to_y_js\").value;" << "\n";
    out2 << " var x_max_file=document.getElementById(\"sizex\").firstChild;" << "\n";
    out2 << " var y_max_file=document.getElementById(\"sizey\").firstChild;" << "\n";
    out2 << " if(new_x>=0 && new_y>=0 && new_x<=(x_max_file.data-1) && new_y<=(y_max_file.data-1) ){}" << "\n";
    out2 << "	else { return ;}"<<"\n";
    out2 << "	var pos_x= document.getElementById(\"pos_x\").firstChild ;" << "\n";
    out2 << "	var pos_y= document.getElementById(\"pos_y\").firstChild ;" << "\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл" << "\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл" << "\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно" << "\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно" << "\n";
    out2 << "	pos_x.data=new_x;" << "\n";
    out2 << "	pos_y.data=new_y;" << "\n";
    out2 << "	if(new_x>=0 && new_y>=0 && new_x<=(num_x_file*max_screen_in_file_x-1) && new_y<=(num_y_file*max_screen_in_file_y-1) )" << "\n";
    out2 << "	{" << "\n";
    out2 << "		tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл" << "\n";
    out2 << "		tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл" << "\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		xin_file=pos_x.data%max_screen_in_file_x;" << "\n";
    out2 << "		yin_file=pos_y.data%max_screen_in_file_y;" << "\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");" << "\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);" << "\n";
    out2 << "	}" << "\n";
    out2 << "}" << "\n";
    out2 << "function onup()" << "\n";
    out2 << "{" << "\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;	" << "\n";
    out2 << "	var y_file=Math.floor(pos_y.data/max_screen_in_file_y);  // текущий файл" << "\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; 			// текущее окно" << "\n";
    out2 << "	if(pos_y.data==0) return;" << "\n";
    out2 << "	pos_y.data--;" << "\n";
    out2 << "	y_file		=Math.floor(pos_y.data/max_screen_in_file_y);  // текущий файл"<<"\n";
    out2 << "	yin_file	=pos_y.data%max_screen_in_file_y; 			// текущее окно"<<"\n";
    out2 << "	if(yin_file==max_screen_in_file_y-1)"<<"\n";
    out2 << "	{" << "\n";
    out2 << "		onup_file();" << "\n";
    out2 << "		return ;" << "\n";
    out2 << "	}" << "\n";
    out2 << "	removeElement(\"tablemain\");" << "\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);" << "\n";
    out2 << "}" << "\n";
    out2 << "function onup_file() // ioe?uou aa?oiee oaee" << "\n";
    out2 << "{" << "\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;" << "\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;" << "\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл" << "\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл" << "\n";
    out2 << "	if(tmp_y_file>=0)" << "\n";
    out2 << "	{" << "\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onleft()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x	=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var x_file	=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	if( pos_x.data==0) return;"<<"\n";
    out2 << "	pos_x.data--;"<<"\n";
    out2 << "	x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	if(xin_file==max_screen_in_file_x-1)"<<"\n";
    out2 << "	{" << "\n";
    out2 << "		onleft_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onleft_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	if(tmp_x_file>=0)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}" << "\n";
    out2 << "function onright()"<<"\n";
    out2 << "{" << "\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	var x_max_file=document.getElementById(\"sizex\").firstChild;"<<"\n";
    out2 << "	if( pos_x.data==(x_max_file.data - 1) )"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		return;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	pos_x.data++;"<<"\n";
    out2 << "	var x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	if(xin_file == 0)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		onright_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onright_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	if(tmp_x_file<=(num_x_file-1))"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function ondown()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "	var y_max_file=document.getElementById(\"sizey\").firstChild;"<<"\n";
    out2 << "	if( pos_y.data == (y_max_file.data-1) )"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		return;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	pos_y.data++;"<<"\n";
    out2 << "	y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "	if(yin_file==0)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		ondown_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function ondown_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	if(tmp_y_file<=(num_y_file-1))"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function endPart()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "		var x=getX();"<<"\n";
    out2 << "		var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "		var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "		var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "		var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "		var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "		var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "		var a_size=screen_size_x;  //?acia? ii x"<<"\n";
    out2 << "		var b_size=screen_size_y;  //?acia? ii y"<<"\n";
    out2 << "		var xs= parseInt(+tmp_x_file)*file_size_x+parseInt(+xin_file)*screen_size_x;"<<"\n";
    out2 << "		var ys= parseInt(+tmp_y_file)*file_size_y+parseInt(+yin_file)*screen_size_y;"<<"\n";
    out2 << "		if( (nx+dx-xs) < screen_size_x )"<<"\n";
    out2 << "		{"<<"\n";
    out2 << "			a_size=(nx+dx-xs);"<<"\n";
    out2 << "		}"<<"\n";
    out2 << "		if( ( ny+dy-ys ) < screen_size_y )"<<"\n";
    out2 << "		{"<<"\n";
    out2 << "			b_size=(ny+dy-ys);"<<"\n";
    out2 << "		}"<<"\n";
    out2 << "		var table1=matrix(a_size ,b_size , xs, ys, x);"<<"\n";
    out2 << "		document.getElementById(\"inserttable\").innerHTML=table1;"<<"\n";
    out2 << "		setmatsize();"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function getElementsByName_iefix(tag, name) {"<<"\n";
    out2 << "     var elem = document.getElementsByTagName(tag);"<<"\n";
    out2 << "     var arr = new Array();"<<"\n";
    out2 << "     for(i = 0,iarr = 0; i < elem.length; i++) {"<<"\n";
    out2 << "          att = elem[i].getAttribute(\"name\");"<<"\n";
    out2 << "          if(att == name) {"<<"\n";
    out2 << "               arr[iarr] = elem[i];"<<"\n";
    out2 << "               iarr++;"<<"\n";
    out2 << "          }"<<"\n";
    out2 << "     }"<<"\n";
    out2 << "     return arr;"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function setmatsize()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function plusplus()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	if(tmp_size<33)tmp_size++;"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function minusminus()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	if(tmp_size>0) tmp_size--;"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";


    file2.close();

//создаем индексный файл для матрицы

    QFile file3(path+"\\index.html");
    lsProcessedFiles.push_back(file3.fileName().toStdString());
    if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out3( &file3);

    QStringList & list=make_index_file_string(false);

    QList<QString>::iterator it2=list.begin();
    while( it2 != list.end())
    {
        out3<<( *it2);
        ++it2;
    }
    file3.close();

    return 0;
}

QStringList & make_index_file_string(bool isCont)
{
    QStringList * str=new QStringList();
    *str << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">"<<"\n";
    *str << "<HEAD>" << "\n";
    *str << "<TITLE>" << "\n";
    *str << "</TITLE>" << "\n";
    *str << "<style type=\"text/css\">"<<"\n";
    *str << "#header_text " << "\n";
    *str << "{" << "\n";
    *str << "	text-align: center;" << "\n";
    *str << "	 font: 32px serif;" << "\n";
    *str << "}" << "\n";
    *str << "p, td, th, input" << "\n";
    *str << "{" << "\n";
    *str << "	font: 12px serif;" << "\n";
    *str << "}" << "\n";
    *str << "#one " << "\n";
    *str << "{" << "\n";
    *str << "	background-color: RGB( 226, 197, 29);" << "\n";
    *str << "}" << "\n";
    *str << "th" << "\n";
    *str << "{" << "\n";
    *str << "	background-color: RGB( 180, 170, 170);" << "\n";
    *str << "}	" << "\n";
    *str << "#tablemain" << "\n";
    *str << "{" << "\n";
    *str << "	border: 4px normal black;" << "\n";
    *str << "	border-collapse: collapse;" << "\n";
    *str << "}" << "\n";
    *str << "td" << "\n";
    *str << "{" << "\n";
    *str << "	text-align: center;" << "\n";
    *str << "}" << "\n";
    *str << "</style>" << "\n";
    *str << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=windows-1251\">" << "\n";
    *str << "<script type=\"text/javascript\" src=\"data\\control.js\" ></script>" << "\n";
    *str << "<script type=\"text/javascript\">" << "\n";
    *str << "function sIncrease( i, ii) { // "<< QObject::tr("Po vozrastaniy").toStdString().c_str() << "\n";
    *str << "    if ( i[0] > ii[0])" << "\n";
    *str << "        return 1;" << "\n";
    *str << "    else if (i[0] < ii[0])" << "\n";
    *str << "        return -1;" << "\n";
    *str << "    else" << "\n";
    *str << "        return 0;" << "\n";
    *str << "}" << "\n";
    *str << "//загрузить скрипт" << "\n";
    *str << "function loadjscssfile(filename, filetype,idname){" << "\n";
    *str << " if ( filetype == \"js\"){ //if filename is a external JavaScript file" << "\n";
    *str << "  var fileref= document.createElement( 'script')" << "\n";
    *str << "  fileref.setAttribute( \"id\", idname);" << "\n";
    *str << "  fileref.setAttribute( \"type\", \"text/javascript\")"<<"\n";
    *str << "  fileref.setAttribute( \"src\", filename)" << "\n";
    *str << " }" << "\n";
    *str << " else if ( filetype == \"css\"){ //if filename is an external CSS file" << "\n";
    *str << "  var fileref= document.createElement( \"link\")" << "\n";
    *str << "  fileref.setAttribute( \"rel\", \"stylesheet\")" << "\n";
    *str << "  fileref.setAttribute( \"type\", \"text/css\")" << "\n";
    *str << "  fileref.setAttribute( \"href\", filename)" << "\n";
    *str << " }" << "\n";
    *str << " if ( typeof fileref != \"undefined\")" << "\n";
    *str << "  document.getElementsByTagName( \"head\")[ 0].appendChild( fileref)" << "\n";
    *str << "}" << "\n";
    *str << "function removeElement( id)	{" << "\n";
    *str << "	var Node = document.getElementById( id);" << "\n";
    *str << "	Node.parentNode.removeChild( Node);" << "\n";
    *str << "}" << "\n";
    *str << "// строит матрицу N количество элементов в матрице" << "\n";
    *str << "Array.prototype.unique ="<< "\n";
    *str << "function() {"<< "\n";
    *str << " var a = [];"<< "\n";
    *str << " var l = this.length;"<< "\n";
    *str << " for(var i=0; i<l; i++) {"<< "\n";
    *str << "   for(var j=i+1; j<l; j++) {"<< "\n";
    *str << "     if (this[i][0] == this[j][0] &&this[i][1] == this[j][1]  )"<< "\n";
    *str << "       j = ++i;"<< "\n";
    *str << "   }"<< "\n";
    *str << "   a.push(this[i]);"<< "\n";
    *str << " }"<< "\n";
    *str << " return a;"<< "\n";
    *str << "};" << "\n";





    *str << "Array.prototype.unique2 =" << "\n";
    *str << "function() {" << "\n";
    *str << "    var a = [];" << "\n";
    *str << "    var l = this.length;" << "\n";
    *str << "    for(var i=0; i<l; i++) {" << "\n";
    *str << "    for(var j=i+1; j<l; j++) {" << "\n";
    *str << "       if (this[i][0] == this[j][0]  )" << "\n";
    *str << "          j = ++i;" << "\n";
    *str << "    }" << "\n";
    *str << "    a.push(this[i]);" << "\n";
    *str << "}" << "\n";
    *str << "return a;" << "\n";
    *str << "};" << "\n";




    *str << "function matrix( N_x, N_y, X_START, Y_START, ARRAY)" << "\n";
    *str << "{" << "\n";
    *str << "       ARRAY=ARRAY.unique();" << "\n";
    *str << "	//alert( \"N_x:\"+N_x+\"N_y:\"+N_y+\" X_START:\"+X_START+\" Y_START:\"+Y_START); " << "\n";
    *str << "	var line_array =new Array();" << "\n";
    *str << "	for( var i=0; i < ARRAY.length; i++)" << "\n";
    *str << "	{" << "\n";
    *str << "		var a=ARRAY[ i][ 0];" << "\n";
    *str << "		var b=ARRAY[ i][ 1];" << "\n";
    *str << "               var c=ARRAY[ i][ 2];" << "\n";
    *str << "		if( ( a >= X_START) && ( a < (X_START+N_x) ) && ( b >= (\"\"+Y_START)) && ( b < (Y_START+N_y)) )" << "\n";
    *str << "		{" << "\n";
    *str << "                       var mytmp=new Array(2);" << "\n";
    *str << "                       mytmp[0]=(a-X_START+(b-Y_START)*N_x);" << "\n";
    *str << "                       mytmp[1]=c;" << "\n";
    *str << "			line_array.push(mytmp);" << "\n";
    *str << "		}" << "\n";
    *str << "	}" << "\n";
    *str << "	line_array.sort( sIncrease);" << "\n";
    *str << " line_array=line_array.unique2();" << "\n";
    *str << "	var tmp_x=0;" << "\n";
    *str << "	var tmp_y=Y_START;" << "\n";
    *str << "	var w_length=( \"\"+(X_START+N_x)).length;" << "\n";
    *str << "	var h_length=( \"\"+(Y_START+N_y)).length;" << "\n";
    *str << "	var next_one=0;" << "\n";
    *str << "	var pres_poss=0;" << "\n";
    *str << "	var table_str=\"<table id='tablemain' border=1 align=\\\"center\\\">\";" << "\n";
    *str << "	table_str+=\"<tr><th name=\\\"tbtext\\\" >NN</th>\";" << "\n";
    *str << "	for(var i=X_START; i < (N_x+X_START); i++)" << "\n";
    *str << "	{" << "\n";
    *str << "		//распечатываем горизонтальные номера в матрице." << "\n";
    *str << "		table_str+=\"<th name=\\\"tbtext\\\" TITLE=\\\""<<QObject::tr("fynkcia").toStdString().c_str()<<" \"+function_name[i][0]+\" "<<QObject::tr("is faila").toStdString().c_str()<<" \"+function_name[i][1]+\"\\\" >\";" << "\n";
    *str << "		var sstr=\"\";" << "\n";
    *str << "		//вставка пробелов" << "\n";
    *str << "		for( var j=0; j < w_length-(\"\"+(1+i) ).length; j++)" << "\n";
    *str << "		{" << "\n";
    *str << "			sstr=sstr+\"&nbsp;\";	" << "\n";
    *str << "		}" << "\n";
    *str << "		table_str+=(i+1)+sstr+\"</th>\";	" << "\n";
    *str << "	}" << "\n";
    *str << "	table_str+=\"</tr>\";" << "\n";
    *str << "	for( var i=0; i < line_array.length; i++)" << "\n";
    *str << "	{" << "\n";
    *str << "		next_one=line_array[ i][0]-pres_poss;" << "\n";
    *str << "		for(; next_one>0 ;)" << "\n";
    *str << "		{" << "\n";
    *str << "			if( pres_poss%N_x == 0 ) " << "\n";
    *str << "			{" << "\n";
    *str << "				tmp_x=0;" << "\n";
    *str << "				table_str+=\"</tr>\"+\"<tr><th name=\\\"tbtext\\\" TITLE=\\\""<<QObject::tr("fynkcia").toStdString().c_str()<<" \"+function_name[ tmp_y][0]+\" "<<QObject::tr("v faile").toStdString().c_str()<<" \"+function_name[ tmp_y][1]+\"\\\">\"+(1+tmp_y)+\"</th>\";" << "\n";
    *str << "				tmp_y++;" << "\n";
    *str << "			}" << "\n";
    *str << "			tmp_x++;" << "\n";
    *str << "			table_str+=\"<td name=\\\"tbtext\\\" TITLE=\\\"\"+\""<<QObject::tr("Net cviazi mejdy fynkciiami").toStdString().c_str()<<" \"+function_name[tmp_y-1][0]+\", \"+function_name[X_START+tmp_x-1][0]+\"\\\">0</td>\";" << "\n";
    *str << "			pres_poss++;" << "\n";
    *str << "			next_one--;" << "\n";
    *str << "		}"<<"\n";
    *str << "		//"<<QObject::tr("perehod na noviyiy stroky ").toStdString().c_str() << "\n";
    *str << "		if( pres_poss%N_x == 0 )" << "\n";
    *str << "		{" << "\n";
    *str << "			tmp_x=0;" << "\n";
    *str << "			table_str+=\"</tr>\"+\"<tr><th name=\\\"tbtext\\\" TITLE=\\\""<<QObject::tr("fynkcia").toStdString().c_str()<<" \"+function_name[ tmp_y][0]+\" "<<QObject::tr("v faile").toStdString().c_str()<<" \"+function_name[ tmp_y][1]+\"\\\">\"+(tmp_y+1)+\"</th>\";" << "\n";
    *str << "			tmp_y++;" << "\n";
    *str << "		}" << "\n";
    *str << "		tmp_x++;" << "\n";

    *str << "               if( line_array[i][1])" << "\n";

    if( !isCont )
    {
        *str << "                   table_str+=\"<td name=\\\"tbtext\\\" id=\\\"one\\\" TITLE=\\\"\"+\""<<QObject::tr("Sviaz mejdy fynkciami").toStdString().c_str()<<" \"+function_name[tmp_y-1][0]+\", \"+function_name[X_START+tmp_x-1][0]+\" "<<QObject::tr("po elementy").toStdString().c_str()<<" \"+line_array[i][1]+\"\\\">1</td>\";" << "\n";
    }
    else
    {
                *str << "                   table_str+=\"<td name=\\\"tbtext\\\" id=\\\"one\\\" TITLE=\\\"\"+\""<<QObject::tr("Sviaz mejdy fynkciami").toStdString().c_str()<<" \"+function_name[tmp_y-1][0]+\", \"+function_name[X_START+tmp_x-1][0]+\">1</td>\";" << "\n";
    }

    *str << "               else" << "\n";
        *str << "                   table_str+=\"<td name=\\\"tbtext\\\" id=\\\"one\\\" TITLE=\\\"\"+\""<<QObject::tr("Sviaz mejdy fynkciami").toStdString().c_str()<<" \"+function_name[tmp_y-1][0]+\", \"+function_name[X_START+tmp_x-1][0]+\"\\\">1</td>\";" << "\n";

    *str << "		pres_poss++;" << "\n";
    *str << "	}" << "\n";
    *str << "	var NN=N_x*N_y;" << "\n";
    *str << "	for(; pres_poss < NN;)" << "\n";
    *str << "	{" << "\n";
    *str << "		if( pres_poss%N_x == 0 )" << "\n";
    *str << "		{" << "\n";
    *str << "			tmp_x=0;" << "\n";
        *str << "			table_str+=\"</tr>\"+\"<tr><th name=\\\"tbtext\\\" TITLE=\\\""<<QObject::tr("fynkcia").toStdString().c_str()<<" \"+function_name[tmp_y][0]+\" "<<QObject::tr("v faile").toStdString().c_str()<<" \"+ function_name[ tmp_y][1] +\"\\\">\"+ (1+tmp_y)+\"</th>\";" << "\n";
    *str << "			tmp_y++;" << "\n";
    *str << "		}" << "\n";
    *str << "		tmp_x++;" << "\n";
        *str << "		table_str+=\"<td name=\\\"tbtext\\\" TITLE=\\\"\"+\""<<QObject::tr("Net sviazi mejdy fynkciami").toStdString().c_str()<<" \"+function_name[tmp_y-1][0]+\", \"+function_name[X_START+tmp_x-1][0]+\"\\\">0</td>\";"<<"\n";
    *str << "		pres_poss++;" << "\n";
    *str << "	}" << "\n";
    *str << "	table_str+=\"</table>\";" << "\n";
    *str << "	return table_str;" << "\n";
    *str << "}" << "\n";
    *str << "function removeElement( id)	{" << "\n";
    *str << "	var Node = document.getElementById( id);" << "\n";
    *str << "	if( Node != null)" << "\n";
    *str << "	Node.parentNode.removeChild( Node);" << "\n";
    *str << "}" << "\n";
    *str << "function deletetable( id)" << "\n";
    *str << "{" << "\n";
    *str << "	removeElement( id);" << "\n";
    *str << "}" << "\n";
    *str << "</script>" << "\n";
    *str << "</head>" << "\n";
    *str << "<body>" << "\n";
    if( !isCont )
    {   // информационные связи
        *str << "<H3 id=\"header_text\">"<<QObject::tr("Matrica sviazei funkcii po informacii").toStdString().c_str()<<"</h3>" << "\n";
    }else
    {   // по вызову.
        *str << "<H3 id=\"header_text\">"<<QObject::tr("Matrica sviazei funkcii po ypravleniy").toStdString().c_str()<<"</h3>" << "\n";
    }
    *str << "<table id=\"mega_controller\" align=\"center\">" << "\n";
    *str << "<tr>" << "\n";
    *str << "	<td>" << "\n";
    *str << "		<form>" << "\n";
    *str << "			<table  border=0>" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td></td>" << "\n";
    *str << "					<td>" << "\n";
    *str << "						<input type=\"button\" onclick=\"onup();\" value=\""<<QObject::tr("vverh").toStdString().c_str()<<"\"></input>" << "\n";
    *str << "					</td>" << "\n";
    *str << "					<td></td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td>" << "\n";
    *str << "						<input type=\"button\" onclick=\"onleft();\" value=\""<<QObject::tr("vlevo").toStdString().c_str()<<"\"></input>" << "\n";
    *str << "					</td>" << "\n";
    *str << "					<td></td>" << "\n";
    *str << "					<td>" << "\n";
    *str << "						<input type=\"button\" onclick=\"onright();\" value=\""<<QObject::tr("vpravo").toStdString().c_str()<<"\"></input>" << "\n";
    *str << "					</td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td></td>" << "\n";
    *str << "					<td>" << "\n";
    *str << "						<input type=\"button\" onclick=\"ondown();\" value=\" "<<QObject::tr("vniz").toStdString().c_str()<<"   \"></input>" << "\n";
    *str << "					</td>" << "\n";
    *str << "					<td></td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "			</table>" << "\n";
    *str << "		</form>" << "\n";
    *str << "	</td><td>" << "\n";
    *str << "	<table border=0 id=\"polojenie\">" << "\n";
    *str << "		<tr>" << "\n";
    *str << "				<td>"<<QObject::tr("tekyschee polojenie x").toStdString().c_str()<<"</td>" << "\n";
    *str << "				<td>"<<QObject::tr("tekyschee polojenie y").toStdString().c_str()<<"</td>" << "\n";
    *str << "				<td>"<<QObject::tr("Chislo failov po X").toStdString().c_str()<<"</td>" << "\n";
    *str << "				<td>"<<QObject::tr("Chislo failov po Y").toStdString().c_str()<<"</td>" << "\n";
    *str << "		</tr>" << "\n";
    *str << "		<tr>" << "\n";
    *str << "				<td id=\"pos_x\"  >0</td>" << "\n";
    *str << "				<td id=\"pos_y\"  >0</td>" << "\n";
    *str << "				<td id=\"sizex\">0</td>" << "\n";
    *str << "				<td id=\"sizey\">0</td>" << "\n";
    *str << "		</tr>" << "\n";
    *str << "	</table>" << "\n";
    *str << "</td><td>" << "\n";
    *str << "		<form>" << "\n";
    *str << "			<table border=0 id=\"forma1\">" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td>"<<QObject::tr("Pereiti po X").toStdString().c_str()<<"</td>" << "\n";
    *str << "					<td>"<<QObject::tr("Pereiti po Y").toStdString().c_str()<<"</td>" << "\n";
    *str << "					<td rowspan=2> <input type=\"button\" onclick=\"gotojs();\" name=\"go_to_js\" value=\""<<QObject::tr("Pereiti").toStdString().c_str()<<"\" ></input></td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td><input type=\"text\" id=\"go_to_x_js\" value=\"0\"></input></td>" << "\n";
    *str << "					<td><input type=\"text\" id=\"go_to_y_js\" value=\"0\"></input></td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "			</table>" << "\n";
    *str << "		</form>" << "\n";
    *str << "</td><td>" << "\n";
    *str << "		<form>" << "\n";
    *str << "			<table border =0 id=\"forma2\">" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td colspan=\"2\" >"<<QObject::tr("Razmer matrici").toStdString().c_str()<<"</td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "				<tr>" << "\n";
    *str << "					<td><input type=\"button\" id=\"plus\" value=\"+\" onclick=\"plusplus();\"></input></td>" << "\n";
    *str << "					<td><input type=\"button\" id=\"minux\" value=\"-\" onclick=\"minusminus();\"></input></td>" << "\n";
    *str << "				</tr>" << "\n";
    *str << "			</table>" << "\n";
    *str << "		</form>" << "\n";
    *str << "</td>" << "\n";
    *str << "</tr>" << "\n";
    *str << "</table>" << "\n";
    *str << "<script type=\"text/javascript\">" << "\n";
    *str << "loadjscssfile( \"data\\\\\"+arrayfile[ 0][ 0], \"js\",\"110\");" << "\n";
    *str << "document.getElementById( \"sizex\").firstChild.data=( Math.ceil( function_name.length/screen_size_x));" << "\n";
    *str << "document.getElementById( \"sizey\").firstChild.data=( Math.ceil( function_name.length/screen_size_x));" << "\n";
    *str << "file_size_x=nx;" << "\n";
    *str << "file_size_y=ny;" << "\n";
    *str << "</script>" << "\n";
    *str << "<div id=\"inserttable\">" << "\n";
    *str << "&nbsp;" << "\n";
    *str << "</div> " << "\n";
    *str << "<script type=\"text/javascript\">" << "\n";
    *str << "gotojs();" << "\n";
    *str << "</script>" << "\n";
    *str << "</body>" << "\n";
    *str << "</html>" << "\n";
    return *str;
}


/**
  * return              возвращает ошибку
  * @path               путь к отчетам
  * @NUM                число функций
  * @NUM_SCREEN         число функций на экране
  * @lsProcessedFiles   структура данных
  **/

// функция сравнения.
bool myfunction (int i,int j)
{
    return (i<j);
}

//выводит всю матрицу в текстовом виде
void matrix( int n_x, int n_y, int x_start, int y_start, vector< vector< int> > &array )
{
    vector< long> line_array;
    for( int i=0; i< array.size(); i++)
    {
        int a=array[ i][ 0];
        int b=array[ i][ 1];
        if( (a >= x_start) && ( a < (x_start+ n_x)) &&
                ( b >= y_start) && ( b < (y_start+ n_y)) )
        {
            line_array.push_back( (long)( a- x_start+ ( b- y_start)* n_x) );
        }
    }
    sort(line_array.begin(),line_array.end(),myfunction);
    int tmp_x=0;
    int tmp_y=y_start;
    int w_length= log( x_start+ n_x);
    int h_length= log( y_start+ n_y);
    int next_one= 0;
    int pres_poss= 0;
    //тут нужно сделать вывод заголовка для матрицы
    for( int i= x_start; i< (n_x+ x_start); i++)
    {

    }
    for( int i= 0; i< line_array.size(); i++)
    {
        next_one= line_array[ i]-pres_poss;
        for(; next_one>0; )
        {
            if( pres_poss% n_x == 0)
            {
                // печатаем строку по горизонтали
                tmp_x= 0;
                cout << "\n"<< QObject::tr("NOMER1").toStdString().c_str();
                tmp_y++;
            }

            tmp_x++;
            cout << "0";
            pres_poss++;
            next_one--;
        }
        //переход на новую строку
        if( pres_poss% n_x== 0)
        {
            tmp_x= 0;
            cout << "\n"<<QObject::tr("NOMER2").toStdString().c_str();
            tmp_y++;
        }
        tmp_x++;
        cout << "1";
        pres_poss++;
    }
    int NN = n_x*n_y;
    for(; pres_poss < NN; )
    {
        if( 0 == pres_poss% n_x)
        {
            tmp_x= 0;
            cout << "\n" << QObject::tr("NOMER3").toStdString().c_str();
            tmp_y++;
        }
        tmp_x++;
        cout << "0";
        pres_poss++;
    }
    cout << "\n";
}




int matrix_cont_probe( QString path, int NUM, int NUM_SCREEN, list< string> & lsProcessedFiles, double prob)
{

    QMap< int, int> idtonum;
    QMap< int, int> numtoid;
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    QList< int> all_func;
    QMap< int, QString>full_name;

    int scope_number=allscope.size();
    for( int i=0; i<allscope.size(); i++)
    {
        idtonum[ it[ i].id]=i;
        numtoid[ i]=it[i].id;
        all_func.push_back( it[ i].id);
        full_name[ it[ i].id]= QString( it[ i].getQualifiedName2().c_str());
    }
    QMap< int, QList< int> > mlink;
    QORMList< sLink> data (CALL_LINK);
    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if((*j).id!=0 && (*j).idB!=0)
            mlink[ (*j).id].push_back( (*j).idB);
    }
    // на этом закончился подготовительный этап
    int count= ceil( double( all_func.size())/NUM);
    vector< vector< Matrix_Rect *> > mrv_array;
    for( int i=0; i<count; i++)
    {
        vector< Matrix_Rect*> ab;
        for( int j=0; j<count; j++)
        {
            ab.push_back( new Matrix_Rect());
        }
        mrv_array.push_back( ab);
    }
    for( int i=0; i < count; i++)
    {
        for( int j=0; j<count; j++)
        {
            mrv_array[ i][ j]->x_start_num=i+1;
            mrv_array[ i][ j]->y_start_num=j+1;
            mrv_array[ i][ j]->size=NUM;
        }
    }
    //разложим все еденицы по необходимым квадратикам
    for( QMap< int, QList< int> >::const_iterator it( mlink.constBegin()); it != mlink.constEnd(); it++)
    {
        int id1=it.key();
        int id2;
        int num1,num2;
        int numx_rect,numy_rect;
        for( QList< int>::const_iterator jt=it.value().constBegin(); jt != it.value().constEnd(); ++jt)
        {
            id2=*jt;
            numx_rect=( int)floor( double( idtonum[ id1])/NUM);
            numy_rect=( int)floor( double( idtonum[ id2])/NUM);
            //cout<<"numx_rect="<<numx_rect<<endl;
            //cout<<"numy_rect="<<numy_rect<<endl;
            //cout<<"id1="<<id1<<endl;
            //cout<<"id2="<<id2<<endl;
            mrv_array[ numx_rect][ numy_rect]->setInThisRect( id1, id2);// in_this_rect[id1].push_back(id2);
        }
    }

    //файлы настройки
    vector< vector< QString> > file_name_mat;
    for( int i=0; i<count; i++)
    {
        vector< QString> ab;
        for( int j=0; j < count; j++)
        {
            QString aaa;
            ab.push_back( aaa );
        }
        file_name_mat.push_back( ab);
    }
    mkdir( ( path+"\\data").toStdString().c_str());
    for( int i=0; i<count; i++)
    {
        for( int j=0; j<count; j++)
        {
            QString filename( "x("+QString::number(i)+")"+"y("+QString::number(j)+").js");
            QFile file( path+"\\data\\"+filename);
            file_name_mat[ i][ j]=filename;
            if ( !file.open( QIODevice::WriteOnly | QIODevice::Text))
                return -1;
            QTextStream out( &file);
            int numx=NUM;
            int numy=NUM;
            if( i == count-1) numx= all_func.size()%NUM;
            if( j == count-1) numy= all_func.size()%NUM;
            out << "var nx=" << QString::number( numx) << ";" << "\n";   // тут надо ввести NUM
            out << "var ny=" << QString::number( numy) << ";" << "\n";
            out << "var dx=" << QString::number( i*NUM) << ";" << "\n";
            out << "var dy=" << QString::number( j*NUM) << ";" << "\n";
            out << "function getX()"<<"\n";
            out << "{" << "\n";
            out << "var x=[";
            QMap< int, QList< int> *> *pmap=&mrv_array[ i][ j]->in_this_rect;
            for( QMap< int, QList< int> *>::iterator it=pmap->begin(); it != pmap->end(); ++it)
            {
                QList< int> * plst=it.value();
                for( QList< int>::iterator jt=plst->begin(); jt != plst->end(); ++jt)
                {
                    out << "[" << QString::number( idtonum[ it.key()]) << "," << QString::number( idtonum[ *jt]) << "]";
                    it++;
                    jt++;
                    if( it != pmap->end()) out << ",";
                    else
                    {
                        if( jt != plst->end() ) out << ",";
                    }
                    it--;
                    jt--;
                }
            }
            out << "];" << "\n";
            out << "return x;" << "\n";
            out << "}" << "\n";
            file.close();
        }
    }

    //записываем имена в список файлов
    QFile file2(path+"\\data\\control.js");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out2( &file2);
    out2 << "var arrayfile=[";
    for( int i=0; i < count; i++)
    {
        if( i != 0) out2 << ",";
        out2 << "[";
        for( int j=0; j < count; j++)
        {
            if( j != 0) out2 << ",";
            out2 << "\"" << file_name_mat[ i][ j] << "\"";
        }
        out2 << "]";
    }
    out2 << "];" << "\n";

    out2 << "var function_name=[";
    for( int i=0; i < full_name.size(); i++)
    {

        sScopeFull s( numtoid[ i]);
        s.refresh();
        out2 << "[\"" << full_name[ numtoid[ i]].replace( QString( "\""), QString("") ) << "\",\"" <<  QString( s.file_path.c_str()).replace(QString("\\"),QString("/")) << "\"]";
        if( i != ( full_name.size()- 1) ) out2 << ",";
    }
    out2 << "];" << "\n";

    out2 << "var num_x_file=arrayfile.length;  // eiee?anoai oaeeia" << "\n";
    out2 << "var num_y_file=num_x_file; 	      // eiee?anoai oaeeia" << "\n";
    out2 << "var screen_size_x=" << NUM_SCREEN << "; //oaeouee ?acia? ieia X" << "\n";
    out2 << "var screen_size_y=" << NUM_SCREEN << "; //oaeouee ?acia? ieia Y" << "\n";
    out2 << "var file_size_x=" << NUM << ";  // eiee?anoai ooieoee a oaeea" << "\n";
    out2 << "var file_size_y=" << NUM << ";  // eiee?anoai ooieoee a oaeea" << "\n";
    out2 << "var max_screen_in_file_x=Math.ceil(file_size_x/screen_size_x); // eiee?anoai ye?aiia a oaeea" << "\n";
    out2 << "var max_screen_in_file_y=Math.ceil(file_size_y/screen_size_y); // eiee?anoai ye?aiia a oaeea" << "\n";
    out2 << "var x_in_file=0;   // eii?aeiaoa a oaeea X" << "\n";
    out2 << "var y_in_file=0;   // eii?aeiaoa a oaeea Y" << "\n";
    out2 << "var tmp_x_pos=num_x_file*screen_size_x;  // eiee?anoai ooieoee"<<"\n";
    out2 << "var tmp_y_pos=num_y_file*screen_size_y;  // eiee?anoai ooieoee"<<"\n";
    out2 << "var tmp_size=20;   // oaeouee ?acia? o?eooa"<<"\n";
    out2 << "var sleep_time=50;  // oaeouey caaa??ea ia?aa ioia?a?aieai iao?eou"<<"\n";
    out2 << "function gotojs()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	gotojs_file();"<<"\n";
    out2 << "}"<<"\n";

    out2 << "function gotojs_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var new_x=document.getElementById(\"go_to_x_js\").value;"<<"\n";
    out2 << "	var new_y=document.getElementById(\"go_to_y_js\").value;"<<"\n";
    out2 << "var x_max_file=document.getElementById(\"sizex\").firstChild;"<<"\n";
    out2 << "var y_max_file=document.getElementById(\"sizey\").firstChild;"<<"\n";
    out2 << "if(new_x>=0 && new_y>=0 && new_x<=(x_max_file.data-1) && new_y<=(y_max_file.data-1) ){}"<<"\n";


    out2 << "	else { return ;}"<<"\n";

    out2 << "	var pos_x= document.getElementById(\"pos_x\").firstChild ;"<<"\n";
    out2 << "	var pos_y= document.getElementById(\"pos_y\").firstChild ;"<<"\n";

    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";

    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	pos_x.data=new_x;"<<"\n";
    out2 << "	pos_y.data=new_y;"<<"\n";
    out2 << "	if(new_x>=0 && new_y>=0 && new_x<=(num_x_file*max_screen_in_file_x-1) && new_y<=(num_y_file*max_screen_in_file_y-1) )"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "		tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		xin_file=pos_x.data%max_screen_in_file_x;"<<"\n";
    out2 << "		yin_file=pos_y.data%max_screen_in_file_y;"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";

    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";

    out2 << "function onup()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;	"<<"\n";
    out2 << "	var y_file=Math.floor(pos_y.data/max_screen_in_file_y);  // текущий файл"<<"\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; 			// текущее окно"<<"\n";
    out2 << "	if(pos_y.data==0) return;"<<"\n";
    out2 << "	pos_y.data--;"<<"\n";
    out2 << "	y_file		=Math.floor(pos_y.data/max_screen_in_file_y);  // текущий файл"<<"\n";
    out2 << "	yin_file	=pos_y.data%max_screen_in_file_y; 			// текущее окно"<<"\n";
    out2 << "	if(yin_file==max_screen_in_file_y-1)"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		onup_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << " 	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";

    out2 << "function onup_file() // ioe?uou aa?oiee oaee"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";

    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";

    out2 << "	if(tmp_y_file>=0)"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onleft()"<<"\n";
    out2 << "{"<<"\n";


    out2 << "	var pos_x	=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var x_file	=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";


    out2 << "	if( pos_x.data==0) return;"<<"\n";
    out2 << "	pos_x.data--;"<<"\n";
    out2 << "	x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";

    out2 << "	if(xin_file== max_screen_in_file_x-1)"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		onleft_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onleft_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";


    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";


    out2 << "	if(tmp_x_file>=0)"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onright()"<<"\n";
    out2 << "{"<<"\n";


    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	var x_max_file=document.getElementById(\"sizex\").firstChild;"<<"\n";


    out2 << "	if( pos_x.data==(x_max_file.data - 1) )"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		return;"<<"\n";
    out2 << "	}"<<"\n";

    out2 << "	pos_x.data++;"<<"\n";

    out2 << "	var x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";

    out2 << "	if(xin_file == 0)"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		onright_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onright_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	if(tmp_x_file<=(num_x_file-1))"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function ondown()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "	var y_max_file=document.getElementById(\"sizey\").firstChild;"<<"\n";
    out2 << "	if( pos_y.data == (y_max_file.data-1) )"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		return;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	pos_y.data++;"<<"\n";
    out2 << "	y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";


    out2 << "	if(yin_file==0)"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		ondown_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function ondown_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";

    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";

    out2 << "	if(tmp_y_file<=(num_y_file-1))"<<"\n";
    out2 << "	{"<<"\n";

    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function endPart()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "		var x=getX();"<<"\n";
    out2 << "		var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "		var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "		var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "		var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "		var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "		var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "		var a_size=screen_size_x;  //?acia? ii x"<<"\n";
    out2 << "		var b_size=screen_size_y;  //?acia? ii y"<<"\n";
    out2 << "		var xs= parseInt(+tmp_x_file)*file_size_x+parseInt(+xin_file)*screen_size_x;"<<"\n";
    out2 << "		var ys= parseInt(+tmp_y_file)*file_size_y+parseInt(+yin_file)*screen_size_y;"<<"\n";
    out2 << "		if( (nx+dx-xs) < screen_size_x )"<<"\n";
    out2 << "		{"<<"\n";
    out2 << "			a_size=(nx+dx-xs);"<<"\n";
    out2 << "		}"<<"\n";
    out2 << "		if( ( ny+dy-ys ) < screen_size_y )"<<"\n";
    out2 << "		{"<<"\n";
    out2 << "			b_size=(ny+dy-ys);"<<"\n";
    out2 << "		}"<<"\n";
    out2 << "		var table1=matrix(a_size ,b_size , xs, ys, x);"<<"\n";
    out2 << "		document.getElementById(\"inserttable\").innerHTML=table1;"<<"\n";
    out2 << "		setmatsize();"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function getElementsByName_iefix(tag, name) {"<<"\n";
    out2 << "     var elem = document.getElementsByTagName(tag);"<<"\n";
    out2 << "     var arr = new Array();"<<"\n";
    out2 << "     for(i = 0,iarr = 0; i < elem.length; i++) {"<<"\n";
    out2 << "          att = elem[i].getAttribute(\"name\");"<<"\n";
    out2 << "          if(att == name) {"<<"\n";
    out2 << "               arr[iarr] = elem[i];"<<"\n";
    out2 << "               iarr++;"<<"\n";
    out2 << "          }"<<"\n";
    out2 << "     }"<<"\n";
    out2 << "     return arr;"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function setmatsize()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function plusplus()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	if(tmp_size<33)tmp_size++;"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function minusminus()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	if(tmp_size>0) tmp_size--;"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{" << "\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}" << "\n";
    out2 << "}" << "\n";


    file2.close();

//создаем индексный файл для матрицы

    QFile file3(path+"\\index.html");
    lsProcessedFiles.push_back(file3.fileName().toStdString());
    if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))

        return -1;
    QTextStream out3( &file3);

    QStringList & list=make_index_file_string(true);

    QList< QString>::iterator it2=list.begin();
    while( it2!=list.end())
    {
        out3 << ( *it2);
        ++it2;
    }
    file3.close();

    return 0;
}




int matrix_cont_legend(QString path ,int NUM,int NUM_SCREEN, std::list <std::string> & lsProcessedFiles )
{
    QMap< int, int> idtonum;
    QMap< int, int> numtoid;
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    QList< int> all_func;
    QMap< int, QString>full_name;
    int scope_number= allscope.size();
    string arg_fun;
    for( int i=0; i< allscope.size(); i++)
    {
        idtonum[ it[ i].id]= i;
        numtoid[ i]= it[ i].id;
        all_func.push_back( it[ i].id);
        get_scope_metadata ( it[ i].id, "args", &arg_fun);
        full_name[ it[ i].id]= QString( it[ i].getQualifiedName().c_str())+ arg_fun.c_str();
    }
    QMap< int, QList< int> > mlink;
    QORMList< sLink> data (CALL_LINK);
    ofstream legend_file( (path+"\\"+"Legend.txt").toStdString().c_str());
    legend_file << "Связь функций по управлению\n";
    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if( 0 != (*j).id && 0 != (*j).idB ) // fuck
            mlink[ (*j).id].push_back( (*j).idB);
//                legend_file<<"("<<idtonum[ (*j).id ]<<", "<<idtonum[ (*j).idB]<<")"<<"   "<<"("<<(*j).id<<", "<<(*j).idB<<")"<<"   "<<full_name[(*j).id].toStdString()<<"   "<<full_name[(*j).idB].toStdString()<< "\n";

    }





    for( QMap< int, QList< int> >::iterator it= mlink.begin(); it!= mlink.end(); it++)
    {
        for( QList< int>::iterator jt=it.value().begin(); jt!= it.value().end(); ++jt)
        {
            legend_file << "(" << idtonum[ it.key() ]
                        << ", " << idtonum[ (*jt)]
                        << ")" << "   " << "(" << it.key()
                        << ", " << (*jt) << ")" << "   "
                        << full_name[it.key()].toStdString()
                        << "   " << full_name[ (*jt)].toStdString() <<  "\n";
        }
    }
    legend_file.close();
    return 1;
}


int matrix_cont( QString path, int NUM, int NUM_SCREEN, list< string> & lsProcessedFiles )
{
    //int NUM=100;
    QMap< int, int> idtonum;
    QMap< int, int> numtoid;
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    QList< int> all_func;
    QMap< int, QString>full_name;
    //cout<<"starting taking scope!!!"<<endl;
    int scope_number=allscope.size();
    for( int i=0; i<allscope.size(); i++)
    {
        idtonum[ it[ i].id]=i;
        numtoid[ i]=it[i].id;
        all_func.push_back( it[ i].id);
        full_name[ it[ i].id]= QString( it[ i].getQualifiedName2().c_str());
    }
    QMap< int, QList< int> > mlink;
    QORMList< sLink> data (CALL_LINK);
    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if((*j).id!=0 && (*j).idB!=0)
            mlink[ (*j).id].push_back( (*j).idB);
    }
    // на этом закончился подготовительный этап
    int count= ceil( double( all_func.size())/NUM);
    vector< vector< Matrix_Rect *> > mrv_array;
    for( int i=0; i<count; i++)
    {
        vector< Matrix_Rect*> ab;
        for( int j=0; j<count; j++)
        {
            ab.push_back( new Matrix_Rect());
        }
        mrv_array.push_back( ab);
    }
    for( int i=0; i < count; i++)
    {
        for( int j=0; j<count; j++)
        {
            mrv_array[ i][ j]->x_start_num=i+1;
            mrv_array[ i][ j]->y_start_num=j+1;
            mrv_array[ i][ j]->size=NUM;
        }
    }
    //разложим все еденицы по необходимым квадратикам
    for( QMap< int, QList< int> >::const_iterator it( mlink.constBegin()); it != mlink.constEnd(); it++)
    {
        int id1=it.key();
        int id2;
        int num1,num2;
        int numx_rect,numy_rect;
        for( QList< int>::const_iterator jt=it.value().constBegin(); jt != it.value().constEnd(); ++jt)
        {
            id2=*jt;
            numx_rect=( int)floor( double( idtonum[ id1])/NUM);
            numy_rect=( int)floor( double( idtonum[ id2])/NUM);
            //cout<<"numx_rect="<<numx_rect<<endl;
            //cout<<"numy_rect="<<numy_rect<<endl;
            //cout<<"id1="<<id1<<endl;
            //cout<<"id2="<<id2<<endl;
            mrv_array[ numx_rect][ numy_rect]->setInThisRect( id1, id2);// in_this_rect[id1].push_back(id2);
        }
    }

    //файлы настройки
    vector< vector< QString> > file_name_mat;
    for( int i=0; i<count; i++)
    {
        vector< QString> ab;
        for( int j=0; j<count; j++)
        {
            QString aaa;
            ab.push_back( aaa);
        }
        file_name_mat.push_back( ab);
    }
    mkdir( ( path+"\\data").toStdString().c_str());
    for( int i=0; i<count; i++)
    {
        for( int j=0; j<count; j++)
        {
            QString filename( "x("+QString::number(i)+")"+"y("+QString::number(j)+").js");
            QFile file( path+"\\data\\"+filename);
            file_name_mat[ i][ j]=filename;
            if ( !file.open( QIODevice::WriteOnly | QIODevice::Text))
                return -1;
            QTextStream out( &file);
            int numx=NUM;
            int numy=NUM;
            if( i == count-1) numx= all_func.size()%NUM;
            if( j == count-1) numy= all_func.size()%NUM;
            out << "var nx=" << QString::number( numx) << ";" << "\n";   // тут надо ввести NUM
            out << "var ny=" << QString::number( numy) << ";" << "\n";
            out << "var dx=" << QString::number( i*NUM) << ";" << "\n";
            out << "var dy=" << QString::number( j*NUM) << ";" << "\n";
            out << "function getX()"<<"\n";
            out << "{" << "\n";
            out << "var x=[";
            QMap< int, QList< int> *> *pmap=&mrv_array[ i][ j]->in_this_rect;
            for( QMap< int, QList< int> *>::iterator it=pmap->begin(); it != pmap->end(); ++it)
            {
                QList< int> * plst=it.value();
                for( QList< int>::iterator jt=plst->begin(); jt != plst->end(); ++jt)
                {
                    if(idtonum.contains(it.key())&& idtonum.contains(*jt))
                    {
                        //out << "[" << QString::number( idtonum[ it.key()]) << "," << QString::number( idtonum[ *jt]) <<",\"\""<< "]";
                        out << "[" << QString::number( idtonum[ *jt]) << "," <<  QString::number( idtonum[ it.key()])  <<",\"\""<< "]";
                        it++;
                        jt++;
                        if( it != pmap->end()) out << ",";
                        else
                        {
                            if( jt != plst->end() ) out << ",";
                        }
                        it--;
                        jt--;
                    }
                }
            }
            out << "];" << "\n";
            out << "return x;" << "\n";
            out << "}" << "\n";
            file.close();
        }
    }

    //записываем имена в список файлов
    QFile file2(path+"\\data\\control.js");
    if (!file2.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;
    QTextStream out2( &file2);
    out2 << "var arrayfile=[";
    for( int i=0; i < count; i++)
    {
        if( i != 0) out2 << ",";
        out2 << "[";
        for( int j=0; j < count; j++)
        {
            if( j != 0) out2 << ",";
            out2 << "\"" << file_name_mat[ i][ j] << "\"";
        }
        out2 << "]";
    }
    out2 << "];" << "\n";

    out2 << "var function_name=[";
    for( int i=0; i < full_name.size(); i++)
    {
        //line.replace('\\','/').constData();
        sScopeFull s( numtoid[ i]);
        s.refresh();
        out2 << "[\"" << full_name[ numtoid[ i]].replace( QString( "\""), QString("") ) << "\",\"" <<  QString( s.file_path.c_str()).replace(QString("\\"),QString("/")) << "\"]";
        if( i != ( full_name.size()- 1) ) out2 << ",";
    }
    out2 << "];" << "\n";

    out2 << "var num_x_file=arrayfile.length;  // eiee?anoai oaeeia" << "\n";
    out2 << "var num_y_file=num_x_file; 	      // eiee?anoai oaeeia" << "\n";
    out2 << "var screen_size_x=" << NUM_SCREEN << "; //oaeouee ?acia? ieia X" << "\n";
    out2 << "var screen_size_y=" << NUM_SCREEN << "; //oaeouee ?acia? ieia Y" << "\n";
    out2 << "var file_size_x=" << NUM << ";  // eiee?anoai ooieoee a oaeea" << "\n";
    out2 << "var file_size_y=" << NUM << ";  // eiee?anoai ooieoee a oaeea" << "\n";
    out2 << "var max_screen_in_file_x=Math.ceil(file_size_x/screen_size_x); // eiee?anoai ye?aiia a oaeea" << "\n";
    out2 << "var max_screen_in_file_y=Math.ceil(file_size_y/screen_size_y); // eiee?anoai ye?aiia a oaeea" << "\n";
    out2 << "var x_in_file=0;   // eii?aeiaoa a oaeea X" << "\n";
    out2 << "var y_in_file=0;   // eii?aeiaoa a oaeea Y" << "\n";
    out2 << "var tmp_x_pos=num_x_file*screen_size_x;  // eiee?anoai ooieoee"<<"\n";
    out2 << "var tmp_y_pos=num_y_file*screen_size_y;  // eiee?anoai ooieoee"<<"\n";
    out2 << "var tmp_size=20;   // oaeouee ?acia? o?eooa"<<"\n";
    out2 << "var sleep_time=50;  // oaeouey caaa??ea ia?aa ioia?a?aieai iao?eou"<<"\n";
    out2 << "function gotojs()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	gotojs_file();"<<"\n";
    out2 << "}"<<"\n";

    out2 << "function gotojs_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var new_x=document.getElementById(\"go_to_x_js\").value;"<<"\n";
    out2 << "	var new_y=document.getElementById(\"go_to_y_js\").value;"<<"\n";
    out2 << "var x_max_file=document.getElementById(\"sizex\").firstChild;"<<"\n";
    out2 << "var y_max_file=document.getElementById(\"sizey\").firstChild;"<<"\n";
    out2 << "if(new_x>=0 && new_y>=0 && new_x<=(x_max_file.data-1) && new_y<=(y_max_file.data-1) ){}"<<"\n";

//out2<<"	if(new_x>=0 && new_y>=0 && new_x<=(num_x_file*max_screen_in_file_x-1) && new_y<=(num_y_file*max_screen_in_file_y-1) ){}"<<"\n";
    out2 << "	else { return ;}"<<"\n";

    out2 << "	var pos_x= document.getElementById(\"pos_x\").firstChild ;"<<"\n";
    out2 << "	var pos_y= document.getElementById(\"pos_y\").firstChild ;"<<"\n";
    //alert("x="+new_x+" y="+new_y+" posx="+pos_x.data+" posy="+pos_y.data);
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";

    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	pos_x.data=new_x;"<<"\n";
    out2 << "	pos_y.data=new_y;"<<"\n";
    out2 << "	if(new_x>=0 && new_y>=0 && new_x<=(num_x_file*max_screen_in_file_x-1) && new_y<=(num_y_file*max_screen_in_file_y-1) )"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "		tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		xin_file=pos_x.data%max_screen_in_file_x;"<<"\n";
    out2 << "		yin_file=pos_y.data%max_screen_in_file_y;"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";

    out2 << "function onup()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;	"<<"\n";
    out2 << "	var y_file=Math.floor(pos_y.data/max_screen_in_file_y);  // текущий файл"<<"\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; 			// текущее окно"<<"\n";
    out2 << "	if(pos_y.data==0) return;"<<"\n";
    out2 << "	pos_y.data--;"<<"\n";
    out2 << "	y_file		=Math.floor(pos_y.data/max_screen_in_file_y);  // текущий файл"<<"\n";
    out2 << "	yin_file	=pos_y.data%max_screen_in_file_y; 			// текущее окно"<<"\n";
    out2 << "	if(yin_file==max_screen_in_file_y-1)"<<"\n";
    out2 << "	{"<<"\n";
    //alert("up");
    out2 << "		onup_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << " 	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";

    out2 << "function onup_file() // ioe?uou aa?oiee oaee"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";

    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";

    out2 << "	if(tmp_y_file>=0)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onleft()"<<"\n";
    out2 << "{"<<"\n";


    out2 << "	var pos_x	=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var x_file	=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";


    out2 << "	if( pos_x.data==0) return;"<<"\n";
    out2 << "	pos_x.data--;"<<"\n";
    out2 << "	x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";

    out2 << "	if(xin_file== max_screen_in_file_x-1)"<<"\n";
    out2 << "	{"<<"\n";
    //alert("left");
    out2 << "		onleft_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onleft_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";


    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";


    out2 << "	if(tmp_x_file>=0)"<<"\n";
    out2 << "	{"<<"\n";
    //alert(\"left\");
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onright()"<<"\n";
    out2 << "{"<<"\n";


    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "	var x_max_file=document.getElementById(\"sizex\").firstChild;"<<"\n";


    out2 << "	if( pos_x.data==(x_max_file.data - 1) )"<<"\n";
    out2 << "	{"<<"\n";
    //alert("end x");
    out2 << "		return;"<<"\n";
    out2 << "	}"<<"\n";

    out2 << "	pos_x.data++;"<<"\n";

    out2 << "	var x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";

    out2 << "	if(xin_file == 0)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		onright_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function onright_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	if(tmp_x_file<=(num_x_file-1))"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function ondown()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "	var y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "	var y_max_file=document.getElementById(\"sizey\").firstChild;"<<"\n";
    out2 << "	if( pos_y.data == (y_max_file.data-1) )"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		return;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	pos_y.data++;"<<"\n";
    out2 << "	y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";

    out2 << "	if(yin_file==0)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		ondown_file();"<<"\n";
    out2 << "		return ;"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	removeElement(\"tablemain\");"<<"\n";
    out2 << "	setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function ondown_file()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "	var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";

    out2 << "	var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "	var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "	if(tmp_y_file<=(num_y_file-1))"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		removeElement(\"110\");"<<"\n";
    out2 << "		loadjscssfile((\"data\\\\\"+arrayfile[tmp_x_file][tmp_y_file]), \"js\",\"110\");"<<"\n";
    out2 << "		removeElement(\"tablemain\");"<<"\n";
    out2 << "		setTimeout(\"endPart()\", sleep_time);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function endPart()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "		var x=getX();"<<"\n";
    out2 << "		var pos_x=document.getElementById(\"pos_x\").firstChild;"<<"\n";
    out2 << "		var pos_y=document.getElementById(\"pos_y\").firstChild;"<<"\n";
    out2 << "		var tmp_x_file=Math.floor(pos_x.data/max_screen_in_file_x); // текущий файл"<<"\n";
    out2 << "		var tmp_y_file=Math.floor(pos_y.data/max_screen_in_file_y); // текущий файл"<<"\n";
    out2 << "		var yin_file=pos_y.data%max_screen_in_file_y; // текущее окно"<<"\n";
    out2 << "		var xin_file=pos_x.data%max_screen_in_file_x; // текущее окно"<<"\n";
    out2 << "		var a_size=screen_size_x;  //?acia? ii x"<<"\n";
    out2 << "		var b_size=screen_size_y;  //?acia? ii y"<<"\n";
    out2 << "		var xs= parseInt(+tmp_x_file)*file_size_x+parseInt(+xin_file)*screen_size_x;"<<"\n";
    out2 << "		var ys= parseInt(+tmp_y_file)*file_size_y+parseInt(+yin_file)*screen_size_y;"<<"\n";
    out2 << "		if( (nx+dx-xs) < screen_size_x )"<<"\n";
    out2 << "		{"<<"\n";
    out2 << "			a_size=(nx+dx-xs);"<<"\n";
    out2 << "		}"<<"\n";
    out2 << "		if( ( ny+dy-ys ) < screen_size_y )"<<"\n";
    out2 << "		{"<<"\n";
    out2 << "			b_size=(ny+dy-ys);"<<"\n";
    out2 << "		}"<<"\n";
    out2 << "		var table1=matrix(a_size ,b_size , xs, ys, x);"<<"\n";
    out2 << "		document.getElementById(\"inserttable\").innerHTML=table1;"<<"\n";
    out2 << "		setmatsize();"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function getElementsByName_iefix(tag, name) {"<<"\n";
    out2 << "     var elem = document.getElementsByTagName(tag);"<<"\n";
    out2 << "     var arr = new Array();"<<"\n";
    out2 << "     for(i = 0,iarr = 0; i < elem.length; i++) {"<<"\n";
    out2 << "          att = elem[i].getAttribute(\"name\");"<<"\n";
    out2 << "          if(att == name) {"<<"\n";
    out2 << "               arr[iarr] = elem[i];"<<"\n";
    out2 << "               iarr++;"<<"\n";
    out2 << "          }"<<"\n";
    out2 << "     }"<<"\n";
    out2 << "     return arr;"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function setmatsize()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function plusplus()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	if(tmp_size<33)tmp_size++;"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "}"<<"\n";
    out2 << "function minusminus()"<<"\n";
    out2 << "{"<<"\n";
    out2 << "	if(tmp_size>0) tmp_size--;"<<"\n";
    out2 << "	var el2=getElementsByName_iefix(\"td\", \"tbtext\");"<<"\n";
    out2 << "	var el3=getElementsByName_iefix(\"th\", \"tbtext\");"<<"\n";
    out2 << "	for(var i=0;i<el3.length;i++)"<<"\n";
    out2 << "	{"<<"\n";
    out2 << "		el2.push(el3[i]);"<<"\n";
    out2 << "	}"<<"\n";
    out2 << "	for(var i=0;i<el2.length;i++)"<<"\n";
    out2 << "	{" << "\n";
    out2 << "		el2[i].style.fontSize=tmp_size+\"px\";"<<"\n";
    out2 << "	}" << "\n";
    out2 << "}" << "\n";


    file2.close();

//создаем индексный файл для матрицы

    QFile file3(path+"\\index.html");
    lsProcessedFiles.push_back(file3.fileName().toStdString());
    if (!file3.open(QIODevice::WriteOnly | QIODevice::Text))
        return -1;

    QTextStream out3( &file3);
    QStringList & list=make_index_file_string(true);
    QList< QString>::iterator it2=list.begin();
    while( it2!=list.end())
    {
        out3 << ( *it2);
        ++it2;
    }
    file3.close();

    return 0;
}

HRET Matrix_cont2( sReportInfo* pa)
{
    QString s( pa->strOutputDirectory.c_str());
    if( pa->do_html_matrix) if( matrix_cont( s, 1000, 100, *( pa->lsProcessedFiles)  )==-1)
        {
            // matrix_cont_legend( s, 1000, 100, *( pa->lsProcessedFiles));
            return -1;
        }
    return 1;
}
HRET Matrix_info2(sReportInfo* pa)
{
    QString s( pa->strOutputDirectory.c_str());
    if( pa->do_html_matrix) if( matrix_info( s, 1000, 100, *( pa->lsProcessedFiles)  )==-1)
        {
            // matrix_info_legend( s, 1000, 100, *( pa->lsProcessedFiles));
            return -1;
        }
    return 1;
}
int Matrix_cont_report_html( int cx, int cy,
                             string  fnames,
                             string pathname,
                             string project_name,
                             string version,
                             list <string> & lsProcessedFiles)
{
    return 0;
    //для y - ка все функции.
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    vector< string> full_name;
    for(int i=0; i < allscope.size(); i++)
    {
        full_name.push_back( it[ i].getQualifiedName());
    }
    int x=allscope.size();
    int y=x;
    //расчет количества html файлов
    int dx=0;
    int dy=0;
    if( 0 != x%cx )
    {
        dx=x/cx+1;
    }
    else
    {
        dx=x/cx;
    }
    if( 0 == x )
    {
        dx=1;
    }
    if( 0 != y%cy)
    {
        dy=y/cy+1;
    }
    else
    {
        dy=y/cy;
    }
    if( y == 0)
    {
        dy=1;
    }
    //зодаем массив аштмл файлов.
    fstream **files=new fstream* [ dy];///******************
    for( int i=0; i < dy; i++)
    {
        files[ i]=new fstream [ dx];
    }
    //создать имяна для html файлов
    string **file_name=new string*[ dy];//********************
    for( int i=0; i < dy; i++)
    {
        file_name[ i]=new string [ dx];
    }
    string **fname=new  string*[ dy];//********************
    for( int i=0; i < dy; i++)
    {
        fname[ i]=new string [ dx];
    }
    //создадим названия для файлов
    for( int j=0; j < dy; j++)
    {
        for( int i=0; i<dx; i++)
        {
            char ax[ 10];
            char ay[ 10];
            char app[ 10];
            itoa( i, ax, 10);
            itoa( j, ay, 10);
            itoa( j*dy+i, app, 10);
            file_name[ j][ i]
            .append( pathname.c_str() )
            .append( "\\")
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
            fname[ j][ i]
            .append( fnames.c_str())
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
        }
    }
    //начнем писать в html файлы матрицы
    for( int j=0; j < dy; j++)
    {
        for( int i=0; i < dx; i++)
        {
            //откроем все файлы html отчетов
            files[ j][ i].open( file_name[ j][ i].c_str(), ios::out);
            lsProcessedFiles.push_back( file_name[ j][ i]);
            if( files[ j][ i].is_open() == false)
            {
                cerr << QObject::tr("fail").toStdString().c_str() << file_name[ j][ i].c_str() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
                return -1;
            }
            //введем заголовок
            files[ j][ i] << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n";
            files[ j][ i] << getCSS().c_str();
            files[ j][ i] << "</HEAD><BODY>" << endl;
            zagolovok_html_file( &files[ j][ i], QObject::tr("MATRICIA SVIAZEI PO YPRAVLENIY").toStdString().c_str(), project_name, version,fname[ j][ i], j*dy+i+1, dy*dx);
            //таблица для таблицы и стрелочек в ней
            files[j][i]<<"<table  class=ta align=\"center\"> <tr> <td>	\n";
            if( j-1 >= 0 && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j-1][ i-1], "..\\up_left.JPG");
            }//влево и вверх
            files[ j][ i] << "</td> <td>";
            if( j-1 >= 0 && i < dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i], "..\\up.JPG");
            }//вверх
            files[ j][ i] << "</td> <td>";
            if( j-1 >= 0 && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i+1], "..\\up_right.JPG");
            }//вверх и вправо
            files[ j][ i] << " </td> </tr><tr> <td>";
            if( j < dy && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j][ i-1], "..\\left.JPG");
            }//влево
            files[ j][ i] <<"</td> <td>\n";
            files[ j][ i] <<"<table border=1 class=matrix >\n";
            //выведем матрицу
            if( is_matrix_region_nonzero( CALL_LINK, i*cx, (i+1)*cx, j*cy, (j+1)*cy))
            {
                for( int gy=0; gy < cy+1; gy++)
                {
                    files[ j][ i] << "<tr>";
                    sScope c;
                    c.id= it[ gy+j*cy-1].id;
                    QORMList< sScopeConnected> tmp2 ( c, CALL_LINK, i*cx, (i+1)*cx);
                    QORMList< sScopeConnected>::iterator jt = tmp2.begin();
                    if( gy != 0)
                    {
                        files[ j][ i] << "<th>" << gy+j*cy << "</th>\n";
                    }
                    for( int gx=0; gx < cx+1; gx++)
                    {
                        if( gy == 0 && gx == 0)
                        {
                            files[ j][ i] << "<th>NN</th>";
                        }
                        if( gy == 0 && gx != 0)
                        {
                            int www1=( int)log10(gx+i*cx);
                            int www2=1+( int)log10(cx);
                            int ggga=www2-www1;
                            files[ j][ i] << "<th align=\"center\">"<<gx+i*cx;
                            for( int ga=0; ga<ggga; ga++)
                            {
                                files[ j][ i] << "&nbsp";
                            }
                            files[ j][ i] << "</th>\n";
                        }
                        if( gy != 0 && gx != 0 )
                        {
                            bool b=( *jt).is_connected;
                            jt++;
                            if( b==true)
                            {
                                files[ j][ i] << "<td title=\"" << QObject::tr("Sviaz mejdy ").toStdString().c_str() << full_name[ gy+j*cy-1] << QObject::tr(" i ").toStdString().c_str() << full_name[ gx+i*cx-1] << "\" align=\"center\" class=one>" << b << "</td>\n";
                            }
                            else files[ j][ i] << "<td title=\"" << QObject::tr("Net sviazi mejdy ").toStdString().c_str() << full_name[ gy+j*cy-1] << QObject::tr(" i ").toStdString().c_str() << full_name[gx+i*cx-1] << "\" align=\"center\" >" << b << "</td>\n";
                        }
                        if( gx+i*cx == x) break;
                    }
                    files[ j][ i] << "</tr>\n";
                    if( gy+j*cy == y) break;
                }
            }
            else
            {
                files[ j][ i] << "<tr>" << "<th>NN</th>" << "<th>" << QObject::tr("s ").toStdString().c_str() << i*cx << QObject::tr(" po ").toStdString().c_str() << (i+1)*cx << "</th>" << "</tr>";
                files[ j][ i] << "<tr>" << "<th>" << QObject::tr("s ").toStdString().c_str() << j*cy << "<br>"<<QObject::tr(" po ").toStdString().c_str() << (j+1)*cy << "</th>" << "<td align=\"center\">0</td>" << "</tr>";
            }
            files[ j][ i] << "</table>\n";
            files[ j][ i] << "</td> <td>";
            if( j < dy && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j][ i+1], "..\\right.JPG");
            }
            else {}//вправо
            files[ j][ i] << " </td> </tr> <tr> <td>";
            if( j+1 < dy && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j+1][ i-1], "..\\down_left.JPG");
            }
            else {}//влево и вниз
            files[ j][ i] << "</td> <td> \n";
            if( j+1 < dy && i < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i], "..\\down.JPG");
            }
            else {}//вниз
            files[ j][ i] << "</td> <td>";
            if( j+1 < dy && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i+1], "..\\down_right.JPG");
            }
            else {}//вниз и вправо
            files[ j][ i] << "</td> </tr>";
            files[ j][ i] << "</table>";
            files[ j][ i] << "</BODY></HEAD></HTML>";
            files[ j][ i].close();
        }
    }
    //удаляем имена файлов.
    for( int i=0; i < dy; i++)
    {
        delete file_name[i];
    }
    delete [] file_name;
    //удаляем имена файлов.
    for( int i=0; i < dy; i++)
    {
        delete fname[i];
    }
    delete [] fname;
    //удаляем объекты файлы.
    for( int i= 0; i < dy; i++)
    {
        delete files[ i];
    }
    delete [] files;
    return 0;
}


int Matrix_info_report_html2( int cx, int cy, string  fnames,
                              string  pathname,
                              string project_name,
                              string version,
                              list < string> & lsProcessedFiles)
{
    return 0;
    //для y - ка все функции.
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    vector< string> full_name;
    for( int i=0; i < allscope.size(); i++)
    {
        full_name.push_back( it[ i].getQualifiedName());
    }
    int x=allscope.size();
    int y=x;
    //расчет количества html файлов
    int dx=0;
    int dy=0;
    if( x%cx != 0)
    {
        dx=x/cx+1;
    }
    else
    {
        dx=x/cx;
    }
    if( x == 0)
    {
        dx=1;
    }
    if( y%cy != 0)
    {
        dy=y/cy+1;
    }
    else
    {
        dy=y/cy;
    }
    if( y == 0)
    {
        dy=1;
    }
    //зодаем массив аштмл файлов.
    fstream **files=new fstream* [ dy];///******************
    for( int i=0; i < dy; i++)
    {
        files[ i]=new fstream [ dx];
    }
    //создать имяна для html файлов
    string **file_name=new string*[ dy];//********************
    for( int i=0; i < dy; i++)
    {
        file_name[ i]=new string [ dx];
    }
    string **fname=new string*[ dy];//********************
    for( int i=0; i < dy; i++)
    {
        fname[ i]=new string [ dx];
    }
    //создадим названия для файлов
    for( int j=0; j < dy; j++)
    {
        for( int i=0; i < dx; i++)
        {
            char ax[ 10];
            char ay[ 10];
            char app[ 10];
            itoa( i, ax, 10);
            itoa( j, ay, 10);
            itoa( j*dy+i, app, 10);
            file_name[ j][ i]
            .append( pathname.c_str() )
            .append( "\\")
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
            fname[ j][ i]
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
        }
    }
    //начнем писать в html файлы матрицы
    for( int j=0; j < dy; j++)
    {
        for( int i=0; i < dx; i++)
        {
            //откроем все файлы html отчетов
            files[ j][ i]
            .open( file_name[ j][ i].c_str(), ios::out);
            lsProcessedFiles
            .push_back( file_name[ j][ i]);
            if( files[ j][ i]
                    .is_open()==false)
            {
                cerr << QObject::tr("fail").toStdString().c_str() << file_name[ j][ i].c_str() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
                return -1;
            }
            string str_buff="";
            //введем заголовок
            str_buff.append( "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n");
            str_buff.append( getCSS());
            str_buff.append( "<script type=\"text/javascript\">\n\
                                        function printtable( x, startx, starty, dx, dy)\n\
					{if(x.length==0)return;\n\
						var lencol=(\"\"+(startx+x.length)).length;\n\
							document.write(\"<table border=1>\");\n\
						for(var yc=0;yc<x.length+1;yc++)\n\
						{\n\
							if(yc==0)\n\
							{\n\
								document.write(\"<tr>\");\n\
								document.write(\"<th>NN</th>\");\n\
								for(var t=startx;t<startx+x[0].length;t++)\n\
								{	\n\
								var numprob=(lencol-(\"\"+(t+1)).length);\n\
									document.write(\"<th>\");\n\
									for(var g=0;g<numprob;g++){\n\
										document.write(\"&nbsp;\");\n\
									}\n\
									document.write(t+1);\n\
									document.write(\"</th>\");\n\
								}\n\
								document.write(\"</tr>\");\n\
							}\n\
							if(yc!=0)\n\
							{\n\
								document.write(\"<tr>\");\n\
								for(var xc=0;xc<x[0].length+1;xc++)\n\
								{\n\
									if(xc==0)\n\
									{\n\
										document.write(\"<th>\"+(starty+yc)+\"</th>\");\n\
									}\n\
									else\n\
									{\n\
										var tit;\n\
										var clss;\n\
										if(x[yc-1][xc-1][0]){clss=\" class=\\\"one\\\" \"; tit=\"Связь между \"+namey[yc-1]+\" и \"+namex[xc-1]+\" по элементу \"+x[yc-1][xc-1][1];}\n\
                                        else{clss=\"\";tit=\"Нет связь между \"+namey[yc-1]+\" и \"+namex[xc-1];}\n\
										document.write(\"<td align=center\" +\" title=\\\"\"+tit + \"\\\"\"+ clss+\" >\"+x[yc-1][xc-1][0]+\"</td>\");\n\
									};\n\
								}\n\
								document.write(\"</tr>\");\n\
							}\n\
						}\n\
						document.write(\"</table>\");\n\
					}\n\
							</script>\n");
            str_buff.append( "</HEAD><BODY>\n");
            files[j][i]<<str_buff;
            str_buff="";

            zagolovok_html_file( &files[ j][ i], QObject::tr("MATRICI SVIAZEI PO INFORMACII").toStdString().c_str(), project_name, version, fname[ j][ i], j*dy+i+1, dy*dx);

            //таблица для таблицы и стрелочек в ней
            files[j][i]<<"<table  class=ta align=\"center\"> <tr> <td>	\n";
            if( j-1 >= 0 && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j-1][ i-1],"..\\up_left.JPG");
            }//влево и вверх
            files[j][i]<<"</td> <td>";
            if( j-1 >= 0 && i < dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i],"..\\up.JPG");
            }//вверх
            files[ j][ i] << "</td> <td>";
            if( j-1 >= 0 && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i+1],"..\\up_right.JPG");
            }//вверх и вправо
            files[ j][ i]<<" </td> </tr><tr> <td>";
            if( j < dy && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j][ i-1], "..\\left.JPG");
            }//влево
            files[ j][ i] << " </td><td>";
            //выведем матрицу
            if( is_matrix_region_nonzero( INFORMATION_LINK, i*cx, (i+1)*cx, j*cy, (j+1)*cy))
            {
                files[ j][ i] << "<script type=\"text/javascript\">";
                files[ j][ i] << "var starty=" << j*cy << ";\n";
                files[ j][ i] << "var startx=" << i*cx << ";\n";
                files[ j][ i] << "var cy=" << cy << ";\n";
                files[ j][ i] << "var cx=" << cx << ";\n";
                files[ j][ i] << "var namex=[";
                string ss1;
                for( int h= i*cx; h < (i+1)*cx; h++)
                {
                    int i1= it[ h].id;
                    get_scope_metadata( i1, "args", &ss1);
                    string sasa= it[ h].getQualifiedName() +ss1;
                    int k=0;
                    while( sasa.find( "\"", k)!= string::npos)
                    {
                        k=sasa.find( "\"", k);
                        sasa.erase( k, 1);
                        sasa.insert( k, "\\\"");
                        k=k+2;
                    }
                    files[ j][ i] << "\"" << sasa << "\"";
                    if( (h != x-1)&&
                            (h < ((i+1)*cx-1)) )
                    {
                        files[j][i] << ",";
                    }
                    if( h == x-1) break;
                }
                files[ j][ i] << "];\n";
                string ss2;
                files[ j][ i] << "var namey=[";
                for( int h=j*cy; h < (j+1)*cy; h++)
                {
                    int i1=it[h].id;
                    get_scope_metadata( i1, "args", &ss2);
                    string sasa= it[h].getQualifiedName()+ss2;
                    int k=0;
                    while( sasa.find( "\"", k) != string::npos)
                    {
                        k=sasa.find( "\"", k);
                        sasa.erase( k, 1);
                        sasa.insert( k, "\\\"");
                        k=k+2;
                    }
                    files[ j][ i] << "\"" << sasa << "\"";
                    if( h != y-1 && h < ((j+1)*cy-1) )
                    {
                        files[ j][ i] << ",";
                    }
                    if( h == y-1) break;
                }
                files[ j][ i] << "];\n";
                bool test=false;
                string str="var x=[";
                for( int gy=0; gy < cy; gy++)
                {
                    sScope c;
                    c.id=it[gy+j*cy].id;
                    QORMList< sScopeConnected> tmp2 ( c, INFORMATION_LINK, i*cx, (i+1)*cx);
                    QORMList< sScopeConnected>::iterator jt = tmp2.begin();
                    str.append( "[");
                    for( int gx=0; gx < cx; gx++)
                    {
                        str.append( "[");
                        test=true;
                        if( jt[ gx+i*cx].is_connected)
                        {
                            str.append( "1,\"" + (*jt).getCommonObject()+"\"");
                        }
                        else
                        {
                            str.append( "0");
                        }
                        str.append( "]");

                        if( ((gx+i*cx) != (x-1))&& (gx < (cx-1) ) )
                        {
                            str.append(",");
                        }
                        if( gx+i*cx == x-1) break;
                    }
                    str.append( "]");
                    if( gy+j*cy != y-1 && (gy < cy-1) )
                    {
                        str.append(",");
                    }
                    if( gy+j*cy == y-1 ) break;
                }
                if( test)
                {
                    str.append( "];\n printtable(x,startx,starty,cx,cy);");
                }
                str.append( "</script>");
                files[ j][ i] << str;
                str="";
            }
            else
            {
                files[ j][ i] << html_breaket( "TABLE", "border=1 class=matrix",
                                               html_tr(
                                                   html_th( "№")+
                                                   html_th( QObject::tr("s ").toStdString()+inttostring( i*cx)+QObject::tr(" po ").toStdString()+inttostring( (i+1)*cx) )
                                               )+
                                               html_tr(
                                                   html_th( QObject::tr("s ").toStdString()+inttostring( j*cy)+QObject::tr(" po ").toStdString()+inttostring( (j+1)*cy))+
                                                   html_breaket( "TD", "align=\"center\"", "0")
                                               )
                                             ) << endl;
            }
            files[ j][ i] << "</td> <td>";
            if( j < dy && i+1 < dx )
            {
                files[ j ][ i] << strelka ( fname[ j][ i+1], "..\\right.JPG");
            }//вправо
            files[ j][ i] << " </td> </tr> <tr> <td>";
            if( j+1 < dy && i-1 >= 0)
            {
                files[j][i]<<strelka(fname[j+1][i-1],"..\\down_left.JPG");
            }//влево и вниз
            files[ j][ i] << "</td> <td> \n";
            if( j+1 < dy && i < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i], "..\\down.JPG");
            }//вниз
            files[ j][ i] << "</td> <td>";
            if( j+1 < dy && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i+1], "..\\down_right.JPG");
            }//вниз и вправо
            files[ j][ i] << "</td> </tr>";
            files[ j][ i] << "</table>";
            files[ j][ i] << "</BODY></HEAD></HTML>";
            files[ j][ i].close();
        }
    }
    //удаляем имена файлов.
    for( int i=0; i < dy; i++)
    {
        delete file_name[ i];
    }
    delete [] file_name;
    //удаляем имена файлов.
    for( int i=0; i < dy; i++)
    {
        delete fname[ i];
    }
    delete [] fname;
    //удаляем объекты файлы.
    for( int i=0; i < dy; i++)
    {
        delete files[ i];
    }
    delete [] files;
    return 0;
}


// олд
int Matrix_info_report_html( int cx, int cy, string  fnames,
                             string  pathname,
                             string project_name,
                             string version,
                             list <string> & lsProcessedFiles)
{
    return 0;
    //для y - ка все функции.
    QORMList< sScope> allscope ;
    QORMList< sScope>::iterator it=allscope.begin();
    vector< string> full_name;
    for( int i=0; i < allscope.size(); i++)
    {
        full_name.push_back( it[ i].getQualifiedName());
    }
    int x=allscope.size();
    int y=allscope.size();
    //расчет количества html файлов
    int dx=0;
    int dy=0;
    if( x%cx != 0)
    {
        dx=x/cx+1;
    }
    else
    {
        dx=x/cx;
    }
    if( x == 0) dx=1;
    if( y%cy != 0)
    {
        dy=y/cy+1;
    }
    else
    {
        dy=y/cy;
    }
    if( y == 0) dy=1;
    //зодаем массив аштмл файлов.
    fstream **files=new  fstream* [ dy];
    for( int i=0; i < dy; i++)
    {
        files[ i]=new fstream [ dx];
    }
    //создать имяна для html файлов
    string **file_name=new string*[ dy];
    for( int i=0; i < dy; i++)
    {
        file_name[ i]=new string [ dx];
    }
    //создать имяна для html файлов
    string **fname=new string*[ dy];
    for( int i=0; i < dy; i++)
    {
        fname[ i]=new string [ dx];
    }
    //создадим названия для файлов
    for( int j=0; j < dy; j++)
    {
        for( int i=0; i < dx; i++)
        {
            char ax[ 10];
            char ay[ 10];
            char app[ 10];
            itoa( i, ax, 10);
            itoa( j, ay, 10);
            itoa( j*dy+i, app, 10);
            file_name[ j][ i]
            .append( pathname.c_str() )
            .append( "\\")
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
            fname[ j][ i]
            .append( fnames.c_str() )
            .append( ax)
            .append( ".")
            .append( ay)
            .append( ".")
            .append( app)
            .append( ".html");
        }
    }
    //начнем писать в html файлы матрицы
    for( int j=0; j < dy; j++)
    {
        for( int i=0; i < dx; i++)
        {
            files[ j][ i].open( file_name[ j][ i].c_str(), ios::out);
            lsProcessedFiles.push_back( file_name[ j][ i]);
            if( files[ j][ i].is_open() == false)
            {
                cerr << QObject::tr("fail").toStdString().c_str() << file_name[ j][ i].c_str() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
                return -1;
            }
            //введем заголовок
            files[ j][ i] << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n";
            files[ j][ i] << getCSS().c_str();
            files[ j][ i] << "</HEAD><BODY>" << endl;
            zagolovok_html_file( &files[ j][ i], QObject::tr("MATRICIA SVIAZEI PO INFORMACII").toStdString().c_str(), project_name, version, fname[ j][ i], j*dy+i+1, dy*dx);
            //таблица для таблицы и стрелочек в ней
            if( j-1 >= 0 && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j-1][ i-1], "..\\up_left.JPG");
            }//влево и вверх
            files[ j][ i] << "</td> <td>";
            if( j-1 >= 0 && i < dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i], "..\\up.JPG");
            }//вверх
            files[ j][ i] << "</td> <td>";
            if( j-1 >= 0 && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j-1][ i+1], "..\\up_right.JPG");
            }//вверх и вправо
            files[ j][ i] << " </td> </tr><tr> <td>";
            if( j < dy && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j][ i-1], "..\\left.JPG");
            }//влево

            files[ j][ i] << "<table border=1 align=center class=matrix >\n";
            if( is_matrix_region_nonzero( INFORMATION_LINK, i*cx, (i+1)*cx, j*cy, (j+1)*cy))
            {
                //выведем матрицу
                for( int gy=0; gy<cy+1; gy++)
                {
                    files[ j][ i] << "<tr>";
                    if( gy != 0)
                    {
                        files[ j][ i] << "<th>" << gy+j*cy << "</th>\n";
                    }
                    sScope c;
                    c.id=it[ gy+j*cy-1].id;
                    QORMList< sScopeConnected> tmp2 ( c, INFORMATION_LINK, i*cx, (i+1)*cx);
                    QORMList< sScopeConnected>::iterator jt = tmp2.begin();
                    for( int gx=0; gx < cx+1; gx++)
                    {
                        if( gy == 0 && gx == 0)
                        {
                            files[ j][ i] << "<th>NN</th>";
                        }
                        if( gy == 0 && gx != 0)
                        {
                            int www1= ( int)log10(gx+i*cx);
                            int www2=1+(int)log10(cx);
                            int ggga=www2-www1;
                            files[ j][ i] << "<th align=\"center\">" << gx+i*cx;
                            for( int ga=0; ga<ggga; ga++)
                            {
                                files[ j][ i] << "&nbsp";
                            }
                            files[ j][ i] << "</th>\n";
                        }
                        if( gy != 0 && gx != 0)
                        {
                            bool b=(*jt).is_connected;
                            if( b == true)
                            {
                                files[ j][ i]
                                        << "<td title=\"" << QObject::tr("Sviaz mejdy ").toStdString().c_str()<< full_name[ gy+j*cy-1] << QObject::tr(" i ").toStdString() << full_name[ gx+i*cx-1]
                                        << QObject::tr(" po jbiekty " ).toStdString().c_str()<< (*jt).getCommonObject() << "\" align=\"center\" class=one>" << b << "</td>\n";
                            }
                            else files[ j][ i]
                                        << "<td title=\"" << QObject::tr("Net sviazi mejdy ").toStdString().c_str() << full_name[ gy+j*cy-1]<< QObject::tr(" i ").toStdString() << full_name[gx+i*cx-1] << full_name[ gx+i*cx-1] << "\" align=\"center\" >" << b << "</td>\n";
                            ++jt;
                        }
                        if( gx+i*cx == x ) break;
                    }
                    files[ j][ i] << "</tr>\n";
                    if( gy+j*cy == y) break;
                }

            }
            else
            {
                files[ j][ i] << html_tr(
                                  html_th( "№")+
                                  html_th( QObject::tr("s ").toStdString().c_str()+inttostring(i*cx)+QObject::tr(" po ").toStdString().c_str()+inttostring((i+1)*cx) )
                              );
                files[ j][ i] << html_tr(
                                  html_th( QObject::tr("s ").toStdString().c_str()+inttostring(j*cy)+QObject::tr(" po ").toStdString().c_str()+inttostring((j+1)*cy))+
                                  html_breaket( "TD", "align=\"center\"", "0")
                              );
            }
            files[ j][ i] << "</table>\n";
            files[ j][ i] << "</td> <td>";
            if( j < dy && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j][ i+1], "..\\right.JPG");

            }//вправо
            files[ j][ i] << " </td> </tr> <tr> <td>";
            if( j+1 < dy && i-1 >= 0)
            {
                files[ j][ i] << strelka( fname[ j+1][ i-1], "..\\down_left.JPG");

            }//влево и вниз
            files[ j][ i] << "</td> <td> \n";
            if( j+1 < dy && i < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i], "..\\down.JPG");

            }//вниз
            files[ j][ i] << "</td> <td>";
            if( j+1 < dy && i+1 < dx)
            {
                files[ j][ i] << strelka( fname[ j+1][ i+1], "..\\down_right.JPG");

            }//вниз и вправо
            files[ j][ i]<<"</td> </tr>";
            files[ j][ i]<<"</table>";
            files[ j][ i]<<"</BODY></HEAD></HTML>";
            files[ j][ i].close();
        }
    }
    //удаляем имена файлов.
    for( int i=0; i < dy; i++)
    {
        delete file_name[i];
    }
    delete [] file_name;
    //удаляем имена файлов.
    for( int i=0; i < dy; i++)
    {
        delete fname[ i];
    }
    delete [] fname;
    //удаляем объекты файлы.
    for(int i=0; i < dy; i++)
    {
        delete files[ i];
    }
    delete [] files;
    return 0;
}

int make_cont_Matrix_report(    string file, string path,
                                string project_name, string version,
                                list < string>& lsProcessedFiles)
{
    //для y - ка все функции.
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it=allscope.begin();
    fstream of;
    //имя файла
    string sf= path+"\\"+file+"txt";
    string fname= file+"txt";
    of.open( sf.c_str(), fstream::out);
    lsProcessedFiles.push_back( sf);
    if( of.is_open()==false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << "matrix1.txt" << QObject::tr(" ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }
    of << "                "<<QObject::tr("MATRICIA SVIAZIEI PO YPRAVLIENIY").toStdString().c_str() << endl;
    of << QObject::tr("Proekt: ").toStdString().c_str() << project_name.c_str() << endl;
    of << version.c_str() << endl;
    of << QObject::tr("Imia faila: ").toStdString().c_str() << fname << endl;
    of << endl << endl << endl << endl;
    int matlen=allscope.size();
    int size_a;
    if( matlen != 0) size_a=int( log( matlen));
    else
    {
        size_a=0;
    }
    char buffer[ 10];
    memset( buffer, ' ', 10);
    of << QObject::tr("Primechanie: nomera stolbciov matrici chitaiutsia sverhy-vniz.").toStdString().c_str()<<endl<<endl;
    //выводим первую строку с номером строк.
    for( int j=size_a-1; j != -1; j--)
    {
        of << setw( size_a+2) << ' ';
        for( int i=0; i<matlen; i++)
        {
            stringstream ss;
            ss << (i+1);
            string temps=ss.str();
            string s(temps.rbegin(),temps.rend());
            if( size_t( j) < s.size())
            {
                of << s.at(j);
            }
            else
            {
                of << " ";
            }
        }
        of << endl;
    }
    of << endl;
    //вывод матрицы
    for(int i=0; i < allscope.size() ; i++)
    {
        of << setw( size_a) << right << i+1 <<"  ";
        sScope c;
        c.id=it[ i].id;
        QORMList< sScopeConnected> tmp2 ( c, CALL_LINK);
        QORMList< sScopeConnected>::iterator jt = tmp2.begin();
        string str="";
        for( int j=0; j < allscope.size() ; j++)
        {
            str.append( jt[ j].is_connected? "1" : "0");
        }
        of << str << endl;
        str="";
    }
    return 0;
}
int make_info_Matrix_report(std::string file,std::string path,
                            std::string project_name,std::string version,
                            std::list <std::string> & lsProcessedFiles)
{
    //для y - ка все функции.
    QORMList< sScope > allscope;
    QORMList< sScope >::iterator it=allscope.begin();
    fstream of;
    //имя файла
    string sf= path+"\\"+file+"txt";
    string fname= file+"txt";
    of.open( sf.c_str(), fstream::out);
    lsProcessedFiles.push_back( sf);
    if( of.is_open() == false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << "matrix1.txt" << QObject::tr(" ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }
    of << "                "<<QObject::tr("MATRICIA SVIAZEI PO INFORMACII").toStdString().c_str() << endl;
    of << QObject::tr("Proekt: ").toStdString().c_str() << project_name.c_str() << endl;
    of << version.c_str()<< endl;
    of << QObject::tr("Imia faila: ").toStdString().c_str() << fname << endl;
    of << endl << endl << endl << endl;
    int matlen=allscope.size();
    int size_a;
    if( matlen != 0) size_a= int( log( matlen));
    else
    {
        size_a=0;
    }
    //выводим первую строку с номером строк.Примечание: номера столбцов матрицы читаются сверху-вниз.
    of << QObject::tr("Primechanie: nomera stolbciov matrici chitaiutsia sverhy-vniz.").toStdString().c_str()<<endl;
    for( int j=size_a-1; j != -1; j--)
    {
        of << setw( size_a+2) <<' ';
        for( int i=0; i < matlen; i++)
        {
            stringstream ss;
            ss << (i+1);
            string temps= ss.str();
            string s( temps.rbegin(), temps.rend());

            if( size_t( j ) < s.size() )
            {
                of << s.at( j );
            }
            else
            {
                of << " ";
            }
        }
        of << endl;
    }
    of << endl;
    //вывод матрицы
    long size_ff=allscope.size();
    for( int i=0; i < allscope.size() ; i++)
    {
        of << setw( size_a) << right << i+1 << "  ";
        sScope c;
        c.id=it[ i].id;
        QORMList< sScopeConnected> tmp2 ( c, INFORMATION_LINK);
        QORMList< sScopeConnected>::iterator jt = tmp2.begin();
        string str="";
        for( int j=0; j < allscope.size() ; j++)
        {
            str.append( jt[ j].is_connected ? "1" : "0");
        }
        of << str << endl;
        str="";
    }
    return 0;
}
