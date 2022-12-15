#include "..\public\GameInstacne.h"


IMPLEMENT_SINGLETON(CGameInstacne)
CGameInstacne::CGameInstacne()
	: m_pDevice(CGraphic_Device::Get_Instance())
	, m_pLevel(CLevel_Manager::Get_Instance())
	, m_pGameObject(CGameObject_Manager::Get_Instance())
	, m_pComponent(CComponent_Manager::Get_Instance())
	, m_pTimer(CTime_Manager::Get_Instance())
	, m_pInput(CInput_Device::Get_Instance())
	, m_pPipe(CPipeLine::Get_Instance())
	, m_pPicking(CPicking::Get_Instance())
	, m_pLight(CLight_Manager::Get_Instance())
	, m_pKeyManager(CKey_Manager::Get_Instance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pPipe);
	Safe_AddRef(m_pLevel);
	Safe_AddRef(m_pGameObject);
	Safe_AddRef(m_pComponent);
	Safe_AddRef(m_pTimer);
	Safe_AddRef(m_pInput);
	Safe_AddRef(m_pPicking);
	Safe_AddRef(m_pLight);
	Safe_AddRef(m_pKeyManager);
}

HRESULT CGameInstacne::Initialize_Engine(_uint iNumLevel, HWND hWnd, HINSTANCE hInst)
{
	if (nullptr == m_pGameObject ||
		nullptr == m_pComponent  ||
		nullptr == m_pInput		 ||
		nullptr == m_pPicking)
		return E_FAIL;

	if (FAILED(m_pGameObject->Reserve_Container_ProtoType(iNumLevel)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pGameObject->Reserve_Container(iNumLevel)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pComponent->Reserve_Container(iNumLevel)))
	{
		return E_FAIL;
	}
	if (FAILED(m_pInput->NativeConstruct(hInst, hWnd)))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CGameInstacne::Tick(_double TimeDelta)
{
	if (nullptr == m_pLevel ||
		nullptr == m_pGameObject ||
		nullptr == m_pInput ||
		nullptr == m_pKeyManager)
	{
		return -1;
	}
	m_pInput->Invalidate_Input_Device();
	m_pKeyManager->Key_Update();

	m_pGameObject->Tick(TimeDelta);
	m_pLevel->Tick(TimeDelta);

	return _int();
}

_int CGameInstacne::Late_Tick(_double TimeDelta)
{
	if (nullptr == m_pGameObject)
	{
		return -1;
	}

	_int iProgress = m_pGameObject->Late_Tick(TimeDelta);

	return iProgress;
}

void CGameInstacne::Clear(_uint iLevel)
{
	if (nullptr == m_pGameObject ||
		nullptr == m_pComponent)
	{
		return;
	}

	m_pGameObject->Clear(iLevel);
	m_pComponent->Clear(iLevel);
}

#pragma region Graphic

HRESULT CGameInstacne::Ready_Graphic_Device(HWND hWnd, CGraphic_Device::WINMODE eMode, _uint iWinCX, _uint iWinCY, LPDIRECT3DDEVICE9 * ppGraphic_Device)
{
	if (nullptr != m_pDevice)
	{
		m_pDevice->Ready_Graphic_Device(hWnd, eMode, iWinCX, iWinCY, ppGraphic_Device);
	}

	/* 디바이스 Set을 위한 추가 */
	if (nullptr != m_pPicking)
	{
		m_pPicking->NativeConstruct(m_pDevice->Get_Device());
	}
	return S_OK;
}

void CGameInstacne::Render_Begin()
{
	if (nullptr != m_pDevice)
	{
		m_pDevice->Render_Begin();
	}
}

void CGameInstacne::Render_End(HWND hWnd)
{
	if (nullptr != m_pDevice)
	{
		m_pDevice->Render_End(hWnd);
	}
}
LPDIRECT3DDEVICE9 CGameInstacne::Get_Device()
{
	if (nullptr == m_pDevice)
	{
		return nullptr;
	}
	return m_pDevice->Get_Device();
}
LPD3DXSPRITE CGameInstacne::Get_Sprite()
{
	if (nullptr == m_pDevice)
	{
		return nullptr;
	}
	return m_pDevice->Get_Sprite();
}
LPD3DXFONT CGameInstacne::Get_Font()
{
	if (nullptr == m_pDevice)
	{
		return nullptr;
	}
	return m_pDevice->Get_Font();
}
LPD3DXLINE CGameInstacne::Get_Line()
{
	if (nullptr == m_pDevice)
	{
		return nullptr;
	}
	return m_pDevice->Get_Line();
}
#pragma endregion

#pragma region INPUT

_byte CGameInstacne::Get_DIKeyState(_byte byKey)
{
	if (nullptr == m_pInput)
	{
		return 0;
	}

	return m_pInput->Get_DIKeyState_Current(byKey);
}

_byte CGameInstacne::Get_DIMouseKeyState(CInput_Device::DIM eMouseKey)
{
	if (nullptr == m_pInput)
	{
		return 0;
	}
	return m_pInput->Get_DIMouseKeyState_Current(eMouseKey);
}

_long CGameInstacne::Get_DIMouseMoveState(CInput_Device::DIMM eMouseMove)
{
	if (nullptr == m_pInput)
	{
		return 0;
	}
	return m_pInput->Get_DIMouseMoveState_Current(eMouseMove);
}
_bool CGameInstacne::Input_KeyBoard_Down(_ubyte byKey)
{
	if (nullptr == m_pInput)
	{
		return false;
	}
	return m_pInput->Input_KeyBoard_Down(byKey);
}
_bool CGameInstacne::Input_KeyBoard_Up(_ubyte byKey)
{
	if (nullptr == m_pInput)
	{
		return false;
	}
	return m_pInput->Input_KeyBoard_Up(byKey);
}
_bool CGameInstacne::Input_KeyBoard_Pressing(_ubyte byKey)
{
	if (nullptr == m_pInput)
	{
		return false;
	}
	return m_pInput->Input_KeyBoard_Pressing(byKey);
}
_bool CGameInstacne::Input_KeyMouse_Down(CInput_Device::DIM eMouseKey)
{
	if (nullptr == m_pInput)
	{
		return false;
	}
	return m_pInput->Input_KeyMouse_Down(eMouseKey);
}
_bool CGameInstacne::Input_KeyMouse_Up(CInput_Device::DIM eMouseKey)
{
	if (nullptr == m_pInput)
	{
		return false;
	}
	return m_pInput->Input_KeyMouse_Up(eMouseKey);
}
_bool CGameInstacne::Input_KeyMouse_Pressing(CInput_Device::DIM eMouseKey)
{
	if (nullptr == m_pInput)
	{
		return false;
	}
	return m_pInput->Input_KeyMouse_Pressing(eMouseKey);
}
#pragma endregion

#pragma region KEY_MANAGER

_bool CGameInstacne::Key_Down(_ulong dwKey)
{
	if (nullptr == m_pKeyManager)
	{
		return false;
	}
	return m_pKeyManager->Key_Down(dwKey);
}
_bool CGameInstacne::Key_Up(_ulong dwKey)
{
	if (nullptr == m_pKeyManager)
	{
		return false;
	}
	return m_pKeyManager->Key_Up(dwKey);
}
_bool CGameInstacne::Key_Pressing(_ulong dwKey)
{
	if (nullptr == m_pKeyManager)
	{
		return false;
	}
	return m_pKeyManager->Key_Pressing(dwKey);
}
#pragma endregion

#pragma region Level
HRESULT CGameInstacne::SetUp_Level(CLevel * pLevel)
{
	if (nullptr == m_pLevel)
	{
		return E_FAIL;
	}
	return m_pLevel->SetUp_Level(pLevel);
}

HRESULT CGameInstacne::Render_Level()
{
	if (nullptr == m_pLevel)
	{
		return E_FAIL;
	}
	return m_pLevel->Render_Level();
}

_matrix CGameInstacne::Get_Transform(D3DTRANSFORMSTATETYPE eState)
{
	if (nullptr == m_pPipe)
	{
		return _matrix();
	}
	return m_pPipe->Get_Transform(eState);
}

_float3 CGameInstacne::Get_CamPosition()
{
	if (nullptr == m_pPipe)
	{
		return _float3();
	}
	return m_pPipe->Get_CamPosition();
}

#pragma endregion

#pragma region GameObject

HRESULT CGameInstacne::Add_Prototype_Object(_uint iTypeNum, const _tchar * pPrototypeTag, CGameObject * pGameObject)
{
	if (nullptr == m_pGameObject)
	{
		return E_FAIL;
	}
	return m_pGameObject->Add_Prototype(iTypeNum,pPrototypeTag,pGameObject);
}

HRESULT CGameInstacne::Add_GameObject_Clone(_uint iNumLevel, const _tchar * pPrototypeTag, const _tchar * pLayerTag, void * pArg)
{
	if (nullptr == m_pGameObject)
	{
		return E_FAIL;
	}
	return m_pGameObject->Add_GameObject_Clone(iNumLevel,pPrototypeTag,pLayerTag,pArg);
}

CComponent * CGameInstacne::Find_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndexNum)
{
	if (nullptr == m_pGameObject)
	{
		return nullptr;
	}
	return m_pGameObject->Find_Component(iLevelIndex,pLayerTag,pComponentTag,iIndexNum);
}

