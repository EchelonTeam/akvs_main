/*
 * graf.cpp
 *
 *  Created on: 17.12.2008
 *      Author: javamain
 */
#include "libservice.h"

#include <queue>
#include <string>
#include <list>
#include <QSet>
#include <QTime>
#include <Queue>
#include <QMap>
#include <ctime>
#include <QStringList>
#include <set>
using namespace std;
HRET GraphReport( sReportInfo* pa)
{
    QTime delta_time;
    delta_time.start();
    cout << QObject::tr("(log):Sozdanie otcheta - grafa vizova.").toStdString().c_str()<< endl;   // (log):Создание отчета - граф вызова.
    //cout << "Making GraphReport..." << endl;
    GraphReportf_f2_for_critical_path( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName, pa->strOutputDirectory, 20000, *( pa->lsProcessedFiles));
    cout << QObject::tr("Vipolneno za").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl; // Выполнено за   сек.
    return 1;
}

HRET GraphReport2( sReportInfo* pa)
{
    QTime delta_time;
    delta_time.start();

    cout << QObject::tr("(log):Sozdanie otcheta - graf vizova.").toStdString().c_str()<< endl;  // (log):Создание отчета - граф вызова.
    //cout << "Making GraphReport..." << endl;

    GraphReportf_f2_for_critical_path( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName, pa->strOutputDirectory, 20000, *( pa->lsProcessedFiles));
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl; // Выполнено за      сек.
    return 1;
}




// для сортировки элементов по убыванию
bool compare_nocase (std::pair< sFile , int > first, std::pair< sFile, int>  second)
{
    return (first.second > second.second);
}



