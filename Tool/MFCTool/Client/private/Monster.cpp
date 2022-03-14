#include "stdafx.h"
#include "Client_Defines.h"
#include "..\public\Monster.h"
#include "BackGround.h"
#include "GameInstacne.h"
#include "Bullet.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CGameObject(pGraphic_Device)
{

}

CMonster::CMonster(const CMonster & rhs)
	:CGameObject(rhs)
{

}

HRESULT CMonster::NativeConstruct_Prototype()
{
	if (FAILED(__super::NativeConstruct_Prototype()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
	{
		return E_FAIL;
	}
	return S_OK;
}

_int CMonster::Tick(_double TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CMonster::Late_Tick(_double TimeDelta)
{
	__super::Late_Tick(TimeDelta);



	return _int();
}

HRESULT CMonster::Render()
{
	__super::Render();

	return S_OK;
}


void CMonster::Free()
{
	__super::Free();
}
