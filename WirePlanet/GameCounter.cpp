#include "GameCounter.h"
#include"SecureObject.h"
#include"GPIUtil.h"

namespace{
	using s_uint = secure_obj::SecureObject < unsigned int >;
	struct _count{
		s_uint _base_score = 0u;
		s_uint _defeated_enemy =0u;
		s_uint _exploded_enemy = 0u;
		s_uint _received_damage = 0u;
		s_uint _shooted_bullet = 0u;
		s_uint _made_clone = 0u;
		void _reset(){
			_base_score.set(0);
			_defeated_enemy.set(0);
			_exploded_enemy.set(0);
			_received_damage.set(0);
			_shooted_bullet.set(0);
			_made_clone.set(0);
		}
	};
	_count _level_count;
//	_count _game_count;
//	s_uint _total_game_score(0u);
}

namespace GameCounter{
	namespace LevelCounter{
		void ResetLevelCount(){
			_level_count._reset();
		}

		void ApplyLevelCountToGPI(){
			using namespace GPIUtil;
			AddDefeatedEnemyCount(_level_count._defeated_enemy.get());
			AddShootedBulletCount(_level_count._shooted_bullet.get());
			AddMadeCloneCount(_level_count._made_clone.get());
		}
		void AddBaseScore(unsigned int n){
			_level_count._base_score.instance() += n;
			_level_count._base_score.synchronize();
		}
		void AddDefeatedEnemy(unsigned int n){
			_level_count._defeated_enemy.instance() += n;
			_level_count._defeated_enemy.synchronize();
		}
		void AddShootedBullet(unsigned int n){
			_level_count._shooted_bullet.instance() += n;
			_level_count._shooted_bullet.synchronize();
		}
		void AddExplodedEnemy(unsigned int n){
			_level_count._exploded_enemy.instance() += n;
			_level_count._exploded_enemy.synchronize();
		}
		void AddReceivedDamage(unsigned int n){
			_level_count._received_damage.instance() += n;
			_level_count._received_damage.synchronize();
		}
		void AddMadeClone(unsigned int n){
			_level_count._made_clone.instance() += n;
			_level_count._made_clone.synchronize();
		}
		unsigned int GetBaseScore(){
			return _level_count._base_score.get();
		}
		unsigned int GetDefeatedEnemy(){
			return _level_count._defeated_enemy.get();
		}
		unsigned int GetShootedBullet(){
			return _level_count._shooted_bullet.get();
		}
		unsigned int GetExplodedEnemy(){
			return _level_count._exploded_enemy.get();
		}
		unsigned int GetReceivedDamage(){
			return _level_count._received_damage.get();
		}
		unsigned int GetMadeClone(){
			return _level_count._made_clone.get();
		}
	}

	/*void ResetGameCount(){
		_game_count._reset();
		_level_count._reset();
		_total_game_score.set(0);
	}

	void ApplyGameCountToGPI(){
		using namespace GPIUtil;
		AddGamePoint(_total_game_score.get());
		int phs = GetHightScore();
		if (phs < _total_game_score.get()){ SetHighScore(_total_game_score.get()); }
		AddDefeatedEnemyCount(_game_count._defeated_enemy.get());
		AddShootedBulletCount(_game_count._shooted_bullet.get());
	}

	void AddBaseScore(unsigned int n){
		_game_count._base_score.instance() += n;
		_game_count._base_score.synchronize();
	}
	void AddDefeatedEnemy(unsigned int n){
		_game_count._defeated_enemy.instance() += n;
		_game_count._defeated_enemy.synchronize();
	}
	void AddShootedBullet(unsigned int n){
		_game_count._shooted_bullet.instance() += n;
		_game_count._shooted_bullet.synchronize();
	}
	void AddExplodedEnemy(unsigned int n){
		_game_count._exploded_enemy.instance() += n;
		_game_count._exploded_enemy.synchronize();
	}
	void AddReceivedDamage(unsigned int n){
		_game_count._received_damage.instance() += n;
		_game_count._received_damage.synchronize();
	}
	unsigned int GetBaseScore(){
		return _game_count._base_score.get();
	}
	unsigned int GetDefeatedEnemy(){
		return _game_count._defeated_enemy.get();
	}
	unsigned int GetShootedBullet(){
		return _game_count._shooted_bullet.get();
	}
	unsigned int GetExplodedEnemy(){
		return _game_count._exploded_enemy.get();
	}
	unsigned int GetReceivedDamage(){
		return _game_count._received_damage.get();
	}
	void SetTotalScore(unsigned int n){
		_total_game_score.set(n);
	}*/
}
