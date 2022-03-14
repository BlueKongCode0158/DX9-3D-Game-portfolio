#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CStatic_Mesh;
class CTransform;
class CRenderer;
class CFrustum;
class CShader;
END

BEGIN(Client)
class CStatic_Object final:public CGameObject
{
public:
	enum PROB { RROB_TREE, PROB_ROCK, PROB_GRASS, PROB_BROKEN_RING, PROB_RING , PROB_END };
private:
	explicit CStatic_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatic_Object(const CStatic_Object& rhs);
	virtual  ~CStatic_Object() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(PROB eType);
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT	Render()					override;
private:
	HRESULT Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	CStatic_Mesh*	m_pMeshCom		= nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CFrustum*		m_pFrustumCom	= nullptr;
	CShader*		m_pShaderCom	= nullptr;
private:
	PROB			m_eProbType = PROB_END;
	const _tchar*	m_pLayerTag = nullptr;
public:
	static	CStatic_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device, PROB eType);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
END
