#include "..\public\Input_Device.h"


IMPLEMENT_SINGLETON(CInput_Device)
CInput_Device::CInput_Device()
{

}

_bool CInput_Device::Input_KeyBoard_Down(_ubyte byKey)
{
	if ((Get_DIKeyState_Current(byKey) & 0x80) && !(Get_DIKeyState_Preview(byKey) & 0x80))
	{
		return true;
	}

	return false;
}

_bool CInput_Device::Input_KeyBoard_Up(_ubyte byKey)
{
	if (!(Get_DIKeyState_Current(byKey) & 0x80) && (Get_DIKeyState_Preview(byKey) & 0x80))
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Input_KeyBoard_Pressing(_ubyte byKey)
{
	if (Get_DIKeyState_Current(byKey) & 0x80)
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Input_KeyMouse_Down(DIM eMouseKey)
{
	if ((Get_DIMouseKeyState_Current(eMouseKey) & 0x80) && !(Get_DIMouseKeyState_Preview(eMouseKey) & 0x80))
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Input_KeyMouse_Up(DIM eMouseKey)
{
	if (!(Get_DIMouseKeyState_Current(eMouseKey) & 0x80) && (Get_DIMouseKeyState_Preview(eMouseKey) & 0x80))
	{
		return true;
	}
	return false;
}

_bool CInput_Device::Input_KeyMouse_Pressing(DIM eMouseKey)
{
	if (Get_DIMouseKeyState_Current(eMouseKey) & 0x80)
	{
		return true;
	}
	return false;
}

HRESULT CInput_Device::NativeConstruct(HINSTANCE hInst, HWND hWnd)
{
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&m_pInputSDK), nullptr)))
	{
		return E_FAIL;
	}

#pragma region KEYBOARD
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, nullptr)) ||
		FAILED(m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)) ||
		FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)) ||
		FAILED(m_pKeyboard->Acquire()))
	{
		return E_FAIL;
	}
#pragma endregion

#pragma region MOUSE
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)) ||
		FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)) ||
		FAILED(m_pMouse->Acquire()))
	{
		return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

_int CInput_Device::Invalidate_Input_Device()
{
	m_iCurrentState = !m_iCurrentState;

	if (FAILED(m_pKeyboard->GetDeviceState(256, &m_KeyState[m_iCurrentState])))
	{
		return -1;
	}
	if (FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState[m_iCurrentState])))
	{
		return -1;
	}

	return _int();
}

void CInput_Device::Free()
{
	Safe_Release(m_pKeyboard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}
