#include "CdecFile.h"

using namespace cdec;

File::File()
{
}

File::File(const std::string& ext) :extension(ext){}

File::~File()
{
	if (data_top){
		delete[] data_top;
	}
}

std::string File::GetExtension()const{ return extension; }
unsigned int File::GetSize()const{ return size; }
const void* File::GetTopPointer()const{ return data_top; }
FileStatus File::GetStatus()const{ return status; }

void File::UnloadData(){
	if (data_top){
		size = 0;
		delete[] data_top;
		data_top = nullptr;
		status = FileStatus::Unloaded;
	}
}

void File::ErrorOccured(){
	status = FileStatus::Error;
}

void File::SetData(void* dp, unsigned int s, bool ef){
	if (data_top){ delete[] data_top; }
	data_top = dp;
	size = s;
	status = ef ? FileStatus::Encrypted : FileStatus::Available;
}