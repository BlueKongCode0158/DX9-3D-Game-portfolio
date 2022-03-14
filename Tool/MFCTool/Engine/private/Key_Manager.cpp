#include "..\public\Key_Manager.h"
#include "Input_Device.h"


IMPLEMENT_SINGLETON(CKey_Manager)
CKey_Manager::CKey_Manager()
	:m_dwKey(0)
	,m_dwKeyDown(0)
	,m_dwKeyUp(0)
{

}

void CKey_Manager::Key_Update()
{
	m_dwKey = 0;
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_W) & 0x80)
	{
		m_dwKey |= KEY_W;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_A) & 0x80)
	{
		m_dwKey |= KEY_A;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_S) & 0x80)
	{
		m_dwKey |= KEY_S;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_D) & 0x80)
	{
		m_dwKey |= KEY_D;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_TAB) & 0x80)
	{
		m_dwKey |= KEY_TAB;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_LSHIFT) & 0x80)
	{
		m_dwKey |= KEY_SHIFT;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_R) & 0x80)
	{
		m_dwKey |= KEY_R;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_Q) & 0x80)
	{
		m_dwKey |= KEY_Q;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_LCONTROL) & 0x80)
	{
		m_dwKey |= KEY_CTRL;
	}
	if (CInput_Device::Get_Instance()->Get_DIKeyState_Current(DIK_ESCAPE) & 0x80)
	{
		m_dwKey |= KEY_ESCAPE;
	}
	if (CInput_Device::Get_Instance()->Get_DIMouseKeyState_Current(CInput_Device::DIM_LBUTTON) & 0x80)
	{
		m_dwKey |= KEY_LBUTTON;
	}
	if (CInput_Device::Get_Instance()->Get_DIMouseKeyState_Current(CInput_Device::DIM_RBUTTON) & 0x80)
	{
		m_dwKey |= KEY_RBUTTON;
	}
}

_bool CKey_Manager::Key_Down(_ulong dwKey)
{
	if (m_dwKey & dwKey && !(m_dwKeyDown & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	else if (!(m_dwKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}
	return false;
}

_bool CKey_Manager::Key_Up(_ulong dwKey)
{
	if (m_dwKey & dwKey)
	{
		m_dwKeyUp |= dwKey;
		return false;
	}
	else if (m_dwKeyUp & dwKey)
	{
		m_dwKeyUp ^= dwKey;
		return true;
	}
	return false;
}

_bool CKey_Manager::Key_Pressing(_ulong dwKey)
{
	if (m_dwKey & dwKey)
	{
		return true;
	}
	return false;
}

void CKey_Manager::Free()
{
}
