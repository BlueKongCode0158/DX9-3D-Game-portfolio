#pragma once
#include "MFC_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CRenderer;
class CGameObject;
END

BEGIN(MFC_TOOL)
class CPivotPoint;
class CPivot final : public CBase
{
	DECLARE_SINGLETON(CPivot)
private:
	explicit CPivot();
	virtual  ~CPivot() = default;
public:
	HRESULT NativeConstruct(LPDIRECT3DDEVICE9 pGraphic_Device);
	_int	Tick(_double TimeDelta);
	_int	Late_Tick(_double TimeDelta);
	HRESULT	Render();
public:
	HRESULT Set_PivotObject(CPivotPoint* pGameObject);
private:
	HRESULT Add_Pivot(_uint iState, CPivotPoint* pPivotPoint);
private:
	LPDIRECT3DDEVICE9		m_pGraphicDevice	= nullptr;
private:
	map<_uint, CPivotPoint*>	m_mapPivotPoints;
	class CGameObject*			m_pTransformObject = nullptr;
public:
	virtual void Free() override;
};
END
