#include "Snake.h"

int RandomNum(int min, int max)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> xd(min, max);
	return xd(rng);
}

CFood::CFood()
{
	this->setSize({ 20.f, 20.f });
	this->setFillColor(Color::Red);
}

void CFood::Spawn(CSnake& snake)
{
	this->m_vecPosition = { float(20 * RandomNum(1, 18)), float(20 * RandomNum(1, 18)) };

	for (unsigned int i = 0; i < snake.Body.size(); i++)
	{
		while (this->m_vecPosition.x == snake.Body.at(i).getPosition().x &&this->m_vecPosition.y == snake.Body.at(i).getPosition().y)
			this->m_vecPosition = { float(20 * RandomNum(1, 18)), float(20 * RandomNum(1, 18)) };
	}

	this->setPosition(this->m_vecPosition);
}

void CFood::Draw(RenderWindow* window)
{
	window->draw(*this);
}

void CSnake::Initialize()
{
	//clear out everything.
	this->Body.clear(); 
	this->m_vecVelocity = { 0.f, 0.f };
	this->m_iScore = 0;

	//init the head
	this->Head.setSize({ 20.f, 20.f });
	this->Head.setFillColor(Color::Green);
	this->Head.setPosition({ float(20 * RandomNum(1, 10)), float(20 * RandomNum(1, 10)) });

	this->Body.emplace_front(this->Head); // add the head to the front of the body EMPLACE_FRONT
}

void CSnake::Draw(RenderWindow* window)
{
	if (!this->Body.empty())
	{
		for (UINT i = 0; i < this->Body.size(); i++)
		{
			window->draw(this->Body.at(i));
		}
	}
}

void CSnake::Think()
{
	if (this->m_bGameOver)
		return;

	bool bHasBody = this->Body.size() > 1;

	if (bHasBody) //ghetto way of preventing the snake from going into itself.
	{
		if (Keyboard::isKeyPressed(Keyboard::Left) && this->m_vecVelocity.x != 20.f)
		{
			this->m_vecVelocity.x = -20.f;
			this->m_vecVelocity.y = 0.f;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right) && this->m_vecVelocity.x != -20.f)
		{
			this->m_vecVelocity.x = 20.f;
			this->m_vecVelocity.y = 0.f;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up) && this->m_vecVelocity.y != 20.f)
		{
			this->m_vecVelocity.x = 0.f;
			this->m_vecVelocity.y = -20.f;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down) && this->m_vecVelocity.y != -20.f)
		{
			this->m_vecVelocity.x = 0.f;
			this->m_vecVelocity.y = 20.f;
		}
	}
	else
	{
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			this->m_vecVelocity.x = -20.f;
			this->m_vecVelocity.y = 0.f;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			this->m_vecVelocity.x = 20.f;
			this->m_vecVelocity.y = 0.f;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			this->m_vecVelocity.x = 0.f;
			this->m_vecVelocity.y = -20.f;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			this->m_vecVelocity.x = 0.f;
			this->m_vecVelocity.y = 20.f;
		}
	}
	
	//it took me forever to figure this out. never again hopefully ( an hour and a half of messing with 5 lines ).
	RectangleShape head = this->Body.front();
	this->Body.pop_back(); //first tried it with erase but pop_back is much easier to use
	head.move({ this->m_vecVelocity.x, this->m_vecVelocity.y });
	this->Body.push_front(head);
}

void CSnake::Grow()
{
	//i used to call push_front but it would spawn the body part inside the head 
	//so collision detection would complain every time you ate the food. spawning it behind the head fixes it.
	this->Body.emplace(this->Body.begin() + 1); 
}

void CSnake::Eat(CFood& food)
{
	bool x = (this->Body.front().getPosition().x == food.getPosition().x);
	bool y = (this->Body.front().getPosition().y == food.getPosition().y);
	bool bIsColliding = (x && y);
	
	if (bIsColliding)
	{
		food.Spawn(*this);
		this->Grow();
		this->m_iScore++;
	}
}

bool CSnake::Die()
{
	RectangleShape head = this->Body.front();
	bool x = (head.getPosition().x < 0 || head.getPosition().x == g_WindowData.width);
	bool y = (head.getPosition().y < 0 || head.getPosition().y == g_WindowData.height);
	bool bIsOutOfBounds = (x || y);

	bool bIsCollidingWithSelf = false;

	//start the loop from 1 so it doesnt count in the head ( prevents colliding with self all the time ).
	for (UINT i = 1; i < this->Body.size(); i++)
	{
		RectangleShape body = this->Body.at(i);

		if (head.getPosition().x == body.getPosition().x && head.getPosition().y == body.getPosition().y)
			bIsCollidingWithSelf = true;
	}

	return (bIsOutOfBounds || bIsCollidingWithSelf);
}

//you need to overlay the texture because the body technically consists of multiple heads so the 
//the entire body ends up being textured. this might not be the best solution but its the only other one i could think of ( without altering the game logic ).
void CSnake::SetFace(Texture *pFace, RenderWindow* window) 
{
	float angDirection = std::atan2f(this->m_vecVelocity.x, this->m_vecVelocity.y) * -180 / M_PI;
	RectangleShape head = this->Body.front();

	//this looks so demented
	Sprite face;
	face.setTexture(*pFace); //thank you (hip)p0 for texture!
	face.setRotation(angDirection);
	face.setOrigin({ face.getGlobalBounds().width / 2.f, face.getGlobalBounds().height / 2.f});
	face.setPosition({ head.getPosition().x + head.getSize().x / 2.f,  head.getPosition().y + head.getSize().y / 2.f }); //hardcoding :(

	window->draw(face);
}

void CSnake::GameOver(RenderWindow* window, CFood& food)
{
	this->m_bGameOver = this->Die();

	if (this->m_bGameOver)
	{
		Text text;
		text.setFont(g_WindowData.font);
		text.setCharacterSize(28);
		text.setFillColor(Color::White);
		text.setStyle(Text::Regular);
		text.setString("        YOU GOOFED IT!\nPRESS SPACE TO RESTART!"); //lol wtf
		text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);
		text.setPosition(g_WindowData.width / 2.f, g_WindowData.height / 2.f);
		window->draw(text);

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			this->m_bGameOver = false;
			this->Initialize(); //re-initialize the snake
			food.Spawn(*this); //spawn the food in a different location just because.
		}
	}
}

void InitializeSnakeGame(CSnake& snake, CFood& food, RenderWindow* window)
{
	if (!snake.m_SnakeFace.loadFromFile("snak.png"))
	{
		std::cout << "Cant load texture!" << std::endl;
		return;
	}

	snake.Initialize();
	food.Spawn(snake);

	snake.m_ScoreText.setFont(g_WindowData.font);
	snake.m_ScoreText.setCharacterSize(14);
	snake.m_ScoreText.setFillColor(Color::White);
	snake.m_ScoreText.setStyle(Text::Regular);
	snake.m_ScoreText.setPosition(10, 10);
}

void RunSnake(CSnake& snake, CFood& food, RenderWindow* window)
{
	food.Draw(window);
	snake.Think();
	snake.Eat(food);
	snake.Draw(window);
	snake.SetFace(&snake.m_SnakeFace,window);
	snake.GameOver(window, food);
	snake.m_ScoreText.setString("Score:" + std::to_string(snake.m_iScore));
	window->draw(snake.m_ScoreText);
}

