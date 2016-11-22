// Author: Karm

#ifndef CResourceServerManager_h
#define CResourceServerManager_h

#include <Scripting/ResourceSystem/CResourceManager.h>
#include <Network/CNetworkModule.h>
#include <map>

#define CLIENT_FILE_DIRECTORY "client_files"

class CResourceServerManager : public CResourceManager
{
private:
	static CResourceServerManager*	s_pInstance;
	std::map<CString, FileList*> m_FileLists;

public:
	CResourceServerManager();
	~CResourceServerManager();

	static CResourceServerManager * GetInstance() { return s_pInstance; }

	void PrepareResourceFilesForClient(CResource* pResource);

	FileList* GetResourceFileList(CResource* pResource);

	void DeleteFileList(CResource * pResource);

	bool StartServerResource(CResource * pResource);

	void UnloadResource(CResource* pResource);
};

#endif // CResourceServerManager_h