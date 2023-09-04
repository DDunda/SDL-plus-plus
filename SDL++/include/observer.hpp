#pragma once

#ifndef SDLpp_observer_h_
#define SDLpp_observer_h_

#include <vector>
#include <map>
#include <functional>

template<typename... Ts>
struct ISubject;

template<typename... Ts>
struct IObserver
{
	virtual void AddSubject(ISubject<Ts...>& s) = 0;
	virtual void RemoveSubject(ISubject<Ts...>& s) = 0;
	virtual void Register(ISubject<Ts...>& s) = 0;
	virtual void Unregister(ISubject<Ts...>& s) = 0;
	virtual void Notify(Ts... args) = 0;

	virtual inline ~IObserver() = 0;
};

template<typename... Ts>
IObserver<Ts...>::~IObserver() {}

template<typename... Ts>
struct ISubject
{
	virtual void AddObserver(IObserver<Ts...>& o) = 0;
	virtual void RemoveObserver(IObserver<Ts...>& o) = 0;
	virtual void Register(IObserver<Ts...>& o) = 0;
	virtual void Unregister(IObserver<Ts...>& o) = 0;
	virtual void Notify(Ts... args) = 0;

	virtual inline ~ISubject() = 0;
};

template<typename... Ts>
ISubject<Ts...>::~ISubject() {}

template<typename... Ts>
class Observer : public IObserver<Ts...> {
protected:
	std::vector<ISubject<Ts...>*> subjects = {};

public:
	virtual void AddSubject(ISubject<Ts...>& s) { subjects.push_back(&s); }

	virtual void RemoveSubject(ISubject<Ts...>& s)
	{
		auto it = std::find(subjects.begin(), subjects.end(), &s);
		if (it == subjects.end()) return;
		subjects.erase(it);
	}

	virtual void Register(ISubject<Ts...>& s)
	{
		AddSubject(s);
		s.AddObserver(*this);
	}

	virtual void Unregister(ISubject<Ts...>& s)
	{
		RemoveSubject(s);
		s.RemoveObserver(*this);
	}

	virtual void Notify(Ts... args) = 0;
	
	virtual inline ~Observer() { while(!subjects.empty()) Unregister(*subjects.front()); }
};

template<typename... Ts>
class Subject : public ISubject<Ts...>
{
protected:
	std::vector<IObserver<Ts...>*> observers = {};

public:
	virtual void AddObserver(IObserver<Ts...>& o) { observers.push_back(&o); }

	virtual void RemoveObserver(IObserver<Ts...>& o) {
		auto it = std::find(observers.begin(), observers.end(), &o);
		if (it == observers.end()) return;
		observers.erase(it);
	}

	virtual void Register(IObserver<Ts...>& o)
	{
		AddObserver(o);
		o.AddSubject(*this);
	}

	virtual void Unregister(IObserver<Ts...>& o)
	{
		RemoveObserver(o);
		o.RemoveSubject(*this);
	}

	virtual void Notify(Ts... args)
	{
		for (auto o : observers) o->Notify(args...);
	}

	virtual inline ~Subject() { while (!observers.empty()) Unregister(*observers.front()); }
};

template<typename... Ts>
class Listener : public Observer<Ts...> {
protected:
	std::function<void(Ts...)> function;
public:
	Listener(std::function<void(Ts...)> function) : function(function) {};
	Listener(std::function<void(Ts...)> function, ISubject<Ts...>& subject) : function(function) { this->Register(subject); };
	virtual void Notify(Ts... args) { function(args...); }
};

#endif SDLpp_observer_h_