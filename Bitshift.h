#ifndef BITSHIFT_H
#define BITSHIFT_H

class Bitshift {
public:
	unsigned int store;
	bool grab(unsigned int place);
	Bitshift& operator= (const unsigned int &x);
	void flip(unsigned int place, bool value);
	Bitshift();
};

#endif //guard
