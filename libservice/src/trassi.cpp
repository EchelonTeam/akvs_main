/*
 * trassi.cpp
 *
 *  Created on: 17.12.2008
 *      Author: javamain
 */
#include "libservice.h"
#include <vector>
#include <string>
#include "htmlfunction.h"
#include <dir.h>
#include <QMap>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <libdb_interface.h>
#include <locale.h>
#include <QString>
#include <QtGlobal>
#include <QTranslator>
#include <QCoreApplication>

using namespace alex;
using namespace ak_vs_db;
using namespace std;


//��������������� ��������� ������ ��� ������� �������� ����� ������� ����� ������� - ������� javascript
//todo
namespace alex
{
class Elem
{
public:
    Elem() {}
    Elem( int _id, string _name, string _info): id(_id), name( _name), info( _info) {}
    int id; // id ��������
    string name; // ���
    string info; // ���������� �� ��������
};
}

string get_info( int id)
{
    // �������� ��� ������ �������:
    // 1. ������ ��� �������.
    // 2. ������ ������ �������.
    // 3. ������ ������.
    // 4. ��� �����.
    QString func_info;

    string ss1;
    sScopeFull s(id);
    s.refresh();

    get_scope_metadata(id, "arg",&ss1);
    if(s.pos_x1 < 0 or s.pos_x2 < 0 )
    {
        func_info = QString(QObject::tr("Polnoe imia:%1 <br> Imia faila:%2").toStdString().c_str()).
                    arg( (s.getQualifiedName2() + ss1).c_str() ).
                    arg( s.file_path.c_str());
    }
    else
    {
        func_info = QString(QObject::tr("Polnoie imia:%1 <br> Imia faila:%2 <br> Stroka nachala:%3 <br> Stroka koncia:%4").toStdString().c_str()).
                    arg( (s.getQualifiedName2() + ss1).c_str() ).
                    arg( s.file_path.c_str()).
                    arg( s.pos_x1).
                    arg( s.pos_x2);
    }

    return func_info.toStdString();
}


// ������� �������� ����� ������� ��� ������� ����� ������� - �������.
int TraceReportf_f_new_javascript(
    string project_name,       // ��� �������
    string version,            // ��� ������
    string base_file_name,     // ������ ����� �����
    string path,               // ����
    int mod_for_html_file,          // ������ ��� ������
    list< string> & lsProcessedFiles) // ��������� ��� ����� � ��������� ������
{
    setlocale ( LC_ALL,".1251");
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it= allscope.begin();
    QMap< int, QString> full_name;   // id - full_name
    int scope_number= allscope.size();
    for( int i=0; i< allscope.size(); i++)
    {
        full_name[ it[ i].id]= QString( it[ i].getQualifiedName().c_str());
    }


    QMap< int, QList< Elem> > mlink;
    QORMList< sLink> data ( CALL_LINK);

    sqlAdapterForScopesWithParent adapt;
    adapt.GetByPerentAndType( EXE_SCOPE, eFirstScope);
    for(; adapt.next();)
    {
        Elem a( adapt.getId(), full_name[ adapt.getId()].toStdString(), get_info( adapt.getId() ));
        mlink[ eFirstScope ].push_back( a);
    }


    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        Elem a( ( *j).idB, full_name[ ( *j).idB].toStdString(), get_info( ( *j).idB));
        mlink[ ( *j).id].push_back( a);
    }
    // �� ���� ���������� ���������������� ����

    // ������������� � ����
    QString report_path= ( path+"\\"+base_file_name+".html").c_str();

    ofstream out( report_path.toStdString().c_str());
    if( out.is_open()!= true)
    {
        return -1;
    }

    out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">" << "\n";
    out << "<html dir=\"ltr\" xmlns=\"http://www.w3.org/1999/xhtml\"  lang=\"ru\">" << "\n";
    out << "<head>" << "\n";
    out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=win-1251\" />" << "\n";
    out << "<title>����� �� �������</title>" << "\n";
    out << "<style>" << "\n";
    out << "		td.one {background:#cccccc;}" << "\n";
    out << "		td { border-color:#000000;}" << "\n";
    out << "		body {" << "\n";
    out << "			margin: 20px;" << "\n";
    out << "			background: #FFFFFF;" << "\n";
    out << "			font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;" << "\n";
    out << "			font-size: 13px;" << "\n";
    out << "			color: #000000;" << "\n";
    out << "		}" << "\n";
    out << "		h1, h2, h3 {" << "\n";
    out << "			margin: 0;" << "\n";
    out << "			font-family: Georgia, \"Times New Roman\", Times, serif;" << "\n";
    out << "			font-weight: normal;" << "\n";
    out << "			color: #000000;" << "\n";
    out << "		}" << "\n";
    out << "		h1 {" << "\n";
    out << "			font-size: 2em;" << "\n";
    out << "		}" << "\n";
    out << "		h2 {" << "\n";
    out << "			font-size: 1.6em;" << "\n";
    out << "		}" << "\n";
    out << "		h3 {" << "\n";
    out << "			font-size: 1.6em;" << "\n";
    out << "		}" << "\n";
    out << "		p, ul, ol {" << "\n";
    out << "			margin-top: 0;" << "\n";
    out << "			line-height: 180%;" << "\n";
    out << "		}" << "\n";
    out << "		ul, ol {" << "\n";
    out << "		}" << "\n";
    out << "		a {" << "\n";
    out << "			text-decoration: none;" << "\n";
    out << "			border-bottom: 1px dotted #999999;" << "\n";
    out << "			color: #000;" << "\n";
    out << "		}" << "\n";
    out << "		table{" << "\n";
    out << "		padding: 20px;" << "\n";
    out << "		border-collapse:collapse;" << "\n";
    out << "			border-color:Black; " << "\n";
    out << "			border-width: 1px 1px 1px 1px;" << "\n";
    out << "		}" << "\n";
    out << "		th {border-color:Black;background-color: #CCCCCC;}" << "\n";

    out << "#main_table " << "\n";
    out << "{" << "\n";
    out << "	border: 1px solid black;" << "\n";
    out << "	width  : 90%;" << "\n";
    out << "}" << "\n";

    out << "#main_table2 " << "\n";
    out << "{" << "\n";
    out << "	border-color: #fff;" << "\n";
    out << "	width : 90%;" << "\n";
    out << "	height : 90%;" << "\n";
    out << "}" << "\n";

    out << "#functions_td" << "\n";
    out << "{" << "\n";
    out << "	padding-left: 20px;" << "\n";
    out << "	border: 1px solid black;" << "\n";
    out << "	width : 30%;" << "\n";
    out << "}" << "\n";

    out << "#info_td" << "\n";
    out << "{" << "\n";
    out << "        vertical-align: top;" << "\n";
    out << "	border: 1px solid black;" << "\n";
    out << "	width : 70%;" << "\n";
    out << "}" << "\n";
    out << "#midl_td" << "\n";
    out << "{	" << "\n";
    out << "	border-top-color: #0cc;" << "\n";
    out << "	border-bottom-color: white;" << "\n";
    out << "	width : 2%;" << "\n";
    out << "}" << "\n";
    out << ".goluboi" << "\n";
    out << "{" << "\n";
    out << "	color : #00f;" << "\n";
    out << "	font-size : 14px; " << "\n";
    out << "	font-style : bold; " << "\n";
    out << "}" << "\n";
    out << "</style>" << "\n";



    out << "<script language=\"javascript\">" << "\n";
    out << "var mx=" << "\n";
    out << "{" << "\n";
