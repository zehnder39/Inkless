#include "header.hpp"

#include <iostream>


void move_player()
{
	float sx = 0;
	float sy = 0;
	if (moving_left) {
		sx += -5;
	}
	if (moving_right) {
		sx += 5;
	}
	if (moving_up)
		sy += -5;
	if (moving_down)
		sy += 5;
	player.move(sx, sy);
}