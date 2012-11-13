
#include "libservice.h"
#include <stdio.h>
#include "common_libservice.h"

using namespace std;
using namespace alex;




int build_func_objects_list ( string& strOutputDir)
{
    return 1;
}



int NumFunctionInDb()
{
    QORMList< sScope> allscope;
    QORMList< sScope>::iterator it= allscope.begin();
    return allscope.size();
}


void silki_na_sleduiuchiy( fstream * p_html_file, vector< string>  fnames, int num_html_file)
{
    //вставляем ссылку в каждый htlm файл на предыдущий файл и на последующий
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i] << "<TABLE class=\"zagolovok\" WIGHT=\"100%\" ALIGN=\"center\" border=1>\n";
        //если файл отчета первый
        if( i==0 && ( i!= (num_html_file-1) ) )
        {
            p_html_file[ i] << html_tr(
                                html_th(QObject::tr("Eto pervii fail v otchete ").toStdString().c_str()) + // Это первый файл в отчете
                                html_th(
                                    html_a(fnames[i+1],QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // Следующий файл отчета
                                )
                            );
            p_html_file[ i] << "</TABLE>" << endl;
        }
        //если файл отчета последний
        if( i == num_html_file-1 && i != 0 )
        {
            p_html_file[ i] << html_tr(
                                html_th(
                                    html_a(fnames[i-1],QObject::tr("Predidyschii fail v otchete").toStdString().c_str()) // Предыдущий файл в отчете
                                )+
                                html_th(QObject::tr("Eto poslednii fail otcheta").toStdString().c_str()) // Это последний файл отчета
                            );
            p_html_file[ i] << "</TABLE>" << endl;
        }
        //если файл отчета первый и последний
        if( num_html_file-1 == 0 )
        {
            p_html_file[ i] << "</TABLE>\n";
        }
        //если файл отчета в середине
        if( i != 0 && i != ( num_html_file-1) )
        {
            p_html_file[ i] << html_tr(
                                html_th(
                                    html_a(fnames[i-1].c_str(),QObject::tr("Predidyschii fail otcheta").toStdString().c_str()) // Предыдущий файл отчета
                                )+
                                html_th(
                                    html_a( fnames[i+1].c_str(),QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // Следующий файл отчета
                                )
                            );
            p_html_file[ i] << "</TABLE>" << endl;
        }
    }
}

void silki_na_sleduiuchiyg( GroupReportFile& gfile)
{
    //вставляем ссылку в каждый htlm файл на предыдущий файл и на последующий
    gfile.WriteToAllFile( "<TABLE class=\"zagolovok\" WIGHT=\"100%\" ALIGN=\"center\" border=1>\n" );
    for( int i=0; i< gfile.Size(); i++)
    {

        //если файл отчета первый
        if( i==0 && ( i!= (gfile.Size()-1) ) )
        {
            gfile.GetFile( i) << html_tr(
                                  html_th(QObject::tr("Eto pervii fail v otchete ").toStdString().c_str()) + // Это первый файл в отчете
                                  html_th(
                                      html_a( gfile.GetFileName( i+1).toStdString(),QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // Следующий файл отчета
                                  )
                              ).c_str();
            gfile.GetFile( i) << "</TABLE>" << endl;
        }

        //если файл отчета последний
        if( i == gfile.Size()-1 && i != 0 )
        {
            gfile.GetFile( i) << html_tr(
                                  html_th(
                                      html_a( gfile.GetFileName(i-1).toStdString(),QObject::tr("Predidyschii fail v otchete").toStdString().c_str()) // Предыдущий файл в отчете
                                  )+
                                  html_th(QObject::tr("Eto poslednii fail otcheta").toStdString().c_str()) // Это последний файл отчета
                              ).c_str();
            gfile.GetFile( i) << "</TABLE>" << endl;
        }
        //если файл отчета первый и последний
        if( gfile.Size()-1 == 0 )
        {
            gfile.GetFile( i) << "</TABLE>\n";
        }
        //если файл отчета в середине
        if( i != 0 && i != ( gfile.Size()-1) )
        {
            gfile.GetFile( i) << html_tr(
                                  html_th(
                                      html_a( gfile.GetFileName( i-1).toStdString(),QObject::tr("Predidyschii fail otcheta").toStdString().c_str()) // Предыдущий файл отчета
                                  )+
                                  html_th(
                                      html_a( gfile.GetFileName( i+1).toStdString(),QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // Следующий файл отчета
                                  )
                              ).c_str();
            gfile.GetFile( i) << "</TABLE>" << endl;
        }
    }
}


//обход дерева и выявления информационных объектов
void get_from_Tree_Functional_object(sScopeFull cur, std::vector<NameScope > &vs_exe)
{
    QORMList<sScopeFull> data (cur); // Вот он, контейнер
    //перечисляем всех детей
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //копируем все функции в выходной буфер
        if( (*j).system_type == EXE_SCOPE )
        {
            NameScope ns;
            ns.name=cur.name+"."+(*j).name;
            ns.file=(*j).file_path;
            ns.s.id=(*j).id;
            vs_exe.push_back(ns);
        }
        //входим во все name_space и информационные элементы
        {
            std::vector<NameScope > va;
            get_from_Tree_Functional_object(*j,va);
            for(size_t i=0; i< va.size(); i++)
            {
                va[i].name=cur.name +"."+va[i].name;
            }
            vs_exe.insert(vs_exe.end(),va.begin(),va.end());
        }
    }
}

//обход дерева и выявления информационных объектов
void get_from_Tree_flowing_Functional_object(sScopeFull cur, std::vector<NameScope > &vs_exe)
{
    QORMList<sScopeFull> data (cur); // Вот он, контейнер
    //перечисляем всех детей
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //копируем все функции в выходной буфер
        if( (*j).system_type == EXE_SCOPE)
        {
            if((*j).isFlowing())
            {
                NameScope ns;
                ns.name=cur.name+"."+(*j).name;
                ns.file=(*j).file_path;
                ns.s.id=(*j).id;
                vs_exe.push_back(ns);
            }
        }
        //входим во все name_space и информационные элементы
        {
            std::vector<NameScope > va;
            get_from_Tree_Functional_object(*j,va);
            for(size_t i=0; i< va.size(); i++)
            {
                va[i].name=cur.name +"."+va[i].name;
            }
            vs_exe.insert(vs_exe.end(),va.begin(),va.end());
        }
    }
}

