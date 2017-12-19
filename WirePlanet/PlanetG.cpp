#include "PlanetG.h"

#include<algorithm>
#include"GraphicController2D.h"
#include"GraphicManager.h"
#include"Animation.h"

/*����
���ݕ�2048�̃e�N�X�`���������Ă���̂�PC�ɂ���Ă̓e�N�X�`���T�C�Y�̐����Ɉ���������\������B
���S�̃e�N�X�`�������ʂɂȂ��Ă�
*/

using namespace std;

namespace{
	const int V_SEPARATION(5); //�|���S���`�掞�̏c������
	const string CORE_ANIM("PlanetCore"); //�R�A�̃A�j���t�@�C����
	const string CORE_PROTECTER_ANIM("PlanetCoreProtecter"); //�R�A�v���e�N�^�[�̃A�j���t�@�C����
}

PlanetG::PlanetG(const std::string& ogn, double m_m, double m_f, double m_r, double m_s) : Planet(m_m, m_f, m_r, m_s), _v_separation(V_SEPARATION), _core_anim(make_unique<AnimLib::Animation>()), _core_prtctr_anim(make_unique<AnimLib::Animation>()), _origin_graph_name(ogn)
{
}


PlanetG::~PlanetG()
{
	delete[] _vtxs;
	delete[] _p_indxs;
	if (_texture_ghandle != ghandle_origine){
		DeleteGraph(_texture_ghandle);
	}
}

int PlanetG::Init(){
	if (Planet::Init() < 0){ return -1; }
	if (_v_separation < 1){ return -2; }
	//�摜�̗p��
	ghandle_origine = GraphicManager::GetInstance().GetGraphHandle(_origin_graph_name);
	GetGraphSize(ghandle_origine, &_g_size.x, &_g_size.y);
	//�T�C�Y��2�̗ݏ�łȂ�������摜�쐬
	if (is2Pow(_g_size.x) == false || is2Pow(_g_size.y) == false){
		int buf_gh = MakeScreen(_g_size.x, _g_size.y, true);
		int cur_draw_scr = GetDrawScreen();
		_t_size.Set(getMin2Pow(_g_size.x), getMin2Pow(_g_size.y));
		_texture_ghandle = MakeGraph(_t_size.x, _t_size.y);
		SetDrawScreen(buf_gh);
		DrawGraph(0, 0, ghandle_origine, true);
		GetDrawScreenGraph(0, 0, _g_size.x, _g_size.y, _texture_ghandle);
		SetDrawScreen(cur_draw_scr);
		DeleteGraph(buf_gh);
	}
	//�T�C�Y��2�̗ݏ悾�����炻�̂܂܎g��
	else{
		_texture_ghandle = ghandle_origine;
		_t_size = _g_size;
	}

	//�|���S���̏���
	_polygon_num = separation * 2 * _v_separation;
	_vertex_num = (_v_separation + 1)*(separation + 1);
	_vtxs = new VERTEX3D[_vertex_num];

	//�e�N�X�`���̗��p�̈�
	Vector2D<double> _tex_use_area((double)_g_size.x / _t_size.x, (double)_g_size.y / _t_size.y);
	//�摜���W�̃Z�b�g�ƒ��_���̃��Z�b�g
	for (int i = 0; i < separation + 1; ++i){ //���������͎��v���ɃZ�b�g���Ă���
		for (int j = 0; j < _v_separation + 1; ++j){ //���������͏ォ�珇�ɃZ�b�g���Ă���
			//�e�N�X�`�����W�̃Z�b�g
			_vtxs[(_v_separation + 1)*i + j].u = (float)i / separation * _tex_use_area.x;
			_vtxs[(_v_separation + 1)*i + j].v = (float)j / _v_separation * _tex_use_area.y;
			//���̑��l�̃��Z�b�g
			_vtxs[(_v_separation + 1)*i + j].norm = VGet(0.0f, 0.0f, -1.0f);
			_vtxs[(_v_separation + 1)*i + j].dif = GetColorU8(255, 255, 255, 255);
			_vtxs[(_v_separation + 1)*i + j].spc = GetColorU8(0, 0, 0, 0);
			_vtxs[(_v_separation + 1)*i + j].su = 0;
			_vtxs[(_v_separation + 1)*i + j].sv = 0;

		}
	}

	//���_�C���f�b�N�X�쐬
	_p_indxs = new unsigned short[_polygon_num * 3];
	for (int i = 0; i < separation; ++i){
		for (int j = 0; j < _v_separation; ++j){
			_p_indxs[i*(_v_separation * 6) + j * 6] = i*(_v_separation + 1) + j;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 1] = i*(_v_separation + 1) + j + 1;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 2] = (i + 1)*(_v_separation + 1) + j;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 3] = i*(_v_separation + 1) + j + 1;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 4] = (i + 1)*(_v_separation + 1) + j;
			_p_indxs[i*(_v_separation * 6) + j * 6 + 5] = (i + 1)*(_v_separation + 1) + j + 1;
		}
	}

	//�R�A
	if (_core_anim->SetAnimationData(CORE_ANIM) < 0){
		printf("PlanetG::PlanetG �A�j���[�V�����t�@�C��(�R�A)�̎擾�Ɏ��s���܂����B\n");
		return -3;
	}
	//�R�A�v���e�N�^
	if (_core_prtctr_anim->SetAnimationData(CORE_PROTECTER_ANIM) < 0){
		printf("PlanetG::PlanetG �A�j���[�V�����t�@�C��(�R�A�v���e�N�^)�̎擾�Ɏ��s���܂����B\n");
		return -4;
	}
	return 0;
}

