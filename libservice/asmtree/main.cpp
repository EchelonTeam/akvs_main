#include <QtCore/QCoreApplication>
#include <list>
#include <map>
#include <string>
using namespace std;

class AsmNode
{
public :
    AsmNode() {}
    string NAME;
    string TYPE;
    string FILE_NAME;
};

class AsmTree
{

};



class AsmTree
{

public :
    AsmTree() {}
    AsmTree(AsmNode curr)
    {
        this->curr=curr;
    }
    //������
    map<AsmNode,list<AsmNode> > tree;

    //���������� ������� ��������.
    bool SetCurrent( AsmNode node)
    {
        if(tree[node].size()!=0)curr=node;
        else curr=NULL;

    }

    //�������� ���� � ��������
    bool AddElement( AsmNode node)
    {
        if(curr==NULL) return false;
        tree[this->curr].push_back(node);
        return true;
    }
    AsmNode FindElement(string where,string TYPE, string XXX)
    {
        if(where.compare("UPP")==0) {}
        if(where.compare("PARRENT")==0) {}
        if(where.compare("")==0) {}
        if(where.compare("")==0) {}
    }

private:
    //������� �������
    AsmNode curr;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    map<AsmNode ,list<AsmNode> > mscope;

    for(map::iterator it=mscope.begin(); it!=mscope.end(); it++)
    {

    }


    return a.exec();
}
