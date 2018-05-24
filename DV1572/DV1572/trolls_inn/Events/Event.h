#pragma once
#include "../../trolls_inn/AI_Base/Inn.h"
#include "../../InGameConsole.h"

template <class t, typename r>
class CallQueue
{
	r _Execute(void (t::*func)(), t* c)
	{
		return (*c.*func)();
	}
	std::queue<r(t::*)()> queue;
public:
	
	r Execute(t * type)
	{
		if (!Empty())
		{
			return _Execute(queue.front(), type);
		}
		return r();
	}
	bool Empty()
	{
		return queue.empty();
	}
	void Push(void (t::*func)())
	{
		queue.push(func);
	}
	void Pop()
	{
		if (queue.size())
			queue.pop();
	}
	size_t Size()
	{
		return queue.size();
	}

	
};

class Event
{
private:
	bool m_exit = false;
protected:
	Inn * m_inn;
	RoomCtrl * m_roomCtrl;
	void exitState();

public:
	Event(Inn * inn, RoomCtrl * roomctrl);
	virtual ~Event();

	virtual void Update() = 0;
	virtual void Draw() = 0;

	bool exit() const;
};
