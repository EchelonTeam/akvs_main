#include "servicespace.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <QObject>
using namespace std;

const char *servicespace::Types[] =
{
    "",
    "",
    "",
    "",
    QObject::tr(" Kon. ").toStdString().c_str(),
    QObject::tr(" Nach. ").toStdString().c_str(),
    QObject::tr(" Lin1.").toStdString().c_str(),
    QObject::tr(" Vet. ").toStdString().c_str()
};

void servicespace::aist_probe_log(int type, int findex, int pindex, int mindex, int index, int id, int line, string name)
{
    string stype = Types[type];

    if (line < 0) line = 0;

    cout << setw(2) << type << "    " <<
         findex << setw(4) << pindex << setw(4) << index <<
         stype << setw(4) << line <<
         setw(4) << mindex << "  ____Din_Go (\"" <<
         id << "\"," << pindex << ");/*"<< type <<
         "u=1\""<< name << "\"*/  " << name << endl;

    if (type == 6 || type == 4)
        cout << endl;
    cout.flush();
}
