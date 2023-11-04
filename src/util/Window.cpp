#include "Window.h"

Window::Window(
	const uint16_t width, 
	const uint16_t height, 
	const uint8_t scale, 
	const std::string& name
)
	:
	m_renderWindow{ sf::VideoMode(width * scale, height * scale), name, sf::Style::Default },
	m_scale { scale }
{}

void Window::pollEvents()
{
	sf::Event event;

	if (m_renderWindow.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::Closed:			m_renderWindow.close();		break;
		case sf::Event::GainedFocus:	m_is_focused = true;		break;
		case sf::Event::LostFocus:		m_is_focused = false;		break;
		}
}

void Window::update()
{
	pollEvents();
}

void Window::clear(const sf::Color clearColor = sf::Color{ 0, 0, 0, 255 })
{
	m_renderWindow.clear(clearColor);
}

void Window::draw(const sf::Image& buffer)
{
	// begin draw
	m_renderWindow.clear(sf::Color::Magenta);

	// get image buffer into a sprite
	m_textureBuffer.loadFromImage(buffer);
	m_spriteBuffer.setTexture(m_textureBuffer);

	m_spriteBuffer.setScale(sf::Vector2f( m_scale, m_scale ));

	// actually drawing
	m_renderWindow.draw(m_spriteBuffer);
	
	// finishing the draw
	m_renderWindow.display();
}