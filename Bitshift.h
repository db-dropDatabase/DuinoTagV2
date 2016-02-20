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

struct packet { //this just makes my life easier
	Bitshift data1;  //1 bit for packet type, 7 bits for playerID 
	Bitshift data2;  //6 or 8 bits for team and damage or message data
};

#endif //guard
