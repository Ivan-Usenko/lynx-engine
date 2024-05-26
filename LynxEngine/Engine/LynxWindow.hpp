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
	private:
		std::unordered_map<sf::Event::EventType, std::list<std::function<void(sf::Event)>>> m_events_handlers;
	};
}