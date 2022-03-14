#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
class CRenderer;
class CShader;
END

BEGIN(Client)
class CSkyBox final : public CGameObject
{
public:
	typedef struct tagSKYDesc
	{
		_uint iCurrentIndex	= LEVEL_GAMEPLAY0;
		_uint iSkyBoxNum = 0;;
	}SKYDESC;
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype() override;
	virtual HRESULT	NativeConstruct(void* pArg) override;
	virtual _int	Tick(double TimeDelta)		override;
	virtual _int	Late_Tick(double TimeDelta) override;
	virtual HRESULT Render()					override;
private:
	HRESULT Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CTransform*		m_pTransformCom = nullptr;
	CTexture*		m_pTextureCom	= nullptr;
	CVIBuffer_Cube*	m_pVIBufferCom	= nullptr;
	CRenderer*		m_pRendrerCom	= nullptr;
	CShader*		m_pShaderCom	= nullptr;
private:
	SKYDESC			m_tSkyDesc;
public:
	static	 CSkyBox*	  Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	 CGameObject * Clone(void * pArg = nullptr) override;
	virtual	 void		  Free();
};
END
