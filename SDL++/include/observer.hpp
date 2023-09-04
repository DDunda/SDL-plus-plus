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
	virtual inline ~IObserver() = 0;

	virtual void AddSubject(ISubject<Ts...>& s) = 0;
	virtual void RemoveSubject(ISubject<Ts...>& s) = 0;
	virtual void Register(ISubject<Ts...>& s) = 0;
	virtual void Unregister(ISubject<Ts...>& s) = 0;
	virtual void Notify(Ts... args) = 0;
};

template<typename... Ts>
struct ISubject
{
	virtual inline ~ISubject() = 0;

	virtual void AddObserver(IObserver<Ts...>& o) = 0;
	virtual void RemoveObserver(IObserver<Ts...>& o) = 0;
	virtual void Register(IObserver<Ts...>& o) = 0;
	virtual void Unregister(IObserver<Ts...>& o) = 0;
	virtual void Notify(Ts... args) = 0;
};

template<typename... Ts>
class Observer : public IObserver<Ts...> {
protected:
	std::vector<ISubject<Ts...>*> subjects = {};

public:
	inline Observer();
	inline Observer(const Observer<Ts...>& ob);
	inline Observer(Observer<Ts...>&& ob) noexcept;

	virtual inline Observer& operator=(const Observer<Ts...>& ob);
	virtual inline Observer& operator=(Observer<Ts...>&& ob) noexcept;
	virtual inline ~Observer();

	virtual void AddSubject(ISubject<Ts...>& s);
	virtual void RemoveSubject(ISubject<Ts...>& s);
	virtual void Register(ISubject<Ts...>& s);
	virtual void Unregister(ISubject<Ts...>& s);
	virtual void Notify(Ts... args) = 0;
};

template<typename... Ts>
class Subject : public ISubject<Ts...>
{
protected:
	std::vector<IObserver<Ts...>*> observers = {};

public:
	inline Subject();
	inline Subject(const Subject<Ts...>& sub);
	inline Subject(Subject<Ts...>&& sub) noexcept;

	virtual inline Subject& operator=(const Subject<Ts...>& sub);
	virtual inline Subject& operator=(Subject<Ts...>&& sub) noexcept;
	virtual inline ~Subject();

	virtual void AddObserver(IObserver<Ts...>& o);
	virtual void RemoveObserver(IObserver<Ts...>& o);
	virtual void Register(IObserver<Ts...>& o);
	virtual void Unregister(IObserver<Ts...>& o);
	virtual void Notify(Ts... args);
};

template<typename... Ts>
class Listener : public Observer<Ts...> {
protected:
	std::function<void(Ts...)> function;
public:
	Listener(std::function<void(Ts...)> function) : function(function) {};
	Listener(std::function<void(Ts...)> function, ISubject<Ts...>& subject) : function(function) { subject.Register(*this); };
	virtual void Notify(Ts... args) { function(args...); }
};

template<typename... Ts>
IObserver<Ts...>::~IObserver() {}

template<typename... Ts>
ISubject<Ts...>::~ISubject() {}

#pragma region Observer

#pragma region Constructors

template<typename... Ts>
inline Observer<Ts...>::Observer() : subjects() {}

template<typename... Ts>
inline Observer<Ts...>::Observer(const Observer<Ts...>& ob)
{
	for (auto& s : ob.subjects)
	{
		Register(*s);
	}
}

template<typename... Ts>
inline Observer<Ts...>::Observer(Observer<Ts...>&& ob) noexcept : subjects()
{
	while (!ob.subjects.empty())
	{
		Register(*ob.subjects.front());
		ob.Unregister(*ob.subjects.front());
	}
}

#pragma endregion

#pragma region Assignment

template<typename... Ts>
inline Observer<Ts...>& Observer<Ts...>::operator=(const Observer<Ts...>& ob)
{
	for (auto& s : ob.subjects)
	{
		Register(*s);
	}
	return *this;
}

template<typename... Ts>
inline Observer<Ts...>& Observer<Ts...>::operator=(Observer<Ts...>&& ob) noexcept
{
	while (!ob.subjects.empty())
	{
		Register(*ob.subjects.front());
		ob.Unregister(*ob.subjects.front());
	}
	return *this;
}

#pragma endregion

#pragma region Methods

template<typename... Ts>
void Observer<Ts...>::AddSubject(ISubject<Ts...>& s)
{
	subjects.push_back(&s);
}

template<typename... Ts>
void Observer<Ts...>::RemoveSubject(ISubject<Ts...>& s)
{
	auto it = std::find(subjects.begin(), subjects.end(), &s);
	if (it == subjects.end()) return;
	subjects.erase(it);
}

template<typename... Ts>
void Observer<Ts...>::Register(ISubject<Ts...>& s)
{
	AddSubject(s);
	s.AddObserver(*this);
}

template<typename... Ts>
void Observer<Ts...>::Unregister(ISubject<Ts...>& s)
{
	RemoveSubject(s);
	s.RemoveObserver(*this);
}

#pragma endregion

template<typename... Ts>
inline Observer<Ts...>::~Observer()
{
	while (!subjects.empty())
	{
		Unregister(*subjects.front());
	}
}

#pragma endregion

#pragma region Subject

#pragma region Constructors

template<typename... Ts>
inline Subject<Ts...>::Subject() : observers() {}

template<typename... Ts>
inline Subject<Ts...>::Subject(const Subject<Ts...>& sub)
{
	for (auto& o : sub.observers)
	{
		Register(*o);
	}
}

template<typename... Ts>
inline Subject<Ts...>::Subject(Subject<Ts...>&& sub) noexcept : observers()
{
	while (!sub.observers.empty())
	{
		Register(*sub.observers.front());
		sub.Unregister(*sub.observers.front());
	}
}

#pragma endregion

#pragma region Assignment

template<typename... Ts>
inline Subject<Ts...>& Subject<Ts...>::operator=(const Subject<Ts...>& sub)
{
	for (auto& o : sub.observers)
	{
		Register(*o);
	}
	return *this;
}

template<typename... Ts>
inline Subject<Ts...>& Subject<Ts...>::operator=(Subject<Ts...>&& sub) noexcept
{
	while (!sub.observers.empty())
	{
		Register(*sub.observers.front());
		sub.Unregister(*sub.observers.front());
	}
	return *this;
}
#pragma endregion

#pragma region Methods

template<typename... Ts>
void Subject<Ts...>::AddObserver(IObserver<Ts...>& o)
{
	observers.push_back(&o);
}

template<typename... Ts>
void Subject<Ts...>::RemoveObserver(IObserver<Ts...>& o)
{
	auto it = std::find(observers.begin(), observers.end(), &o);
	if (it == observers.end()) return;
	observers.erase(it);
}

template<typename... Ts>
void Subject<Ts...>::Register(IObserver<Ts...>& o)
{
	AddObserver(o);
	o.AddSubject(*this);
}

template<typename... Ts>
void Subject<Ts...>::Unregister(IObserver<Ts...>& o)
{
	RemoveObserver(o);
	o.RemoveSubject(*this);
}

template<typename... Ts>
void Subject<Ts...>::Notify(Ts... args)
{
	for (auto& o : observers) o->Notify(args...);
}

#pragma endregion

template<typename... Ts>
inline Subject<Ts...>::~Subject()
{
	while (!observers.empty()) Unregister(*observers.front());
}

#pragma endregion

#endif SDLpp_observer_h_