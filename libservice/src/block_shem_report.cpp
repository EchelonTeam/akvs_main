/*
 * block_shem_report.cpp
 *
 *  Created on: 30.10.2008
 *      Author: javamain
 */

#include "block_shem_report.h"
#include <QDir>
#include <QMap>
#include <QList>
#include <QPainter>
#include <QSvgGenerator>
#include <QTextCodec>
#include "htmlfunction.h"
#include <QTextStream>
#include <QFileInfo>
#include <QString>

extern QString liblang;

// содержит информацию для построения сводного отчета об блок схемах
class FileBlockShem{
public :
    FileBlockShem(){}
    FileBlockShem( QString file_svg, int func_id, QString func_name, QString file_for_fucn){
            this->file_svg_=file_svg;
            this->func_id_=func_id;
            this->func_name_=func_name;
            this->file_for_func_=file_for_fucn;
    }
private:
    QString file_svg_;
    int     func_id_;
    QString func_name_;
    QString file_for_func_;
public:
    QString getfile_svg(){return file_svg_;}
    int     getfunc_id(){return func_id_;}
    QString getfunc_name(){return func_name_;}
    QString getfile_for_func(){return file_for_func_;}
};



QList<FileBlockShem> lst_fileblockshem;

QString makeTable( QList< FileBlockShem > * lst_FileBlock );

void test(QList< FileBlockShem > * lst_FileBlock);

/** получить мап файлов и функций в них */
QMap<QString, QList<QString> > * makeFuncList();

void TestFunc()
{
    QORMList<sScopeFull> allscope (EXE_SCOPE);
    QORMList<sScopeFull>::iterator it=allscope.begin();
    for( int i = 0 ; it != allscope.end() ; ++it, i++)
    {
        cout << "num=" << i << endl;
        Tree tree;
        sScope a;
        a = (*it);
    }
}

/*
  Прочитать файл в переменную
  */
QString &ReadTemplateFile(QString templatefile)
{
       QString *pstr=new QString();
       QFile file(templatefile);
       if (!file.open(QIODevice::ReadOnly ))
         return (*(new QString("")));
       (*pstr)+=file.readAll();
       return *pstr;
}
/*
  Вернуть строку из файла по координатам
*/
QString &ReadFileStringByPos( QString filename, int x1, int y1, int x2, int y2)
{
    QString *pstr=new QString();
    //(*pstr) +=QString("x1=%1, y1=%2, x2=%3, y2=%4").arg(QString::number(x1)).arg(QString::number(y1)).arg(QString::number(x2)).arg(QString::number(y2));
    //cout << "Read file to string by pos=" << filename.toStdString() << endl;
    QFile file( filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text ))
        return (*(new QString("")));
    QTextStream in(&file);
    int currentx=0;
    QString line="";
    for (;!in.atEnd();currentx++) {
        line="";
        if( currentx < x1)
        {
            in.readLine();
            continue;
        }
        if( currentx == x1 )
        {
            line = QString(in.readLine()).mid( y1 );
            (*pstr)+="\n"+line;
        }
        if( currentx == x2 )
        {
            line = QString(in.readLine()).mid(0, y2+1 );
            (*pstr)+="\n"+line;
        }
        if( currentx > x1 and currentx < x2)
        {
            line = in.readLine();
            (*pstr)+="\n"+line;
        }
        if( currentx > x2) break;
    }
    return *pstr;
}



/*
    Сделать html  файл для блок схем
*/
HRET Make_Html_For_Bloc_Schem( QString svgfilename, QString codefilename, QString indexfile)
{
    // словарь для замены
    QMap<QString,QString> dict;
    dict["{{CODE_FILE}}"]=codefilename;
    dict["{{SVG_FILE_HTML}}"]=svgfilename;


    QString filetemplate;
    // шаблон файлов
    if( "ru" == liblang )
        filetemplate = ReadTemplateFile("./template/template_for_block_scheme_index.tmp");
    else if( "en" == liblang )
        filetemplate = ReadTemplateFile("./template/template_for_block_scheme_index_en.tmp");
    else
        filetemplate = ReadTemplateFile("./template/template_for_block_scheme_index.tmp");

    // открываем файл для записи
    QFile file( indexfile );
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "cannot open file " << indexfile.toStdString() << endl;
        return S_FAILED;
    }
    QTextStream out( &file );
    // замена слов
    for(QMap<QString, QString>::iterator it=dict.begin(); it != dict.end(); ++it)
    {
        filetemplate.replace( it.key() , it.value() );
    }
    out << filetemplate << endl;
    file.close();
}

/*
  Функция копирует файл с кодом для отчета блок схем
  */
void CopyCodeToFile(QString file_with_code, int x1, int y1, int x2, int y2, QString output_file, QString func_name)
{
    QString &codefilename= ReadFileStringByPos( file_with_code, x1, y1, x2, y2);
    // словарь для замены
    codefilename.replace( "<", "&lt;");
    codefilename.replace( ">", "&gt;");
    QMap<QString,QString> dict;
    if( ak_vs_db::get_scope_metadata(0, "code_in_block_schem") != 1)
    {
       dict["{{CODE_IN_FILE}}"]="";
    } else
    {
       dict["{{CODE_IN_FILE}}"]=codefilename;
    }
    dict["{{FUNCTION_FILE_NAME}}"]=QString("<a href=\"%1\">%2</a>")
            .arg( "file:///"+file_with_code )
            .arg( file_with_code );
    dict["{{FUNCTION_NAME}}"]=func_name;
    dict["{{FUNCTION_START_LINE}}"]=QString::number(x1 );
    dict["{{FUNCTION_END_LINE}}"]=QString::number(x2 );

    QString filetemplate;
    // считаем файл с шаблоном
    if( "ru" == liblang )
         filetemplate= ReadTemplateFile( "./template/template_for_block_scheme_code.tmp" );
    else if( "en" == liblang )
        filetemplate = ReadTemplateFile( "./template/template_for_block_scheme_code_en.tmp" );
    else
        filetemplate = ReadTemplateFile( "./template/template_for_block_scheme_code.tmp" );
    // открываем файл для записи
    QFile file( output_file );
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       return;
    QTextStream out(&file);
    // замена слов
    for(QMap<QString, QString>::iterator it=dict.begin(); it != dict.end(); ++it)
    {
        filetemplate.replace( it.key() , it.value() );
    }
    out << filetemplate << endl;
    file.close();
}


void copyFile(QString file, QString dirto)
{
    //cout << "copy from=" << file.toStdString() << endl;
    //cout << "copy to  =" << dirto.toStdString() << endl;
    QFile f(file);
    f.copy(dirto);
}
#include <QFile>
#include <QTextStream>

void copyFileReplace(QString file, QString dirto,QString word, QString repstr)
{
    //cout << "copy from=" << file.toStdString() << endl;
    //cout << "copy to  =" << dirto.toStdString() << endl;
    copyFile( file, dirto);

    //QString &codefilename= ReadFileStringByPos( file_with_code, x1, y1, x2, y2);
    QFile file1( dirto );
    if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file1);
    QString text=in.readAll();
    file1.close();

    if (!file1.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QTextStream out(&file1);
    out << text.replace(word,repstr);
    file1.close();
}

