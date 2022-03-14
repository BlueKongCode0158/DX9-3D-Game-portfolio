#pragma once
#include "Base.h"

BEGIN(Engine)
class CLight;
class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	explicit CLight_Manager();
	virtual  ~CLight_Manager() = default;
public:
	_uint	Get_IndexSize()
	{
		return m_vecLights.size();
	}
public:
	D3DLIGHT9	Get_LightDesc(_uint iIndex = 0);
public:
	HRESULT		Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc);
	HRESULT		Render();
public:
	HRESULT		Level_Relese_Light();
private:
	list<CLight*>			m_vecLights;
	typedef list<CLight*>	LIGHTS;
public:
	virtual void Free() override;
};
END

