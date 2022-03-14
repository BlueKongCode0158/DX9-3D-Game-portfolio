#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CStatic_Mesh;
END

BEGIN(Client)
class CGun_Right final : public CGameObject
{
private:
	explicit CGun_Right(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CGun_Right(const CGun_Right& rhs);
	virtual	 ~CGun_Right() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT Render()					override;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	CShader*		m_pShaderCom	= nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CTransform*		m_pTransformCom	= nullptr;
	CStatic_Mesh*	m_pMeshCom		= nullptr;
private:
	_matrix			m_OriginMatrix;
	const _matrix*	m_pParentBoneMatrix		= nullptr;
	const _matrix*	m_pParentWorldMatrix	= nullptr;
public:
	static  CGun_Right*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg)	override;
	virtual void			Free()				override;
};
END
