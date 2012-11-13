#ifndef OLTDEBUGMANAGER_H
#define OLTDEBUGMANAGER_H

#include "olt_xmlmanager.h"

class OLTDebugManager : OLTXMLManager
{
public:
    OLTDebugManager();
    ~OLTDebugManager();
    virtual HRET createScope(QString&, eScopeType); //если уже есть такой - не создается
    virtual HRET enterScope(QString&, eScopeType); // здесь - маска типа, а не сам тип - битовая проверка
    virtual HRET forceScope(QString&, eScopeType); // вызывает последовательно createS, enterS
    virtual HRET goUp(eScopeType); // Поднимаюсь наверх, пока не встречу объект соответствующий битовой маске (проверяем & ), если его нет, то курсор остается на месте, а на выходе -1

    virtual HRET linkScope(QString&, eLinkType, eVisionArea); //связать текущий с нужным объектом с учетом выбранных областей видимости (в третьем параметре их может быть несколько);
    virtual HRET linkSiblingScope( QString& ,QString&, eLinkType, eVisionArea); // все тоже самое, только в первой строке указывается имя непосредственного наследника текущего объекта который вызывает,  и имя объекта (тоже наследника которого вызывают)

    //продвинутые методы для работы с объектами
    virtual HRET setScope( QStringList& );//Сделать текущим нужный объект от начала скоупа  по указанному пути (путь хранится в виде упорядоченного списка строк)
    //  вот пример пути: "Родитель1-> Родитель2-> Родитель3-> Сын 4" (сделали текущим объект на 4 уровня в глубине)

    virtual HRET setFile (QString&); // все выдаваемые объекты будут принадлежать такому-то файлу, если его нет - добавляется в структуру, если уже есть - используется уже существующая копия
    virtual HRET setMetaStr(QString name, QString value, eObjectRef ref=CURRENT_REF); //Сохранить метаданные строки
    virtual HRET setMetaInt(QString name, int value, eObjectRef ref=CURRENT_REF); //Сохранить интовые метаданные

};

#endif // OLTDEBUGMANAGER_H
