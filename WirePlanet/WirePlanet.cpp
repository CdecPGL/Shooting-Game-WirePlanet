#include "WirePlanet.h"
#include"GraphicController2D.h"
#include<algorithm>

using namespace std;

namespace{
	const double Wave_Deflation(10.0); //�g�̉����k�x
	const double Wave_Attenuation(0.98); //�g�̌�����
	const double Wave_Amplitude(0.01); //�g�̐U���{��
	const double Wave_Length_Rate(0.1); //�g�̒����̔{��(�����͂��̒萔*�͂̋����ɂȂ�)

	const double Wave_Max_Force(5000.0); //�͂�������Ƃ��̔g�ɉe������ő�l
	const double WAVE_IGNORE_FORCE(1.0); //�g�̌v�Z�Ŗ�������͂̑傫��
	const double WAVE_VELOCITY(10);

	const int DESTROY_RADIUS(50); //�j�󔼌a(Game.cpp�ƍ��킹��)
}


WirePlanet::WirePlanet(double m, double f, double r, double s) :Planet(m, f, r, s), _col(-1), _wave_velocity(WAVE_VELOCITY)
{
}


WirePlanet::~WirePlanet()
{
}

int WirePlanet::Init(){
	_col = GetColor(255, 255, 255);
	//�g�p�h�b�g�f�[�^������
	_dis_data.resize(separation);
	if (Planet::Init() < 0){ return -1; }
	return 0;
}

void WirePlanet::Update(){
	//�g�̌v�Z
	for (DisData& dt : _dis_data){
		dt.gap = 0;
		dt.velocity = 0;
	}
	//�g�f�[�^���猻�݂̊e�h�b�g�̏������߂�
	for (Wave& w : _wave){
		//���v���
		double wt = w.s_t_rad + w.cnt * _wave_velocity / radius; //���݈ʒu
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
		//�����v����
		//�g�̎n�_�Ƙf���̒��S��ʂ钼�������ɐ��Ώ̂Ȕg���l����
		wt = w.s_t_rad - w.cnt * _wave_velocity / radius; //���݈ʒu
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
			if (p != w.s_t_rad / (DX_PI * 2.0 / separation)){//�g�̎n�_�͎��v���̏����ň������̂ŃX�L�b�v����
				_dis_data.at(p).gap += w.power * sin((p*(DX_PI * 2.0 / separation) - wt) * Wave_Deflation) * Wave_Amplitude *pow((1.0 - (i - e_pos)*(DX_PI * 2.0 / separation)*radius / w.length), 2);
				_dis_data.at(p).velocity += w.power*cos((p*(DX_PI * 2.0 / separation) - wt)*Wave_Deflation) * Wave_Amplitude*pow((1.0 - (i - e_pos)*(DX_PI * 2.0 / separation)*radius / w.length), 2);
			}
		}
		//�g��i�߂�
		//		w.t += wave_velocity / radius;
		w.power *= Wave_Attenuation;
		++w.cnt;
	}
	_wave.erase(remove_if(_wave.begin(), _wave.end(), [](Wave& w)->bool{
		return abs(w.power) * Wave_Amplitude < 1.0;
	}), _wave.end());
}

void WirePlanet::DrawBody()const{
	std::vector<DotData>::const_iterator it, nit; //���Ă�[��,���̃h�b�g�̂��Ă�[��
	std::vector<DisData>::const_iterator wit, wnit;
	int i = 0;
	for (it = dot_data.begin(),wit = _dis_data.begin(); it != dot_data.end(); ++it,++wit){
		if (it != dot_data.end() - 1){       //�C�e���[�^���Ō�łȂ����nit��it�̎�
			nit = it + 1;
		}
		else{
			nit = dot_data.begin();         //�C�e���[�^���Ōゾ������nit�͍ŏ�
		}
		//wnit��������
		if (wit != _dis_data.end() - 1){       //�C�e���[�^���Ō�łȂ����nit��it�̎�
			wnit = wit + 1;
		}
		else{
			wnit = _dis_data.begin();         //�C�e���[�^���Ōゾ������nit�͍ŏ�
		}
		double sx = (radius + (*it).gap + (*wit).gap) * cos((double)2 * DX_PI / separation * i) + position.x, sy = (radius + (*it).gap + (*wit).gap) * sin((double)2 * DX_PI / separation * i) + position.y;
		double ex = (radius + (*nit).gap + (*wnit).gap) * cos((double)2 * DX_PI / separation * (i + 1)) + position.x, ey = (radius + (*nit).gap + (*wnit).gap) * sin((double)2 * DX_PI / separation * (i + 1)) + position.y;
		DrawLine3D(VGet((float)sx, (float)sy, 0), VGet((float)ex, (float)ey, 0), _col);
		i++;
	}
	//�}�[�J
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
	//�j�󔼌a
	gc2d::DrawCircle(position.x, position.y, DESTROY_RADIUS, GetColor(255, 0, 0), false);
}

void WirePlanet::SetColor(int r, int g, int b){
	_col = GetColor(r, g, b);
}

void WirePlanet::AddForce(double t_deg, double f){
	Planet::AddForce(t_deg, f);
	//�g�𔭐�������
	//��������͂̑傫��
	f = -f;
	if (abs(f) <= WAVE_IGNORE_FORCE){ return; }
	//�͐���
	if (abs(f) > Wave_Max_Force){ f = (f >= 0.0 ? 1 : -1)*Wave_Max_Force; }
	if (t_deg < 0){ t_deg = fmod(t_deg, 360) + 360; }
	t_deg = t_deg / 180 * DX_PI;
	//�g�𐶐�
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
