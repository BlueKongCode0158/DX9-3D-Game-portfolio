#pragma once
#include "Base.h"

BEGIN(Engine)
class CComponent;
class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	enum STATE	{ OBJ_ATTCK, OBJ_IDLE, OBJ_DEAD,OBJ_END	};
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;
public:
	_float Get_CamDistance() const;
public:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar * pComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	CComponent* Find_ObjectComponent(const _tchar* pComponentTag);
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg = nullptr);
	virtual _int	Tick(_double TimeDelta);
	virtual _int	Late_Tick(_double TimeDelta);
	virtual	HRESULT	Render();
public:
	_bool	IsDead();
protected:
	LPDIRECT3DDEVICE9		m_pGraphicDevice	= nullptr;
	_float					m_fCamDistance		= 0.f;
protected:
	_bool					m_isDead			= false;
protected:
	map<const _tchar*, CComponent*>		m_Componenets; 
	typedef map<const _tchar*, CComponent*> COMPONENT;
protected:
	void Compute_CameraDistance(class CTransform* pTransform);
private:
	CComponent* Find_Component(const _tchar* pComponentTag);
public:
	virtual CGameObject* Clone(void* pArg = nullptr)PURE;
	virtual void		 Free() override;
};
END

