#pragma once
#include "Scene.h"
class BackGround;
class ViewPlayInformations :
	public Scene
{
public:
	ViewPlayInformations();
	~ViewPlayInformations();
	int Init(shared_ptr<DataContainer>)override;
	int End()override;
	void Update()override;
	void Draw()override;
private:
	unsigned long long _play_frame;
	unsigned long long _play_second;
	unsigned long long _pre_c_play_second;
	double _ave_fps;
	Counter& _cntr;
	static void _calc_hms_from_s(unsigned long long, unsigned int*, unsigned int*, unsigned int*); //•b‚©‚ç•ª•b‚ğ‹‚ß‚é
	std::unique_ptr<BackGround> bg;
};

