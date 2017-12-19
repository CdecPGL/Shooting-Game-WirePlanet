#include "Planet.h"
#include<algorithm>

using namespace std;

namespace{
	const double ATTACK_OBJECT_HEIGHT_ATTENUATION_PER_FRAME(1); //�U���I�u�W�F�N�g�̌�����
	const double PLANET_HARDNESS(1000.0); //�f���̌^��
	const double PLANET_FORCE_EFCT_DISTANCE(30); //�͂����������̉e���͈�
}

Planet::Planet(double m_m, double m_f, double m_r, double m_s) :Object(m_m, m_f), radius(m_r), separation(m_s), current_minimam_distance(m_r)
{
}


Planet::~Planet()
{
}

int Planet::Init(){
	if (Object::Init() < 0){ return -1; }
	//�h�b�g�f�[�^������
	dot_data.resize(separation);
	return 0;
}

//�ϐ��擾�֐�
double Planet::GetRadius()const{ return radius; };
int Planet::GetSeparation()const{ return separation; };

void Planet::Update(){

}

void Planet::Draw(){
	DrawBody();
}

double Planet::GetDistance(double t_deg)const{
	while (t_deg < 0.0){ t_deg += 360.0; }
	if (t_deg >= 360.0){ t_deg = fmod(t_deg, 360.0); }
	double positiond = t_deg / 360.0 * GetSeparation();
	int positions = (int)floor(positiond);
	int positione;
	positiond = positiond - positions;
	if (positions == GetSeparation() - 1){ positione = 0; }
	else{ positione = positions + 1; }
	return (1 - positiond) * (radius + dot_data.at(positions).gap) + positiond * (radius + dot_data.at(positione).gap);
	return 0;
}

void Planet::AddForce(double t_deg, double f){
	while (t_deg < 0.0){ t_deg += 360.0; }
	if (t_deg >= 360.0){ t_deg = fmod(t_deg, 360.0); }
	f /= PLANET_HARDNESS;

	double t_rad = t_deg / 180.0*DX_PI; //�͂��������̃��W�A��
	double rad_per_sep = DX_PI*2 / separation; //�ꕪ��������̊p�x
	double range_rad = PLANET_FORCE_EFCT_DISTANCE / radius; //�e���͈͂�f����̊p�x�ɕϊ�
	int s_idx = (int)((t_rad - range_rad) / rad_per_sep - 1); //�͈͂�f���h�b�g�C���f�b�N�X�ɕϊ�
	int e_idx = (int)((t_rad + range_rad) / rad_per_sep + 1);
	//�͈͓��̃h�b�g�ɁA�͂�������_����̋����ɉ����ė͂�������
	for (int i = s_idx; i <= e_idx; ++i){
		//������͂̑傫���͒��S�_�̋�����cos���狁�߂�
		double pf = f*(cos((i*rad_per_sep - t_rad) / range_rad * 2)+1)/2;
		//�f���C���f�b�N�X�𐳂����͈͓��ɕϊ�
		int p_idx = i + separation;
		p_idx %= separation;
		dot_data[p_idx].gap -= pf;
		//�ύX�����_�̒��S�������ŏ��ɂȂ��Ă���L�^����
		if (dot_data[p_idx].gap + radius < current_minimam_distance){ current_minimam_distance = dot_data[p_idx].gap + radius; }
	}
	return;
}

void Planet::RepairPlanet(double rate){
	for (auto it = dot_data.begin(); it != dot_data.end(); ++it){
		it->gap += radius*rate;
		if (it->gap > 0){ it->gap = radius; }
	}
	current_minimam_distance += radius*rate;
	if (current_minimam_distance > radius){ current_minimam_distance = radius; }
}

void Planet::Reset(){
	for (auto it = dot_data.begin(); it != dot_data.end(); ++it){
		it->gap = 0;
	}
	current_minimam_distance = radius;
}

void Planet::DrawBody()const{
	std::vector<DotData>::const_iterator it, nit; //���Ă�[��,���̃h�b�g�̂��Ă�[��
	int i = 0;
	for (it = dot_data.begin(); it != dot_data.end(); it++){
		if (it != dot_data.end() - 1){       //�C�e���[�^���Ō�łȂ����nit��it�̎�
			nit = it + 1;
		}
		else{
			nit = dot_data.begin();         //�C�e���[�^���Ōゾ������nit�͍ŏ�
		}
		double sx = (radius + (*it).gap) * cos((double)2 * DX_PI / separation * i) + position.x, sy = (radius + (*it).gap) * sin((double)2 * DX_PI / separation * i) + position.y;
		double ex = (radius + (*nit).gap) * cos((double)2 * DX_PI / separation * (i + 1)) + position.x, ey = (radius + (*nit).gap) * sin((double)2 * DX_PI / separation * (i + 1)) + position.y;
		DrawLine((int)sx, (int)sy, (int)ex, (int)ey, GetColor(0, 150, 0));
		i++;
	}
}

void Planet::RandomTransform(int mag){
	for (int i = 0; i < separation; ++i){
		int num = GetRand(mag * 2) - mag;
		dot_data[i].gap = num;
	}
}

double Planet::GetMinimamDistance()const{ return current_minimam_distance; }

Planet::DotData::DotData() :gap(0.0){}