void PlanetG::Update(){
	_core_prtctr_anim->Update();
	Planet::Update();
}

//�{�̕`��
void PlanetG::DrawBody()const{
	std::vector<DotData>::const_iterator it, nit; //���Ă�[��,���̃h�b�g�̂��Ă�[��
	float x0, y0, x1, y1; //����,�E��,���E���̍��W
	vector<float> _v_dises; //�e���_�̋�����
	//�g���X�̋��������߂�
	for (int i = 0; i < separation; ++i){
		_v_dises.push_back(radius + dot_data[i].gap);
	}
	_v_dises.push_back(_v_dises[0]);
	float cx, cy; //���S���W;
	cx = static_cast<float>(position.x); cy = static_cast<float>(position.y);

//	for (it = dot_data.begin(); it != dot_data.end(); it++){
//		if (it != dot_data.end() - 1){       //�C�e���[�^���Ō�łȂ����nit��it�̎�
//			nit = it + 1;
//		}
//		else{
//			nit = dot_data.begin();         //�C�e���[�^���Ōゾ������nit�͍ŏ�
//		}
//		double d_rate = 1.0;
//		double dis = radius + (*it).gap, ndis = radius + (*nit).gap;
//		x0 = static_cast<float>(d_rate * dis * cos((double)2 * DX_PI / separation * i) + position.x), y0 = static_cast<float>(d_rate * dis * sin((double)2 * DX_PI / separation * i) + position.y);
//		x1 = static_cast<float>(d_rate * ndis * cos((double)2 * DX_PI / separation * (i + 1)) + position.x), y1 = static_cast<float>(d_rate * ndis * sin((double)2 * DX_PI / separation * (i + 1)) + position.y);
////		SetDrawMode(DX_DRAWMODE_BILINEAR);
////		gc2d::DrawModiGraphF(x0, y0, x1, y1, cx, cy, cx, cy, ghandle[i], false);
////		DrawModiGraphF(x0, y0, x1 + 1, y1, cx + 1, cy + 1, cx, cy + 1, ghandle[i], true);
////		gc2d::DrawModiGraphF(x0, y0, x1, y1, cx, cy, cx, cy, ghandle[i], true);
////		SetDrawMode(DX_DRAWMODE_NEAREST);
//		for (int j = 0; j < _v_separation; ++j){
//			_vtxs[i*(_v_separation + 1)].pos = VGet(x0, y0, 0);
//		}
//		_vtxs[i*(_v_separation + 1) + _v_separation].pos = VGet(cx, cy, 0);
//		++i;
//	}
	//�e���_�̍��W�Z�b�g
	for (int i = 0; i < separation + 1; ++i){
		float angle_rad = 2.0 * DX_PI / separation * i; //���݂̘f�����_�̈ʒu�p�x
		for (int j = 0; j < _v_separation; ++j){
			float dis_ratio = 1.0f - (float)j / _v_separation;
			_vtxs[i*(_v_separation + 1) + j].pos = VGet(cx + _v_dises[i] * dis_ratio * cos(angle_rad), cy + _v_dises[i] * dis_ratio * sin(angle_rad), 0);
		}
		//���S���W
		_vtxs[i*(_v_separation + 1) + _v_separation].pos = VGet(cx, cy, 0);
	}
	//�\��
	DrawPolygonIndexed3D(_vtxs, _vertex_num, _p_indxs, _polygon_num, ghandle_origine, true);

	//�R�A�ƃv���e�N�^�̕\��
	gc2d::DrawRotaGraphF(position.x, position.y, 1.0, 0.0, _core_anim->GetCurrentGraph(), true);
	gc2d::DrawRotaGraphF(position.x, position.y, 1.0, 0.0, _core_prtctr_anim->GetCurrentGraph(), true);

	return;



	//���C���[�t���[����\��
	for (int i = 0; i < _polygon_num; ++i){
		unsigned short* p_idx = _p_indxs + i * 3;
		float pos[3][2] = { { _vtxs[p_idx[0]].pos.x, _vtxs[p_idx[0]].pos.y }, { _vtxs[p_idx[1]].pos.x, _vtxs[p_idx[1]].pos.y }, { _vtxs[p_idx[2]].pos.x, _vtxs[p_idx[2]].pos.y } };
		DrawLine3D(VGet(pos[0][0], pos[0][1], 0), VGet(pos[1][0], pos[1][1], 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(pos[1][0], pos[1][1], 0), VGet(pos[2][0], pos[2][1], 0), GetColor(255, 0, 0));
		DrawLine3D(VGet(pos[2][0], pos[2][1], 0), VGet(pos[0][0], pos[0][1], 0), GetColor(255, 0, 0));
	}
}

bool PlanetG::is2Pow(int num){
	int bit = sizeof(num) * 8;
	bool flag=false;
	for (int i = 0; i < bit; ++i,num>>=1){
		if (num & 1){
			if (flag){
				return false;
			}
			else{
				flag = true;
			}
		}
	}
	if (flag){
		return true;
	}
	else{
		return false;
	}
}

int PlanetG::getMin2Pow(int num){
	int out = 1;
	int bit = sizeof(num) * 8;
	for (int i = 0; i < bit && out<num; ++i, out <<= 1){
	}
	return out;
}
