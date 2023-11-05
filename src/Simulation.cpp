#include "Simulation.h"

#include <iostream>

#include "util/Math.h"

Simulation::Simulation(
	const uint16_t width, 
	const uint16_t height, 
	const uint8_t scale, 
	const std::string& name
)
	:
	m_window{ width, height, scale, name },
	width{ width },
	height{ height },
	dim{ width, height },
	scale{ scale },
	m_grid{ new Grid{ width, height } }
	//m_next_grid{ new Grid{ width, height } }
{
	this->init();
}

Simulation::~Simulation()
{
	delete m_grid;
	//delete m_next_grid;
}

void Simulation::init()
{
	const int w = m_grid->getWidth();
	const int h = m_grid->getHeight();

	// create the buffer so we can draw our cells on it
	m_buffer.create(w, h, sf::Color(255, 0, 255));

	const int blobs_to_add = num_blobs;

	auto getColor = [](int id) -> sf::Color {
		sf::Color color(
			(((id << 2) * 370) % 256),//(256/2)) * 2,
			(((id << 3) * 4813) % 256),//(256/2)) * 2,
			(((id << 4) * 171) % 256) //(256/2)) * 2
		);
		return color;
	};

	for (int i = 0; i < num_blobs; ++i)
	{
		blobs.emplace_back(*m_grid, i + 1);

		const Vec2i pos{
			Math::rng(0,w),
			Math::rng(0,h),
		};

		blobs[i].spawn(pos);

		blob_colors.emplace_back(getColor(i + 1));
	}

}

void Simulation::start()
{
	m_isRunning = true;

	this->applicationLoop();
}

void Simulation::stop()
{
	m_isRunning = false;
}

void Simulation::applicationLoop()
{
	sf::Clock timer;

	int FPS{ 60 };
	double FRAME_DURATION{ 1.0 / FPS };

	int TPS{ 100 };
	double TICK_DURATION{ 1.0 / TPS };

	sf::Time currentFrame, prevFrame;
	sf::Time currentTick,  prevTick;

	double deviation{};

	double dtTimeAccumulator{};
	double dtTickAccumulator{};

	int frame{};
	int tick{};

	while (m_isRunning)
	{
		// time
		prevFrame = currentFrame;
		currentFrame = timer.getElapsedTime();
		const sf::Time dtTime{ currentFrame - prevFrame };

		dtTimeAccumulator += dtTime.asSeconds();

		while (dtTimeAccumulator > FRAME_DURATION)
		{
			frame++;

			this->handleInput();
			this->render();

			dtTimeAccumulator -= FRAME_DURATION;
		}

		// tick
		prevTick = currentTick;
		currentTick = timer.getElapsedTime();
		const sf::Time dtTick{ currentTick - prevTick };

		dtTickAccumulator += dtTick.asSeconds();

		if (dtTickAccumulator > TICK_DURATION)
		{
			tick++;

			this->update(tick);

			dtTickAccumulator -= TICK_DURATION;
		}

		// frame debugging
		static constexpr bool FRAME_DEBUGGING{ false };
		if constexpr (FRAME_DEBUGGING)
			if (frame % FPS == 0)
			{
				std::cout << "Cur Time: " << currentFrame.asSeconds() << '\n'
					<< "Avg Frame Dev: " << deviation / frame << '\n'
					<< "DT: " << dtTime.asSeconds() << '\n'
					<< "DT Error: " << dtTime.asSeconds() - FRAME_DURATION << '\n'
					<< "Tick: " << tick << '\n'
					<< std::endl;
			}
	}
}

void Simulation::handleInput()
{
	const sf::Vector2i& cursor_pos{ sf::Mouse::getPosition(m_window.getWindow()) };

	sf::Vector2i selected_cell{ cursor_pos / scale };

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		Cell c;
		c.setType(placing_cell_type);

		m_grid->setCell(selected_cell.x, selected_cell.y, c);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		m_grid->clear();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		placing_cell_type = Cell::Type::Filled;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		placing_cell_type = Cell::Type::Empty;
}

void Simulation::update(const int tick)
{
	m_window.update();
	
	if (!m_window.getWindow().isOpen())
		m_isRunning = false;

	// sim code
	for (int i = 0; i < 100; ++i)
		for (Blob& b : blobs)
			b.grow();

	int filled_cells = 0;

	for (Blob& b : blobs)
	{
		b.cullPerimeter();
		filled_cells += b.getArea();
	}

	const bool is_filled = (filled_cells == width * height);

	if (is_filled && iterations_left != 0)
	{
		m_grid->clear();
		iterations_left--;
	}
}

void Simulation::render()
{
	static const sf::Color EMPTY{ 223, 224, 255 };
	static const sf::Color FILLED{  12,  70, 230 };

	for (int i = 0; i < m_grid->getSize(); ++i)
	{
		// we need x and y because thats how we interface with m_buffer
		const int x{ i % m_grid->getWidth() };
		const int y{ i / m_grid->getWidth() };

		const Cell& c = m_grid->getCell(x, y);
		const int id = c.getID();

		const bool non_blob_id = (id <= 0 || id >= blob_colors.size());

		const sf::Color color = non_blob_id ? sf::Color::Black : blob_colors[id - 1];

		switch (c.getType())
		{
			case Cell::Type::Empty:		m_buffer.setPixel(x, y, EMPTY);		break;
			case Cell::Type::Filled:	m_buffer.setPixel(x, y, color);		break;

			default:	m_buffer.setPixel(x, y, sf::Color::Magenta);		break;
		}
	}

	m_window.draw(m_buffer);
}