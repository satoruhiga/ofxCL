#include "ofxCLProgram.h"

OFX_CL_BEGIN_NAMESPACE

bool Program::load(string path)
{
	if (!ofFile::doesFileExist(path))
	{
		ofLogError("ofxCL") << "file not found: " << path;
		return false;
	}
	
	this->path = path;
	
	string src = ofBufferFromFile(path).getText();
	cl::Program::Sources source(1, make_pair(src.c_str(), src.size()));
	
	Context &ctx = Context::getCurrentContext();
	
	program = cl::Program(ctx.context, source);
	
	{
		std::vector<cl::Device> devices;
		devices.push_back(ctx.device);
		
		const char* options = "-cl-fast-relaxed-math -cl-single-precision-constant";
		cl_int err = program.build(devices, options);
		
		if (err == CL_BUILD_PROGRAM_FAILURE)
		{
			string error_str;
			program.getBuildInfo(ctx.device(), CL_PROGRAM_BUILD_LOG, &error_str);
			cout << error_str << endl;
			
			return false;
		}
		
		reportError(err);
	}
	
	map<string, Kernel::Ref>::iterator it = kernels.begin();
	while (it != kernels.end())
	{
		it->second->setupKernel(program);
		it++;
	}
	
	return true;
}

bool Program::reload()
{
	return load(path);
}

Kernel::Ref Program::getKernel(string name)
{
	if (kernels.find(name) == kernels.end())
	{
		Kernel::Ref o = Kernel::Ref(new Kernel(name, *this));
		kernels[name] = o;
		return o;
	}
	else
	{
		return kernels[name];
	}
}

OFX_CL_END_NAMESPACE