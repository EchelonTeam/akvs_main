
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
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
    for( int i=0; i < num_html_file; i++)
    {
        p_html_file[ i] << "<TABLE class=\"zagolovok\" WIGHT=\"100%\" ALIGN=\"center\" border=1>\n";
        //���� ���� ������ ������
        if( i==0 && ( i!= (num_html_file-1) ) )
        {
            p_html_file[ i] << html_tr(
                                html_th(QObject::tr("Eto pervii fail v otchete ").toStdString().c_str()) + // ��� ������ ���� � ������
                                html_th(
                                    html_a(fnames[i+1],QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // ��������� ���� ������
                                )
                            );
            p_html_file[ i] << "</TABLE>" << endl;
        }
        //���� ���� ������ ���������
        if( i == num_html_file-1 && i != 0 )
        {
            p_html_file[ i] << html_tr(
                                html_th(
                                    html_a(fnames[i-1],QObject::tr("Predidyschii fail v otchete").toStdString().c_str()) // ���������� ���� � ������
                                )+
                                html_th(QObject::tr("Eto poslednii fail otcheta").toStdString().c_str()) // ��� ��������� ���� ������
                            );
            p_html_file[ i] << "</TABLE>" << endl;
        }
        //���� ���� ������ ������ � ���������
        if( num_html_file-1 == 0 )
        {
            p_html_file[ i] << "</TABLE>\n";
        }
        //���� ���� ������ � ��������
        if( i != 0 && i != ( num_html_file-1) )
        {
            p_html_file[ i] << html_tr(
                                html_th(
                                    html_a(fnames[i-1].c_str(),QObject::tr("Predidyschii fail otcheta").toStdString().c_str()) // ���������� ���� ������
                                )+
                                html_th(
                                    html_a( fnames[i+1].c_str(),QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // ��������� ���� ������
                                )
                            );
            p_html_file[ i] << "</TABLE>" << endl;
        }
    }
}

void silki_na_sleduiuchiyg( GroupReportFile& gfile)
{
    //��������� ������ � ������ htlm ���� �� ���������� ���� � �� �����������
    gfile.WriteToAllFile( "<TABLE class=\"zagolovok\" WIGHT=\"100%\" ALIGN=\"center\" border=1>\n" );
    for( int i=0; i< gfile.Size(); i++)
    {

        //���� ���� ������ ������
        if( i==0 && ( i!= (gfile.Size()-1) ) )
        {
            gfile.GetFile( i) << html_tr(
                                  html_th(QObject::tr("Eto pervii fail v otchete ").toStdString().c_str()) + // ��� ������ ���� � ������
                                  html_th(
                                      html_a( gfile.GetFileName( i+1).toStdString(),QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // ��������� ���� ������
                                  )
                              ).c_str();
            gfile.GetFile( i) << "</TABLE>" << endl;
        }

        //���� ���� ������ ���������
        if( i == gfile.Size()-1 && i != 0 )
        {
            gfile.GetFile( i) << html_tr(
                                  html_th(
                                      html_a( gfile.GetFileName(i-1).toStdString(),QObject::tr("Predidyschii fail v otchete").toStdString().c_str()) // ���������� ���� � ������
                                  )+
                                  html_th(QObject::tr("Eto poslednii fail otcheta").toStdString().c_str()) // ��� ��������� ���� ������
                              ).c_str();
            gfile.GetFile( i) << "</TABLE>" << endl;
        }
        //���� ���� ������ ������ � ���������
        if( gfile.Size()-1 == 0 )
        {
            gfile.GetFile( i) << "</TABLE>\n";
        }
        //���� ���� ������ � ��������
        if( i != 0 && i != ( gfile.Size()-1) )
        {
            gfile.GetFile( i) << html_tr(
                                  html_th(
                                      html_a( gfile.GetFileName( i-1).toStdString(),QObject::tr("Predidyschii fail otcheta").toStdString().c_str()) // ���������� ���� ������
                                  )+
                                  html_th(
                                      html_a( gfile.GetFileName( i+1).toStdString(),QObject::tr("Sledyiyschii fail otcheta").toStdString().c_str()) // ��������� ���� ������
                                  )
                              ).c_str();
            gfile.GetFile( i) << "</TABLE>" << endl;
        }
    }
}


//����� ������ � ��������� �������������� ��������
void get_from_Tree_Functional_object(sScopeFull cur, std::vector<NameScope > &vs_exe)
{
    QORMList<sScopeFull> data (cur); // ��� ��, ���������
    //����������� ���� �����
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //�������� ��� ������� � �������� �����
        if( (*j).system_type == EXE_SCOPE )
        {
            NameScope ns;
            ns.name=cur.name+"."+(*j).name;
            ns.file=(*j).file_path;
            ns.s.id=(*j).id;
            vs_exe.push_back(ns);
        }
        //������ �� ��� name_space � �������������� ��������
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

//����� ������ � ��������� �������������� ��������
void get_from_Tree_flowing_Functional_object(sScopeFull cur, std::vector<NameScope > &vs_exe)
{
    QORMList<sScopeFull> data (cur); // ��� ��, ���������
    //����������� ���� �����
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //�������� ��� ������� � �������� �����
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
        //������ �� ��� name_space � �������������� ��������
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
    QORMList<sScopeFull> data (cur); // ��� ��, ���������
    //����������� ���� �����
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //�������� ��� ������� � �������� �����
        if( (*j).system_type == INFO_SCOPE)
        {
            NameScope ns;
            ns.name=cur.name+"."+(*j).name;
            ns.file=(*j).file_path;
            ns.s.id=(*j).id;
            vs_exe.push_back(ns);

        }
        //������ �� ��� name_space � �������������� ��������
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
    QORMList<sScopeFull> data (cur); // ��� ��, ���������
    //����������� ���� �����
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //�������� ��� ������� � �������� �����
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
    QORMList<sScopeFull> data (cur); // ��� ��, ���������
    //����������� ���� �����
    for(QORMList<sScopeFull>::iterator j = data.begin(); j != data.end(); j++)
    {
        //�������� ��� ������� � �������� �����
        if( (*j).system_type ==EXE_SCOPE)
        {
            vs_exe.push_back(*j);
        }
        std::vector<sScopeFull > va;
        get_from_Base_Func_obj(*j,va);
        vs_exe.insert(vs_exe.end(),va.begin(),va.end());
    }
}


//��������� ������
// � - ������� �� �������� �������� ����������� . v - ���� ���������� ��� ��������
int treetoline(sScope a,std::vector<sScope> &v)
{
    v.push_back(a);
    QORMList<sScope> data(a); // ��� ��, ���������
    for(QORMList<sScope>::iterator j = data.begin(); j != data.end(); ++j)
    {
        treetoline( *j,v);
    }
    return 0;
}

std::vector<sScope>& wighttree(std::vector<sScope> &a,std::vector<sScope> &b,sScope c)
{
    QORMList<sScope> data(c); // ��� ��, ���������
    QORMList<sScope>::iterator j=data.begin();
    std::vector<sScope> vec; // ������ ��� ���������� �������� d
    //����������� � vec ���.
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
    //� ����� ������� ���� ������������� ��������
    //������ � ������������� � ������� ����� ��� ������������ � 0
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
    //������� � vec2 ��� �������� �� vec � �� ������� �����
    std::vector<sScope> vec2;
    for(size_t i=0; i<vec.size(); i++)
    {
        if(vec[i].d!=0)
        {
            vec2.push_back(vec[i]);
        }
    }

    //�������� � b ���� d
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
