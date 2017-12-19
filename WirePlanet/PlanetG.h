#pragma once
#include "Planet.h"
#include"boost/logic/tribool.hpp"
//#include"Animation.h"

namespace AnimLib{
	class Animation;
}

class PlanetG :
	public Planet
{
public:
	//�R���X�g���N�^(���摜��,���ʁA���C�A���a�A������)
	PlanetG(const std::string&,double,double,double,double);
	~PlanetG();
	int Init()override;
	void Update()override;
	/*void Reset()override;*/
private:
	//�{�̕`��֌W
	std::string _origin_graph_name; //���摜��
	int ghandle_origine; //planet���摜�n���h��
	int _texture_ghandle; //�e�N�X�`���摜�n���h��
	VERTEX3D* _vtxs; //���_�f�[�^
	unsigned short* _p_indxs; //polygon���\�����钸�_�f�[�^
	Vector2D<int> _g_size;  //�摜�T�C�Y
	Vector2D<int> _t_size; //�e�N�X�`���T�C�Y
	int _v_separation; //�c������
	int _polygon_num; //�|���S����
	int _vertex_num; //���_��
	//�R�A�֘A
	std::unique_ptr<AnimLib::Animation> _core_anim;
	//�R�A�v���e�N�^�֘A
	std::unique_ptr<AnimLib::Animation> _core_prtctr_anim;

	void DrawBody()const override;

	static bool is2Pow(int); //2�̗ۏォ���ׂ�
	static int getMin2Pow(int); //�w�肳�ꂽ�������傫���čŏ���2�̗ݏ���擾����
};

