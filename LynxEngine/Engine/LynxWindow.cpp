#include "LynxWindow.hpp"
#include "Math/LynxMath.hpp"

namespace lynx
{
	LynxWindow::LynxWindow(sf::VideoMode mode, std::string title) : sf::RenderWindow(mode, title)
	{
		m_default_font.loadFromFile("C:\\Windows\\Fonts\\Consola.ttf");
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

	void LynxWindow::drawCircle(float cx, float cy, float radius, float rotation, sf::Color ocolor, sf::Color fcolor)
	{
		sf::CircleShape circle;
		circle.setPosition(cx, cy);
		circle.setOrigin(radius, radius);
		circle.setRadius(radius);
		circle.setRotation(rotation);
		circle.setOutlineThickness(-1.f);
		circle.setOutlineColor(ocolor);
		circle.setFillColor(fcolor);

		sf::Vertex line[2];
		line[0].color = line[1].color = ocolor;

		line[0].position = Vector2(circle.getRadius(), circle.getRadius());
		line[1].position = Vector2(circle.getRadius() + circle.getRadius(), circle.getRadius());

		this->draw(circle);
		this->draw(line, 2, sf::Lines, circle.getTransform());
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
		line[0].position = sf::Vector2f(x1, y1);
		line[1].position = sf::Vector2f(x2, y2);
		this->draw(line, 2, sf::Lines);
	}

	void LynxWindow::drawVector(Vector2 origin, Vector2 dir, float length, sf::Color color)
	{
		sf::Vertex arrow[4];
		for (sf::Vertex& v : arrow) v.color = color;

		arrow[0].position = origin;
		arrow[1].position = origin + dir * length;

		Vector2 base = origin + dir * (length - 15.f);
		arrow[2].position = base + Vector2(-dir.y, dir.x) * 5.f;
		arrow[3].position = base + Vector2(dir.y, -dir.x) * 5.f;

		this->draw(arrow, 2, sf::Lines);
		this->draw(arrow + 1, 3, sf::Triangles);
	}

	void LynxWindow::drawLabel(std::string label, unsigned int char_size, Vector2 position, sf::Color color)
	{
		sf::Text text(label, m_default_font, char_size);
		text.setPosition(position);
		text.setFillColor(color);
		this->draw(text);
	}

	sf::Vector2f LynxWindow::getRelMousePos()
	{
		return (sf::Vector2f)sf::Mouse::getPosition(*this);
	}

	sf::Font& LynxWindow::getDefaultFont()
	{
		return m_default_font;
	}

	void LynxWindow::setBackgroundColor(sf::Color bg_color)
	{
		m_bg_color = bg_color;
	}

	void LynxWindow::clear()
	{
		sf::RenderWindow::clear(m_bg_color);
	}
}