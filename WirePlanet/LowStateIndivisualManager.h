#pragma once

#include<string>
#include<unordered_map>
#include<memory>

//下位状態インターフェス
class LowState{
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Leave() = 0;
};
//下位状態
template <class C>
class LowStateC : public LowState{
private:
	using LowStateFunc = void(C::*)();
public:
	LowStateC(C*, LowStateFunc, LowStateFunc, LowStateFunc);
	void Enter()override;
	void Update()override;
	void Leave()override;
private:
	C* p;
	LowStateFunc enter;
	LowStateFunc update;
	LowStateFunc leave;
};
template<class C>
LowStateC<C>::LowStateC(C* m_p,LowStateFunc e, LowStateFunc u, LowStateFunc l) :p(m_p),enter(e), update(u), leave(l){}
template<class C>
void LowStateC<C>::Enter(){ if (enter){ (p->*enter)(); } }
template<class C>
void LowStateC<C>::Update(){ if (update){ (p->*update)(); } }
template<class C>
void LowStateC<C>::Leave(){ if (leave){ (p->*leave)(); } }

// 下位状態個別管理クラス
class LowStateIndivisualManager{
public:
	LowStateIndivisualManager();
	int ChangeLowState(const std::string&);
	void Update();
protected:
	std::unordered_map<std::string, std::unique_ptr<LowState>> ls_map;
	std::unique_ptr<LowState>* current_lowstate;
};
//下位状態個別管理クラステンプレート
template<class C>
class LowStateIndivisualManagerC : public LowStateIndivisualManager{
private:
	using LowStateFunc = void(C::*)();
public:
	LowStateIndivisualManagerC(C*);
	void ResistLowState(const std::string&, LowStateFunc, LowStateFunc, LowStateFunc);
private:
	C* p;
};
template<class C>
LowStateIndivisualManagerC<C>::LowStateIndivisualManagerC(C* m_p) :p(m_p){}
template<class C>
void LowStateIndivisualManagerC<C>::ResistLowState(const std::string& n, LowStateFunc e, LowStateFunc u, LowStateFunc l){
	ls_map.insert(std::unordered_map<std::string, std::unique_ptr<LowState>>::value_type(n, std::make_unique<LowStateC<C>>(p, e, u, l)));
}
