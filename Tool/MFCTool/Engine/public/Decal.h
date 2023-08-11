#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Cube;
class ENGINE_DLL CDecal : public CGameObject
{
private:
	explicit CDecal(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CDecal(const CDecal& rhs);
	virtual ~CDecal() = default;
public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT	NativeConstruct(void* pArg = nullptr);
private:
	HRESULT	Add_Component(_uint iLevel);
public:
	_bool	Change_Target(CGameObject* pTarget);
private:
	CShader*		m_pShaderCom	= nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CTransform*		m_pTransformCom	= nullptr;
	CVIBuffer_Cube*	m_pVIBufferCom	= nullptr;
private:
	CGameObject*	m_pTarget		= nullptr;
public:
	static	CDecal*		 Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void		 Free() override;
};
END