// ����� � ���� ���� �� id ��������
// ������ ���� ��������� ������ ��������� ��

    for( QMap< int, QList< Elem> >::const_iterator it( mlink.constBegin()); it!= mlink.constEnd(); )
    {
        // ���������� �����
        int id1= it.key();
        Elem el;

        out << id1 << ":"<< "\n";
        out << "["<< "\n";
        for( QList< Elem>::const_iterator jt= it.value().constBegin(); jt!= it.value().constEnd();)
        {
            // ���������� ������ ��������
            el = *jt;
            QString sss= QString( "%1")
                         .arg( QString::number( el.id) );
            out << sss.toStdString().c_str();
            ++jt;
            if( jt != it.value().constEnd() ) out << ",";
        }
        out << "]";
        it++;
        if( it != mlink.constEnd()) out << "," << "\n";
    }

    out << "};"<< "\n";
    out << "var all_func_info={";
    QORMList< sScope>::iterator it2=allscope.begin();
    QString tmps;
    int tnum=0;
    for( int i=0; i < allscope.size(); i++ )
    {
        tnum=it2[i].id;
        tmps=QString("%1:[\"%2\",\"%3\"]\n").arg( QString::number( tnum))
             .arg( full_name[ tnum])
             .arg( get_info(tnum).c_str() );
        out << tmps.toStdString();
        if(i+1 < allscope.size() ) out << ",";
    }
    out << "};" << "\n";
    out << "var func_ids=[\n";
    for( int i=0; i < allscope.size(); i++ )
    {
        out << it2[ i].id ;
        if(i+1 < allscope.size() ) out << ",";
    }
    out << "];" << "\n";

    out << "var temp_id=0;"  << "\n";
    out << "// ���������� ���������� ��� ���� "  << "\n";
    out << "var path_func_array=new Array();"  << "\n";
    out << "// ���������� ���������� ��� ������"  << "\n";
    out << "var func_array=new Array();"  << "\n";
    out << "// ���������� ���������� ��� ������ ���������� � ������� "  << "\n";
    out << "var func_info=\"\";"  << "\n";
    out << ""  << "\n";
    out << "// ������� �� ������� ����� ������ ��� ��� ������ ������ �������"  << "\n";
    out << "function make_functions_array( func_array)"  << "\n";
    out << "{"  << "\n";
    out << "        var string=\"<ol type=\\\"1\\\">\";"  << "\n";
    out << "        for( var i=0; i<func_array.length; i++ )"  << "\n";
    out << "        {"  << "\n";
    out << "                string+= \"<li id=\\\"\"+"  << "\n";
    out << "                func_array[ i][ 1]+ \"\\\" onclick=\\\"click(\"+"  << "\n";
    out << "                func_array[ i][ 1]+ \",\"+ i+ \");\\\" >\"+"  << "\n";
    out << "                func_array[ i][ 2]+"  << "\n";
    out << "                                 \"</li>\";"  << "\n";
    out << "        }"  << "\n";
    out << "        string+= \"</ol>\";"  << "\n";
    out << "        return string;"  << "\n";
    out << "}"  << "\n";
    out << "// ����� � ������� x �������"   << "\n";
    out << "// � �������� ������������ id"  << "\n";
    out << "function get_with_perent_id(x,id)"  << "\n";
    out << "{"  << "\n";
    out << "        var tmp=new Array();"  << "\n";
    out << "        for( var i=0; i< x.length; i++)"  << "\n";
    out << "        {"  << "\n";
    out << "                if( x[ i][ 0]== id)"  << "\n";
    out << "                {"  << "\n";
    out << "                        var tt= new Array();"  << "\n";
    out << "                        tt.push( x[ i][ 0]);"  << "\n";
    out << "                        tt.push( x[ i][ 1]);"  << "\n";
    out << "                        tt.push( x[ i][ 2]);"  << "\n";
    out << "                        tt.push( x[ i][ 3]);"  << "\n";
    out << "                        tmp.push( tt);"  << "\n";
    out << "                }"  << "\n";
    out << "        }"  << "\n";
    out << "        return tmp;"  << "\n";
    out << "}"  << "\n";
    out << "function get_with_perent_id_x(mx,id)"  << "\n";
    out << "{"  << "\n";
    out << "        var array=mx[id];"  << "\n";
    out << "        if( array==null)"  << "\n";
    out << "        {"  << "\n";
    out << "                return [];"  << "\n";
    out << "        }"  << "\n";
    out << "        var tmp=new Array();"  << "\n";
    out << "        for( var i=0; i< array.length; i++)"  << "\n";
    out << "        {"  << "\n";
    out << "                var a=new Array();"  << "\n";
    out << "                a.push(id);"  << "\n";
    out << "                a.push(array[ i]);"  << "\n";
    out << "                a.push(all_func_info[ array[ i]][0] );"  << "\n";
    out << "                a.push(all_func_info[ array[ i]][1] );"  << "\n";
    out << "                tmp.push(a);"  << "\n";
    out << "        }"  << "\n";
    out << "        return tmp;"  << "\n";
    out << "}"  << "\n";

    out << " function get_allfunc()" << "\n";
    out << "{"<< "\n";
    out << "        var tmp=new Array();"<< "\n";
    out << "        for( var i=0; i< func_ids.length; i++)"<< "\n";
    out << "        {"<< "\n";
    out << "                var a=new Array();"<< "\n";
    out << "                a.push(0);"<< "\n";
    out << "                a.push(func_ids[i]);"<< "\n";
    out << "                a.push(all_func_info[ func_ids[i]][0] );"<< "\n";
    out << "                a.push(all_func_info[ func_ids[i]][1] );"<< "\n";
    out << "                tmp.push(a);"<< "\n";
    out << "        }"<< "\n";
    out << "        return tmp;"<< "\n";
    out << "}"<< "\n";

    out << "// ������������ ���� �������"  << "\n";
    out << "function draw_path()"  << "\n";
    out << "{"  << "\n";
    out << "// ������������ ��� ����"  << "\n";
    out << "        var str= make_func_path( path_func_array);"  << "\n";
    out << "        var func_path= document.getElementById( \"functions_path\");"  << "\n";
    out << "        func_path.innerHTML= str;"  << "\n";
    out << "}"  << "\n";
    out << "// ���������� ������ �������"  << "\n";
    out << "function draw_func_list()"  << "\n";
    out << "{"  << "\n";
    out << "        var a= document.getElementById( \"functions\");"  << "\n";
    out << "        var str= make_functions_array( func_array);"  << "\n";
    out << "        a.innerHTML= str;"  << "\n";
    out << "}"  << "\n";
    out << "// �� ������� �� ������� � ������ �������"  << "\n";
    out << "// ��� ������ ��������� � ������ ������� ����"  << "\n";
    out << "// ������ ���������� ������ �������"  << "\n";
    out << "function click( id, i)" << "\n";
    out << "{"  << "\n";
    out << "        //alert( \"id=\"+id+\" i=\"+i);"  << "\n";
    out << "// ��������� ����� �������� � ����"  << "\n";
    out << "        var g= new Array();"  << "\n";
    out << "        g.push( func_array[ i][ 0]);"  << "\n";
    out << "        g.push( func_array[ i][ 1]);"  << "\n";
    out << "        g.push( func_array[ i][ 2]);"  << "\n";
    out << "        g.push( func_array[ i][ 3]);"  << "\n";
    out << "        path_func_array.push( g);"  << "\n";
    out << "// ������������ ��� ����"  << "\n";
    out << "        draw_path();"  << "\n";
    out << "// ������� ������� ��� ������"  << "\n";
    out << "        func_array= get_with_perent_id_x( mx, id);"  << "\n";
    out << "// �������������� ������ �������"  << "\n";
    out << "        draw_func_list();"  << "\n";
    out << "   var temp_temp_temp=\"<p class=\\\"goluboi\\\" >�� �� \"+ g[2]+\"</p>\"+" << "\n";
    out << "\"<p>"<<QObject::tr("Posledovatelno vizivaiytsia sledyiyschie fynkcionalnie obiekti:").toStdString().c_str()<<"</p>\"" << "\n";
    out << "       draw_func_list( temp_temp_temp);" << "\n";
    out << "        //alert( g);"  << "\n";
    out << "// � ���� ��� ���������� � ������� ��������� �����."  << "\n";
    out << "        func_info= g[ 3];"  << "\n";
    out << "        draw_func_info();"  << "\n";
    out << "}"  << "\n";
    out << "// ������� ������ �� ������� ���� �� �������"  << "\n";
    out << "// n - ������ ��� ���� �������."  << "\n";
    out << "function make_func_path( n)"  << "\n";
    out << "{"  << "\n";
    out << "var string= \"<a class=\\\"goluboi\\\" onclick=\\\"click_path(0,-1) \\\" >&nbsp;::</a>\";" << "\n";
    out <<  "if(n.length!=0 ) string+=\"&nbsp;&rarr;&nbsp;\";" << "\n";
    out << "	for( var i= 0; i< n.length; i++)" << "\n";
    out << "{" << "\n";
    out << "			if( i!=n.length-1)" << "\n";
    out << "			{" << "\n";
    out << "				string+= \"<nobr><a  onclick=\\\"click_path(\"+ n[ i][ 1]+ \",\"+ i+ \");\\\">\"+ n[ i][ 2]+ \"</a>\";" << "\n";
    out << "       }" << "\n";
    out << "			else" << "\n";
    out << "			{" << "\n";
    out << "				string+= \"<nobr><a class=\\\"goluboi\\\" onclick=\\\"click_path(\"+ n[ i][ 1]+ \",\"+ i+ \");\\\">\"+ n[ i][ 2]+ \"</a>\";" << "\n";
    out << "			}" << "\n";
    out << "			if( (i+ 1)< n.length) string+= \"&nbsp;&rarr;&nbsp;\";" << "\n";
    out << "}" << "\n";

    out << "        return string;"  << "\n";
    out << "}"  << "\n";
    out << "// ��� ����� i -�� ������� � ������"  << "\n";
    out << "// ����� ������� ��� �������� ����� i-��� �������� � ������ ����"  << "\n";
    out << "// ������������ ������ �������"  << "\n";
    out << "// ������������ ���� �������"  << "\n";
    out << "function click_path( id, k)"  << "\n";
    out << "{"  << "\n";
    out << "// ��������� ������ ����"  << "\n";
    out << "        var arr= new Array();"  << "\n";
    out << "        for( var i= 0; i<= k; i++)"  << "\n";
    out << "        {"  << "\n";
    out << "                arr.push( path_func_array[ i]);"  << "\n";
    out << "        }"  << "\n";
    out << "        path_func_array= arr;"  << "\n";
    out << "// �������������� ����"  << "\n";
    out << "        draw_path();"  << "\n";
    out << "// ������� ������� ��� ������"  << "\n";
    out << "        var id= 0;"  << "\n";
    out << "        if( arr.length!= 0) id= arr[ arr.length- 1][ 1];"  << "\n";
    out << "        func_array= get_with_perent_id_x( mx, id);"  << "\n";
    out << "// �������������� ������ �������"  << "\n";
    out << "// � ���� ��� ���������� � ������� ��������� �����."  << "\n";
    out << "        if( k != -1)"  << "\n";
    out << "        {"  << "\n";
    out << "               var temp_temp_temp=\"<p class=\\\"goluboi\\\">�� �� \"+path_func_array[ path_func_array.length-1][2]+\"</p>\"+ " << "\n";
    out << "              \"<p>"<<QObject::tr("Posledovatelno vizivaiytsia sledyiyschie fynkcionalnie obiekti:").toStdString().c_str()<<"</p>\" " << "\n";
    out << "                draw_func_list( temp_temp_temp);" << "\n";

    out << "                func_info= path_func_array[ k][ 3];"  << "\n";
    out << "                draw_func_info();"  << "\n";
    out << "        }"  << "\n";
    out << "        else"  << "\n";
    out << "        {"  << "\n";
    out << "             start();" << "\n";
    out << "        }"  << "\n";
    out << "}"  << "\n";
    out << "// ������� ������������� �������� �������."  << "\n";
    out << "function draw_func_info()"  << "\n";
    out << "{"  << "\n";
    out << "        var g= document.getElementById( \"info\");"  << "\n";
    out << "        g.innerHTML= func_info;"  << "\n";
    out << "}"  << "\n";

    out << "function start()" << "\n";
    out << "{" << "\n";
    out << "	func_array=get_allfunc();" << "\n";
    out << "	var str= make_functions_array( func_array);" << "\n";
    out << "	var a= document.getElementById(\"functions\");" << "\n";
    out << "	a.innerHTML= str;" << "\n";
    out << "}" << "\n";

    out << "</script>"  << "\n";
    out << "</head>"  << "\n";
    out << "<body>" << "\n";
    out << "<h1 align=\"center\">����� �� �������</h1>"  << "\n";

    out << "<h2 align=\"center\" id=\"proj_name\"></h2>" << "\n";
    out << "<script type=\"text/javascript\">" << "\n";
    out << "var proj_name=\"" << project_name.c_str() << "\";" << "\n";
    out << "var el=document.getElementById(\"proj_name\"); " << "\n";
    out << "el.innerHTML=\""<<QObject::tr("Proekt:").toStdString().c_str()<<" \" + proj_name; " << "\n";
    out << "</script>" << "\n";
    out << "<h2 class=\"otctup\">������� �������:</h2>" << "\n";
    out << "<table id=\"main_table\" class=\"otctup\" border=1>" << "\n";
    out << "<tr>"  << "\n";
    out << "        <td colspan=2>"  << "\n";
    out << "                <div id=\"functions_path\">&nbsp;</div>"  << "\n";
    out << "        </td>"  << "\n";
    out << "</tr>" << "\n";
    out << "</table>" << "\n";
    out << "<br/>" << "\n";
    out << "<table id=\"main_table2\" class=\"otctup\" >" << "\n";
    out << "<tr>"  << "\n";
    out << "        <td id=\"functions_td\">"  << "\n";
    out << "<!-- ��� ����������� ������� ������� ������ -->"  << "\n";
    out << "                <div id=\"functions\">"  << "\n";
    out << "                </div>"  << "\n";
    out << "        </td>"  << "\n";
    out << "        <td id=\"midl_td\">&nbsp;</td>" << "\n";
    out << "        <td id=\"info_td\">" << "\n";
    out << "<!-- ��� ����������� ���������� � ������� -->"  << "\n";
    out << "                <div id=\"info\"></div>"  << "\n";
    out << "        </td>"  << "\n";
    out << "</tr>"  << "\n";
    out << "</table>"  << "\n";
    out << "<script>"  << "\n";
    out << "      start();" << "\n";
    out << "</script>"  << "\n";
    out << "</body>"   << "\n";
    out << "</html>"  << "\n";



    out.close();
    return 0;
}






