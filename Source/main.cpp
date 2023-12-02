#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"
#include "cmath"
#include "functional"
#include "imgui-SFML.h"
#include "imgui.h"
#include "string"

using namespace sf;

static int colour[3] = {255, 255, 255};
class RFuncSprite : public Sprite
{
public:
	std::vector<std::vector<float>> AU;
	std::vector<std::vector<float>> BU;

	void Create(const Vector2u &size)
	{
		_AUALL.create(size.x, size.y);
		_texturen1.loadFromImage(_AUALL);
		_GradientImage.create(_AUALL.getSize().x, _AUALL.getSize().y, Color::Transparent);
		AU.resize(size.x, std::vector<float>(size.y));
		BU.resize(size.x, std::vector<float>(size.y));

		_AUALL.create(size.x, size.y, Color::White);
		_BUALL.create(size.x, size.y, Color::White);
		_CUALL.create(size.x, size.y, Color::White);
		_DUALL.create(size.x, size.y, Color::White);
		_allImage.create(_AUALL.getSize().x * 2, _AUALL.getSize().y * 2, Color::White);
	}
	void Paintpixel(sf::Image &image, int x, int y, float n, sf::Uint8 r, sf::Uint8 g, sf::Uint8 b)
	{
		sf::Color pixelColor;
		pixelColor.r = static_cast<sf::Uint8>(((r) * (n + 1.f)) / 2.f);
		pixelColor.g = static_cast<sf::Uint8>(((g) * (n + 1.f)) / 2.f);
		pixelColor.b = static_cast<sf::Uint8>(((b) * (n + 1.f)) / 2.f);
		image.setPixel(x, y, pixelColor);
	}

	void DrawRFunc(const std::function<float(const sf::Vector2f &)> &rfunc, const sf::FloatRect &subSpace, int r, int g,
				   int b)
	{
		for (unsigned x = 0; x < _AUALL.getSize().x; ++x)
		{
			for (unsigned y = 0; y < _AUALL.getSize().y; ++y)
			{
				Vector2f spaceStep = {
					subSpace.width / static_cast<float>(_AUALL.getSize().x),
					subSpace.height / static_cast<float>(_AUALL.getSize().y),
				};
				Vector2f spacePoint = {
					subSpace.left + static_cast<float>(x) * spaceStep.x,
					subSpace.top + static_cast<float>(y) * spaceStep.y,
				};

				const sf::Vector2f spacePoint1 = {spacePoint.x + spaceStep.x, spacePoint.y};
				const sf::Vector2f spacePoint2 = {spacePoint.x, spacePoint.y + spaceStep.y};

				Vector3f v1 = {spacePoint.x, spacePoint.y, rfunc(spacePoint)};

				Vector3f v2 = {spacePoint1.x, spacePoint1.y, rfunc(spacePoint1)};

				Vector3f v3 = {spacePoint2.x, spacePoint2.y, rfunc(spacePoint2)};

				float A = v1.y * (v2.z - v3.z) - v2.y * (v1.z - v3.z) + v3.y * (v1.z - v2.z);
				float B = -(v1.x * (v2.z - v3.z) - v2.x * (v1.z - v3.z) + v3.x * (v1.z - v2.z));
				float C = v1.x * (v2.y - v3.y) - v2.x * (v1.y - v3.y) + v3.x * (v1.y - v2.y);
				float D = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v2.x * (v1.y * v3.z - v3.y * v1.z) +
							v3.x * (v1.y * v2.z - v2.y * v1.z));

				float auall = A / (sqrt(A * A + B * B + C * C + D * D));
				float buall = B / (sqrt(A * A + B * B + C * C + D * D));
				float cuall = C / (sqrt(A * A + B * B + C * C + D * D));
				float duall = D / (sqrt(A * A + B * B + C * C + D * D));
				AU[x][y] = auall;
				BU[x][y] = buall;

				Paintpixel(_AUALL, x, y, auall, r, g, b);
			}
		}
		_texturen1.loadFromImage(_AUALL);
		_texturen2.loadFromImage(_BUALL);
		_texturen3.loadFromImage(_CUALL);
		_texturen4.loadFromImage(_DUALL);
	}
	void setauall() { setTexture(_texturen1); }
	void SetGradient() { setTexture(_GradientTexture); }
	
	void CombineImages()
	{
		_allImage.copy(_AUALL, 0, 0);
		_allImage.copy(_BUALL, _AUALL.getSize().x, 0);
		_allImage.copy(_CUALL, 0, _AUALL.getSize().y);
		_allImage.copy(_DUALL, _AUALL.getSize().x, _AUALL.getSize().y);
		_textureAll.loadFromImage(_allImage);
	}
	void Save()
	{
		_AUALL.saveToFile("./Source/n1.jpg");
		_BUALL.saveToFile("./Source/n2.jpg");
		_CUALL.saveToFile("./Source/n3.jpg");
		_DUALL.saveToFile("./Source/n4.jpg");
	}
	void SetTextureAll() { setTexture(_textureAll); }
	float getMvalue(unsigned int x, unsigned int y, int n)
	{
		if (n == 1 && x < AU.size() && y < AU[x].size())
		{
			return AU[x][y];
		}
		else if (n == 2 && x < BU.size() && y < BU[x].size())
		{
			return BU[x][y];
		}
		return 0.0f;
	}

	
	void findGradient(float startX, float startY, float step)
	{
		sf::Color pixelColor;
		pixelColor.r = 123;
		pixelColor.g = 56;
		pixelColor.b = 205;
		float x = startX;
		float y = startY;
		while (true)
		{
			if (x < 0 || y < 0 || x >= _AUALL.getSize().x || y >= _AUALL.getSize().y)
			{
				break;
			}
			_GradientImage.setPixel(x, y, pixelColor);
			float currentCx = getMvalue((int)x, (int)y, 1);
			float currentCy = getMvalue((int)x, (int)y, 2);
			x += (step * currentCx);
			y += (step * currentCy);
		}
		_GradientTexture.loadFromImage(_GradientImage);
	}

