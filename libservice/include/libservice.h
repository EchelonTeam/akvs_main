#ifndef __LIB_SERVICE_INCLUDE__
#define __LIB_SERVICE_INCLUDE__

#include <stack>
#include "matrix_report.h"
#include "block_shem_report.h"
#include "groupfiles.h"

using namespace std;
int build_inform_objects_list ( string& strOutputDir);

// функция выводит в файл все содержание vs

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

// функция выводит в файл все содержание vs
// vs                - вектор содержит массив строк названий информационного объекта объекта
// project_name		 - имя проекта
// base_file_name    - имя отчета он будет индексироваться и будте два варианта txt и html обязательно английское!!!!!
// название идет типа "мой файл" функция сама к ниму добавит индекс и расшерения "txt" и "html"
// mod_for_html_file - количество строчек в html файле
// mod_for_txt_file  - количество строчек в txt файле
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
//**************поставить в соответствие скоупам блоки и вес поместить в блоки.
// функция взвешивания жэлементов в дереве
// взвесим дерево
// в функцию посылаются вектор последних элементов
// функция начинает расставлять d от них до корневого элемента c
// дальше переписывае в b (плосское дерево ) поле d .
vector< sScopeFull>& wighttree(
    vector< sScopeFull> &a,
    vector< sScopeFull> &b,
    sScopeFull c);
class NameScope;

//поиск висящих функций
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

// класс перевода sScopeFull на номер.
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
//структура хранящая соответствие id - num
    vector< pair< sScopeFull, int> > sScopeFull_num;
//найти num, по id
    int find_num( sScopeFull scope)
    {
        for( size_t i= 0; i< sScopeFull_num.size(); i++)
        {
            if( sScopeFull_num[ i].first.id== scope.id) return sScopeFull_num[ i].second;
        }
        return 1;
    }

//найти id, по num
    sScopeFull find_sScopeFull( int num)
    {
        for( size_t i= 0; i< sScopeFull_num.size(); i++)
        {
            if( sScopeFull_num[ i].second== num) return sScopeFull_num[i].first;
        }
        return 1;
    }
};





//<<<<<<<<<<<<<<<<<<<<<<<<<<<конец отрисовки блок схем<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//отчет по трассам трассы
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
    string project_name,       // имя проекта
    string version,            // имя версии
    string base_file_name,     // начала имени файла
    string path,               // путь
    int mod_for_html_file,          // модуль для файлов
    list< string> & lsProcessedFiles); // структура для путей к созданным файлам
//эти функции нужно вызывать из ГУЯ записывая в структуру необходимую информацию
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
HRET FilePrint_func_object_quick(sReportInfo* pa);

HRET FilePrint_func_object( sReportInfo* pa);             ////
HRET FilePrint_info_object( sReportInfo* pa);             ////
HRET Trace_Report( sReportInfo* pa);                        ////
HRET Trace_Report_cluster( sReportInfo* pa);            ////
HRET SignatureReport( sReportInfo* pa);                    ////
HRET DinamicReport( sReportInfo* pa);                     ////
HRET GraphReport( sReportInfo* pa);                        ////
HRET GraphReport2( sReportInfo* pa);                      // данный граф строится только по файлам с уязвимостями
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}



// тестовая версия функции трасс ф-ф.
int TraceReportf_f_new_javascript(
    string project_name,       // имя проекта
    string version,            // имя версии
    string base_file_name,     // начала имени файла
    string path,               // путь
    int mod_for_html_file,          // модуль для файлов
    list< string> & lsProcessedFiles); // структура для путей к созданным файлам
void silki_na_sleduiuchiy( fstream * p_html_file , vector<string>  fnames,int num_html_file);
void silki_na_sleduiuchiyg( GroupReportFile& gfiles);

HRET Trace_Report_ersatz( sReportInfo* pa);

void test();

int NumFunctionInDb();

void dbg( const char *msg, ... );


void initLibservice( std::string lang );

#endif //__LIB_SERVICE_INCLUDE__
