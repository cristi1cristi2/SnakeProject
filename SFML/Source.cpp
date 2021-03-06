#include <SFML/Graphics.hpp>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>


int path[1000][1000], graph[1000][1000];

int dx[4] = { 1, 0, -1, 0 };
int dy[4] = { 0, 1, 0, -1 };

struct point
{
	int x = 0;
	int y = 0;

} p[1000];

struct pair
{
	int first = 0;
	int second = 0;

} initial;

bool mySol(int n, int graph[][1000], int cur1, int cur2)
{
	if (cur1 != initial.first || cur2 != initial.second)
		return false;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (graph[i][j] == 0)
				return false;
		}
	}
	return true;
}

bool myVerifi(int n, int graph[][1000], int cur1, int cur2)
{
	return 0 <= cur1 && cur1 < n && 0 <= cur2 && cur2 < n&& graph[cur1][cur2] != 1;
}

bool cyclu(int n, int graph[][1000], int path[][1000], int cur1, int cur2)
{//the algorithm for computing the hamiltonian cycle
	for (int i = 0; i < 4; i++)
	{
		int dirx = cur1 + dx[i], diry = cur2 + dy[i];
		if (mySol(n, graph, dirx, diry))
		{
			for (int index = 0; index < n; index++)
			{
				for (int j = 0; j < n; j++)
					printf("%d ", path[index][j]);
				printf("  ");
				for (int j = 0; j < n; j++)
					printf("%d ", graph[index][j]);

				printf("\n");
			}
			printf("\n a\n");
			srand(time(NULL));
			if (rand() % 3 == 1)
				return true;
			else return false;
		}

		if (myVerifi(n, graph, dirx, diry))
		{
			graph[dirx][diry] = 1;
			path[dirx][diry] = path[cur1][cur2] + 1;
			if (cyclu(n, graph, path, dirx, diry)) {
				return true;

			}
			graph[dirx][diry] = 0;
			path[dirx][diry] = 0;
		}
	}
	return false;
}


