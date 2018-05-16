#pragma once
#include "../../trolls_inn/AI_Base/Inn.h"
#include "../../InGameConsole.h"

template <class t>
class CallQueue
{
	void _Execute(void (t::*func)(), t* c)
	{
		(*c.*func)();
	}
	std::queue<void(t::*)()> queue;
public:
	void Execute(t * type)
	{
		if (!empty())
		{
			_Execute(queue.front(), type);
		}
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
		queue.pop();
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
