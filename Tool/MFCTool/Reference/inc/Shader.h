#pragma once
#include "Component.h"

BEGIN(Engine)
class CTexture;

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CShader(const CShader& rhs);
	virtual  ~CShader() = default;
public:
	virtual HRESULT	NativeConstruct_Prototype(const _tchar* pFilePath);
	virtual HRESULT	NativeConstruct(void* pArg) override;
public:
	HRESULT	SetUp_ConstantTable(D3DXHANDLE hHandle, const void* pData,_uint iLengthByte);
	HRESULT	SetUp_TextureConstantTable(D3DXHANDLE hHandle, CTexture* pTexture, _uint uiTextureIndex);
	HRESULT	SetUp_TextureConstantTable(D3DXHANDLE hHandle, LPDIRECT3DBASETEXTURE9 pTexture);
	HRESULT	SetUp_Bool(D3DXHANDLE hHandle, BOOL isBool);
public:
	HRESULT	Begin_Shader(_uint iIndexNum);
	void	Commit();
	HRESULT	End_Shader();
protected:
	LPD3DXEFFECT		m_pEffect	= nullptr;
public:
	static	CShader*		Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pFilePath);
	virtual	CComponent*		Clone(void* pArg = nullptr) override;
public:
	virtual void Free() override;
};
END
