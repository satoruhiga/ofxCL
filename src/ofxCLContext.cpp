#include "ofxCLContext.h"

OFX_CL_BEGIN_NAMESPACE

Context* Context::current_context;

Context& Context::getCurrentContext()
{
	assert(current_context);
	return *current_context;
}

void Context::listDevices()
{
	printf("-------------------------------------\n");
	
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	
	cl::Platform platform = platforms.back();
	
	cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(), 0};
	cl::Context context(CL_DEVICE_TYPE_ALL, properties);
	
	std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
	for (int i = 0; i < devices.size(); i++)
	{
		cl::Device &dev = devices[i];
		printf( "device[%i] %s - %s\n", i,
			   dev.getInfo<CL_DEVICE_VENDOR>().c_str(),
			   dev.getInfo<CL_DEVICE_NAME>().c_str());
		
	}
	
	printf("-------------------------------------\n");
}

void Context::setup(int device_id, bool withOpenGL)
{
	if (context()) return;
	
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	
	cl::Platform platform = platforms.back();
	
	if (!withOpenGL)
	{
		cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(), 0};
		context = cl::Context(CL_DEVICE_TYPE_ALL, properties);
	}
	else
	{
#ifdef TARGET_OSX
		CGLContextObj kCGLContext = CGLGetCurrentContext();
		CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(kCGLContext);
		cl_context_properties properties[] = { CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE, (cl_context_properties)kCGLShareGroup, 0 };
#else
		cl_context_properties properties[] = {
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
			CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0])(),
			0};
#endif
		
		context = cl::Context(CL_DEVICE_TYPE_ALL, properties);
	}
	
	std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
	device = devices[device_id];
	
	cl_int err = CL_SUCCESS;
	queue = cl::CommandQueue(context, device, 0, &err);
	reportError(err);
	
	current_context = this;
}

void Context::begin()
{
	current_context = this;
}

void Context::end()
{
	current_context = NULL;
}

void Context::finish()
{
	cl::finish();
}


OFX_CL_END_NAMESPACE