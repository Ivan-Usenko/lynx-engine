#include "LynxWindow.hpp"

namespace lynx
{
	LynxWindow::LynxWindow(sf::VideoMode mode, std::string title) : sf::RenderWindow(mode, title)
	{
		for (int i = 0; i < sf::Event::Count; i++)
		{
			m_events_handlers.insert(
				std::pair<sf::Event::EventType, std::list<std::function<void(sf::Event)>>>(
					(sf::Event::EventType)i,
					std::list<std::function<void(sf::Event)>>()
				)
			);
		}

		addEventHandler(sf::Event::Closed, [&](sf::Event) { this->close(); });
	}

	void LynxWindow::addEventHandler(sf::Event::EventType type, std::function<void(sf::Event)> handler)
	{
		m_events_handlers.at(type).push_back(handler);
	}

	void LynxWindow::clearEventHandlers(sf::Event::EventType type)
	{
		m_events_handlers.at(type).clear();
	}

	void LynxWindow::handleEvents()
	{
		sf::Event e;
		while (this->pollEvent(e))
		{
			for (std::function<void(sf::Event)>& handler : m_events_handlers.at(e.type))
			{
				handler(e);
			}
		}
	}
}