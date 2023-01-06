#pragma once
#include "Base.h"
#include "Attribute.h"
#include "Particle_System.h"

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
public:
	HRESULT	DeleteAll_ParticleSystem(const _tchar* pLayerTag);
	HRESULT	Delete_ParticleSystem(const _tchar* pLayerTag, _int iIndex);
private:
	CParticle_System*			Find_PrototypeParticle(const _tchar* pLayerTag);
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

public:
	typedef struct ParticleInfo
	{
		CAttribute::PDESC			m_ParticleDesc;
		CParticle_System::PSDESC	m_SystemDesc;
	}PINFO;
};
END

