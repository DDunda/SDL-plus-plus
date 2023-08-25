#pragma once

#ifndef SDLpp_observer_h_
#define SDLpp_observer_h_

#include <vector>
#include <map>
#include <functional>

template<typename... Ts>
class Subject;

template<typename... Ts>
class Observer {
	friend class Subject<Ts...>;
protected:
	std::vector<Subject<Ts...>*> subjects;
public:
	~Observer() {
		for (Subject<Ts...>* subject : subjects) subject->Unregister(*this);
	}

	virtual void Notify(Ts... args) = 0;
};

template<typename... Ts>
class Subject {
protected:
	std::vector<Observer<Ts...>*> observers;
public:
	~Subject() {
		for (Observer<Ts...>* o : observers) Unregister(*o);
	}
	void Register(Observer<Ts...>& o) {
		observers.push_back(&o);
		o.subjects.push_back(this);
	}
	void Unregister(Observer<Ts...>& o) {
		observers.erase(std::remove(observers.begin(), observers.end(), &o));
		o.subjects.erase(std::remove(o.subjects.begin(), o.subjects.end(), this));
	}
	virtual void Notify(Ts... args) {
		for (Observer<Ts...>* o : observers) o->Notify(args...);
	}
};

template<typename... Ts>
class Listener : public Observer<Ts...> {
protected:
	std::function<void(Ts...)> function;
public:
	Listener(std::function<void(Ts...)> function) : function(function) {};
	void Notify(Ts... args) { function(args...); }
};

#endif SDLpp_observer_h_