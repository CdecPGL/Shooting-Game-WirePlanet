#pragma once

#include<memory>
#include"RectAngle.h"

class Object;
class Camera2D;

class CameraController
{
public:
	CameraController(const CameraController&) = delete;
	~CameraController();
	static CameraController& GetInstance();
	int Reset();
	void Update();
	void SetTarget(std::weak_ptr<Object>, bool = true); //�^�[�Q�b�g(�V�����^�[�Q�b�g[�A��Ԃ��肩])
	enum class CameraMode{ Normal = 0, Wide = 1, Zoom = 2, FocusPlanet = 3, Free = 4 }; //�J�������[�h
	void ChangeCameraMode(CameraMode, bool = true); //���[�h�ύX(���[�h[�A��Ԃ��肩])
	CameraMode GetCameraMode()const;
	void SetVibrationPower(double);
	void SetCameraViewArea(const RectAngle<double>&);
private:
	CameraController();
	std::weak_ptr<Object> _target; //�^�[�Q�b�g
	Camera2D& _camera;
	CameraMode _mode;

	bool _polar_coordinates; //�ɍ��W���[�h��
	bool _interpolation_trigger; //��ԃg���K�[
	int _interp_cntr;
	double _calc_interp_ratio(); //���݂̕�Ԋ������v�Z

	double _vibration_power; //�U���̑傫��

	struct CameraViewPointInfo{
		Vector2D<double> pos;
		double distance_in_camera = 0.0;
		double rotation_rad = 0.0;
		double scale = 0.0;
		CameraViewPointInfo operator +(const CameraViewPointInfo&);
		CameraViewPointInfo operator -(const CameraViewPointInfo&);
		CameraViewPointInfo operator *(double);
		void clear();
	};

	CameraViewPointInfo _objective_vp; //�ڕW���_
	CameraViewPointInfo _current_vp; //���݂̎��_
	CameraViewPointInfo _vp_interpolation; //���_��Ԓl

	RectAngle<double> _view_area; //���E�͈�(�T�C�Y��(-1,-1)�Ő����Ȃ�)
};
