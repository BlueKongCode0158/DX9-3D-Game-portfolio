#include "stdafx.h"
#include "Monster.h"
#include "..\public\Monster_Data.h"

CMonster_Data::CMonster_Data(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CState(pGraphic_Device)
{
}

HRESULT CMonster_Data::NativeConstruct(CGameObject * pObject)
{
	if (nullptr == pObject)
	{
		return E_FAIL;
	}

	m_pMonster = dynamic_cast<CMonster*>(pObject);
	if (nullptr == m_pMonster)
	{
		return E_FAIL;
	}
	return S_OK;
}

_uint CMonster_Data::HandleInput()
{
	return _uint();
}

void CMonster_Data::Enter()
{
}

void CMonster_Data::Update(_float TimeDelta)
{
	m_fSkil0 -= TimeDelta;
	m_fSkil1 -= TimeDelta;
	m_fSkil2 -= TimeDelta;

	if (m_fSkil0 < 0)
	{
		m_fSkil0 = 0.f;
	}
	if (m_fSkil1 < 0)
	{
		m_fSkil1 = 0.f;
	}
	if (m_fSkil2 < 0)
	{
		m_fSkil2 = 0.f;
	}
}

void CMonster_Data::Late_Update(_float TimeDelta)
{
}

void CMonster_Data::Exit()
{
}

const _float CMonster_Data::Get_IdleTime() const
{
	return m_fIdleTime;
}

void CMonster_Data::Set_IdleTime(_float fTime)
{
	m_fIdleTime = fTime;
}

const _float3 CMonster_Data::Get_SkilTime() const
{
	return _float3(m_fSkil0, m_fSkil1, m_fSkil2);
}

void CMonster_Data::Set_SkilTime(_float fSkil0, _float fSkil1, _float fSkil2)
{
	m_fSkil0 = fSkil0;
	m_fSkil1 = fSkil1;
	m_fSkil2 = fSkil2;
}

const _bool CMonster_Data::Get_Recog() const
{
	return m_isRecog;
}

void CMonster_Data::Set_Recog(_bool isRecog)
{
	m_isRecog = isRecog;
}

void CMonster_Data::Free()
{
	__super::Free();
}
