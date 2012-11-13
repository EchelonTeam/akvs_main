#include "oltdebugmanager.h"

#include <iostream>
#include <QDebug>

QString my_ident;

OLTDebugManager::OLTDebugManager()
{
    qDebug() << QObject::tr("Creating OLTManager");
}


OLTDebugManager::~OLTDebugManager()
{
    qDebug() << QObject::tr("Finishing OLTManager");
}

HRET OLTDebugManager::createScope(QString& name, eScopeType type) //���� ��� ���� ����� - �� ���������
{
    my_ident += QString("\t");
    qDebug() << my_ident << QObject::tr("Creating scope ") << name << " type: " << type;
    return S_OKEY;
}

HRET OLTDebugManager::enterScope(QString& name, eScopeType mask) // ����� - ����� ����, � �� ��� ��� - ������� ��������
{
    qDebug() << my_ident << "Entering " << name << " mask: " << mask;
    return S_OKEY;
}

HRET OLTDebugManager::forceScope(QString& name, eScopeType type) // �������� ��������������� createS, enterS
{
    createScope(name,type);
    enterScope(name,type);
    return S_OKEY;
}
HRET OLTDebugManager::goUp(eScopeType mask) // ���������� ������, ���� �� ������� ������ ��������������� ������� ����� (��������� & ), ���� ��� ���, �� ������ �������� �� �����, � �� ������ -1
{
    qDebug () << "goUp till " << mask;
    return S_OKEY;
}
HRET OLTDebugManager::linkScope(QString& link_a, eLinkType type, eVisionArea vision) //������� ������� � ������ �������� � ������ ��������� �������� ��������� (� ������� ��������� �� ����� ���� ���������);
{
    qDebug () << "Linking " << link_a << " type:" << type << " with vision " << vision;
    return S_OKEY;
}

HRET OLTDebugManager::linkSiblingScope( QString& link_a,QString& link_b, eLinkType type, eVisionArea vision) // ��� ���� �����, ������ � ������ ������ ����������� ��� ����������������� ���������� �������� ������� ������� ��������,  � ��� ������� (���� ���������� �������� ��������)
{
    qDebug () << "Linking " << link_a << " with " << link_b << " type:" << type << " with vision " << vision;
    return S_OKEY;
}
//����������� ������ ��� ������ � ���������

HRET OLTDebugManager::setScope( QStringList& list)//������� ������� ������ ������ �� ������ ������  �� ���������� ���� (���� �������� � ���� �������������� ������ �����)
//  ��� ������ ����: "��������1-> ��������2-> ��������3-> ��� 4" (������� ������� ������ �� 4 ������ � �������)
{
    cerr << "Setting";
    foreach(QString str, list)
    {
        cerr << str.toStdString() << ":";
    }
    return S_OKEY;
}

HRET OLTDebugManager::setFile (QString& filename) // ��� ���������� ������� ����� ������������ ������-�� �����, ���� ��� ��� - ����������� � ���������, ���� ��� ���� - ������������ ��� ������������ �����
{
    qDebug() << "current File: "<<filename;
    return S_OKEY;
}


HRET OLTDebugManager::setMetaStr(QString name, QString value, eObjectRef ref) //��������� ���������� ������
{
    return S_OKEY;
}

HRET OLTDebugManager::setMetaInt(QString name, int value, eObjectRef ref) //��������� ������� ����������
{
    return S_OKEY;
}
