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
		for (auto subject : subjects) subject->Unregister(*this);
	}

	virtual void Notify(Ts... args) = 0;
};

template<typename... Ts>
class Subject {
protected:
	std::vector<Observer<Ts...>*> observers;
public:
	~Subject() {
		for (auto o : observers) Unregister(*o);
	}
	void Register(Observer<Ts...>& o) {
		observers.push_back(&o);
		o.subjects.push_back(this);
	}
	void Unregister(Observer<Ts...>& o) {
		auto it1 = std::find(observers.begin(), observers.end(), &o);
		if (it1 != observers.end()) observers.erase(it1);

		auto it2 = std::find(o.subjects.begin(), o.subjects.end(), this);
		if (it2 != o.subjects.end()) o.subjects.erase(it2);
	}
	virtual void Notify(Ts... args) {
		for (auto o : observers) o->Notify(args...);
	}
};

template<typename... Ts>
class Listener : public Observer<Ts...> {
protected:
	std::function<void(Ts...)> function;
public:
	Listener(std::function<void(Ts...)> function) : function(function) {};
	Listener(std::function<void(Ts...)> function, Subject<Ts...>& subject) : function(function)
	{
		subject.Register(*this);
	};
	void Notify(Ts... args) { function(args...); }
};

#endif SDLpp_observer_h_