int GraphReportf_f(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles)
{
    string colorstr[]=
    {
        "coral",
        "crimson",
        "aliceblue",
        "blanchedalmond",
        "deeppink",
        "greenyellow",
        "blue",
        "firebrick",
        "blueviolet",
        "hotpink",
        "cadetblue",
        "gainsboro",
        "indianred",
        "lightyellow",
        "cornflowerblue",
        "lightpink",
        "palegoldenrod",
        "darkslateblue",
        "honeydew",
        "lightsalmon",
        "yellow",
        "deepskyblue",
        "ivory",
        "maroon",
        "yellowgreen",
        "dodgerblue",
        "lavender",
        "mediumvioletred",
        "indigo",
        "lavenderblush",
        "orangered",
        "lightblue",
        "lemonchiffon",
        "palevioletred",
        "chartreuse",
        "lightskyblue",
        "linen",
        "pink",
        "darkgreen",
        "lightslateblue",
        "mintcream",
        "red",
        "darkolivegreen",
        "mediumblue",
        "mistyrose",
        "salmon",
        "darkseagreen",
        "mediumslateblue",
        "moccasin",
        "tomato",
        "forestgreen",
        "midnightblue",
        "violetred",
        "green",
        "navy",
        "oldlace",
        "greenyellow",
        "navyblue",
        "papayawhip",
        "lawngreen",
        "powderblue",
        "peachpuff",
        "beige",
        "lightseagreen",
        "royalblue",
        "seashell",
        "brown",
        "limegreen",
        "skyblue",
        "burlywood",
        "mediumseagreen",
        "slateblue",
        "thistle",
        "chocolate",
        "mediumspringgreen",
        "steelblue",
        "wheat",
        "mintcream",
        "khaki",
        "olivedrab",
        "peru",
        "palegreen",
        "blueviolet",
        "rosybrown",
        "seagreen",
        "darkorchid",
        "saddlebrown",
        "springgreen",
        "darkviolet",
        "darkslategray",
        "yellowgreen",
        "dimgray",
        "sienna",
        "mediumorchid",
        "gray",
        "tan",
        "mediumpurple",
        "gray",
        "aquamarine",
        "mediumvioletred",
        "lightgray",
        "cyan",
        "orchid",
        "lightslategray",
        "darkorange",
        "darkturquoise",
        "palevioletred",
        "slategray",
        "orange",
        "lightcyan",
        "plum",
        "orangered",
        "mediumaquamarine",
        "purple",
        "mediumturquoise",
        "violet",
        //"black",
        "paleturquoise",
        "violetred"
    };

    string name_for_aph_file;


    //зададим имя  файлов в строках.
    fstream fstr;
    fstr.open( "graff_deb.txt", ios::out);
    name_for_aph_file= path+"\\"+base_file_name+"dge";


    //файл куда будет сброшен граф
    fstream p_aph_file;


    p_aph_file.open( name_for_aph_file.c_str(), ios::out);
    lsProcessedFiles.push_back( name_for_aph_file);

    if( p_aph_file.is_open()==false)
    {
        cerr << QObject::tr("fail").toStdString().c_str() << name_for_aph_file << QObject::tr("ne otkrilsia").toStdString().c_str() << endl; //файл не открылся
        return -1;
    }

    // начинаем новый граф
    p_aph_file << "digraph a { \n"
               "node [style=filled];\n";

    QSet<int> idset;

    sScopeFull cur;
    string str;

    int numlater= 20;
    srand ( time( NULL) );

    QORMList< sFile> sfile;
    QORMList< sFile>::iterator it= sfile.begin();
    int ti=0;
    int valu;
    // строем кластеры
    for( QORMList< sFile>::iterator it=sfile.begin(); it != sfile.end(); ++it,ti++)
    {
        //начинаем новый кластер
        p_aph_file << "subgraph cluster" << ti << " {\n";

        valu= rand()%( sizeof( colorstr)/sizeof( colorstr[ 0])) ;

        p_aph_file << "style=filled;\n color=" << colorstr[ valu ] << ";\n";
        p_aph_file << "label= \"" << "file: " << QString(( *it).getFileName().c_str()).replace("\"","\\\"").toStdString() << "\"\n";

        // в каждый кластер подписываем ns_scope
        QORMList < sScope> lsFuncs( ( ID_FILE)( *it).id, NS_SCOPE);
        for( QORMList< sScope>::iterator jt = lsFuncs.begin(); jt != lsFuncs.end(); ++jt)
        {
            string ss1;
            int i1=( *jt).id;
            ak_vs_db::get_scope_metadata( i1, "args", &ss1);
            string fname= ak_vs_db::get_scope_full_qualified_name(i1)+ss1;
            string fname2;

            for( size_t i=0; i < ( fname.size()/ numlater) +1; i++)
            {
                fname2.append( fname.substr( i*numlater, numlater));
                fname2.append( "\\n");
            }

            idset.contains(i1);
            p_aph_file << "\"" << i1 << "\"[ label=\"" << QString(fname2.c_str()).replace("\"","\\\"").toStdString() << "\",shape=box, color=green];\n";



        }
        // подписываем exe_scpoe
        QORMList < sScope> lsFuncs2( ( ID_FILE)( *it).id, EXE_SCOPE);
        for( QORMList< sScope>::iterator jt = lsFuncs2.begin(); jt != lsFuncs2.end(); ++jt)
        {
            string ss1;

            int i1= ( *jt).id;
            ak_vs_db::get_scope_metadata ( i1, "args", &ss1);

            string fname= ak_vs_db::get_scope_full_qualified_name( i1)+ss1;
            string fname2;

            for( size_t i=0; i<(fname.size()/numlater)+1; i++)
            {
                fname2.append( fname.substr( i*numlater, numlater));
                fname2.append( "\\n");
            }
            QString tmpstr=QString(fname2.c_str()).replace("\"","\\\"");
            idset.contains(i1);
            p_aph_file << "\"" << i1 << "\"[ label=\"" << tmpstr.toStdString() << "\",shape=box, color=green];\n";

        }
        // закрываем кластер
        p_aph_file << "}\n";
    }

    // пишем динамические связи
    QORMList< sLinkComprasion> tmp2 ( CALL_LINK, DYNAMIC_LINK, EXE_SCOPE);
    QORMList< sLinkComprasion>::iterator it2 = tmp2.begin();

    fstr << "id1 id2 F S\n";

    for(; it2!=tmp2.end(); ++it2)
    {
        fstr << ( *it2).id << " " << ( *it2).id2 << " " << ( *it2).hasFirst << " " << ( *it2).hasSecond<<"\n";
        if(  ( *it2).hasFirst != 0  )
        {
            string ss2;
            string ss1;

            int i1= ( *it2).id;
            int i2= ( *it2).id2;

            ak_vs_db::get_scope_metadata( i1, "args", &ss1);
            ak_vs_db::get_scope_metadata( i2, "args", &ss2);

            // связь статическая
            p_aph_file << " edge [color=black];\n";
            p_aph_file << "		" << "\"" << i1 << "\" -> \"" << ( i2) << "\" ;\n";
            // связь динамическая
            if( ( ( *it2).hasSecond != 0) )
            {
                p_aph_file << " edge [color=red];\n";
                p_aph_file << "		" << "\"" << i1 << "\" -> \"" << i2 << "\" ;\n";
            }
        }
    }

    fstr.close();
    p_aph_file << "} \n";

    p_aph_file.close();

    return 0;
}



