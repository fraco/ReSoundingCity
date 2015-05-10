#include "ofApp.h"

float pbClr;
class ofxColorChangingBehavior : public ofxBehavior
{
    void actUpon(ofxRParticle *particle, ofVec3f &pos, ofVec3f &vel, ofVec3f &acc, float dt)
    {
        float finalColor;
        ofVec3f &home = particle->getHome();
        ofVec3f delta = home - pos;
        ofColor clr = ofColor::white;
        float len = delta.length()*2.0; //iridescence of faded force
        if (pbClr >= 57) {
            finalColor = pbClr;
        } else if (pbClr <= 56) {
            finalColor = len;
        }
        clr.setHsb(len, len, 255, finalColor); //particle color
        particle->setColor(clr);
        
    }
};

void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofHideCursor();
    
    isVoidEnabled = true;
    isVoidHeadEnabled = true;
    isVoidHandsEnabled = false;
    
    isTransitionEnabled = false;
    isPSAlphaEnabled = false;
    pbClr = 0;
    
    isBrushEnabled = false;
    
    //// KINECT
    
    smallFont.loadFont("selena.otf", 16); //http://openfontlibrary.org/en/font/selena
    largeFont.loadFont("selena.otf", 48);
    
    ofSetLineWidth(8);
    ofSetFrameRate(60);
    
    kinect.setup(12345, smallFont);
    
    skeletons = kinect.getSkeletons();
    
    rendererK.setup(skeletons);
    
    //// GENERATIVE
    float distRadius = 222;
    ps = new ofxRParticleSystem();
    renderer = ps->getRendererPtr();
    renderer->setAdditiveBlending(true);
    renderer->setPointSize(12.0);
    
    interval = 8;
    width = ofGetWidth();
    height = ofGetHeight();
    
    for(float y = 0; y <= height; y+=interval)
    {
        for(float x = 0; x <= width; x+=interval)
        {
            ps->addParticle( new ofxRParticle( ofVec3f( x, y, 0 ) ) );
        }
    }
    
    ps->setAccerationLimit(1.25);
    ps->setVelocityLimit(4);
    
    ofxHomingBehavior *homing = new ofxHomingBehavior();
    homing->setMagnitude(0.15);
    ps->addBehavior(homing);
    
    damper = new ofxDamperBehavior();
    damper->setMagnitude(0.95);
    ps->addBehavior(damper);
    //HEAD
    distHead = new ofxDistorterBehavior();
    distHead->setPosition( ofVec3f( ofGetWidth()*0.5, ofGetHeight()*0.5,0.0 ) );
    distHead->setRadius(distRadius);
    distHead->setExpFactor(1.5);
    ps->addBehavior(distHead);
    //LEFT HAND
    distLHand = new ofxDistorterBehavior();
    distLHand->setPosition( ofVec3f( ofGetWidth()*0.5, ofGetHeight()*0.5,0.0 ) );
    distLHand->setRadius(distRadius);
    distLHand->setExpFactor(1.5);
    ps->addBehavior(distLHand);
    //RIGHT HAND
    distRHand = new ofxDistorterBehavior();
    distRHand->setPosition( ofVec3f( ofGetWidth()*0.5, ofGetHeight()*0.5,0.0 ) );
    distRHand->setRadius(distRadius);
    distRHand->setExpFactor(1.5);
    ps->addBehavior(distRHand);
    //LEFT FOOT
    distLFoot = new ofxDistorterBehavior();
    distLFoot->setPosition( ofVec3f( ofGetWidth()*0.5, ofGetHeight()*0.5,0.0 ) );
    distLFoot->setRadius(distRadius);
    distLFoot->setExpFactor(1.5);
    ps->addBehavior(distLFoot);
    //RIGHT FOOT
    distRFoot = new ofxDistorterBehavior();
    distRFoot->setPosition( ofVec3f( ofGetWidth()*0.5, ofGetHeight()*0.5,0.0 ) );
    distRFoot->setRadius(distRadius);
    distRFoot->setExpFactor(1.5);
    ps->addBehavior(distRFoot);
    
    expFacTr = 0;
    
    ps->addBehavior(new ofxColorChangingBehavior());
    
    
    //// DRAWNETIC
    ofSetLineWidth(8);
    inputbufferSize = 256;
    buffer = new float[inputbufferSize];
    soundStream.setup(this, 0, 2, 44100, inputbufferSize, 2);
    
    mouse.x = ofGetMouseX();
    mouse.y = ofGetMouseY();
    
    //RGHTTBrush
    path = new ofxSuperPath();
    path->reset();
    marking = NULL;
    brush = NULL;
    paint = NULL;
    
    //LEFTBrush
    pathL = new ofxSuperPath();
    pathL->reset();
    markingL = NULL;
    brushL = NULL;
    paintL = NULL;
    
}

