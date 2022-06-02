#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <cassert>

#include "Component.h"
#include "ecs.h"
#include "Entity.h"
#include "System.h"

namespace ecs {

class Manager {

public:
	Manager();
	virtual ~Manager();

	inline Entity* addEntity(grpId gId = _grp_GENERAL) 
	{
		auto e = new Entity(gId);
		e->alive_ = true;
		entsByGroup_[gId].push_back(e);
		return e;
	}

	inline void setAlive(Entity *e, bool alive) {
		e->alive_ = alive;
	}

	inline bool isAlive(Entity *e) {
		return e->alive_;
	}

	template<typename T, typename ...Ts>
	inline T* addComponent(Entity *e, Ts &&... args) 
	{
		constexpr cmpId_type cId = T::id;
		assert(cId < maxComponentId);
		//removeComponent<T>(e);
		Component *c = new T(std::forward<Ts>(args)...);
		c->setContext(e, this);
		c->initComponent();
		e->cmps_[cId] = c;
		e->currCmps_.push_back(c);
		return static_cast<T*>(c);
	}


	/*template<typename T>
	inline void removeComponent(Entity *e) 
	{
		constexpr cmpId_type cId = T::id;
		assert(cId < maxComponentId);

		if (e->cmps_[cId] != nullptr) 
		{
			auto iter = std::find(e->currCmps_.begin(), e->currCmps_.end(),
					e->cmps_[cId]);
			assert(iter != e->currCmps_.end());
			e->currCmps_.erase(iter);
			delete e->cmps_[cId];
			e->cmps_[cId] = nullptr;
		}
	}*/


	template<typename T>
	inline T* getComponent(Entity *e) {

		constexpr cmpId_type cId = T::id;
		assert(cId < maxComponentId);

		return static_cast<T*>(e->cmps_[cId]);
	}

	template<typename T>
	inline bool hasComponent(Entity *e) {

		constexpr cmpId_type cId = T::id;
		assert(cId < maxComponentId);

		return e->cmps_[cId] != nullptr;
	}

	inline grpId_type groupId(Entity *e) {
		return e->gId_;
	}

	inline const auto& getEntities(grpId_type gId = _grp_GENERAL) {
		return entsByGroup_[gId];;
	}

	inline void setHandler(hdlrId_type hId, Entity *e) {
		assert(hId < maxHandlerId);
		hdlrs_[hId] = e;
	}

	inline Entity* getHandler(hdlrId_type hId) {
		assert(hId < maxHandlerId);
		return hdlrs_[hId];
	}

	template<typename T, typename ...Ts>
	inline T* addSystem(Ts &&... args) {

		constexpr sysId_type sId = T::id;
		assert(sId < maxSystemId);
		removeSystem<T>();
		System *s = new T(std::forward<Ts>(args)...);
		s->setContext(this);
		sys_[sId] = s;
		return static_cast<T*>(s);
	}

	template<typename T>
	inline void removeSystem() {

		constexpr sysId_type sId = T::id;
		assert(sId < maxSystemId);

		if (sys_[sId] != nullptr) {
			delete sys_[sId];
			sys_[sId] = nullptr;
		}
	}

	template<typename T>
	inline T* getSystem() {

		constexpr sysId_type sId = T::id;
		assert(sId < maxSystemId);

		return static_cast<T*>(sys_[sId]);
	}

	inline void send(const Message &m, bool delay = false) {
		if (!delay) {
			for (System *s : sys_) {
				if (s != nullptr)
					s->receive(m);
			}
		} else {
			msgs_.emplace_back(m);
		}
	}

	
	/*inline void flushMessages() {
		auto size = msgs_.size();
		for (auto i = 0u; i < size; i++) {
			auto &m = msgs_[i];
			for (System *s : sys_) {
				if (s != nullptr)
					s->receive(m);
			}
		}
		msgs_.erase(msgs_.begin(), msgs_.begin() + size);
	}*/

	
	/*inline void flushMessagesWithSwap() {
		std::swap(msgs_, msgs_aux_);
		for (auto &m : msgs_aux_) {
			for (System *s : sys_) {
				if (s != nullptr)
					s->receive(m);
			}
		}

		msgs_aux_.clear();
	}*/
	
	void refresh();

private:

	std::array<Entity*, maxHandlerId> hdlrs_;
	std::array<std::vector<Entity*>, maxGroupId> entsByGroup_;
	std::array<System*, maxSystemId> sys_;

	std::vector<Message> msgs_;
	std::vector<Message> msgs_aux_;

};

} 
