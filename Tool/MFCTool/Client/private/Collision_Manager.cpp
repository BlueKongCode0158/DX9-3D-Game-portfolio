#include "stdafx.h"
#include "..\public\Collision_Manager.h"

IMPLEMENT_SINGLETON(CCollision_Manager)
CCollision_Manager::CCollision_Manager()
{
}



HRESULT CCollision_Manager::Delete_CollisionManager()
{
	Free();
}

void CCollision_Manager::Free()
{

}