void ofApp::update(){
    
    ////KINECT
    kinect.update();
    
    ////GENERATIVE
    distHead->setMagnitude(distHead->getMagnitude()*0.5);
    distHead->setExpFactor(expFacTr);
    distLHand->setMagnitude(distLHand->getMagnitude()*0.5);
    distLHand->setExpFactor(expFacTr);
    distRHand->setMagnitude(distRHand->getMagnitude()*0.5);
    distRHand->setExpFactor(expFacTr);
    
    distLFoot->setMagnitude(distLFoot->getMagnitude()*0.5);
    distRFoot->setMagnitude(distRFoot->getMagnitude()*0.5);
    
    ps->update();
    
    if (isTransitionEnabled) {
        expFacTr = expFacTr - .005;
        if (expFacTr <= 1.0) {
            expFacTr = .95;
        }
    } else if (isTransitionEnabled == false){
        expFacTr = expFacTr + .0075;
        if (expFacTr >= 2.0){
            expFacTr = 2.0;
        }
    }
    if (isPSAlphaEnabled) {
        pbClr += 3;
        if (pbClr >= 255) {
            pbClr = 255;
        }
    } else if (isPSAlphaEnabled == false) {
        pbClr -= 5;
        if (pbClr <= 0) {
            pbClr = 0;
        }
    }
    
    ////DRAWNETIC
    
    //RIGHT
    for(int i = 0; i < markings.size(); i++) { markings[i]->update(); }
    //LEFT
    for(int i = 0; i < markingsL.size(); i++) { markingsL[i]->update(); }
    
    ////FOREGROUND
    if (isGAlphaEnabled) {
        gAlpha += 3;
        if (gAlpha >= 255) {
            gAlpha = 255;
        }
    } else if (isGAlphaEnabled == false) {
        gAlpha -= 5;
        if (gAlpha <= 0) {
            gAlpha = 0;
        }
    }
    
}

