#pragma once
#include <assert.h>
#include <vector>
class CBase;

using namespace std;

/****************************************************/
/*	IDFlag											*/
/****************************************************/
#define NULL_SPACE					'0'

#define MYSELF_WHITE_TANK			'1'		//■①②③⑴⑵⑶
#define FRIEND_WITHE_TANK			'2'
#define ENEMY_RED_TANK				'3'
#define ENEMY_YELLOW_TANK			'4'
#define ENEMY_GREEN_TANK			'5'

#define MYSELF_WHITE_BULLET			'6'		//＠
#define FRIEND_WITHE_BULLET			'7'
#define ENEMY_RED_BULLET			'8'
#define ENEMY_YELLOW_BULLET			'9'
#define ENEMY_GREEN_BULLET			'a'

#define STEEL_WHITE_BRICK			'b'		//■
#define REVER_BLUE_BRICK			'c'		//≈	河流
#define NORMAL_PURPLE_BRICK			'd'		//▓

#define THE_RED_KING	'e'		//王

//上面的IDFlag可以做碰撞码,下面是扩展的**************
#define COLLISIONCODE_OUTSIDE		'f'		//这是一个碰撞码,子弹碰撞检测时用到;
#define COLLISIONCODE_NOCOLLISION	'0'

/****************************************************/
/*	常用颜色定义									*/
/****************************************************/

// 字体颜色
#define RED    0x0004|0x0008        // 亮红
#define YELLOW 0x0002|0x0004|0x0008 // 亮黄
#define GREEN  0x0002|0x0008        // 亮绿
#define CYAN   0x0002|0x0004|0x0008 // 亮青
#define BLUE   0x0001|0x0008        // 亮蓝
#define PURPLE 0x0001|0x0004|0x0008 // 亮紫
#define WHITE  0x0004|0x0002|0x0001|0x0008

//BULLET_WND 32*20***********************************************
#define LEFT_BULLET_WND 0
#define TOP_BULLET_WND 0
#define RIGHT_BULLET_WND 31
#define BOTTOM_BULLET_WND 19

//direction ****************************************************
#define LEFT_DIRECTION 0
#define TOP_DIRECTION 1
#define RIGHT_DIRECTION 2
#define BOTTOM_DIRECTION 3

//level **********************************************
#define LEVEL0 0
#define LEVEL1 1
#define LEVEL2 2
 
//CPoint **********************************************
typedef struct _CPOINT
{
	int m_xPos;
	int m_yPos;
}CPoint, *PCPIONT;

//子弹碰撞后的返回信息; **********************************
typedef struct _COLLISIONINFO
{
	int m_iCollisionCode;		//reference iIDFlag;
	CBase* m_pBase;				//被碰撞的对象的指针;该指针是不用结构体对象去释放的,因为不是它创建的空间,只是指向了该空间;
	bool m_bLeftVisible;		//是否应该绘制左操作数(碰撞发起者)子弹;
	bool m_bLeftDie;			//对象是否要析构
	bool m_bRightVisible;		//是否应该绘制右操作数(碰撞发起者)子弹;
	bool m_bRightDie;			//对象是否要析构
	bool m_bRiver;				//碰撞检测的位置是否有河流

}COLLISIONINFO, *PCOLLISIONINFO;


extern int g_iCurrentTime;


#define ERROR_RETURN_VALUE 9527















class _CGlobal
{
public:
	_CGlobal();
	~_CGlobal();
};


//************************************
// 名称: WriteChar
// 功能: 设置光标位置，并在指定位置打印指定颜色的字符串
// 继承: private 
// 返回: void
// 参数: int Wide       行
// 参数: int High       列
// 参数: char * pszChar 字符串
// 参数: WORD wArr      颜色
//************************************
extern void WriteChar(int x, int y, char* pszChar, WORD wColor);