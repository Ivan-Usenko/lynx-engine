#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <list>
#include <functional>

namespace lynx
{
	class LynxWindow : public sf::RenderWindow
	{
	public:
		LynxWindow(sf::VideoMode mode = sf::VideoMode(400, 400), std::string title = "LynxEngine Window");
		void addEventHandler(sf::Event::EventType type, std::function<void(sf::Event)> handler);
		void clearEventHandlers(sf::Event::EventType type);
		void handleEvents();

		void drawCircle(float cx, float cy, float radius, sf::Color ocolor = sf::Color::White, sf::Color fcolor = sf::Color::Transparent);
		void drawRectangle(float cx, float cy, float dx, float dy, float rotation = 0.f, sf::Color ocolor = sf::Color::White, sf::Color fcolor = sf::Color::Transparent);
		void drawLine(float x1, float y1, float x2, float y2, sf::Color color = sf::Color::White);

		sf::Vector2f getRelMousePos();
	private:
		std::unordered_map<sf::Event::EventType, std::list<std::function<void(sf::Event)>>> m_events_handlers;
	};
}