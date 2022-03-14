#include "stdafx.h"
#include "..\public\Pivot.h"
#include "PivotPoint.h"

IMPLEMENT_SINGLETON(CPivot)
CPivot::CPivot()
{
}

HRESULT CPivot::NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	if (nullptr == pGraphic_Device)
	{
		return E_FAIL;
	}
	m_pGraphicDevice = pGraphic_Device;
	Safe_AddRef(pGraphic_Device);

	//RIGHT,TOP,LOOK
	CPivotPoint* pRightPoint	= CPivotPoint::Create(m_pGraphicDevice);
	pRightPoint->Set_PivotType(CPivotPoint::RIGHT);

	CPivotPoint* pLookPoint		= CPivotPoint::Create(m_pGraphicDevice);
	pLookPoint->Set_PivotType(CPivotPoint::LOOK);

	CPivotPoint* pUpPoint		= CPivotPoint::Create(m_pGraphicDevice);
	pRightPoint->Set_PivotType(CPivotPoint::UP);

	Add_Pivot(CPivotPoint::RIGHT, pRightPoint);
	Add_Pivot(CPivotPoint::UP	, pUpPoint);
	Add_Pivot(CPivotPoint::LOOK	, pLookPoint);

	return S_OK;
}

HRESULT CPivot::Add_Pivot(_uint iState, CPivotPoint * pPivotPoint)
{
	if (nullptr == pPivotPoint)
	{
		return E_FAIL;
	}

	m_mapPivotPoints.emplace(iState, pPivotPoint);
	return S_OK;
}

_int CPivot::Tick(_double TimeDelta)
{
	for (auto Pair : m_mapPivotPoints)
	{
		Pair.second->Tick(TimeDelta);
	}

	return _int();
}

_int CPivot::Late_Tick(_double TimeDelta)
{
	for (auto Pair : m_mapPivotPoints)
	{
		Pair.second->Late_Tick(TimeDelta);
	}

	return _int();
}

HRESULT CPivot::Render()
{
	for (auto Pair : m_mapPivotPoints)
	{
		Pair.second->Render();
	}

	return S_OK;
}

HRESULT CPivot::Set_PivotObject(CPivotPoint * pGameObject)
{
	if (nullptr == pGameObject)
	{
		return E_FAIL;;
	}

	if (nullptr != m_pTransformObject)
	{
		Safe_Release(m_pTransformObject);
	}

	m_pTransformObject = pGameObject;
	Safe_AddRef(m_pTransformObject);

	return S_OK;
}

void CPivot::Free()
{
	Safe_Release(m_pGraphicDevice);
	Safe_Release(m_pTransformObject);

	for (auto Pair : m_mapPivotPoints)
	{
		Safe_Release(Pair.second);
	}
	m_mapPivotPoints.clear();


}