int GraphReportf_f2(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles)
{
    string colorstr[]=
    {
        "coral",
        "crimson",
        "aliceblue",
        "blanchedalmond",
        "deeppink",
        "greenyellow",
        "blue",
        "firebrick",
        "blueviolet",
        "hotpink",
        "cadetblue",
        "gainsboro",
        "indianred",
        "lightyellow",
        "cornflowerblue",
        "lightpink",
        "palegoldenrod",
        "darkslateblue",
        "honeydew",
        "lightsalmon",
        "yellow",
        "deepskyblue",
        "ivory",
        "maroon",
        "yellowgreen",
        "dodgerblue",
        "lavender",
        "mediumvioletred",
        "indigo",
        "lavenderblush",
        "orangered",
        "lightblue",
        "lemonchiffon",
        "palevioletred",
        "chartreuse",
        "lightskyblue",
        "linen",
        "pink",
        "darkgreen",
        "lightslateblue",
        "mintcream",
        "red",
        "darkolivegreen",
        "mediumblue",
        "mistyrose",
        "salmon",
        "darkseagreen",
        "mediumslateblue",
        "moccasin",
        "tomato",
        "forestgreen",
        "midnightblue",
        "violetred",
        "green",
        "navy",
        "oldlace",
        "greenyellow",
        "navyblue",
        "papayawhip",
        "lawngreen",
        "powderblue",
        "peachpuff",
        "beige",
        "lightseagreen",
        "royalblue",
        "seashell",
        "brown",
        "limegreen",
        "skyblue",
        "burlywood",
        "mediumseagreen",
        "slateblue",
        "thistle",
        "chocolate",
        "mediumspringgreen",
        "steelblue",
        "wheat",
        "mintcream",
        "khaki",
        "olivedrab",
        "peru",
        "palegreen",
        "blueviolet",
        "rosybrown",
        "seagreen",
        "darkorchid",
        "saddlebrown",
        "springgreen",
        "darkviolet",
        "darkslategray",
        "yellowgreen",
        "dimgray",
        "sienna",
        "mediumorchid",
        "gray",
        "tan",
        "mediumpurple",
        "gray",
        "aquamarine",
        "mediumvioletred",
        "lightgray",
        "cyan",
        "orchid",
        "lightslategray",
        "darkorange",
        "darkturquoise",
        "palevioletred",
        "slategray",
        "orange",
        "lightcyan",
        "plum",
        "orangered",
        "mediumaquamarine",
        "purple",
        "mediumturquoise",
        "violet",
        // "black",
        "paleturquoise",
        "violetred"
    };

    string name_for_aph_file;
    QSet<int> idset;

    //зададим имя  файлов в строках.
    fstream fstr;
    fstr.open( "graff_deb.txt", ios::out);
    name_for_aph_file= path+"\\"+base_file_name+"dge";


    //файл куда будет сброшен граф
    fstream p_aph_file;


    p_aph_file.open( name_for_aph_file.c_str(), ios::out);

    lsProcessedFiles.push_back( name_for_aph_file);

    if( p_aph_file.is_open()==false)
    {
        cerr << QObject::tr("fail").toStdString().c_str() << name_for_aph_file << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;   // файл  не открылся
        return -1;
    }

    // начинаем новый граф
    p_aph_file << "digraph a { \n"
               "node [style=filled];\n";

    sScopeFull cur;
    string str;

    int numlater= 20;
    srand ( time( NULL) );

    QORMList< sFile> sfile;
    QORMList< sFile>::iterator it= sfile.begin();
    int ti=0;
    int valu;
    // строем кластеры
    list<pair<int, int> > edg2;
    for( QORMList< sFile>::iterator it=sfile.begin(); it != sfile.end(); ++it)
    {
        //начинаем новый кластер
        p_aph_file << "subgraph cluster" << ti << " {\n";
        ti++;
        valu= rand()%( sizeof( colorstr)/sizeof( colorstr[ 0])) ;

        p_aph_file << "style=filled;\n color=" << colorstr[ valu ] << ";\n";
        p_aph_file << "label= \"" << "file: " << QString(( *it).getFileName().c_str()).replace("\"","\\\"").toStdString() << "\";\n";

        // в каждый кластер подписываем ns_scope
        QORMList < sScope> lsFuncs( ( ID_FILE)( *it).id, NS_SCOPE);
        for( QORMList< sScope>::iterator jt = lsFuncs.begin(); jt != lsFuncs.end(); ++jt)
        {
            string ss1;
            int i1=( *jt).id;
            ak_vs_db::get_scope_metadata( i1, "args", &ss1);

            string fname= ( *jt).getQualifiedName2()+ss1;
            string fname2;
            for( size_t i=0; i < ( fname.size()/ numlater) +1; i++)
            {
                fname2.append( fname.substr( i*numlater, numlater));
                fname2.append( "\\n");
            }
            QString tmpstr=QString(fname2.c_str()).replace("\"","\\\"");
            idset.contains(i1);
            p_aph_file << "\"" << i1 << "\"[ label=\"" << tmpstr.toStdString() << "\",shape=ellipse  , color=gold];\n";

            p_aph_file << "   subgraph cluster" << ti << "{" << endl;
            ti++;
            valu= rand()%( sizeof( colorstr)/sizeof( colorstr[ 0])) ;
            p_aph_file << "   style=filled;\n   color=" << colorstr[ valu ] << ";\n";
            p_aph_file << "   label= \"" << "func: " << QString(fname.c_str()).replace("\"","\\\"").toStdString() << "\";\n";
            // найти все дерево данного скоупа
            queue< sScope> tree;
            sScope sf(i1);
            sf.refresh();
            tree.push( sf);

            for(; !tree.empty() ;) // по всем ветвям
            {
                QORMList< sScope> data( tree.front());
                for( QORMList< sScope>::iterator jtt = data.begin(); jtt != data.end(); ++jtt)
                {
                    edg2.push_back( pair<int,int>( tree.front().id, (*jtt).id ));
                    tree.push( *jtt);
                }

                if(i1 != tree.front().id)
                {
                    idset.contains( tree.front().id);
                    p_aph_file << "   \"" << tree.front().id << "\"[ label=\"" << QString( tree.front().getQualifiedName2().c_str()).replace("\"","\\\"").toStdString() << "\",shape=polygon, color=gold];\n";
                }
                tree.pop();
            }
            p_aph_file << "   }" << endl;

        }
        // подписываем exe_scpoe
        QORMList < sScope> lsFuncs2( ( ID_FILE)( *it).id, EXE_SCOPE);
        for( QORMList< sScope>::iterator jt = lsFuncs2.begin(); jt != lsFuncs2.end(); ++jt)
        {
            string ss1;
            int i1= ( *jt).id;
            ak_vs_db::get_scope_metadata ( i1, "args", &ss1);

            string fname= ( *jt).getQualifiedName2()+ss1;
            string fname2;
            for( size_t i=0; i<(fname.size()/numlater)+1; i++)
            {
                fname2.append( fname.substr( i*numlater, numlater));
                fname2.append( "\\n");
            }
            QString tmpstr=QString(fname2.c_str()).replace("\"","\\\"");
            idset.contains(i1);
            p_aph_file << "\"" << i1 << "\"[ label=\"" << tmpstr.toStdString() << "\", shape=ellipse, color=gold];\n";

            p_aph_file << "   subgraph cluster" << ti << " {" << endl;
            ti++;
            valu= rand()%( sizeof( colorstr)/sizeof( colorstr[ 0])) ;
            p_aph_file << "   style=filled;\n   color=" << colorstr[ valu ] << ";\n";

            p_aph_file << "   label= \"" << "func: " << QString(fname.c_str()).replace("\"","\\\"").toStdString() << "\";\n";
            // найти все дерево данного скоупа
            queue< sScope> tree;
            sScope sf( i1);
            sf.refresh();
            tree.push( sf);
            for(; !tree.empty() ;) // по всем ветвям
            {
                QORMList< sScope> data( tree.front());
                for( QORMList< sScope>::iterator jtt = data.begin(); jtt != data.end(); ++jtt)
                {
                    edg2.push_back( pair<int,int>( tree.front().id, (*jtt).id ));
                    tree.push( *jtt);
                }

                if(i1 != tree.front().id)
                {
                    idset.contains(tree.front().id);
                    p_aph_file << "   \"" << tree.front().id << "\"[ label=\"" << QString( tree.front().getQualifiedName2().c_str()).replace("\"","\\\"").toStdString() << "\",shape=polygon, color=gold];\n";
                }
                tree.pop();
            }
            p_aph_file << "   }" << endl;
        }
        // закрываем кластер
        p_aph_file << "   }\n";
    }

    // пишем динамические связи
    QORMList< sLinkComprasion> tmp2 ( CALL_LINK, DYNAMIC_LINK, EXE_SCOPE);
    QORMList< sLinkComprasion>::iterator it2 = tmp2.begin();

    fstr << "id1 id2 F S\n";

    for(; it2!=tmp2.end(); ++it2)
    {
        fstr << ( *it2).id << " " << ( *it2).id2 << " " << ( *it2).hasFirst << " " << ( *it2).hasSecond<<"\n";
        if(  ( *it2).hasFirst != 0  )
        {
            string ss2;
            string ss1;

            int i1= ( *it2).id;
            int i2= ( *it2).id2;

            ak_vs_db::get_scope_metadata( i1, "args", &ss1);
            ak_vs_db::get_scope_metadata( i2, "args", &ss2);

            // связь статическая
            p_aph_file << " edge [color=black];\n";
            p_aph_file << "		" << "\"" << i1 << "\" -> \"" << ( i2) << "\" ;\n";
            // связь динамическая
            if( ( ( *it2).hasSecond != 0) )
            {
                p_aph_file << " edge [color=red];\n";
                p_aph_file << "		" << "\"" << i1 << "\" -> \"" << i2 << "\" ;\n";
            }
        }
    }

    for( list<pair<int,int> >::iterator it=edg2.begin(); it!= edg2.end(); it++)
    {
        p_aph_file << " edge [color=black];\n";
        p_aph_file << "		" << "\"" << it->first << "\" -> \"" <<  it->second << "\" ;\n";
    }

    fstr.close();
    p_aph_file << "} \n";

    p_aph_file.close();

    return 0;
}

