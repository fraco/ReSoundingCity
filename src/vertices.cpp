//
//  genTriangles.cpp
//  genTriangles
//
//  Created by Fraco OSX on 3/22/15.
//
//

#include "vertices.h"


Vertices::Vertices(){ //Vertices object CONSTRUCTOR
    dir = ofRandom(.83)>.38 ? -1 : 1;
}

void Vertices::move(){
    scal = ofMap(sin(theta), -1,1, .25,3.8);
    loc.x = org.x + sin(theta+offset)*radius*scal;
    loc.y = org.y + cos(theta+offset)*radius*scal;
    theta += (ofSignedNoise(0.0232)/7.4*dir);
    ofSetColor(ofColor::orange,150);
    ofCircle(loc, 2);
}
