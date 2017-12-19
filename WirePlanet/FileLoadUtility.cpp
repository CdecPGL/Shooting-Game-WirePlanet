#include "FileLoadUtility.h"

#include"FileLoadManager.h"
#include"SVLoader.h"
#include"DataScript.h"
#include"lua.hpp"
#include"Action.h"

//FileLoadUtil::LoadedData FileLoadUtil::loaded_data = FileLoadUtil::LoadedData::Set;

void FileLoadUtil::SetLoadedDataTreat(LoadedData ld){
	loaded_data = ld;
}

int FileLoadUtil::CreateGraphFromFLM(const std::string& fn, bool unload_flag){
	using namespace cdec;
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int gh = CreateGraphFromMem(file->GetTopPointer(), (signed)file->GetSize());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	return gh;
}

int FileLoadUtil::CreateDivGraphFromFLM(const std::string& fn, int all_num, int x_num, int y_num, int width, int height, int* buf, bool unload_flag){
	using namespace cdec;
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int res = CreateDivGraphFromMem(file->GetTopPointer(), (signed)file->GetSize(), all_num, x_num, y_num, width, height, buf);
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	return res;
}

int FileLoadUtil::CreateSoundMemFromFLM(const std::string& fn, bool unload_flag){
	using namespace cdec;
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int sh = LoadSoundMemByMemImage(file->GetTopPointer(), (signed)file->GetSize());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	return sh;
}

int FileLoadUtil::CreatePixelShaderFromFLM(const std::string& fn, bool unload_flag){
	using namespace cdec;
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int sh = LoadPixelShaderFromMem(file->GetTopPointer(), (signed)file->GetSize());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	return sh;
}

int FileLoadUtil::LoadCSVFromFLM(CSVLoader* svl, const std::string& fn, bool unload_flag){
	using namespace cdec;
	if (svl == nullptr){ return -1; }
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int res = svl->LoadCSVByBynary(file->GetTopPointer(), file->GetSize());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	if (res){ return -2; }
	return 0;
}

int FileLoadUtil::LoadDataScriptFromFLM(DataScript* ds, const std::string& fn, bool unload_flag){
	using namespace cdec;
	if (ds == nullptr){ return -1; }
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int res = ds->LoadDataFromBynary(file->GetTopPointer(), file->GetSize());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	if (res){ return -2; }
	return 0;
}

int FileLoadUtil::LoadLuaScriptFromFLM(lua_State* L, const std::string& fn, bool unload_flag){
	using namespace cdec;
	if (L == nullptr){ return -1; }
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int res = luaL_loadbuffer(L, (const char*)file->GetTopPointer(), file->GetSize(), fn.c_str());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	if (res == LUA_ERRSYNTAX){ return -LUA_ERRSYNTAX; }
	else if (res == LUA_ERRMEM){ return -LUA_ERRMEM; }
	return 0;
}

int FileLoadUtil::LoadActionDataFromFLM(ActionData* ad, const std::string& fn, bool unload_flag){
	using namespace cdec;
	if (ad == nullptr){ return -1; }
	std::shared_ptr<File> file = FileLoadManager::GetInstance().FindFile(fn);
	if (file == nullptr){ return -10; }
	if (file->GetStatus() != FileStatus::Available){ return -11; }
	int res = ad->ReadActionFileFromMem(file->GetTopPointer(), file->GetSize());
	if (loaded_data == LoadedData::Unload || (loaded_data == LoadedData::Set && unload_flag)){ file->UnloadData(); }
	if (res < 0){ return res; }
	return 0;
}