#ifndef OLTDEBUGMANAGER_H
#define OLTDEBUGMANAGER_H

#include "olt_xmlmanager.h"

class OLTDebugManager : OLTXMLManager
{
public:
    OLTDebugManager();
    ~OLTDebugManager();
    virtual HRET createScope(QString&, eScopeType); //���� ��� ���� ����� - �� ���������
    virtual HRET enterScope(QString&, eScopeType); // ����� - ����� ����, � �� ��� ��� - ������� ��������
    virtual HRET forceScope(QString&, eScopeType); // �������� ��������������� createS, enterS
    virtual HRET goUp(eScopeType); // ���������� ������, ���� �� ������� ������ ��������������� ������� ����� (��������� & ), ���� ��� ���, �� ������ �������� �� �����, � �� ������ -1

    virtual HRET linkScope(QString&, eLinkType, eVisionArea); //������� ������� � ������ �������� � ������ ��������� �������� ��������� (� ������� ��������� �� ����� ���� ���������);
    virtual HRET linkSiblingScope( QString& ,QString&, eLinkType, eVisionArea); // ��� ���� �����, ������ � ������ ������ ����������� ��� ����������������� ���������� �������� ������� ������� ��������,  � ��� ������� (���� ���������� �������� ��������)

    //����������� ������ ��� ������ � ���������
    virtual HRET setScope( QStringList& );//������� ������� ������ ������ �� ������ ������  �� ���������� ���� (���� �������� � ���� �������������� ������ �����)
    //  ��� ������ ����: "��������1-> ��������2-> ��������3-> ��� 4" (������� ������� ������ �� 4 ������ � �������)

    virtual HRET setFile (QString&); // ��� ���������� ������� ����� ������������ ������-�� �����, ���� ��� ��� - ����������� � ���������, ���� ��� ���� - ������������ ��� ������������ �����
    virtual HRET setMetaStr(QString name, QString value, eObjectRef ref=CURRENT_REF); //��������� ���������� ������
    virtual HRET setMetaInt(QString name, int value, eObjectRef ref=CURRENT_REF); //��������� ������� ����������

};

#endif // OLTDEBUGMANAGER_H
