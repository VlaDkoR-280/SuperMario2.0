#pragma once

class Mario
{
	bool crouching;
	bool dead;
	bool flipped;
	bool on_ground;

	//Oh, this is a funny story!
	//So I was working on level 1-2 when I discovered a simple bug.
	//The bug was that when 2 Goombas stand SUPER close to each other and Mario jumps on them, one goomba dies and the other kills Mario.
	//This was happening because the first Goomba was setting Mario's vertical speed below 0 after dying.
	//Then the second Goomba checked the collision with Mario and saw that Mario wasn't squishing anymore and instead was going upwards.
	//So he was killing Mario.
	//And by adding this variable, I fixed the bug.
	//Hehe.
	float enemy_bounce_speed;
	float horizontal_speed;
	float vertical_speed;
	float x;
	float y;
	int start_time;
	int last_time;
	

	//The longer we press the jump button, the higher Mario jumps. And we use this variable for that.
	unsigned char jump_timer;
	//I was thinking about adding Fire Mario.
	//But I figured that it's gonna be too much work and I'm too lazy for that.
	//Maybe in the next part?
	unsigned char powerup_state;

	unsigned short death_timer;
	unsigned short growth_timer;
	unsigned short invincible_timer;

	std::vector<Mushroom> mushrooms;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation big_walk_animation;
	Animation walk_animation;
public:
	Mario();

	sf::Font font;
	
	int coins = 0;
	

	sf::Text coinsTextView;

	bool get_dead() const;

	float get_vertical_speed() const;
	float get_x() const;
	float get_y() const;
	int get_coins();
	int get_time_life();
	void start_timer();
	void save_time_as_file();
	void die(const bool i_instant_death);
	void draw(sf::RenderWindow& i_window);
	void draw_mushrooms(const unsigned i_view_x, sf::RenderWindow& i_window);
	void reset();
	void set_position(const float i_x, const float i_y);
	void set_vertical_speed(const float i_value);
	void update(const unsigned i_view_x, MapManager& i_map_manager);

	sf::FloatRect get_hit_box() const;
};