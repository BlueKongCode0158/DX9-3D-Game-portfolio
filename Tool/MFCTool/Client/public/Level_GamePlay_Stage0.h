#pragma once
#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)
class CLevel_GamePlay_Stage0 final : public CLevel
{
private:
	explicit CLevel_GamePlay_Stage0(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual  ~CLevel_GamePlay_Stage0() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double DeltaTime) override;
	virtual HRESULT Render() override;
private:
	_bool	m_isStart = false;
	_bool	m_isCreate = false;
private:
	HRESULT Ready_Light_Desc();
	HRESULT	Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Pod(const _tchar* pLayerTag);
	HRESULT Ready_Light(const _tchar* pLayerTag);
public:
	static CLevel_GamePlay_Stage0*	Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void					Free();
};
END
