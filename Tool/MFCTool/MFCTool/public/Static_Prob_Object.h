#pragma once
#include "MFC_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CTransform;
class CStatic_Mesh;
END

BEGIN(MFC_TOOL)
class CStatic_Prob_Object final : public CGameObject
{
public:
	enum Crete_Object {
		TREE,
		ROCK,
		GRASS,
		RING,
		RING_BREAK,
		PLAYER_SELECT,
		CRETEOBJECT_END
	};
private:
	explicit CStatic_Prob_Object(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CStatic_Prob_Object(const CStatic_Prob_Object& rhs);
	virtual  ~CStatic_Prob_Object() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(CStatic_Prob_Object::Crete_Object eType);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT	Render()					override;
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CShader*		m_pShaderCom	= nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CStatic_Mesh*	m_pMeshCom		= nullptr;
private:
	Crete_Object	m_eType = CRETEOBJECT_END;
private:
	Crete_Object	m_eCreateType = CStatic_Prob_Object::Crete_Object::CRETEOBJECT_END;
	const	_tchar* m_pPrototypeTag = L"";
public:
	static CStatic_Prob_Object* Create(LPDIRECT3DDEVICE9 pGraphic_Device, CStatic_Prob_Object::Crete_Object eType);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END