HRET Trace_Report_ersatz( sReportInfo* pa)
{
    ofstream file;
    string repname=pa->strOutputDirectory+"\\"+pa->strBaseFileName+"txt";
    file.open( repname.c_str() );
    if( !file.is_open( ))
    {
        cerr << "Error. Can not open file " << repname.c_str() << endl;
        return 0;
    }

    int size_line = 4096;
    char *line=new char [ size_line];
    file << "id"<<"       "<<QObject::tr("Imia fynkcii").toStdString().c_str()<<"                               "<<QObject::tr("Imia faila").toStdString().c_str()<<"\n";

    // �������� ���� �������
    QORMList< sScopeFull> allscope ( EXE_SCOPE);
    QORMList< sScopeFull>::iterator it=allscope.begin();
    for( ; it!=allscope.end(); ++it)
    {
        sprintf( line, "%-7d  %-40s  %-60s\0", (*it).id,  (*it).getQualifiedName().c_str(), (*it).file_path.c_str());
        file << line << endl;
        memset( line, '\0', size_line);

    }

    QORMList< sLink> data ( CALL_LINK );
    for( QORMList< sLink>::iterator j = data.begin(); j != data.end(); ++j)
    {
        file << (*j).id << " -> " << (*j).idB << endl;
    }


    delete line;
// �������� ���� ������






    file.close();
    return 1;
}


