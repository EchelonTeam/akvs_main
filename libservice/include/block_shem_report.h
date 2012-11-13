/*
 * block_shem_report.h
 *
 *  Created on: 30.10.2008
 *      Author: javamain
 */
#ifndef BLOCK_SHEM_REPORT_H_
#define BLOCK_SHEM_REPORT_H_
#include <QPainter>
#include <sqladapter.h>
#define BLOCK_X 60    //размер блока по x
#define BLOCK_Y 30    //размер блока по y
#define BLOCK_D 50    //меж блочное расстояние
#define LENTH_RIGHT 7 //расстояние между уровнями стрелок справо
using namespace std;
class SBlock;
class SLink;
class Tree;
class xyString;

typedef vector< Tree*> vec_pTree;
typedef Tree* pTree;
typedef Tree & rTree;
void TestFunc();

class Tree
{
public:
    Tree() : d( 0 ) {}
    sScope data; //инфа в дереве
    vec_pTree a; //дети
    int d; // размер с низу.
    pTree parent;
    ~Tree()
    {
        for( vec_pTree::iterator it= this->a.begin(); it!= this->a.end(); ++it )
        {
            delete ( *it);
        }
    }
};

//блок для отрисовки схемы
class SBlock
{
public :
    int x;			// x на QImage
    int y;			// y на QImage
    int d;			// вложенность блока.
    int type; 		// тип блока для отрисовки стандартных блоков eBLOCK
    int number;		// номер в функции
    int id;			// id блока
};
class xyString
{
public:
    int x;
    int y;
    string str;
};
class SLink
{
public :
    int type;		//тип связи
    int d;			//номер траэктории
    int number_out;         //номер блока откоторого
    int number_in;          //номер блока к которому
    int id_out;		//id блока от которого
    int id_in;		//id блока к которому

    eScopeType type_out;	//тип блока от которого ведем.
    eScopeType type_in;	//тип блока к которому ведем

    int x_out;		//x блока выхода
    int y_out;		//y блока выхода
    int x_in;		//x блока входа
    int y_in;		//y блока входа
};
enum eLINK
{
    DOWN = 1,
    RIGHT_DOWN = 2,
    LEFT_DOWN = 3,
    DOWN_LEFT = 4,
    LEFT_UP = 5,
    LEFT_UP_LEFT = 6
};
vec_pTree & alltree( rTree a, vec_pTree &v);
rTree AnalizeCONTINUE( rTree a);
int AnalizeTree( rTree a, vec_pTree & block_vec, list< SLink>& link_vec);
inline QPainter & drawTRY( int x, int y, QPainter &pt, int id);
inline QPainter & drawCATCH( int x, int y, QPainter &pt, int id);
inline QPainter & draw_Begin( int x, int y, QPainter &pt, int id);
inline QPainter & draw_Cycl( int x, int y, QPainter &pt, int id);
inline QPainter & draw_END( int x, int y, QPainter &pt, int id);
inline QPainter & drawBreak( int x, int y, QPainter &pt, int id);
inline QPainter & drawcase( int x, int y, QPainter &pt, int id);
inline QPainter & drawCODE( int x, int y, QPainter &pt, int id);
inline QPainter & drawcontinue( int x, int y, QPainter &pt, int id);
inline QPainter & drawDO( int x, int y, QPainter &pt, int id);
inline QPainter & drawDO_WHILE( int x, int y, QPainter &pt, int id);
inline QPainter & drawelse( int x, int y, QPainter &pt, int id);
inline QPainter & drawGOTO( int x, int y, QPainter &pt, int id);
inline QPainter & drawIF_ROMB( int x, int y, QPainter &pt, int id);
inline QPainter & drawLink_down( int x, int y, QPainter &pt);
inline QPainter & drawLink_Down_left( int x_out, int y_out, int x_in, int y_in, int d, QPainter &pt);
inline QPainter & drawLink_Down_right( int x_out, int y_out, int x_in, int y_in, int d, QPainter &pt);
inline QPainter & drawLink_Up_Left( int x_out, int y_out, int x_in, int y_in, int d, QPainter &pt);
inline QPainter & drawLink_Left_Up_Left( int x_out, int y_out, int x_in, int y_in, int d, QPainter &pt);
inline QPainter & drawreturn( int x, int y, QPainter &pt, int id);
inline QPainter & drawSwitch( int x, int y, QPainter &pt, int id);
inline QPainter & drawthen( int x, int y, QPainter &pt, int id);
sScopeFull findfather( sScopeFull &a, sScopeFull &b);
rTree findFather( rTree a, rTree b);
sScopeFull findfirst( sScopeFull a);
rTree findlast(rTree a);
sScopeFull findlast( sScopeFull a);
inline vec_pTree & findlasttoconnectDOWN( rTree a);
rTree fromBaseToTree( rTree c, sScope& b);
//Tree& fromBaseToTree2( Tree &c, sScope& b);
vec_pTree & getlatest( rTree a, vec_pTree &b);
vector< sScopeFull> getlatest( sScopeFull a, vector< sScopeFull> &b);
sScope & getnextperent( sScope& a);
int MakeBLokShemReport( vector< xyString>& xystr, vector< SBlock> vb, list< SLink> vl, string file_name, string func_file, string func_name, int bigd);
void printTree( rTree a, string  s);
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
HRET Trace_Block_Shem( sReportInfo* pa);/////////
//}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}}
vec_pTree & treetoline( rTree a, vec_pTree & v);
void wighttree( rTree a);
void wighttree2( rTree a);
pTree findfirstNotBlock( pTree a);
pTree delsomblocksTree( pTree t);


#endif /* BLOCK_SHEM_REPORT_H_ */
