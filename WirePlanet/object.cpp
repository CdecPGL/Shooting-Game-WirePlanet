#include "object.h"
#include"Planet.h"
#include"character.h"

using namespace std;

//����̃R���X�g���N�^
Object::Object() :Object(1,0){
}

//�R���X�g���N�^
Object::Object(double m_m, double m_friction) : Object(m_m, m_friction, ObjectBelongingTo::Neutral){
}

Object::Object(double m_m, double m_friction,ObjectBelongingTo obt)
	: parent(nullptr), t(0), r(0), m(m_m), friction(m_friction), position(0, 0), vr(0), rotation(0),rota_vel(0), vh(0), visible(true),_obj_belonging_to(obt){
}

//�f�X�g���N�^
Object::~Object(){}

int Object::Init(){
	return 0;
}

int Object::End(){
	return 0;
}

//���b�Z�[�W�󂯎��
bool Object::ReceiveMessage(const string&,double){
	return false;
}

//�d��(dependence=true�ŋ@�\)
void Object::Gravity(double rate){
	if (myplanet){
		double F = GRAVITIONAL_CONSTANT * m * myplanet->GetM() / pow(r,0) * rate;
		vr -= F/m;
	}
	else{
		vr -= GRAVITIONAL_CONSTANT * m * 1000 * rate / m;
	}
}

//�f���ɍ��W���烏�[���h���W�֕ϊ�
void Object::ConvertPTW(){	
	if (myplanet){
		position = myplanet->GetPositionXY() + Vector2D<double>(r * cos(t / 180 * DX_PI), r * sin(t / 180 * DX_PI));
	}
	else{
		position.Set(t, -r);
	}
}

//���[���h���W����f���ɍ��W�֕ϊ�
void Object::ConvertWTP(){
	if (myplanet){ //������
		Vector2D<double> r_pos = position - myplanet->GetPositionXY();
		t = fmod(atan2(r_pos.y, r_pos.x) + DX_PI * 2, DX_PI * 2) / DX_PI * 180.0;
		r = r_pos.Length();
	}
	else{
		t = position.x;
		r = -position.y;
	}
}

//�f���ɍ��W���x��f���ɍ��W�ɓK�p����
void Object::ApplyVelocity(){
	if (myplanet){
		//��������
		r += vr;
		//��������
		if (r != 0){ t += vh / (r*DX_PI / 180.0); }
		t = fmod(t + 360, 360.0);
	}
	else{
		r += vr;
		t += vh;
	}
	//�p�x
	rotation += rota_vel;
	rotation = fmod(rotation + 360, 360.0);
}

void Object::Draw(){}

//�Փˏ���
void Object::ProcessCollision(){}
//�}�l�[�W��
//void Object::ResistToManager(){}
//void Object::UnresistFromManager(){}

double Object::GetVH()const{ return vh; }
double Object::GetVR()const{ return vr; }
double Object::GetRotation()const{ return rotation; }
double Object::GetRotationVelocity()const{ return rota_vel; }
//�����e��Ԃ�
shared_ptr<Object> Object::GetParent()const{
	return parent;
}

//�����f��
shared_ptr<Planet> Object::GetBelongingPlanet()const{
	return myplanet;
}

//����
ObjectBelongingTo Object::GetBelongingTo()const{ return _obj_belonging_to; }

//�^�[�Q�b�g�擾
shared_ptr<CharacterBase> Object::GetTarget()const{ return target; }

Vector2D<double> Object::GetPositionXY()const{ return position; }


//�Z�b�^�[
void Object::SetBelongingPlanet(shared_ptr<Planet> p){
//	lock_guard<recursive_mutex> lock(_planet_pointer_mtx);
	if (myplanet){
		ConvertPTW(); //XY���W���v�Z��
	}
	myplanet = p;
	ConvertWTP();//�V�����f���̋ɍ��W���t�Z����
}

void Object::SetXY(const Vector2D<double>& xy){
	position = xy;
	ConvertWTP();
}

void Object::SetTR(double m_t, double m_r){
//	lock_guard<recursive_mutex> lock_t(_t_mtx);
//	lock_guard<recursive_mutex> lock_r(_r_mtx);
	t = m_t; r = m_r;
	ConvertPTW();
}

void Object::SetVhVr(double m_w, double m_vr){
//	lock_guard<recursive_mutex> lock_vh(_vh_mtx);
//	lock_guard<recursive_mutex> lock_vr(_vr_mtx);
	vh = m_w; vr = m_vr;
}

void Object::SetRotation(double rot){ rotation = rot; }

void Object::SetPolarVelocity(double spd, double dir_deg){
	vh = spd * cos(dir_deg / 180.0*DX_PI);
	vr = spd * sin(dir_deg / 180.0*DX_PI);
}

void Object::SetRatationVelocity(double rv){
	rota_vel = rv;
}

