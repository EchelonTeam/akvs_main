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
// ��� ��������� �. ��� ������� ������ ������.
typedef void (*                 WORK_ON_SCOPE           )(sNode&);
typedef void (OLTManager::*     WORK_ON_SCOPE_MANAGER   )(sNode&);
typedef void (*                 WORK_ON_SCOPE_TREE      )(sNode&,OLTManager& p);

//������ ����������� ����� �����
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

    //����� ����������� ������ sNode ���, ���  � ���� � �����?????
    //�������� ���� ������ sScopeFull ����� �������� (getName � �.�.) ����� ����� ������� ������ sScopeFull

    /**
     *  ������    GET
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
     *   ������   SET
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
    sScopeFull data; // ��� ����� ������� ��� ��� ����� ��� ����.
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
    //������� ������ ��� ������ � ���������
    virtual HRET createScope( QString , eScopeType); //���� ��� ���� ����� - �� ���������
    virtual HRET createScope( sNode  a); // ������� ����� �� ��� �������� ����.
    virtual HRET enterScope( QString NAME, eScopeType TYPE); // ����� - ����� ����, � �� ��� ��� - ������� ��������
    virtual HRET forceScope( QString, eScopeType); // �������� ��������������� createS, enterS
    virtual HRET finishScopeUp( eScopeType); // ���������� ������, ���� �� ������� ������ ��������������� ������� ����� (��������� & ), ���� ��� ���, �� ������ �������� �� �����, � �� ������ -1
    virtual HRET linkScope( QString, eScopeType, eVisionArea); //������� ������� � ������ �������� � ������ ��������� �������� ��������� (� ������� ��������� �� ����� ���� ���������);
    virtual HRET linkSiblingScope( QString ,QString, eScopeType, eVisionArea); // ��� ���� �����, ������ � ������ ������ ����������� ��� ����������������� ���������� �������� ������� ������� ��������,  � ��� ������� (���� ���������� �������� ��������)
    //����������� ������ ��� ������ � ���������
    virtual HRET setScope( QStringList& );//������� ������� ������ ������ �� ������ ������  �� ���������� ���� (���� �������� � ���� �������������� ������ �����)
    //  ��� ������ ����: "��������1-> ��������2-> ��������3-> ��� 4" (������� ������� ������ �� 4 ������ � �������)
    virtual HRET setFile ( QString); // ��� ���������� ������� ����� ������������ ������-�� �����, ���� ��� ��� - ����������� � ���������, ���� ��� ���� - ������������ ��� ������������ �����
    virtual HRET setMetaStr( QString name, QString value, eObjectRef ref= CURRENT_REF); //��������� ���������� ������
    virtual HRET setMetaInt( QString name, int value, eObjectRef ref= CURRENT_REF); //��������� ������� ����������
    virtual HRET setLineSource( int* p);
    virtual HRET commitData(); //��������� � �� ��� ���� �� ��� ���� ������
    virtual void commitLink();
    static  void commitNode( sNode& node, OLTManager &p);   // �������� ���� ���
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
    //���������� ������� ����
    bool        setCurrNode( sNode &a)
    {
        this->currNOD= &a;
        return true;
    }
    sNode* getCurrNode()
    {
        return this->currNOD;
    }
    //������� ������� �������� ����
    bool addChild( sNode &a)
    {
        if( this->currNOD== NULL) return false;
        treeObj[ currNOD->getId()].push_back( &a);
        nodeList.push_back( &a);
        return true;
    }
private :
    //����� ��� �������� �����
    QList< QPair< sNode*, sNode*> >   Links;
    sNode *rootNOD;                            // ������� ���
    sNode *currNOD;                            // ������� ���
    QMap < uint ,QList< sNode* > > treeObj;    // ������ ����
    QMap < uint , sNode* > treeUP;             // ����� � ����������.
    QList<sNode*>                  nodeList;   // ���� ������



    QList<QPair<QString,QString> > scope_link; // ����� ������ ���������
    // ��������- ����������

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
