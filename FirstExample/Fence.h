#ifndef __FENCE__
#define __FENCE__

#include "Object.h"

class Fence : public Object
{
private:

public:
	Fence(GLuint uniformWorld, GLuint uniformShininess, float fShininess);
	~Fence();

	virtual void Render();
};

#endif

