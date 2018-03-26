#pragma once

class State
{
private:

protected:
	bool m_exitState = false;
	State * m_newState = nullptr;

public:
	State() : m_exitState(false), m_newState(nullptr) {}
	virtual ~State() {}

	virtual void update(double deltaTime) = 0;
	virtual void draw() = 0;

	virtual State * newState() { return this->m_newState; }
	virtual bool exit() { return this->m_exitState; }
};

