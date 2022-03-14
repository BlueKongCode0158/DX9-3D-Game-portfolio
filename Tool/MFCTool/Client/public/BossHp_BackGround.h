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
class CBossHp_BackGround final : public CGameObject
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
	explicit CBossHp_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBossHp_BackGround(const CBossHp_BackGround& rhs);
	virtual  ~CBossHp_BackGround() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
public:
	void		Set_HpPer(_float fHpPer);
	HRESULT		Set_UIDesc(UIDESC* tUIDesc);
private:
	HRESULT Add_Component();
private:
	LPDIRECT3DTEXTURE9	m_pTexture = nullptr;
private:
	CVIBuffer_Rect*		m_pVIBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CRenderer*			m_pRendererCom = nullptr;
	CShader*			m_pShaderCom = nullptr;
private:
	_float				m_fHpPer = 1.f;
	UIDESC				m_tUIDesc;
	_matrix				m_ProjMatrix;
public:
	static	CBossHp_BackGround*		Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject*			Clone(void* pArg = nullptr) override;
	virtual void					Free() override;
};
END
