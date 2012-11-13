#ifndef OLTMANAGER_H
#define OLTMANAGER_H


#include <QMap>
#include <QSet>
#include <QHash>
#include "olt_xmlmanager.h"
#include "common.h"
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace ak_vs_db;
class sNode;
class OLTManager;
// тип указателя ф. для функции обхода дерева.
typedef void (*                 WORK_ON_SCOPE           )(sNode&);
typedef void (OLTManager::*     WORK_ON_SCOPE_MANAGER   )(sNode&);
typedef void (*                 WORK_ON_SCOPE_TREE      )(sNode&,OLTManager& p);

//данная конструкция будет нодом
class sNode
{
public:
    sNode()
    {

        this->setId(index);
        index++;
    }
    static int index;
    sNode(const sNode & a):NAME(a.NAME),TYPE(a.TYPE),FILE(a.FILE)
    {
        this->setId(index);
        index++;
        this->data.name=a.getName().toStdString();
        this->data.system_type=a.getType();
        this->data.file_path=a.getFile().toStdString();

    }

    sNode( QString N, eScopeType T)
    {
        this->setId( index);
        index++;
        this->setName( N);
        this->setType( T);
        this->data.name= N.toStdString();
        this->data.system_type= T;
    }

    sNode( QString N, eScopeType T, QString F)
    {
        this->setId( index);
        index++;
        this->setName( N);
        this->setType( T);
        this->setFile( F);
        this->data.name= N.toStdString();
        this->data.system_type= T;
        this->data.file_path= F.toStdString();
    }

    QString toQString()
    {
        return QString( "[name:")+ this->getName()+ QString( " type:")+ QString::number( this->getType(), 10)+ QString( " id:")+ QString::number( this->getId(), 10)+ QString( " pid:")+ QString::number( this->getPerentId(), 10)+ QString( "] ");
    }

    //Нахер дублировать внутри sNode имя, тип  и путь к файлу?????
    //Посмотри поля класса sScopeFull этими методами (getName и т.п.) может сразу вызвать методы sScopeFull

    /**
     *  Методы    GET
     **/
    QString     getName()       const
    {
        return this->NAME;
    }
    uint        getPerentId()   const
    {
        return this->p_id;
    }
    eScopeType  getType()       const
    {
        return this->TYPE;
    }
    QString     getFile()       const
    {
        return this->FILE;
    }
    uint        getFileID()     const
    {
        return id_file;
    }
    uint        getId()         const
    {
        return this->id;
    }

    /**
     *   Методы   SET
     **/
    void        setFile( QString f)
    {
        this->FILE= f;
    }
    void        setName( QString n)
    {
        this->NAME= n;
    }
    void        setType( eScopeType t)
    {
        this->TYPE= t;
    }
    void        setId( int Id)
    {
        this->id= Id;
    }
    void        setPerentID( int Id)
    {
        this->p_id= Id;
    }
    void        setFileID( ID_FILE idf)
    {
        this->id_file= idf;
    }

private:
    QString     NAME;
    eScopeType  TYPE;
    QString     FILE;
    uint        id;
    uint        p_id;
    uint        id_file;
public:
    QMap< QString, QString >   metadata;
    QMap< QString, int >       metadata_int;
    sScopeFull data; // тут будем хранить все что нужно для базы.
};



class OLTManager : OLTXMLManager
{
public:
    OLTManager()
    {
        index= 0;
        curLine= NULL;
        cout<< "making tree!!!" << endl;
        this->currNOD= NULL;
        this->rootNOD= NULL;
    }
    //базовые методы для работы с объектами
    virtual HRET createScope( QString , eScopeType); //если уже есть такой - не создается
    virtual HRET createScope( sNode  a); // создать скоуп из уже готового нода.
    virtual HRET enterScope( QString NAME, eScopeType TYPE); // здесь - маска типа, а не сам тип - битовая проверка
    virtual HRET forceScope( QString, eScopeType); // вызывает последовательно createS, enterS
    virtual HRET finishScopeUp( eScopeType); // Поднимаюсь наверх, пока не встречу объект соответствующий битовой маске (проверяем & ), если его нет, то курсор остается на месте, а на выходе -1
    virtual HRET linkScope( QString, eScopeType, eVisionArea); //связать текущий с нужным объектом с учетом выбранных областей видимости (в третьем параметре их может быть несколько);
    virtual HRET linkSiblingScope( QString ,QString, eScopeType, eVisionArea); // все тоже самое, только в первой строке указывается имя непосредственного наследника текущего объекта который вызывает,  и имя объекта (тоже наследника которого вызывают)
    //продвинутые методы для работы с объектами
    virtual HRET setScope( QStringList& );//Сделать текущим нужный объект от начала скоупа  по указанному пути (путь хранится в виде упорядоченного списка строк)
    //  вот пример пути: "Родитель1-> Родитель2-> Родитель3-> Сын 4" (сделали текущим объект на 4 уровня в глубине)
    virtual HRET setFile ( QString); // все выдаваемые объекты будут принадлежать такому-то файлу, если его нет - добавляется в структуру, если уже есть - используется уже существующая копия
    virtual HRET setMetaStr( QString name, QString value, eObjectRef ref= CURRENT_REF); //Сохранить метаданные строки
    virtual HRET setMetaInt( QString name, int value, eObjectRef ref= CURRENT_REF); //Сохранить интовые метаданные
    virtual HRET setLineSource( int* p);
    virtual HRET commitData(); //сохраняет в БД всю инфу из все базы данных
    virtual void commitLink();
    static  void commitNode( sNode& node, OLTManager &p);   // комитить один нод
    virtual ~OLTManager();
    HRET    go_tree( WORK_ON_SCOPE func, sNode& a);
    HRET    go_tree2( WORK_ON_SCOPE_TREE func, sNode& a);
    virtual HRET print_tree2( sNode& a);
    void Up();
    void SetMetadata( QString &key, QString &value);
    void SetMetadata( QString &key, int value);

    sNode*      getRoot() const
    {
        return this->rootNOD;
    }
    //установить текущий ноуд
    bool        setCurrNode( sNode &a)
    {
        this->currNOD= &a;
        return true;
    }
    sNode* getCurrNode()
    {
        return this->currNOD;
    }
    //сделать ребенка текущему нуду
    bool addChild( sNode &a)
    {
        if( this->currNOD== NULL) return false;
        treeObj[ currNOD->getId()].push_back( &a);
        nodeList.push_back( &a);
        return true;
    }
private :
    //найти для текущего ноуда
    QList< QPair< sNode*, sNode*> >   Links;
    sNode *rootNOD;                            // главный нод
    sNode *currNOD;                            // текущий нод
    QMap < uint ,QList< sNode* > > treeObj;    // дерево вниз
    QMap < uint , sNode* > treeUP;             // связи с родителями.
    QList<sNode*>                  nodeList;   // тупо список



    QList<QPair<QString,QString> > scope_link; // имена линков связанные
    // вызывает- вызываемый

private:
    QList <sFile> fList;

public:
    int index;
    uint curFile;
    uint* curLine;
    sFile getFromIndex(int index);
    int indexOfFile(sFile file);
    ID_FILE addFile(QString file);
    QString proj_name;

};

eScopeType StringToType(QString str);



#endif // OLTMANAGER_H
