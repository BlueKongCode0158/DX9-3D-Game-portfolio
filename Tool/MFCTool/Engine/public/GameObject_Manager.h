#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CComponent;
class CLayer;
class CGameObject_Manager final
	:public CBase
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	explicit CGameObject_Manager();
	virtual ~CGameObject_Manager() = default;
public:
	void	Clear(_uint iLevel);
public:
	HRESULT Reserve_Container_ProtoType(_uint iTypeNum);
	HRESULT Reserve_Container(_uint iNumLevel);
public:
	HRESULT Add_Prototype(_uint iTypeNum, const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT Add_GameObject_Clone(_uint iNumLevel, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
public:
	HRESULT	Delete_Prototype(_uint iTypeNum, const _tchar* pPrototypeTag);
	HRESULT Delete_GameObject(_uint iTypeNum, const _tchar* pLayerTag, _int iIndex);
	HRESULT Delete_GameObjects(_uint iTypeNum, const _tchar* pLayerTag);
public:
	_uint	Get_GameObject_ListSize(_uint iLevelIndex, const _tchar* pLayerTag);
public:
	_int	Tick(_double TimeDelta);
	_int	Late_Tick(_double TimeDelta);
private:
	map<const _tchar*, CGameObject*>*	m_pPrototypes = nullptr;
	typedef map<const _tchar*, CGameObject*> PROTOTYPE;
private:
	map<const _tchar*, CLayer*>*		m_pLayers = nullptr;
	typedef map<const _tchar*, CLayer*>	LAYER;
private:
	_uint		m_iNumLevel = 0;
public:
	CGameObject*	Find_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _int iIndex);
	CComponent*		Find_Component(_uint iLevelIndex, const _tchar* pLayerTag, const _tchar* pComponentTag, _uint iIndexNum = 0);
private:
	CGameObject* Find_Prototype(_uint iTypeNum,const _tchar* pPrototypeTag);
	CLayer*      Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
private:
	HRESULT	Delete_Layer(_uint iTypeNum, const _tchar* pLayerTag);
public:
	virtual void Free() override;
};
END
