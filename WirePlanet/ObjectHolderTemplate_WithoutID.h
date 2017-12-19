#pragma once

#include<vector>
#include<memory>
#include<algorithm>

template <class C_obj>
class ObjectHolderTemplate_WithoutID{
public:
	ObjectHolderTemplate_WithoutID();
	void process_resist_list();
	void process_unresist_list();
	int resist_object(std::shared_ptr<C_obj>);
	int unresist_object(std::weak_ptr<C_obj>);
	int unresist_object(C_obj*);
	bool is_exist(std::weak_ptr<C_obj>)const;
	bool is_exist(C_obj*)const;
	int size()const;
	void do_all(void(C_obj::*)());

	void clear();
	void clean();

	std::vector<std::shared_ptr<C_obj>> objects;
	std::vector<std::shared_ptr<C_obj>> res_list;
	std::vector<C_obj*> unres_list;
};

template <class C_obj>
ObjectHolderTemplate_WithoutID<C_obj>::ObjectHolderTemplate_WithoutID(){}

template <class C_obj>
void ObjectHolderTemplate_WithoutID<C_obj>::process_resist_list(){
	for (auto it = res_list.begin(); it != res_list.end(); ++it){
		objects.push_back(*it);
	}
	res_list.clear();
}

template <class C_obj>
void ObjectHolderTemplate_WithoutID<C_obj>::process_unresist_list(){
	objects.erase(std::remove_if(objects.begin(), objects.end(), [this](std::shared_ptr<C_obj> o)->bool{return std::find(unres_list.begin(), unres_list.end(), o.get()) != unres_list.end(); }), objects.end());
	unres_list.clear();
}

template <class C_obj>
int ObjectHolderTemplate_WithoutID<C_obj>::resist_object(std::shared_ptr<C_obj> obj){
	res_list.push_back(obj);
	return 0;
}

template <class C_obj>
int ObjectHolderTemplate_WithoutID<C_obj>::unresist_object(std::weak_ptr<C_obj> obj){
	if (std::find(objects.begin(), objects.end(), obj.lock()) == objects.end()){ return -1; }
	unres_list.push_back(obj.lock().get());
	return 0;
}
template <class C_obj>
int ObjectHolderTemplate_WithoutID<C_obj>::unresist_object(C_obj* obj){
	if (std::find_if(objects.begin(), objects.end(), [obj](shared_ptr<C_obj> c)->bool{ return c.get() == obj; }) == objects.end()){ return -1; }
	unres_list.push_back(obj);
	return 0;
}

template <class C_obj>
bool ObjectHolderTemplate_WithoutID<C_obj>::is_exist(std::weak_ptr<C_obj> obj)const{
	return std::find(objects.begin(), objects.end(), obj.lock()) == objects.end() ? false : true;
}
template <class C_obj>
bool ObjectHolderTemplate_WithoutID<C_obj>::is_exist(C_obj* obj)const{
	return std::find_if(objects.begin(), objects.end(), [obj](std::shared_ptr<C_obj> o)->bool{return o.get() == obj; }) == objects.end() ? false : true;
}

template <class C_obj>
int ObjectHolderTemplate_WithoutID<C_obj>::size()const{ return (int)objects.size(); }

template <class C_obj>
void ObjectHolderTemplate_WithoutID<C_obj>::clear(){
	objects.clear();
	res_list.clear();
	unres_list.clear();
}

template <class C_obj>
void ObjectHolderTemplate_WithoutID<C_obj>::clean(){
	objects.erase(std::remove_if(objects.begin(), objects.end(), [](std::shared_ptr<C_obj> p)->bool{ return p.use_count() == 1 ? true : false; }), objects.end());
	unres_list.clear();
}

template <class C_obj>
void ObjectHolderTemplate_WithoutID<C_obj>::do_all(void(C_obj::* func)()){
	for (auto it = objects.begin(); it != objects.end(); ++it){
		(*(*it).*func)();
	}
}
