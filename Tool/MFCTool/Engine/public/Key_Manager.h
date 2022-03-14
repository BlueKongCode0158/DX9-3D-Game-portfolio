#pragma once
#include "Base.h"

#define KEY_LBUTTON	0x0001
#define KEY_RBUTTON	0x0002
#define KEY_W		0x0004
#define KEY_A		0x0008
#define KEY_S		0x0010
#define KEY_D		0x0020
#define KEY_R		0x0040
#define KEY_Q		0x0080
#define KEY_TAB		0x0100
#define KEY_CTRL	0x0200
#define KEY_SHIFT	0x0400
#define KEY_ESCAPE	0x0800



BEGIN(Engine)
class CKey_Manager : public CBase
{
	DECLARE_SINGLETON(CKey_Manager)
public:
	explicit	CKey_Manager();
	virtual		~CKey_Manager() = default;
public:
	void  Key_Update();
	_bool Key_Down(_ulong dwKey);
	_bool Key_Up(_ulong dwKey);
	_bool Key_Pressing(_ulong dwKey);
private:
	_ulong	m_dwKey;
	_ulong	m_dwKeyDown;
	_ulong	m_dwKeyUp;
public:
	virtual	void	Free() override;
};
END