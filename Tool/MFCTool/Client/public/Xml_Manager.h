#pragma once
#include "Base.h"

BEGIN(Client)
class CXml_Manager final : public CBase
{
	DECLARE_SINGLETON(CXml_Manager)
private:
	explicit CXml_Manager();
	virtual  ~CXml_Manager() = default;
public:
	void Read_XmlFile(const _tchar* pFilePath);
public:
	virtual void Free() override;
};
END