// ������� ������ �������� ������ �� ���������� ������� ����� �� ���������
HRET Trace_Report_cluster( sReportInfo* pa)
{
    QORMList < sCluster> tmp;
    int i=0;
    for( QORMList< sCluster>::iterator j = tmp.begin(); j != tmp.end(); j++, i++)
    {
        db_set_current_cluster( ( *j).id);
        char ch [ 10];
        itoa( (*j).id,ch,10);
        string sss= pa->strOutputDirectory+"\\"+ string( ch); 	//��������� ���������� ��� ������.
        mkdir( sss.c_str() );


        if(TraceReportf_f(pa->strProjectName,ak_vs_db::get_program_name(),pa->strBaseFileName+"1.",sss,20000,*(pa->lsProcessedFiles) )==-1)return -1;
        if(pa->hasBrunches)
        {
            if( TraceReportv_f( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"2.", sss, 20000, *( pa->lsProcessedFiles) )== -1) return -1;
            if( TraceReportv_v( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"3.", sss, 20000, *( pa->lsProcessedFiles) )== -1) return -1;
        }
    }
    db_set_current_cluster( NULL);
    return 1;
}

// ������� ������ �������� ������ �� ���������� ������� �����
HRET Trace_Report( sReportInfo* pa)
{
    QTime del_time;

    del_time.start();
    cout << QObject::tr("(log):Sozdanie otcheta trassi vizovov fynkcia - fynkcia(js)." ).toStdString().c_str()<< endl;
    //cout << "Making trace function-function... (js)" << endl;

    if( TraceReportf_f_new_javascript( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"_js_1.", pa->strOutputDirectory, 20000, *( pa->lsProcessedFiles) )== -1) return -1;

    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << del_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;

    del_time.restart();

    cout << QObject::tr("(log):Sozdanie otcheta trassi vizovov fynkcia - fynkcia.").toStdString().c_str() << endl;
    //cout << "Making trace function-function..." << endl;

    if( TraceReportf_f( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"1.", pa->strOutputDirectory, 20000, *( pa->lsProcessedFiles) )== -1)
        return -1;

    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << del_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;

    del_time.restart();

    cout << QObject::tr("Sozdat' dopolnitelnii otchet trass vizovov.").toStdString().c_str() << endl;
    //cout << "Making ersatz trace..." << endl;
    Trace_Report_ersatz( pa);
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << del_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;

    if( /*pa->hasBrunches ==*/ true )
    {
        del_time.restart();

        cout << QObject::tr("Sozdat' trassy vetvi - fynkcia...").toStdString().c_str() << endl;
        //cout << "Making trace vetve-function..." << endl;
        if( TraceReportv_f( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName+"2.", pa->strOutputDirectory, 20000, *(pa->lsProcessedFiles) ) ==-1 )return -1;
        cout << QObject::tr("Vipolneno za ").toStdString().c_str() << del_time.elapsed()/1000 << QObject::tr(" sek." ).toStdString().c_str()<< endl;

        del_time.restart();
        cout << QObject::tr("Sozdat' otchet trass vetvi - vetvie...").toStdString().c_str() << endl;
        //cout << "Making trace vetve-vetve..." << endl;
        if( TraceReportv_v( pa->strProjectName, ak_vs_db::get_program_name(),pa->strBaseFileName+"3.",pa->strOutputDirectory,20000,*(pa->lsProcessedFiles) )==-1)return -1;
        cout << QObject::tr("Vipolneno za ").toStdString().c_str() << del_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;
    }
    return 1;
}



