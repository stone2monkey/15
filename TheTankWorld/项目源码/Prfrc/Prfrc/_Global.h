#pragma once
#include <cassert>
#include <windows.h>


//Attributions ***********************************************************************************
//Method *****************************************************************************************

//************************************************************************************************
// ����: 
// ����: 
// �̳�:  
// ����: 
// ����: 
// ����: 
// ����: 
// ����: 
//************************************************************************************************

//*** IDFlag ***************************************************
#define NULL_SPACE						0

#define ONEPLAYER_WHITE_TANK		1		//���٢ڢۢŢƢ�
#define TWOPLAYER_WHITE_TANK		2
#define ENEMY_RED_TANK				3
#define ENEMY_YELLOW_TANK			4
#define ENEMY_GREEN_TANK			5

#define ONEPLAYER_WHITE_BULLET		6		//��
#define TWOPLAYER_WHITE_BULLET		7
#define ENEMY_RED_BULLET			8
#define ENEMY_YELLOW_BULLET			9
#define ENEMY_GREEN_BULLET			10

#define STEEL_WHITE_BRICK			11		//��
#define REVER_BLUE_BRICK			12		//��	����
#define NORMAL_PURPLE_BRICK			13		//��

#define THE_RED_KING				14		//��

//̹�˵��е�,��������Ϊ�е���̹�������㲻ͬ,�����������,���ض������»����ʾBUG
#define TANK_CENTER_POINT			15		

//*** ������ɫ���� *************************************************
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
#define LEVEL1 1
#define LEVEL2 2
#define LEVEL3 3

// + CollisionCode **********************************
#define NO_COLLISION				0		//û�з�����ײ
#define FRIEND_WHITE_TANK			2
#define ENEMY_RED_TANK				3
#define FRIEND_WHITE_BULLET			7
#define ENEMY_RED_BULLET			8
#define STEEL_WHITE_BRICK			11		//��
#define REVER_BLUE_BRICK			12		//��	����
#define NORMAL_PURPLE_BRICK			13		//��
#define THE_RED_KING				14		//��
#define OUTSIDE						15		//����






class _Global
{
public:
	_Global();
	~_Global();
};

