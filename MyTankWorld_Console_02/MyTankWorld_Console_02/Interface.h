#pragma once
#include "_CGlobal.h"
#include "GameRule.h"
#include "Mapp.h"
#include "BulletMoveManager.h"
#include "TankMoveManager.h"


class CInterface
{
public:
	CInterface();
	~CInterface();
	void SetWindow();
	// 欢迎界面
	void Welcome(CGameRule& oGameRule);
	// 游戏中界面
	void Gaming(CGameRule& oGameRule);
	// 游戏结束界面
	void Over(CGameRule& oGameRule);
};