int TraceReportf_f(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles
)
{
    QORMList< sScopeFull> allscope(EXE_SCOPE);
    QORMList< sScopeFull>::iterator it=allscope.begin();
    //>>>>>>>>>>>>>>>>>>>>>����� HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //����������� ����� ������
    int numstrok=allscope.size(); //����� ��������� � �������
    int num_html_file; //����� html ������ ����������� ��� ������ �����
    num_html_file= numstrok/mod_for_html_file+1; // ������� html ������ ����� ���������

    //�������� numfile ����� ���� �����

    vector< string> name_for_html_file;
    vector< string> fnames;

    //������� ��� html ������ � �������.
    for(int i=0; i < num_html_file; i++)
    {
        QString c= QString::number( i+1);
        name_for_html_file.push_back( path + "\\" + base_file_name + c.toStdString() + ".html");
        fnames.push_back( base_file_name + c.toStdString() + ".html");
    }
    //������� ������ ���������� �� html �����
    fstream *p_html_file=new fstream [num_html_file];
    //��������� html �����
    for( int i=0; i< num_html_file; i++)
    {
        p_html_file[ i].open( name_for_html_file[ i].c_str(), ios::out);
        lsProcessedFiles.push_back(name_for_html_file[ i]);
        if( p_html_file[ i].is_open() == false )
        {
            cerr << QObject::tr("fail").toStdString().c_str() << name_for_html_file[ i] << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
            return -1;
        }
    }
    //���������� ��������� � ������ html ����
    for( int i=0; i<num_html_file; i++)
    {
        //�������� ��������� � ���� HTML ���������
        //������ ��������� � html ����
        p_html_file[ i] << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n";
        //����� ��������� ����� � html ����
        p_html_file[ i] << getCSS().c_str();
        p_html_file[ i] << "</HEAD>\n<BODY>";
    }
    //��������� ��������� HTML ������
    for( int i=0; i<num_html_file; i++)
    {
        zagolovok_html_file( &p_html_file[ i], QObject::tr("OTCHET PO TRASSAM VIZOVOV (FYNKCIA-FYNKCIA)").toStdString().c_str(), project_name, version, fnames[ i], i+1, num_html_file);
    }
    silki_na_sleduiuchiy( p_html_file, fnames, num_html_file);

    //��������� � html ���� ����� ������ ����� � �������
    for( int i=0; i<num_html_file; i++)
    {
        //�� ����� �� �������� ����� ���� ��������
        p_html_file[i]<< "<br><table class=\"content\" border=1 align=\"center\"> \n";
        p_html_file[i]<< html_tr(
                          html_th( "�")+
                          html_th( QObject::tr("Imia fynkcii").toStdString().c_str())+
                          html_th( QObject::tr("Fail").toStdString().c_str())+
                          html_th( QObject::tr("Vizivaemie fynkcii").toStdString().c_str())+
                          html_th( QObject::tr("Fail").toStdString().c_str())
                      );
        p_html_file[i]<< endl;
    }

    //����� ���������� ������� � html �����
    for( int i=0; i < num_html_file; i++)
    {
        QString num_str;
        int k=0;
        for( int j=0; j < mod_for_html_file; j++)
        {
            k= j+i*mod_for_html_file;
            if( k >= numstrok)
            {
                break;
            }
            num_str = QString::number(k+1);

            string ss2;

            ak_vs_db::get_scope_metadata ( it[ k].id, "args", &ss2);
            p_html_file[ i] << "<tr>" << "<th>" << num_str.toStdString() << "</th>" << "<td>" << ak_vs_db::get_scope_full_qualified_name2( it[ k].id)+ss2<< "</td><td>"<< it[ k].file_path<< "</td><td>\n";
            //�� ���������� ��������

            sqlAdapterForLinks link_func;
            link_func.getConnectedFunc( it[ k].id );

            for(; link_func.next() ;)
            {
                string ss1;
                ak_vs_db::get_scope_metadata( link_func.getId( ), "args", &ss1);
                p_html_file[ i] << ak_vs_db::get_scope_full_qualified_name2( link_func.getId( ))+ss1<<"<br>\n";
            }

            p_html_file[ i] << "</td><td>";

            sqlAdapterForLinks link_func2;
            link_func2.getConnectedFunc( it[ k].id );

            for(; link_func2.next() ;)
            {

                p_html_file[ i] << getFilePathByScopeId( link_func2.getId()) << "<br>";
            }

            p_html_file[ i] << "</td></tr>\n";
        }
    }
    //����������� ������� ��� html ������
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i] << "</TABLE><br>\n";
    }
    //����� ����� �������� � html � txt ����� ��� ��� ������ ����� �������
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
//��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
    silki_na_sleduiuchiy( p_html_file, fnames, num_html_file);

    //����� ��������� ����� htlm ����������
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i]<< "</BODY></HTML>";
    }
    //��������� ��� html �����
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i].close();
    }
    delete [] p_html_file;
