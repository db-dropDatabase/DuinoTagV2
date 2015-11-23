#ifndef BITSHIFT_CPP
#define BITSHIFT_CPP

#include <Bitshift.h>

Bitshift::Bitshift(void) {
	store = 0;
}

Bitshift& Bitshift::operator= (const unsigned int &x) {
	store = x;
	return *this;
}

bool Bitshift::grab(unsigned int place) {
	return store & (1 << place);
}

void Bitshift::flip(unsigned int place, bool value) {
#ifdef DEBUG
	if (value>7 || value<0) {
		Serial.println("Value out of bounds!");
	}
#endif
	if (value) {
		store = store | (1 << place);
	}
	else {
		store = store & ~(1 << place);
	}
}

#endif //guard