#include "oltmanager.h"

int OLTManager::indexOfFile(sFile file)
{
    for(int i = 0; i < fList.count(); i++)
        if(fList[i].id==file.id)
            return i;
    return -1;

}

ID_FILE OLTManager::addFile(QString file)//�������� ���� � ���������
{
    //curFile=ak_vs_db::insert_file(file.toStdString()) ;
    return curFile;

}

sFile OLTManager::getFromIndex(int index)//�������� ���� �� ��� �������
{
    if(index > 0 && index < fList.count())
    {
        return fList[index];
    }
}
