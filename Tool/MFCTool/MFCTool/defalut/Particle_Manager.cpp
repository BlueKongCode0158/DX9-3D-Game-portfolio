#include "stdafx.h"
#include "MFC_Defines.h"
#include "Particle_Manager.h"


IMPLEMENT_SINGLETON(CParticle_Manager)
CParticle_Manager::CParticle_Manager()
{
}

HRESULT CParticle_Manager::Create_Particle(const _tchar *)
{
	return E_NOTIMPL;
}

HRESULT CParticle_Manager::Delete_Particle(const _tchar *, _int iIndex)
{
	return S_OK;
}

_int CParticle_Manager::Tick(_float fTime_Delta)
{
	return _int();
}

_int CParticle_Manager::Late_Tick(_float fTime_Delta)
{
	return _int();
}

HRESULT CParticle_Manager::Render_Particles()
{
	return E_NOTIMPL;
}
