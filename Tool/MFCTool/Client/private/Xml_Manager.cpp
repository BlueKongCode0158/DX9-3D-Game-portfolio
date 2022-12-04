#include "stdafx.h"
#include "tinyxml2.h"
#include "Client_Defines.h"
#include "..\public\Xml_Manager.h"

IMPLEMENT_SINGLETON(CXml_Manager)

CXml_Manager::CXml_Manager()
{
}

void CXml_Manager::Read_XmlFile(const _tchar* pFilePath)
{
	tinyxml2::XMLError xmlError = tinyxml2::XML_ERROR_COUNT;
	tinyxml2::XMLDocument doc;

	char	chTemp[MAX_PATH];
	int		iLen = MAX_PATH;
	WideCharToMultiByte(CP_ACP, 0, pFilePath, iLen, chTemp, iLen, NULL, NULL);

	xmlError = doc.LoadFile(chTemp);
	if (xmlError == tinyxml2::XML_SUCCESS)
	{
		MSGBOX("Parse Success");
		tinyxml2::XMLNode*	  pNode		= doc.FirstChild();
		tinyxml2::XMLElement* pElement  = pNode->FirstChildElement();

		int iType	= 0;
		int iID		= 0;
		int iLevel	= 0;
		const char* pText = nullptr;

		while (pElement != NULL)
		{
			
			for (const tinyxml2::XMLAttribute* pAttribute = pElement->FirstAttribute(); pAttribute != NULL; pAttribute = pAttribute->Next())
			{
				if (strcmp(pAttribute->Name(), "eType") == 0)
				{
					pAttribute->QueryIntValue(&iType);
				}
				else if (strcmp(pAttribute->Name(), "eID") == 0)
				{
					pAttribute->QueryIntValue(&iID);
				}
				else if (strcmp(pAttribute->Name(), "Text") == 0)
				{
					pText = pAttribute->Value();
				}
			}
			pElement = pElement->NextSiblingElement();
		}
		cout << iType << '\n';
		cout << iID << '\n';
		cout << iLevel << '\n';
		cout << pText << '\n';

	}
}

void CXml_Manager::Free()
{
}
