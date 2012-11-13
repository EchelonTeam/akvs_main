#ifndef __LIB_SERVICE_INCLUDE__
#define __LIB_SERVICE_INCLUDE__

#include <stack>
#include "matrix_report.h"
#include "block_shem_report.h"
#include "groupfiles.h"

using namespace std;
int build_inform_objects_list ( string& strOutputDir);

// ������� ������� � ���� ��� ���������� vs

class NameScope;
class NameScope2;

int FilePrint_func_object( vector< NameScope > &vs,
                           vector< NameScope > &vs2,
                           string project_name, string version,
                           string base_file_name, string path,
                           int mod_for_html_file,
                           int mod_for_txt_file,
                           list < string> & lsProcessedFiles);

int FilePrint_func_object_quick(
    QList< NameScope2 > &vs,
    QList< NameScope2 > &vs2,
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    int mod_for_txt_file,
    list< string> &lsProcessedFiles);

// ������� ������� � ���� ��� ���������� vs
// vs                - ������ �������� ������ ����� �������� ��������������� ������� �������
// project_name		 - ��� �������
// base_file_name    - ��� ������ �� ����� ��������������� � ����� ��� �������� txt � html ����������� ����������!!!!!
// �������� ���� ���� "��� ����" ������� ���� � ���� ������� ������ � ���������� "txt" � "html"
// mod_for_html_file - ���������� ������� � html �����
// mod_for_txt_file  - ���������� ������� � txt �����
int FilePrint_info_object( vector< NameScope > &vs,
                           vector< NameScope > &vs2,
                           string project_name ,
                           string version,
                           string base_file_name,
                           string path,
                           int mod_for_html_file,
                           int mod_for_txt_file,
                           list < string> & lsProcessedFilesf);
/*
int FilePrint_flowing_func_object_quick(
                QList< NameScope2> &vs,
                QList< NameScope2> &vs2,
                string project_name,
                string version,
                string base_file_name,
                string path,
                int mod_for_html_file,
                int mod_for_txt_file,
                list < string> & lsProcessedFiles
                );
*/
int FilePrint_flowing_func_object(
    vector< NameScope > &vs,
    vector< NameScope > &vs2,
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    int mod_for_txt_file,
    list < string> & lsProcessedFiles);
//**************��������� � ������������ ������� ����� � ��� ��������� � �����.
// ������� ����������� ���������� � ������
// ������� ������
// � ������� ���������� ������ ��������� ���������
// ������� �������� ����������� d �� ��� �� ��������� �������� c
// ������ ����������� � b (�������� ������ ) ���� d .
vector< sScopeFull>& wighttree(
    vector< sScopeFull> &a,
    vector< sScopeFull> &b,
    sScopeFull c);
class NameScope;

//����� ������� �������
void get_from_Tree_flowing_Functional_object(
    sScopeFull cur,
    vector< NameScope > &vs_exe);


class NameScope
{
public:
    string name;
    string file;
    sScope s;
    int start_pos;
};

class NameScope2
{
public:
    int id;
    string name;
    string file;
    string arg;
};

// ����� �������� sScopeFull �� �����.
class sScopeFull_to_number
{
public:
    sScopeFull_to_number() {}
    sScopeFull_to_number( vector< sScopeFull> a)
    {
        for(size_t i= 0; i< a.size(); i++)
        {
            sScopeFull_num.push_back( pair< sScopeFull, int> ( a[ i], sScopeFull_num.size() ) );
        }
    }
//��������� �������� ������������ id - num
    vector< pair< sScopeFull, int> > sScopeFull_num;
//����� num, �� id
    int find_num( sScopeFull scope)
    {
        for( size_t i= 0; i< sScopeFull_num.size(); i++)
        {
            if( sScopeFull_num[ i].first.id== scope.id) return sScopeFull_num[ i].second;
        }
        return 1;
    }

//����� id, �� num
    sScopeFull find_sScopeFull( int num)
    {
        for( size_t i= 0; i< sScopeFull_num.size(); i++)
        {
            if( sScopeFull_num[ i].second== num) return sScopeFull_num[i].first;
        }
        return 1;
    }
};





//<<<<<<<<<<<<<<<<<<<<<<<<<<<����� ��������� ���� ����<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//����� �� ������� ������
string & graph( sScopeFull cur,
                string &str,
                stack< string> &stk);
int signaturereportM(	string project_name ,
                        string version,
                        string base_file_name,
                        string path,
                        int mod_for_html_file,
                        list< string> &lsProcessedFiles);
int GraphReportf_f(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles);

int GraphReportf_f2(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles);

int GraphReportf_f2_for_critical_path(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles);

int TraceReportf_f(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list < string> & lsProcessedFilesf);
int TraceReportv_f(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list <string> & lsProcessedFilesf);
int TraceReportv_v(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list <string> & lsProcessedFilesf);
int SignatureReport(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list <string> & lsProcessedFilesf);
int DinamicReport(
    string project_name ,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles);
int DinamicReport_vetvi(
    string project_name,
    string version,
    string base_file_name,
    string path,
    int mod_for_html_file,
    list< string> & lsProcessedFiles);
int TraceReportf_f_new(
    string project_name,       // ��� �������
    string version,            // ��� ������
    string base_file_name,     // ������ ����� �����
    string path,               // ����
    int mod_for_html_file,          // ������ ��� ������
    list< string> & lsProcessedFiles); // ��������� ��� ����� � ��������� ������
//��� ������� ����� �������� �� ��� ��������� � ��������� ����������� ����������
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
HRET FilePrint_func_object_quick(sReportInfo* pa);

HRET FilePrint_func_object( sReportInfo* pa);             ////
HRET FilePrint_info_object( sReportInfo* pa);             ////
HRET Trace_Report( sReportInfo* pa);                        ////
HRET Trace_Report_cluster( sReportInfo* pa);            ////
HRET SignatureReport( sReportInfo* pa);                    ////
HRET DinamicReport( sReportInfo* pa);                     ////
HRET GraphReport( sReportInfo* pa);                        ////
HRET GraphReport2( sReportInfo* pa);                      // ������ ���� �������� ������ �� ������ � ������������
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}



// �������� ������ ������� ����� �-�.
int TraceReportf_f_new_javascript(
    string project_name,       // ��� �������
    string version,            // ��� ������
    string base_file_name,     // ������ ����� �����
    string path,               // ����
    int mod_for_html_file,          // ������ ��� ������
    list< string> & lsProcessedFiles); // ��������� ��� ����� � ��������� ������
void silki_na_sleduiuchiy( fstream * p_html_file , vector<string>  fnames,int num_html_file);
void silki_na_sleduiuchiyg( GroupReportFile& gfiles);

HRET Trace_Report_ersatz( sReportInfo* pa);

void test();

int NumFunctionInDb();

void dbg( const char *msg, ... );


void initLibservice( std::string lang );

#endif //__LIB_SERVICE_INCLUDE__
