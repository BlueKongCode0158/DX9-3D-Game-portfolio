#pragma once
#include "MFC_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(MFC_TOOL)
class CUI_Dummy : public CGameObject
{
private:
	explicit CUI_Dummy(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_Dummy(CUI_Dummy&  rhs);
	virtual ~CUI_Dummy() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_double TimeDelta)		override;
	virtual _int	Late_Tick(_double TimeDelta)override;
	virtual HRESULT	Render()					override;
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();

private:
	CShader*		m_pShaderCom		= nullptr;
	CRenderer*		m_pRendererCom		= nullptr;
	CTransform*		m_pTransformCom		= nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom		= nullptr;
public:
	static	CUI_Dummy*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END