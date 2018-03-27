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

	virtual void Update(double deltaTime) = 0;
	virtual void Draw() = 0;

	virtual State * NewState() { return this->m_newState; }
	virtual bool Exit() { return this->m_exitState; }
};

