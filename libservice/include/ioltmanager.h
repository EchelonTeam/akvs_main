#ifndef IOLTMANAGER_H
#define IOLTMANAGER_H

#include <common.h>

#include <QString>
#include <QStringList>

enum eVisionArea
{
    UPPER_AREA =0x1,  //�� ��� ���� ��� � ����� ����� � ��� ������� (���� ��� ���������, ���� ��������� ��� ��������� � �.�.)
    PARENTS_AREA = 0x2, //����� �� ��� ��������� (� �.�. � ��������� ���������), �� ����� ������ �� ����
    SAME_AREA = 0x4, // ��� �������, ������� ���� �� ����������������� ��������, �.�. ������ ������� ������ ������������������ �������� (���������� =1)
    SIBLINGS = 0x8, // ������ ������ ������� ����� ������� (���������� =1)
    CHILDREN = 0x10 // ��� ������� � ������� ���� ����� ������� �� ����� ����������
};


enum eObjectRef
{
    CURRENT_REF=0x1, //������� �����
    LAST_INSERTION_REF=0x2 //��������� ����������� � ���� ������ ������

};

class IOLTManager
{
public:

public:

    //������� ������ ��� ������ � ���������
    virtual HRET createScope(QString, eScopeType)=0; //���� ��� ���� ����� - �� ���������
    virtual HRET enterScope(QString, eScopeType)=0; // ����� - ����� ����, � �� ��� ��� - ������� ��������
    virtual HRET forceScope(QString, eScopeType)=0; // �������� ��������������� createS, enterS
    virtual HRET finishScopeUp(eScopeType)=0; // ���������� ������, ���� �� ������� ������ ��������������� ������� ����� (��������� & ), ���� ��� ���, �� ������ �������� �� �����, � �� ������ -1

    virtual HRET linkScope(QString, eScopeType, eVisionArea)=0; //������� ������� � ������ �������� � ������ ��������� �������� ��������� (� ������� ��������� �� ����� ���� ���������);
    virtual HRET linkSiblingScope( QString ,QString, eScopeType, eVisionArea)=0; // ��� ���� �����, ������ � ������ ������ ����������� ��� ����������������� ���������� �������� ������� ������� ��������,  � ��� ������� (���� ���������� �������� ��������)

    //����������� ������ ��� ������ � ���������
    virtual HRET setScope( QStringList& )=0;//������� ������� ������ ������ �� ������ ������  �� ���������� ���� (���� �������� � ���� �������������� ������ �����)
    //  ��� ������ ����: "��������1-> ��������2-> ��������3-> ��� 4" (������� ������� ������ �� 4 ������ � �������)

    virtual HRET setFile (QString)=0; // ��� ���������� ������� ����� ������������ ������-�� �����, ���� ��� ��� - ����������� � ���������, ���� ��� ���� - ������������ ��� ������������ �����

    virtual HRET setMetaStr(QString name, QString value, eObjectRef ref)=0; //��������� ���������� ������ =CURRENT_REF
    virtual HRET setMetaInt(QString name, int value, eObjectRef ref)=0; //��������� ������� ���������� =CURRENT_REF


    virtual HRET commitData()=0; //��������� � �� ��� ���� �� ��� ���� ������

};

#endif // IOLTMANAGER_H
