#pragma once
#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CCollision_Manager final
	: public CBase
{
	DECLARE_SINGLETON(CCollision_Manager)
public:
	explicit CCollision_Manager();
	virtual  ~CCollision_Manager() = default;
public:
	_bool	Create(_int iStage);
	_bool	Clear(_int iStage);
private:
	map<const _tchar*, list<CCollider*>>	m_pColiders[LEVEL_END];
	typedef	map<const _tchar*, list<CCollider*>> COLLIDERS;
private:
	_int	m_iCurStage = 0;
private:
	virtual void Free() override;
};
END
