#pragma once

namespace Engine
{
	class CTagFinder
	{
	public:
		explicit CTagFinder(const _tchar* pTargetTag)
			:m_pTargetTag(pTargetTag)
		{}
		~CTagFinder() = default;

	public:
		template<class T>
		bool operator () (T Pair)
		{
			if (0 == lstrcmp(Pair.first, m_pTargetTag))
			{
				return true;
			}
			return false;
		}

	private:
		const _tchar*	m_pTargetTag = nullptr;
	};

	template<typename T>
	class CGet_Instance
	{
	public:
		CGet_Instance(T* Instance) { m_pInstance = GET_INSTANCE(typeid(Instance).name()); }
		~CGet_Instance() { RELEASE_INSTANCE(typeid(Instance).name()); }
	public:
		T*	Get_Pointer() const     
		{
			return m_pInstance;
		}
	private:
		T*	m_pInstance = nullptr;
	};

}