private:

	Texture _GradientTexture;
	Image _GradientImage;
	Texture _texturen1;
	Texture _texturen2;
	Texture _texturen3;
	Texture _texturen4;
	Image _allImage;
	Texture _textureAll;
	Image _AUALL;
	Image _BUALL;
	Image _CUALL;
	Image _DUALL;
};

float RAnd(float w1, float w2) { return w1 + w2 - sqrt(w1 * w1 + w2 * w2); }
float ROr(float w1, float w2) { return w1 + w2 + sqrt(w1 * w1 + w2 * w2); }
float head(const sf::Vector2f &point) { return 1 - powf(point.x, 2) / 16 - powf(point.y + 3, 2) / 9; }
float body(const sf::Vector2f &point) { return 1 - (point.x * point.x) / 9 - powf(point.y - 4, 2) / 25; }
float tail(const sf::Vector2f &point) { return 1 - powf(point.x + 5, 2) / 1 - powf(point.y - 7.5, 2) / 0.25; }
float w1(const sf::Vector2f &point) { return 1 - (point.x - 3) / -2 - (point.y + 6) / 3; }
float w2(const sf::Vector2f &point) { return 1 - (point.x - 3) / -2 - (point.y + 6) / -1; }
float w3(const sf::Vector2f &point) { return 1 - (point.x - 3) / 0.5 - (point.y + 6) / 3; }
float w4(const sf::Vector2f &point) { return 1 - (point.x + 3) / 2 - (point.y + 6) / 3; }
float w5(const sf::Vector2f &point) { return 1 - (point.x + 3) / 2 - (point.y + 6) / -1; }
float w6(const sf::Vector2f &point) { return 1 - (point.x + 3) / -0.5 - (point.y + 3) / 3; }
float leg1(const sf::Vector2f &point) { return 1 - powf(point.x + 1.5, 2) / 0.25 - powf(point.y - 6.5, 2) / 4; }
float leg2(const sf::Vector2f &point) { return 1 - powf(point.x - 1.5, 2) / 0.25 - powf(point.y - 6.5, 2) /4; }
float WP (const sf::Vector2f &point) { return RAnd(w1(point), RAnd(w2(point), w3(point))); }
float WL (const sf::Vector2f &point) { return RAnd(w4(point), RAnd(w5(point), w6(point))); }

float myFunc(const sf::Vector2f &point)
{
	return ROr(head (point), ROr (WP (point), ROr (WL (point), ROr (tail (point), ROr (body (point), ROr (leg1(point),leg2(point))))))) ;
}

RFuncSprite rFuncSprite;


void RenderGui(RenderWindow &window)
{
	static float color[3];
	ImGui::Begin("Default window");
	if (ImGui::Button("Save"))
	{
		rFuncSprite.Save();
	}
	if (ImGui::ColorPicker3("color picker", color))
	{
		window.clear(Color::White);
		colour[0] = int(color[0]*255.0f);
		colour[1] = int(color[1] * 255.0f);
		colour[2] = int(color[2] * 255.0f);
	}
	ImGui::End();
}
void Update(sf::RenderWindow &window, const sf::Time &deltaClock)
{
	// Make some time-dependent updates, like: physics, gameplay logic, animations, etc.
}
void Render(sf::RenderWindow &window)
{
	window.clear(Color::White);
	rFuncSprite.DrawRFunc(&myFunc, FloatRect(-9, -10, 15, 20), colour[0], colour[1], colour[2]);
	rFuncSprite.setauall();
	window.draw(rFuncSprite);
	rFuncSprite.findGradient(1,1,1);
	rFuncSprite.SetGradient();
	window.draw(rFuncSprite);
}

void HandleUserInput(sf::RenderWindow &window, const sf::Event &event)
{
	switch (event.type)
	{
	case Event::Closed:
		window.close();
		break;
	case sf::Event::MouseButtonPressed:
	{
		if (event.key.code == Mouse::Left)
		{
			rFuncSprite.findGradient((float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y, 0.1);
			rFuncSprite.SetGradient();
			window.draw(rFuncSprite);
		}
	}
	default:
		break;
	}
}
int main()
{
	rFuncSprite.Create(Vector2u(600, 600));
	RenderWindow window(sf::VideoMode(600, 600), "Lab3");
	window.setFramerateLimit(60);

	/*sf::Clock deltaClock;
	if (!ImGui::SFML::Init(window))
	{
		std::cout << "ImGui initialization failed\n";
		return -1;
	}*/
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			/*ImGui::SFML::ProcessEvent(window, event);*/
			HandleUserInput(window, event);
		}
		/*sf::Time deltaTime = deltaClock.restart();
		ImGui::SFML::Update(window, deltaTime);
		Update(window, deltaTime);

		RenderGui(window);*/
		Render(window);

		/*ImGui::SFML::Render(window);*/
		window.display();
	}
	/*ImGui::SFML::Shutdown();*/
	return 0;
};
