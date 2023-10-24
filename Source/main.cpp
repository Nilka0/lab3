#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"

sf::Sprite sprite—rimson;
sf::Texture spriteTexture—rimson;
sf::Sprite spriteYellow;
sf::Texture spriteTextureYellow;
sf::Sprite spritePurple;
sf::Texture spriteTexturePurple;

sf::Color bgColor(sf::Color::White);
void HandleUserInput(sf::RenderWindow& window, const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
		window.close();
		break;
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			if (sprite—rimson.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
			{
				bgColor = sf::Color(245, 0, 88);
			}
			if (spriteYellow.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
			{
				bgColor = sf::Color(224, 188, 0);
			}
			if (spritePurple.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
			{
				bgColor = sf::Color(130, 17, 244);
			}
		}
		break;
	default:
		break;
	}
}

int main()
{
	

	sf::RenderWindow window(sf::VideoMode(800, 800), "Geometry modeling 1");
	window.setFramerateLimit(60);

	spriteTexture—rimson.loadFromFile("./Source/button/button1.png");
	sprite—rimson.setTexture(spriteTexture—rimson);
	sprite—rimson.setTextureRect(sf::IntRect(0, 0, 214, 94));
	sprite—rimson.setPosition(60, 60);

	
	spriteTextureYellow.loadFromFile("./Source/button/button2.png");
	spriteYellow.setTexture(spriteTextureYellow);
	spriteYellow.setTextureRect(sf::IntRect(0, 0, 166, 94));
	spriteYellow.setPosition(314,60);

	
	spriteTexturePurple.loadFromFile("./Source/button/button3.png");
	spritePurple.setTexture(spriteTexturePurple);
	spritePurple.setTextureRect(sf::IntRect(0,0,224,94));
	spritePurple.setPosition(520, 60);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			HandleUserInput(window, event);
		}
		window.clear(bgColor);
		window.draw(spritePurple);
		window.draw(sprite—rimson);
		window.draw(spriteYellow);
		window.display();
	}

	return 0;
}
