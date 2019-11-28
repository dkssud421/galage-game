class entity
{
protected:
	short x, y;
	short l, r, t, b;
	bool tf = false;
public:
	entity(short x, short y)
	{
		this->x = x; this->y = y;
	}

	short show_x();
	short show_y();

	short show_l();
	short show_r();
	short show_t();
	short show_b();

	bool drow();
	void die();

	virtual void move_x(short);
	virtual void move_y(short);
	
	~entity()
	{
		x = 0; y = 0; l = 0; r = 0; t = 0; b = 0; tf = false;
	}
};

class plane :public entity
{
private:
	short hp = 0;
public:
	plane(short fx, short fy) :entity(fx, fy)
	{
		l = 0; t = 0; r = 88; b = 109; tf = true;
		hp = 3;
	}

	short show_hp();
	void damege();
	void move_x(short);
	void move_y(short);
	~plane()
	{
		tf = false;
		hp = 0;
		x = 0; y = 0;
	}
};

class bleat :public entity
{
private:

public:
	bleat(short fx, short fy) :entity(fx, fy)
	{
		l = 0; t = 0; r = 30; b = 30;
		tf = false;
	}
	void move_bleat();
	void bleat_shot();
	void bleat_crach();
	virtual void bmove_y();
	~bleat()
	{
		x = -70; y = -70; tf = false; 
	}
};

class bleat2 : public bleat
{
public:
	bleat2(short fx, short fy) :bleat(fx, fy)
	{
		l = 0; t = 0; r = 30; b = 30;
		tf = false;
	}
	void bmove_y();
	~bleat2()
	{
		x = -70; y = -70; tf = false;
	}
};

class meteor :public entity
{
protected:
	short hp;
public:
	meteor(short fx, short fy) :entity(fx, fy)
	{
		l = 0; t = 0; r = 70; b = 70; tf = true;
		hp = 0;
	}
	~meteor()
	{
		x = 0; y = 0; tf = false;
	}
	virtual void move_meteor();
	virtual void meteor_crach1();
	virtual void meteor_crach2();
	void lie_meteor();
};
class meteor1 :public meteor
{
public:
	meteor1(short fx, short fy) :meteor(fx, fy)
	{
		l = 0; t = 0; r = 70; b = 70; tf = false;
		hp = 1;
	}
	 void move_meteor();
};
class meteor2 :public meteor
{
public:
	meteor2(short fx, short fy) :meteor(fx, fy)
	{
		l = 0; t = 0; r = 70; b = 70; tf = false;
		hp = 2;
	}
	void move_meteor();
};
class meteor3 :public meteor
{
public:
	meteor3(short fx, short fy) :meteor(fx, fy)
	{
		l = 0; t = 0; r = 70; b = 70; tf = false;
		hp = 3;
	}
	void move_meteor();
};
class meteor_boss :public meteor
{
public:
	meteor_boss(short fx, short fy) :meteor(fx, fy)
	{
		hp = 8; tf = false;
	}
	void move_meteor();
	void meteor_true();
};