void ofApp::draw(){
    
    ofBackground(ofColor::black);
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    
    ////KINECT V2
    kinect.drawDebug();
    
    smallFont.drawString("fps:\n" + ofToString(ofGetFrameRate()), 20, ofGetHeight() - 100);
    
    for(int i = 0; i < skeletons->size(); i++) {
        skeleton = &skeletons->at(i);
        
        jHead = skeletons->at(i).getHead();
        jointHandR = skeletons->at(i).getHandRight();
        jointHandL = skeletons->at(i).getHandLeft();
        jFoL = skeletons->at(i).getFootLeft();
        jFoR = skeletons->at(i).getFootRight();
        
        pHead = jHead.getPoint();
        pointHandR = jointHandR.getPoint();
        pointHandL = jointHandL.getPoint();
        pFoL = jFoL.getPoint();
        pFoR = jFoR.getPoint();
        
        easing = 0.075;
        
        ////GENERATIVE
        
        if(isVoidEnabled){
            
            //ÐHEADÐ\\
            
            int a = .75;
            int b = 50;
            targetHeadx = pHead.x;
            difHeadx = targetHeadx - headx;
            if(abs(difHeadx)>a){
                headx += difHeadx * easing;
            }
            targetHeady = pHead.y;
            difHeady = targetHeady - heady;
            if(abs(difHeady)>a){
                heady += difHeady * easing;
            }
            if (abs(difHeadx)>b || abs(difHeady)>b){
                distHead->setMagnitude(222);
                distHead->setPosition(ofVec3f(pHead.x, pHead.y));
            }
            if(isVoidHeadEnabled){   }
            
            //ÑHANDSÑ\\
            
            if(isVoidHandsEnabled){
                ///ÐLeft handÐ\\\
                
                targetHaLx = pointHandL.x;
                difHaLx = targetHaLx - HaLx;
                if(abs(difHaLx)>a){
                    HaLx += difHaLx * easing;
                }
                targetHaLy = pointHandL.y;
                difHaLy = targetHaLy - HaLy;
                if(abs(difHaLy)>a){
                    HaLy += difHaLy * easing;
                }
                if (abs(difHaLx)>b || abs(difHaLy)>b){
                    distLHand->setMagnitude(222);
                    distLHand->setPosition(ofVec3f(pointHandL.x, pointHandL.y));
                }
                
                ///ÐRIGHT handÐ\\\
                
                targetHaRx = pointHandR.x;
                difHaRx = targetHaRx - HaRx;
                if(abs(difHaRx)>a){
                    HaRx += difHaRx * easing;
                }
                targetHaRy = pointHandR.y;
                difHaRy = targetHaRy - HaRy;
                if(abs(difHaRy)>a){
                    HaRy += difHaRy * easing;
                }
                if (abs(difHaRx)>b || abs(difHaRy)>b){
                    distRHand->setMagnitude(222);
                    distRHand->setPosition(ofVec3f(pointHandR.x, pointHandR.y));
                }
            }
            
            //ÐOTHER2Ð\\
            
            if (isVoidOtherEnabled) {
                //        ///ÑLEFT footÑ\\\
                //
                //        targetFoLx = pFoL.x;
                //        difFoLx = targetFoLx - FoLx;
                //        if(abs(difFoLx)>1.25){
                //            FoLx += difFoLx * easing;
                //        }
                //        targetFoLy = pFoL.y;
                //        difFoLy = targetFoLy - FoLy;
                //        if(abs(difHaLy)>1.25){
                //            FoLy += difFoLy * easing;
                //        }
                //        if (abs(difFoLx)>100 || abs(difFoLy)>100){
                //            distLFoot->setMagnitude(222);
                //            distLFoot->setPosition(ofVec3f(pFoL.x, pFoL.y));
                //        }
                //
                //        ///ÑRIGHT footÑ\\\
                //
                //        targetFoRx = pFoR.x;
                //        difFoRx = targetFoRx - FoRx;
                //        if(abs(difFoRx)>1.25){
                //            FoRx += difFoRx * easing;
                //        }
                //        targetFoRy = pFoR.y;
                //        difFoRy = targetFoRy - FoRy;
                //        if(abs(difHaRy)>1.25){
                //            FoRy += difFoRy * easing;
                //        }
                //        if (abs(difFoRx)>100 || abs(difFoRy)>100){
                //            distRFoot->setMagnitude(222);
                //            distRFoot->setPosition(ofVec3f(pFoR.x, pFoR.y));
                //        }
            }
        }
    }
    
    ////ÐDRAWNETIC
    if(isBrushEnabled){
        
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        rendererK.draw();
        ofHideCursor();
        
        //RIGHT BRUSH
        for(int i = 0; i < markings.size(); i++) { markings[i]->draw(); }
        if(marking != NULL) { marking->draw(); }
        
        //LEFT BRUSH
        for(int i = 0; i < markingsL.size(); i++) { markingsL[i]->draw(); }
        if(markingL != NULL) { markingL->draw(); }
        
        //        //lowerRIGHT BRUSH
        //        for(int i = 0; i < markingsLR.size(); i++) { markingsLR[i]->draw(); }
        //        if(markingLR != NULL) { markingLR->draw(); }
        //
        //        //lowerLEFT BRUSH
        //        for(int i = 0; i < markingsLL.size(); i++) { markingsLL[i]->draw(); }
        //        if(markingLL != NULL) { markingLL->draw(); }
    }
    
    if(isVoidEnabled){
        ps->draw();
    }
    
    ////ÑÆÆ
    
    float incNum = 0;
    if(isGenTrianglesEnabled){
        
        
        incNum+=0.05;
        
        
        if (num<101) {
            num = num + incNum;
        } else {
            num = num;
        }
//        else if (num>101){
//            incNum = 101;
//        }
        for (int i = 0; i<num; i++) {
            float radius = ofMap(sin(4*ofGetElapsedTimef()), -1, 1, 2, 12);
            ofNoFill();
            pdots[i].move();
            if(i==0){
                pdots[i].loc = pointHandL;
                ofCircle(pointHandL, radius);
            }
            if(i==1){
                pdots[i].loc = pointHandR;
                ofCircle(pointHandR, radius);
            }
            if(i==2){
                pdots[i].loc = pHead;
                ofCircle(pHead, radius*1.5);
            }
            //        if(i<6){
            //            float theta = ofMap(i, 0, 6, 0, TWO_PI);
            //            float radius = ofMap(sin(8*ofGetElapsedTimef()), -1, 1, 80, 120);
            //            pdots[i].loc.x=mouseX + sin(theta) * radius;
            //            pdots[i].loc.y=mouseY + cos(theta) * radius;
            //        }
            pd1 = pdots[i];
            pd1.neighbours.push_back(pd1);
            
            //iterating the second class of pointDots
            for (int j = i+1; j<num; j++) {
                pd2 = pdots[j];
                
                //comparing location of both classes of pointDots
                float d = pd1.loc.distance(pd2.loc);
                
                if (d>0 && d<maxDist){
                    pd1.neighbours.push_back(pd2);
                }
            }
            //drawing triangles
            if (pd1.neighbours.size()>1) {
                addTriangles(pd1.neighbours);
            }
        }
        drawTriangles();
    } else if (isGenTrianglesEnabled == false){
        incNum = 0;
    }
    
    ////FOREGROUND
//    if(isGAlphaEnabled){
//        ofDisableAlphaBlending();
//    }
//        ofSetColor(0,gAlpha);
//        ofRect(0, 0, ofGetWidth(), ofGetHeight());
//        cout<< gAlpha<<endl;
    
    
}

