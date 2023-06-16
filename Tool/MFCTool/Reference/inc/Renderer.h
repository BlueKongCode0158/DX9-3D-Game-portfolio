#pragma once
#include "Component.h"

BEGIN(Engine)
class CComponent;
class CGameObject;
class CParticle_System;
class CVIBuffer_Rect_Viewport;
class CTarget_Manager;
class CShader;
class ENGINE_DLL CRenderer final: public CComponent
{
public:
	enum     RENDERGROUP { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END};
public:
	explicit CRenderer(LPDIRECT3DDEVICE9 pGraphic_Device);
	//explicit CRenderer(CRenderer& rhs); - Clone을 만들 때 사용한다.
	virtual  ~CRenderer() = default;
public:
	virtual	HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eGroup, CGameObject* pGameObject);
	HRESULT Add_RenderEffect(CParticle_System* pObject);
#ifdef _DEBUG
	HRESULT	Add_RenderDebug(CComponent* pComponent);
	HRESULT	Render_Debug();
#endif // _DEBUG
	HRESULT Render_GameObject();
private:
	HRESULT	Render_Priority();
	HRESULT Render_NonAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_UI();
private:
	HRESULT	Render_LightAcc();
	HRESULT	Render_Blend();
private:
	list<CGameObject*>		m_RenderObjects[RENDER_END];
	list<CComponent*>		m_DebugObject;
private:
	class CTarget_Manager*			m_pTarget_Manager	= nullptr;
	class CShader*					m_pShader			= nullptr;
	class CVIBuffer_Rect_Viewport*	m_pVIBuffer			= nullptr;
private:
	_bool	m_isDebugRender = false;
public:
	static CRenderer*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*	Clone(void* pArg = nullptr) override;
	virtual void		Free() override;

};
END

