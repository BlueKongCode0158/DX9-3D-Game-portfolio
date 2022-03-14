#pragma once
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CRenderer;
class CTexture;
class CShader;
END

BEGIN(Client)
class CNormal_Monster_Hp final : public CGameObject
{
public:
	typedef struct tagHpDesc
	{
		_float3			vIniPos = { 0.f,0.f,0.f };
		const _matrix*	pParentMatrix = nullptr;
	}HPDESC;
private:
	explicit CNormal_Monster_Hp(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CNormal_Monster_Hp(const CNormal_Monster_Hp& rhs);
	virtual  ~CNormal_Monster_Hp() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
public:
	void		Set_HpPer(_float fHpPer);
	void		Set_tagHpDesc(HPDESC* tHpDesc);
private:
	HRESULT Add_Component();
	HRESULT	SetUp_ConstantTable();
private:
	CVIBuffer_Rect*	m_pVIBufferCom	= nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom	= nullptr;
	CTexture*		m_pTextureCom	= nullptr;
	CShader*		m_pShaderCom	= nullptr;
private:
	_matrix			m_OriginMatrix;
private:
	_float			m_fHpPer		= 1.f;
	_float3			m_vScale;
private:
	HPDESC			m_tHpdesc;
public:
	static	CNormal_Monster_Hp*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg = nullptr) override;;
	virtual void					Free() override;
};
END