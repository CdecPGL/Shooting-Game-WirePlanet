#pragma once

#include"object.h"

namespace CreateObjFuncs{
	//画像オブジェクトの作成(画像名)
	std::shared_ptr<Object> CreateGraphicObject(const std::string&);
}