CGameObject * CGameInstacne::Find_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndexNum)
{
	if (nullptr == m_pGameObject)
	{
		return nullptr;
	}
	return m_pGameObject->Find_GameObject(iLevelIndex,pLayerTag,iIndexNum);
}

HRESULT CGameInstacne::Delete_Prototype(_uint iTypeNum, const _tchar * pPrototypeTag)
{
	if (nullptr == m_pGameObject)
	{
		return E_FAIL;
	}
	return m_pGameObject->Delete_Prototype(iTypeNum,pPrototypeTag);
}

HRESULT CGameInstacne::Delete_GameObject(_uint iTypeNum, const _tchar * pLayerTag, _int iIndex)
{
	if (nullptr == m_pGameObject)
	{
		return E_FAIL;
	}
	return m_pGameObject->Delete_GameObject(iTypeNum, pLayerTag, iIndex);
}

HRESULT CGameInstacne::Delete_GameObjects(_uint iTypeNum, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject)
	{
		return E_FAIL;
	}
	return m_pGameObject->Delete_GameObjects(iTypeNum, pLayerTag);
}

HRESULT CGameInstacne::Delete_GameObject_End(_uint iTypeNum, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject)
	{
		return E_FAIL;
	}
	return m_pGameObject->Delete_GameObject_End(iTypeNum, pLayerTag);
}

