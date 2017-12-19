#include"CharacterBase.h"

namespace CreateCharaFuncs{
	//弾発射(初期化ID、方向(°)、速さ、攻撃力倍率、発射キャラ、[回転速度])
	std::shared_ptr<CharacterBase> FireBullet(const std::string&, double, double,double, const CharacterBase*,double=0.0);
	//持続時間指定版弾発射(初期化ID、方向(°)、速さ、攻撃力倍率、発射キャラ、持続時間、[回転速度])
	std::shared_ptr<CharacterBase> FireBulletAssignSuspend(const std::string&, double, double, double, const CharacterBase*, int, double = 0.0);
	//剣を投げる(方向(°),速さ,、攻撃力倍率、投げたキャラのID[,投げ初めのキャラからの距離])
	std::shared_ptr<CharacterBase> ThrowSword(double, double,double, int, int = 0);
	//ドロップエネルギーボール作成(エネルギー量、生成元、ターゲットに向かわせるか)
	std::shared_ptr<CharacterBase> CreateDropEnergyBall(int,const CharacterBase*,bool);
	//ドロップエネルギーボール散布(エネルギー量、生成元、ターゲットに向かわせるか)
	void SprayDropEnergyBall(int, const CharacterBase*, bool);
}