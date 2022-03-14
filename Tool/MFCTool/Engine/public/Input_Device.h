#pragma once
#include "Base.h"

BEGIN(Engine)
class CInput_Device final : public CBase
{
	DECLARE_SINGLETON(CInput_Device)
public:
	enum DIM { DIM_LBUTTON, DIM_RBUTTON, DIM_WHEELBUTTON, DIM_XBUTTON};
	enum DIMM { DIMM_X, DIMM_Y, DIMM_WHEEL };
private:
	explicit CInput_Device();
	virtual  ~CInput_Device() = default;
public:
	_ubyte Get_DIKeyState_Current(_ubyte byKey)
	{
		return m_KeyState[m_iCurrentState][byKey];
	}
	_ubyte Get_DIMouseKeyState_Current(DIM eMouseKey)
	{
		return m_MouseState[m_iCurrentState].rgbButtons[eMouseKey];
	}
	_long Get_DIMouseMoveState_Current(DIMM eMouseMove)
	{
		return (reinterpret_cast<_long*>(&m_MouseState[m_iCurrentState]))[eMouseMove];
	}
	_ubyte Get_DIKeyState_Preview(_ubyte byKey)
	{
		return m_KeyState[!m_iCurrentState][byKey];
	}
	_ubyte Get_DIMouseKeyState_Preview(DIM eMouseKey)
	{
		return m_MouseState[!m_iCurrentState].rgbButtons[eMouseKey];
	}
	_long Get_DIMouseMoveState_Preview(DIMM eMouseMove)
	{
		return (reinterpret_cast<_long*>(&m_MouseState[!m_iCurrentState]))[eMouseMove];
	}

public:
	_bool	Input_KeyBoard_Down(_ubyte byKey);
	_bool	Input_KeyBoard_Up(_ubyte byKey);
	_bool	Input_KeyBoard_Pressing(_ubyte byKey);
public:
	_bool	Input_KeyMouse_Down(DIM eMouseKey);
	_bool	Input_KeyMouse_Up(DIM eMouseKey);
	_bool	Input_KeyMouse_Pressing(DIM eMouseKey);
public:
	HRESULT NativeConstruct(HINSTANCE hInst, HWND hWnd);
	_int	Invalidate_Input_Device();
private:
	LPDIRECTINPUT8			m_pInputSDK		= nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyboard		= nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse		= nullptr;
private:
	DIMOUSESTATE			m_MouseState[2];
	_ubyte					m_KeyState[2][256];
private:
	_int					m_iCurrentState = 0;
public:
	virtual void Free();
};
END

