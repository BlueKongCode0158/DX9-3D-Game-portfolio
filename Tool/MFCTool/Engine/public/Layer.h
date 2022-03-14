#pragma once
#include "GameObject.h"

 BEGIN(Engine)
class CLayer final: public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;
public:
	_uint	Get_ListSize()
	{
		return m_Objects.size();
	}
public:
	CGameObject*	Get_GameObjectPointer(_uint iIndex);
	CComponent*		Find_Component(const _tchar* ComponentTag,_uint iIndexNum);
public:
	HRESULT NativeConstruct();
	HRESULT Add_GameObject(CGameObject* pGameObject);
	_int	Tick(_double TimeDelta);
	_int	Late_Tick(_double TimeDelta);

private:
	list<CGameObject*>	m_Objects;

public:
	static CLayer* Create();
	virtual void   Free() override;
};
 END