//главный файл по построению блок схем
HRET Trace_Block_Shem( sReportInfo* pa)
{

#ifdef DEBAG_ALEX2
    timeb time1, time2;
#endif
#ifdef DEBAG_ALEX2
    ftime( &time1);
#endif

    //cout << "Trace_Block_Shem:start..." << endl;
    cout.flush();
    sScope a;
    a = pa->iscStartBlock ; // константа начальная ветка дерева пока можно не париться и указывать 0

    QString file_name       = QString( pa->strOutputDirectory.c_str())+ "\\"+ QString(pa->strBaseFileName.c_str())+ ".svg";
    QString code_file_name  = QString( pa->strOutputDirectory.c_str())+ "\\"+ QString(pa->strBaseFileName.c_str())+ ".code.html";
    QString index_file_name = QString( pa->strOutputDirectory.c_str())+ "\\"+ QString(pa->strBaseFileName.c_str())+ ".html";

    // читаем файл
    list< SLink> vl;					//вектор для стрелок.
    Tree tree;
    tree.data.id = a.id;
    //помещаем в дерево все из базы
#ifdef DEBAG_ALEX2
    timeb sec1, sec2;
    sec1.time = 0;
    sec2.time = 0;
    sec1.millitm = 0;
    sec2.millitm = 0;
    ftime( &sec1 );
#endif
    // перегружаем в дерево из базы все скоупы
    //cout << "From BD to tree..." << endl;
    fromBaseToTree( tree, a);
    //взвесим дерево.
#ifdef DEBAG_ALEX2
    ftime( &sec2);
    cout << "   frombase_to_mamory_Trace_Block_Shem=" << sec2.time*1000+sec2.millitm-sec1.millitm -sec1.time*1000 << endl;
#endif
    if( tree.a.size() == 0)
    {
        cout << QObject::tr("Error: Net vosmojnosti postroit block-shemy dlia fynkcii, tak kak AST derevo fynkcii pysto.").toStdString() << endl;
        return -1;
    }
    debug_message( 5, "Add func start and end scope..." );

    pa->lsProcessedFiles->push_back( index_file_name.toStdString() ); // забиваем имя файла.



    //file_name
    if( tree.a.size() == 1 and tree.a[ 0 ] != NULL)
    {
        //вставка в функцию конец
        Tree* tr = new Tree;
        sScope k;
        k.id = -1U;
        tr->data = k;
        tr->data.system_type = CODE_END;
        tr->parent = &tree;
        tree.a[ 0 ]->a.push_back( tr );
    }

    if( tree.a.size() == 1 and tree.a[ 0 ] != NULL )
    {
        //вставка в функцию начало
        Tree* tr = new Tree;
        sScope k;
        k.id= -1U -1;
        tr->data= k;
        tr->data.system_type = CODE_FUNC;
        tr->parent = &tree;
        tree.a[ 0 ]->a.insert( tree.a[ 0 ]->a.begin(), tr );
    }
    debug_message(5, "wight tree..." );
    wighttree( tree );
    debug_message(5,  "wight tree2..." );
    wighttree2( tree );
    //корректировка взвешивания для отрисовки.
    vector< Tree*> vt;
    //записываем ссылки на элементы в вектор.
    debug_message(5, "tree to line..." );
    treetoline( tree, vt);
    //блоки
    vector< SBlock> vb2( vt.size());
    SBlock an;
    vector< xyString> xystr;

    for( size_t i = 0; i < vt.size(); i++)
    {
        an.d = vt[ i]->data.d;
        an.type = vt[ i]->data.system_type;
        an.number = i;
        an.id = vt[ i]->data.id;
        vb2[ i] = an;
    }

    debug_message(5,  "delet some blocks...");
    delsomblocksTree( &tree );
    debug_message(5,  "Analize Tree..." );
    AnalizeTree( tree, vt, vl);
    //конец
    debug_message(5,  "Analize end" );
    sScopeFull b;
    b.id = a.id;
    b.refresh();
    int bigd = 0;


    for( list< SLink>::iterator it = vl.begin(); it != vl.end(); ++it)
    {
        if( bigd < it->d )
                bigd = ( *it).d;
    }
    string tempstr;
    tempstr = b.file_path;
    debug_message(5,  "Make report...");
    // строим отчет для блок схемы


    QFileInfo svg_tmp( file_name );
    QFileInfo code_tmp( code_file_name );
    QFile temp_svg_html_file    ( QFileInfo(code_file_name).absoluteDir().absolutePath()+"\\svg_html("+QString::number(b.id)+").html");

    QString svg_html_filename=QFileInfo(temp_svg_html_file).fileName();

    Make_Html_For_Bloc_Schem(
                                svg_html_filename,
                                code_tmp.fileName(),
                                index_file_name
                             );

    MakeBLokShemReport( xystr, vb2, vl, file_name.toStdString(), tempstr,  b.getQualifiedName().c_str(), bigd);

    ifstream fsvgi;
    ofstream fsvgo;
    fsvgi.open( file_name.toStdString().c_str());

    stringstream svg_cont;
    svg_cont << "<?xml version=\"1.0\" encoding=\"windows-1251\" standalone=\"no\"?>\n";
    string str1;
    getline( fsvgi, str1 );

    while( getline( fsvgi, str1 ) )
    {
        svg_cont << str1 << endl;
    }
    string straa = svg_cont.str();
    fsvgi.close();
    fsvgo.open( file_name.toStdString().c_str());
    size_t i = straa.find( "</g>", 4);

    if( i != straa.npos)
    {
        fsvgo << straa.substr( 0, i);
        fsvgo << "<text fill=\"#000000\" fill-opacity=\"1\" stroke=\"none\" x=\"10\" y=\"30\" font-family=\"Times\" font-size=\"7pt\" font-weight=\"40\" font-style=\"normal\" "
              << " >" << QObject::tr("Fynkcia: ").toStdString().c_str() << b.getQualifiedName() << "</text>" << endl; // Функция
        fsvgo << "<text fill=\"#000000\" fill-opacity=\"1\" stroke=\"none\" x=\"10\" y=\"40\" font-family=\"Times\" font-size=\"7pt\" font-weight=\"40\" font-style=\"normal\" "
              << " >" << QObject::tr(" v faile: ").toStdString().c_str() << tempstr.c_str() << "</text>" << endl; //  в файле:
        fsvgo << "</g>" << endl;
        fsvgo << straa.substr( i+4);
    }
    fsvgo.close();
    string ss1;
    ak_vs_db::get_scope_metadata ( b.id  , "args" , &ss1);
    // сопирую код в папку с отчетами
    CopyCodeToFile(QString(b.file_path.c_str()), b.pos_x1, b.pos_y1-1, b.pos_x2, b.pos_y2, code_file_name, QString( b.getQualifiedName().c_str())+ss1.c_str() );



    lst_fileblockshem.push_back( FileBlockShem( QFileInfo( file_name ).fileName(), a.id, QString( b.getQualifiedName().c_str() )+ss1.c_str(), QString( b.file_path.c_str() )  ) );
    test( &lst_fileblockshem );
    QFile temp_vs_file          ( QFileInfo(code_file_name).absoluteDir().absolutePath()+"\\vs.css");
    QFile temp_js_file          ( QFileInfo(code_file_name).absoluteDir().absolutePath()+"\\highlight.pack.js");
    QFile temp_up_file          ( QFileInfo(code_file_name).absoluteDir().absolutePath()+"\\up.html");
    QFile temp_down_file        ( QFileInfo(code_file_name).absoluteDir().absolutePath()+"\\down.html");
    //QFile temp_svg_html_file    ( QFileInfo(code_file_name).absoluteDir().absolutePath()+"\\svg_html("+QString::number(b.id)+").html");
    if(
        (false == temp_up_file.exists())
            ||
        (false == temp_down_file.exists())
            ||
        (false == temp_js_file.exists())
            ||
        (false == temp_vs_file.exists())
      )
    {
        copyFile( "./template\\vs.css" , QFileInfo(temp_vs_file).filePath() );
        copyFile( "./template\\highlight.pack.js" , QFileInfo(temp_js_file).filePath() );
        if( "ru" == liblang )
            copyFile( "./template\\template_for_block_scheme_up.tmp", QFileInfo(temp_up_file).filePath() );
        else if( "en" == liblang )
            copyFile( "./template\\template_for_block_scheme_up_en.tmp", QFileInfo(temp_up_file).filePath() );

        if( "ru" == liblang )
            copyFile( "./template\\template_for_block_scheme_down.tmp", QFileInfo(temp_down_file).filePath() );
        else if( "en" == liblang )
            copyFile( "./template\\template_for_block_scheme_down_en.tmp", QFileInfo(temp_down_file).filePath() );
    }
    //copyFile( "./template\\template_for_block_scheme_svg.tmp", QFileInfo(temp_svg_html_file).filePath() );
    copyFileReplace( "./template\\template_for_block_scheme_svg.tmp",
                     QFileInfo(temp_svg_html_file).filePath(),
                     QString("{{SVG_FILE}}"),
                     QString(svg_tmp.fileName())
                   );

#ifdef DEBAG_ALEX2
    ftime( &time2);
    cout << "   all_Trace_Block_Shem="
         << time2.time*1000 + time2.millitm - time1.time*1000 - time1.millitm
         << endl;
    cout << " work_time="
         << ( time2.time*1000 + time2.millitm - time1.time*1000 - time1.millitm)
         - ( sec2.time*1000 + sec2.millitm - sec1.millitm - sec1.time*1000)
         << "  base_time=" << ( 1.0*( sec2.time*1000 + sec2.millitm - sec1.millitm - sec1.time*1000))/( time2.time*1000 + time2.millitm - time1.time*1000 - time1.millitm)*100 << "%" << endl;

#endif
    return 1;
}

Tree & AnalizeCONTINUE(Tree &a)
{
    Tree * p =&a;
    for(int i=0; p->data.system_type!=CODE_CYCLE_FOR && p->data.system_type!=CODE_CYCLE_WHILE && p->data.system_type!=CODE_CYCLE_DO ; i++)
    {
        p=p->parent;
    }
    return *p;
}


vector< Tree*>& alltree( Tree &a, vector< Tree*> &v)
{
    v.push_back( &a);
    for( size_t i=0; i < a.a.size(); i++)
    {
        treetoline( *( a.a[ i]), v);
    }
    return v;
}


