/*
 * signature.cpp
 *
 *  Created on: 17.12.2008
 *      Author: javamain
 */

#include "libservice.h"
#include <QTime>
#include <QFile>
#include <map>
using namespace std;






// функци€ самого верхнего уровн€ по построению сигнатурного анализа.
HRET SignatureReport(sReportInfo* pa)
{
    QTime delta_time;
    delta_time.start();
    cout << QObject::tr("Sozlanie otcheta po signaturnomy analizy.").toStdString().c_str() << endl;
            //"Make signature report..." << endl;

    QFile( ".\\res\\highlight.js" ).copy(QString(pa->strOutputDirectory.c_str())+"\\highlight.js");
    QFile( ".\\res\\highlight.pack.js" ).copy(QString(pa->strOutputDirectory.c_str())+"\\highlight.pack.js");
    QFile( ".\\res\\vs.css" ).copy(QString(pa->strOutputDirectory.c_str())+"\\vs.css");

    if( signaturereportM( pa->strProjectName, ak_vs_db::get_program_name(), pa->strBaseFileName, pa->strOutputDirectory, 10000, *(pa->lsProcessedFiles))==-1)return -1;
    cout << QObject::tr("Vipolneno za ").toStdString().c_str() << delta_time.elapsed()/1000 << QObject::tr(" sek.").toStdString().c_str() << endl;
    return 1;
}



struct myclass
{
    bool operator() ( pair< string, int>  i, pair< string, int>  j)
    {
        return ( i.second>j.second);
    }
} myobject;


class AAAC
{
public:
    sFile f;
    sSAResult r;
};

// вспомогательна€ функци€
string Translit(const string& src)
{
    string Rus = ":јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЁёяабвгдеЄжзийклмнопрстуфхцчшщьыъэю€ /\\";
    const string Lat[] = {" ","A", "B", "V", "G", "D", "E", "E", "Zh", "Z", "I", "I", "K",
                          "L", "M", "N", "O", "P", "R", "S", "T", "U", "F", "H", "C", "Ch", "Sh", "Sh'", "E",
                          "Yu", "Ya", "a", "b", "v", "g", "d", "e", "e", "zh", "z", "i", "i", "k", "l", "m", "n",
                          "o", "p", "r", "s", "t", "u", "f", "h", "c", "ch", "sh", "sch", "", "i", "", "e", "yu",
                          "ya", "_", "_", "_"
                         };
    string Res;
    for ( string::const_iterator i = src.begin() ; i != src.end() ; ++i)
    {
        string::size_type Pos = Rus.find(*i);
        if (Pos != std::string::npos)
            Res += Lat[Pos];
        else
        {
            if (*i < 0)
                Res += '_';
            else
                Res += *i;
        }
    }
    return Res;
}

