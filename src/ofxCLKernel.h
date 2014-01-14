#pragma once

#include "ofxCLConstants.h"

#include "ofxCLContext.h"

OFX_CL_BEGIN_NAMESPACE

class Program;

class Kernel
{
	friend class Program;
	
	struct X {};
	
	template <typename T>
	inline void COUNT(unsigned int& idx, T& arg) { idx++; }
	inline void COUNT(unsigned int &idx, X arg) {}
	
	template <typename T>
	inline void ARG(unsigned int& idx, T& arg)
	{
		reportError(kernel.setArg(idx, arg));
		idx++;
	}
	
	inline void ARG(unsigned int &idx, X arg) {}
	
	inline void ARG(unsigned int &idx, cl::ImageGL arg)
	{
		gl_objects.push_back(arg);
		reportError(kernel.setArg(idx, arg));
		idx++;
	}
	
	inline void ARG(unsigned int &idx, cl::BufferGL arg)
	{
		gl_objects.push_back(arg);
		reportError(kernel.setArg(idx, arg));
		idx++;
	}
	
public:
	
	typedef ofPtr<Kernel> Ref;
	
	void offset(int x, int y = 0, int z = 0);
	void global(int x, int y = 0, int z = 0);
	void local(int x, int y = 0, int z = 0);
	
	template <
	typename T0 = X,
	typename T1 = X,
	typename T2 = X,
	typename T3 = X,
	typename T4 = X,
	typename T5 = X,
	typename T6 = X,
	typename T7 = X,
	typename T8 = X,
	typename T9 = X,
	typename T10 = X,
	typename T11 = X,
	typename T12 = X,
	typename T13 = X,
	typename T14 = X,
	typename T15 = X
	>
	bool call(const T0& t0 = X(),
			  const T1& t1 = X(),
			  const T2& t2 = X(),
			  const T3& t3 = X(),
			  const T4& t4 = X(),
			  const T5& t5 = X(),
			  const T6& t6 = X(),
			  const T7& t7 = X(),
			  const T8& t8 = X(),
			  const T9& t9 = X(),
			  const T10& t10 = X(),
			  const T11& t11 = X(),
			  const T12& t12 = X(),
			  const T13& t13 = X(),
			  const T14& t14 = X(),
			  const T15& t15 = X())
	{
		assert(kernel());
		
		gl_objects.clear();
		
		{
			// check args count
			
			unsigned int num_arg = 0;
			
			COUNT(num_arg, t0);
			COUNT(num_arg, t1);
			COUNT(num_arg, t2);
			COUNT(num_arg, t3);
			COUNT(num_arg, t4);
			COUNT(num_arg, t5);
			COUNT(num_arg, t6);
			COUNT(num_arg, t7);
			COUNT(num_arg, t8);
			COUNT(num_arg, t9);
			COUNT(num_arg, t10);
			COUNT(num_arg, t11);
			COUNT(num_arg, t12);
			COUNT(num_arg, t13);
			COUNT(num_arg, t14);
			COUNT(num_arg, t15);
			
			if (num_arg != kernel.getInfo<CL_KERNEL_NUM_ARGS>())
			{
				ofLogError("ofxCL::Kernel") << "Argument count mismatch on: " << name << "()";
				return false;
			}
		}
		
		if (_global.dimensions() == 0)
		{
			ofLogError("ofxCL::Kernel") << "Invalid global work dimension";
			return false;
		}
		
		{
			unsigned int idx = 0;
			ARG(idx, t0);
			ARG(idx, t1);
			ARG(idx, t2);
			ARG(idx, t3);
			ARG(idx, t4);
			ARG(idx, t5);
			ARG(idx, t6);
			ARG(idx, t7);
			ARG(idx, t8);
			ARG(idx, t9);
			ARG(idx, t10);
			ARG(idx, t11);
			ARG(idx, t12);
			ARG(idx, t13);
			ARG(idx, t14);
			ARG(idx, t15);
		}
		
		cl::CommandQueue &queue = Context::getCurrentContext().queue;
		
		cl::Event event;
		cl_int err = CL_SUCCESS;
		
		if (gl_objects.size())
		{
			glFinish();
			
			cl::Event e;
			cl_int err = queue.enqueueAcquireGLObjects(&gl_objects, NULL, &e);
			e.wait();
		}
		
		err = queue.enqueueNDRangeKernel(kernel,
										 _offset,
										 _global,
										 _local,
										 NULL,
										 &event);
		
		if (!reportError(err)) return false;
		if (!reportError(event.wait())) return false;
		
		if (gl_objects.size())
		{
			cl::Event e;
			queue.enqueueReleaseGLObjects(&gl_objects, NULL, &e);
			e.wait();
			
			glFinish();
		}
		
		return true;
	}
	
private:
	
	string name;
	
	cl::NDRange _offset, _global, _local;
	cl::Kernel kernel;
	
	vector<cl::Memory> gl_objects;
	
	Kernel(string name, Program& prog);
	void setupKernel(cl::Program& program);
};

OFX_CL_END_NAMESPACE