void get_from_Tree_Informational_object(sScopeFull cur, std::vector<NameScope> &vs_exe)
{
    QORMList<sScopeFull> data (cur); // Вот он, контейнер
    //перечисляем всех детей
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //копируем все функции в выходной буфер
        if( (*j).system_type == INFO_SCOPE)
        {
            NameScope ns;
            ns.name=cur.name+"."+(*j).name;
            ns.file=(*j).file_path;
            ns.s.id=(*j).id;
            vs_exe.push_back(ns);

        }
        //входим во все name_space и информационные элементы
        {
            std::vector<NameScope > va;
            get_from_Tree_Informational_object(*j,va);
            for(size_t i=0; i< va.size(); i++)
            {
                va[i].name=cur.name +"."+va[i].name;
            }
            vs_exe.insert(vs_exe.end(),va.begin(),va.end());
        }
    }
}


void get_from_Base_Func_obj(sScopeFull cur, std::vector<sScopeFull> &vs_exe)
{
    QORMList<sScopeFull> data (cur); // Вот он, контейнер
    //перечисляем всех детей
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //копируем все функции в выходной буфер
        if( (*j).system_type ==EXE_SCOPE )
        {
            vs_exe.push_back(*j);
        }
        std::vector<sScopeFull > va;
        get_from_Base_Func_obj(*j,va);
        vs_exe.insert(vs_exe.end(),va.begin(),va.end());
    }
}

void get_from_Base_Flowing_Func_obj(sScopeFull cur, std::vector<sScopeFull> &vs_exe)
{
    QORMList<sScopeFull> data (cur); // Вот он, контейнер
    //перечисляем всех детей
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //копируем все функции в выходной буфер
        if( (*j).system_type ==EXE_SCOPE)
        {
            vs_exe.push_back(*j);
        }
        std::vector<sScopeFull > va;
        get_from_Base_Func_obj(*j,va);
        vs_exe.insert(vs_exe.end(),va.begin(),va.end());
    }
}


//распрямим дерево
// а - элемент от которого начинаем распрямлять . v - куда записываем все элементы
int treetoline(sScope a,std::vector<sScope> &v)
{
    v.push_back(a);
    QORMList<sScope> data(a); // Вот он, контейнер
    for(QORMList<sScope>::iterator j = data.begin(); j != data.end(); ++j)
    {
        treetoline( *j,v);
    }
    return 0;
}

std::vector<sScope>& wighttree(std::vector<sScope> &a,std::vector<sScope> &b,sScope c)
{
    QORMList<sScope> data(c); // Вот он, контейнер
    QORMList<sScope>::iterator j=data.begin();
    std::vector<sScope> vec; // массив для временного хранения d
    //расставляем в vec вес.
    for(size_t i=0; i<a.size(); i++)
    {
        sScope te=a[i];
        int k=1;
        while(te.id!=(*j).id)
        {
            te.d=k;
            vec.push_back(te);
            te=te.getParent();
            k++;
        }
    }
    //в нашем векторе есть повторяющиеся элементы
    //пускай у повторяющихся с меньшим весом вес привратиться в 0
    for(size_t i=0; i<vec.size(); i++)
    {
        for(size_t j=i+1; j<vec.size(); j++)
        {
            if((vec[i].id==vec[j].id))
            {
                if(vec[i].d>=vec[j].d)
                {
                    vec[j].d=0;
                }
            }
        }
    }
    //вставим в vec2 все элементы из vec с не нулевым весом
    std::vector<sScope> vec2;
    for(size_t i=0; i<vec.size(); i++)
    {
        if(vec[i].d!=0)
        {
            vec2.push_back(vec[i]);
        }
    }

    //копируем в b поле d
    for(size_t i=0; i<b.size(); i++)
    {
        for(size_t j=0; j<vec2.size(); ++j)
        {
            if(b[i].id==vec2[j].id)
            {
                b[i].d=vec2[j].d;
            }
        }
    }
    return b;
}



void  dbg( const char *msg, ... )
{
    va_list ap;
    va_start( ap, msg );
    printf( msg, ap );
    va_end( ap );
}