// настройка для графа.
class Graph_config
{
private :
    Graph_config()
    {
        // по умолчанию будем делать по 200 файлам и по первым

        this->num_file=200;
    }

public:

    static Graph_config& Instance()
    {
        static Graph_config me;
        return me;
    }

    void Conf( int num )
    {
        Instance().num_file = num;
    }

    int  num_file;   // количество файлов.
};

// цвета в строках
string colorstr[]=
{
    "coral",
    "crimson",
    "aliceblue",
    "blanchedalmond",
    "deeppink",
    "greenyellow",
    "blue",
    "firebrick",
    "blueviolet",
    "hotpink",
    "cadetblue",
    "gainsboro",
    "indianred",
    "lightyellow",
    "cornflowerblue",
    "lightpink",
    "palegoldenrod",
    "darkslateblue",
    "honeydew",
    "lightsalmon",
    "yellow",
    "deepskyblue",
    "ivory",
    "maroon",
    "yellowgreen",
    "dodgerblue",
    "lavender",
    "mediumvioletred",
    "indigo",
    "lavenderblush",
    "orangered",
    "lightblue",
    "lemonchiffon",
    "palevioletred",
    "chartreuse",
    "lightskyblue",
    "linen",
    "pink",
    "darkgreen",
    "lightslateblue",
    "mintcream",
    "red",
    "darkolivegreen",
    "mediumblue",
    "mistyrose",
    "salmon",
    "darkseagreen",
    "mediumslateblue",
    "moccasin",
    "tomato",
    "forestgreen",
    "midnightblue",
    "violetred",
    "green",
    "navy",
    "oldlace",
    "greenyellow",
    "navyblue",
    "papayawhip",
    "lawngreen",
    "powderblue",
    "peachpuff",
    "beige",
    "lightseagreen",
    "royalblue",
    "seashell",
    "brown",
    "limegreen",
    "skyblue",
    "burlywood",
    "mediumseagreen",
    "slateblue",
    "thistle",
    "chocolate",
    "mediumspringgreen",
    "steelblue",
    "wheat",
    "mintcream",
    "khaki",
    "olivedrab",
    "peru",
    "palegreen",
    "blueviolet",
    "rosybrown",
    "seagreen",
    "darkorchid",
    "saddlebrown",
    "springgreen",
    "darkviolet",
    "darkslategray",
    "yellowgreen",
    "dimgray",
    "sienna",
    "mediumorchid",
    "gray",
    "tan",
    "mediumpurple",
    "gray",
    "aquamarine",
    "mediumvioletred",
    "lightgray",
    "cyan",
    "orchid",
    "lightslategray",
    "darkorange",
    "darkturquoise",
    "palevioletred",
    "slategray",
    "orange",
    "lightcyan",
    "plum",
    "orangered",
    "mediumaquamarine",
    "purple",
    "mediumturquoise",
    "violet",
//"black",
    "paleturquoise",
    "violetred"
};

