#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>


int main()
{
	std::srand(std::time(nullptr));

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Minecrap - v0.1", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	window.setPosition(sf::Vector2i(0, 0));
	window.setMouseCursorVisible(false);
	

	sf::Texture texture, bg, hammer;
	texture.loadFromFile("Textures/minecrap.png");

	bg.loadFromFile("Textures/fundo.jpg");
	hammer.loadFromFile("Textures/hammer.png");
	sf::Sprite object(texture), background(bg), ham(hammer);

	sf::Font font;
	font.loadFromFile("Fonts/Minecraft.ttf");
	sf::Text score, life, gameover_text, f1;

	bool pressed{ false }, gameOver{false}, isPaused{false};
	int points{}, health{ 5 };
	float velocity = 5.f;

	//F1
	f1.setFont(font);
	f1.setFillColor(sf::Color::Yellow);
	f1.setString("Press F1 to restart game");
	f1.setPosition(400.f, 500.f);

	//Score
	score.setFont(font);
	score.setString("Score: " + std::to_string(points));
	score.setFillColor(sf::Color::White);
	score.setPosition(5.0f, 5.0f);

	//Life
	life.setFont(font);
	life.setString("Life: " + std::to_string(health));
	life.setFillColor(sf::Color::White);
	life.setPosition(1150.f, 5.f);

	//gameover
	gameover_text.setFont(font);
	gameover_text.setString("GAME OVER");
	gameover_text.setFillColor(sf::Color::White);
	gameover_text.setPosition(350.f, 300.f);
	gameover_text.setCharacterSize(100);

	float x = static_cast<float>(std::rand() % static_cast<int>(window.getSize().x - texture.getSize().x - 10));
	object.setPosition(sf::Vector2f(x, 10.f));

	
	std::vector<sf::Sprite> objs;
	size_t max_objs{ 5 };
	float obj_vel_max{ 10.f };
	float obj_vel = obj_vel_max;

	sf::Vector2i pos_mouse_win;
	sf::Vector2f pos_mouse_coord;

	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}

			if (event.type == sf::Event::MouseButtonPressed)
			{
				pressed = false;
			}

			if ((event.type == sf::Event::KeyPressed))
			{
				const sf::Keyboard::Key keycode = event.key.code;
				if (keycode == sf::Keyboard::Escape)
					isPaused = true;
				
			}

			pos_mouse_win = sf::Mouse::getPosition(window);
			pos_mouse_coord = window.mapPixelToCoords(pos_mouse_win);
			ham.setPosition((sf::Vector2f)pos_mouse_win);

		}


		if (gameOver)
		{
			

			window.setMouseCursorVisible(true);
			window.clear();
			window.draw(score);
			window.draw(f1);
			window.draw(gameover_text);
			window.display();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1))
			{
				objs.clear();
				health = 5;
				life.setString("Life: " + std::to_string(health));
				points = 0;
				score.setString("Score: " + std::to_string(points));
				gameOver = false;
				velocity = 5.f;
				window.setMouseCursorVisible(false);
			}

		}
		else
		{
			if (isPaused)
			{
				window.setMouseCursorVisible(true);
				window.clear();
				window.display();
			
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.setMouseCursorVisible(false);
					isPaused = false;
				}
				

			}
			else
			{
				if (objs.size() < max_objs)
				{
					if (obj_vel >= obj_vel_max)
					{
						x = static_cast<float>(std::rand() % static_cast<int>(window.getSize().x - texture.getSize().x - 10));
						object.setPosition(x, 0.f);
						objs.push_back(object);
						obj_vel = 0.f;
					}
					else
					{
						obj_vel += 1.f;
					}
				}

				for (size_t i{}; i < objs.size(); ++i)
				{
					bool del = false;
					objs[i].move(0, velocity);
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
					{
						if (objs[i].getGlobalBounds().contains(pos_mouse_coord))
						{
							del = true;
							points += 10;
							score.setString("Score: " + std::to_string(points));
							pressed = true;

							if (points % 100 == 0)
							{
								velocity += 0.1f;

							}
							if (points % 200 == 0)
							{
								++health;

							}
						}
					}
					if (objs[i].getPosition().y > window.getSize().y)
					{
						--health;
						life.setString("Life: " + std::to_string(health));
						del = true;
						if (health <= 0)
						{
							gameOver = true;
						}
					}
					if (del)
					{
						objs.erase(objs.begin() + i);
					}
				}

				window.clear();
				window.draw(background);
				window.draw(score);
				window.draw(life);

				for (auto& e : objs)
				{
					window.draw(e);
				}
				window.draw(ham);
				window.display();
			}
		}

	
	}

	return EXIT_SUCCESS;
}