//находим линии в дереве.
int AnalizeTree( Tree &a, vec_pTree & block_vec, list< SLink>& link_vec)
{
    debug_message(5, "AnalizeTree: start" );
    //пройдемся по дереву еще раз для формирования вектора связей.
    for( size_t ji=0 ; ji < a.a.size() ; ji++ ) // j - это дети корня дерева
    {
        debug_message(5, "AnalizeTree: for start");

        Tree *p= a.a[ ji ];
        if( p == NULL) continue;
        debug_message( 5, "AnalizeTree: switch");

        //если текущий
        switch( p->data.system_type )
        {
        case CODE_BLOCK:
        {
            debug_message(5, "AnalizeTree: switch CODE_BLOCK");
            //проблема
            //если имеет детей то строим стрелку вниз до следующего ребенка, если нет детей то до следующего
            //SLink lin;
            //интеграция блоков
            for( int i=0 ; i < p->a.size() ; ++i )
            {
                if( p->a[ i ] == NULL ) continue;
                SLink lin;
                debug_message( 5, "AnalizeTree: switch CODE_BLOCK for" );
                if( i != (p->a.size()-1) )
                {
                    //=== рисуем линии от последних элементов к следующим
                    //находим последние элементы
                    vec_pTree *v= &findlasttoconnectDOWN( *( p->a[ i ]));
                    debug_message( 5, "findlasttoconnectDOWN: end");

                    for( size_t j = 0 ;  j < v->size() ; j++ ) // перебираем все последние элементы
                    {
                        debug_message( 5, "AnalizeTree: switch CODE_BLOCK for for");
                        lin.id_out= ( *v)[ j ]->data.id; // они будут исходящими для стрелки
                        debug_message( 5, "AnalizeTree: ( *v)[ j]->data.id" );
                        lin.type_out= ( *v)[ j ]->data.system_type;
                        debug_message( 5, "AnalizeTree: ( *v)[ j]->data.system_type" );
                        lin.d= ( *v)[ j ]->d;
                        debug_message( 5, "AnalizeTree: ( *v)[ j]->d" );

                        //p->a[  i+1 ]->data.system_type != CODE_BLOCK;
                        //cout << p->a[ i+1 ] << endl;
                        debug_message(5, "p->a[ i+1]->data.system_type != CODE_BLOCK");
                        // следующий должен отображаться. (иметь детей)
                        if( ( p->a.size() > i+1)  &&  p->a[ i+1 ] != NULL  ) // текущий элимент (для которого искали последних) должен иметь детей
                        {
                            if(  p->a[ i+1 ]->data.system_type != CODE_BLOCK ) // если следующим не коде блок, то стрелка до него
                            {
                                debug_message(5, "AnalizeTree: switch CODE_BLOCK for for if");
                                lin.id_in   = p->a[ i+1 ]->data.id;             //
                                lin.type_in = p->a[ i+1 ]->data.system_type;    //
                            }
                            else
                            {   // нийти последний элемент и его сделать в него ссылку

                                debug_message(5, "AnalizeTree: switch CODE_BLOCK for for if else");
                                pTree next = findfirstNotBlock( p->a[ i+1 ] );
                                lin.id_in = next->data.id;
                                lin.type_in = next->data.system_type;
                            }
                        }
                        else
                        {

                        }

                        debug_message( 5, "AnalizeTree: switch CODE_BLOCK for switch" );
                        // если последний элемент .... то делаем азные типы его окончания
                        switch( ( *v)[ j ]->data.system_type )
                        {
                            //если последний элемент IF
                            case CODE_IF:
                            {
                                if( ( *v)[ j ]->a.size() == 0)
                                {
                                    if( lin.id_in-lin.id_out == 1 )
                                    {
                                        lin.type= DOWN;
                                        link_vec.push_back( lin );
                                    }
                                    if( lin.id_in-lin.id_out > 1 )
                                    {
                                        lin.type= LEFT_DOWN;
                                        link_vec.push_back( lin );
                                    }
                                    lin.type= RIGHT_DOWN;
                                    link_vec.push_back( lin);
                                }
                                if( ( *v)[ j ]->a.size() == 1)
                                {
                                    lin.type= RIGHT_DOWN;
                                    link_vec.push_back( lin);
                                }
                                break;
                            }
                            case CODE_CODE:
                            {
                                if( lin.id_in-lin.id_out == 1 ) lin.type= DOWN;
                                if( lin.id_in-lin.id_out > 1) lin.type= LEFT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_FUNC:
                            {
                                if( lin.id_in-lin.id_out == 1) lin.type= DOWN;
                                if( lin.id_in-lin.id_out > 1) lin.type= LEFT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_CYCLE_FOR:
                            {
                                lin.type= RIGHT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_SWITCH:
                            {
                                lin.type= RIGHT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_SWITCH_VALUE:
                            {
                                lin.type= LEFT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_TRUE:
                            {
                                if( ( lin.id_in-lin.id_out) == 1) lin.type= DOWN;
                                if( ( lin.id_in-lin.id_out) != 1) lin.type= LEFT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_FALSE:
                            {
                                if( ( lin.id_in-lin.id_out) == 1 ) lin.type= DOWN;
                                if( ( lin.id_in-lin.id_out) != 1) lin.type= LEFT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_CYCLE_WHILE:
                            {
                                if( (lin.id_in-lin.id_out) == 1 ) lin.type= RIGHT_DOWN;
                                if( (lin.id_in-lin.id_out) != 1) lin.type= RIGHT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            case CODE_CYCLE_DO_WHILE:
                            {
                                if( (lin.id_in-lin.id_out) == 1 ) lin.type= DOWN;
                                if( (lin.id_in-lin.id_out) != 1) lin.type= LEFT_DOWN;
                                link_vec.push_back( lin);
                                break;
                            }
                            default:
                            {
                            }
                        }
                    }
                }
            }
            debug_message(5, "AnalizeTree: switch CODE_BLOCK end");
            //интеграция блоков
            break;
        }
        //блок иф
        case CODE_IF:
        {
            debug_message( 5, "AnalizeTree: switch CODE_IF");
            // рисуем стрелочку к первому ребенку
            //иф без кода для выполнения по условию
            //иф без елса
            if( p->a.size() == 1)
            {
                // от элемента if к true
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin );
            }
            // у ифа есть и иф и елс блоки.
            if( p->a.size() == 2)
            {
                // от элемента if к true
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);

                // от элемента if к else
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 1]->data.id;
                lin.type_in= p->a[ 1]->data.system_type;
                lin.d= p->d;
                lin.type= RIGHT_DOWN;
                link_vec.push_back( lin);
            }
            // если у if может быть скажем такие элементы как elif (язык питор)
            if( p->a.size() > 2 )
            {
                // от элемента if к true
                SLink lin;
                lin.id_out   = p->data.id;
                lin.type_out = p->data.system_type;
                lin.id_in    = p->a[ 0 ]->data.id;
                lin.type_in  = p->a[ 0 ]->data.system_type;
                lin.d        = p->d;
                lin.type     = DOWN;
                link_vec.push_back( lin );

                // от элемента if ко всем else
                for( int i=1 ; i < p->a.size() ; i++ )
                {
                    lin.id_out= p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in= p->a[ i ]->data.id;
                    lin.type_in= p->a[ i ]->data.system_type;
                    lin.d= p->d;
                    lin.type= RIGHT_DOWN;
                    link_vec.push_back( lin );
                }
            }
            break;
        }
        case CODE_TRY:
        {
            debug_message(5, "AnalizeTree: switch CODE_TRY");
            // рисуем стрелочку к первому ребенку
            //иф без кода для выполнения по условию
            //иф без елса
            if( p->a.size() == 1 &&  p->a[ 0 ] != NULL )
            {
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);
            }
            // у ифа есть и иф и елс блоки.
            if(p->a.size() == 2 && p->a[ 0]!=NULL && p->a[ 1]!=NULL)
            {
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);

                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 1]->data.id;
                lin.type_in= p->a[ 1]->data.system_type;
                lin.d= p->d;
                lin.type= RIGHT_DOWN;
                link_vec.push_back( lin);
            }
            // если у if может быть скажем такие элементы как elif (язык питор)
            if(p->a.size() > 2 && p->a[ 0]!=NULL)
            {
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);



                for(int i=1; i< p->a.size() ; i++)
                {
                    lin.id_out= p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in= p->a[ i ]->data.id;
                    lin.type_in= p->a[ i ]->data.system_type;
                    lin.d= p->d;
                    lin.type= RIGHT_DOWN;
                    link_vec.push_back( lin);
                }
            }
            break;
        }
        case CODE_CATCH:
        {
            debug_message(5, "AnalizeTree: switch CODE_CATCH");
            // рисуем стрелочку к первому ребенку
            //иф без кода для выполнения по условию
            //иф без елса
            if( p->a.size() == 1 && p->a[ 0]!=NULL)
            {
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);
            }
            // у ифа есть и иф и елс блоки.
            if(p->a.size() == 2 && p->a[ 0]!=NULL && p->a[ 1]!=NULL)
            {
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);

                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 1]->data.id;
                lin.type_in= p->a[ 1]->data.system_type;
                lin.d= p->d;
                lin.type= RIGHT_DOWN;
                link_vec.push_back( lin);
            }
            // если у if может быть скажем такие элементы как elif (язык питор)
            if( p->a.size() > 2 && p->a[ 0 ] != NULL)
            {
                SLink lin;
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0 ]->data.id;
                lin.type_in= p->a[ 0 ]->data.system_type;
                lin.d= p->d;
                lin.type= DOWN;
                link_vec.push_back( lin);

                for(int i=1; i< p->a.size() ; i++)
                {
                    if( p->a[ i ] == NULL) continue;
                    lin.id_out= p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in= p->a[ i ]->data.id;
                    lin.type_in= p->a[ i ]->data.system_type;
                    lin.d= p->d;
                    lin.type= RIGHT_DOWN;
                    link_vec.push_back( lin);
                }
            }
            break;
        }
        case CODE_TRUE:
        {
            debug_message(5, "AnalizeTree: switch CODE_TRUE" );
            SLink lin;
            //если в нем неходиться блок
            if( (p->a.size() != 0) && p->a[ 0]!=NULL && ( p->a[ 0 ]->data.system_type== CODE_BLOCK) && ( p->a[ 0 ]->a.size() != 0  ) && p->a[ 0]->a[ 0 ]!=NULL)
            {
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0]->a[ 0]->data.id;
                lin.type_in= p->a[ 0]->a[ 0]->data.system_type;
                lin.d= p->data.d;
                lin.type= DOWN;
                link_vec.push_back( lin);
            }
            //если блока нет
            if( ( p->a.size() != 0 && p->a[ 0 ]!=NULL && p->a[ 0 ]->data.system_type != CODE_BLOCK  ) )
            {
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0 ]->data.id;
                lin.type_in= p->a[ 0 ]->data.system_type;
                lin.d= p->data.d;
                lin.type= DOWN;
                link_vec.push_back( lin );
            }
            break;
        }
        case CODE_FALSE:
        {
            debug_message(5, "AnalizeTree: switch CODE_FALSE");
            SLink lin;
            //если в нем неходиться блок
            if( ( p->a.size() != 0) && ( p->a[ 0 ]->a.size() != 0 ) && p->a[ 0 ] != NULL && p->a[ 0 ]->a[ 0 ] != NULL && ( p->a[ 0 ]->data.system_type == CODE_BLOCK) )
            {
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0 ]->a[ 0 ]->data.id;
                lin.type_in= p->a[ 0 ]->a[ 0 ]->data.system_type;
                lin.d= p->data.d;
                lin.type= DOWN;
                link_vec.push_back( lin );
            }
            //если блока нет
            if(  p->a.size() != 0 && p->a[ 0 ] != NULL && p->a[ 0 ]->data.system_type != CODE_BLOCK   )
            {
                lin.id_out= p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in= p->a[ 0 ]->data.id;
                lin.type_in= p->a[ 0 ]->data.system_type;
                lin.d= p->data.d;
                lin.type= DOWN;
                link_vec.push_back( lin );
            }
            break;
        }
        //код переключатель
        case CODE_SWITCH:
        {
            debug_message( 5, "AnalizeTree: switch CODE_SWITCH" );
            ////От switch рисуем справа вниз стрелочки к первым блокам детей.
            SLink lin;
            if( p->a.size() != 0 )
            {
                if( p->a[ 0 ]!=NULL && p->a[ 0 ]->a.size() != 0)
                {
                    for( size_t j=0; j< p->a[ 0]->a.size(); j++)
                    {
                        if( p->a[0]->a[ j] == NULL) continue;
                        lin.id_out= p->data.id;
                        lin.type_out= p->data.system_type;
                        lin.id_in= p->a[ 0]->a[ j]->data.id;
                        lin.type_in= p->a[ 0]->a[ j]->data.system_type;
                        lin.d= p->d;
                        lin.type= RIGHT_DOWN;
                        link_vec.push_back( lin );
                    }
                }
            }
            break;
        }
        //блок в переключателе
        case CODE_SWITCH_VALUE:
        {
            debug_message(5, "AnalizeTree: switch CODE_SWITCH_VALUE");
            ////От switch рисуем справа вниз стрелочки к первым блокам детей.
            SLink lin;
            if(  p->a.size()!= 0 && p->a[ 0] != NULL)
            {
                lin.id_out= p->data.id;
                lin.d= p->d;
                pTree next= findfirstNotBlock( p->a[ 0]);
                lin.id_in= next->data.id;
                lin.type_in= next->data.system_type;
                lin.type= DOWN;
                link_vec.push_back( lin);
            }
            break;
        }			// ...
        case CODE_CYCLE_FOR:
        {
            debug_message(5, "AnalizeTree: switch CODE_CYCLE_FOR");
            if( p->a.size() == 0)
            {
                SLink lin;
                //обратная стрелка.
                lin.id_out= p->data.id;
                lin.type_out=p->data.system_type;
                lin.id_in= p->data.id;
                lin.type_in= p->data.system_type;
                lin.d=p->d;
                lin.type= LEFT_UP;
                link_vec.push_back( lin);
            }
            if( p->a.size() != 0)
            {
                //если есть блок и в нем что-то есть
                if( p->a.size() != 0 && ( p->a[ 0]->a.size()!= 0) && (  p->a[ 0] != NULL &&  p->a[ 0]->a[ 0]!=NULL && p->a[ 0]->data.system_type== CODE_BLOCK) )
                {
                    SLink lin;
                    //стрелка вниз.
                    lin.id_out      = p->data.id;
                    lin.type_out    = p->data.system_type;
                    lin.id_in       = p->a[ 0]->a[ 0]->data.id;
                    lin.type_in     = p->a[ 0]->a[ 0]->data.system_type;
                    lin.d           = p->d;
                    lin.type        = DOWN;
                    link_vec.push_back( lin);
                    //стрелка обратно.
                    vec_pTree *v = &findlasttoconnectDOWN( *( p->a[ 0]->a[ p->a[ 0]->a.size()-1 ]));
                    for( size_t j=0; j< v->size(); j++ )
                    {
                        lin.id_out  = ( *v)[ j]->data.id;
                        lin.type_out= ( *v)[ j]->data.system_type;
                        lin.id_in   = p->data.id;
                        lin.type_in = p->data.system_type;
                        lin.d       = p->d;
                        //нужно перечислить все возможные случаи.
                        switch((*v)[j]->data.system_type)
                        {
                        case CODE_TRUE:
                            lin.type    = LEFT_UP;
                            break;
                        case CODE_FALSE:
                            lin.type    = LEFT_UP;
                            break;
                        case CODE_CODE:
                            lin.type    = LEFT_UP;
                            break;
                        case CODE_CONTINUE:
                            lin.type    = LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH:
                            lin.type    = LEFT_UP_LEFT;
                            break;
                        case CODE_IF:
                        {
                            if( (*v)[j]->a.size()==0 )
                            {
                                lin.type=LEFT_UP;
                                link_vec.push_back(lin);
                            }
                            lin.type=LEFT_UP_LEFT;
                        }
                        break;
                        case CODE_CYCLE_FOR:
                            lin.type    = LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_WHILE:
                            lin.type    = LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH_VALUE:
                            lin.type    = LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_DO_WHILE:
                            lin.type    = LEFT_UP_LEFT;
                            break;
                        default:
                        {}
                        }
                        link_vec.push_back(lin);
                    }
                }
                //если есть блок и внем ничего нет
                if(  p->a[ 0] != NULL && ( p->a[ 0]->data.system_type== CODE_BLOCK) &&
                        ( p->a[ 0]->a.size()== 0) )
                {
                    SLink lin;
                    //обратная стрелка.
                    lin.id_out  = p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in   = p->data.id;
                    lin.type_in = p->data.system_type;
                    lin.d       = p->data.d;
                    lin.type    = LEFT_UP;
                    link_vec.push_back( lin);
                }
                //если в есть подскоуп и он не блок
                if(  p->a.size()!=0 && p->a[ 0]!=NULL && p->a[ 0]->data.system_type!= CODE_BLOCK)
                {
                    //рисуем стрелку вниз
                    SLink lin;
                    lin.id_out  = p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in   = p->a[0]->data.id;
                    lin.type_in = p->a[0]->data.system_type;
                    lin.d       = p->d;
                    lin.type    = DOWN;
                    link_vec.push_back(lin);
                    //обратная стрелка.
                    vector< Tree*> *v   = &findlasttoconnectDOWN( *( p->a[ p->a.size()-1 ]));
                    for( size_t j=0; j< v->size(); j++ )
                    {
                        if((*v)[j] == NULL) continue;
                        lin.id_out  =   (*v)[j]->data.id;
                        lin.type_out=   (*v)[j]->data.system_type;
                        lin.id_in   =   p->data.id;
                        lin.type_in =   p->data.system_type;
                        lin.d       =   p->d;
                        //нужно перечислить все возможные случаи.
                        switch((*v)[j]->data.system_type)
                        {
                        case CODE_TRUE:
                            lin.type    =   LEFT_UP;
                            break;
                        case CODE_FALSE:
                            lin.type    =   LEFT_UP;
                            break;
                        case CODE_CODE:
                            lin.type    =   LEFT_UP;
                            break;
                        case CODE_CONTINUE:
                            lin.type    =   LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH:
                            lin.type    =   LEFT_UP_LEFT;
                            break;
                        case CODE_IF:
                        {
                            if( (*v)[j]->a.size()==0 )
                            {
                                lin.type    =   LEFT_UP;
                                link_vec.push_back(lin);
                            }
                            lin.type=LEFT_UP_LEFT;
                        }
                        break;
                        case CODE_CYCLE_FOR:
                            lin.type    =   LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_WHILE:
                            lin.type    =   LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH_VALUE:
                            lin.type    =   LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_DO_WHILE:
                            lin.type    =   LEFT_UP_LEFT;
                            break;
                        default:
                        {}
                        }
                        link_vec.push_back(lin);
                    }
                }
            }
            break;
        }
        case CODE_CYCLE_DO:
        {
            debug_message(5, "AnalizeTree: switch CODE_CYCLE_DO");
            //Рисуем стрелку вниз от do  до первого блока листов.
            //От последнего рисуем прямую вниз линию до while.
            //От while рисуем стрелку с права (от блока) до блока do.
            //рисуем стрелку вниз
            SLink lin;
            if( p->a.size() == 1 &&  p->a[ 0]!=NULL && p->a[0]->data.system_type == CODE_CYCLE_DO_WHILE )
            {
                //стрелка вниз
                lin.id_out  = p->data.id;
                lin.id_in   = p->a[0]->data.id;
                lin.d       = p->d;
                lin.type    = DOWN;
                link_vec.push_back(lin);
                //обратная стрелка
                lin.id_out  = p->a[0]->data.id;
                lin.id_in   = p->data.id;
                lin.d       = p->d;
                lin.type    = LEFT_UP;
                link_vec.push_back(lin);
            }
            if( p->a.size()== 2 &&  p->a[ 0]!=NULL  &&  p->a[ 1 ]!=NULL && p->a[ 0]->data.system_type!= CODE_BLOCK)
            {
                //стрелка вниз
                lin.id_out  = p->data.id;
                lin.id_in   = p->a[0]->data.id;
                lin.d       = p->d;
                lin.type    = DOWN;
                link_vec.push_back(lin);
                //стрелка вниз
                lin.id_out  = p->a[ 0]->data.id;
                lin.id_in   = p->a[ 1]->data.id;
                lin.d       = p->d;
                lin.type    = DOWN;
                link_vec.push_back( lin);
                //обратная стрелка
                lin.id_out  = p->a[ 1]->data.id;
                lin.id_in   = p->data.id;
                lin.d       = p->d;
                lin.type    = LEFT_UP;
                link_vec.push_back( lin);
            }
            if( p->a.size() == 2 &&  p->a[ 0]!=NULL && p->a[ 0]->data.system_type == CODE_BLOCK )
            {
                //стрелка вниз
                lin.id_out  = p->data.id;
                Tree *next  = findfirstNotBlock( p);
                lin.id_in   = next->data.id;
                lin.d       = p->d;
                lin.type    = DOWN;
                link_vec.push_back(lin);
                //стрелка вниз
                vec_pTree *v=&findlasttoconnectDOWN( *( p->a[ 0]) );
                //стрелка от последнего
                for( size_t j=0; j<v->size(); j++ )
                {
                    lin.id_out  = ( *v)[ j]->data.id;
                    lin.type_out= ( *v)[ j]->data.system_type;
                    lin.d       = p->d;
                    lin.id_in   = p->a[ 1]->data.id;
                    lin.type_in = p->a[ 1]->data.system_type;
                    switch( ( *v)[ j]->data.system_type )
                    {
                        //если последний элемент IF
                    case CODE_IF:
                    {
                        if( ( *v)[ j]->a.size()==0 )
                        {
                            if( lin.id_in-lin.id_out==1 )
                            {
                                lin.type  = DOWN;
                                link_vec.push_back(lin);
                            }
                            if( lin.id_in-lin.id_out >1 )
                            {
                                lin.type  = LEFT_DOWN;
                                link_vec.push_back(lin);
                            }
                            lin.type=RIGHT_DOWN;
                            link_vec.push_back(lin);
                        }
                        if( (*v)[j]->a.size()==1 )
                        {
                            lin.type    =   RIGHT_DOWN;
                            link_vec.push_back( lin);
                        }
                        break;
                    }
                    case CODE_CODE:
                    {
                        if( lin.id_in-lin.id_out==1 ) lin.type=DOWN;
                        if( lin.id_in-lin.id_out >1 ) lin.type=LEFT_DOWN;
                        link_vec.push_back( lin);
                        break;
                    }
                    case CODE_CYCLE_FOR:
                    {
                        lin.type    =   RIGHT_DOWN;
                        link_vec.push_back( lin);
                        break;
                    }
                    case CODE_SWITCH:
                    {
                        lin.type    =   RIGHT_DOWN;
                        link_vec.push_back( lin);
                        break;
                    }
                    case CODE_SWITCH_VALUE:
                    {
                        lin.type    =   LEFT_DOWN;
                        link_vec.push_back( lin);
                        break;
                    }
                    case CODE_TRUE:
                    {
                        if( lin.id_in-lin.id_out==1 ) lin.type= DOWN;
                        if( lin.id_in-lin.id_out!=1 ) lin.type= LEFT_DOWN;
                        link_vec.push_back( lin);
                        break;
                    }
                    case CODE_FALSE:
                    {
                        if( lin.id_in-lin.id_out==1 ) lin.type=DOWN;
                        if( lin.id_in-lin.id_out!=1 ) lin.type=LEFT_DOWN;
                        link_vec.push_back(lin);
                        break;
                    }
                    case CODE_CYCLE_WHILE:
                    {
                        if( lin.id_in-lin.id_out==1 ) lin.type=RIGHT_DOWN;
                        if( lin.id_in-lin.id_out!=1 ) lin.type=RIGHT_DOWN;
                        link_vec.push_back(lin);
                        break;
                    }
                    case CODE_CYCLE_DO_WHILE:
                    {
                        if( lin.id_in-lin.id_out==1 ) lin.type=DOWN;
                        if( lin.id_in-lin.id_out!=1 ) lin.type=LEFT_DOWN;
                        link_vec.push_back(lin);
                        break;
                    }
                    default:
                    {}
                    }
                }
                //обратная стрелка
                lin.id_out  = p->a[ 1]->data.id;
                lin.id_in   = p->data.id;
                lin.d       = p->d;
                lin.type    = LEFT_UP;
                link_vec.push_back( lin);
            }
            break;
        }
        // ...
        case CODE_CYCLE_WHILE:
        {
            debug_message(5, "AnalizeTree: switch CODE_CYCLE_WHILE");
            if( p->a.size() == 0 )
            {
                SLink lin;
                //обратная стрелка.
                lin.id_out  = p->data.id;
                lin.type_out= p->data.system_type;
                lin.id_in   = p->data.id;
                lin.type_in = p->data.system_type;
                lin.d       = p->d;
                lin.type    = LEFT_UP;
                link_vec.push_back(lin);
            }
            if( p->a.size()!=0 )
            {
                //если есть блок и в нем что-то есть
                if(  p->a[ 0]!=NULL && (p->a[ 0 ]->a.size() != 0 ) && p->a[ 0]->a[ 0]!=NULL && (p->a[0]->data.system_type==CODE_BLOCK)  )
                {
                    SLink lin;
                    //стрелка вниз.
                    lin.id_out  = p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in   = p->a[0]->a[0]->data.id;
                    lin.type_in = p->a[0]->a[0]->data.system_type;
                    lin.d       = p->d;
                    lin.type    = DOWN;
                    link_vec.push_back(lin);

                    //стрелка обратно.

                    vec_pTree *v=&findlasttoconnectDOWN( *(p->a[0]->a[ p->a[0]->a.size()-1 ]) );
                    for( size_t j=0; j<v->size(); j++)
                    {
                        lin.id_out  =  ( *v)[ j]->data.id;
                        lin.type_out=  ( *v)[ j]->data.system_type;
                        lin.id_in   =  p->data.id;
                        lin.type_in =  p->data.system_type;

                        lin.d=p->d;
                        //нужно перечислить все возможные случаи.
                        switch( (*v)[ j]->data.system_type )
                        {
                        case CODE_TRUE:
                            lin.type=LEFT_UP;
                            break;
                        case CODE_FALSE:
                            lin.type=LEFT_UP;
                            break;
                        case CODE_CODE:
                            lin.type=LEFT_UP;
                            break;
                        case CODE_CONTINUE:
                            lin.type=LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH:
                            lin.type=LEFT_UP_LEFT;
                            break;
                        case CODE_IF:
                        {
                            if( (*v)[j]->a.size()==0 )
                            {
                                lin.type=LEFT_UP;
                                link_vec.push_back(lin);
                            }
                            lin.type=LEFT_UP_LEFT;
                        }
                        break;
                        case CODE_CYCLE_FOR:
                            lin.type=LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_WHILE:
                            lin.type=LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH_VALUE:
                            lin.type=LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_DO_WHILE:
                            lin.type=LEFT_UP_LEFT;
                            break;
                        default:
                        {}
                        }
                        link_vec.push_back( lin);
                    }
                }
                //если есть блок и внем ничего нет
                if(  p->a.size() != 0 && p->a[ 0] != NULL && (p->a[ 0 ]->data.system_type == CODE_BLOCK) && ( p->a[ 0]->a.size() == 0) )
                {
                    SLink lin;
                    //обратная стрелка.
                    lin.id_out  = p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in   = p->data.id;
                    lin.type_in = p->data.system_type;
                    lin.d       = p->d;
                    lin.type    = LEFT_UP;
                    link_vec.push_back( lin);
                }

                //если в есть подскоуп и он не блок
                if(  p->a.size()!=0 && p->a[ 0 ] != NULL && p->a[0]->data.system_type != CODE_BLOCK)
                {
                    //рисуем стрелку вниз
                    SLink lin;
                    lin.id_out  = p->data.id;
                    lin.type_out= p->data.system_type;
                    lin.id_in   = p->a[ 0]->data.id;
                    lin.type_in = p->a[ 0]->data.system_type;
                    lin.d       = p->d;
                    lin.type    = DOWN;
                    link_vec.push_back( lin);

                    //обратная стрелка.
                    vec_pTree *v=&findlasttoconnectDOWN( *( p->a[ p->a.size()-1 ]) );
                    for( size_t j=0; j<v->size(); j++ )
                    {
                        if( (*v)[ j ] == NULL) continue;

                        lin.id_out  = (*v)[j]->data.id;
                        lin.type_out= (*v)[j]->data.system_type;
                        lin.id_in   = p->data.id;
                        lin.type_in = p->data.system_type;
                        lin.d       = p->d;
                        //нужно перечислить все возможные случаи.
                        switch( (*v)[j]->data.system_type )
                        {
                        case CODE_TRUE:
                            lin.type= LEFT_UP;
                            break;
                        case CODE_FALSE:
                            lin.type= LEFT_UP;
                            break;
                        case CODE_CODE:
                            lin.type= LEFT_UP;
                            break;
                        case CODE_CONTINUE:
                            lin.type= LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH:
                            lin.type= LEFT_UP_LEFT;
                            break;
                        case CODE_IF:
                        {
                            if((*v)[j]->a.size()==0)
                            {
                                lin.type=LEFT_UP;
                                link_vec.push_back(lin);
                            }
                            lin.type=LEFT_UP_LEFT;
                        }
                        break;
                        case CODE_CYCLE_FOR:
                            lin.type= LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_WHILE:
                            lin.type= LEFT_UP_LEFT;
                            break;
                        case CODE_SWITCH_VALUE:
                            lin.type= LEFT_UP_LEFT;
                            break;
                        case CODE_CYCLE_DO_WHILE:
                            lin.type= LEFT_UP_LEFT ;
                            break;
                        default:
                        {}
                        }
                        link_vec.push_back( lin);
                    }
                }
            }
            break;
        }
        default:
        {
            debug_message(5, "AnalizeTree: switch default");
        }
        }
        debug_message(5, "AnalizeTree: down" );
        AnalizeTree( *p, block_vec, link_vec);
    }
    debug_message(5, "AnalizeTree: return" );
    return 1;
}


