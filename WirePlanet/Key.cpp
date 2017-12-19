#include "Key.h"
#include <cassert>

namespace {
	constexpr unsigned int DPAD_UP = 1; //�\�����͏�
	constexpr unsigned int DPAD_DOWN = 2; //�\�����͉�
	constexpr unsigned int DPAD_LEFT = 4; //�\�����͍�
	constexpr unsigned int DPAD_RIGHT = 8; //�\�����͉E
	constexpr int ANALOG_INPUT_FREE_PER_1000 = 100; //�A�i���O���̗͂V��
}

//�L�[���̓N���X

//�R���X�g���N�^
nsKey::nsKey(){
	for(int i=0;i<256;i++){
		key[i] = 0;
		pkey[i] = 0;
		burstflag[i] = false;
		counter[i] = 0;
		interval[i] = 0;
	}
}

//�f�X�g���N�^
nsKey::~nsKey(){}

int nsKey::Update() {
	UpdateKey();
	UpdateJoyPad();
	UpdateDirectionInput();
	return 0;
}

//�L�[�f�[�^�̍X�V
int nsKey::UpdateKey(){
	for(int i=0;i<256;i++){
		pkey[i] = key[i];
		if(burstflag[i] == true){
			counter[i]++;
		}
	}
	pmouse_input = mouse_input;
	mouse_input = GetMouseInput();
	return GetHitKeyStateAll(key);
}

int nsKey::UpdateJoyPad() {
	ppad_input = pad_input;
	pad_input = GetJoypadInputState(DX_INPUT_PAD1);
	return 0;
}

//�L�[�̓��͏󋵂��擾
char nsKey::GetKeyStatus(int n) const{
	if(n<0 || 255<n){return -1;}
	return key[n];
}

//�L�[�������ꂽ��
char nsKey::GetKeyPush(int n,bool bcrc) const{
	if(n<0 || 255<n){return -1;}
	if(burstflag[n] == false){ //�A�˂Ȃ�
		if(pkey[n] == 0 && key[n] == 1){
			return 1;
		}
		return 0;
	}else{ //�A�˂���
		if(key[n] == 1 && counter[n] >= interval[n]){
			if (bcrc){ counter[n] = 0; }
			return 1;
		}
		return 0;
	}
}

//����L�[��burstflag���擾
bool nsKey::GetBurstFlag(int n) const{
	if(n<0 || 255<n){return false;}
	return burstflag[n];
}

//����L�[��burstflag���Z�b�g
void nsKey::SetBurstFlag(int n,bool flag){
	if(n<0 || 255<n){return;}
	burstflag[n] = flag;
	counter[n] = 0;
}

//����L�[�̘A�ˊԊu���Z�b�g
void nsKey::SetInterval(int n,int m_interval){
	if (n<0 || 255<n){ return; }
	interval[n] = m_interval;
}

//�}�E�X�̓���{�^���̏�Ԃ��擾
bool nsKey::GetMouseButtonState(int bk){
	if (mouse_input && bk){ return true; }
	else{ return false; }
}

//�}�E�X����{�^���������ꂽ��
bool nsKey::GetMouseButtonPush(int bk){
	int mouse_push = ~pmouse_input & mouse_input;
	if (mouse_push && bk){ return true; }
	else{ return false; }
}

//�J�[�\���̍��W���擾
void nsKey::GetMousePosition(int& x, int& y){
	GetMousePoint(&x, &y);
}

int nsKey::UpdateDirectionInput() {
	if (GetJoypadNum() > 0) {
		const auto direction_definer = [](int dd)->int {
			switch (dd) {
			case 0:return DPAD_UP; break; //��
			case 1:return DPAD_UP | DPAD_RIGHT; break; //�E��
			case 2:return DPAD_RIGHT; break; //�E
			case 3:return DPAD_RIGHT | DPAD_DOWN; break; //�E��
			case 4:return DPAD_DOWN; break; //��
			case 5:return DPAD_DOWN | DPAD_LEFT; break; //����
			case 6:return DPAD_LEFT; break; //��
			case 7:return DPAD_LEFT | DPAD_UP; break; //����
			default:
				assert(false);
				return 0;
			}
		};

		pdir_input = dir_input;
		DINPUT_JOYSTATE din;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &din);
		//�A�i���O����
		int adir_input = 0;
		if (std::abs(din.X) >= ANALOG_INPUT_FREE_PER_1000 || std::abs(din.Y) >= ANALOG_INPUT_FREE_PER_1000) {
			double raw_dir = std::atan2(din.X, -din.Y);
			int norm_dir = static_cast<int>(raw_dir / DX_PI * 18000);
			if (norm_dir < 0) { norm_dir += 36000; }
			adir_input = direction_definer(((norm_dir + 2250) / 4500) % 8);
		}

		//�n�b�g�L�[
		int hdir_input = 0;
		if (din.POV[0] != 0xFFFFFFFF) {
			hdir_input = direction_definer(((din.POV[0] + 2250) / 4500) % 8);
		}

		dir_input = adir_input | hdir_input;
	}
	return 0;
}

