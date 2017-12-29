#pragma once
#include "Base.h"
#include "Bullet.h"

class CTank :
	public CBase
{
public:
	CTank(int xPos, int yPos, int iID, int iLevel = 1, int iDirect = 1);
	~CTank();

	typedef struct _point
	{
		int m_x;
		int m_y;
	}point, *ppoint;

	//Attributions ***********************************************************************************

	int m_iDirect;

	UINT m_uMoveSpeed;
	UINT m_uLastMoveTime = 0;

	UINT m_uShotSpeed;
	UINT m_uLastShotTime = 0;

	point m_arrSixPt[6] = {};


	//Method *****************************************************************************************

	void Init(int iDirect);

	void Move(int iDirect);
	void am_MoveToLeft();
	void am_MoveToTop();
	void am_MoveToRight();
	void am_MoveToBottom();

	bool JustChangeDirict(int iDirect);

	void FillSixPtArr();
	void am_FillSixPtArrViaArr(int arr[10]);

	CBullet* ShotBullet();
};

