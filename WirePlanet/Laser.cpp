#include "Laser.h"
#include"LaserManager.h"
#include"Planet.h"
#include"SoundManager.h"

const int TREMOR(2); //レーザーのゆらぎ

Laser::Laser() :_laser_ghandle(-1), _laser_col_ghandle(-1), _lg_width(0), _lg_height(0), _color(-1), _r(255), _g(255), _b(255), _suspension(10), _duration(60), _inc_cnt(0), _current_width(0), _width(32), _length(900), _inversion(false), _col_pos_with_planet(-1,-1)
{
}


Laser::~Laser()
{
}

int Laser::Init(){
	_inc_cnt = 0;
	return 0;
}

int Laser::End(){
	if (_inc_cnt < _suspension + _duration){
		_inc_cnt = _suspension + _duration;
	}
	return 0;
}

void Laser::Update(){
	if (_inc_cnt == 0){
		SoundManager::GetInstance().PlaySoundByID("Laser0");
	}
	if (_inc_cnt < _suspension){
		_current_width = _width * ((double)_inc_cnt / _suspension);
	}
	else if (_inc_cnt < _suspension + _duration){
		_current_width = _width + (_inc_cnt%3-1)*TREMOR;
	}
	else if(_inc_cnt < _suspension*2+_duration){
		_current_width = _width * (1.0 - ((double)(_inc_cnt - _suspension - _duration) / _suspension));
	}else{
		LaserManager::GetInstance().DeleteLaser(this);
	}
	//所属惑星があれば惑星とレーザーの接触点を調べる
	if (myplanet){
		_length = (int)(r - myplanet->GetDistance(t));
	}
	ApplyVelocity();
	ConvertPTW();
	calculate_length();
	++_inc_cnt;
}

void Laser::Draw(){
	SetDrawBright(_r, _g, _b);
	//	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	gc2d::DrawRotaGraph3(position.x, position.y, _lg_width / 2, 0, (double)_current_width / _lg_width, (double)_length / _lg_height, (GetDrawRotation() - 90) / 180.0*DX_PI, _laser_ghandle, true, 0);
	gc2d::DrawRotaGraph(position.x, position.y, (double)_current_width / _lcg_size.x, 0, _laser_col_ghandle, true, false);
	if (_col_pos_with_planet.x != 0xFFFFFFFF){
		gc2d::DrawRotaGraph(_col_pos_with_planet.x, _col_pos_with_planet.y, (double)_current_width / _lcg_size.x, 0, _laser_col_ghandle, true, false);
	}
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawBright(255, 255, 255);
	//	SetDrawBright(255, 255, 255);
	SetDrawBlendMode(_inversion ? DX_BLENDMODE_SUB : DX_BLENDMODE_ADD, 255);
	gc2d::DrawRotaGraph3(position.x, position.y, _lg_width / 2, 0, (double)_current_width / _lg_width * 0.7, (double)_length / _lg_height, (GetDrawRotation() - 90) / 180.0*DX_PI, _laser_ghandle, true, 0);
	gc2d::DrawRotaGraph(position.x, position.y, (double)_current_width / _lcg_size.x, 0, _laser_col_ghandle, true, false);
	if (_col_pos_with_planet.x != 0xFFFFFFFF){
		gc2d::DrawRotaGraph(_col_pos_with_planet.x, _col_pos_with_planet.y, (double)_current_width / _lcg_size.x, 0, _laser_col_ghandle, true, false);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//	SetDrawBright(255, 255, 255);
}

void Laser::SetDirection(double dir_deg){
	rotation = dir_deg;
}

void Laser::SetWidth(int w){ _width = w; }
void Laser::SetSuspension(int s){ _suspension = s; }
void Laser::SetDuration(int d){ _duration = d; }
void Laser::SetGraphicHandle(int lg,int lcg){
	_laser_ghandle = lg;
	_laser_col_ghandle = lcg;
	GetGraphSize(lg, &_lg_width, &_lg_height);
	GetGraphSize(lcg, &_lcg_size.x, &_lcg_size.y);
}
void Laser::SetColor(int r, int g, int b){
	_r = r;
	_g = g;
	_b = b;
	_color = GetColor(r,g,b); 
}

void Laser::SetInversion(bool in){ _inversion = in; }

int Laser::GetLength()const{ return _length; }
int Laser::GetWidth()const{ return _width; }

void Laser::calculate_length(){
	const int MAX_LENGTH(1000);
	const double RESOLUTION(0.02); //判定の分解能
	_length = MAX_LENGTH;
	_col_pos_with_planet.Set(0xFFFFFFFF, 0);
	if (myplanet){
		double po_l_dis = r * sin((rotation - 90) / 180.0*DX_PI); //レーザと惑星中心の距離
		auto calc_laser_ps_dis = [po_l_dis, this](double t_rad)->double{return po_l_dis / sin(t_rad - (t + rotation - 270) / 180.0*DX_PI); }; //特定位置でのレーザーと惑星中心の距離を求める関数
		double s_rad((t) / 180.0*DX_PI), e_rad((t + rotation - 270) / 180.0*DX_PI); //惑星との位置関係を調べる範囲の開始位置、終了位置
		bool clockwise = rotation-90 >= 0 ? false : true; //時計回りか
		if (clockwise){
			if (e_rad < s_rad){ e_rad += DX_PI * 2; }
			for (double i = s_rad; i < e_rad; i += RESOLUTION){
				double c_dis = calc_laser_ps_dis(i);
				if (c_dis < myplanet->GetDistance(i / DX_PI*180.0)){
					_col_pos_with_planet = Vector2D<double>(myplanet->GetDistance(i / DX_PI*180.0)*cos(i), myplanet->GetDistance(i / DX_PI*180.0)*sin(i)) + myplanet->GetPositionXY();
					_length = (position - _col_pos_with_planet).Length();
					break;
				}
			}
		}
		else{
			if (e_rad > s_rad){ e_rad -= DX_PI * 2; }
			for (double i = s_rad; i > e_rad; i -= RESOLUTION){
				double c_dis = calc_laser_ps_dis(i);
				if (c_dis < myplanet->GetDistance(i / DX_PI*180.0)){
					_col_pos_with_planet = Vector2D<double>(myplanet->GetDistance(i / DX_PI*180.0)*cos(i), myplanet->GetDistance(i / DX_PI*180.0)*sin(i)) + myplanet->GetPositionXY();
					_length = (position - _col_pos_with_planet).Length();
					break;
				}
			}
		}
	}
}

