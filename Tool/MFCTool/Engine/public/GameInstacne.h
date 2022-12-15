#pragma once
#include "Level_Manager.h"
#include "GameObject_Manager.h"
#include "Light_Manager.h"
#include "Graphic_Device.h"
#include "Component_Manager.h"
#include "Time_Manager.h"
#include "Input_Device.h"
#include "PipeLine.h"
#include "Picking.h"
#include "Light_Manager.h"
#include "Target_Manager.h"
#include "Key_Manager.h"

BEGIN(Engine)
class ENGINE_DLL CGameInstacne final : public CBase
{
	DECLARE_SINGLETON(CGameInstacne)
private:
	explicit CGameInstacne();
	virtual ~CGameInstacne() = default;
public:
	HRESULT	Initialize_Engine(_uint iNumLevel, HWND hWnd, HINSTANCE hInst);
	_int	Tick(_double TimeDelta);
	_int	Late_Tick(_double TimeDelta);
	void	Clear(_uint iLevel);

#pragma region GRAPHIC
public:
	HRESULT Ready_Graphic_Device(HWND hWnd, CGraphic_Device::WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9* ppGraphic_Device);
	void	Render_Begin();
	void	Render_End(HWND hWnd = nullptr);
public:
	LPDIRECT3DDEVICE9	Get_Device();
	LPD3DXSPRITE		Get_Sprite();
	LPD3DXFONT			Get_Font();
	LPD3DXLINE			Get_Line();
#pragma endregion

#pragma region INPUT
public:
	_byte Get_DIKeyState(_byte byKey);
	_byte Get_DIMouseKeyState(CInput_Device::DIM eMouseKey);
	_long Get_DIMouseMoveState(CInput_Device::DIMM eMouseMove);
public:
	_bool	Input_KeyBoard_Down(_ubyte byKey);
	_bool	Input_KeyBoard_Up(_ubyte byKey);
	_bool	Input_KeyBoard_Pressing(_ubyte byKey);
public:
	_bool	Input_KeyMouse_Down(CInput_Device::DIM eMouseKey);
	_bool	Input_KeyMouse_Up(CInput_Device::DIM eMouseKey);
	_bool	Input_KeyMouse_Pressing(CInput_Device::DIM eMouseKey);
#pragma endregion

#pragma region KEY_MANAGER
public:
	_bool Key_Down(_ulong dwKey);
	_bool Key_Up(_ulong dwKey);
	_bool Key_Pressing(_ulong dwKey);
#pragma endregion

#pragma region LEVEL
public:
	HRESULT SetUp_Level(class CLevel* pLevel);
	HRESULT	Render_Level();
#pragma endregion
	
#pragma region PIPE
public:
	_matrix		Get_Transform(D3DTRANSFORMSTATETYPE eState);
	_float3		Get_CamPosition();
#pragma endregion

#pragma region OBJECT
public:
	HRESULT Add_Prototype_Object(_uint iTypeNum, const _tchar* pPrototypeTag, CGameObject* pGameObject);
	HRESULT Add_GameObject_Clone(_uint iNumLevel, const _tchar* pPrototypeTag, const _tchar* pLayerTag, void* pArg = nullptr);
	CComponent* Find_Component(_uint iLevelIndex, const _tchar* pLayerTag,const _tchar* pComponentTag,_uint iIndexNum = 0);
	CGameObject* Find_GameObject(_uint iLevelIndex, const _tchar* pLayerTag, _uint iIndexNum = 0);
public:
	HRESULT	Delete_Prototype(_uint iTypeNum, const _tchar* pPrototypeTag);
	HRESULT Delete_GameObject(_uint iTypeNum, const _tchar* pLayerTag, _int iIndex);
	HRESULT Delete_GameObjects(_uint iTypeNum, const _tchar* pLayerTag);
	HRESULT Delete_GameObject_End(_uint iTypeNum, const _tchar* pLayerTag);
public:
	_uint	Get_GameObject_ListSize(_uint iLevelIndex, const _tchar* pLayerTag);
#pragma endregion

#pragma region COMPONENT
public:
	HRESULT		Add_Prototype_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, CComponent* pPrototype);
	CComponent*	Clone_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, void* pArg = nullptr);
#pragma endregion
#pragma region TIMER
public:
	HRESULT	Add_Timers(const _tchar* pTimerTag);
	_float	Compute_Time(const _tchar* pTimerTag);
	_float	Get_Time(const _tchar* pTimerTag);
#pragma endregion
#pragma region PICKING
public:
	HRESULT		Compute_MouseCursorPosInWorld(HWND hWnd);
	_float3*	Compute_PickingPoint(class CVIBuffer* pVIBuffer, _matrix WorldMatrix);
	_float3*	Compute_PickingPoint(LPD3DXMESH pMesh, _matrix WorldMatrix);
public:
	HRESULT		Compute_WindowCenterPosition();
	_float3*	Compute_CenterPoint(class CVIBuffer* pVIBuffer, _matrix WorldMatrix);
	_float3*	Compute_CenterPoint(class CCollider* pCollider, _matrix WorldMatrix);
	_float3*	Compute_CenterPoint(LPD3DXMESH pMesh, _matrix WorldMatrix);
#pragma endregion
#pragma region LIGHT
public:
	D3DLIGHT9	Get_LightDesc(_uint iIndex = 0);
	HRESULT		Level_Relese_Light();
	HRESULT		Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9& LightDesc);
#pragma endregion
public:
	static void Release_Engine();
private:
	CGraphic_Device*		m_pDevice		= nullptr;
	CLevel_Manager*			m_pLevel		= nullptr;
	CGameObject_Manager*	m_pGameObject	= nullptr;
	CComponent_Manager*		m_pComponent	= nullptr;
	CTime_Manager*			m_pTimer		= nullptr;
	CInput_Device*			m_pInput		= nullptr;
	CPipeLine*				m_pPipe			= nullptr;
	CPicking*				m_pPicking		= nullptr;
	CLight_Manager*			m_pLight		= nullptr;
	CKey_Manager*			m_pKeyManager	= nullptr;
public:
	virtual void Free() override;

};
END

