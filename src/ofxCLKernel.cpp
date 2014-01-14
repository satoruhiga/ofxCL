#include "ofxCLKernel.h"

#include "ofxCLProgram.h"

OFX_CL_BEGIN_NAMESPACE

Kernel::Kernel(string name, Program& prog) : name(name), _offset(cl::NullRange), _global(cl::NullRange), _local(cl::NullRange)
{
	setupKernel(prog);
}

void Kernel::setupKernel(cl::Program& program)
{
	cl_int err = CL_SUCCESS;
	kernel = cl::Kernel(program, name.c_str(), &err);
	reportError(err);
}

void Kernel::offset(int x, int y, int z)
{
	if (x >= 1 && y >= 1 && z >= 1)
		_offset = cl::NDRange(x, y, z);
	else if (x >= 1 && y >= 1)
		_offset = cl::NDRange(x, y);
	else if (x >= 1)
		_offset = cl::NDRange(x);
}

void Kernel::global(int x, int y, int z)
{
	if (x >= 1 && y >= 1 && z >= 1)
		_global = cl::NDRange(x, y, z);
	else if (x >= 1 && y >= 1)
		_global = cl::NDRange(x, y);
	else if (x >= 1)
		_global = cl::NDRange(x);
}

void Kernel::local(int x, int y, int z)
{
	if (x >= 1 && y >= 1 && z >= 1)
		_local = cl::NDRange(x, y, z);
	else if (x >= 1 && y >= 1)
		_local = cl::NDRange(x, y);
	else if (x >= 1)
		_local = cl::NDRange(x);
}

OFX_CL_END_NAMESPACE