//>>>>>>>>>>>>>>>>>>>>> ����� ������ � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}
// ������� ���������� ������� ����� ����� - �������.
int TraceReportv_f(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string>  &lsProcessedFiles)
{
    QORMList< sScopeFull> allscope ( CODE_BLOCK);
    QORMList< sScopeFull>::iterator it= allscope.begin();
    //>>>>>>>>>>>>>>>>>>>>>����� HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //����������� ����� ������
    int numstrok= allscope.size(); //����� ��������� � �������
    int num_html_file; //����� html ������ ����������� ��� ������ �����
    num_html_file= numstrok/mod_for_html_file+1; // ������� html ������ ����� ���������
    //�������� numfile ����� ���� �����
    vector< string> name_for_html_file;
    vector< string> fnames;
    //������� ��� html ������ � �������.
    for( int i=0; i < num_html_file; i++)
    {
        char c[ 10];
        itoa( i+1, c, 10);
        name_for_html_file.push_back( path+"\\"+base_file_name+c+".html");
        fnames.push_back( base_file_name+c+".html");
    }
    //������� ������ ���������� �� html �����
    fstream *p_html_file=new fstream [num_html_file];
    //��������� html �����
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i].open( name_for_html_file[ i].c_str(), fstream::out);
        lsProcessedFiles.push_back( name_for_html_file[ i]);
        if( p_html_file[ i].is_open()== false)
        {
            cerr<< QObject::tr("fail").toStdString().c_str() << name_for_html_file[ i] << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
            return -1;
        }
    }
    //���������� ��������� � ������ html ����
    for( int i=0; i<num_html_file; i++)
    {
        p_html_file[ i] << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n";
        p_html_file[ i] << getCSS().c_str();
        p_html_file[ i] << "</HEAD>\n<BODY>";
    }
    //��������� ��������� HTML ������
    for( int i=0; i < num_html_file; i++)
    {
        zagolovok_html_file( &p_html_file[ i], QObject::tr("OTCHET PO TRASSAM VIZOVOV (VETVI-FYNKCIA)").toStdString().c_str(), project_name, version, fnames[ i], i+1, num_html_file);
    }
    silki_na_sleduiuchiy( p_html_file, fnames, num_html_file);
    //��������� � html ���� ����� ������ ����� � �������
    for(int i=0; i < num_html_file; i++)
    {
        //�� ����� �� �������� ����� ���� ��������
        p_html_file[ i] << "<br><table class=\"content\" border=1 align=\"center\"> \n";
        p_html_file[ i] << html_tr(
                            html_th( "�")+
                            html_th( QObject::tr("Imia vetvi").toStdString().c_str())+
                            html_th( QObject::tr("Stroka").toStdString().c_str())+
                            html_th( QObject::tr("Vizivaemie fynkcii").toStdString().c_str())
                        );
        p_html_file[ i] << endl;
    }
    //����� ���������� ������� � html �����
    for( int i=0; i< num_html_file; i++)
    {
        int k= 0;
        for( int j=0; j < mod_for_html_file; j++)
        {
            k= j+i*mod_for_html_file;
            if( k >= numstrok)
            {
                break;
            }




            p_html_file[ i] << "<tr>" << "<th>"<< k+1<< "</th>"<< "<td>"<< ak_vs_db::get_scope_full_qualified_name2 ( it[ k].id)<< "</td><td>"<< it[ k].file_path<< "</td><td align=\"center\">" << it[ k].pos_x1 << "</td><td>\n";
            sqlAdapterForLinks link_func;
            link_func.getConnectedFunc( it[ k].id );
            for(; link_func.next() ;)
            {
                string ss1;
                ak_vs_db::get_scope_metadata( link_func.getId( ), "args", &ss1);
                p_html_file[ i] << ak_vs_db::get_scope_full_qualified_name2( link_func.getId( ))+ss1<<"<br>\n";
            }




            p_html_file[ i]<< "</td> </tr>\n";
        }
    }
    //����������� ������� ��� html ������
    for( int i=0; i<num_html_file; i++)
    {
        p_html_file[ i] << "</TABLE><br>\n";
    }
    //����� ����� �������� � html � txt ����� ��� ��� ������ ����� �������
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
//��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
    silki_na_sleduiuchiy( p_html_file, fnames, num_html_file);

    //����� ��������� ����� htlm ����������
    for(int i=0; i < num_html_file; i++)
    {
        p_html_file[ i] << "</BODY></HTML>";
    }
    //��������� ��� html �����
    for(int i=0; i < num_html_file; i++)
    {
        p_html_file[ i].close();
    }

    delete [] p_html_file;
