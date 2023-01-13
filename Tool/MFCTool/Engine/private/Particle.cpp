#include "..\public\Particle.h"
#include "VIBuffer_Point.h"
#include "Transform.h"
#include "Attribute.h"
#include "Shader.h"

CParticle::CParticle(LPDIRECT3DDEVICE9 pGraphicDevice)
	:CComponent(pGraphicDevice)
{
}

CParticle::CParticle(const CParticle & rhs)
	:CComponent(rhs)
{
}

HRESULT CParticle::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CParticle::NativeConstruct(void * pArg)
{
	return S_OK;
}

void CParticle::Free()
{
	__super::Free();
}
