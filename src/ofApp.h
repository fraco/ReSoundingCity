#pragma once

#include "ofMain.h"
#include "ofxKinectV2OSC.h"
#include "ofxGenerative.h"
#include "ofxDrawnetic.h"
#include "vertices.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
    
        void toggleVoid();
        void voidHead();
        void voidHands();
        void voidOther();
        void togglePSAlpha();
    
        void toggleTransition();
    
        void toggleBrush();
        void enableBrushStr();
    
        void toggleGenTri();
        void toggleIncrement();
    
        void toggleGAlpha();
    
        void lightUp();
        void lightOff();
		
		ofxKinectV2OSC kinect;
		Skeleton* skeleton;
		vector<Skeleton>* skeletons;
		ofTrueTypeFont smallFont, largeFont;

		BodyRenderer rendererK;
        Joint jHead, jointHandR, jointHandL, jFoL, jFoR;
        ofPoint pHead, pointHandR, pointHandL, pFoL, pFoR;
    
        //swipeVbles
        float easing;
        //HEAD
        float headx;
        float heady;
        float targetHeadx;
        float targetHeady;
        float difHeadx;
        float difHeady;
        //LEFT H
        float HaLx;
        float HaLy;
        float targetHaLx;
        float targetHaLy;
        float difHaLx;
        float difHaLy;
        //RIGHT H
        float HaRx;
        float HaRy;
        float targetHaRx;
        float targetHaRy;
        float difHaRx;
        float difHaRy;
        //LEFT F
        float FoLx;
        float FoLy;
        float targetFoLx;
        float targetFoLy;
        float difFoLx;
        float difFoLy;
        //RIGHT F
        float FoRx;
        float FoRy;
        float targetFoRx;
        float targetFoRy;
        float difFoRx;
        float difFoRy;
    
        ////VOID
    
        int width;
        int height;
        int interval;
        ofxRParticleSystem *ps;
        ofxRParticleRenderer *renderer;
        ofxDistorterBehavior *distLHand;
        ofxDamperBehavior *damper;
        //
        ofxDistorterBehavior *distHead;
        ofxDistorterBehavior *distRHand;
        //
        ofxDistorterBehavior *distLFoot;
        ofxDistorterBehavior *distRFoot;
        float expFacTr;
    
        ////BRUSHES
    
        void audioIn(float * input, int bufferSize, int nChannels);
        float *buffer;
        int inputbufferSize;
        ofSoundStream soundStream;
    
        ofPoint mouse;
        float hue;
        ofColor color;
    
        //rightBRUSH
        ofxMarking* marking;
        ofxSuperPath* path;
        ofxAudioPaint* paint;
        ofxRibbonBrush* brush;
        vector<ofxMarking *> markings;
        
        //leftBRUSH
        ofxMarking* markingL;
        ofxSuperPath* pathL;
        ofxAudioPaint* paintL;
        //ofxPerlinBrush* brushL;
        ofxRibbonBrush* brushL;
        vector<ofxMarking *> markingsL;
    
    
        ////ÆÆ

        int fc, maxDist=121;
        float num =1; //ofVec3f numD(num,num,num); //ofPoint numD;
        Vertices pdots[270];
        
        Vertices pd1, pd2, pdM; //class of dots to see if two dots are near
        
        ofMesh triangles;
        void addTriangles(vector<Vertices> otherNeighbours);
        void drawTriangles();
    
    ////
    float gAlpha;
    
protected:
        bool isVoidEnabled, isVoidHeadEnabled, isVoidHandsEnabled, isVoidOtherEnabled, isPSAlphaEnabled, isTransitionEnabled, isBrushEnabled, isGenTrianglesEnabled, isIncrementEnabled, isGAlphaEnabled;
};