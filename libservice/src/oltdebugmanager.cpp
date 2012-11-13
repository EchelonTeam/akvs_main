#include "oltdebugmanager.h"

#include <iostream>
#include <QDebug>

QString my_ident;

OLTDebugManager::OLTDebugManager()
{
    qDebug() << QObject::tr("Creating OLTManager");
}


OLTDebugManager::~OLTDebugManager()
{
    qDebug() << QObject::tr("Finishing OLTManager");
}

HRET OLTDebugManager::createScope(QString& name, eScopeType type) //если уже есть такой - не создается
{
    my_ident += QString("\t");
    qDebug() << my_ident << QObject::tr("Creating scope ") << name << " type: " << type;
    return S_OKEY;
}

HRET OLTDebugManager::enterScope(QString& name, eScopeType mask) // здесь - маска типа, а не сам тип - битовая проверка
{
    qDebug() << my_ident << "Entering " << name << " mask: " << mask;
    return S_OKEY;
}

HRET OLTDebugManager::forceScope(QString& name, eScopeType type) // вызывает последовательно createS, enterS
{
    createScope(name,type);
    enterScope(name,type);
    return S_OKEY;
}
HRET OLTDebugManager::goUp(eScopeType mask) // Поднимаюсь наверх, пока не встречу объект соответствующий битовой маске (проверяем & ), если его нет, то курсор остается на месте, а на выходе -1
{
    qDebug () << "goUp till " << mask;
    return S_OKEY;
}
HRET OLTDebugManager::linkScope(QString& link_a, eLinkType type, eVisionArea vision) //связать текущий с нужным объектом с учетом выбранных областей видимости (в третьем параметре их может быть несколько);
{
    qDebug () << "Linking " << link_a << " type:" << type << " with vision " << vision;
    return S_OKEY;
}

HRET OLTDebugManager::linkSiblingScope( QString& link_a,QString& link_b, eLinkType type, eVisionArea vision) // все тоже самое, только в первой строке указывается имя непосредственного наследника текущего объекта который вызывает,  и имя объекта (тоже наследника которого вызывают)
{
    qDebug () << "Linking " << link_a << " with " << link_b << " type:" << type << " with vision " << vision;
    return S_OKEY;
}
//продвинутые методы для работы с объектами

HRET OLTDebugManager::setScope( QStringList& list)//Сделать текущим нужный объект от начала скоупа  по указанному пути (путь хранится в виде упорядоченного списка строк)
//  вот пример пути: "Родитель1-> Родитель2-> Родитель3-> Сын 4" (сделали текущим объект на 4 уровня в глубине)
{
    cerr << "Setting";
    foreach(QString str, list)
    {
        cerr << str.toStdString() << ":";
    }
    return S_OKEY;
}

HRET OLTDebugManager::setFile (QString& filename) // все выдаваемые объекты будут принадлежать такому-то файлу, если его нет - добавляется в структуру, если уже есть - используется уже существующая копия
{
    qDebug() << "current File: "<<filename;
    return S_OKEY;
}


HRET OLTDebugManager::setMetaStr(QString name, QString value, eObjectRef ref) //Сохранить метаданные строки
{
    return S_OKEY;
}

HRET OLTDebugManager::setMetaInt(QString name, int value, eObjectRef ref) //Сохранить интовые метаданные
{
    return S_OKEY;
}
