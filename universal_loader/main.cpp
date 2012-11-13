
#include <libdb_interface.h>
#include <data_orm.h>
#include <orm_analyzer.h>
#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <list>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <metrix.h>
#include <QTextCodec>
#include "args_proc.h"
#include <sqladapter.h>
#include <libservice.h>
#include <QList>
#include <sqladapter.h>
#include <QTranslator>

using namespace ak_vs_db;
using namespace std;

static std::list< std::string > lsProcessedFiles;

void addToReport( sReportInfo* data, eReportType type ); //< The fucntion generates index report
void signatureAnalysis();
void drawGraph();
void dynamicReport();


// проверяет нужно ли делать отчет
bool checkisNeed_Report_exit()
{
        if( (alex::CountScopeInDB() == 0) and  (alex::CountValnurabilaty() == 0)  and !get_scope_metadata (0,"chxSignatures") )
        {
                if( !ak_vs_db::get_scope_metadata(0, "quiet_mode") )
                {
                        QMessageBox msgBox;
                        msgBox.setText( QObject::tr("Otcheti ne bydyt potroeni t.k. BD pysta.") );
                        msgBox.exec();
                }
                exit( -1);
                return false;
        }
        else
                return true;
}

//export std::fstream alex_debug.out;


int main( int argc, char *argv[])
{
    time_t start,end;

    setlocale( LC_CTYPE, "rus.1251");

    QApplication a( argc, argv);

    QTextCodec::setCodecForTr( QTextCodec::codecForName( "Windows-1251"));
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "Windows-1251"));
    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "Windows-1251"));

    // установка языка
    QString setlang="ru";
    for( int i=0; i < argc ; i++ )
    {
        if( QString(argv[ i ]) == "--ru")
        {
            setlang="ru";
            argc--;
        }
        if( QString(argv[ i ]) == "--en")
        {
            argc--;
            setlang="en";
        }
    }

    InitLibDb_Interface( setlang.toStdString() );
    initLibservice( setlang.toStdString() );


    if( qApp != NULL )
    {
        QTranslator * qtTranslator= new QTranslator;
        if( false == qtTranslator->load(QString("translation/universal_loader_")+setlang) )
            QMessageBox::critical(NULL, QObject::tr("Oschibka"), QObject::tr("Fail s perevodom dlia universal_loader ne podgrysilsia") );
        qApp->installTranslator( qtTranslator );
    }
    else
    {
        int argc=0;
        char ** argv;
        QApplication *a=new QApplication(argc,argv);
        QTranslator * qtTranslator= new QTranslator;
        if( false == qtTranslator->load(QString("translation/universal_loader_")+setlang) )
            QMessageBox::critical(NULL, QObject::tr("Oschibka"), QObject::tr("Fail s perevodom dlia universal_loader ne podgryzilsia") );
        qApp->installTranslator(qtTranslator);
    }

    QList< std::string > qlist;

    QString input_file, output_file;

    cout << QObject::tr("Kolichestvo argymentov:").toStdString().c_str() << argc << endl;



    if( argc < 2 )
    {
            cout << QObject::tr("Nedostatochno argumentov dlia zapyska programmi").toStdString().c_str() << endl; exit(-1);
    }

    if( ak_vs_db::db_connect( 1 ) == S_FAILED)
    {
            cout << QObject::tr("Ne ydalos soedinitsia s basoi dannih").toStdString().c_str() << endl;
            exit( -1);
    }

    if( 0 )
    {   // debug
            cout << "get_scope_metadata (0, \"chxBrunches\" )=" << get_scope_metadata (0, "chxBrunches" )<< endl;
            cout << "get_scope_metadata (0, \"chxBuildTXTMatrix\")=" << get_scope_metadata (0, "chxBuildTXTMatrix") << endl;
            cout << "get_scope_metadata (0, \"chxLists\" )=" << get_scope_metadata (0, "chxLists" ) << endl;
            cout << "get_scope_metadata (0, \"chxMatrixes\" )=" << get_scope_metadata (0, "chxMatrixes" ) << endl;
            cout << "get_scope_metadata (0, \"chxTraces\" )=" << get_scope_metadata (0, "chxTraces" ) << endl;
            cout << "get_scope_metadata (0, \"chxSchemes\" )=" << get_scope_metadata (0, "chxSchemes" ) << endl;
            cout << "get_scope_metadata (0, \"chxSignatures\")=" << get_scope_metadata (0, "chxSignatures") << endl;
            cout << "get_scope_metadata (0,\"chxGraphs\")=" << get_scope_metadata (0,"chxGraphs") << endl;
    }

    data.strProjectName = get_project_name();
    basisOutputDirectory = string( argv[ 1 ] ) ;//+ string("\\") + data.strProjectName;
    qOutputDir.setPath( basisOutputDirectory.c_str() );
    data.lsProcessedFiles = &lsProcessedFiles;
    data.strOutputDirectory = basisOutputDirectory;


    if( argc == 3 )     // сигнатурный анализатор
    {
            time( &start );
            if( get_scope_metadata ( 0, "NotCleanDB") )
                signatureAnalysis();
            time( &end);
            ak_vs_db::db_dconnect("universal_loader");
            return 0;
    }

    if( argc == 4)     // нарисовать  граф
    {
            checkisNeed_Report_exit();
            time(&start);
            drawGraph();
            time(&end);
            ak_vs_db::db_dconnect("universal_loader2");
            return 0;
    }

    cout << "Выходная папка для отчетов: " << argv[1] << endl;
    std::string str;

    if( argc == 5)
    {
            str=data.strOutputDirectory + "\\dynindex.html";
    }
    else
    {
            str=data.strOutputDirectory + "\\index.html";
    }


    data.fIndexReport.open(str.c_str());
    data.fIndexReport << "<html>";
    data.fIndexReport << getCSS();
    data.fIndexReport << "<body><h1>"<<QObject::tr("Perechen otchetov AK-VS.").toStdString().c_str()<<"</h1>";
    data.iReportsCount = 0;

    if( argc == 5 )
    {
            cout << QObject::tr("Postroenie otchetov po dinamike").toStdString().c_str() << endl;
            checkisNeed_Report_exit();
            time(&start);
            dynamicReport();
            time(&end);
            ak_vs_db::db_dconnect("universal_loader3");
            return 0;
    }


    //Проверка того, стоим мы ветви или нет
    if( get_scope_metadata (0, "chxBranches" ))
    {
        data.hasBrunches = true;
    }
    else
    {
        data.hasBrunches = false;
    }

    if(get_scope_metadata (0, "chxBuildTXTMatrix"))
            data.do_txt_matrix = true;
    else
            data.do_txt_matrix = false;
    ////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////
    if(get_scope_metadata (0, "chxLists" ))
    {
            checkisNeed_Report_exit();
            try
            {
                    time( &start);
                    cout << QObject::tr("Postroenie spiska FO.").toStdString().c_str() << endl;

                    data.strBaseFileName="pril2.5.";
                    data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 2.5";
                    qOutputDir.mkdir(data.strOutputDirectory.c_str() );

                    if(FilePrint_func_object(&data)) addToReport(&data,rptListsExe);
                    data.lsProcessedFiles->clear();
                    cout << QObject::tr("(log):Postroenie spiska IO.").toStdString().c_str() << endl;
                    data.strBaseFileName="pril6.1.";
                    data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 6";
                    qOutputDir.mkdir(data.strOutputDirectory.c_str() );

                    if(FilePrint_info_object(&data)) addToReport(&data,rptListsInfo);
                    data.lsProcessedFiles->clear();
            }
            catch(...)
            {
                if( !ak_vs_db::get_scope_metadata(0, "quiet_mode") )
                {
                    QMessageBox mbox;
                    mbox.setText( QObject::tr("Proizoschla oschibka pri postroenii spiska FO i IO.") );
                    mbox.exec();
                }
            }
    }

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
    if(get_scope_metadata (0, "chxMatrixes" ))
    {
            checkisNeed_Report_exit();
            try
            {
                cout << QObject::tr("(log):Postroenie matric po ypravleniy.").toStdString().c_str() << endl;
                data.do_html_matrix=1;
                data.strBaseFileName="pril4.1.";
                data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 4";
                qOutputDir.mkdir(data.strOutputDirectory.c_str() );

                if(Matrix_cont(&data)) addToReport(&data,rptMatrixesExe);
                data.lsProcessedFiles->clear();



                cout << QObject::tr("(log):Postroenie matric po informacii.").toStdString().c_str() << endl;
                data.strBaseFileName="pril5.1.";
                data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 5";
                qOutputDir.mkdir(data.strOutputDirectory.c_str() );

                if(Matrix_info(&data)) addToReport(&data,rptMatrixesInfo);
                data.lsProcessedFiles->clear();
            }
            catch(...)
            {
                if( !ak_vs_db::get_scope_metadata(0, "quiet_mode") )
                {
                    QMessageBox mbox;
                    mbox.setText( QObject::tr("Proisoschla oschibka pri postroenii matric") );
                    mbox.exec();
                }
            }
    }

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
    if(get_scope_metadata (0, "chxTraces" ))
    {
            checkisNeed_Report_exit();
            try
            {
                    cout << QObject::tr("(log):Postroenie trass vizovov.").toStdString().c_str() << endl;
                    data.strBaseFileName="pril13.1.";
                    data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 13";
                    qOutputDir.mkdir(data.strOutputDirectory.c_str() );

                    if(Trace_Report(&data)) addToReport(&data,rptTraces);
                    data.lsProcessedFiles->clear();
            }
            catch(...)
            {
                    if( !ak_vs_db::get_scope_metadata(0, "quiet_mode"))
                    {
                        QMessageBox mbox;
                        mbox.setText( QObject::tr("Proizoschla oschibka pri postroenii otchetov trass vizovov.") );
                        mbox.exec();
                    }
            }
    }

    if(get_scope_metadata (0, "chxSchemes" ))
    {

                checkisNeed_Report_exit();
                try
                {
                    cout << QObject::tr("(log):Postroenie block-shem.").toStdString().c_str() << endl;

                            data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 12";
                            qOutputDir.mkdir( data.strOutputDirectory.c_str() );
                            int x=0, y=0; //counters
                            QORMList <sFile> lsFiles; //( rptSchemes);
                            // дебаг
                            //cout << "For block-shem report have: " << lsFiles.size()<< " files" << endl;

                            for( QORMList<sFile>::iterator i = lsFiles.begin(); i != lsFiles.end(); i++)
                            {
                                        QORMList <sScope> lsFuncs( (*i).id, EXE_SCOPE);
                                        //cout << "Number of func for block shem: " << lsFuncs.size() << " func" << endl;

                                        data.iscStartFile = (*i).id;
                                        data.lsSourceFiles.push_back( (*i).getFileName() );

                                        for( QORMList<sScope>::iterator j = lsFuncs.begin(); j != lsFuncs.end(); j++)
                                        {
                                                    //std::stringstream strtmp;
                                                    QString strtmp= "pril12." + QString::number( x) + "." + QString::number( y);
                                                    //strtmp << "pril12."<< x << "." << y;
                                                    data.strBaseFileName = strtmp.toStdString();  //strtmp.str();
                                                    //data.strBaseFileName += itoa( (*j).id );
                                                    data.iscStartBlock = (*j).id;
                                                    if( !Trace_Block_Shem( &data))
                                                    {
                                                        cout << QObject::tr("Oschibka pri postroenii block-shem").toStdString().c_str();
                                                        continue;
                                                    }
                                                    y++;
                                        }
                                        x++;
                            }

                            addToReport(&data,rptSchemes);
                            data.lsProcessedFiles->clear();
                            data.lsSourceFiles.clear();
                }
                catch(...)
                {
                    if( !ak_vs_db::get_scope_metadata(0, "quiet_mode"))
                    {
                            QMessageBox mbox;
                            mbox.setText( QObject::tr("Proizoschla oschibka pri postroenii block-shem.") );
                            mbox.exec();
                    }
                }
    }

    if( get_scope_metadata (0,"chxSignatures"))
    {
            try
            {
            cout << QObject::tr("(log):Obrabotka signatyrnogo analiza.").toStdString().c_str() << endl;
                    data.strBaseFileName = "pril10.";
                    data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 10";
                    qOutputDir.mkdir(data.strOutputDirectory.c_str() );
                    if(SignatureReport(&data))
                        addToReport(&data,rptSignatures);
                    data.lsProcessedFiles->clear();
            }
            catch(...)
            {
                if( !ak_vs_db::get_scope_metadata(0, "quiet_mode"))
                {
                    QMessageBox mbox;
                    mbox.setText( QObject::tr("Proizoschla oschibka pri postroenii otchetov signatyrnogo analiza.") );
                    mbox.exec();
                }
            }
    }

    if(get_scope_metadata (0,"chxGraphs"))
    {
        checkisNeed_Report_exit();
        try
        {
            cout << QObject::tr("(log):Postroenie grafa.").toStdString().c_str() << endl;
                data.strBaseFileName = "pril9.1.";
                data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 9";
                qOutputDir.mkdir(data.strOutputDirectory.c_str() );
                HRET gret = GraphReport(&data);
                if (gret == S_OKEY)
                {
                    input_file = QString(data.strOutputDirectory.c_str()) + "\\" + QString(data.strBaseFileName.c_str()) + "dge";
                    output_file = QString(data.strOutputDirectory.c_str()) + "\\" + QString(data.strBaseFileName.c_str()) + "svg";
                    QProcess psDot;
                    QProcess psNull;

                    psDot.setStandardOutputProcess(&psNull);
                    psDot.setStandardErrorFile (QString("dot.log"));

                    QString exe_program = "./modules/dot/dot.exe";
                    QStringList dot_arguments;
                    dot_arguments << "-Tsvg" << input_file << "-o" << output_file;
                    if( QFile::exists( input_file ) )
                    {
                            psDot.setStandardOutputFile("dot.output");
                            psDot.execute(exe_program,dot_arguments);
                            data.lsProcessedFiles->clear();
                            lsProcessedFiles.push_back(output_file.toLocal8Bit().constData());
                            addToReport(&data,rptGraphs);
                            /*
                            if( psDot.waitForStarted(2000)  )
                            {
                                psDot.waitForFinished(3600000);
                                data.lsProcessedFiles->clear();
                                lsProcessedFiles.push_back(output_file.toLocal8Bit().constData());
                                addToReport(&data,rptGraphs);
                            }
                            else cout << "Ошибка при построении графа" <<endl;
                            */
                    }
                    else
                        cout << QObject::tr("Oschibka: fail ne syschestvyiet: ").toStdString().c_str() << input_file.toLocal8Bit().data() << endl;
                }
                else
                        cout << QObject::tr("Oschibka pri formirovanii grafa").toStdString().c_str() << endl;
        }
        catch(...)
        {
            if( !ak_vs_db::get_scope_metadata(0, "quiet_mode"))
            {
                    QMessageBox mbox;
                    mbox.setText( QObject::tr("Proizoschla oschibka pri postroenii otcheta grafa.") );
                    mbox.exec();
            }

        }
    }

    if( get_scope_metadata (0,"chxMetriki") )
    {
                checkisNeed_Report_exit();
                cout << QObject::tr("(log):Postroenie metrik.").toStdString().c_str() << endl;
                // сделать отчет по метрике
                data.strBaseFileName="report";
                data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 16";
                qOutputDir.mkdir( data.strOutputDirectory.c_str() );
                data.lsProcessedFiles->clear();
                MetrixReport( &data );
                addToReport( &data, rptMetrix);
                data.lsProcessedFiles->clear();
    }

    data.fIndexReport << QObject::tr("<p><b>Ishodnie teksti modylia obrabotki signalov datchikov (v dinamicheskom analize) predstavleni v failah  <b>__probe_cxx.cpp</b> i <b>__probe_cxx.h</b></p>").toStdString().c_str();
    data.fIndexReport << "</body></html>";
    data.fIndexReport.close();
    cout << QObject::tr("(log):Generacia otchetov zaverschena.").toStdString().c_str()<<endl;
    ak_vs_db::db_dconnect("universal_loader4");
    return 0;
}


