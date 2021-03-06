#pragma once

#include<string>
#include<memory>

template<class C>
class CreatorTemplate
{
public:
	CreatorTemplate();
	virtual ~CreatorTemplate();
	virtual int Init();
	virtual std::shared_ptr<C> Create(const std::string&) = 0; //初期化ID
	virtual bool CheckInitIDExistence(const std::string&)const; //指定の初期化IDが存在するか
	bool CheckInitialized()const;
private:
	bool initialized;
};

template<class C>
CreatorTemplate<C>::CreatorTemplate() :initialized(false)
{
}

template<class C>
CreatorTemplate<C>::~CreatorTemplate()
{
}

template<class C>
int CreatorTemplate<C>::Init(){
	initialized = true;
	return 0;
}

template<class C>
bool CreatorTemplate<C>::CheckInitIDExistence(const std::string&)const{ return true; }

template<class C>
bool CreatorTemplate<C>::CheckInitialized()const{ return initialized; }
