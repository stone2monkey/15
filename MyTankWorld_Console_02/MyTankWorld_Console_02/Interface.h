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
	// ��ӭ����
	void Welcome(CGameRule& oGameRule);
	// ��Ϸ�н���
	void Gaming(CGameRule& oGameRule);
	// ��Ϸ��������
	void Over(CGameRule& oGameRule);
};

