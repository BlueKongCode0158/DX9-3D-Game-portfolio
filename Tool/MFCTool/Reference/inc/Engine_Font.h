#pragma once
#include "Component.h"

BEGIN(Engine)
class CShader;
class CTransform;
class ENGINE_DLL CEngine_Font final : public CComponent
{
public:
	typedef struct tagFontDesc
	{
		const _matrix*	pParentMatrix	= nullptr;
		_float3			vInitPos		= _float3(0.f, 0.f, 0.f);
		_float3			vScale			= _float3(1.f, 1.f, 1.f);
		_float3			vRotation		= _float3(0.f, 0.f, 0.f);
	}FONTDESC;
private:
	explicit CEngine_Font(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CEngine_Font(const CEngine_Font& rhs);
	virtual  ~CEngine_Font() = default;
public:
	HRESULT NativeConstruct_Prototype() override;
	HRESULT NativeConstruct(void* pArg) override;
public:
	void	Update_Font();
	HRESULT	Render_Font(const _tchar* pText, RECT rect);
private:
	LPD3DXSPRITE		m_pSprite	= nullptr;
	LPD3DXFONT			m_pFont		= nullptr;
private:
	CShader*			m_pShader		= nullptr;
	CTransform*			m_pTransform	= nullptr;
	FONTDESC			m_tFontDesc;
private:
	_matrix				m_OriginMatrix;
	_matrix				m_ParentMatrix;
public:
	static	CEngine_Font*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent*		Clone(void* pArg)	override;
	virtual void			Free()				override;
};
END