void Object::SetObjectBelongingTo(ObjectBelongingTo obt){ _obj_belonging_to = obt; }
void Object::SetT(double mt){ t = mt; }
void Object::SetR(double mr){ r = mr; }
void Object::SetVH(double mvh){ vh = mvh; }
void Object::SetVR(double mvr){ vr = mvr; }

//�Փ˂�b����y�N�g�ǉ�
void Object::AddCollisionData(const CollisionDataWithPlanet& c){
	col_data_p.push_back(c);
}
void Object::AddCollisionData(const CollisionDataWithCharacter& c){
	col_data_c.push_back(c);
}

//�`��̉�]�擾
double Object::GetDrawRotation()const{
	double p_rot = 0.0;
	if (myplanet){
		p_rot = t + 90;
	}
	return fmod(p_rot + rotation, 360.0);
}

//�^�[�Q�b�g�֘A
double Object::GetTargetT()const{ return target ? target->GetT() : -1.0; }
double Object::GetTargetR()const{ return target ? target->GetR() : -1.0; }
double Object::GetTargetDistance()const{
	if (target == nullptr){ return -1.0; }
	Vector2D<double> pp = target->GetPositionXY();
	return Vector2D<double>::Length(pp - position);
}
//�^�[�Q�b�g�Z�b�g
void Object::SetTarget(shared_ptr<CharacterBase> p){ 
//	lock_guard<recursive_mutex> lock(_target_pointer_mtx);
	target = p; 
}

//�^�[�Q�b�g�̑���T�擾
double Object::GetTargetTFromMe()const{
	double out = GetTargetT() - t;
	if (myplanet){
		if (out < 0.0){ out += 360.0; }
		return out <= 180.0 ? out : out - 360.0;
	}
	else{
		return out;
	}
}

//�^�[�Q�b�g�̑���R�擾
double Object::GetTargetRFromMe()const{
	return GetTargetR() - r;
}

//�^�[�Q�b�g�̕����擾
double Object::GetTargetDirectionByDeg()const{
	return atan2(GetTargetRFromMe(), GetTargetTFromMe() / 180.0*DX_PI*(r + (target ? target->GetR() : 0)) / 2) / DX_PI*180.0;
}

/*�����ړ��֘A�֐�*/

bool Object::IsAutoMoving()const{
	return _auto_move != nullptr;
}

void Object::StartLinearMove(const Vector2D<double>& to, int f){
	_auto_move = make_unique<LinearMove>(Vector2D<double>(t, r), to, f);
}

void Object::StartHermiteMove(const Vector2D<double>& to , const Vector2D<double>& s_v, const Vector2D<double>& e_v, int f){
	_auto_move = make_unique<HermiteMove>(Vector2D<double>(t, r), to, s_v, e_v, f);
}

void Object::StartBezierMove(const Vector2D<double>& to, const std::vector<Vector2D<double>>& point, int f){
	_auto_move = make_unique<BezierMove>(Vector2D<double>(t, r), to, point,f);
}

/*�����ړ�*/

//�����ړ�����
void Object::ProcessAutoMove(){
	if (_auto_move){
		Vector2D<double> pos;
		if (_auto_move->move(&pos) == false){
			_auto_move.release();
		}
		if (myplanet){
			t = fmod(pos.x, 360.0);
			while (t < 0){ t += 360.0; }
		}
		else{
			t = pos.x;
		}
		r = pos.y;
	}
}

//�����ړ�
Object::LinearMove::LinearMove(const Vector2D<double>& from, const Vector2D<double>& to, int f) :_interp(){
	_interp.push_back(from,0);
	_interp.push_back(to, f);
	it = _interp.begin();
}

bool Object::LinearMove::move(Vector2D<double>* pos){
	++it;
	*pos = *it;
	if (it == _interp.end()){ return false; }
	else{ return true; }
}

//�G���~�[�g�ړ�
Object::HermiteMove::HermiteMove(const Vector2D<double>& from, const Vector2D<double>& to, const Vector2D<double>& s_v, const Vector2D<double>& e_v, int f):_hCurve(from,s_v,to,e_v,f),it(_hCurve.begin()){
}

bool Object::HermiteMove::move(Vector2D<double>* pos){
	++it;
	*pos = *it;
	if (it == _hCurve.end()){ return false; }
	else{ return true; }
}

//�x�W�F�ړ�
Object::BezierMove::BezierMove(const Vector2D<double>& from, const Vector2D<double>& to, const vector<Vector2D<double>>& point, int f){
	_bCurve.push_back(from);
	for (auto& p : point){
		_bCurve.push_back(p);
	}
	_bCurve.push_back(to);
	it = _bCurve.begin();
}

bool Object::BezierMove::move(Vector2D<double>* pos){
	++it;
	*pos = *it;
	if (it == _bCurve.end()){ return false; }
	else{ return true; }
}
