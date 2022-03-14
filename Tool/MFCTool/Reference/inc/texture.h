#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
public:
	enum TEXTURETYPE { TEXTURE_GENERIC,TEXTURE_CUBE,TEXTURE_END };
private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CTexture(const CTexture& rhs);
	virtual  ~CTexture() = default;
public:
	_uint	Get_vecTexture_Size()
	{
		return m_vTexture.size();
	}
public:
	LPDIRECT3DBASETEXTURE9 Get_Texture(_uint uIndex)
	{
		return m_vTexture[uIndex];
	}
public:
	virtual HRESULT NativeConstruct_Prototype(TEXTURETYPE eTextureType, const _tchar* pFilePath  ,_uint iTextureNum);
	virtual HRESULT NativeConstruct(void* pArg) override;
public:
	HRESULT	SetUp_OnDevice_Texture(_uint iStage, _uint iTextureNum = 0);
private:
	vector<IDirect3DBaseTexture9*>		m_vTexture;
public:
	static CTexture*	Create(TEXTURETYPE eType, const _tchar* pFilePath, LPDIRECT3DDEVICE9 pGraphic_Device, _uint iTextureNum = 1);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free() override;
	
};
END