// рисование всех блоков на QImage.
// создать QImage и рисовать на нем блоки.
//vb - вектор SBlock-ов которые мы будем рисовать на холст
//vl - вектор SLink -ов стрелок.
int MakeBLokShemReport( vector< xyString>& xystr, vector< SBlock> vb, list< SLink> vl, string file_name, string func_file, string func_name, int bigd)
{

    ///=============добавление стрелок вниз от всех не имеющих блочков к последнему блоку.
    std::list<SBlock> lst_has_no_out;
    SBlock end_elem;

    for(int i=0; i < vb.size(); i++)
    {
        int currid = vb[i].id;
        bool has_out=false;
        for( list<SLink>::iterator it = vl.begin(); it!= vl.end() ; it++)
        {
            if( (*it).id_out == currid )
                has_out = true;
        }
        if(
            has_out != true &&
            vb[i].type != CODE_END &&
            vb[i].type != CODE_FUNC && i !=0
          )     lst_has_no_out.push_back( vb[ i] );

        if( vb[i].type == CODE_END )
                end_elem=vb[i];
    }
    //cout << " new block scheme" << endl;
    int current_d_tmp=1;
    for(list<SBlock>::iterator it= lst_has_no_out.begin(); it != lst_has_no_out.end(); it++)
    {
        SLink new_link;
        // cout << " id = " << (*it).id << " type= " << (*it).type  << endl;
        new_link.d      = current_d_tmp++;
        new_link.id_in  = end_elem.id;
        new_link.id_out = (*it).id;
//        new_list.number_in  =
//        new_list.number_out =
        new_link.type   = LEFT_DOWN;
//        new_list.type_in    =
//        new_list.type_out   =
//        new_list.x_in   =
//        new_list.x_out  =
//        new_list.y_in   =
//        new_list.y_out  =
        vl.push_back(new_link);
    }
    //cout << " end block scheme" << endl;
////===================================================

//высчитываем размеры холста
    //расчитываем размеры полотна по Y из размера vb
    int y= vb.size()*( BLOCK_Y+BLOCK_D);
    //устанавливаем x холста в дольнейшем нужно подобрать.
    int x= bigd*LENTH_RIGHT*2+2*BLOCK_X;
//конец вычисления размер холста
    QSvgGenerator svg;
    svg.setFileName( file_name.c_str());
    svg.setSize( QSize( x, y));
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "Windows-1251"));
    QPainter pt;
