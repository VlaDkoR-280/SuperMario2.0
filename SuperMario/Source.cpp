#include <array>
#include <chrono>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



#include "Animation.h"
#include "Global.h"
#include "MapManager.h"
#include "Mushroom.h"
#include "Mario.h"
#include "Enemy.h"
#include "Goomba.h"
#include "Koopa.h"
#include "ConvertSketch.h"
#include <fstream>
#include <iostream>



void update_data(int* best_time) {
	std::ifstream fin("records.txt");
	if (fin.is_open()) {
		fin >> *best_time;
		fin.close();
	}
	else {
		std::ofstream fout("records.txt");
		fout << -1;
		*best_time = -1;
		fout.close();
	}
}

void setMenuButtonsColor(sf::Text* start, sf::Text* result, sf::Text* exit, int state_menu) {
	switch (state_menu) {
	case 0:
	{
		(*start).setOutlineColor(sf::Color::Red);
		(*result).setOutlineColor(sf::Color::Black);
		(*exit).setOutlineColor(sf::Color::Black);
		break;
	}
	case 1:
	{
		(*result).setOutlineColor(sf::Color::Red);
		(*start).setOutlineColor(sf::Color::Black);
		(*exit).setOutlineColor(sf::Color::Black);
		break;
	}
	case 2:
	{
		(*exit).setOutlineColor(sf::Color::Red);
		(*result).setOutlineColor(sf::Color::Black);
		(*start).setOutlineColor(sf::Color::Black);
		break;
	}
	}
}

