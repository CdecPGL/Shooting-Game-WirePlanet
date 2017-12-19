#include "WirePlanet.h"
#include"GraphicController2D.h"
#include<algorithm>

using namespace std;

namespace{
	const double Wave_Deflation(10.0); //波の横収縮度
	const double Wave_Attenuation(0.98); //波の減衰率
	const double Wave_Amplitude(0.01); //波の振幅倍率
	const double Wave_Length_Rate(0.1); //波の長さの倍率(長さはこの定数*力の強さになる)

	const double Wave_Max_Force(5000.0); //力を加えるときの波に影響する最大値
	const double WAVE_IGNORE_FORCE(1.0); //波の計算で無視する力の大きさ
	const double WAVE_VELOCITY(10);

	const int DESTROY_RADIUS(50); //破壊半径(Game.cppと合わせる)
}


WirePlanet::WirePlanet(double m, double f, double r, double s) :Planet(m, f, r, s), _col(-1), _wave_velocity(WAVE_VELOCITY)
{
}


WirePlanet::~WirePlanet()
{
}

int WirePlanet::Init(){
	_col = GetColor(255, 255, 255);
	//波用ドットデータ初期化
	_dis_data.resize(separation);
	if (Planet::Init() < 0){ return -1; }
	return 0;
}

void WirePlanet::Update(){
	//波の計算
	for (DisData& dt : _dis_data){
		dt.gap = 0;
		dt.velocity = 0;
	}
	//波データから現在の各ドットの情報を求める
	for (Wave& w : _wave){
		//時計回り
		double wt = w.s_t_rad + w.cnt * _wave_velocity / radius; //現在位置
		double st;
		if (w.cnt * _wave_velocity >= w.length){
			st = wt - w.length / radius;
		}
		else{
			st = w.s_t_rad;
		}
		int s_pos = static_cast<int>(st / (DX_PI * 2.0 / separation));
		int e_pos = static_cast<int>(wt / (DX_PI * 2.0 / separation));
		if (e_pos < s_pos){ e_pos += separation; }
		for (int i = s_pos; i <= e_pos; ++i){
			int p = i < 0 ? (i % separation + separation) : i%separation;
			_dis_data.at(p).gap += w.power * sin((wt - p*(DX_PI * 2.0 / separation)) * Wave_Deflation) * Wave_Amplitude *pow((1.0 - (e_pos - i)*(DX_PI * 2.0 / separation)*radius / w.length), 2);
			_dis_data.at(p).velocity += w.power*cos((wt - p*(DX_PI * 2.0 / separation))*Wave_Deflation) * Wave_Amplitude*pow((1.0 - (e_pos - i)*(DX_PI * 2.0 / separation)*radius / w.length), 2);
		}
		//半時計周り
		//波の始点と惑星の中心を通る直線を軸に線対称な波を考える
		wt = w.s_t_rad - w.cnt * _wave_velocity / radius; //現在位置
		if (w.cnt * _wave_velocity >= w.length){
			st = wt + w.length / radius;
		}
		else{
			st = w.s_t_rad;
		}
		s_pos = static_cast<int>(st / (DX_PI * 2.0 / separation));
		e_pos = static_cast<int>(wt / (DX_PI * 2.0 / separation));
		if (e_pos > s_pos){ e_pos -= separation; }
		for (int i = s_pos; i >= e_pos; --i){
			int p = i;
			if (p >= separation){ p = p%separation; }
			while (p < 0){ p += separation; }
			if (p != w.s_t_rad / (DX_PI * 2.0 / separation)){//波の始点は時計回りの処理で扱ったのでスキップする
				_dis_data.at(p).gap += w.power * sin((p*(DX_PI * 2.0 / separation) - wt) * Wave_Deflation) * Wave_Amplitude *pow((1.0 - (i - e_pos)*(DX_PI * 2.0 / separation)*radius / w.length), 2);
				_dis_data.at(p).velocity += w.power*cos((p*(DX_PI * 2.0 / separation) - wt)*Wave_Deflation) * Wave_Amplitude*pow((1.0 - (i - e_pos)*(DX_PI * 2.0 / separation)*radius / w.length), 2);
			}
		}
		//波を進める
		//		w.t += wave_velocity / radius;
		w.power *= Wave_Attenuation;
		++w.cnt;
	}
	_wave.erase(remove_if(_wave.begin(), _wave.end(), [](Wave& w)->bool{
		return abs(w.power) * Wave_Amplitude < 1.0;
	}), _wave.end());
}

