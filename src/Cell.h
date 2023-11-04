#pragma once

#include "util/EnumArray.h"

class Cell
{
public:
	static constexpr int INVALID_ID{ -1 };

	enum class Type
	{
		Empty,
		Filled,
		SIZE
	};

	Cell() { this->reset(); }
	Cell(Type t, int id = 0) 
	{ 
		this->reset();
		m_type = t;
		m_id = id;
	}

	// Cell Interfaces
	void reset() 
	{ 
		m_type = Type::Empty; 
		m_id = 0;
	}

	bool isValid() const { return m_id != INVALID_ID; }

	// util functions
	bool isEmpty() const { return m_type == Type::Empty; }

	// setters
	void setType(const Type type) { m_type = type; }

	// getters
	Type getType() const { return m_type; }

	// setters
	void setID(int id) { m_id = id; }

	// getters
	int getID() const { return m_id; }

private:
	Type m_type;
	int m_id{ 0 };

};
