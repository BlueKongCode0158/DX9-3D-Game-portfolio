#pragma once
namespace Engine
{
	float static GetRandomFloat(float flowBound, float fhighBound)
	{
		if (flowBound >= fhighBound)
		{
			return flowBound;
		}

		float fTemp = (rand() % 10000) * 0.0001f;

		return (fTemp*(fhighBound - flowBound)) + flowBound;
	}

	void static GetRandomVector(D3DXVECTOR3& rOut, D3DXVECTOR3& rMin, D3DXVECTOR3& rMax)
	{
		rOut.x = GetRandomFloat(rMin.x, rMax.x);
		rOut.y = GetRandomFloat(rMin.y, rMax.y);
		rOut.z = GetRandomFloat(rMin.z, rMax.z);
	}

	void static GetRandomDirVertor(D3DXVECTOR3& rOut, D3DXVECTOR3& rAxis, float fRadian)
	{
		
	}
}

template <typename T>
void Safe_Delete(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& pPointer)
{
	if (nullptr != pPointer)
	{
		delete[] pPointer;
		pPointer = nullptr;
	}
}

template <typename T>
unsigned long Safe_AddRef(T& pInstance)
{
	unsigned long		dwRefcnt = 0;

	if (nullptr != pInstance)
		dwRefcnt = pInstance->AddRef();

	return dwRefcnt;
}


template <typename T>
unsigned long Safe_Release(T& pInstance)
{
	unsigned long		dwRefcnt = 0;

	if (nullptr != pInstance)
	{
		dwRefcnt = pInstance->Release();

		if (0 == dwRefcnt)
			pInstance = nullptr;
	}

	return dwRefcnt;
}
