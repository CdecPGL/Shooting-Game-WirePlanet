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
	void SetTarget(std::weak_ptr<Object>, bool = true); //ターゲット(新しいターゲット[、補間ありか])
	enum class CameraMode{ Normal = 0, Wide = 1, Zoom = 2, FocusPlanet = 3, Free = 4 }; //カメラモード
	void ChangeCameraMode(CameraMode, bool = true); //モード変更(モード[、補間ありか])
	CameraMode GetCameraMode()const;
	void SetVibrationPower(double);
	void SetCameraViewArea(const RectAngle<double>&);
private:
	CameraController();
	std::weak_ptr<Object> _target; //ターゲット
	Camera2D& _camera;
	CameraMode _mode;

	bool _polar_coordinates; //極座標モード化
	bool _interpolation_trigger; //補間トリガー
	int _interp_cntr;
	double _calc_interp_ratio(); //現在の補間割合を計算

	double _vibration_power; //振動の大きさ

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

	CameraViewPointInfo _objective_vp; //目標視点
	CameraViewPointInfo _current_vp; //現在の視点
	CameraViewPointInfo _vp_interpolation; //視点補間値

	RectAngle<double> _view_area; //視界範囲(サイズが(-1,-1)で制限なし)
};
