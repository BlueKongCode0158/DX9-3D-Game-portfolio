#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CUI_BackGround final : public CGameObject
{
private:
	explicit CUI_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI_BackGround(const CUI_BackGround& rhs);
	virtual  ~CUI_BackGround() = default;
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
	LPDIRECT3DTEXTURE9	m_pTexture		= nullptr;
	CShader*			m_pShaderCom	= nullptr;
	CRenderer*			m_pRendererCom	= nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CVIBuffer_Rect*		m_pVIBufferCom	= nullptr;
public:
	static CUI_BackGround*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;;
	virtual void			Free() override;
};
END
