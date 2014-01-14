#pragma once

#include "ofxCLConstants.h"

OFX_CL_BEGIN_NAMESPACE

class Context
{
	friend class Program;
	friend class Kernel;
	
public:
	
	void setup(int device_id = 0, bool withOpenGL = true);
	
	// for context switch
	void begin();
	void end();
	
	void finish();
	
	static void listDevices();
	static Context& getCurrentContext();
	
	cl::Context get() { return context; }
	cl::CommandQueue getCommandQueue() { return queue; }
	
private:
	
	static Context* current_context;
	
	cl::Context context;
	cl::Device device;
	cl::CommandQueue queue;
};

OFX_CL_END_NAMESPACE

