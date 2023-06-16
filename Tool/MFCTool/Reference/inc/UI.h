#pragma once
#include "GameObject.h"

/*
2D UI Class
*/
BEGIN(Engine)
class CVIBuffer_Rect;
class CRenderer;
class CTexture;
class CShader;
class ENGINE_DLL CUI : public CGameObject
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
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CUI(const CUI& rhs);
	virtual  ~CUI() = default;
public:
	virtual HRESULT NativeConstruct_Prototype()		override;
	virtual HRESULT NativeConstruct(void* pArg)		override;
	virtual _int	Tick(_double TimeDelta)			override;
	virtual _int	Late_Tick(_double TimeDelta)	override;
	virtual HRESULT Render()						override;
protected:
	CVIBuffer_Rect*		m_pVIBufferCom		= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CShader*			m_pShaderCom		= nullptr;
	CTexture*			m_pTextureCom		= nullptr;
	_uint				m_iShaderIndex		= 0;
	UIDESC				m_tUIDesc;
	_matrix				m_ProjMatrix;
public:
	virtual CGameObject*	Clone(void* pArg)		PURE;
	virtual void			Free()					override;
};
END

