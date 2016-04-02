#pragma once

#include "ofxCLConstants.h"

OFX_CL_BEGIN_NAMESPACE

// Buffer

template <typename T>
inline void convert(const vector<T>& data, cl::Buffer& buffer, cl_mem_flags flag = CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR)
{
	cl_int err = CL_SUCCESS;
	
	if (!(flag & CL_MEM_USE_HOST_PTR)
		&& !(flag & CL_MEM_COPY_HOST_PTR))
	{
		ofLogError("ofxCL") << "Invalid flag";
		assert(false);
	}
	
	buffer = cl::Buffer(ofxCL::Context::getCurrentContext().get(),
						flag,
						sizeof(T) * data.size(),
						(void*)&data[0],
						&err);
	
	ofxCL::reportError(err);
}

template <typename T>
inline cl::Event copy(cl::Buffer& buffer, vector<T>& data, bool block = true)
{
	cl_int err = CL_SUCCESS;

	const cl::CommandQueue &queue = ofxCL::Context::getCurrentContext().getCommandQueue();
	
	cl::Event event;
	err = queue.enqueueReadBuffer(buffer,
								  block,
								  0,
								  data.size() * sizeof(T),
								  &data[0],
								  NULL,
								  &event);
	
	ofxCL::reportError(err);
	
	return event;
}

template <typename T>
inline cl::Event copy(const vector<T>& data, cl::Buffer& buffer, bool block = true)
{
	cl_int err = CL_SUCCESS;
	
	const cl::CommandQueue &queue = ofxCL::Context::getCurrentContext().getCommandQueue();
	
	cl::Event event;
	err = queue.enqueueWriteBuffer(buffer,
								   block,
								   0,
								   data.size() * sizeof(T),
								   &data[0],
								   NULL,
								   &event);
	
	ofxCL::reportError(err);
	
	return event;
}

// Image

inline void convert(const ofPixels& pix, cl::Image2D& image, cl_mem_flags flag = CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR)
{
	cl_int err = CL_SUCCESS;
	
	if (!(flag & CL_MEM_USE_HOST_PTR)
		&& !(flag & CL_MEM_COPY_HOST_PTR))
	{
		ofLogError("ofxCL") << "invalid flag";
		assert(false);
	}

	cl::ImageFormat fmt;
	fmt.image_channel_data_type = CL_UNORM_INT8;
	
	switch (pix.getNumChannels())
	{
		case 1:
			fmt.image_channel_order = CL_R;
			break;
		case 4:
			fmt.image_channel_order = CL_RGBA;
			break;
		default:
			throw;
			break;
	}

	image = ::clCreateImage2D(ofxCL::Context::getCurrentContext().get()(),
							  flag,
							  &fmt,
							  pix.getWidth(),
							  pix.getHeight(),
							  pix.getWidth() * sizeof(unsigned char) * pix.getNumChannels(),
							  (void*)pix.getPixels(),
							  &err);

	ofxCL::reportError(err);
}

inline cl::Event copy(const cl::Image2D& image, ofPixels& pix, bool block = true)
{
	cl_int err = CL_SUCCESS;
	
	const cl::CommandQueue &queue = ofxCL::Context::getCurrentContext().getCommandQueue();
	
	cl::size_t<3> origin, region;
	region[0] = pix.getWidth();
	region[1] = pix.getHeight();
	region[2] = 1;
	
	cl::Event event;
	err = queue.enqueueReadImage(image,
								 block,
								 origin,
								 region,
								 0,
								 0,
								 pix.getPixels(),
								 NULL,
								 &event);
	
	ofxCL::reportError(err);
	
	return event;
}

inline cl::Event copy(const ofPixels& pix, cl::Image2D& image, bool block = true)
{
	cl_int err = CL_SUCCESS;
	
	const cl::CommandQueue &queue = ofxCL::Context::getCurrentContext().getCommandQueue();
	
	cl::size_t<3> origin, region;
	region[0] = pix.getWidth();
	region[1] = pix.getHeight();
	region[2] = 1;
	
	cl::Event event;
	err = queue.enqueueWriteImage(image,
								  block,
								  origin,
								  region,
								  0,
								  0,
								  (void*)pix.getPixels(),
								  NULL,
								  &event);
	
	ofxCL::reportError(err);
	
	return event;
}

// ImageGL

inline void convert(ofTexture& v, cl::ImageGL& image, cl_mem_flags flag = CL_MEM_READ_WRITE)
{
	cl_int err = CL_SUCCESS;
	
	if ((flag & CL_MEM_USE_HOST_PTR)
		|| (flag & CL_MEM_COPY_HOST_PTR))
	{
		ofLogError("ofxCL") << "invalid flag";
		assert(false);
	}

	ofTextureData data = v.getTextureData();
	
	v.bind();
	int level = -1;
	glGetTexParameteriv(data.textureTarget, GL_TEXTURE_BASE_LEVEL, &level);
	v.unbind();
	
	image = cl::ImageGL(ofxCL::Context::getCurrentContext().get(),
						flag,
						data.textureTarget,
						level,
						data.textureID,
						&err);
	
	ofxCL::reportError(err);
}

// BufferGL

inline void convert(GLuint bufobj, cl::BufferGL& buffer, cl_mem_flags flag = CL_MEM_READ_WRITE)
{
	cl_int err = CL_SUCCESS;
	
	if ((flag & CL_MEM_USE_HOST_PTR)
		|| (flag & CL_MEM_COPY_HOST_PTR))
	{
		ofLogError("ofxCL") << "invalid flag";
		assert(false);
	}

	buffer = cl::BufferGL(ofxCL::Context::getCurrentContext().get(),
						  flag,
						  bufobj,
						  &err);
	
	ofxCL::reportError(err);
}

OFX_CL_END_NAMESPACE