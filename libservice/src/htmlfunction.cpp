#include "htmlfunction.h"
#include <QObject>
using namespace std;

void debug_message(int i, string str)
{
#ifdef DEBUG_ON
    cout<< str << endl;
    cout.flush();
#endif
}

string  alex::html_breaket ( string type, const char * type2, string  text)
{
    return string("<")
           .append( type)
           .append( " ")
           .append( type2)
           .append( " ")
           .append( ">")
           .append( text)
           .append( "</")
           .append( type)
           .append( ">")
           .append( "\n");
}

string  alex::html_breaket( const char* type, const char * type2, const char *  text)
{
    return string( "<")
           .append( type)
           .append( " ")
           .append( type2)
           .append( " ")
           .append( ">")
           .append( text)
           .append( "</")
           .append( type)
           .append( ">")
           .append( "\n");
}

string  alex::html_breaket( const char* type, const char * type2, string  text)
{
    return string( "<")
           .append( type)
           .append( " ")
           .append( type2)
           .append( " ")
           .append( ">")
           .append( text)
           .append( "</")
           .append( type)
           .append( ">")
           .append( "\n");
}

string  alex::html_breaket( const char* type)
{
    return string( "<")
           .append( type)
           .append( " ")
           .append( "/>")
           .append( "\n");
}



string alex::html_a( const char * href, string text)
{
    return 	html_breaket( string( "A"), string( " HREF=\"").append( href).append( "\"").c_str(), text);
}

string alex::html_a( const char * href, const char * text)
{
    return 	html_breaket( string( "A"), string( " HREF=\"").append( href).append( "\"").c_str(), text);
}
string alex::html_a( string href, const char * text)
{
    return 	html_breaket( string( "A"), string( " HREF=\"").append( href).append( "\"").c_str(), text);
}

string alex::html_th( string text)
{
    return 	html_breaket( "TH", "", text).append( "\n");
}
string alex::html_th( const char * text)
{
    return 	html_breaket( "TH", "", text).append( "\n");
}

string alex::html_tr(string text)
{
    return 	html_breaket( "TR", "", text).append( "\n");
}

string alex::html_tr( const char* text)
{
    return 	html_breaket( "TR", "", text).append( "\n");
}

string alex::inttostring( int i)
{
    char a[ 100];
    itoa( i, a, 10);
    return a;
}

string alex::html_td( string text)
{
    return 	html_breaket( "TD", "", text).append( "\n");
}

string alex::html_td( const char* text)
{
    return 	html_breaket( "TD", "", text).append( "\n");
}

string& getCSS()
{
    string *str=new string( "\n<style type=\"text/css\">"
                            "td.one {background:#cccccc;}"
                            "td { border-color:#000000;}"
                            "body {"
                            "	margin: 20px;"
                            "	background: #FFFFFF;"
                            "	font-family: \"Trebuchet MS\", Arial, Helvetica, sans-serif;"
                            "	font-size: 13px;"
                            "	color: #000000;"
                            "}"
                            "h1, h2, h3 {"
                            "	margin: 0;"
                            "	font-family: Georgia, \"Times New Roman\", Times, serif;"
                            "	font-weight: normal;"
                            "	color: #000000;"
                            "}"
                            "h1 {"
                            "	font-size: 2em;"
                            "}"
                            "h2 {"
                            "	font-size: 1.6em;"
                            "}"
                            "h3 {"
                            "	font-size: 1.6em;"
                            "}"
                            "p, ul, ol {"
                            "	margin-top: 0;"
                            "	line-height: 180%;"
                            "}"
                            "ul, ol {"
                            "}"
                            "a {"
                            "	text-decoration: none;"
                            "	border-bottom: 1px dotted #999999;"
                            "	color: #A42424;"
                            "}"
                            "table{"
                            "padding: 20px;"
                            "border-collapse:collapse;"
                            "	border-color:Black; "
                            "	border-width: 1px 1px 1px 1px;"
                            "}"
                            "th {border-color:Black;background-color: #CCCCCC;"
                            "</style>"
                            "<meta http-equiv=\"content-type\" content=\"text/html; charset=windows-1251\" />"
                            "\n") ;
    return *str;
}

string strelka( string &fname, const char* ch)
{
    return alex::html_breaket("P","align=\"center\"",
                              alex::html_a( fname.c_str(),
                                            alex::html_breaket( "IMG", ( "src=\""+ string( ch)+ "\" border=0 ALT=\""+ fname+ "\"").c_str(),"")
                                          )
                             );
}

using namespace alex;


string make_zagolovok(string report, string project, string version, string file_name)
{
    string str_buff;
    str_buff.append( html_breaket( "H3","ALIGN=\"center\"", report));
    str_buff.append( html_breaket( "P","ALIGN=\"center\"", QObject::tr("Proekt: ").toStdString()+project ));
    str_buff.append( html_breaket( "P", "ALIGN=\"center\"", version));
    str_buff.append( html_breaket( "P", "ALIGN=\"center\"", QObject::tr("Imia faila: %1").arg(file_name.c_str()
                                                                                              ).toStdString().c_str()
                                  )
                    );
    str_buff.append( "</BR>\n");

    return str_buff;
}


void zagolovok_html_file( fstream* p_html_file, const char * nazvanie, string &project_name, string& version, string& fnames, int i, int num_html_file)
{
    string str_buff= "";
    str_buff.append( html_breaket( "H3","ALIGN=\"center\"", nazvanie));
    str_buff.append( html_breaket( "P","ALIGN=\"center\"", QObject::tr("Proekt: ").toStdString()+project_name ));
    str_buff.append( html_breaket( "P", "ALIGN=\"center\"", version));
    str_buff.append( html_breaket( "P", "ALIGN=\"center\"", QObject::tr("Imia faila: ").toStdString()+fnames));
    str_buff.append( html_breaket( "P", "ALIGN=\"center\"", QObject::tr("Fail %1 is %2").arg(i).arg(num_html_file).toStdString() ));
    str_buff.append( "</BR>\n");
    (*p_html_file)<< str_buff;
}

void zagolovok_html_gfile( GroupReportFile& gfile, const char * nazvanie, string &project_name, string& version)
{
    gfile.WriteToAllFile( html_breaket( "H3","ALIGN=\"center\"", nazvanie).c_str() );



    gfile.WriteToAllFile( html_breaket( "P","ALIGN=\"center\"", QObject::tr("Proekt: ").toStdString()+project_name ).c_str() );


    gfile.WriteToAllFile( html_breaket( "P", "ALIGN=\"center\"", version).c_str() );



    for( int j=0; j < gfile.Size(); j++)
    {
        gfile.GetFile(j)<< html_breaket( "P", "ALIGN=\"center\"", QObject::tr("Imia faila: %1").arg(gfile.GetFileName(j).toStdString().c_str()
                                                                                                    ).toStdString().c_str()
                                         ) ;
        gfile.GetFile(j) <<  html_breaket( "P", "ALIGN=\"center\"", QObject::tr("Fail %1 is %2").arg(j+1).arg(gfile.Size()).toStdString().c_str()
                                           );
    }
    gfile.WriteToAllFile( "</BR>\n");
}
