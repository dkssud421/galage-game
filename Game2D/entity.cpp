#include"entity.h"
short entity::show_x()
{
	return x;
}
short entity::show_y()
{
	return y;
}
short entity::show_l()
{
	return l;
}
short entity::show_r()
{
	return r;
}
short entity::show_t()
{
	return t;
}
short entity::show_b()
{
	return b;
}
void entity::move_x(short fx)
{
	x = fx;
}
void entity::move_y(short fy)
{
	y = fy;
}

void entity::die()
{
	tf = false;
}
bool entity::drow()
{
	return tf;
}

short plane::show_hp()
{
	return hp;
}
void plane::damege()
{
	hp = hp - 1;
	if (hp <= 0)
		tf = false;

}
void plane::move_x(short fx)
{
	if (fx >= 640 - r)
		fx = 640 - r;
	else if (fx <= 0)
		fx = 0;
	x = fx;
}
void plane::move_y(short fy)
{
	if (fy >= 430 - b)
		fy = 430 - b;
	else if (fy <= t)
		fy = t;
	y = fy;
}

void bleat::move_bleat()
{
	y = y - 10;
}
void bleat::bleat_shot()
{
	tf = true;
}
void bleat::bleat_crach()
{
	if (tf == true)
		tf = false;
}
void bleat::bmove_y()
{
	y = y - 10;
	if (y == -30)
		die();
}

void bleat2::bmove_y()
{
	y = y - 20;
	if (y == -30)
		die();
}

void meteor::lie_meteor()
{
	tf = true;
}
void meteor::move_meteor()
{
	y = y + 5;
	if (y == 500)
		die();
	if (y >= 430)
	{
		y = 0; tf = true;
	}
}

void meteor::meteor_crach1()
{
	hp = hp - 1;
	if (hp <= 0)
		tf = false;
}

void meteor::meteor_crach2()
{
	hp = hp - 3;
	if (hp <= 0)
		tf = false;
}

void meteor1::move_meteor()
{
	y = y + 5;
	if (y == 500)
		die();
	if (y >= 430)
	{
		y = 0; tf = true; hp = 1;
	}
}
void meteor2::move_meteor()
{
	y = y + 10;
	x = x + 5;
	if (x >= 640)
		x = 0;
	if (y == 500)
		die();
	if (y >= 430)
	{
		y = 0; tf = true; hp = 2;
	}
}

void meteor3::move_meteor()
{
	y = y + 10;
	if (y == 500)
		die();
	if (y >= 430)
	{
		y = 0; tf = true; hp = 3;
	}
}

void meteor_boss::move_meteor()
{
	y = y + 2;
}
void meteor_boss::meteor_true()
{
	tf = true;
}