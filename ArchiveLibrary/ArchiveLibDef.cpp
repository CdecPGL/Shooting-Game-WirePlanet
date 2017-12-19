#include"ArchiveLibDef.h"

unsigned int ArcLib::ConvertModeToUnsignedInt(Mode m){
	switch (m){
	case Mode::SimpleBind:
		return 0;
		break;
	case Mode::Compress:
		return 1;
		break;
	case Mode::EXOREncrypt:
		return 2;
		break;
	default:
		return 0;
		break;
	}
}
std::string ArcLib::ConvertModeToString(Mode m){
	switch (m){
	case Mode::SimpleBind:
		return "SimpleBind";
		break;
	case Mode::Compress:
		return "Compress";
		break;
	case Mode::EXOREncrypt:
		return "EXOREncrypt";
		break;
	default:
		return "ERROR";
		break;
	}
}
ArcLib::Mode ArcLib::ConvertUnsignedIntToMode(unsigned int in){
	switch (in){
	case 0:
		return Mode::SimpleBind;
		break;
	case 1:
		return Mode::Compress;
		break;
	case 2:
		return Mode::EXOREncrypt;
		break;
	default:
		return Mode::SimpleBind;
		break;
	}
}