#include "oltmanager.h"
#include "libdb_interface.h"

#include "common.h"

#include <QPair>
#include <QStack>
#include <QDebug>

using namespace ak_vs_db;
using namespace std;
#define rootPerentID_NULL -1
int sNode::index=0;
//базовые методы для работы с объектами
/*HRET OLTManager::createScope(QString& NAME, eScopeType TYPE) //если уже есть такой - не создается
{


    return createScope(sNode(NAME,TYPE));
}*/
HRET OLTManager::createScope(QString NAME, eScopeType TYPE) //если уже есть такой - не создается
{
    qDebug() << NAME << TYPE;
    return createScope(sNode(NAME,TYPE));
}

/*
HRET OLTManager::createScope(sNode & a)
{
    sNode * p =new sNode(a);
    if(this->currNOD==NULL)
    {
        this->nodeList.push_back(p);
        this->currNOD=p;
        if(this->rootNOD==NULL)
        {
            this->rootNOD=p;
            this->getRoot()->data.parent_id=eFirstScope;
        }
        //cout<<p->toQString().toStdString();
        return 1;
    }
    p->setPerentID(this->currNOD->getId());

    this->treeUP[p->getId()]=this->currNOD;
    this->nodeList.push_back(p);
    this->treeObj[this->currNOD->getId()].push_back(p);
   // cout<<p->toQString().toStdString();
    return 1;
}
*/

HRET OLTManager::createScope(sNode  a)
{
    sNode *p=new sNode(a);
    if(this->currNOD==NULL)
    {
        this->nodeList.push_back(&a);
        this->currNOD=p;
        this->rootNOD=p;
        this->getRoot()->data=eFirstScope;
        return 1;
    }
    p->setPerentID(this->currNOD->getId());
    p->setFileID(curFile);
    if(curLine) p->data.pos_x1=*curLine;
    this->treeUP[p->getId()]=this->currNOD;
    this->nodeList.push_back(&a);
    this->treeObj[this->currNOD->getId()].push_back(p);
    return 1;
}

HRET OLTManager::enterScope(QString NAME, eScopeType TYPE) // здесь - маска типа, а не сам тип - битовая проверка
{
    if(this->currNOD==NULL) return -1;
    QList<sNode*> lst( this->treeObj[this->currNOD->getId()] );
    for(QList<sNode* >::iterator it=lst.begin(); it!=lst.end(); ++it)
    {
        if(NAME.compare((*it)->getName())==0 && ( (TYPE & (*it)->getType())!=0) )
        {
            currNOD=(*it);
            return 1;
        }
    }
    return 0;
}

HRET OLTManager::forceScope(QString NAME, eScopeType TYPE) // вызывает последовательно createS, enterS
{
    createScope(NAME, TYPE);
    enterScope(NAME,TYPE);
    return 1;
}

HRET OLTManager::finishScopeUp(eScopeType TYPE) // Поднимаюсь наверх, пока не встречу объект соответствующий битовой маске (проверяем & ), если его нет, то курсор остается на месте, а на выходе -1
{
    if(this->currNOD==NULL) return -1;
    sNode * temp=NULL;
    sNode * temp2=NULL;
    temp=this->currNOD;
    if(curLine) temp->data.pos_x2=*curLine;
    for(;;)
    {
        temp2=this->treeUP[temp->getId()];
        if(temp2==NULL)
        {
            return -1;
        }
        if(temp2->getType()&TYPE)
        {
            this->currNOD=temp2;
            return 1;
        }
    }
    return 1;
}


