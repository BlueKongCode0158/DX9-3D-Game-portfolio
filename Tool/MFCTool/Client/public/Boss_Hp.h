#pragma once
#include "UI.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTransform;
class CRenderer;
class CTexture;
class CShader;
END

BEGIN(Client)
class CBoss_Hp final : public CGameObject
{
public:
	typedef struct tagUIDesc
	{
		_float m_fX;
		_float m_fY;
		_float m_fSizeX;
		_float m_fSizeY;
		_float m_fWinSizeX;
		_float m_fWinSizeY;
	}UIDESC;
private:
	explicit CBoss_Hp(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBoss_Hp(const CUI& rhs);
	virtual  ~CBoss_Hp() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
private:
	HRESULT Add_Component();
public:
	void		Set_HpPer(_float fHpPer);
	HRESULT		Set_UIDesc(UIDESC* tUIDesc);
private:
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;
	CShader*		m_pShaderCom = nullptr;
private:
	UIDESC			m_tUIDesc;
	_float			m_fHpPer		=1.f;
	_matrix			m_ProjMatrix;
public:
	static	CBoss_Hp*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*	Clone(void* pArg = nullptr) override;
	virtual void			Free() override;
};
END
