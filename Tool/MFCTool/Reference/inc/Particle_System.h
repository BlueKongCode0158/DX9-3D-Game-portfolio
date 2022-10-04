#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CParticle_System : public CBase
{
protected:
	explicit CParticle_System(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CParticle_System(const CParticle_System& rhs);
	virtual  ~CParticle_System() = default;
public:

};
END
