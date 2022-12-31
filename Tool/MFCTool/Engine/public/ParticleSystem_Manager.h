#pragma once
#include "Base.h"

BEGIN(Engine)
class CParticle_System;
class CParticleSystem_Manager final : public CBase
{
	DECLARE_SINGLETON(CParticleSystem_Manager)
private:
	explicit CParticleSystem_Manager();
	virtual ~CParticleSystem_Manager() = default;
public:
	HRESULT	Add_ParticleSystem(const _tchar* pLayerTag);
	HRESULT	Add_ParticleSystem(const _tchar* pLayerTag, void* pArg);
	HRESULT	Delete_ParticleSystem(const _tchar* pLayerTag);
private:
	CParticle_System*	Find_PrototypeParticle(const _tchar* pLayerTag);
	list<CParticle_System*>*	Find_Particles(const _tchar* pLayerTag);
private:
	void	Update_Index();
private:
	HRESULT	Add_Layer(const _tchar* pLayerTag, void* pArg);
private:
	map<const _tchar*, list<CParticle_System*>>	m_mapParticles;
	typedef map<const _tchar*, list<CParticle_System*>> PARTICLES;
private:
	map<const _tchar*, CParticle_System*> m_mapPrototypeParticles;
	typedef map<const _tchar*, CParticle_System*> PROTOTYPE_PARTICLES;
public:
	virtual void Free() override;
};
END