void ofApp::exit(){
    ////GENERATIVE
    delete ps;
    
    ////DRAWNETIC

    //RIGHTBrush
    for(int i = 0; i < markings.size(); i++) { delete markings[i]; }
    markings.clear();
    if(marking != NULL) { delete marking; }
    
    //LEFTBrush
    for(int i = 0; i < markingsL.size(); i++) { delete markingsL[i]; }
    markingsL.clear();
    if(markingL != NULL) { delete markingL; }
    
    delete[] buffer;
}

void ofApp::keyPressed(int key){
    if(key == '1') toggleVoid();
    if(key == 'q') togglePSAlpha();
    if(key == 'a') voidHands();
    if(key == 'z') toggleTransition();
    
    if(key == '2') toggleBrush();
    if(key == 'x') ;
    
    if(key == '4') toggleGenTri();
    if(num<270 && key == 'f'){
        num++;
    } else if (key == 'v'){
        num--;
    }
    
    if(key == 'j') rendererK.toggleJoints();
    
    if(key == 'k') ofSetFullscreen(true);
    
    if(key == 'l') toggleGAlpha();
}

void ofApp::keyReleased(int key){
}

void ofApp::mouseMoved(int x, int y ){
}

void ofApp::mouseDragged(int x, int y, int button){
    ////DRAWNETIC
    
    hue = fmodf(ofGetElapsedTimef() * 10, 255);
    color.setHsb(hue, 255, 255);
    
    //RIGHTBrush
    path->lineTo(pointHandR.x, pointHandR.y,0, ofFloatColor(0), ofGetFrameNum(), 0);
    //LEFTBrush
    pathL->lineTo(pointHandL.x, pointHandL.y,0, ofFloatColor(0), ofGetFrameNum(), 0);
    
}

