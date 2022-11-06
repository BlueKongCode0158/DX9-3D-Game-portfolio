#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CNavigation;
class CEngine_Font;
class CCollider;
class CRenderer;
class CTransform;
class CDynamic_Mesh;
END

BEGIN(Client)
class CPlayer_Select_Dummy final : public CGameObject
{
private:
	explicit CPlayer_Select_Dummy(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CPlayer_Select_Dummy(const CPlayer_Select_Dummy& rhs);
	virtual  ~CPlayer_Select_Dummy() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT Render() override;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	CDynamic_Mesh*		m_pMeshCom			= nullptr;
	CShader*			m_pShaderCom		= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CTransform*			m_pTransformCom		= nullptr;
	CEngine_Font*		m_pFont				= nullptr;
public:
	static CPlayer_Select_Dummy*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
