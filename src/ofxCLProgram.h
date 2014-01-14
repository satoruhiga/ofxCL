#pragma once

#include "ofxCLConstants.h"

#include "ofxCLKernel.h"

OFX_CL_BEGIN_NAMESPACE

class Program
{
public:
	
	bool load(string path);
	bool reload();
	
	Kernel::Ref getKernel(string name);
	
	operator cl::Program&() { return program; }
	
private:
	
	string path;
	cl::Program program;
	
	map<string, Kernel::Ref> kernels;
};

OFX_CL_END_NAMESPACE

