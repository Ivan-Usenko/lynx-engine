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

	void LynxWindow::drawCircle(float cx, float cy, float radius, sf::Color ocolor, sf::Color fcolor)
	{
		sf::CircleShape circle;
		circle.setPosition(cx, cy);
		circle.setOrigin(radius, radius);
		circle.setRadius(radius);
		circle.setOutlineThickness(-1.f);
		circle.setOutlineColor(ocolor);
		circle.setFillColor(fcolor);
		this->draw(circle);
	}

	void LynxWindow::drawRectangle(float cx, float cy, float dx, float dy, float rotation, sf::Color ocolor, sf::Color fcolor)
	{
		sf::RectangleShape rect;
		rect.setPosition(cx, cy);
		rect.setOrigin(dx, dy);
		rect.setSize(2.f * sf::Vector2f(dx, dy));
		rect.setRotation(rotation);
		rect.setOutlineThickness(-1.f);
		rect.setOutlineColor(ocolor);
		rect.setFillColor(fcolor);
		this->draw(rect);
	}

	void LynxWindow::drawLine(float x1, float y1, float x2, float y2, sf::Color color)
	{
		sf::Vertex line[2];
		line[0].color = line[1].color = color;
		line[0] = sf::Vector2f(x1, y1);
		line[1] = sf::Vector2f(x2, y2);
		this->draw(line, 2, sf::Lines);
	}
}