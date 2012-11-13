/*
 * matrix_report.h
 *
 *  Created on: 30.10.2008
 *      Author: javamain
 */

#ifndef MATRIX_REPORT_H_
#define MATRIX_REPORT_H_
#include <libdb_interface.h>
#include "htmlfunction.h"
#include <dir.h>


using namespace std;



//быстрый способ построить матрицу
HRET Matrix_cont2(sReportInfo* pa);
HRET Matrix_info2(sReportInfo* pa);

QStringList & make_index_file_string(bool isCont);
//быстрый способ построить матрицу
int matrix_cont( QString path, int NUM, int NUM_SCREEN, list < string> & lsProcessedFiles);
int matrix_cont_legend( QString path, int NUM, int NUM_SCREEN, list < string> & lsProcessedFiles );
int matrix_info( QString path, int NUM, int NUM_SCREEN, list < string> & lsProcessedFiles);
int matrix_info_legend( QString path, int NUM, int NUM_SCREEN, list <string> & lsProcessedFiles);

//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
HRET Matrix_cont( sReportInfo* pa);   	    ///
HRET Matrix_cont_cluster( sReportInfo* pa); ///
HRET Matrix_info( sReportInfo* pa);	    ///
HRET Matrix_info_cluster( sReportInfo* pa); ///
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
int Matrix_cont_report_html2( int cx,
                              int cy,
                              string  fnames,
                              string  pathname,
                              string project_name,
                              string version,
                              list < string> & lsProcessedFiles);
int Matrix_cont_report_html( int cx,
                             int cy,
                             string  fnames,
                             string  pathname,
                             string project_name,
                             string version,
                             list < string> & lsProcessedFiles);
int Matrix_cont_report_html3( int cx,
                              int cy,
                              string fnames,
                              string pathname,
                              string project_name,
                              string version,
                              list< string> & lsprocessedFiles);
int Matrix_info_report_html2( int cx,
                              int cy,
                              string  fnames,
                              string  pathname,
                              string project_name,
                              string version,
                              list < string> & lsProcessedFiles);
int Matrix_info_report_html( int cx,
                             int cy,
                             string  fnames,
                             string  pathname,
                             string project_name,
                             string version,
                             list < string> & lsProcessedFiles);
int Matrix_info_report_html3( int cx,
                              int cy,
                              string  fnames,
                              string  pathname,
                              string project_name,
                              std::string version,std::list <std::string> & lsProcessedFiles);
int make_cont_Matrix_report( string file,
                             string path,
                             string project_name,
                             string version,
                             list < string>& lsProcessedFiles);

int make_info_Matrix_report( string file,
                             string path,
                             string project_name,
                             string version,
                             list < string> & lsProcessedFiles);
#endif /* MATRIX_REPORT_H_ */
