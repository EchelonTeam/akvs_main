#ifndef OLT_XMLMANAGER_H
#define OLT_XMLMANAGER_H

#include "ioltmanager.h"

#include <QTextStream>

class OLTXMLManager :  IOLTManager
{
public:
    OLTXMLManager();
    HRET importFromXML(QTextStream&); // Экспортирует данные из стандартизованного формата обмена АК-ВС
    HRET exportToXML(QTextStream); // Экспортирует данные в стандартизованный формат обмена АК-ВС

    HRET importFromXMLVM(QTextStream&); //Импортирует данные из XMLVM

};

#endif // OLT_XMLMANAGER_H
