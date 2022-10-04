#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CTransform;
class CTexture;
class CRenderer;
class CShader;
END

BEGIN(Client)
class CLogo_Terrain final : public CGameObject
{
private:
	explicit CLogo_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CLogo_Terrain(const CLogo_Terrain& rhs);
	virtual  ~CLogo_Terrain() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg = nullptr) override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual _int	Late_Tick(_double TimeDelta) override;
	virtual	HRESULT	Render() override;
private:
	HRESULT	Add_Component();
	HRESULT SetUp_ConstantTable();
private:
	CTransform*			m_pTransformCom		= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CTexture*			m_pTextureCom		= nullptr;
	CVIBuffer_Terrain*	m_pVIBufferCom		= nullptr;
	CShader*			m_pShaderCom		= nullptr;
private:
	LPDIRECT3DTEXTURE9	m_pFilterTexture	= nullptr;
public:
	static	CLogo_Terrain*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;

};
END
