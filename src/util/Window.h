#pragma once

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <string>

class Window final
{
private:
	sf::RenderWindow m_renderWindow;	// the window is where we render the buffer to

	sf::Texture m_textureBuffer;	// before we render the buffer, we need to turn it into a sf::Texture
	sf::Sprite m_spriteBuffer;		// before we render the texture, we need to apply it to a sprite

	bool m_is_focused{ true };

	uint8_t m_scale;

public:
	Window(
		const uint16_t width, 
		const uint16_t height, 
		const uint8_t scale, 
		const std::string& name
	);

	void update();
	void clear(const sf::Color clearColor);
	void draw(const sf::Image& buffer);

	sf::RenderWindow& getWindow() { return m_renderWindow; }

private:

	void pollEvents();
};