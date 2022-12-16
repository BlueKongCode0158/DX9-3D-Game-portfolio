#pragma once
#include "MFC_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CVIBuffer_Rect;
END

BEGIN(MFC_TOOL)
class CUI_Dummy : public CGameObject
{
public:
	typedef struct tInfo
	{
		int m_iLevel				= 0;
		const char* m_PrototypeTag  = nullptr;
		const char* m_TexturePath	= nullptr;
	}Info;
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
public:
	HRESULT Set_Position(_float3 vecPosition);
	void	Set_Select(_bool isSelect);
	void	Set_Dead(_bool isDead);
	void	Update_Matrix();
public:
	void	Get_Information(UIINFO& rInfo);
private:
	HRESULT	Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CString			m_pLayerTag;
private:
	CShader*		m_pShaderCom		= nullptr;
	CTexture*		m_pTextureCom		= nullptr;
	CRenderer*		m_pRendererCom		= nullptr;
	CTransform*		m_pTransformCom		= nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom		= nullptr;
private:
	CTransform*		m_pParentTransform	= nullptr;
	_matrix			m_ParentMatrix;
	_matrix			m_OriginMatrix;
	_matrix			m_WorldMatrix;
private:
	_bool			m_isSelect = false;
public:
	static	CUI_Dummy*			Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
END