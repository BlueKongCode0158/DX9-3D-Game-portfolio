#pragma once
#include "Level.h"

BEGIN(Client)
class CLevel_Logo final : public CLevel
{
private:
	explicit CLevel_Logo(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double TimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT	Ready_Light_Desc();
	HRESULT	Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT	Ready_Layer_SkyBox(const _tchar* pLayerTag);
	HRESULT Ready_Camera(const _tchar* pLayerTag);
	HRESULT Ready_UI(const _tchar* pLayerTag);
public:
	static CLevel_Logo* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void		Free() override;
};
END