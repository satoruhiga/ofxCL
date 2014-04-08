#include "ofMain.h"

#include "ofxCL.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp
{
public:
	
	ofxCL::Context ocl;
	
	ofxCL::Program noiseProgram;
	
	ofImage destImage;
	vector<string> methodNames;
	ofParameter<int> methodIndex;
	
	ofParameter<int> Resolution;
	
	ofParameter<float> Scale;
	ofParameter<float> ScaleMultiplier;
	ofParameter<float> Lacunarity;
	ofParameter<float> Increment;
	ofParameter<float> Octaves;
	ofParameter<float> Amplitude;
	ofParameter<bool> AutoMove;
	ofParameter<float> AutoMoveMagnitude;
	
	ofTrueTypeFont fontSmall;
	
	ofxPanel gui;
	bool drawGui;
	
	// ---------------------------------------------------------------------------------------
	//
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
		
		ofxCL::Context::listDevices();
		
		ocl.setup( 0, true );
				
		noiseProgram.load("NoiseKernel.cl");
		
		methodNames.push_back( "GradientNoiseImage2d" );
		methodNames.push_back( "RidgedMultiFractalImage2d" );
		methodNames.push_back( "MonoFractalImage2d" );
		methodNames.push_back( "TurbulenceImage2d" );
		
		Resolution.addListener(this, &ofApp::resolutionChanged);
		
		gui.setup("Settings", "Settings.xml");

		gui.add( methodIndex.set( "methodIndex", 3, 0, methodNames.size()-1 ) );
		
		gui.add( Resolution.set( "Resolution", 1024, 2, 4096 ) );
		
		gui.add( Scale.set( "Scale", 1.0, 0.0, 1.0 ) );
		gui.add( ScaleMultiplier.set( "Scale Multiplier", 3.0, 0.001, 100.0f ) );
	
		gui.add( Lacunarity.set( "Lacunarity", 2.02f, 0.001f, 10.0f ) );
		gui.add( Increment.set( "Increment", 1.0f, -1.0, 1.0f ) );
		gui.add( Octaves.set( "Octaves", 3.3f, 0.001, 20.0f ) );
		gui.add( Amplitude.set( "Amplitude", 1.0f, 0.001, 10.0f ) );

		gui.add( AutoMove.set( "AutoMove", true ) );
		gui.add( AutoMoveMagnitude.set( "AutoMoveMagnitude", 0.5f, 0.001, 10.0f ) );
		
		drawGui = true;
				
		fontSmall.loadFont("DIN.otf", 8 );
	}
	
	// ---------------------------------------------------------------------------------------
	//
	void resolutionChanged( int &_res )
	{
		destImage.allocate( _res, _res, OF_IMAGE_COLOR );
	}
	
	// ---------------------------------------------------------------------------------------
	//
	void update()
	{
		float time = ofGetElapsedTimef();
		
		ocl.begin();
		
			cl::ImageGL cl_destImage;
			ofxCL::convert( destImage.getTextureReference(), cl_destImage );
				
			ofxCL::Kernel::Ref noiseKernel = noiseProgram.getKernel( methodNames.at(methodIndex) );
			noiseKernel->global( destImage.getWidth(), destImage.getHeight() );

			float t = time * 0.1f;
		
			// ofParameters do not seem to get passed on correctly. Todo: avoid this workaround.
			float tmpLacunarity = Lacunarity;
			float tmpIncrement = Increment;
			float tmpOctaves = Octaves;
			float tmpAmplitude = Amplitude;
		
			ofVec2f bias(0,0);
		
			if( AutoMove )
			{
				bias.x = ofSignedNoise(  t, t * -0.30f ) * AutoMoveMagnitude;
				bias.y = ofSignedNoise( -t, t *  0.33f ) * AutoMoveMagnitude;
			}
				
			float tmpScale = Scale * ScaleMultiplier;
	
			if( methodIndex == 0 )
			{
				noiseKernel->call( cl_destImage, bias, ofVec2f(tmpScale,tmpScale), tmpAmplitude );
			}
			else
			{
				noiseKernel->call( cl_destImage, bias, ofVec2f(tmpScale,tmpScale), tmpLacunarity, tmpIncrement, tmpOctaves, tmpAmplitude );
			}
			
		ocl.end();
		
	}
	
	// ---------------------------------------------------------------------------------------
	//
	void draw()
	{
		ofSetColor( ofColor::white );
		ofDisableAlphaBlending();
		destImage.draw( 0, 0, ofGetWidth(), ofGetHeight() );
		
		if( drawGui )
		{
			gui.draw();
		}

		drawStringShadowed( fontSmall,  "Method " + methodNames.at(methodIndex), 5, ofGetHeight() - 25 );
		drawStringShadowed( fontSmall,  "Scale " + ofToString( Scale * ScaleMultiplier ), 5, ofGetHeight() - 5 );
		drawStringShadowed( fontSmall, ofToString( ofGetFrameRate(), 2), ofGetWidth() - 30, ofGetHeight() - 5);
	}

	// ---------------------------------------------------------------------------------------
	//
	static void drawStringShadowed( ofTrueTypeFont& _font, string _s, float _x, float _y,
								   ofColor _frontColor = ofColor(255,255,255), ofColor _backColor = ofColor(0,0,0) )
	{
		ofSetColor( _backColor );
		_font.drawString( _s, _x + 1, _y + 1 );
		
		ofSetColor( _frontColor );
		_font.drawString( _s, _x, _y );
	}
	
};



// ---------------------------------------------------------------------------------------
//
int main(int argc, const char** argv)
{
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp);
	return 0;
}
