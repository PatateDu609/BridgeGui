#pragma once
#ifndef GAME_H
#define GAME_H

#include <array>
#include "Cards.h"

namespace game {
	typedef std::array<int, 2> Contract;
	typedef std::array<int, 2> Score;
	void showContract(Contract const& c);
	cards::Card compare(int color, int contract, cards::Card const& a, cards::Card const& b);
}

#endif
