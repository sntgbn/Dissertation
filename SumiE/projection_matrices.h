#ifndef _PROJECTION_MATRICES_H
#define _PROJECTION_MATRICES_H_

#include <maths_funcs.h> //Anton's math class

class ProjectionMatrices {
public:
	mat4 projection;
	mat4 view;
	mat4 model;
	mat4 ortho;
};

#endif //_PROJECTION_MATRICES_H