HRET OLTManager::linkScope(QString NAME, eScopeType TYPE, eVisionArea AREA) //связать текущий с нужным объектом с учетом выбранных областей видимости (в третьем параметре их может быть несколько);
{
    if(this->currNOD==NULL) return -1;
    qDebug() << endl << "Searching targets of links" << NAME << "from scope" << this->currNOD->getName()
             << "TYPES:" << (int)TYPE << "AREA:" << (int)AREA;
    /*
      UPPER_AREA =0x3,  //всё что выше его и имеет общих с ним предков (дети его родителей, дети родителей его родителей и т.п.)
      PARENTS_AREA = 0x2, //любой из его родителей (в т.ч. и родителей родителей), на любой высоте от него
      SAME_AREA = 0x4, // все объекты, имеющие того же непосредственного родителя, т.е. прямые потомки только непостредственного родителя (расстояние =1)
      SIBLINGS = 0x8, // только прямые потомки этого объекта (расстояние =1)
      CHILDREN = 0xA // все дальние и близкие дети этого объекта на любом расстоянии
      */

    sNode * result=NULL;
    if( UPPER_AREA& AREA )  // смотрим все верхние связи
    {
        qDebug() << "Upper area look";
        sNode *temp=this->currNOD;
        while(1)
        {
            qDebug() << "From:" << temp->getId();
            temp=this->treeUP[temp->getId()];
            if(temp==NULL)
            {
                break;
                qDebug()<<"Null node";
            }
            qDebug() << "Got:" << temp->getId();
            qDebug() << "Comparing" << NAME << "with" << temp->getName();
            if(temp->getName().compare(NAME)==0 && (temp->getType()&TYPE) )
            {
                qDebug() << "matched!";
                result=temp;
            }


            QList<sNode *> lst( this->treeObj[temp->getId()]);
            qDebug() << "Iterating childrens";
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                qDebug() << "Comparing" << NAME << "with" << (*it)->getName();
                if((*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
                {
                    qDebug() << "matched!";
                    result=(*it);
                    break;
                }
            }
            qDebug() << "Next iteration";
        }

    }

    if( (PARENTS_AREA& AREA) && result==NULL)
    {
        qDebug() << "Parent area look";
        sNode *temp=this->currNOD;
        while(1)
        {
            temp=this->treeUP[temp->getId()];
            if(temp==NULL)break;
            if(temp->getName().compare(NAME)==0 && (temp->getType()&TYPE) )
            {
                result=temp;
                break;
            }
        }
    }
    if( (SAME_AREA& AREA) && result==NULL)
    {
        qDebug() << "Same area look";
        sNode *temp=this->currNOD;
        temp=this->treeUP[temp->getId()];
        if(temp!=NULL)
        {
            QList<sNode *> lst( this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                if( (*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
                {
                    result=(*it);
                    break;
                }
            }
        }
    }
    if( (SIBLINGS& AREA) && result ==NULL)
    {
        qDebug() << "Siblings area look";
        sNode *temp=this->currNOD;
        temp=this->treeUP[temp->getId()];
        QList<sNode *> lst( this->treeObj[temp->getId()]);
        for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
        {
            if((*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
            {
                result=(*it);
                break;
            }
        }
    }
    if( (CHILDREN& AREA) && result==NULL)
    {
        qDebug() << "Children area look";
        sNode * temp=this->currNOD;
        QStack<sNode *> stck;
        stck.push(temp);
        while(!stck.isEmpty())
        {
            temp=stck.pop();
            QList<sNode*> lst(this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                stck.push(*it);
                if( (*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
                {
                    result=(*it);
                    break;
                }
            }
        }
    }
    if(result==NULL)return -1;
    this->Links.push_back( QPair<sNode *,sNode*>(this->currNOD,result));
    return 1;
}

HRET OLTManager::linkSiblingScope( QString NAME,QString NAME2, eScopeType TYPE, eVisionArea AREA) // все тоже самое, только в первой строке указывается имя непосредственного наследника текущего объекта который вызывает,  и имя объекта (тоже наследника которого вызывают)
{
    if(this->currNOD==NULL) return -1;

    /*
      UPPER_AREA =0x3,  //всё что выше его и имеет общих с ним предков (дети его родителей, дети родителей его родителей и т.п.)
      PARENTS_AREA = 0x2, //любой из его родителей (в т.ч. и родителей родителей), на любой высоте от него
      SAME_AREA = 0x4, // все объекты, имеющие того же непосредственного родителя, т.е. прямые потомки только непостредственного родителя (расстояние =1)
      SIBLINGS = 0x8, // только прямые потомки этого объекта (расстояние =1)
      CHILDREN = 0xA // все дальние и близкие дети этого объекта на любом расстоянии
      */

    sNode * result=NULL;
    if( UPPER_AREA& AREA )  // смотрим все верхние связи
    {
        sNode *temp=this->currNOD;
        while(1)
        {

            temp=this->treeUP[temp->getId()];
            if(temp==NULL)break;
            if(temp->getName().compare(NAME)==0 && (temp->getType()&TYPE) )
            {
                result=temp;
            }
            QList<sNode *> lst( this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                if((*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
                {
                    result=(*it);
                    break;
                }
            }
        }
    }

    if( (PARENTS_AREA& AREA) && result==NULL)
    {
        sNode *temp=this->currNOD;
        while(1)
        {
            temp=this->treeUP[temp->getId()];
            if(temp==NULL)break;
            if(temp->getName().compare(NAME)==0 && (temp->getType()&TYPE) )
            {
                result=temp;
                break;
            }
        }
    }
    if( (SAME_AREA& AREA) && result==NULL)
    {
        sNode *temp=this->currNOD;
        temp=this->treeUP[temp->getId()];
        if(temp!=NULL)
        {
            QList<sNode *> lst( this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                if( (*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
                {
                    result=(*it);
                    break;
                }
            }
        }
    }
    if( (SIBLINGS& AREA) && result ==NULL)
    {
        sNode *temp=this->currNOD;
        temp=this->treeUP[temp->getId()];
        QList<sNode *> lst( this->treeObj[temp->getId()]);
        for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
        {
            if((*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
            {
                result=(*it);
                break;
            }
        }
    }
    if( (CHILDREN& AREA) && result==NULL)
    {
        sNode * temp=this->currNOD;
        QStack<sNode *> stck;
        stck.push(temp);
        while(!stck.isEmpty())
        {
            temp=stck.pop();

            QList<sNode*> lst(this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                stck.push(*it);
                if( (*it)->getName().compare(NAME)==0 && ((*it)->getType()&TYPE) )
                {
                    result=(*it);
                    break;
                }
            }
        }
    }

//==================================


    sNode * result2=NULL;
    if( UPPER_AREA& AREA )  // смотрим все верхние связи
    {
        sNode *temp=this->currNOD;
        while(1)
        {

            temp=this->treeUP[temp->getId()];
            if(temp==NULL)break;
            if(temp->getName().compare(NAME2)==0 && (temp->getType()&TYPE) )
            {
                result2=temp;
            }
            QList<sNode *> lst( this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                if((*it)->getName().compare(NAME2)==0 && ((*it)->getType()&TYPE) )
                {
                    result2=(*it);
                    break;
                }
            }
        }
    }

    if( (PARENTS_AREA& AREA) && result2==NULL)
    {
        sNode *temp=this->currNOD;
        while(1)
        {
            temp=this->treeUP[temp->getId()];
            if(temp==NULL)break;
            if(temp->getName().compare(NAME2)==0 && (temp->getType()&TYPE) )
            {
                result2=temp;
                break;
            }
        }
    }
    if( (SAME_AREA& AREA) && result2==NULL)
    {
        sNode *temp=this->currNOD;
        temp=this->treeUP[temp->getId()];
        if(temp!=NULL)
        {
            QList<sNode *> lst( this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                if( (*it)->getName().compare(NAME2)==0 && ((*it)->getType()&TYPE) )
                {
                    result2=(*it);
                    break;
                }
            }
        }
    }
    if( (SIBLINGS& AREA) && result2 ==NULL)
    {
        sNode *temp=this->currNOD;
        temp=this->treeUP[temp->getId()];
        QList<sNode *> lst( this->treeObj[temp->getId()]);
        for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
        {
            if((*it)->getName().compare(NAME2)==0 && ((*it)->getType()&TYPE) )
            {
                result2=(*it);
                break;
            }
        }
    }
    if( (CHILDREN& AREA) && result2==NULL)
    {
        sNode * temp=this->currNOD;
        QStack<sNode *> stck;
        stck.push(temp);
        while(!stck.isEmpty())
        {
            temp=stck.pop();

            QList<sNode*> lst(this->treeObj[temp->getId()]);
            for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
            {
                stck.push(*it);
                if( (*it)->getName().compare(NAME2)==0 && ((*it)->getType()&TYPE) )
                {
                    result2=(*it);
                    break;
                }
            }
        }
    }

    if(result==NULL)return -1;
    if(result2==NULL)return -1;
    this->Links.push_back( QPair<sNode *,sNode*>(result,result2));
    return 1;
}



HRET OLTManager::print_tree2(sNode& a)
{
    cout<<a.toQString().toStdString()<<endl;
    QList<sNode *> lst(this->treeObj[a.getId()]);
    for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
    {
        print_tree2(**it);
    }
}

HRET OLTManager::go_tree(WORK_ON_SCOPE func,sNode& a)
{
    if(this->rootNOD==NULL)
    {
        cerr<<"rootNOD==NULL!!!"<<endl;
        return -1;
    }
    QList<sNode *> lst(this->treeObj[a.getId()]);
    func(a);
    for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
    {
        go_tree(func,**it);
    }
    return 1;
}

HRET OLTManager::go_tree2(WORK_ON_SCOPE_TREE func,sNode& a)
{
    static int num=0;
    num++;
    // cout<<"vhod="<<num<<endl;
    if(this->rootNOD==NULL)
    {
        cerr<<"rootNOD==NULL!!!"<<endl;
        return -1;
    }
    QList<sNode *> lst(this->treeObj[a.getId()]);
    func(a,*this);
    for(QList<sNode *>::iterator it=lst.begin(); it!=lst.end(); ++it)
    {
        go_tree2(func,**it);
    }
    // cout<<"vihod="<<num<<endl;
    return 1;
}

//продвинутые методы для работы с объектами
HRET OLTManager::setScope( QStringList& )//Сделать текущим нужный объект от начала скоупа  по указанному пути (путь хранится в виде упорядоченного списка строк)
{
    if(this->currNOD==NULL) return -1;
    return 1;
}
//  вот пример пути: "Родитель1-> Родитель2-> Родитель3-> Сын 4" (сделали текущим объект на 4 уровня в глубине)
HRET OLTManager::setFile (QString FILE) // все выдаваемые объекты будут принадлежать такому-то файлу, если его нет - добавляется в структуру, если уже есть - используется уже существующая копия
{

    //this->currNOD->setFileID(addFile(FILE));
    addFile(FILE);
    if(this->currNOD==NULL) return -1;
    this->currNOD->setFile(FILE);
    return 1;
}

HRET OLTManager::commitData() //сохраняет в БД всю инфу из все базы данных
{
    if(this->currNOD==NULL) return -1;
    /*if(!db_connect(1)) // подсоединимся к базе
    {
        cerr<<"can not connect to data base massage number = 112233112233 ."<<endl;
        return -1;
    }*/
    if(!db_begin("commitData in libdb_interface")) //< Запуск новой транзакции
    {
        cerr<<"can not start transaction no data base massage number = 112233112233 ."<<endl;
        return -1;
    }

    this->go_tree2(&(commitNode),*(this->getRoot()) ); //коммитим дерево
    this->commitLink(); // коммитим линки

    if(!db_commit("commitData in libdb_interface")) //< коммит транзакции
    {
        cerr<<"can not commit transaction no data base massage number = 112233112312 ."<<endl;
        return -1;
    }
    // db_dconnect();
    return 1;
}

void OLTManager::commitLink()
{
    cerr << "Commiting links..." << endl;
    for(QList<QPair<sNode*,sNode*> >::iterator it=this->Links.begin(); it!=this->Links.end(); ++it)
    {
        cerr << "Link: " << it->first->data.id << " " << it->second->data.id << endl;
        create_link ( it->first->data.id,  it->second->data.id);
    }
}

void OLTManager::commitNode(sNode& node, OLTManager &p)
{
    //sScope scope(node.data.name, node.data.system_type, /*node.data.parent_id*/);

    int got_id= create_scope(node.data.name,node.data.system_type,node.data.parent_id,"",0);
    set_scope_full_data( got_id, node.getFileID(), node.data.pos_x1, node.data.pos_y1, node.data.pos_x2, node.data.pos_y2);

    for(QMap<QString,QString>::iterator it=node.metadata.begin(); it!=node.metadata.end(); ++it)
    {
        QString k=it.key();
        QString v=it.value();
        set_scope_metadata( got_id, k.toStdString(), v.toStdString());
    }
    for(QMap<QString,int>::iterator it=node.metadata_int.begin(); it!=node.metadata_int.end(); ++it)
    {
        QString k=it.key();
        int v=it.value();
        set_scope_metadata( got_id, k.toStdString(), v);
    }

    //cout<<"create_scope id="<<got_id<<" name="<<node.data.name<<" parent="<<node.data.parent_id<<endl;
    node.data.id=got_id;
    QList<sNode*> lst=p.treeObj[node.getId()];
    for(QList<sNode*>::iterator it=lst.begin(); it!=lst.end(); ++it)
    {
        (*it)->data.parent_id=got_id;
    }
    /*sScope program("program",EXE_SCOPE),
                code("code program",CODE_SCOPE,&program),
                ifblock("if block",CODE_IF,&program),
                code_if_true("block when true",CODE_SCOPE,&ifblock),
                code_if_false("block when false",CODE_SCOPE,&ifblock),
                code_cycle("cycle",CODE_CYCLE_FOR,&program),
                code2("code",CODE_SCOPE,&code_cycle);*/

}



OLTManager::~OLTManager()
{

}


HRET OLTManager::setLineSource(int* p)
{
    Q_CHECK_PTR ( p );
    if(p) curLine = (uint*)p;
}


HRET OLTManager::setMetaStr(QString name, QString value, eObjectRef ref) //Сохранить метаданные строки
{
    return S_OKEY;
}
void OLTManager::Up()
{
    this->currNOD=this->treeUP[this->currNOD->getId()];
}
void OLTManager::SetMetadata(QString &key, QString &value)
{
    this->currNOD->metadata[key]=value;
}
void OLTManager::SetMetadata(QString &key, int value)
{
    this->currNOD->metadata_int[key]=value;
}
HRET OLTManager::setMetaInt(QString name, int value, eObjectRef ref) //Сохранить интовые метаданные
{
    return S_OKEY;
}



eScopeType StringToType(QString str)
{
    if(str.compare("EXE_SCOPE") ) return EXE_SCOPE;
    if(str.compare("INFO_SCOPE") ) return INFO_SCOPE;
    if(str.compare("NS_SCOPE") ) return NS_SCOPE;
    if(str.compare("MODULE_SCOPE") ) return MODULE_SCOPE;
    if(str.compare("CODE_BLOCK") ) return CODE_BLOCK;
    if(str.compare("CODE_CODE") ) return CODE_CODE;
    if(str.compare("CODE_IF") ) return CODE_IF;
    if(str.compare("CODE_TRUE") ) return CODE_TRUE;
    if(str.compare("CODE_FALSE") ) return CODE_FALSE;
    if(str.compare("CODE_SWITCH") ) return CODE_SWITCH;
    if(str.compare("CODE_SWITCH_VALUE") ) return CODE_SWITCH_VALUE;
    if(str.compare("CODE_CYCLE_FOR") ) return CODE_CYCLE_FOR;
    if(str.compare("CODE_CYCLE_DO") ) return CODE_CYCLE_DO;
    if(str.compare("CODE_CYCLE_DO_WHILE") ) return CODE_CYCLE_DO_WHILE;
    if(str.compare("CODE_CYCLE_WHILE") ) return CODE_CYCLE_WHILE;
    if(str.compare("CODE_FUNC") ) return CODE_FUNC;
    if(str.compare("CODE_BREAK") ) return CODE_BREAK;
    if(str.compare("CODE_CONTINUE") ) return CODE_CONTINUE;
    if(str.compare("CODE_RETURN") ) return CODE_RETURN;
    if(str.compare("CODE_GOTO") ) return CODE_GOTO;
    if(str.compare("CODE_END") ) return CODE_END;
    if(str.compare("SYSTEM_SCOPE_FLAG") ) return SYSTEM_SCOPE_FLAG;
    if(str.compare("OBJECT_FLAG") ) return OBJECT_FLAG;
    return CODE_CODE;
}