//>>>>>>>>>>>>>>>>>>>>> ����� ������ � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}

// ������� ���������� ������ ����� ����� - �����.
int TraceReportv_v(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list <string> & lsProcessedFiles)
{
    QORMList< sScopeFull> allscope ( CODE_BLOCK);
    QORMList< sScopeFull>::iterator it= allscope.begin();
    //>>>>>>>>>>>>>>>>>>>>>����� HTML <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
    //����������� ����� ������
    int numstrok= allscope.size(); //����� ��������� � �������
    int num_html_file; //����� html ������ ����������� ��� ������ �����
    num_html_file= numstrok/mod_for_html_file+1; // ������� html ������ ����� ���������
    //�������� numfile ����� ���� �����
    vector< string> name_for_html_file;
    vector< string> fnames;

    //������� ��� html ������ � �������.
    for( int i=0; i < num_html_file; i++)
    {
        QString c=QString::number(i+1);
        name_for_html_file.push_back( path + "\\" + base_file_name + c.toStdString() + ".html");
        fnames.push_back( base_file_name + c.toStdString() + ".html");
    }
    //������� ������ ���������� �� html �����
    fstream *p_html_file= new fstream [ num_html_file];
    //��������� html �����
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i].open( name_for_html_file[ i].c_str(), fstream::out);
        lsProcessedFiles.push_back( name_for_html_file[ i]);
        if( p_html_file[ i].is_open() == false)
        {
            cerr << QObject::tr("fail").toStdString().c_str() << name_for_html_file[ i] << QObject::tr("ne otkrilsia").toStdString().c_str()<< endl;
            return -1;
        }
    }
    //���������� ��������� � ������ html ����
    for( int i=0; i<num_html_file; i++)
    {
        p_html_file[ i] << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>\n";
        p_html_file[ i] << getCSS();
        p_html_file[ i] << "</HEAD>\n<BODY>";
    }
    //��������� ��������� HTML ������
    for( int i=0; i<num_html_file; i++)
    {
        zagolovok_html_file( &p_html_file[ i], QObject::tr("OTCHET PO TRASSAM VIZOVOV (VETV-VETV)").toStdString().c_str(), project_name, version, fnames[ i], i+1, num_html_file);
    }
    silki_na_sleduiuchiy( p_html_file, fnames, num_html_file);

    //��������� � html ���� ����� ������ ����� � �������
    for( int i=0; i<num_html_file; i++)
    {
        //�� ����� �� �������� ����� ���� ��������
        p_html_file[ i]<< "<br><table class=\"content\" border=1 align=\"center\"> \n";
        p_html_file[ i]<< html_tr(
                           html_th("�")+
                           html_th(QObject::tr("Imia vetvi").toStdString().c_str())+
                           html_th(QObject::tr("Imia faila").toStdString().c_str())+
                           html_th(QObject::tr("Stroka").toStdString().c_str())+
                           html_th(QObject::tr("Vietvi").toStdString().c_str())+
                           html_th(QObject::tr("Stroka").toStdString().c_str())
                       );
        p_html_file[ i]<< endl;
    }
    //����� ���������� ������� � html �����
    for( int i=0; i<num_html_file; i++)
    {
        char a[ 10];
        int k= 0;
        for( int j=0; j<mod_for_html_file; j++)
        {
            k= j+i*mod_for_html_file;
            if( k>= numstrok)
            {
                break;
            }
            itoa( k+1, a, 10);
            sScope c;
            c.id= it[ k].id;
            QORMList< sScopeFull> tmp2 (it[k]);
            QORMList< sScopeFull>::iterator jt = tmp2.begin();
            p_html_file[ i] << "<tr>" << "<th>" << k+1 << "</th>" << "<td>" << ak_vs_db::get_scope_full_qualified_name2 ( it[ k].id) << "</td><td>" << it[ k].file_path << "</td><td align=\"center\">" << it[ k].pos_x1 << "</td><td>\n";
            //�� ���������� ��������
            for( int jn=0; jn < tmp2.size(); jn++)
            {
                p_html_file[ i] << ak_vs_db::get_scope_full_qualified_name2( jt[ jn].id) << "<br>"<< "\n";
            }
            p_html_file[ i]<< "</td><td align=\"center\">";
            for( int jn= 0; jn< tmp2.size(); jn++)
            {
                p_html_file[ i] << jt[ jn].pos_x1 << "<br>";
            }
            p_html_file[ i]<< " </td></tr>\n";
        }
    }
    //����������� ������� ��� html ������
    for( int i=0; i<num_html_file; i++)
    {
        p_html_file[ i]<< "</TABLE><br>\n";
    }
    //����� ����� �������� � html � txt ����� ��� ��� ������ ����� �������
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
//��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
    silki_na_sleduiuchiy( p_html_file, fnames, num_html_file);

    //����� ��������� ����� htlm ����������
    for( int i=0; i<num_html_file; i++)
    {
        p_html_file[i]<<"</BODY></HTML>";
    }
    //��������� ��� html �����
    for( int i=0; i<num_html_file; i++)
    {
        p_html_file[i].close();
    }
    delete [] p_html_file;
