#include "CameraController.h"
#include"GraphicController2D.h"

#include"Planet.h"

using namespace std;

namespace{
	const int PLANET_DISTANCE_IN_CAMERA(300);
	const int INTERPOLATION_TIME(30); //ï‚ä‘éûä‘
	const int RONDOM_MAX_NUM(10000);
}

CameraController::CameraController() :_camera(gc2d::GetCamera2D()), _mode(CameraMode::Normal), _polar_coordinates(true), _interpolation_trigger(false), _interp_cntr(0), _vibration_power(0.0), _view_area(-1,-1,-1,-1)
{
}


CameraController::~CameraController()
{
}

CameraController& CameraController::GetInstance(){
	static CameraController cc;
	return cc;
}

int CameraController::Reset(){
	_target.reset();
	_polar_coordinates = true;
	_interpolation_trigger = false;
	_mode = CameraMode::Normal;
	_vibration_power = 0.0;
	_objective_vp.clear();
	_current_vp.clear();
	_vp_interpolation.clear();
	_view_area.Set(-1, -1, -1, -1);
	return 0;
}

void CameraController::Update(){
	Vector2D<int> draw_area_size = gc2d::GetDrawAreaSize();
	shared_ptr<Object> tar = _target.lock();
	Vector2D<double> camera_pos_gap; //ÉJÉÅÉâà íuÇÃÉYÉå
	if (_vibration_power > 0){
		double gap = (double)GetRand(RONDOM_MAX_NUM) / RONDOM_MAX_NUM * _vibration_power;
		double dir_rad = (double)GetRand(RONDOM_MAX_NUM) / RONDOM_MAX_NUM * DX_PI * 2;
		camera_pos_gap.Set(gap*cos(dir_rad), gap*sin(dir_rad));
	}
	if (_interpolation_trigger){
		if (_interp_cntr < INTERPOLATION_TIME){
			double i_ratio = _calc_interp_ratio();
			_vp_interpolation = _vp_interpolation*i_ratio;
		}
		else{
			_vp_interpolation.distance_in_camera = 0;
			_vp_interpolation.pos.Set(0, 0);
			_vp_interpolation.rotation_rad = 0;
			_vp_interpolation.scale = 0;
		}
		_vp_interpolation = _vp_interpolation + _objective_vp;
	}
	switch (_mode){
	case CameraMode::Normal:
		if (tar){
			std::shared_ptr<Planet> p = tar->GetBelongingPlanet();
			_objective_vp.scale = 1.0;
			if (p){
				_objective_vp.pos = p->GetPositionXY();
				_objective_vp.distance_in_camera = PLANET_DISTANCE_IN_CAMERA;
				_objective_vp.rotation_rad = (tar->GetT() + 90.0) / 180.0*DX_PI;
				_polar_coordinates = true;
			}
			else{
				_objective_vp.pos = tar->GetPositionXY();
				_polar_coordinates = false;
			}
		}
		break;
	case CameraMode::Wide:
		if (tar){
			std::shared_ptr<Planet> p = tar->GetBelongingPlanet();
			if (p){
				Vector2D<double> p_pos = p->GetPositionXY();
				double p_radius = p->GetRadius();
				_objective_vp.distance_in_camera = 0;
				_objective_vp.pos = p_pos;
				_objective_vp.rotation_rad = 0;
				_objective_vp.scale = 0.5;
				_polar_coordinates = true;
			}
			else{
				_polar_coordinates = false;
			}
		}
		break;
	case CameraMode::Zoom:
		if (tar){
			std::shared_ptr<Planet> p = tar->GetBelongingPlanet();
			_objective_vp.scale = 2.0;
			if (p){
				_objective_vp.pos = p->GetPositionXY();
				_objective_vp.distance_in_camera = (tar->GetR() + draw_area_size.y / 8);
				_objective_vp.rotation_rad = (tar->GetT()+90.0) / 180.0*DX_PI;
				_polar_coordinates = true;
			}
			else{
				_objective_vp.pos = tar->GetPositionXY();
				_polar_coordinates = false;
			}
		}
		break;
	case CameraMode::FocusPlanet:
		if (tar){
			std::shared_ptr<Planet> p = tar->GetBelongingPlanet();
			_objective_vp.scale = 1.0;
			if (p){
				_objective_vp.pos = p->GetPositionXY();
				_objective_vp.distance_in_camera = 0;
				_objective_vp.rotation_rad = 0;
				_polar_coordinates = true;
			}
			else{
				_objective_vp.pos = tar->GetPositionXY();
				_polar_coordinates = false;
			}
		}
		break;
	case CameraMode::Free:

		break;
	default:
		break;
	}
	//ï‚ä‘ínÇåàÇﬂÇÈ
	if (_interpolation_trigger){
		_vp_interpolation = _vp_interpolation - _objective_vp;
		_interp_cntr = 0;
		//äpìxÇ…Ç¬Ç¢ÇƒÇÕí≤êÆ(ïsÇæÇ¡ÇΩÇÁê≥Ç…Ç»ÇÈÇ‹Ç≈2piÇâ¡éZÇµÅA2piÇí¥Ç¶ÇƒÇ¢ÇΩÇÁÇ†Ç‹ÇËÇéÊÇËÅApiÇÊÇËëÂÇ´Ç©Ç¡ÇΩÇÁ-2piÇµÇƒâÒì]Çîºé¸à»ì‡Ç…Ç∑ÇÈ)
		while ((_vp_interpolation.rotation_rad += DX_PI * 2)<0){}
		_vp_interpolation.rotation_rad = fmod(_vp_interpolation.rotation_rad,DX_PI*2);
		if (_vp_interpolation.rotation_rad > DX_PI){ _vp_interpolation.rotation_rad = _vp_interpolation.rotation_rad - DX_PI * 2; }

		_interpolation_trigger = false;
	}
	//ï‚äÆèàóù
	if (_interp_cntr<INTERPOLATION_TIME){
		CameraViewPointInfo _vpi;
		double i_ratio = _calc_interp_ratio();
		_vpi = _vp_interpolation * i_ratio;
		_current_vp = _objective_vp + _vpi;
		++_interp_cntr;
	}
	else{
		_current_vp = _objective_vp;
	}
	//éãäEîÕàÕäOÇæÇ¡ÇΩÇÁèCê≥Ç∑ÇÈ
	if (_view_area.size.x >= 0.0 && _view_area.size.y >= 0.0){
		Vector2D<double> view_size(_camera.size * (1.0 / _camera.scale));
		if (_current_vp.pos.x < _view_area.left() + view_size.x / 2){
			_current_vp.pos.x = _view_area.left() + view_size.x / 2;
		}
		else if (_current_vp.pos.x > _view_area.right() - view_size.x / 2){
			_current_vp.pos.x = _view_area.right() - view_size.x / 2;
		}
		if (_current_vp.pos.y < _view_area.top() + view_size.y / 2){
			_current_vp.pos.y = _view_area.top() + view_size.y / 2;
		}
		else if (_current_vp.pos.y > _view_area.bottom() - view_size.y / 2){
			_current_vp.pos.y = _view_area.bottom() - view_size.y / 2;
		}
	}
	//ÉYÉåÇâ¡éZ
	_current_vp.pos += camera_pos_gap;
	if (_polar_coordinates){
		_camera.scale = _current_vp.scale;
		_camera.position = _current_vp.pos + Vector2D<double>(/*p_radius*/_current_vp.distance_in_camera *cos(_current_vp.rotation_rad - DX_PI / 2.0), /*p_radius*/_current_vp.distance_in_camera*sin(_current_vp.rotation_rad - DX_PI / 2.0));
		//			_camera.position.Set(320, 240);
		_camera.rotate = (_current_vp.rotation_rad/* + DX_PI / 2.0*/);
		//			_camera.rotate = DX_PI/2;
	}
	else{
		_camera.scale = _current_vp.scale;
		_camera.position = _current_vp.pos;
		_camera.rotate = (_current_vp.rotation_rad/* + DX_PI / 2.0*/);
	}

}

