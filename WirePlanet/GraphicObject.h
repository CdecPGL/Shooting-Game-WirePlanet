#pragma once
#include "object.h"
class GraphicObject :
	public Object
{
public:
	GraphicObject();
	virtual ~GraphicObject();
	void Update()override;
	void Draw()override;
	int SetGraph(const std::string&);
private:
	int _ghandle;
};
