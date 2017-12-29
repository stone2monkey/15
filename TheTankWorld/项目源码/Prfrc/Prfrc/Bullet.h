#pragma once
#include "Base.h"
class CBullet :
	public CBase
{
public:
	CBullet(int xPos, int yPos, int iID, int iLevel = 1, int iDirect = 1);
	~CBullet();


	//Attributions ***********************************************************************************

	int m_iDirect;

	int m_iForce;

	UINT m_uMoveSpeed;
	UINT m_uLastMoveTime = 0;

	//Method *****************************************************************************************

	void Init(int iDirect);

	void Move(int iDirect);
	void am_MoveToLeft();
	void am_MoveToTop();
	void am_MoveToRight();
	void am_MoveToBottom();
};