void CameraController::SetTarget(weak_ptr<Object> tgt,bool intp_f){
	_target = tgt;
	_interpolation_trigger = intp_f;
}

void CameraController::ChangeCameraMode(CameraMode m,bool intp_f){
	_mode = m;
	_interpolation_trigger = intp_f;
}

double CameraController::_calc_interp_ratio(){
	return pow((1 - sin(((double)_interp_cntr / (double)INTERPOLATION_TIME)*DX_PI / 2)),2);
}

CameraController::CameraMode CameraController::GetCameraMode()const { return _mode; }

void CameraController::SetVibrationPower(double vp){ _vibration_power = vp; }

CameraController::CameraViewPointInfo CameraController::CameraViewPointInfo::operator + (const CameraViewPointInfo& in){
	CameraViewPointInfo out;
	out.distance_in_camera = distance_in_camera + in.distance_in_camera;
	out.pos = pos + in.pos;
	out.rotation_rad = rotation_rad + in.rotation_rad;
	out.scale = scale + in.scale;
	return out;
}
CameraController::CameraViewPointInfo CameraController::CameraViewPointInfo::operator - (const CameraViewPointInfo& in){
	CameraViewPointInfo out;
	out.distance_in_camera = distance_in_camera - in.distance_in_camera;
	out.pos = pos - in.pos;
	out.rotation_rad = rotation_rad - in.rotation_rad;
	out.scale = scale - in.scale;
	return out;
}
CameraController::CameraViewPointInfo CameraController::CameraViewPointInfo::operator * (double in){
	CameraViewPointInfo out;
	out.distance_in_camera = distance_in_camera * in;
	out.pos = pos * in;
	out.rotation_rad = rotation_rad * in;
	out.scale = scale * in;
	return out;
}

void CameraController::CameraViewPointInfo::clear(){
	distance_in_camera = 0.0;
	rotation_rad = 0.0;
	scale = 0.0;
	pos.Set(0.0, 0.0);
}

void CameraController::SetCameraViewArea(const RectAngle<double>& ma){ _view_area = ma; }