std::list< sFile>  GetBadFile()
{
    // колличество файлов по умолчанию для отчета
    const int DEFAULT_NUM=50;

    std::list< sFile> badfiles;

    std::list< std::pair< sFile, int> > bad_files_num;  // пара файл - колличество уязвимостей в нем.

    QORMList< sFile> sfile;

    for(QORMList< sFile>::iterator it=sfile.begin() ; it != sfile.end(); ++it)
    {
        bad_files_num.push_back( pair<sFile, int>( *it, ak_vs_db::count_sa( (*it).id)) );
    }

    // сортируем по убыванию
    bad_files_num.sort( compare_nocase);

    // берем из базы, по скольким файлам будем строить отчет
    int num_graph_files = ak_vs_db::get_scope_metadata(0,"grafvalue");

    //cout << "write in data base: " << num_graph_files << endl;

    if(  num_graph_files  ==  0 )
    {
        num_graph_files = DEFAULT_NUM;
    }

    Graph_config::Instance().Conf( num_graph_files);


    // отправляем файлы на отчеты , отправляем первые N файлов на отчет
    int num=0;
    for( std::list< pair<sFile, int> >::iterator it=bad_files_num.begin(); it != bad_files_num.end(); it++)
    {
        if ( num++ ==  Graph_config::Instance().num_file ) break;
        badfiles.push_back( it->first);
    }
    if( badfiles.size() == 0 )
    {
        // если нет уязвимостей то выбираем первые N файлов
        QORMList< sFile>::iterator jt = sfile.begin();
        int y=0;
        for(; jt != sfile.end(); jt++)
        {
            //cout << 1 << endl;
            if( y++ < Graph_config::Instance().num_file )
                badfiles.push_back( *jt );
            else
                break;
        }

    }

    return badfiles;
}