int main()
{
	bool win = false;
	unsigned char current_level = 0;

	unsigned short level_finish = 0;

	int best_time;
	update_data(&best_time);
	sf::Font font;
	font.loadFromFile("C:\\Users\\vladk\\Desktop\\PakenhamBlItalic.ttf");
	sf::Text myTextView("COINS: 0\tTime: 0s\tBest Time: None", font, 20);
	sf::Text winText("YOU WIN!!!", font, 70);
	myTextView.setOutlineThickness(2);
	winText.setOutlineColor(sf::Color::Black);
	winText.setOutlineThickness(3);
	winText.setFillColor(sf::Color::Yellow);

	sf::Text startText("START", font, 30), resultsText("RESULTS", font, 30), exitText("EXIT", font, 30);
	sf::Text labelText("MARIO", font, 90);
	sf::Text bestRecordText("Best Time: ", font, 40);
	startText.setOutlineColor(sf::Color::Red);
	startText.setOutlineThickness(3);
	startText.setPosition(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 60);

	resultsText.setOutlineColor(sf::Color::Black);
	resultsText.setOutlineThickness(3);
	resultsText.setPosition(SCREEN_WIDTH - 55, SCREEN_HEIGHT);

	exitText.setOutlineColor(sf::Color::Black);
	exitText.setOutlineThickness(3);
	exitText.setPosition(SCREEN_WIDTH - 30, SCREEN_HEIGHT + 60);

	labelText.setFillColor(sf::Color::Red);
	labelText.setOutlineColor(sf::Color::Black);
	labelText.setOutlineThickness(5);
	labelText.setPosition(SCREEN_WIDTH - 115, 10);

	bestRecordText.setOutlineColor(sf::Color::Black);
	bestRecordText.setOutlineThickness(4);
	bestRecordText.setPosition(SCREEN_WIDTH - 120, SCREEN_HEIGHT - 60);

	int state = 0; //0 - menu, 1 - rusults, 2 - game
	int state_menu = 0; // 0 - start, 1 - results, 2 - exit (ActiveButton)


	sf::Texture textureBack;
	textureBack.loadFromFile("C:\\Users\\vladk\\Desktop\\backg.jpg");
	sf::Sprite spriteBack(textureBack);
	spriteBack.setPosition(-70, 0);
	spriteBack.setScale(0.6, 0.6);

	sf::Music music;
	
	if (!music.openFromFile("C:\\Users\\vladk\\Desktop\\music.ogg")) return 1;
	music.setLoop(true);
	
	//We'll use this to make the game framerate-independent.
	std::chrono::microseconds lag(0);

	std::chrono::steady_clock::time_point previous_time;

	//Using smart pointer because I'm smart.
	//(Because we need to store both Goomba and Koopa objects in the same vector).
	std::vector<std::shared_ptr<Enemy>> enemies;

	sf::Color background_color = sf::Color(0, 219, 255);

	sf::Event event;

	sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Super Mario Bros", sf::Style::Close);
	window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));

	sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

	MapManager map_manager;

	Mario mario;

	convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);

	previous_time = std::chrono::steady_clock::now();
	
	music.play();
	
	while (window.isOpen())
	{
		std::cout << music.getPosition().x << " " << music.getPosition().y << " " << music.getPosition().z << " " << std::endl;
		if (state == 2) {
			std::chrono::microseconds delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

			lag += delta_time;

			previous_time += delta_time;

			//Soooooo, how was your day?
			//Mine was good. I ate some cookies.
			//Then I watched Youtube.
			//...
			//...
			//...
			//Yeah.

			while (FRAME_DURATION <= lag)
			{
				unsigned view_x;

				lag -= FRAME_DURATION;

				while (1 == window.pollEvent(event))
				{
					switch (event.type)
					{
					case sf::Event::Closed:
					{
						window.close();

						break;
					}
					case sf::Event::KeyPressed:
					{
						switch (event.key.code)
						{
						case sf::Keyboard::Enter:
						{
							myTextView.setFillColor(sf::Color::White);
							win = false;
							mario.start_timer();
							current_level = 0;
							enemies.clear();
							mario.reset();
							update_data(&best_time);
							convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);
						} break;
						case sf::Keyboard::Escape:
						{
							myTextView.setFillColor(sf::Color::White);
							win = false;
							mario.start_timer();
							current_level = 0;
							enemies.clear();
							mario.reset();
							update_data(&best_time);
							convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);
							state = 0;
							continue;
						}
						}
					}
					}
				}

				//Once Mario goes beyond the finish, we move on to the next level.
				if (CELL_SIZE * level_finish <= mario.get_x())
				{
					current_level++;

					enemies.clear();

					mario.reset();

					convert_sketch(current_level, level_finish, enemies, background_color, map_manager, mario);
				}


				//Keeping Mario at the center of the view.
				view_x = std::clamp<int>(round(mario.get_x()) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * map_manager.get_map_width() - SCREEN_WIDTH);

				map_manager.update();

				mario.update(view_x, map_manager);

				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					enemies[a]->update(view_x, enemies, map_manager, mario);
				}

				for (unsigned short a = 0; a < enemies.size(); a++)
				{
					if (1 == enemies[a]->get_dead(1))
					{
						//We don't have to worry about memory leaks since we're using SMART POINTERS!
						enemies.erase(a + enemies.begin());

						a--;
					}
				}

				if (FRAME_DURATION > lag)
				{

					view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
					window.setView(view);
					window.clear(background_color);



					//If the background color is sf::Color(0, 0, 85), the level is underground.
					map_manager.draw_map(1, sf::Color(0, 0, 85) == background_color, view_x, window);

					mario.draw_mushrooms(view_x, window);

					map_manager.draw_map(0, sf::Color(0, 0, 85) == background_color, view_x, window);

					for (unsigned short a = 0; a < enemies.size(); a++)
					{
						enemies[a]->draw(view_x, window);
					}

					myTextView.setString("COINS: " + std::to_string(mario.get_coins()) + "\tTime: " + std::to_string(mario.get_time_life()) + ((best_time != -1) ? "\tBest Time: " + std::to_string(best_time) : ""));

					myTextView.setPosition(view_x, 0);

					window.draw(myTextView);
					mario.draw(window);

					if (current_level == 1) {
						if (mario.get_x() >= 2860 && mario.get_x() <= 2862 && mario.get_y() >= 100) {
							win = true;
							mario.save_time_as_file();
							update_data(&best_time);
						}
						if (win) {
							winText.setPosition(view_x, SCREEN_HEIGHT / 2 - 70);
							window.draw(winText);

							myTextView.setFillColor(sf::Color::Yellow);
						}
					}


					window.display();
				}
			}
		}
		else if (state == 0) {

			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					window.close();

					break;
				}
				case sf::Event::KeyPressed:
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Enter:
					{
						switch (state_menu)
						{
						case 0:
						{
							state = 2;
							break;
						}
						case 1:
						{
							state = 1;
							break;
						}
						case 2:
						{
							music.stop();
							window.close();
							break;
						}


						} break;
					case sf::Keyboard::Escape:
					{
						music.stop();
						window.close();

						break;
					}
					case sf::Keyboard::Down:
					{

						state_menu = (state_menu < 2) ? state_menu + 1 : state_menu;
						setMenuButtonsColor(&startText, &resultsText, &exitText, state_menu);
						break;
					}
					case sf::Keyboard::Up:
					{
						state_menu = (state_menu > 0) ? state_menu - 1 : state_menu;
						setMenuButtonsColor(&startText, &resultsText, &exitText, state_menu);
						break;
					}
					}
					}
				}
				}
			}

			
			window.setView(window.getDefaultView());
			window.draw(spriteBack);
			
			window.draw(labelText);
			window.draw(startText);
			window.draw(resultsText);
			window.draw(exitText);
			
			window.display();
		}
		else if (state == 1) {
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed:
				{
					music.stop();
					window.close();
					break;
				}
				case sf::Event::KeyPressed:
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Escape:
					{
						state = 0;

						break;
					}
					}
					break;
				}
				}
			}


			update_data(&best_time);
			if (best_time != -1) {
				bestRecordText.setString("Best Time: " + std::to_string(best_time));
			}
			else {
				bestRecordText.setString("Best Time: None\nYou have to win!");
			}

			
			window.draw(spriteBack);
			window.draw(labelText);
			window.draw(bestRecordText);
			window.display();
		}




	}
}