void WirePlanet::DrawBody()const{
	std::vector<DotData>::const_iterator it, nit; //いてれーた,次のドットのいてらーた
	std::vector<DisData>::const_iterator wit, wnit;
	int i = 0;
	for (it = dot_data.begin(),wit = _dis_data.begin(); it != dot_data.end(); ++it,++wit){
		if (it != dot_data.end() - 1){       //イテレータが最後でなければnitはitの次
			nit = it + 1;
		}
		else{
			nit = dot_data.begin();         //イテレータが最後だったらnitは最初
		}
		//wnitも同じく
		if (wit != _dis_data.end() - 1){       //イテレータが最後でなければnitはitの次
			wnit = wit + 1;
		}
		else{
			wnit = _dis_data.begin();         //イテレータが最後だったらnitは最初
		}
		double sx = (radius + (*it).gap + (*wit).gap) * cos((double)2 * DX_PI / separation * i) + position.x, sy = (radius + (*it).gap + (*wit).gap) * sin((double)2 * DX_PI / separation * i) + position.y;
		double ex = (radius + (*nit).gap + (*wnit).gap) * cos((double)2 * DX_PI / separation * (i + 1)) + position.x, ey = (radius + (*nit).gap + (*wnit).gap) * sin((double)2 * DX_PI / separation * (i + 1)) + position.y;
		DrawLine3D(VGet((float)sx, (float)sy, 0), VGet((float)ex, (float)ey, 0), _col);
		i++;
	}
	//マーカ
	auto draw_maker = [this](double t_deg,int l)->void{
		Vector2D<float> ls, le;
		double t_rad = t_deg / 180.0*DX_PI;
		ls = position + Vector2D<float>((GetDistance(t_deg) - l)*cos(t_rad), (GetDistance(t_deg) - l)*sin(t_rad));
		le = position + Vector2D<float>(GetDistance(t_deg)*cos(t_rad), GetDistance(t_deg)*sin(t_rad));
		DrawLine3D(VGet(ls.x, ls.y, 0), VGet(le.x, le.y, 0), _col);
	};
	//0
	draw_maker(0, 10);
	//90
	draw_maker(89, 10);
	draw_maker(91, 10);
	//180
	draw_maker(178, 10);
	draw_maker(180, 10);
	draw_maker(182, 10);
	//270
	draw_maker(267, 10);
	draw_maker(269, 10);
	draw_maker(271, 10);
	draw_maker(273, 10);
	//破壊半径
	gc2d::DrawCircle(position.x, position.y, DESTROY_RADIUS, GetColor(255, 0, 0), false);
}

void WirePlanet::SetColor(int r, int g, int b){
	_col = GetColor(r, g, b);
}

void WirePlanet::AddForce(double t_deg, double f){
	Planet::AddForce(t_deg, f);
	//波を発生させる
	//無視する力の大きさ
	f = -f;
	if (abs(f) <= WAVE_IGNORE_FORCE){ return; }
	//力制限
	if (abs(f) > Wave_Max_Force){ f = (f >= 0.0 ? 1 : -1)*Wave_Max_Force; }
	if (t_deg < 0){ t_deg = fmod(t_deg, 360) + 360; }
	t_deg = t_deg / 180 * DX_PI;
	//波を生成
	Wave w;
	w.power = f;
	w.length = abs(w.power)*Wave_Length_Rate;
	w.s_t_rad = t_deg;
	_wave.push_back(w);
}

double WirePlanet::GetDistance(double t_deg)const{
	double tb = t_deg;
	if (t_deg < 0){ t_deg += 360.0; t_deg = fmod(t_deg, 360); }
	else{ t_deg = fmod(t_deg, 360); }
	double positiond = t_deg / 360 * GetSeparation();
	long positions = (long)floor(positiond);
	long positione;
	positiond = positiond - positions;
	if (positions == GetSeparation() - 1){ positione = 0; }
	else{ positione = positions + 1; }
	return (1 - positiond) * (radius + dot_data.at(positions).gap + _dis_data.at(positions).gap) + positiond * (radius + dot_data.at(positione).gap + _dis_data.at(positione).gap);
	return 0;
}

WirePlanet::DisData::DisData() :gap(0), velocity(0){}

WirePlanet::Wave::Wave():s_t_rad(0),cnt(0),length(0),power(0){}
