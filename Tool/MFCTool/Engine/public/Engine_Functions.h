#pragma once
namespace Engine
{
	float static GetRandomFloat(float fLowBound, float fHighBound)
	{
		if (fLowBound >= fHighBound)
		{
			return fLowBound;
		}

		float fTemp = (rand() % 10000) * 0.0001f;

		return (fTemp*(fHighBound - fLowBound)) + fLowBound;
	}

	void static GetRandomVector(D3DXVECTOR3& rOut, D3DXVECTOR3& rMin, D3DXVECTOR3& rMax)
	{
		rOut.x = GetRandomFloat(rMin.x, rMax.x);
		rOut.y = GetRandomFloat(rMin.y, rMax.y);
		rOut.z = GetRandomFloat(rMin.z, rMax.z);
	}

	void static GetRandomVector_Circle(D3DXVECTOR3& rOut, float fRadian, D3DXVECTOR3* pCenter)
	{
		D3DXVECTOR3 vDir = D3DXVECTOR3(GetRandomFloat(0.f, 1.f), GetRandomFloat(0.f, 1.f), 0.f);
		D3DXVec3Normalize(&vDir, &vDir);

		vDir *= fRadian;
		if (nullptr != pCenter)
		{
			vDir += *pCenter;
		}

		rOut = vDir;
	}

	void static GetRandomVector_Circle_AxisX(D3DXVECTOR3& rOut, float fRadian, D3DXVECTOR3* pCenter)
	{

	}

	void static GetRandomVector_Circle_AxisY()
	{

	}

	void static GetRandomVector_Circle_AxisZ()
	{

	}

	void static GetRandomDirVertor(D3DXVECTOR3& rOut, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd)
	{
		vEnd.y += vStart.y;

		rOut.x = vEnd.x - vStart.x;
		rOut.y = vEnd.y - vStart.y;
		rOut.z = vEnd.z - vStart.z;
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