//>>>>>>>>>>>>>>>>>>>>> ����� ������ � TXT ���� <<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    return 0;
}

/*

class translator2
{
public:
    translator2()
    {
        if(qApp!=NULL)
        {
            QTranslator qtTranslator;
            //qtTranslator.load("qt_" + QLocale( QLocale::Russian,QLocale::RussianFederation).name());
            qtTranslator.load("translation/libservice_ru");
            qApp->installTranslator(&qtTranslator);
        }
    }
};
static translator2 AB;
*/

#include <QMessageBox>
#include <QApplication>
QString liblang;
void initLibservice(std::string lang){
    liblang=QString(lang.c_str());
    if( qApp != NULL ){
        QTranslator *qtTranslator= new QTranslator;
        if( false == qtTranslator->load(QString("translation/libservice_")+lang.c_str()) )
            QMessageBox::critical(NULL, QObject::tr("Oschibka"), QObject::tr("Fail s perevodom dlia biblioteki libservice ne podgrusilsia") );
        qApp->installTranslator(qtTranslator);
    }
    else
    {
        int argc=0;
        char **argv;
        QApplication *a=new QApplication(argc,argv);
        QTranslator * qtTranslator= new QTranslator;
        if( false == qtTranslator->load(QString("translation/translation/libservice_")+lang.c_str() ) )
            QMessageBox::critical(NULL, QObject::tr("Oschibka"), QObject::tr("Fail s perevodom dlia biblioteki libservice ne podgrusilsia"));
        qApp->installTranslator(qtTranslator);
    }
}