void ofApp::mousePressed(int x, int y, int button){
    ////DRAWNETIC
    
    //RIGHTBrush
    path = new ofxSuperPath();
    paint = new ofxAudioPaint(path, ofColor(color), buffer, inputbufferSize);
    paint->setEffectHue(true);
    paint->setRange(50);
    brush = new ofxRibbonBrush(path, paint);
    brush->setDynamic(true);
    brush->setSpeed(6);
    brush->setUseAcceleration(false);
    marking = new ofxMarking(path, paint, brush);
    path->reset();
    path->lineStart(pointHandR.x,pointHandR.y ,0, ofFloatColor(0), ofGetFrameNum(), 0);
    
    //LEFTBrush
    pathL = new ofxSuperPath();
    paintL = new ofxAudioPaint(pathL, ofColor(color), buffer, inputbufferSize);
    paintL->setEffectHue(true);
    paintL->setRange(50);
    brushL = new ofxRibbonBrush(pathL, paintL);
    brushL->setDynamic(true);
    brushL->setSpeed(6);
    brushL->setUseAcceleration(false);
    markingL = new ofxMarking(pathL, paintL, brushL);
    pathL->reset();
    pathL->lineStart(pointHandL.x,pointHandL.y ,0, ofFloatColor(0), ofGetFrameNum(), 0);
    
    
}

void ofApp::mouseReleased(int x, int y, int button){
    ////DRAWNETIC

    //RIGHTBrush
    path->lineEnd(pointHandR.x, pointHandR.y, 0, ofGetFrameNum(), -1, 0);
    markings.push_back(marking);
    marking = NULL;
    
    //LEFTBrush
    pathL->lineEnd(pointHandL.x, pointHandL.y, 0, ofGetFrameNum(), -1, 0);
    markingsL.push_back(markingL);
    markingL = NULL;
    
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels) {
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = input[i*nChannels] + input[i*nChannels +1];
    }
}

void ofApp::drawTriangles(){
    ofSetColor(ofColor::tan, 12);
    ofFill();
    triangles.drawFaces();
    ofSetColor(ofColor::tan, 18);
    ofSetLineWidth(.5);
    triangles.drawWireframe();
    triangles.clear();
}

void ofApp::addTriangles(vector<Vertices> otherNeighbours) {
    int s = otherNeighbours.size();
    if (s > 2) {
        for (int i = 1; i < s-1; i ++) {
            for (int j = i+1; j < s; j ++) {
                triangles.addVertex(otherNeighbours[0].loc);
                triangles.addVertex(otherNeighbours[i].loc);
                triangles.addVertex(otherNeighbours[j].loc);
            }
        }
    }
}

void ofApp::toggleVoid() {
    isVoidEnabled = !isVoidEnabled;
}
void ofApp::voidHead() {
    isVoidHeadEnabled = !isVoidHeadEnabled;
}
void ofApp::voidHands() {
    isVoidHandsEnabled = !isVoidHandsEnabled;
}
void ofApp::voidOther() {
    isVoidOtherEnabled = !isVoidOtherEnabled;
}

void ofApp::togglePSAlpha(){
    isPSAlphaEnabled = !isPSAlphaEnabled;
}
void ofApp::toggleTransition(){
    isTransitionEnabled = !isTransitionEnabled;
}

void ofApp::toggleBrush(){
    isBrushEnabled = !isBrushEnabled;
}

void ofApp::toggleGenTri(){
    isGenTrianglesEnabled = !isGenTrianglesEnabled;
}

void ofApp::toggleGAlpha(){
    isGAlphaEnabled = !isGAlphaEnabled;
}

