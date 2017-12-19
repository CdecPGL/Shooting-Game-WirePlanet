#include "FileIOUtilFuncs.h"
#include<Windows.h>

using namespace std;

namespace FileIOUtilFuncs{
	int ExcuteFile(const string& name){
		DWORD i;
		DWORD dwLen;
		char pszPath[1024];
		GetCurrentDirectory(1024, pszPath);

		/*dwLen = GetModuleFileName(NULL, pszPath, 1024);
		for (i = dwLen; i > 0; i--)
		{
			if (pszPath[i] != '\\')
				continue;
			pszPath[i + 1] = NULL;
			break;
		}*/

		HINSTANCE res = ShellExecute(NULL, "open", name.c_str(), NULL, pszPath, SW_NORMAL);
		if ((int)res > 32){
			printf("FileUOUtilFuncs::ExcuteFile ファイル\"%s\"を開きました。\n", name.c_str());
			return 0;
		}
		else{
			printf("FileUOUtilFuncs::ExcuteFile ファイル\"%s\"を開けませんでした。(ShellExecute=%d)\n", name.c_str(), (int)res);
			return -1;
		}
	}
}
