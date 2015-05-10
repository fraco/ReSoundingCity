//
//  genTriangles.h
//  genTriangles
//
//  Created by Fraco OSX on 3/22/15.
//
//

#pragma once
#include "ofMain.h"


class Vertices {
public:
    Vertices();
    void move();
    float theta;
    int dir;
    float scal;
    
    vector<Vertices> neighbours; //vector of vertices where dist<maxDist
    float radius = ofRandom(18,83);
    float offset = ofRandom(TWO_PI);
    int edge = 100;
    ofPoint org = ofPoint(ofRandom(edge, ofGetWindowWidth()-edge), ofRandom(edge, ofGetWindowHeight()-edge));
    ofPoint loc = ofPoint(org.x+radius, org.y);
    
    void theMouse();
    ofPoint mouse;
};

