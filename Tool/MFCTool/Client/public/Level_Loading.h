#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLoader;
class CLevel_Loading final : public CLevel
{
private:
	explicit CLevel_Loading(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual	 ~CLevel_Loading() = default;

public:
	virtual HRESULT NativeConstruct(LEVEL eNextLevel);
	virtual _int	Tick(_double TimeDelta) override;
	virtual HRESULT	Render() override;
private:
	HRESULT	Ready_Layer_UserInterface(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
private:
	CLoader*		m_pLoader = nullptr;
	LEVEL			m_eNextLevel = LEVEL_END;
public:
	static CLevel_Loading* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevel);
	virtual void Free() override;
};
END
