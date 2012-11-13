#ifndef SERVICESPACE_H
#define SERVICESPACE_H

#include <string>

namespace servicespace
{
/**
  * Проверяет, допустимый ли ход.
  * Например, чтобы задать ход e2-e4, напишите isValidMove(5,2,5,4);
  * @param type Тип объекта
  * @param findex Индекс(номер) файла
  * @param pindex   Индекс датчика
  * @param mindex   Индекс объекта (глобальный)
  * @param index   Горизонталь клетки, на которую выполняется ход (1...8)
  * @param id   Номер датчика = 15000+индекс датчика
  * @param line   Номер строки
  * @param name   Имя функции
  */

extern const char *Types[];
void aist_probe_log(int type, int findex, int pindex, int mindex, int index, int id, int line, std::string name);


};

#endif // SERVICESPACE_H