bool nsKey::GetJoypadState(int p) {
	return (pad_input & p) ? true : false;
}

bool nsKey::GetJoypadPush(int p) {
	return (~ppad_input & pad_input & p) ? true : false;
}

bool nsKey::GetDirState(int di) {
	return (dir_input & di) ? true : false;
}

bool nsKey::GetDirPush(int di) {
	return (~pdir_input & dir_input & di) ? true : false;
}

bool nsKey::GetCustomInputState(int cb) {
	try {
		return custom_input_map.at(cb).first();
	} catch (std::out_of_range&) {
		return false;
	}
}

bool nsKey::GetCustomInputPush(int cb) {
	try {
		return custom_input_map.at(cb).second();
	} catch (std::out_of_range&) {
		return false;
	}
}

void nsKey::SetDefaultCustomMap() {
	custom_input_map = {
		{ CUSTOM_INPUT_A,
		{ [this] {return GetJoypadState(PAD_INPUT_3) || GetKeyStatus(KEY_INPUT_C); },[this] {return GetJoypadPush(PAD_INPUT_3) || GetKeyPush(KEY_INPUT_C); } } },
		{ CUSTOM_INPUT_B,
		{ [this] {return GetJoypadState(PAD_INPUT_4) || GetKeyStatus(KEY_INPUT_A); },[this] {return GetJoypadPush(PAD_INPUT_4) || GetKeyPush(KEY_INPUT_A); } } },
		{ CUSTOM_INPUT_X,
		{ [this] {return GetJoypadState(PAD_INPUT_1) || GetKeyStatus(KEY_INPUT_Z); },[this] {return GetJoypadPush(PAD_INPUT_1) || GetKeyPush(KEY_INPUT_Z); } } },
		{ CUSTOM_INPUT_Y,
		{ [this] {return GetJoypadState(PAD_INPUT_2) || GetKeyStatus(KEY_INPUT_X); },[this] {return GetJoypadPush(PAD_INPUT_2) || GetKeyPush(KEY_INPUT_X); } } },
		{ CUSTOM_INPUT_L,
		{ [this] {return GetJoypadState(PAD_INPUT_5) || GetKeyStatus(KEY_INPUT_S); },[this] {return GetJoypadPush(PAD_INPUT_5) || GetKeyPush(KEY_INPUT_S); } } },
		{ CUSTOM_INPUT_R,
		{ [this] {return GetJoypadState(PAD_INPUT_6) || GetKeyStatus(KEY_INPUT_D); },[this] {return GetJoypadPush(PAD_INPUT_6) || GetKeyPush(KEY_INPUT_D); } } },
		{ CUSTOM_INPUT_START,
		{ [this] {return GetJoypadState(PAD_INPUT_12) || GetKeyStatus(KEY_INPUT_SPACE); },[this] {return GetJoypadPush(PAD_INPUT_12) || GetKeyPush(KEY_INPUT_SPACE); } } },
		{ CUSTOM_INPUT_UP,
		{ [this] {return GetDirState(DPAD_UP) || GetKeyStatus(KEY_INPUT_UP); },[this] {return GetDirPush(DPAD_UP) || GetKeyPush(KEY_INPUT_UP); } } },
		{ CUSTOM_INPUT_DOWN,
		{ [this] {return GetDirState(DPAD_DOWN) || GetKeyStatus(KEY_INPUT_DOWN); },[this] {return GetDirPush(DPAD_DOWN) || GetKeyPush(KEY_INPUT_DOWN); } } },
		{ CUSTOM_INPUT_LEFT,
		{ [this] {return GetDirState(DPAD_LEFT) || GetKeyStatus(KEY_INPUT_LEFT); },[this] {return GetDirPush(DPAD_LEFT) || GetKeyPush(KEY_INPUT_LEFT); } } },
		{ CUSTOM_INPUT_RIGHT,
		{ [this] {return GetDirState(DPAD_RIGHT) || GetKeyStatus(KEY_INPUT_RIGHT); },[this] {return GetDirPush(DPAD_RIGHT) || GetKeyPush(KEY_INPUT_RIGHT); } } },
	};
}

//�V���O���g����
Key::Key() :nsKey(){}
Key::Key(const Key&){}
Key::~Key(){}
Key& Key::GetInstance(){
	static Key key;
	return key;
}