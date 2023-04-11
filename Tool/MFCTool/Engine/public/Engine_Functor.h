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
	private:
		CGet_Instance(T) { m_pInstance = GET_INSTANCE(T) }
		~CGet_Instance() { RELEASE_INSTANCE(T) }
	private:
		T*	m_pInstance = nullptr;
	};
}