_uint CGameInstacne::Get_GameObject_ListSize(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (nullptr == m_pGameObject)
	{
		return 0;
	}
	return m_pGameObject->Get_GameObject_ListSize(iLevelIndex,pLayerTag);
}

#pragma endregion

#pragma region Component

HRESULT CGameInstacne::Add_Prototype_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, CComponent * pPrototype)
{
	if (nullptr == m_pComponent)
	{
		return E_FAIL;
	}
	return m_pComponent->Add_Prototype(iLevelIndex,pPrototypeTag,pPrototype);
}

CComponent * CGameInstacne::Clone_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, void * pArg)
{
	if (nullptr == m_pComponent)
	{
		return nullptr;
	}
	return m_pComponent->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
}

HRESULT CGameInstacne::Add_Timers(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer)
	{
		return E_FAIL;
	}
	return m_pTimer->Add_Timers(pTimerTag);
}

_float CGameInstacne::Compute_Time(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer)
	{
		return 0.f;
	}
	return m_pTimer->Compute_Time(pTimerTag);
}

_float CGameInstacne::Get_Time(const _tchar * pTimerTag)
{
	if (nullptr == m_pTimer)
	{
		return 0.f;
	}
	return m_pTimer->Get_Time(pTimerTag);
}

HRESULT CGameInstacne::Compute_MouseCursorPosInWorld(HWND hWnd)
{
	if (nullptr == m_pPicking)
	{
		return E_FAIL;
	}

	return m_pPicking->Compute_MouseCursorPosInWorld(hWnd);
}

