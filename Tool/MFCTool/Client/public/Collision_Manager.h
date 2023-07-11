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

private:
	virtual void Free() override;
};
END
