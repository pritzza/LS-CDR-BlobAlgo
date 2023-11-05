#pragma once

#include "util/Window.h"

#include "Grid.h"

#include <unordered_map>
#include <random>
#include "util/Math.h"
#include "Blob.h"

class Simulation final
{
public:
	Simulation(
		const uint16_t width,
		const uint16_t height, 
		const uint8_t scale,
		const std::string& name
	);
	~Simulation();

	void start();
	void stop();

private:
	Window m_window;

	const uint16_t width;
	const uint16_t height;
	const Vec2i dim;

	Grid* m_grid;

	sf::Image m_buffer;	// the buffer stores all the pixels of the cellular automata

	const uint8_t m_frameRate{ 60 };

	bool m_isRunning{ false };

	int scale{ 1 };

	Cell::Type placing_cell_type{ Cell::Type::Filled };

	const Cell OUT_OF_BOUNDS_CELL{Cell::Type::Filled};

	std::vector<Blob> blobs;
	int num_blobs = 50;
	std::vector<sf::Color> blob_colors;

	int iterations_left{ 0 };

private:
	void init();

	void applicationLoop();

	void handleInput();
	void update(int tick);
	void render();

};