_float3 * CGameInstacne::Compute_PickingPoint(CVIBuffer * pVIBuffer, _matrix WorldMatrix)
{
	if (nullptr == m_pPicking)
	{
		return nullptr;
	}

	return m_pPicking->Compute_PickingPoint(pVIBuffer, WorldMatrix);
}

_float3 * CGameInstacne::Compute_PickingPoint(LPD3DXMESH pMesh, _matrix WorldMatrix)
{
	if (nullptr == m_pPicking)
	{
		return nullptr;
	}
	return m_pPicking->Compute_PickingPoint(pMesh, WorldMatrix);
}

HRESULT CGameInstacne::Compute_WindowCenterPosition()
{
	if (nullptr == m_pPicking)
	{
		return E_FAIL;
	}
	return m_pPicking->Compute_WindowCenterPosition();
}

_float3 * CGameInstacne::Compute_CenterPoint(CVIBuffer * pVIBuffer, _matrix WorldMatrix)
{
	if (nullptr == m_pPicking)
	{
		return nullptr;
	}
	return m_pPicking->Compute_CenterPoint(pVIBuffer, WorldMatrix);
}

_float3 * CGameInstacne::Compute_CenterPoint(CCollider * pCollider, _matrix WorldMatrix)
{
	if (nullptr == m_pPicking)
	{
		return nullptr;
	}
	return m_pPicking->Compute_CenterPoint(pCollider,WorldMatrix);
}

_float3 * CGameInstacne::Compute_CenterPoint(LPD3DXMESH pMesh, _matrix WorldMatrix)
{
	if (nullptr == m_pPicking)
	{
		return nullptr;
	}
	return m_pPicking->Compute_CenterPoint(pMesh, WorldMatrix);
}

D3DLIGHT9 CGameInstacne::Get_LightDesc(_uint iIndex)
{
	if (iIndex >= m_pLight->Get_IndexSize())
	{
		MSGBOX("Light Index is over size");
		return D3DLIGHT9();
	}

	return m_pLight->Get_LightDesc();
}

HRESULT CGameInstacne::Level_Relese_Light()
{
	if (nullptr == m_pLight)
	{
		return E_FAIL;
	}
	return m_pLight->Level_Relese_Light();
}

HRESULT CGameInstacne::Add_Light(LPDIRECT3DDEVICE9 pGraphic_Device, const D3DLIGHT9 & LightDesc)
{
	if (nullptr == m_pLight)
	{
		return E_FAIL;
	}


	return m_pLight->Add_Light(pGraphic_Device, LightDesc);
}

#pragma endregion

void CGameInstacne::Release_Engine()
{
	if (0 != CGameInstacne::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release CGameInstance");
	}
	if (0 != CLevel_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CLevel_Manager");
	}
	if (0 != CGameObject_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CGameObject_Manger");
	}
	if (0 != CPipeLine::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Faild to Relese CPipeLine");
	}
	if (0 != CComponent_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CComponent_Manager");
	}
	if (0 != CTime_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release CTimer_Manager");
	}
	if (0 != CLight_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CLight_Manager");
	}
	if (0 != CKey_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CKey_Manager");
	}
	if (0 != CTarget_Manager::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CTarget_Manager");
	}
	if (0 != CInput_Device::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CInput_Device");
	}
	if (0 != CPicking::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Relese CPicking");
	}
	if (0 != CGraphic_Device::Get_Instance()->Destroy_Instance())
	{
		MSGBOX("Failed to Release CGraphic_Device");
	}
}

void CGameInstacne::Free()
{
	Safe_Release(m_pLight);
	Safe_Release(m_pComponent);
	Safe_Release(m_pGameObject);
	Safe_Release(m_pLevel);
	Safe_Release(m_pTimer);
	Safe_Release(m_pKeyManager);
	Safe_Release(m_pInput);
	Safe_Release(m_pPipe);
	Safe_Release(m_pPicking);
	Safe_Release(m_pDevice);
}
