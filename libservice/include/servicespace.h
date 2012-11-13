#ifndef SERVICESPACE_H
#define SERVICESPACE_H

#include <string>

namespace servicespace
{
/**
  * ���������, ���������� �� ���.
  * ��������, ����� ������ ��� e2-e4, �������� isValidMove(5,2,5,4);
  * @param type ��� �������
  * @param findex ������(�����) �����
  * @param pindex   ������ �������
  * @param mindex   ������ ������� (����������)
  * @param index   ����������� ������, �� ������� ����������� ��� (1...8)
  * @param id   ����� ������� = 15000+������ �������
  * @param line   ����� ������
  * @param name   ��� �������
  */

extern const char *Types[];
void aist_probe_log(int type, int findex, int pindex, int mindex, int index, int id, int line, std::string name);


};

#endif // SERVICESPACE_H
