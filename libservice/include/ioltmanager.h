#ifndef IOLTMANAGER_H
#define IOLTMANAGER_H

#include <common.h>

#include <QString>
#include <QStringList>

enum eVisionArea
{
    UPPER_AREA =0x1,  //всё что выше его и имеет общих с ним предков (дети его родителей, дети родителей его родителей и т.п.)
    PARENTS_AREA = 0x2, //любой из его родителей (в т.ч. и родителей родителей), на любой высоте от него
    SAME_AREA = 0x4, // все объекты, имеющие того же непосредственного родителя, т.е. прямые потомки только непостредственного родителя (расстояние =1)
    SIBLINGS = 0x8, // только прямые потомки этого объекта (расстояние =1)
    CHILDREN = 0x10 // все дальние и близкие дети этого объекта на любом расстоянии
};


enum eObjectRef
{
    CURRENT_REF=0x1, //Текущий скоуп
    LAST_INSERTION_REF=0x2 //Последний вставленный в базу данных объект

};

class IOLTManager
{
public:

public:

    //базовые методы для работы с объектами
    virtual HRET createScope(QString, eScopeType)=0; //если уже есть такой - не создается
    virtual HRET enterScope(QString, eScopeType)=0; // здесь - маска типа, а не сам тип - битовая проверка
    virtual HRET forceScope(QString, eScopeType)=0; // вызывает последовательно createS, enterS
    virtual HRET finishScopeUp(eScopeType)=0; // Поднимаюсь наверх, пока не встречу объект соответствующий битовой маске (проверяем & ), если его нет, то курсор остается на месте, а на выходе -1

    virtual HRET linkScope(QString, eScopeType, eVisionArea)=0; //связать текущий с нужным объектом с учетом выбранных областей видимости (в третьем параметре их может быть несколько);
    virtual HRET linkSiblingScope( QString ,QString, eScopeType, eVisionArea)=0; // все тоже самое, только в первой строке указывается имя непосредственного наследника текущего объекта который вызывает,  и имя объекта (тоже наследника которого вызывают)

    //продвинутые методы для работы с объектами
    virtual HRET setScope( QStringList& )=0;//Сделать текущим нужный объект от начала скоупа  по указанному пути (путь хранится в виде упорядоченного списка строк)
    //  вот пример пути: "Родитель1-> Родитель2-> Родитель3-> Сын 4" (сделали текущим объект на 4 уровня в глубине)

    virtual HRET setFile (QString)=0; // все выдаваемые объекты будут принадлежать такому-то файлу, если его нет - добавляется в структуру, если уже есть - используется уже существующая копия

    virtual HRET setMetaStr(QString name, QString value, eObjectRef ref)=0; //Сохранить метаданные строки =CURRENT_REF
    virtual HRET setMetaInt(QString name, int value, eObjectRef ref)=0; //Сохранить интовые метаданные =CURRENT_REF


    virtual HRET commitData()=0; //сохраняет в БД всю инфу из все базы данных

};

#endif // IOLTMANAGER_H
