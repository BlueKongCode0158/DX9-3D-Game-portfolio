#pragma once
#include "Base.h"

BEGIN(Engine)
class CParticle;
END

BEGIN(MFC_TOOL)
class CParticle_Manager : public CBase
{
	DECLARE_SINGLETON(CParticle_Manager)
private:
	explicit CParticle_Manager();
	virtual  ~CParticle_Manager() = default;
public:
	HRESULT Create_Particle(const _tchar*);
	HRESULT Delete_Particle(const _tchar*, _int iIndex);
public:
	_int	Tick(_float fTime_Delta);
	_int	Late_Tick(_float fTime_Delta);
	HRESULT Render_Particles();
private:

private:
	map<const _tchar*, list<CParticle*>> map_Particles;
public:
	virtual	void Free() override;
};
END
