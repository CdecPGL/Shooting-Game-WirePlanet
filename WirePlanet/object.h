#ifndef DEF_OBJECT_H //��dinclude�h�~

#define DEF_OBJECT_H

#include "define.h"
#include <cmath>
#include "DxLib.h"
#include"CollisionDatas.h"
#include<vector>
#include"curve.h"
#include"interpolating.h"
#include<mutex>

class CharacterBase;
class Planet;

/*
�f���ɏ������ĂȂ��ꍇ��TR���W�̓��[���h���W�Ə㉺���΃i���W�ɂȂ�܂�
*/

//����
enum class ObjectBelongingTo{ Neutral, Friend, Enemy, None }; //�����A�����A�G�A�Ȃ�

//Object�N���X
class Object{
public:
	//����̃R���X�g���N�^
	Object();
	//�R���X�g���N�^(����,���C)
	Object(double, double);
	// �R���X�g���N�^(����, ���C,����)
	Object(double, double, ObjectBelongingTo);
	//�f�X�g���N�^
	virtual ~Object();
	//������
	virtual int Init();
	//�I������
	virtual int End();
	//�`��
	virtual void Draw();
	//�s��
	virtual void Update() = 0;
	//���b�Z�[�W�󂯎��(������true��Ԃ�)
	virtual bool ReceiveMessage(const std::string&, double = 0);
	//�Փ˂̏���
	virtual void ProcessCollision();
	//�}�l�[�W���[�ɓo�^
	//virtual void ResistToManager();
	////�}�l�[�W������폜
	//virtual void UnresistFromManager();

	//�ϐ��擾�֐�
	Vector2D<double> GetPositionXY()const;
	double GetRotation()const;
	double GetT()const{ return t; };
	double GetR()const{ return r; };
	double GetVH()const;
	double GetVR()const;
	double GetM()const{ return m; };
	double GetFriction()const{ return friction; };
	double GetRotationVelocity()const;
	std::shared_ptr<Object> GetParent()const; //�e�̃|�C���^��Ԃ�
	std::shared_ptr<Planet> GetBelongingPlanet()const; //�����f���̃|�C���^
	ObjectBelongingTo GetBelongingTo()const; //�����擾(�����A�����A�G)
	std::shared_ptr<CharacterBase> GetTarget()const; //�^�[�Q�b�g�擾
	//�d��(dependence=true�ŋ@�\)
	void Gravity(double = 1.0);

	void SetBelongingPlanet(std::shared_ptr<Planet>); //SetXY,TR����ɏ����f�����Z�b�g����
	void SetXY(const Vector2D<double>&);
	void SetTR(double, double);
	void SetVhVr(double, double);
	void SetRotation(double);
	void SetPolarVelocity(double, double); //�ɍ��W���x�Z�b�g(�����A����(�x))
	void SetRatationVelocity(double); //��]���x���Z�b�g(�x)
	void SetObjectBelongingTo(ObjectBelongingTo);
	void SetT(double);
	void SetR(double);
	void SetVH(double);
	void SetVR(double);

	//�Փ˃f�[�^�ǉ�
	void AddCollisionData(const CollisionDataWithPlanet&);
	void AddCollisionData(const CollisionDataWithCharacter&);

	//�`��̉�]�x���擾
	double GetDrawRotation()const;

	//�^�[�Q�b�g�̘f���ɍ��W�擾
	double GetTargetT()const;
	double GetTargetR()const;
	double GetTargetDistance()const; //�^�[�Q�b�g�Ƃ̒��������擾
	double GetTargetTFromMe()const; //��������̃^�[�Q�b�g�̑���T���擾
	double GetTargetRFromMe()const; //��������̃^�[�Q�b�g�̑���T���擾
	double GetTargetDirectionByDeg()const;
	//�^�[�Q�b�g�Z�b�g
	void SetTarget(std::shared_ptr<CharacterBase>);

	//�����ړ�
	bool IsAutoMoving()const; //�����ړ������m�F
	void StartLinearMove(const Vector2D<double>&, int); //�����ړ��J�n(�ړI�n�A�ړ��t���[��)
	void StartHermiteMove(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, int); //�G���~�[�g�ړ��J�n(�ړI�n�A�o�����x�A�������x�A�ړ��t���[��)
	void StartBezierMove(const Vector2D<double>&, const std::vector<Vector2D<double>>&, int); //�x�W�F�ړ��J�n(�ړI�n�A����_���X�g�A�ړ��t���[��)
protected:
	//�f���ɍ��W���烏�[���h���W�֕ϊ�
	void ConvertPTW();
	//���[���h���W����f���ɍ��W�֕ϊ�
	void ConvertWTP();
	//���x�����W�ɓK�p����
	void ApplyVelocity();
	Vector2D<double> position; //���W
	Vector2D<double> velocity; //���x
	double rotation; //�p�x(��)(�����f�����Z�b�g����Ă���ꍇ�͉�ʏ�̂ł͂Ȃ��A�f����ł̉�]�Ȃ̂Œ���)
	double rota_vel; //��]���x(�x)
	double m; //�d��
	double friction; //���C(���ׂ�ɂ���)

	bool visible; //�����邩

	ObjectBelongingTo _obj_belonging_to; //����

	//�^�[�Q�b�g
	std::shared_ptr<CharacterBase> target;

	//�e
	std::shared_ptr<Object> parent;

	//�����f��
	std::shared_ptr<Planet> myplanet;
	//�f����ɍ��W
	double t, vh; //�p�x(��),�������x(px)
	double r, vr; //����,�������x

	//�Փ˔��芰�i
	//�Փ˃f�[�^���X�g
	std::vector<CollisionDataWithPlanet> col_data_p; //�Փ˘f���f�[�^
	std::vector<CollisionDataWithCharacter> col_data_c; //�Փ˃L�����f�[�^

	//�����ړ�����������*�f���ɍ��W
	void ProcessAutoMove();

	//	std::recursive_mutex _planet_pointer_mtx; //�f���|�C���^�p�~���[�e�b�N�X
	//	std::recursive_mutex _target_pointer_mtx; //�^�[�Q�b�g�|�C���^�p�~���[�e�b�N�X
	//	std::recursive_mutex _t_mtx, _r_mtx, _vh_mtx, _vr_mtx;
private:
	class AutoMove{
	public:
		virtual ~AutoMove() = default;
		virtual bool move(Vector2D<double>*) = 0; //�ړ�����(�����ɂƂ����|�C���^�Ɋi�[�B�ړ��I����false��Ԃ�)
	};
	class LinearMove : public AutoMove{
	public:
		LinearMove(const Vector2D<double>&, const Vector2D<double>&, int);
		bool move(Vector2D<double>*)override;
	private:
		otd::interpoliting<Vector2D<double>> _interp;
		otd::interpoliting<Vector2D<double>>::iterator it;
	};
	class HermiteMove : public AutoMove{
	public:
		HermiteMove(const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, const Vector2D<double>&, int);
		bool move(Vector2D<double>*)override;
	private:
		otd::HermiteCurve<Vector2D<double>> _hCurve;
		otd::HermiteCurve<Vector2D<double>>::iterator it;
	};
	class BezierMove : public AutoMove{
	public:
		BezierMove(const Vector2D<double>&, const Vector2D<double>&, const std::vector<Vector2D<double>>&, int);
		bool move(Vector2D<double>*)override;
	private:
		otd::BezierCurve<Vector2D<double>> _bCurve;
		otd::BezierCurve<Vector2D<double>>::iterator it;
	};
	std::unique_ptr<AutoMove> _auto_move;
};

#endif