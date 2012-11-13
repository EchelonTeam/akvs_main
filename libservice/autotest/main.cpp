#include <QtCore/QCoreApplication>

#include "oltmanager.h"
#include "common.h"
#include <iostream>
#include <string>

using namespace std;

using namespace ak_vs_db;
void print_node(sNode & a)
{
    cout<<"start printing!!!"<<endl;
    cout<<a.toQString().toStdString()<<endl;
    /*   cout<<"[";
       cout<<" name="<<a.getName().toStdString();
       cout<<" id="<<QString::number((int)a.getId()).toStdString();

       cout<<" type="<<QString::number((int)a.getType()).toStdString();
       cout<<" file="<<a.getFile().toStdString();
       cout<<"]"<<endl;*/
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //соединяемся с базой
    db_connect (1);
    //выбираем кластер
    //db_set_current_cluster(0);
    //сделать список линков
    OLTManager tree;
    //root -овский элемент
    tree.createScope(*(new QString("root")),EXE_SCOPE);

    //tree.createScope(sNode(QString("root"),EXE_SCOPE,QString("1.txt")));
    tree.createScope(*(new QString("123")),EXE_SCOPE);
    tree.createScope(*(new QString("sdfsdf")),EXE_SCOPE);
    tree.createScope(*(new QString("45345")),EXE_SCOPE);
    tree.createScope(*(new QString("==-")),EXE_SCOPE);
    if(tree.enterScope(QString("123"),EXE_SCOPE)==1)
    {
        tree.createScope(*(new QString("123")),EXE_SCOPE);
        tree.createScope(*(new QString("124")),EXE_SCOPE);
        tree.createScope(*(new QString("sdfsdf")),EXE_SCOPE);
        tree.createScope(*(new QString("45345")),EXE_SCOPE);
        tree.createScope(*(new QString("==-")),EXE_SCOPE);
        if(tree.enterScope(QString("123"),EXE_SCOPE)==1)
        {
            //l
            tree.linkScope(QString("123"),EXE_SCOPE,UPPER_AREA);
            //l
            tree.linkScope(QString("124"),EXE_SCOPE,SAME_AREA);

            tree.createScope(*(new QString("123")),EXE_SCOPE);
            tree.createScope(*(new QString("sdfsdf")),CODE_BLOCK);
            tree.createScope(*(new QString("45345")),EXE_SCOPE);
            tree.createScope(*(new QString("==-")),EXE_SCOPE);
            //l
            tree.linkScope(QString("123"),EXE_SCOPE,SIBLINGS);

            if(tree.enterScope(QString("123"),EXE_SCOPE)==1)
            {

                tree.createScope(*(new QString("123")),EXE_SCOPE);
                //l
                tree.linkScope(QString("123"),EXE_SCOPE,CHILDREN);
            }
        }

    }

    // мы распечатываем дерево
    tree.go_tree(&print_node, *(tree.getRoot()) );

    cout<<"start commit!!!"<<endl;
    tree.commitData();
    cout<<"end commit!!!"<<endl;

    cout<<"end"<<endl;



    db_set_current_cluster(NULL);
    db_dconnect();
    return 1;
}