//закрасим весь холст белой краской
    pt.begin( &svg);
    QBrush br( QColor( 255 ,255, 255));
    pt.fillRect( 0, 0, x, y, br);
    pt.setFont( QFont( "Times", 15, QFont::Normal));
//конец
//размещаем на полотне все элементы вводя в структуру элемента координаты на холсте.
//в дольнейшем эти элементы будут отрисовываться
    int seredina= bigd*LENTH_RIGHT+BLOCK_X;
    int const1= BLOCK_Y+BLOCK_D;// переменная для оптимизации.
    char cha[ 10 ];
    for( size_t i=0 ; i < vb.size() ; i++ )
    {
        vb[ i].x  = seredina;
        vb[ i].y  = i*(const1);
        xyString xych;
        xych.x   = seredina;
        xych.y   = i*(const1);
        itoa( vb[ i].id, cha, 10);
        xych.str = cha;
        //get_code_from_file (std::string file_name, int pos_string, std::string& str_code, unsigned int iRange=0);
        xystr.push_back( xych );
    }
//конец
//связем линии с координатами.
    for( list< SLink>::iterator vliter= vl.begin(); vliter!=vl.end(); ++vliter )
    {
        for( vector< SBlock>::iterator vbit=vb.begin(); vbit!=vb.end(); ++vbit )
        {
            if( vliter->id_out== vbit->id )
            {
                vliter->x_out       = vbit->x;
                vliter->y_out       = vbit->y;
                vliter->number_out  = vbit->number;
            }
            if( ( *vliter).id_in== ( *vbit).id)
            {
                vliter->x_in     = vbit->x;
                vliter->y_in     = vbit->y;
                vliter->number_in= vbit->number;
            }
        }
        if( vliter->number_out-vliter->number_in==-1
                && vliter->type_out != CODE_IF
                && vliter->type_out != CODE_CYCLE_FOR
                && vliter->type_out != CODE_SWITCH )
        {
            vliter->type = DOWN;
        }
    }
    pt.setPen( QColor( 0, 0, 0));

