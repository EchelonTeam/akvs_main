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
#define BLOCK_X 60    //������ ����� �� x
#define BLOCK_Y 30    //������ ����� �� y
#define BLOCK_D 50    //��� ������� ����������
#define LENTH_RIGHT 7 //���������� ����� �������� ������� ������
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
    sScope data; //���� � ������
    vec_pTree a; //����
    int d; // ������ � ����.
    pTree parent;
    ~Tree()
    {
        for( vec_pTree::iterator it= this->a.begin(); it!= this->a.end(); ++it )
        {
            delete ( *it);
        }
    }
};

//���� ��� ��������� �����
class SBlock
{
public :
    int x;			// x �� QImage
    int y;			// y �� QImage
    int d;			// ����������� �����.
    int type; 		// ��� ����� ��� ��������� ����������� ������ eBLOCK
    int number;		// ����� � �������
    int id;			// id �����
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
    int type;		//��� �����
    int d;			//����� ����������
    int number_out;         //����� ����� ����������
    int number_in;          //����� ����� � ��������
    int id_out;		//id ����� �� ��������
    int id_in;		//id ����� � ��������

    eScopeType type_out;	//��� ����� �� �������� �����.
    eScopeType type_in;	//��� ����� � �������� �����

    int x_out;		//x ����� ������
    int y_out;		//y ����� ������
    int x_in;		//x ����� �����
    int y_in;		//y ����� �����
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
