#pragma once
#include <assert.h>
#include <vector>
class CBase;

using namespace std;

/****************************************************/
/*	IDFlag											*/
/****************************************************/
#define NULL_SPACE					'0'

#define MYSELF_WHITE_TANK			'1'		//���٢ڢۢŢƢ�
#define FRIEND_WITHE_TANK			'2'
#define ENEMY_RED_TANK				'3'
#define ENEMY_YELLOW_TANK			'4'
#define ENEMY_GREEN_TANK			'5'

#define MYSELF_WHITE_BULLET			'6'		//��
#define FRIEND_WITHE_BULLET			'7'
#define ENEMY_RED_BULLET			'8'
#define ENEMY_YELLOW_BULLET			'9'
#define ENEMY_GREEN_BULLET			'a'

#define STEEL_WHITE_BRICK			'b'		//��
#define REVER_BLUE_BRICK			'c'		//��	����
#define NORMAL_PURPLE_BRICK			'd'		//��

#define THE_RED_KING	'e'		//��

//�����IDFlag��������ײ��,��������չ��**************
#define COLLISIONCODE_OUTSIDE		'f'		//����һ����ײ��,�ӵ���ײ���ʱ�õ�;
#define COLLISIONCODE_NOCOLLISION	'0'

/****************************************************/
/*	������ɫ����									*/
/****************************************************/

// ������ɫ
#define RED    0x0004|0x0008        // ����
#define YELLOW 0x0002|0x0004|0x0008 // ����
#define GREEN  0x0002|0x0008        // ����
#define CYAN   0x0002|0x0004|0x0008 // ����
#define BLUE   0x0001|0x0008        // ����
#define PURPLE 0x0001|0x0004|0x0008 // ����
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

//�ӵ���ײ��ķ�����Ϣ; **********************************
typedef struct _COLLISIONINFO
{
	int m_iCollisionCode;		//reference iIDFlag;
	CBase* m_pBase;				//����ײ�Ķ����ָ��;��ָ���ǲ��ýṹ�����ȥ�ͷŵ�,��Ϊ�����������Ŀռ�,ֻ��ָ���˸ÿռ�;
	bool m_bLeftVisible;		//�Ƿ�Ӧ�û����������(��ײ������)�ӵ�;
	bool m_bLeftDie;			//�����Ƿ�Ҫ����
	bool m_bRightVisible;		//�Ƿ�Ӧ�û����Ҳ�����(��ײ������)�ӵ�;
	bool m_bRightDie;			//�����Ƿ�Ҫ����
	bool m_bRiver;				//��ײ����λ���Ƿ��к���

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
// ����: WriteChar
// ����: ���ù��λ�ã�����ָ��λ�ô�ӡָ����ɫ���ַ���
// �̳�: private 
// ����: void
// ����: int Wide       ��
// ����: int High       ��
// ����: char * pszChar �ַ���
// ����: WORD wArr      ��ɫ
//************************************
extern void WriteChar(int x, int y, char* pszChar, WORD wColor);