int game = 1, dir = 0, size = 1, speed = 30;
bool retry = false;
int randomx = 0, randomy = 0;
bool good = true, human = false, greedy = false, hamiltonian = true, noPath = true, chosen = false;
int windowsize = 400, n = windowsize / 50;
void main()
{
	//declaring and initializing the window, the player, and the food

	sf::RenderWindow window(sf::VideoMode(windowsize, windowsize), "Ssnek", sf::Style::Close);
	sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
	sf::RectangleShape head(sf::Vector2f(30.f, 30.f));
	sf::RectangleShape food(sf::Vector2f(50.f, 50.f));
	head.setFillColor(sf::Color::Black);
	food.setFillColor(sf::Color::Red);
	player.setPosition(0, 0);
	srand(time(NULL));

	//the next bit chooses a random position for the food

	randomx = 1 + rand() % (windowsize - 50);
	randomy = 1 + rand() % (windowsize - 50);
	for (int i = 0; i < n; i++)
	{
		if (randomx <= (1 + i) * 50 && randomx > i * 50) randomx = (i + 1) * 50;
		if (randomy <= (1 + i) * 50 && randomy > i * 50) randomy = (i + 1) * 50;
	}
	//this next part will choose a direction for the snake to go in based on the strategy
	//the direction will then be used to update the snakes' location
	//game=1 means the game is playable, and snake can still move, after the snake bumps into itself or a wall game=2 and at the end it will initialize the game again
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == event.Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Numpad1) {
					human = true;
					greedy = false;
					hamiltonian = false;
					speed += 50;
				}
				if (event.key.code == sf::Keyboard::Numpad2) {
					human = false;
					greedy = true;
					hamiltonian = false;
				}
				if (event.key.code == sf::Keyboard::Numpad3) {
					human = false;
					greedy = false;
					hamiltonian = true;
				}
				if (game == 1 && human == true)
				{
					if (event.key.code == sf::Keyboard::Right && dir != 1 && chosen==false) { dir = 0; chosen = true; }//right
					else if (event.key.code == sf::Keyboard::Left && dir != 0 && chosen == false) { dir = 1; chosen = true; }//left
					else if (event.key.code == sf::Keyboard::Down && dir != 3 && chosen == false) { dir = 2; chosen = true; }//down
					else if (event.key.code == sf::Keyboard::Up && dir != 2 && chosen == false) { dir = 3; chosen = true; }//up
				}
				else if (game == 2)
				{//retry if r is pressed
					if (event.key.code == sf::Keyboard::R && game == 2) retry = true;
				}
			}

		}
		if (game == 2)
		{//automatic reinitialize after the snake dies, comment this if you want retry from r key.
			retry = true;
		}
		if (game == 1 && greedy == true)  //greedy search, checks only the next squares
		{
			int x = abs(p[0].x - food.getPosition().x);
			int y = abs(p[0].y - food.getPosition().y);
			bool u = true, d = true, l = true, r = true;
			for (int i = 1; i < size; i++)//this bit checks if there is no snake body in the up, down,  left, right position and marks them
			{
				if (p[i].x == (p[0].x + 50) && p[i].y == p[0].y)
				{
					r = false;
				}
				if (p[i].x == (p[0].x - 50) && p[i].y == p[0].y)
				{
					l = false;
				}
				if (p[i].y == (p[0].y + 50) && p[i].x == p[0].x)
				{
					d = false;
				}
				if (p[i].y == (p[0].y - 50) && p[i].x == p[0].x)
				{
					u = false;
				}
			}
			printf("left %d right %d  up %d down %d\n", l, r, u, d);
			//printf("%d %d\n", x, y);

			//these next 4 ifs check wheter choosing a direction gets the snake closer to the food
			if ((abs(p[0].x + 50 - food.getPosition().x) < x) && dir != 1 && r)
			{
				dir = 0;

			}
			else

				if ((abs(p[0].x - 50 - food.getPosition().x) < x) && dir != 0 && l)
				{
					dir = 1;

				}
				else

					if ((abs(p[0].y + 50 - food.getPosition().y) < y) && dir != 3 && d)
					{
						dir = 2;

					}
					else

						if ((abs(p[0].y - 50 - food.getPosition().y) < y) && dir != 2 && u)
						{
							dir = 3;

						}//if there is no position available that gets the snake closer to food, he will choose the first one which keeps it alive
						else if (r && p[0].x + 50 < windowsize)
						{
							dir = 0;
						}
						else if (l && p[0].x - 50 > 0)
						{
							dir = 1;
						}
						else if (d && p[0].y + 50 < windowsize)
						{
							dir = 2;
						}
						else if (u && p[0].y - 50 > 0)
						{
							dir = 3;
						}
						else//dont  know if i need these 4 last ifs, but these say if u can only go up go up!!!
						{
							if (l == false && r == true && u == false && d == false)
							{
								dir = 0;
							}
							if (l == true && r == false && u == false && d == false)
							{
								dir = 1;
							}
							if (l == false && r == false && u == false && d == true)
							{
								dir = 2;
							}
							if (l == false && r == false && u == true && d == false)
							{
								dir = 3;
							}
						}
			printf("dir: %d\n", dir);
		}
		else if (game == 1 && hamiltonian == true)//here we follow the path of the hamiltonian cycle
		{
			int x = p[0].y / 50, y = p[0].x / 50;
			if (noPath == true)
			{
				for (int i = 0; i < n; i++)
					for (int j = 0; j < n; j++)
					{
						graph[i][j] = 0;
						path[i][j] = 0;
					}

				graph[x][y] = 1;
				cyclu(n, graph, path, x, y);
				noPath = false;
			}

			if (y + 1 < n)
			{

				if (path[x][y + 1] - path[x][y] == 1 || (path[x][y + 1] == 0 && dir != 1))
				{

					dir = 0;
				}
			}
			if (y - 1 > -1)
			{

				if (path[x][y - 1] - path[x][y] == 1 || (path[x][y - 1] == 0 && dir != 0))
				{

					dir = 1;
				}
			}
			if (x + 1 < n)
			{

				if (path[x + 1][y] - path[x][y] == 1 || (path[x + 1][y] == 0 && dir != 3))
				{

					dir = 2;
				}
			}
			if (x - 1 > -1)
			{

				if (path[x - 1][y] - path[x][y] == 1 || (path[x - 1][y] == 0 && dir != 2))
				{

					dir = 3;
				}
			}
			printf("dir: %d\n", dir);
			printf("coords: %d %d\n", x, y);

		}

		//change the color of the snake by lmb or rmb
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))//click pe mouse pt culori
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			if (0 < mousePos.x && mousePos.x < window.getSize().x && 0 < mousePos.y && mousePos.y < window.getSize().y)
			{
				printf("clicked at (%d, %d)\n", mousePos.x, mousePos.y);
				player.setFillColor(sf::Color::Magenta);
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))//click pe mouse pt culori
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			if (0 < mousePos.x && mousePos.x < window.getSize().x && 0 < mousePos.y && mousePos.y < window.getSize().y)
			{
				printf("Right-clicked at (%d, %d)\n", mousePos.x, mousePos.y);
				player.setFillColor(sf::Color::Cyan);
			}
		}
		window.clear();
		//we clear the window, update the snake position based on direction, choose new position for food if needed and paint the screen again
		if (game == 1)
		{
			for (int i = size; i > 0; i--)
			{
				p[i].x = p[i - 1].x;
				p[i].y = p[i - 1].y;
			}

			if (dir == 0)
				p[0].x += 50;

			if (dir == 1)
				p[0].x -= 50;

			if (dir == 2)
				p[0].y += 50;

			if (dir == 3)
				p[0].y -= 50;

			if (p[0].x == -50 || p[0].y == -50 || p[0].x == windowsize || p[0].y == windowsize) game = 2;

			for (int i = 1; i < size; i++)
			{
				if (p[0].x == p[i].x && p[0].y == p[i].y)
				{
					game = 2;
				}
			}
			if (p[0].x == food.getPosition().x && p[0].y == food.getPosition().y)
			{
				size += 1;
				if (speed > 100)speed -= 20;
				good = true;
				int bothx = 0, bothy = 0;
				while (good && size != n * n + 1)
				{
					randomx = 1 + rand() % (windowsize - 50);
					randomy = 1 + rand() % (windowsize - 50);
					printf("random: %d %d\n", randomx, randomy);

					for (int i = 0; i < n; i++)
					{
						if (randomx <= (1 + i) * 50 && randomx > i * 50)
							if (bothx == 0) {
								randomx = (1 + i) * 50;
								bothx = 1;
							}
							else {
								randomx = i * 50;
								bothx = 0;
							}
						if (randomy <= (1 + i) * 50 && randomy > i * 50)
							if (bothy == 0) {
								randomy = (1 + i) * 50;
								bothy = 1;
							}
							else {
								randomy = i * 50;
								bothy = 0;
							}
					}


					for (int i = 0; i < size; i++)
					{
						if (randomx == p[i].x && randomy == p[i].y)
							break;
						else if ((randomx != p[i].x || randomx != p[i].x) && i == size - 1)
							good = false;
					}

					if (size > 0.8 * n * n) { //last few foods are placed directly in front of the snake, it might use some more randomizing

						printf("bucla! %d\n", size);

						for (int x = 0; x < windowsize; x += 50)
							for (int y = 0; y < windowsize; y += 50) {
								int ok = 0;
								for (int i = 0; i < size; i++)
									if (x == p[i].x && y == p[i].y) {
										ok = 1;
										break;
									}
								if (ok == 0) {
									randomx = x;
									randomy = y;
									good = false;
									break;
								}
							}
						if (size == n * n)
							good = false;
					}
				}

			}

			food.setPosition(randomx, randomy);
			window.draw(food);
			for (int i = 0; i < size; i++)
			{
				player.setPosition(p[i].x, p[i].y);
				window.draw(player);
			}
			head.setPosition(p[0].x + 10, p[0].y + 10);
			window.draw(head);
			window.display();
			int foodx = food.getPosition().x;
			int foody = food.getPosition().y;
			printf(" player: %d %d\n", foodx, foody, p[0].x / 50, p[0].y / 50);
			chosen = false;
			Sleep(speed);
		}
		else if (game == 2)
		{
			/// <summary>
			/// so here we initialize the game after the snake died
			/// we set size to 1 and the position of the snake to 0,0
			/// then we choose a random position for the food like in the begining of the code
			/// we make game = 1 and that s it snake goes right back at it
			/// </summary>
			if (retry == true)
			{

				size = 1;
				p[0].x = 0;
				p[0].y = 0;
				player.setPosition(0, 0);
				head.setPosition(10, 10);
				retry = false;
				noPath = true;
				randomx = 1 + rand() % (windowsize - 50);
				randomy = 1 + rand() % (windowsize - 50);
				for (int i = 0; i < n; i++)
				{
					if (randomx <= (1 + i) * 50 && randomx > i * 50) randomx = (i + 1) * 50;
					if (randomy <= (1 + i) * 50 && randomy > i * 50) randomy = (i + 1) * 50;

					game = 1;
					dir = 0;
					speed = 30;
				}
			}
		}

	}

}
