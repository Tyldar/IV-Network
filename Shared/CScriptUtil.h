// CFolderFiles.h
// Author: Karm

#pragma once

#include <iostream>
#include <direct.h>
#include <io.h>
#include <sys/stat.h>
#include "CScriptUtil.h"

static class CScriptUtil
{
public:
	static bool ExistsFolder(const char * folderName){
		if (_access(folderName, 0) == 0){
			struct stat status;
			stat(folderName, &status);

			if (status.st_mode & S_IFDIR)
				return true;
		}
		return false;
	}

	static bool ExistsFile(const char * fileName){
		FILE* fp = fopen(fileName, "r");
		if (fp) {
			fclose(fp);
			return true;
		}
		return false;
	}
};