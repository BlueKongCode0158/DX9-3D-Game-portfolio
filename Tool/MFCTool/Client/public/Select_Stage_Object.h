#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CStatic_Mesh;
END

BEGIN(Client)
class CSelect_Stage_Object final : public CGameObject
{
private:
	explicit CSelect_Stage_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CSelect_Stage_Object() = default;
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
	CShader*		m_pShaderCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CStatic_Mesh*	m_pMeshCom = nullptr;
public:
	static CSelect_Stage_Object*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
END
