//========= IV: Network - https://github.com/GTA-Network/IV-Network ============
//
// File: CResourceScriptFile.cpp
// Project: Shared
// Author: xForce <xf0rc3.11@gmail.com>
// License: See LICENSE in root directory
//
//==============================================================================

#include "CResourceScriptFile.h"


CResourceScriptFile::CResourceScriptFile(CResource * resource, const char * szShortName, const char * szResourceFileName, CResourceFile::eResourceType resourceType)
	: CResourceFile(resource, szShortName, szResourceFileName)
{
	m_type = resourceType;
}

CResourceScriptFile::~CResourceScriptFile()
{

}

bool CResourceScriptFile::Start()
{
	if (m_resource->GetVM()->LoadScript(m_strShortName))
	{
		// Call the scripting event
		CScriptArguments args;
		args.push(m_strShortName.Get());
		CEvents::GetInstance()->Call("scriptLoaded", &args, CEventHandler::eEventType::NATIVE_EVENT, m_resource->GetVM());
		return true;
	}
	return true;
}

bool CResourceScriptFile::Stop()
{
	// Nothing can be done here
	return true;
}