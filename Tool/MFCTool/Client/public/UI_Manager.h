#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CUI;
END

BEGIN(Client)
class CUI_Manager final
	: public CBase
{
	DECLARE_SINGLETON(CUI_Manager)
public:
	explicit CUI_Manager();
	virtual  ~CUI_Manager() = default;
private:
	map<const _tchar*, list<CUI*>>	m_pUIs[LEVEL_END];
	typedef	map<const _tchar*, list<CUI*>> UI;
private:
	_uint	m_iCurStage = 0;
private:
	virtual void Free() override;
};
END