// фукнци€ низкого уровн€ по построению
int signaturereportM(	string project_name,
                        string version,
                        string base_file_name,
                        string path,
                        int mod_for_html_file,
                        list< string> &lsProcessedFiles
                    )
{
    string html_str=                       "<html>\n"
                                           "<meta http-equiv=\"content-type\" content=\"text/html;charset=windows-1251\">"
                                           "<FRAMESET FRAMEBORDER=\"1\" FRAMESPACING=\"0\" BORDER=\"1\" rows=\"27%,61%,12%\">\n"
                                           "		<FRAME SRC=\"up.html\" NAME=\"menu1\" >\n"
                                           "		<FRAMESET FRAMEBORDER=\"1\" FRAMESPACING=\"0\" BORDER=\"1\" COLS=\"20%,80%\">\n"
                                           "	        <FRAME SRC=\"left.html\" NAME=\"menu2\" >\n"
                                           "			<FRAME SRC=\"right.html\" NAME=\"menu3\" >\n"
                                           "		</FRAMESET>\n"
                                           "        	<FRAME SRC=\"down.html\" NAME=\"menu4\" >\n"
                                           "    <NOFRAMES>"
                                           +string(QObject::tr("Vasch brayser ne podderjivaet fraimi").toStdString().c_str())+
                                           "</NOFRAMES>\n"
                                           "</FRAMESET>\n"
                                           "</html>\n";
    fstream signaturef;
    fstream upf;
    fstream downf;
    fstream rightf;
    fstream leftf;
    upf.open( ( path+"\\"+"up.html").c_str(), ios::out);
    downf.open( ( path+"\\"+"down.html").c_str(), ios::out);
    rightf.open( ( path+"\\"+"right.html").c_str(), ios::out);
    leftf.open( ( path+"\\"+"left.html").c_str(), ios::out);
    signaturef.open( ( path+"\\"+base_file_name+"html").c_str(), ios::out);
    lsProcessedFiles.push_back(path+"\\"+base_file_name+"html");

    if( upf.is_open()==false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << "up.html " << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }

    if( downf.is_open()==false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << "down.html " << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }
    if( rightf.is_open()==false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << "right.html " << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }
    if( leftf.is_open()==false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << "left.html " << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }
    if( signaturef.is_open()==false)
    {
        cerr << QObject::tr("fail ").toStdString().c_str() << path+"\\"+base_file_name+".html" << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
        return -1;
    }
    signaturef << html_str;
    //впишим в начальный фрейм чтонибудь хорошое.
    upf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>"<< getCSS().c_str()<<"</HEAD><body>\n";
    upf << "<meta http-equiv=\"content-type\" content=\"text/html;charset=windows-1251\">\n";
    upf << "<H3 align=\"center\">"<<QObject::tr("OTCHET PO SIGNATYRNOMY ANALIZY I POISKY").toStdString().c_str()<<"<BR> "<<QObject::tr("POTENCIALNO OPASNIH KONSTRYKCII").toStdString().c_str()<<"</H3>\n";
    upf << "<p ALIGN=\"center\">"<< QObject::tr("Proekt: ").toStdString().c_str() << project_name.c_str() << "</p>";
    upf << "<p ALIGN=\"center\">" << version.c_str() << "</p>";
    upf << "</body></html>\n";
    leftf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>"<< getCSS().c_str()<<"</head><body>\n";
    leftf << "<meta http-equiv=\"content-type\" content=\"text/html;charset=windows-1251\">\n";
    //делаем список файлов с угрозами.
    QORMList< sFile> sfile;
    QORMList< sFile>::iterator it=sfile.begin();
    leftf << "<p align=\"center\">"<<QObject::tr("Perechen failov s naidenimi konstrykciami:").toStdString().c_str()<<"</p>";
    leftf << "<a href=\"right.html\" target=\"menu3\">"<<QObject::tr("Svodnii otchet").toStdString().c_str()<<"</a>" << endl;
    leftf << "<ol type=\"1\">";
    int bab_file=0;
    int valnur=0;
    map< string, int> threatnumber;
    map< string, vector< AAAC> > threat2;

    for( ; it != sfile.end() ; ++it )
    {
        if( ak_vs_db::count_sa( (*it).id) == 0) continue;
        bab_file++;
        QORMList< sSAResult> tmp2( *it);
        QORMList< sSAResult>::iterator jt = tmp2.begin();
        //создает правй файл.
        fstream rgf;
        char as[ 5];
        itoa( ( *it).id,as,10);
        string namerightf( path+"\\"+as+".html");
        string namerightff( string("")+as+".html");
        rgf.open(namerightf.c_str(), fstream::out);

        if( rgf.is_open()==false)
        {
            cerr << QObject::tr("fail ").toStdString().c_str() << namerightf.c_str() << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
            return -1;
        }

        rgf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>"
            "<meta http-equiv=\"content-type\" content=\"text/html;charset=windows-1251\">\n"
            "<STYLE type=\"text/css\">\n"
            ".vuln {color:#ff0000;}\n"
            "</STYLE>\n" << getCSS().c_str() << ""
            "<link rel=\"stylesheet\" type=\"text/css\" href=\"vs.css\"></link>\n"
            "<script src=\"highlight.pack.js\"></script>\n"
            "<script type=\"text/javascript\">\n"
            "hljs.tabReplace = '    ';\n"
            "hljs.tabReplace = '<span class=\"indent\">\t</span>';\n"
            "hljs.initHighlightingOnLoad();\n"
            "</script>\n"
            "</HEAD><body>\n";
            rgf << "<h3>" << QObject::tr("Fail:").toStdString().c_str()<<" <a href=\"file:///"<<( *it).file_path<<"\">" << ( *it).file_path << "</a></h3>\n";
        //freat_list << "<p>‘айл: <a href=\"file:///"<<( *it).file_path<<"\">" << ( *it).file_path << "</a></p>\n";
        for( ; jt != tmp2.end(); ++jt)
        {
            rgf << "<hr>\n";
            AAAC ya;
            ya.f= *it;
            ya.r= *jt;
            threat2[ (*jt).threatName()].push_back( ya);
            rgf << "<p>"<<QObject::tr("Stroka: ").toStdString().c_str() << (*jt).strNumber() << "</p>\n";
            rgf << "<p>ID: " << (*jt).id << "</p>\n";
            rgf << "<p>"<<QObject::tr("Yiazvimost: ").toStdString().c_str() << "<a href=\"" << (*jt).getCWE_URL()<<"\">" << (*jt).threatName() << "</a></p>\n";
            rgf << "<p>"<<QObject::tr("Fragment potencialno opasnogo koda:").toStdString().c_str()<<"</p>" <<  (*jt).codeBlock() << "\n";
            valnur++;
        }
        rgf << "</body></html>\n";
        rgf.close();
        leftf << "<li>" << "<a href=\"" << namerightff.c_str() << " \" target=\"menu3\">" << ( *it).getFileName() << "</a></li>";
    }
    leftf << "</ol>";
    leftf << "</body></html>";

    downf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>" << getCSS().c_str() << "</head><body>\n";
    downf << "<meta http-equiv=\"content-type\" content=\"text/html;charset=windows-1251\">\n";
    downf << "<p align=\"center\">"<< QObject::tr("Vsego naideno %1 konstrykcii v %2 failah.").arg(valnur).arg(bab_file).toStdString().c_str() << "</p>";
    downf << "</body></html>";
    rightf << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>" << getCSS().c_str()<<"</head><body>\n";
    rightf << "<meta http-equiv=\"content-type\" content=\"text/html;charset=windows-1251\">\n";
    vector< pair< string, int>  > per;

    for( map< string, vector< AAAC> >::iterator it=threat2.begin(); it != threat2.end(); ++it)
    {
        pair< string, int > *ppt =new pair<string, int>( ( *it).first, ( *it).second.size());
        per.push_back( *ppt);
    }

    sort( per.begin(), per.end(), myobject);
    if( per.size() > 0)
    {
        rightf << "<table  align=\"center\" >" << endl;
        rightf << "<tr><td>"<< QObject::tr("Naimenovanie").toStdString().c_str()<<"</td><td>"<<QObject::tr("Kolichestvo").toStdString().c_str()<<"</td></tr>" << endl;
        for( size_t i=0; i < per.size(); ++i)
        {
            rightf << "<tr><td><a href=\"freat="+Translit( per[ i].first)+".html\">" << per[ i].first << "</a></td><td>" << per[ i].second << "</td></tr>" << endl;
        }
        rightf << "</table>" << endl;
        rightf << "</body></html>";
    }
    if( per.size() == 0)
    {
        rightf << "<h4 align=\"center\">"<<QObject::tr("Yiazvimostei ne naideno").toStdString().c_str()<<"</h4>" << endl;
    }
    // каждый файл по угрозам
    for( map< string, vector< AAAC> >::iterator it2=threat2.begin(); it2 != threat2.end(); ++it2)
    {
        fstream freat_list;
        freat_list.open( ( path+"\\"+"freat="+Translit( ( *it2).first)+".html").c_str(), ios::out);
        if( freat_list.is_open()==false)
        {
            cerr << QObject::tr("fail ").toStdString().c_str() << "freat.html " << QObject::tr("ne otkrilsia").toStdString().c_str() << endl;
        }
        freat_list << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><HEAD><TITLE></TITLE>" << getCSS().c_str() << "\n";
        freat_list << "<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n"
                   "<STYLE type=\"text/css\">\n"
                   ".vuln {color:#ff0000;}\n"
                   "</STYLE>\n"
                   "<link rel=\"stylesheet\" type=\"text/css\" href=\"vs.css\"></link>\n"
                   "<script src=\"highlight.pack.js\"></script>\n"
                   "<script type=\"text/javascript\">\n"
                   "hljs.tabReplace = '    ';\n"
                   "hljs.tabReplace = '<span class=\"indent\">\t</span>';\n"
                   "hljs.initHighlightingOnLoad();\n"
                   "</script>\n"
                   "</HEAD><body>\n";
        //getCWE_URL()
        freat_list << "<h3>"<<QObject::tr("Yiazvimost: ").toStdString().c_str() << ( *it2).first << "</h3>\n";
        freat_list << ( *it2).first;
        for( vector< AAAC>::iterator jt=( *it2).second.begin(); jt!=( *it2).second.end(); ++jt)
        {
            freat_list << "<h5>"<<QObject::tr("Yiazvimost:").toStdString().c_str()<<" <a href=\"" << (*jt).r.getCWE_URL() <<"\" >" << ( *jt).r.threatName() << "</a></h3>\n";
            freat_list << "<p>ID: " << (*jt).r.id << "</p>";
            freat_list << "<p>"<<QObject::tr("Fail: ").toStdString().c_str()<<"<a href=\"file:///"<<( *jt).f.file_path<<"\">" << ( *jt).f.file_path << "</a></p>\n";
            freat_list << "<p>"<<QObject::tr("Stroka: ").toStdString().c_str() << ( *jt).r.strNumber() << "</p>\n";
            freat_list << "<p>"<<QObject::tr("Fragment potencialnogo opasnogo koda:").toStdString().c_str()<<"</p>" << ( *jt).r.codeBlock() << "\n";
            freat_list << "<br><hr>";
        }
        freat_list << "<br><hr>";
        freat_list << "</body></html>";
        freat_list.close();
    }
    signaturef.close();
    upf.close();
    downf.close();
    leftf.close();
    rightf.close();
    return 0;
}
