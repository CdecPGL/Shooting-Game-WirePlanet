#pragma once

#include<memory>
#include<unordered_map>
#include<algorithm>

template <typename T_id, class C_obj>
class ObjectHolderTemplate_WithID{
public:
	ObjectHolderTemplate_WithID();
	void process_resist_list();
	void process_unresist_list();
	int resist_object(std::shared_ptr<C_obj>, const T_id&);
	int unresist_object(const T_id&);
	bool is_exist(const T_id&)const;
	int size()const;
	std::shared_ptr<C_obj> find_object(const T_id&)const;
	void do_all(void(C_obj::*)());

	void clear();
	void clean();

	std::unordered_map<T_id, std::shared_ptr<C_obj>> objects;
	std::unordered_map<T_id, std::shared_ptr<C_obj>> res_list;
	std::vector<T_id> unres_list;
};

template <typename T_id, class C_obj>
ObjectHolderTemplate_WithID<T_id, C_obj>::ObjectHolderTemplate_WithID(){}

template <typename T_id, class C_obj>
void ObjectHolderTemplate_WithID<T_id, C_obj>::process_resist_list(){
	for (auto it = res_list.begin(); it != res_list.end(); ++it){
		objects.insert(*it);
	}
	res_list.clear();
}

template <typename T_id, class C_obj>
void ObjectHolderTemplate_WithID<T_id, C_obj>::process_unresist_list(){
	for (auto it = unres_list.begin(); it != unres_list.end(); ++it){
		auto mit = objects.find(*it);
		if (mit != objects.end()){ objects.erase(mit); }
	}
	unres_list.clear();
}

template <typename T_id, class C_obj>
int ObjectHolderTemplate_WithID<T_id, C_obj>::resist_object(std::shared_ptr<C_obj> obj, const T_id& id){
	if (objects.find(id) != objects.end()){ return -1; }
	res_list.insert(std::unordered_map<T_id, std::shared_ptr<C_obj>>::value_type(id, obj));
	return 0;
}

template <typename T_id, class C_obj>
int ObjectHolderTemplate_WithID<T_id, C_obj>::unresist_object(const T_id& id){
	if (objects.find(id) == objects.end()){ return -1; }
	unres_list.push_back(id);
	return 0;
}

template <typename T_id, class C_obj>
bool ObjectHolderTemplate_WithID<T_id, C_obj>::is_exist(const T_id& id)const{
	return objects.find(id) == objects.end() ? false : true;
}

template <typename T_id, class C_obj>
std::shared_ptr<C_obj> ObjectHolderTemplate_WithID<T_id, C_obj>::find_object(const T_id& id)const{
	auto it = objects.find(id);
	if (it == objects.end()){ return nullptr; }
	return (*it).second;
}

template <typename T_id, class C_obj>
int ObjectHolderTemplate_WithID<T_id, C_obj>::size()const{ return (int)objects.size(); }

template <typename T_id, class C_obj>
void ObjectHolderTemplate_WithID<T_id, C_obj>::clear(){
	objects.clear();
	res_list.clear();
	unres_list.clear();
}

template <typename T_id, class C_obj>
void ObjectHolderTemplate_WithID<T_id, C_obj>::clean(){
	//objects.erase(std::remove_if(objects.begin(), objects.end(), [](std::unordered_map<T_id,std::shared_ptr<C_obj>>::value_type o)->bool{ return o.second.use_count()==1; }), objects.end());
	for (auto it = objects.begin(); it != objects.end();){
		if ((*it).second.use_count() == 1) {
			it = objects.erase(it);
			continue;
		}
		++it;
	}
	unres_list.clear();
}

template <typename T_id, class C_obj>
void ObjectHolderTemplate_WithID<T_id, C_obj>::do_all(void(C_obj::* func)()){
	for (auto it = objects.begin(); it != objects.end(); ++it){
		((*(it->second)).*func)();
	}
}
