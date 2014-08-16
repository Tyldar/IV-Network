// Author: Karm

#include "CResourceServerManager.h"
#include <SharedUtility.h>
#include <tinyxml/tinyxml.h>
#include <tinyxml/ticpp.h>
#include <CLogFile.h>
#include <Scripting/CLuaVM.h>
#include <Scripting/CSquirrelVM.h>

CResourceServerManager* CResourceServerManager::s_pInstance = NULL;

CResourceServerManager::CResourceServerManager()
: CResourceManager("resources", CResourceFile::eResourceType::RESOURCE_FILE_TYPE_SERVER_SCRIPT)
{
	s_pInstance = this;
}

CResourceServerManager::~CResourceServerManager(){

}

// Copy files of the resource for clients into the directory of downloads for clients
void CResourceServerManager::PrepareResourceFilesForClient(CResource* pResource){
	// Only generates the FileList if the resource has client scripts
	if (pResource && pResource->HasClientResourceFilesScripts())
	{
		// We create the XML document to generate the meta.xml for the clients
		TiXmlDocument doc;
		TiXmlElement* metaElement = new TiXmlElement("meta");
		FileList* fileList = new FileList();
		FileListNodeContext context;
		for (auto pFile : *pResource->GetResourceFiles())
		{
			if (pFile->GetType() == CResourceFile::RESOURCE_FILE_TYPE_CLIENT_SCRIPT)
			{
				// If the meta element is not generated...
				if (metaElement->NoChildren()){
					// Create the info element
					TiXmlElement* infoElement = new TiXmlElement("info");
					if (pResource->GetResourceScriptLanguage() == eResourceScriptLanguage::LUA_RESOURCE)
						infoElement->SetAttribute("scriptType", "Lua");
					else if (pResource->GetResourceScriptLanguage() == eResourceScriptLanguage::SQUIRREL_RESOURCE)
						infoElement->SetAttribute("scriptType", "Squirrel");
					metaElement->LinkEndChild(infoElement);
				}

				// script element...
				TiXmlElement* scriptElement = new TiXmlElement("script");
				scriptElement->SetAttribute("src", pFile->GetName());
				scriptElement->SetAttribute("type", "client");
				metaElement->LinkEndChild(scriptElement);

				// We add the file to the FileList that the client will download after
				fileList->AddFile(CString(pResource->GetResourceDirectoryPath() + "/" + pFile->GetName()).Get(), pFile->GetName(), context);
			}
		}
		if (!metaElement->NoChildren()){
			// First, we copy the files into the resource directory. NOTE: if any folder does not exists, the function WriteDataToDisk create it...
			fileList->WriteDataToDisk(SharedUtility::GetAbsolutePath(CLIENT_FILE_DIRECTORY "/resources/%s", pResource->GetName().Get()).Get());
			// Next, we save the XML document. NOTE: we need to create the folder before save the XML document. If any folder does not exists TinyXML does not create it!!!!
			doc.LinkEndChild(metaElement);
			doc.SaveFile(CString(CLIENT_FILE_DIRECTORY "/resources/%s/meta.xml", pResource->GetName().Get()).Get());
			// After, we add the file to the FileList
			fileList->AddFile(SharedUtility::GetAbsolutePath(CLIENT_FILE_DIRECTORY "/resources/%s/meta.xml", pResource->GetName().Get()), "meta.xml", context);
			// Finally, we save the FileList
			m_FileLists.insert(std::pair<CString, FileList*>(pResource->GetName(), fileList));
		}
	}
}

FileList* CResourceServerManager::GetResourceFileList(CResource* pResource){
	auto itFileList = m_FileLists.find(pResource->GetName());
	if (itFileList != m_FileLists.end()){
		return (*itFileList).second;
	}
	return nullptr;
}

void CResourceServerManager::DeleteFileList(CResource * pResource){
	auto itFileList = m_FileLists.find(pResource->GetName());
	if (itFileList != m_FileLists.end()){
		m_FileLists.erase(itFileList);
	}
}

bool CResourceServerManager::StartServerResource(CResource* pResource){
	if (StartResource(pResource)){
		PrepareResourceFilesForClient(pResource);
		return true;
	}
	return false;
}

void CResourceServerManager::UnloadResource(CResource* pResource)
{
	DeleteFileList(pResource);
	Unload(pResource);
}