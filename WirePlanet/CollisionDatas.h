#pragma once
#include"Vector2D.h"
#include<memory>
#include<vector>
#include<unordered_map>

enum class ObjectBelongingTo;
enum CharaAtrribtute;
class Character;
struct CircleObject;

namespace CollisionDatas{
	//�L�����N�^�[�Փ˃I�u�W�F�N�g
	struct CharacterCollisionObject{
		CharacterCollisionObject();
		int hp;
		double t, r;
		Vector2D<double> position;
		std::vector<CircleObject> circle_objects;
		double grotation; //�L�����摜�̉�]�x
		bool reverse; //�L���������]���Ă邩
		ObjectBelongingTo belongto; //�������Ă���L�����̎��
		CharaAtrribtute char_attribute; //�������Ă���L�����̑���
		bool col_with_friend; //�����ɍU���������邩
		//		bool hit_stop; //�q�b�g�X�g�b�v����
	};
	//���[�U�[�Փ˃I�u�W�F�N�g
	struct LaserCollisionObject{
		LaserCollisionObject();
		int power;
		Vector2D<double> position;
		Vector2D<double> direction;
		int length;
		int width;
		ObjectBelongingTo belongto; //�������Ă���L�����̎��
	};
}

struct CircleObject{
	CircleObject();
	Vector2D<double> position;
	double radius;
	int attribute; //����
	int attack_power; //�U����
	int matchless_time; //�^���閳�G����
};

//�L�����Փ˃f�[�^
struct CollisionDataWithCharacter{
	CollisionDataWithCharacter();
	Vector2D<double> push_vector; //�����o���x�N�g��
	Vector2D<double> blow_vector; //������΂��x�N�g��
	CharaAtrribtute char_attribute; //�Փˑ���̃L��������
	//int my_attribute; //�Փ˂��ꂽ�Փ˃I�u�W�F�N�g�̑���
	int give_damage; //�^�����_���[�W
	int receive_damage; //�󂯂�_���[�W
	int col_chara_id; //�Փˑ����ID
	int matchless_time; //���G����
};

//�f���Փ˃f�[�^
struct CollisionDataWithPlanet{
	CollisionDataWithPlanet();
	double t; //�Փ˒n�_�̘f���ɍ��W�p�x
	double r; //�ړ����ׂ����a
	int damage; //�_���[�W
};
