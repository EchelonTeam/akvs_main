#ifndef OLT_XMLMANAGER_H
#define OLT_XMLMANAGER_H

#include "ioltmanager.h"

#include <QTextStream>

class OLTXMLManager :  IOLTManager
{
public:
    OLTXMLManager();
    HRET importFromXML(QTextStream&); // ������������ ������ �� ������������������ ������� ������ ��-��
    HRET exportToXML(QTextStream); // ������������ ������ � ����������������� ������ ������ ��-��

    HRET importFromXMLVM(QTextStream&); //����������� ������ �� XMLVM

};

#endif // OLT_XMLMANAGER_H
