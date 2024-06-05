#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <list>
#include <functional>
#include "Engine/Types.hpp"

namespace lynx
{
	class LynxWindow : public sf::RenderWindow
	{
	public:
		LynxWindow(sf::VideoMode mode = sf::VideoMode(400, 400), std::string title = "LynxEngine Window");
		void addEventHandler(sf::Event::EventType type, std::function<void(sf::Event)> handler);
		void clearEventHandlers(sf::Event::EventType type);
		void handleEvents();

		void drawCircle(float cx, float cy, float radius, float rotation = 0.f, sf::Color ocolor = sf::Color::White, sf::Color fcolor = sf::Color::Transparent);
		void drawRectangle(float cx, float cy, float dx, float dy, float rotation = 0.f, sf::Color ocolor = sf::Color::White, sf::Color fcolor = sf::Color::Transparent);
		void drawLine(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::White);
		void drawVector(Vector2 origin, Vector2 dir, float length, sf::Color color = sf::Color::White);
		void drawLabel(std::string label, unsigned int char_size = 30u, Vector2 position = Vector2(), sf::Color color = sf::Color::White);

		Vector2 getRelMousePos();
		sf::Font& getDefaultFont();
		void setBackgroundColor(sf::Color bg_color);
		void clear();
	private:
		std::unordered_map<sf::Event::EventType, std::list<std::function<void(sf::Event)>>> m_events_handlers;
		sf::Font m_default_font;
		sf::Color m_bg_color;
	};
}