void addToReport(sReportInfo* data, eReportType type)
{
	std::string str;
	data->iReportsCount += data->lsProcessedFiles->size();
    QList<std::string> RepType;

    RepType=QList<std::string>::fromStdList( ak_vs_db::aReportTypes );


	set_scope_metadata(0,"reports_count",data->iReportsCount);
	data->fIndexReport <<
    "<div><h2>" << RepType.value(
	                            type
	                            //(int)(log(type)/log(2))
                                )
	                            << "</h2>"
	<< "<ul>";

	foreach(str,(*data->lsProcessedFiles) )
	{
		data->fIndexReport << "<li><a href='./"
		<< str.erase(0,basisOutputDirectory.size()+1 ) << "'>"<<str
		<<"</a></li>";
	}
	data->fIndexReport << "</ul></div>";
}

void signatureAnalysis()
{
    cout << QObject::tr("Signaturnii analiz zapyschen...").toStdString().c_str() << endl;

    if(!ak_vs_db::db_connect(1))
    {
        cout << QObject::tr("Ne ydalos soedinitsia s basoi dannih").toStdString().c_str()<< endl;
        exit(-1);
    }
	QORMList <sFile> lsFiles;

	for(QORMList<sFile>::iterator i = lsFiles.begin(); i != lsFiles.end(); i++)
	{
		cout << (*i).file_path << endl;
		CFileAnalyzer* sFA = CAnalyzerFactory::getAnalyzer(*i);
		sFA->analyze();
		delete sFA;
	}

        ak_vs_db::db_dconnect("universal_loader5");
        cout << QObject::tr("Signaturnii analiz yspeschno zakonchen").toStdString().c_str() << endl;
}


