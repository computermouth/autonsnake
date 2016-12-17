#include <Game/Player.hpp>
#include <Game/Board.hpp>

Player::Player(int x, int y):
	alive(true),
	currentDirection(Player::RIGHT),
	nextDirection(Player::RIGHT)
{
	// The Snake's head
	this->body.push_back(Body(x, y));

	// Initializing the snake with 2 body pieces
	// (plus the head)
	this->body.push_back(Body(x - 1, y));
	this->body.push_back(Body(x - 1, y));
}
bool Player::isAlive()
{
	return (this->alive);
}
int Player::getSize()
{
	return (int)(this->body.size());
}
int Player::getX()
{
	return (this->body.front().x);
}
int Player::getY()
{
	return (this->body.front().y);
}
void Player::moveTo(int x, int y)
{
	this->body.front().x = x;
	this->body.front().y = y;
}
void Player::move(Direction dir)
{
	this->nextDirection = dir;
}
void Player::kill()
{
	this->alive = false;
}
void Player::update(Board* board)
{
	// We have to make sure the snake doesn't do strange
	// things, like turning around on itself.
	switch(this->nextDirection)
	{
	case Player::RIGHT:
		if (this->currentDirection != Player::LEFT)
			this->currentDirection = this->nextDirection;
		break;

	case Player::LEFT:
		if (this->currentDirection != Player::RIGHT)
			this->currentDirection = this->nextDirection;
		break;

	case Player::UP:
		if (this->currentDirection != Player::DOWN)
			this->currentDirection = this->nextDirection;
		break;

	case Player::DOWN:
		if (this->currentDirection != Player::UP)
			this->currentDirection = this->nextDirection;
		break;
	};

	// Retain snake's last horizontal direction
	static Player::Direction previousDirection = this->currentDirection;
	
	// Get position in map
	int x = this->Player::getX();
	int y = this->Player::getY();

	if(x == 1 && y == 2){
		
		this->nextDirection = Player::RIGHT; // turns right at top left
		previousDirection = Player::RIGHT;   // update previous h-dir
		
	}else if(x == 76 && y == 2){
		
		this->nextDirection = Player::LEFT; // turns left at top right
		previousDirection = Player::LEFT;
		
	}else if(x == 2 && y == 19 && this->currentDirection == Player::LEFT){
		
		this->nextDirection = Player::UP; // turns up at bottom left
		
	}else if(x == 75 && y == 19 && this->currentDirection == Player::RIGHT){
		
		this->nextDirection = Player::UP; // turns up at bottom right
		
	}else if(this->currentDirection == Player::DOWN && 
		previousDirection == Player::RIGHT){
			
		this->nextDirection = Player::LEFT; // if snake was heading right and turned down
		previousDirection = Player::LEFT;   // go left now
	}else if(this->currentDirection == Player::DOWN && 
		previousDirection == Player::LEFT){
			
		this->nextDirection = Player::RIGHT; // if snake was heading left and turned down
		previousDirection = Player::RIGHT;   // go right now
	}else if((x == 3 && this->currentDirection == Player::LEFT && y != 19)||
		(x == 74 && this->currentDirection == Player::RIGHT && y != 19)){
		
		this->nextDirection = Player::DOWN;  // move down if you hit an edge horizontally
	}

	// Making the rest of the body catch up
	for (unsigned int i = (this->body.size() - 1); i > 0; i--)
	{
		this->body[i].x = this->body[i - 1].x;
		this->body[i].y = this->body[i - 1].y;
	}

	// Moving the head
	switch(this->currentDirection)
	{
	case Player::RIGHT:
		this->body.front().x++;
		break;

	case Player::LEFT:
		this->body.front().x--;
		break;
	case Player::UP:
		this->body.front().y--;
		break;

	case Player::DOWN:
		this->body.front().y++;
		break;
	}

	// Checking if the player hit the board's extremes.
	//
	// @warning `teleport` may change player's X and Y!
	if (board->isBorder(this->body.front().x,
	                    this->body.front().y))
	{
		if (board->style == Board::TELEPORT)
			board->teleport(this);
		else
			this->kill();
	}

	int headx = this->body.front().x;
	int heady = this->body.front().y;

	// Checking if the head hits the body
	if (this->bodyHit(headx, heady, true))
		this->kill();

	// Checking for collisions on the board
	if (board->isWall(headx, heady))
		this->kill();
}
void Player::draw(Window* win)
{
	// The body
	for (unsigned int i = 1; i < (this->body.size()); i++)
		win->printChar('o',
		               this->body[i].x,
		               this->body[i].y,
		               Colors::pair(COLOR_GREEN, COLOR_DEFAULT, true));

	// The head
	win->printChar(((this->alive) ?
	                '@' :
	                'X'),
	               this->body.front().x,
	               this->body.front().y,
	               Colors::pair(((this->alive) ?
	                             COLOR_GREEN :
	                             COLOR_RED), COLOR_DEFAULT, true));
}
bool Player::headHit(int x, int y)
{
	return ((this->body.front().x == x) &&
	        (this->body.front().y == y));
}
bool Player::bodyHit(int x, int y, bool isCheckingHead)
{
	int initial = 0;
	if (isCheckingHead) initial = 3;

	for (unsigned int i = initial; i < (this->body.size()); i++)
		if ((this->body[i].x == x) &&
		    (this->body[i].y == y))
			return true;

	return false;
}
void Player::increase()
{
	int lastx = this->body.back().x;
	int lasty = this->body.back().y;

	this->body.push_back(Body(lastx, lasty));
}