//отрисовываем на полотне блоки из вектора блоков
    for( vector< SBlock>::iterator vbit=vb.begin(); vbit!= vb.end(); ++vbit)
    {
        //треугольные блоки на полотне.
        switch( ( *vbit).type)
        {
        case CODE_END:
        {
            draw_END( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_FUNC:
        {
            draw_Begin( ( *vbit).x, ( *vbit).y,pt, ( *vbit).id);
            break;
        }
        case CODE_CYCLE_FOR:
        {
            draw_Cycl( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_CYCLE_WHILE:
        {
            draw_Cycl( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_IF:
        {
            drawIF_ROMB( ( *vbit).x, ( *vbit).y, pt , ( *vbit).id);
            break;
        }
        case CODE_CYCLE_DO:
        {
            drawDO( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_CYCLE_DO_WHILE:
        {
            drawDO_WHILE((*vbit).x,(*vbit).y, pt , (*vbit).id );
            break;
        }
        case CODE_CODE:
        {
            drawCODE( ( *vbit).x, ( *vbit).y,pt, ( *vbit).id);
            break;
        }
        case CODE_SWITCH:
        {
            drawSwitch( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_SWITCH_VALUE:
        {
            drawcase( ( *vbit).x, ( *vbit).y,pt, ( *vbit).id);
            break;
        }
        case CODE_BREAK:
        {
            drawBreak( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_RETURN:
        {
            drawreturn( ( *vbit).x, ( *vbit).y,pt, ( *vbit).id);
            break;
        }
        case CODE_CONTINUE:
        {
            drawcontinue( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_TRUE:
        {
            drawthen( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_FALSE:
        {
            drawelse( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_GOTO:
        {
            drawGOTO( ( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_TRY:
        {
            drawTRY(( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        case CODE_CATCH:
        {
            drawCATCH(( *vbit).x, ( *vbit).y, pt, ( *vbit).id);
            break;
        }
        }
    }
//конец рисования блоков
    //здесь ожидается отрисовка на полотне связи.

    for( list< SLink>::iterator vliter=vl.begin(); vliter!= vl.end(); ++vliter )
    {
        switch( ( *vliter).type)
        {
            //рисование стрелок вниз
        case DOWN:
        {
            //стрелка вниз
            drawLink_down( (*vliter).x_out, (*vliter).y_out, pt);
            break;
        }
        //рисование стрелок справа и вниз
        case RIGHT_DOWN:
        {
            //стрелка вниз с права(в цикле if такая)
            drawLink_Down_right( vliter->x_out, vliter->y_out,
                                 vliter->x_in, vliter->y_in,
                                 vliter->d, pt);
            break;
        }
        //рисование стрелок слева вниз.
        case LEFT_DOWN:
        {
            //стрелка вниз с лева (в цикле if-else такая)
            drawLink_Down_left( vliter->x_out, vliter->y_out,
                                vliter->x_in, vliter->y_in,
                                vliter->d, pt);
            break;
        }
        case LEFT_UP:
        {
            //стрелка слева в верх.(из послеблочья в блок.)
            drawLink_Up_Left( vliter->x_out, vliter->y_out,
                              vliter->x_in, vliter->y_in,
                              vliter->d, pt);
            break;
        }
        case LEFT_UP_LEFT:
        {
            drawLink_Left_Up_Left( vliter->x_out, vliter->y_out,
                                   vliter->x_in, vliter->y_in,
                                   vliter->d, pt);
            break;
        }
        }
    }
    //конец расстоновки стрелочек на полотне
    pt.end();
    //конец рисования объектов на полотне.
    //выходим.
    return 0;
}



//>>>>>>>>>>>>>>>>>>>>>>>Рисование Объектов>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//рисование кводрата на img с кординатами X,Y
inline QPainter & drawTRY( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x, y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.3), ( int)( y+BLOCK_Y/1.7), "TRY");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", ( int)BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}


inline QPainter & drawCATCH( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x, y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.3), ( int)( y+BLOCK_Y/1.7), "CATCH");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", ( int)BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

//рисование кводрата на img с кординатами X,Y
inline QPainter & drawCODE( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x, y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.3), ( int)( y+BLOCK_Y/1.7), "CODE");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", ( int)BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter &   drawBreak( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x, y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.3), ( int)( y+BLOCK_Y/1.7), "BREAK");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}
inline QPainter &   drawthen( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x,y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.3), ( int)( y+BLOCK_Y/1.7), "THEN");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter &   drawelse(int x,int y,QPainter &pt,int id)
{
    pt.drawLine( x, y, x + BLOCK_X, y);
    pt.drawLine( x + BLOCK_X, y, x + BLOCK_X, y + BLOCK_Y);
    pt.drawLine( x + BLOCK_X, y + BLOCK_Y, x, y + BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", (int)(BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( (int)( x+BLOCK_X/3.2), (int)( y+BLOCK_Y/1.7), "ELSE");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawGOTO( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x, y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", (int)(BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( (int)( x+BLOCK_X/3.3), (int)( y+BLOCK_Y/1.7), "GOTO");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal) );
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawcontinue( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+ BLOCK_X, y, x+BLOCK_X, y+ BLOCK_Y);
    pt.drawLine( x+ BLOCK_X, y+ BLOCK_Y, x, y+ BLOCK_Y);
    pt.drawLine( x, y+ BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText(int(x+BLOCK_X/7),int(y+BLOCK_Y/1.7),"CONTINUE");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[10];
    itoa(id,a,10);
    pt.setFont(QFont("Times",BLOCK_Y/3,QFont::Normal));
    pt.drawText(400,y+BLOCK_Y/2,a);
#endif
    return pt;
}

inline QPainter & drawreturn( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y, x+BLOCK_X, y);
    pt.drawLine( x+BLOCK_X, y, x+BLOCK_X, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y, x, y+BLOCK_Y);
    pt.drawLine( x, y+BLOCK_Y, x, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/4), (int)( y+BLOCK_Y/1.7), "RETURN");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[10];
    itoa(id,a,10);

    pt.setFont(QFont("Times",BLOCK_Y/3,QFont::Normal));
    pt.drawText(400,y+BLOCK_Y/2,a);
#endif
    return pt;
}
//рисование ромба с координатами x,y на img
inline QPainter & drawIF_ROMB( int x,int y ,QPainter &pt, int id)
{
    //нужно рисовать по госту
    pt.drawLine( x+BLOCK_X/2, y, x+BLOCK_X, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y/2, x+BLOCK_X/2, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X/2, y+BLOCK_Y, x, y+BLOCK_Y/2);
    pt.drawLine( x, y+BLOCK_Y/2, x+BLOCK_X/2, y);
    //надпись
    pt.setFont( QFont("Times",(int)(BLOCK_Y/3.5),QFont::Normal));
    pt.drawText( (int)( x+BLOCK_X/2.5), (int)( y+BLOCK_Y/1.7), "IF");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawDO( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y+BLOCK_Y/2, x+BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_Y/2, y, x+BLOCK_X-BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X-BLOCK_Y/2, y, x+BLOCK_X, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y/2, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    //надпись
    pt.setFont(QFont("Times",(int)(BLOCK_Y/3.5),QFont::Normal));
    pt.drawText(int(x+BLOCK_X/2.5),int(y+BLOCK_Y/1.7),"DO");
    //тестовая информация
#ifdef DEBAG_ALEX
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawDO_WHILE( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y+BLOCK_Y/2, x+BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_Y/2, y, x+BLOCK_X-BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X-BLOCK_Y/2, y, x+BLOCK_X, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y/2, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    //надпись
    pt.setFont( QFont( "Times", (int)(BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( x+BLOCK_X/4, int(y+BLOCK_Y/1.7), "WHILE");
    //тестовая информация
#ifdef DEBAG_ALEX
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawSwitch( int x, int y ,QPainter &pt, int id)
{
    //нужно рисовать по госту
    pt.drawLine( x+BLOCK_X/2, y, x+BLOCK_X, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y/2, x+BLOCK_X/2, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X/2, y+BLOCK_Y, x, y+BLOCK_Y/2);
    pt.drawLine( x, y+BLOCK_Y/2, x+BLOCK_X/2, y);
    //надпись
    pt.setFont( QFont( "Times", BLOCK_Y/4, QFont::Normal));
    pt.drawText( x+BLOCK_X/4, int( y+BLOCK_Y/1.7), "SWITCH");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawcase( int x, int y, QPainter &pt, int id)
{
    //нужно рисовать по госту
    pt.drawLine( x+BLOCK_X/2, y, x+BLOCK_X, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y/2, x+BLOCK_X/2, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X/2, y+BLOCK_Y, x, y+BLOCK_Y/2);
    pt.drawLine( x, y+BLOCK_Y/2, x+BLOCK_X/2, y);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.3), ( int)(y+BLOCK_Y/1.7), "CASE");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

//<<<<<<<<<<<<<<<<<<<<<<Конец рисования объектов.
//>>>>>>>>>>>>>>>>>>>>>>>>>Рисование линий>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inline QPainter& drawLink_down( int x, int y, QPainter &pt)
{
    pt.drawLine( x+BLOCK_X/2, y+BLOCK_Y,x+BLOCK_X/2, y+BLOCK_Y+BLOCK_D);
    pt.drawLine( x+BLOCK_X/2, y+BLOCK_Y+BLOCK_D, x+BLOCK_X/2+BLOCK_D/5, y+BLOCK_Y+BLOCK_D-BLOCK_D/5);
    pt.drawLine( x+BLOCK_X/2, y+BLOCK_Y+BLOCK_D, x+BLOCK_X/2-BLOCK_D/5, y+BLOCK_Y+BLOCK_D-BLOCK_D/5);
    return pt;
}
//рисуем элепс
inline QPainter & draw_Begin( int x, int y, QPainter &pt, int id)
{
    pt.drawArc( x, y, BLOCK_Y, BLOCK_Y, 90*16, 180*16 );
    pt.drawLine( x+BLOCK_Y/2, y, x+BLOCK_X-BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    pt.drawArc( x+BLOCK_X-BLOCK_Y, y, BLOCK_Y, BLOCK_Y, -90*16, 180*16);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( ( int)( x+BLOCK_X/3.5), ( int)( y+BLOCK_Y/1.7), "BEGIN");
#ifdef DEBAG_ALEX

    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, ( int)y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & draw_END( int x, int y, QPainter &pt, int id)
{
    pt.drawArc( x, y, BLOCK_Y, BLOCK_Y, 90*16, 180*16);
    pt.drawLine( x+BLOCK_Y/2, y, x+BLOCK_X-BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    pt.drawArc( x+BLOCK_X-BLOCK_Y, y, BLOCK_Y, BLOCK_Y, -90*16, 180*16);
    //надпись
    pt.setFont( QFont( "Times", ( int)( BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( int( x+BLOCK_X/2.7), int(y+BLOCK_Y/1.7), "END");
#ifdef DEBAG_ALEX
    //тестовая информация
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}


//рисование цикла
inline QPainter & draw_Cycl( int x, int y, QPainter &pt, int id)
{
    pt.drawLine( x, y+BLOCK_Y/2, x+BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_Y/2, y, x+BLOCK_X-BLOCK_Y/2, y);
    pt.drawLine( x+BLOCK_Y/2, y+BLOCK_Y, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    pt.drawLine( x+BLOCK_X-BLOCK_Y/2, y, x+BLOCK_X, y+BLOCK_Y/2);
    pt.drawLine( x+BLOCK_X, y+BLOCK_Y/2, x+BLOCK_X-BLOCK_Y/2, y+BLOCK_Y);
    //надпись
    pt.setFont( QFont( "Times", (int)(BLOCK_Y/3.5), QFont::Normal));
    pt.drawText( x+BLOCK_X/4, int( y+BLOCK_Y/1.7), "CYCLE");
    //тестовая информация
#ifdef DEBAG_ALEX
    char a[ 10];
    itoa( id, a, 10);
    pt.setFont( QFont( "Times", BLOCK_Y/3, QFont::Normal));
    pt.drawText( 400, y+BLOCK_Y/2, a);
#endif
    return pt;
}

inline QPainter & drawLink_Up_Left( int x_out, int y_out, int x_in, int y_in, int d, QPainter &pt)
{
    pt.drawLine( x_out+BLOCK_X/2,y_out+BLOCK_Y			 ,x_out+BLOCK_X/2	,y_out+BLOCK_Y+BLOCK_D/3);
    pt.drawLine(x_out+BLOCK_X/2	,y_out+BLOCK_Y+BLOCK_D/3 ,x_out-d*LENTH_RIGHT ,y_out+BLOCK_Y+BLOCK_D/3);
    pt.drawLine(x_out-d*LENTH_RIGHT ,y_out+BLOCK_Y+BLOCK_D/3  ,x_in-d*LENTH_RIGHT, y_in-BLOCK_D/3);
    pt.drawLine(x_in-d*LENTH_RIGHT, y_in-BLOCK_D/3, x_in+BLOCK_X/2 , y_in-BLOCK_D/3);
    pt.drawLine(x_in+BLOCK_X/2 , y_in-BLOCK_D/3 ,  x_in+BLOCK_X/2,y_in);
    //рисование стрелочек к блоку
    pt.drawLine(x_in+BLOCK_X/2 , y_in-BLOCK_D/3 ,  x_in+BLOCK_X/2,y_in);
    pt.drawLine(x_in+BLOCK_X/2,y_in, x_in+BLOCK_X/2+BLOCK_D/5,y_in-BLOCK_D/5);
    pt.drawLine(x_in+BLOCK_X/2,y_in, x_in+BLOCK_X/2-BLOCK_D/5,y_in-BLOCK_D/5);
    return pt;
}

inline QPainter & drawLink_Left_Up_Left(int x_out,int y_out,int x_in,int y_in,int d,QPainter &pt)
{
    //влево
    pt.drawLine(x_out,y_out+BLOCK_Y/2,x_out-d*LENTH_RIGHT ,y_out+BLOCK_Y/2);
    //вверх
    pt.drawLine(x_out-d*LENTH_RIGHT ,y_out+BLOCK_Y/2 ,x_in-d*LENTH_RIGHT, y_in-BLOCK_D/3);
    //влево
    pt.drawLine(x_in+BLOCK_X/2,y_in-BLOCK_D/3,x_in-d*LENTH_RIGHT ,y_in-BLOCK_D/3);
    //вниз
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2,y_in-BLOCK_D/3);
    //рисование стрелочки.
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2-BLOCK_D/5 ,y_in-BLOCK_D/5);
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2+BLOCK_D/5 ,y_in-BLOCK_D/5);
    return pt;
}


//in - это блок после которым мы вклиниваемся
QPainter & drawLink_Down_right(int x_out,int y_out, int x_in,int y_in,int d,QPainter &pt)
{
    //стрелочку в право
    pt.drawLine(x_out+BLOCK_X,y_out+BLOCK_Y/2,x_out+BLOCK_X+d*LENTH_RIGHT,y_out+BLOCK_Y/2);
    pt.drawLine(x_out+BLOCK_X+d*LENTH_RIGHT,y_out+BLOCK_Y/2,x_in+BLOCK_X+d*LENTH_RIGHT,y_in-(BLOCK_D)/3);
    pt.drawLine(x_in+BLOCK_X/2,y_in-(BLOCK_D)/3,x_in+BLOCK_X+d*LENTH_RIGHT,y_in-BLOCK_D/3);
    //рисуем последний кончик со стрелкой.
    pt.drawLine(x_in+BLOCK_X/2,y_in-BLOCK_D/3,x_in+BLOCK_X/2,y_in);
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2+BLOCK_D/5,y_in-BLOCK_D/5);
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2-BLOCK_D/5,y_in-BLOCK_D/5);
    return pt;
}

//in - это блок после которым мы вклиниваемся
QPainter & drawLink_Down_left(int x_out,int y_out, int x_in,int y_in,int d,QPainter &pt)
{
    //стрелочка вниз
    pt.drawLine(x_out+BLOCK_X/2,y_out+BLOCK_Y,x_out+BLOCK_X/2,y_out+BLOCK_Y+BLOCK_D/3);
    //стрелочку в лево
    pt.drawLine(x_out+BLOCK_X/2,y_out+BLOCK_Y+BLOCK_D/3,x_out-d*LENTH_RIGHT,y_out+BLOCK_Y+BLOCK_D/3);
    //стрелочка в вниз
    pt.drawLine(x_out-d*LENTH_RIGHT,y_out+BLOCK_Y+BLOCK_D/3,x_in-d*LENTH_RIGHT,y_in-(BLOCK_D/3));
    //стрелочка в лево
    pt.drawLine(x_in+BLOCK_X/2,y_in-BLOCK_D/3,x_in-d*LENTH_RIGHT,y_in-BLOCK_D/3);
    //рисуем последний кончик со стрелкой.
    pt.drawLine(x_in+BLOCK_X/2,y_in-BLOCK_D/3,x_in+BLOCK_X/2,y_in);
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2+BLOCK_D/5,y_in-BLOCK_D/5);
    pt.drawLine(x_in+BLOCK_X/2,y_in,x_in+BLOCK_X/2-BLOCK_D/5,y_in-BLOCK_D/5);
    return pt;
}

sScope & getnextperent(sScope& a)
{
    //находим родителя
    QORMList<sScope> data(QORMList<sScope>::findItem(a.parent_id));
    for(QORMList<sScope>::iterator j=data.begin(); j!=data.end(); ++j)
    {
        if( a.id==(*j).id )
        {
            ++j;
            return *j;
        }
    }
    return a;
}

void wighttree(Tree &a)
{
    //cout << "wighttree: start" << endl;
    //получаем последние элементы
    a.d=1;
    a.data.d=1;
    std::vector<Tree*> vec;
    //cout << "wighttree: getlatest" << endl;
    getlatest(a,vec);
    int dd=1;
    //cout << "wighttree: for" << endl;
    for( int i = vec.size()-1 ; i != -1; i--)
    {
        vec[ i ]->d = vec[ i ]->data.d = dd;
        dd++;
        if( dd % 10==0 ) dd=1;
    }
    //cout << "wighttree: for2" << endl;
    int k=0;
    for( size_t i=0 ; i < vec.size() ; i++ )
    {
        Tree * te=vec[ i ];
        k=te->d;
        for( ; (*(te)).data.id != a.data.id ;)
        {
            if(	te->data.d < k )
            {
                te->data.d = k;
                te->d=k;
            }
            te=( te->parent );
            k++;
        }
    }
    //cout << "wighttree end" << endl;
}

void wighttree2( Tree &a)
{
    //cout << "wighttree2: test" << endl;
    //a.data.system_type;
    //a.a.size();

    //cout << "wighttree2: start" << endl;

    if( ( a.data.system_type==CODE_IF) && a.a.size()==2)
    {
        // cout << "wighttree2: findlasttoconnectDown" << endl;
        vector< Tree*> *v= &findlasttoconnectDOWN( *( a.a[ 0]) );
        //cout << "wighttree2: for" << endl;
        for( size_t iii=0; iii<v->size(); iii++)
        {
            // cout << "wighttree2: if" << endl;
            if( (*v)[ iii]->data.system_type==CODE_CODE  )
            {
                ( *v)[ iii]->d=a.d;
                ( *v)[ iii]->data.d=a.d;
            }
            // cout << "wighttree2: if end" << endl;
        }
    }

    //cout << "wighttree2: for2" << endl;
    for( int i=0; i < a.a.size(); i++)
    {
        if( a.a[ i ] != NULL)
            wighttree2(*(a.a[i]) );
    }
    //cout << "wighttree2: end" << endl;
}

void printTree( Tree &a, string  s)
{
    s += "===";
    for( size_t i=0; i < a.a.size(); i++)
    {
        string ss="";
        if( a.a[ i ]->data.system_type == CODE_BREAK )
        {
            ss.append("BREAK");
        }
        if( a.a[ i ]->data.system_type == CODE_CONTINUE )
        {
            ss.append("CONTINUE");
        }
        if( a.a[ i ]->data.system_type == CODE_CYCLE_DO )
        {
            ss.append("CYCLE_DO");
        }
        if( a.a[ i ]->data.system_type == CODE_CYCLE_FOR )
        {
            ss.append("CYCLE_FOR");
        }
        if( a.a[ i ]->data.system_type == CODE_CYCLE_WHILE )
        {
            ss.append("CYCLE_WHILE");
        }
        if( a.a[ i ]->data.system_type == CODE_CYCLE_DO_WHILE )
        {
            ss.append("CYCLE_DO_WHILE");
        }
        if( a.a[ i ]->data.system_type == CODE_GOTO )
        {
            ss.append("GOTO");
        }
        if( a.a[ i ]->data.system_type == CODE_IF )
        {
            ss.append("IF");
        }
        if( a.a[ i ]->data.system_type == CODE_CODE )
        {
            ss.append("CODE");
        }
        if( a.a[ i ]->data.system_type == CODE_BLOCK )
        {
            ss.append("BLOCK");
        }
        if( a.a[ i ]->data.system_type == CODE_TRUE )
        {
            ss.append("TRUE");
        }
        if( a.a[ i ]->data.system_type == CODE_FALSE )
        {
            ss.append("FALSE");
        }
        if( a.a[ i ]->data.system_type == CODE_SWITCH )
        {
            ss.append("SWITCH");
        }
        if( a.a[ i ]->data.system_type == CODE_SWITCH_VALUE )
        {
            ss.append("SWITCH_VALUE");
        }
        cout << s.c_str() << "[id=" << a.a[ i]->data.id << ",d=" << a.a[ i]->data.d << ",parent_id="
             << a.a[ i ]->parent->data.id << "type=" << ss<<"]" << endl;
        printTree( *( a.a[ i ]), s);
    }
}

Tree& fromBaseToTree( Tree &c, sScope& b)
{
    QORMList< sScope> data( b); // Вот он, контейнер
    int i = 0;
    int base_size=data.size();
    c.a.resize( base_size);
    int number_break=0;
    for( QORMList< sScope>::iterator j = data.begin(); j != data.end(); ++j, i++)
    {
        if( (*j).system_type == EXE_SCOPE )
        {
            number_break++;
            i--;
            continue;
        }
        if( (*j).system_type == INFO_SCOPE )
        {
            number_break++;
            i--;
            continue;
        }

        Tree* tr = new Tree;
        tr->data = (*j);
        tr->parent = &c;
        tr->data.d = 0;
        c.a[ i ] = tr;
        fromBaseToTree( *( c.a[ i]), ( c.a[ i])->data);
    }

    //c.a.resize( i+1 );
    //c.a.resize( base_size-number_break);
    //ссылка на последний элемент
    return c;
}



Tree & findlast( Tree &a)
{
    if( a.a.size() != 0 ) return findlast( *(a.a[ a.a.size()-1]) );
    return a;
}

Tree & findFather( Tree& a, Tree& b)
{
    Tree *tr = new Tree;
    *tr = a;
    if( a.data.id == b.data.id) return a;

    for( size_t i=0; i < (*tr).a.size(); i++)
    {
        if( tr->a[ i]->data.id == b.data.id) return *tr;
        return findFather( *(tr->a[ i]), b);
    }
    return a;
}


inline vector< Tree*>& findlasttoconnectDOWN( Tree &a)
{
    debug_message(5, "findlasttoconnectDOWN: start");
    vector<Tree*> * v = new vector< Tree*>;
    switch( a.data.system_type)
    {
        case CODE_CYCLE_FOR:
        {
            v->push_back( &a);
            return *v;
            break;
        }
        case CODE_CYCLE_WHILE:
        {
            v->push_back( &a);
            return *v;
            break;
        }
        case CODE_CYCLE_DO_WHILE:
        {
            v->push_back( &a);
            return *v;
            break;
        }
        case CODE_CYCLE_DO:
        {
            if( a.a.size() != 0)
            {
                if( a.a[ a.a.size()-1 ] != NULL )
                {
                    v->push_back( a.a[ a.a.size()-1 ] );
                }
                else
                {
                    if( a.a.size()-2  > 0)
                    {
                        v->push_back( a.a[ a.a.size()-2 ] );
                    }
                }
                return *v;
            }
            break;
        }
        case CODE_TRUE:
        {
            if( a.a.size() == 0 || (a.a.size() != 0 && a.a[0]!=NULL &&a.a[ 0]->data.system_type == CODE_BLOCK && a.a[ 0]->a.size() == 0 ) )
            {
                v->push_back( &a);
                return *v;
            }
            if( a.a.size() != 0   )
            {
                if( a.a[ a.a.size()-1 ] != NULL )
                {
                    vector< Tree *> *p= &findlasttoconnectDOWN( *( a.a[ a.a.size()-1 ] ) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
                else
                {
                    if( a.a.size()-2  > 0)
                    {
                        vector< Tree *> *p= &findlasttoconnectDOWN( *( a.a[ a.a.size()-2 ] ) );
                        v->insert( v->end(), p->begin(), p->end() );
                    }
                }
                return *v;
            }
            break;
        }
        case CODE_FALSE:
        {
            if( a.a.size() == 0 || ( a.a.size() != 0 && a.a[0] !=NULL && a.a[ 0 ]->data.system_type == CODE_BLOCK && a.a[ 0 ]->a.size() == 0 ) )
            {
                v->push_back( &a);
                return *v;
            }
            if( a.a.size() != 0   )
            {
                if( a.a[ a.a.size()-1 ] != NULL )
                {
                    vector< Tree *> *p=&findlasttoconnectDOWN( *( a.a[ a.a.size()-1 ] ) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
            }
            else
            {
                if( a.a.size()-2  > 0)
                {
                    vector< Tree *> *p= &findlasttoconnectDOWN( *( a.a[ a.a.size()-2 ] ) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
            }
            return *v;
            //}
            break;
        }
        case CODE_CODE:
        {
            if( a.a.size() == 0)
            {
                v->push_back( &a);
            }
            return *v;
            break;
        }
        case CODE_FUNC :
        {
            if( a.a.size() == 0)
            {
                v->push_back( &a);
            }
            return *v;
            break;
        }
        //если функция
        case EXE_SCOPE:
        {

            for( size_t i = 0; i < a.a.size(); i++)
            {
                if( a.a[ i ] == NULL) continue ;
                vector< Tree *> *p=&findlasttoconnectDOWN( *( a.a[ i]) );
                v->insert( v->end(), p->begin(), p->end() );
            }
            return *v;
            break;
        }
        case CODE_BLOCK:
        {
            if( a.a.size() != 0)
            {
                if(a.a[ a.a.size()-1] !=NULL)
                {
                    vector< Tree *> *p= &findlasttoconnectDOWN( *( a.a[ a.a.size()-1]) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
                return *v;
            }
            break;
        }
        case CODE_IF:
        {
            if( a.a.size() == 0)
            {
                v->push_back( &a);
            }
            if( a.a.size() == 1)
            {
                if( a.a[ 0 ] != NULL )
                {
                    vector< Tree *> *p = &findlasttoconnectDOWN( *( a.a[ 0]) );
                    v->insert( v->end(), p->begin(), p->end() );
                    v->push_back( &a);
                }
            }
            if( a.a.size() == 2)
            {
                if( a.a[ 1 ] != NULL  && a.a[ 0 ] != NULL )
                {
                    vector< Tree *> *p = &findlasttoconnectDOWN( *( a.a[ 0]) );
                    v->insert( v->end(), p->begin(), p->end() );
                    p=&findlasttoconnectDOWN( *( a.a[ 1]) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
            }
            if( a.a.size() > 2)
            {
                for(int ith=0; ith < a.a.size(); ith++)
                {
                    if( a.a[ ith ] != NULL )
                    {
                        vector< Tree *> *p = &findlasttoconnectDOWN( *( a.a[ ith ] ) );
                        //cout << "Number of findlasttoconnect DOWN in if and else if=" << p->size() << endl;
                        v->insert( v->end(), p->begin(), p->end() );
                    }
                }
            }
            return *v;
            break;
        }
        case CODE_SWITCH:
        {
            v->push_back( &a);
            if( a.a.size() != 0)
            {
                if( a.a[ a.a.size()-1] != NULL )
                {
                    vector< Tree *> *p = &findlasttoconnectDOWN( *( a.a[ a.a.size()-1]) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
            }
            return *v;
            break;
        }
        case CODE_SWITCH_VALUE:
        {
            if( a.a.size() == 0)
            {
                v->push_back( &a);
            }
            if( a.a.size() != 0)
            {
                if( a.a[ a.a.size()-1] != NULL )
                {
                    vector< Tree *> *p = &findlasttoconnectDOWN( *( a.a[ a.a.size()-1]) );
                    v->insert( v->end(), p->begin(), p->end() );
                }
            }
            return *v;
            break;
        }
        default:
        {}
    }
    return *v;
}

//поиск самого крайнего листа в дереве справа и внизу.
sScopeFull findlast( sScopeFull a)
{
    QORMList< sScopeFull> data(a); // Вот он, контейнер
    for( QORMList< sScopeFull>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if( (j.m_iPosition) == (data.size()-1) )
        {
            sScopeFull b;
            b=findlast( (*j) );
            return b;
        }
    }
    return a;
}

//поиск родителя для а от b.
sScopeFull findfather( sScopeFull &a, sScopeFull &b)
{
    QORMList< sScopeFull> data( b); // Вот он, контейнер
    for( QORMList< sScopeFull>::iterator j = data.begin(); j != data.end(); ++j)
    {
        if( (*j).id == a.id )
        {
            return b;
        }
        return findfather( a, *j);
    }
    return b;
}

//поиск первого листа из дерева
sScopeFull findfirst( sScopeFull a)
{
    QORMList< sScopeFull> data( a); // Вот он, контейнер
    for( QORMList< sScopeFull>::iterator j = data.begin(); j != data.end(); ++j)
    {
        return *j;
    }
    return a;
}


//возвращает последний
vector< Tree*> & getlatest( Tree & a, vector< Tree*> &b)
{
    //cout << "getlatest: start" << endl;
    Tree *tr=NULL;
    if( &a != NULL )
        tr = &a;
    else
        return b;

    if( tr->a.size() == 0 )
    {
        //cout << "getlatest: if end" << endl;
        b.push_back( tr);
        return b;
    }
    for( size_t i=0; i < tr->a.size(); i++)
    {
        // для каждого ребенка вызываем еще разок
        getlatest( *( tr->a[ i]), b ) ;
    }
    //cout << "getlatest: end" << endl;
    return b;
}


//найти все элементы не имеющие детей.
vector< sScopeFull> getlatest( sScopeFull a, vector< sScopeFull> &b)
{
    QORMList< sScopeFull> data( a); // Вот он, контейнер
    if( data.size() == 0)
    {
        b.push_back( a);
        return b;
    }
    for( QORMList< sScopeFull>::iterator j = data.begin(); j != data.end(); ++j)
    {
        getlatest( *j, b);
    }
    return b;
}

vector< Tree*>& treetoline( Tree &a, vector< Tree*> &v)
{
    if( a.data.system_type != CODE_BLOCK)
    {
        v.push_back( &a);
    }
    for( size_t i=0; i < a.a.size(); i++)
    {
        if(a.a[ i ] != NULL)
            treetoline( *( a.a[ i]), v);
    }
    return v;
}

Tree * findfirstNotBlock( Tree *a)
{
    debug_message(5, "AnalizeTree: findfirstNotBlock()");
    for( size_t i=0; i < a->a.size(); i++)
    {
        if(a->a[i] == NULL )
        {
            continue;
        }
        if( a->a[ i ]->data.system_type != CODE_BLOCK) return a->a[ i];
        Tree * no = findfirstNotBlock( a->a[ i ]);
        if( no->data.id != a->a[ i]->data.id) return no;
    }
    return a;
}


Tree* delsomblocksTree( Tree * t)
{
    //cout << "delsomblocksTree: start" << endl;
    for( int i=0; i < t->a.size(); i++)
    {
        if( t->a[ i ] == NULL)
        {
            t->a.erase( t->a.begin()+i);
            continue;
        }
        //cout << "delsomblocksTree: new delsombolocksTree" << endl;
        delsomblocksTree( t->a[ i ] );
        //cout << "delsomblocksTree: if" << endl;
        if( t->a[ i ]->data.system_type == CODE_BLOCK )
        {
            //cout << "delsomblocksTree: if2" << endl;
            if( ( *findfirstNotBlock( t->a[ i ] ) ).data.id == t->a[ i ]->data.id )
            {
                t->a.erase( t->a.begin()+i );
            }
        }

        /*
        if( t->a[ i ]->data.system_type == CODE_TRY )
        {
            t->a.erase( t->a.begin() + i );
        }

        if( t->a[ i ]->data.system_type == CODE_CATCH)
        {
            t->a.erase( t->a.begin() + i );
        }*/
    }
    //cout << "delsomblocksTree: end" << endl;
    return t;
}


/** получить мап файлов и функций в них */
QMap<QString, QList<QString> > * makeFuncList()
{
    QMap<QString, QList<QString> > *file2func=new QMap<QString, QList<QString> >();
    sqlAdapterForFiles adfiles;
    for( ;adfiles.next(); )
    {
        sqlAdapterForGetScopeByType adfunc;
        adfunc.GetFuncByFile( (int)adfiles.getId(), EXE_SCOPE);
        //(*file2func)[adfiles.get_FilePath()]=QList();
        // получить следующую функцию
        string ss1;
        for(;adfunc.next();)
        {
            // тут мне нужно имя функции.
            //adfunc.get_name();
            ss1="";
            ak_vs_db::get_scope_metadata ( adfunc.getId()  , "args" , &ss1);
            QString funcname=adfunc.get_name()+ss1.c_str();
            (*file2func)[QFileInfo(adfiles.get_FilePath()).completeBaseName()].push_back( funcname );
        }
    }
    return file2func;
}



QString makeTable( QList< FileBlockShem > * lst_FileBlock )
{

    /*
    QString table;
    table="<table>";
    for(QMap<QString, QList<QString> >::iterator it=f2f->begin() ; it!= f2f->end(); ++it)
    {
        QString func_a;
        for(QList<QString>::iterator jt=(it.value().begin()); jt !=(it.value().end()); jt++)
        {
            func_a+= QString(html_a( "" ,(*jt).toStdString().c_str()).c_str())+"</br>";
        }
        table+=QString(
                    html_tr(
                            html_td( it.key().toStdString().c_str() ) +
                            html_td( func_a.toStdString().c_str() )
                            ).c_str()
                       );
    }
    table+="</table>";
    */

    QMap<QString, QList<FileBlockShem> > mp_path_func;
    for(QList< FileBlockShem >::iterator it=lst_FileBlock->begin() ; it!=lst_FileBlock->end(); ++it)
    {
            mp_path_func[(*it).getfile_for_func()].push_back( *it );
    }

    /*
                getfile_svg(){return file_svg_;}
        int     getfunc_id(){return func_id_;}
        QString getfunc_name(){return func_name_;}
        QString getfile_for_func(){return file_for_func_;}
    */

    QString table;
    table="<table>";
    for(QMap<QString, QList<FileBlockShem> >::iterator it=mp_path_func.begin() ; it!= mp_path_func.end(); ++it)
    {
        QString func_a;
        for(QList<FileBlockShem>::iterator jt=(it.value().begin()); jt !=(it.value().end()); jt++)
        {
            func_a+= QString( html_a( QFileInfo( ( *jt).getfile_svg()).fileName().toStdString().c_str() ,(*jt).getfunc_name().toStdString().c_str()).c_str())+"</br>";
        }
        table+=QString(
                     html_tr(
                                html_td( it.key().toStdString().c_str() ) +
                                html_td( func_a.toStdString().c_str() )
                            ).c_str()
                       );
    }
    table+="</table>";

    return table;
}



void test(QList<FileBlockShem> *p)
{
    QFile file("test.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    out << makeTable(p) << "\n";
    file.close();
    return;
}