void drawGraph()
{
    if(!ak_vs_db::db_connect(1))
    {
        cout << QObject::tr("Ne ydalos' soedinitsia s basoi dannih").toStdString().c_str() << endl;
        exit(-1);
    }
	data.strProjectName = get_project_name();
    cout << QObject::tr("Postroenie grafa.").toStdString().c_str() << endl;
	data.strBaseFileName = "pril9.1.";
        data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 9";
	qOutputDir.mkdir(data.strOutputDirectory.c_str() );
        if(GraphReport(&data))
	{
                QString input_file = QString(data.strOutputDirectory.c_str() )+"\\"+data.strBaseFileName.c_str()+ "dge";
                QString output_file = QString(data.strOutputDirectory.c_str() )+"\\"+data.strBaseFileName.c_str()+ "svg";
                QProcess psDot;
                QProcess psNull;

                psDot.setStandardOutputProcess(&psNull);
                psDot.setStandardErrorFile (QString("dot.log"));

                QString exe_program = "./modules/dot/dot.exe";

                QStringList dot_arguments;
                dot_arguments << "-Tsvg" << input_file << "-o" << output_file;
                        //qDebug() << "input file: " << input_file;
                        //qDebug() << "output file: " << output_file;
                //qDebug() << dot_arguments;
                //qDebug() << exe_program << endl;
                //qDebug() << dot_arguments<< endl;
                if(QFile::exists( input_file ) )
                {
                            //psDot.start(exe_program,dot_arguments);
                            psDot.setStandardOutputFile("dot.output");
                            psDot.execute(exe_program,dot_arguments);
                            data.lsProcessedFiles->clear();
                            lsProcessedFiles.push_back(output_file.toLocal8Bit().constData());
                            addToReport(&data,rptGraphs);
                                        /*
                                        if( psDot.waitForStarted(2000)  )
                                        {
                                                psDot.waitForFinished(10000);
                                                //outstr+=" -Tsvg "+fnames+" -o "+base_file_name+"svg";

                                                                //cout<<outstr;
                                                //system(outstr.c_str());

                                                data.lsProcessedFiles->clear();
                                                lsProcessedFiles.push_back(output_file.toLocal8Bit().constData());
                                                addToReport(&data,rptGraphs);
                                        }
                                        else cout << "Ошибка при построении графа (svg)" <<endl;
                                        */
                }
                else
                    cout << QObject::tr("Oschibka: fail ").toStdString().c_str() << input_file.toStdString() << QObject::tr(" ne syschestvyet").toStdString().c_str();
	}
        else cout << QObject::tr("Oschibka pri postroenii grafa (tekst)").toStdString().c_str() <<endl;
        ak_vs_db::db_dconnect("universal_loader7");
}


void dynamicReport()
{
        //if(!ak_vs_db::db_connect(1)) {cout << "Не удалось соединиться с базой данных"<< endl; exit(-1);}
        data.strProjectName = get_project_name();
        cout << QObject::tr("Direktoria dlia sohranenia otcheta dinamicheskogo analiza: ").toStdString().c_str() << data.strOutputDirectory << endl;
        cout << QObject::tr("Obrabotka rezyltatov dinamicheskogo analiza").toStdString().c_str() << endl;

        data.strBaseFileName="pril8.1.";
        data.strOutputDirectory = basisOutputDirectory+"\\"+strAppendixName+" 8";

        qOutputDir.mkdir(data.strOutputDirectory.c_str() );

        if( DinamicReport( &data ) == S_OKEY ) {
                addToReport(&data,rptProbes);
                if( !ak_vs_db::get_scope_metadata(0, "quiet_mode") )
                {
                        QMessageBox mb;
                        mb.setText(QObject::tr("Otcheti po dinamike sozdani yspeschno.").toStdString().c_str());
                        mb.exec();
                }
        }

        data.lsProcessedFiles->clear();
        //ak_vs_db::db_dconnect();
}