// сделать для sScopa нормальное имя
QStringList   make_name(sScope s)
{
    string ss1;
    int numlater= 20;
    int i1=s.id;
    ak_vs_db::get_scope_metadata( i1, "args", &ss1);

    string fname= s.getQualifiedName2()+ss1;
    string fname2;
    for( size_t i=0; i < ( fname.size()/ numlater) +1; i++)
    {
        fname2.append( fname.substr( i*numlater, numlater));
        fname2.append( "\\n");
    }
    QStringList lst;
    lst << QString(fname2.c_str()).replace("\"","\\\"");
    lst << QString(fname.c_str() );
    return lst;
}




int GraphReportf_f2_for_critical_path(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles)
{


    string name_for_aph_file;

    //зададим имя  файлов в строках.

    name_for_aph_file = path + "\\" + base_file_name + "dge";

    //===================================================

    // берем из базы файлы для построения графа
    std::list< sFile> badfiles=GetBadFile();

    //файл куда будет сброшен граф
    fstream p_aph_file;


    p_aph_file.open( name_for_aph_file.c_str(), ios::out);

    lsProcessedFiles.push_back( name_for_aph_file);

    if( p_aph_file.is_open() == false)
    {
        cerr << QObject::tr("fail").toStdString().c_str() << name_for_aph_file << QObject::tr("ne otkrilsia").toStdString().c_str() << endl; //  файл  не открылся
        return -1;
    }


    // начинаем новый граф
    p_aph_file << "digraph a { \n"
               "node [style=filled];\n";
    sScopeFull cur;

    string str;
    //int numlater= 20;

    srand ( time( NULL) );

    QSet<int> idset;

    int ti=0;
    int valu;

    // строем кластеры
    std::set< pair< int , int > > edg2;




    // cout << "Make graph for " << badfiles.size() << " files." << endl;

    for( std::list< sFile>::iterator it=badfiles.begin(); it != badfiles.end(); ++it)
    {
        // cout << "File " << it->file_path << endl;

        //начинаем новый кластер
        p_aph_file << "subgraph cluster" << ti << " {\n";
        ti++;

        // для каждого файла делаем свой квадрат своего цвета.
        valu= rand()%( sizeof( colorstr)/sizeof( colorstr[ 0])) ;
        p_aph_file << "style=filled;\n color=" << colorstr[ valu ] << ";\n";
        p_aph_file << "label= \"" << "file: " << QString( ( *it).getFileName().c_str()).replace("\"","\\\"").toStdString() << "\";\n";

        // в каждый кластер подписываем ns_scope
        eScopeType array[]= {EXE_SCOPE, NS_SCOPE };

        for( int i=0; i< 2; i++)
        {
            QORMList < sScope> lsFuncs( ( ID_FILE)( *it).id , array[ i ] );
            for( QORMList< sScope>::iterator jt = lsFuncs.begin(); jt != lsFuncs.end(); ++jt)
            {

                int i1=(*jt).id;
                QStringList lst=make_name(*jt);
                QString  tmpstr=lst.at( 0 );
                idset.insert( i1 );
                // тут пишем ИД функции  и ее емя для построения кружка
                p_aph_file << "\"" << i1 << "\"[ label=\"" << tmpstr.toStdString() << "\",shape=ellipse  , color=gold];\n";

                // все что находится в данной функции - делаем для нее квадратик
                p_aph_file << "   subgraph cluster" << ti << "{" << endl;
                ti++;
                valu= rand() % ( sizeof( colorstr)/sizeof( colorstr[ 0])) ;
                p_aph_file << "   style=filled;\n   color=" << colorstr[ valu ] << ";\n";
                p_aph_file << "   label= \"" << "func: " << QString( lst.at( 1 ) ).replace("\"","\\\"").toStdString() << "\";\n";

                // распечатаем все дерево данного скоупа
                queue< sScope> tree;
                sScope sf( i1);
                sf.refresh();
                tree.push( sf );

                for(; !tree.empty() ;) // по всем ветвям
                {
                    QORMList< sScope > data( tree.front() );
                    for( QORMList< sScope >::iterator jtt = data.begin(); jtt != data.end(); ++jtt )
                    {
                        edg2.insert( edg2.begin(), pair<int,int>( tree.front().id, (*jtt).id )); // id родителя , id ребенка
                        tree.push( *jtt);
                    }
                    idset.insert( tree.front().id );
                    if(i1 != tree.front().id)  // на всякий случай
                        p_aph_file << "   \"" << tree.front().id << "\"[ label=\"" << QString( tree.front().getQualifiedName2().c_str() ).replace( "\"", "\\\"" ).toStdString() << "\",shape=polygon, color=gold];\n";
                    tree.pop();
                }

                p_aph_file << "   }" << endl;
            }
        }

        // закрываем кластер
        p_aph_file << "   }\n";
    }

    if( badfiles.size() != 0)
    {
        // пишем динамические связи
        QORMList< sLinkComprasion> tmp2 ( CALL_LINK, DYNAMIC_LINK, EXE_SCOPE );
        for(QORMList< sLinkComprasion>::iterator it2 = tmp2.begin(); it2 != tmp2.end(); ++it2)
        {
            if(  ( *it2).hasFirst != 0  )
            {
                int i1= ( *it2).id;
                int i2= ( *it2).id2;

                if( idset.contains(i1) &&
                        idset.contains( i2)  )
                {
                    // связь статическая
                    p_aph_file << " edge [color=black];\n";
                    p_aph_file << "		" << "\"" << i1 << "\" -> \"" << i2 << "\" ;\n";

                    // связь динамическая
                    if( ( *it2).hasSecond != 0 )
                    {
                        p_aph_file << " edge [color=red];\n";
                        p_aph_file << "		" << "\"" << i1 << "\" -> \"" << i2 << "\" ;\n";
                    }
                }
            }
        }
        // статические связи
        for( std::set< pair< int, int> >::iterator it=edg2.begin(); it != edg2.end(); it++)
        {
            // тут распечатываются все связи
            //badfiles.
            p_aph_file << " edge [color=black];\n";
            p_aph_file << "		" << "\"" << it->first << "\" -> \"" <<  it->second << "\" ;\n";
        }
    }
    p_aph_file << "} \n";
    p_aph_file.close();
    return 0;
}




string & graph( sScopeFull cur, string &str, stack< string> &stk)
{
    //начинаем собирать всех детей в контейнет.
    QORMList< sScopeFull> data ( cur); // Вот он, контейнер
    for( QORMList< sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //если мы встречаем класс , то делаем обертку для класа, и делаем рекурсию
        //если функция, то отрисовываем ее и забиваем , продолжаем строить следующий скоуп
        if( (*j).system_type == EXE_SCOPE)
        {
            string ss1;
            ak_vs_db::get_scope_metadata( ( *j).id, "args", &ss1);
            stk.push( string( "\"" + ak_vs_db::get_scope_full_qualified_name( ( *j).id) + ss1 + "\";\n") );
        }

        //если встречаем информационный скоуп, то отрисовываем его
        if( (*j).system_type == INFO_SCOPE)
        {
            string ss1;
            ak_vs_db::get_scope_metadata( ( *j).id, "args", &ss1);
            stk.push( string( "\"" + ak_vs_db::get_scope_full_qualified_name( ( *j).id) + ss1+"\";\n"));
        }
    }
    return str;
}
