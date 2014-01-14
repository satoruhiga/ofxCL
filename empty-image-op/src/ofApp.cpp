#include "ofMain.h"

#include "ofxCL.h"

class ofApp : public ofBaseApp
{
public:
	
	ofxCL::Context ocl;
	ofxCL::Program prog;
	
	ofTexture tex;

	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
		
		ofxCL::Context::listDevices();
		
		ocl.setup();
		prog.load("image_test.cl");
		
		tex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
	}
	
	void update()
	{
		ocl.begin();
		
		if (1) // to gl texture
		{
			cl::ImageGL cl_img;
			ofxCL::convert(tex, cl_img);
			
			ofxCL::Kernel::Ref K = prog.getKernel("random_fill");
			K->global(tex.getWidth(), tex.getHeight());
			K->call(cl_img, tex.getWidth(), tex.getHeight(), ofGetElapsedTimef());
		}
		
		if (0) // to host memory
		{
			ofPixels pix;
			pix.allocate(tex.getWidth(), tex.getHeight(), OF_IMAGE_COLOR_ALPHA);
			
			cl::Image2D cl_img;
			ofxCL::convert(pix, cl_img);
			
			ofxCL::Kernel::Ref K = prog.getKernel("random_fill");
			K->global(tex.getWidth(), tex.getHeight());
			K->call(cl_img, tex.getWidth(), tex.getHeight(), ofGetElapsedTimef());
			
			ofxCL::copy(cl_img, pix).wait();
			tex.loadData(pix);
		}
		
		ocl.end();
	}
	
	void draw()
	{
		tex.draw(0, 0);
	}

	void keyPressed(int key)
	{
	}

	void keyReleased(int key)
	{
	}
	
	void mouseMoved(int x, int y)
	{
	}

	void mouseDragged(int x, int y, int button)
	{
	}

	void mousePressed(int x, int y, int button)
	{
	}

	void mouseReleased(int x, int y, int button)
	{
	}
	
	void windowResized(int w, int h)
	{
	}
};


int main(int argc, const char** argv)
{
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp);
	return 0;
}
