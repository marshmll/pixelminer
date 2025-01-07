#pragma once

class State;

typedef struct 
{
	unsigned int *gridSize;
	std::stack<State *> *states;
	std::map<std::string, sf::Font> *fonts;
	sf::VideoMode *vm;
} StateData;

class State
{
private:
	
protected:
	StateData &data;
	bool dead;

public:
	State(StateData &data);

	virtual ~State();
	
	virtual void update(const float &dt) = 0;

	virtual void render(sf::RenderTarget &target) = 0;

	const bool &isDead() const;

};