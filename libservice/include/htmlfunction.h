#ifndef HTMLFUNCTION_H_alex
#define HTMLFUNCTION_H_alex
#include "groupfiles.h"
#include <stdlib.h>
#include <iostream>


using namespace alex;
using namespace std;
void debug_message(int i, string str);
namespace alex
{
string  html_breaket( const char* type);
string  html_breaket( string type, const char *  type2, string  text);
string  html_breaket( const char* type, const char *  type2, const char *  text);
string  html_breaket( const char* type, const char *  type2, string  text);
string  html_a( const char * href, string text);
string  html_a( string href, const char * text);
string  html_a( const char * href, const char * text);
string  html_th( string text);
string  html_th( const char * text);
string  html_tr( string text);
string  html_tr( const char* text);
string  inttostring( int i);
string  html_td( string text);
string  html_td( const char* text);
};
string& getCSS();
string  strelka( string &fname, const char* ch);
string make_zagolovok(string report_name, string project, string version, string file_name);
void    zagolovok_html_file( fstream* p_html_file, const char * nazvanie, string &project_name, string& version, string& fnames, int i, int num_html_file);
void    zagolovok_html_gfile( GroupReportFile& gfile, const char * nazvanie, string &project_name, string& version);
#endif /*HTMLFUNCTION_H_*/
