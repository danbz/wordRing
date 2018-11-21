#include "ofApp.h"




//--------------------------------------------------------------
void ofApp::setup() {
    // load the font
    // font.load("sans-serif", 18);
    font.load("monospace", 18);
    // font.load("sans-serif", 18); // use different typefaces
    
    font.setGlobalDpi(144);
    sortTypeInfo = "no sort";
    
    // load the txt document into a ofBuffer
    ofBuffer buffer = ofBufferFromFile("freshprince.txt");
    string   content = buffer.getText();
    setupWords(content);
    
    // set remote loading from web URL parameters
    loading=false;
    ofRegisterURLNotification(this);
    
    // set up the 3d autorotate parameters
    autoRotateDeg = 0.0f;
    rotateStep = 0.1f; // autorotate speed
    b_autoRotate = false;
    b_showGui = false;
    
    // set DOF parameters
    depthOfField.setup(ofGetWidth(), ofGetHeight());
    focalDist = 150;
    focalRange = 50;
    //cout << "focal dist: " << depthOfField.getFocalDistance() << " focal range: " << depthOfField.getFocalRange() << endl;
    
}

//--------------------------------------------------------------
void ofApp::update() {
    
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetBackgroundColor(0);
    
    
    depthOfField.begin();
   // cam.begin();
    cam.begin(depthOfField.getDimensions());
    //ofPushStyle();
    ofPushMatrix();
    ofSetColor(255,0,0); // set the color to red to draw the first word
    
    
    float radius = 350;
    
    for(unsigned int i=0; i<words.size()/2; i++) {
        float t = -HALF_PI + ofMap(i, 0, (words.size()/2), 0, TWO_PI);
        float x = cos( t ) * radius;
        float y = sin( t ) * radius;
        float a = ofRadToDeg(atan2(y, x));
        
        ofPushMatrix();
        
        // use autorotate counter to rotate in z and y axes
        ofRotateZDeg(autoRotateDeg*2.0); // autorotate the word circle
        ofRotateYDeg(autoRotateDeg); // autorotate the word circle
        
        ofTranslate(x, y );
        ofRotateZDeg(a );
        float scl = 1;
        glScalef(scl, scl, scl);
        font.drawString(words[i].word, 0, 20);
        ofPopMatrix();
        
        ofSetColor(255); // set all the rest of the words to white
    }
    
    ofSetColor(100);
    font.drawString(sortTypeInfo, -(font.stringWidth(sortTypeInfo)/2), 0);
    
    ofPopMatrix();
    //ofPopStyle();
    cam.end();
    depthOfField.end();
    
    
    // if we are using autorotate then increment the amount to rotate by
    if (b_autoRotate){
        if (autoRotateDeg < 360){
            autoRotateDeg += rotateStep;
        } else {
            autoRotateDeg =0; // if it has rotated 360 steps then start again at 0
        }
    }
    
    if(ofGetKeyPressed(' ')){
        depthOfField.drawFocusAssist(0, 0);
    }
    else{
        depthOfField.getFbo().draw(0, 0);
    }
    
    // instruction
    ofSetColor(200);
    if (b_showGui){
        ofDrawBitmapString("\nPress 1 for no sort\nPress 2 for alphabetical\nPress 3 for word length\nPress 4 for word occurrence\nr to autorotate\nw to load text from the web\nl to load a txt file form disk \nf for fullscreen \nfocalDistance: " + ofToString( focalDist ) + " focalRange: " + ofToString( focalRange ) , 20, 20);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    ofFileDialogResult openFileResult;
    ofHttpResponse loadResult;
    
    switch (key) {
        case '1':
            // sort raw
            sortTypeInfo = "no sort";
            break;
        case '2':
            // sort alphabetically

            sortTypeInfo = "sorting alphabetically";
            ofSort(words, ofApp::sortOnABC);
            break;
            
        case '3':
            // sort by length of word
            sortTypeInfo = "sorting on word length";
            ofSort(words, ofApp::sortOnLength);
            break;
            
        case '4':
            // sort by length of word
            sortTypeInfo = "sorting on word occurrences";
            ofSort(words, ofApp::sortOnOccurrences);
            break;
            
        case 'l':
            
            //Open the Open File Dialog to load text file
            openFileResult = ofSystemLoadDialog("Select a txt file");
            
            //Check if the user opened a file
            if (openFileResult.bSuccess){

                ofLogVerbose("User selected a file");

                //We have a file, check it and process it
                processOpenFileSelection(openFileResult);

            }else {
                ofLogVerbose("User hit cancel");
            }
            
            break;
            
        case 'w':
            
            loadResult = ofLoadURL("http://www.google.com/robots.txt");
            cout << "my awesome loadresult: " << endl;
            // cout << "my awesome loadresult: " << ofToString( loadResult.data) << endl;
            
            urlResponse(loadResult);
            break;
            
        case 'r':
            b_autoRotate=!b_autoRotate;
            break;
            
        case 'f':
            ofToggleFullscreen();
            depthOfField.setup(ofGetWidth(), ofGetHeight());
            break;
            
        case 'g':
            b_showGui =!b_showGui;
            break;
            
        case OF_KEY_UP:
            focalDist +=1;
            depthOfField.setFocalDistance(focalDist);
            break;
            
        case OF_KEY_DOWN:
            if (focalDist>0) focalDist -=1;
            depthOfField.setFocalDistance(focalDist);
            break;
            
        case OF_KEY_RIGHT:
            focalRange+=1;
            depthOfField.setFocalRange( focalRange );
            break;
            
        case OF_KEY_LEFT:
            if (focalRange>0) focalRange-=1;
            depthOfField.setFocalRange( focalRange );
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
//--------------------------------------------------------------

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    
    ofFile file (openFileResult.getPath());
    
    if (file.exists()){
        
        ofLogVerbose("The file exists - now checking the type via file extension");
        string fileExtension = ofToUpper(file.getExtension());
        
        //We only want text
        if (fileExtension == "TXT") {
            
            // load the txt document into an ofBuffer
            ofBuffer buffer = ofBufferFromFile(file);
            string   content = buffer.getText();
            setupWords(content);
            
        }
    }
    
}
//--------------------------------------------------------------

void ofApp::setupWords(string content){
    
    // take our text and process into a vector of words
    words.clear();
    
    // take the content and split it up by spaces
    // we need to also turn new lines into spaces so we can seperate words on new lines as well
    ofStringReplace(content, "\r", " ");
    ofStringReplace(content, "\n", " ");
    
    vector <string> splitString = ofSplitString(content, " ", true, true);
    
    // copy over the words to our object
    for (unsigned int i=0; i<splitString.size(); i++) {
        LyricWord wrd;
        wrd.occurrences = 1;
        wrd.word = ofToLower( splitString[i] );
        words.push_back(wrd);
    }
    
    // clean up the words removing any
    // characters that we do not want
    for (unsigned int i=0; i<words.size(); i++) {
        // run throught this ignore list and replace
        // that char with nothing
        char ignoreList[12] = {',', '.', '(', ')', '?', '!', '-', ':', '"', '\'', '\n', '\t'};
        for(int j=0; j<12; j++) {
            
            // make string from char
            string removeStr;
            removeStr += ignoreList[j];
            
            // remove and of the chars found
            ofStringReplace(words[i].word, removeStr, "");
        }
    }
    
    // count the amount of times that we see a word
    for (unsigned int i=0; i<words.size(); i++) {
        int c = 1;
        for (unsigned int j=0; j<words.size(); j++) {
            if(words[i].word == words[j].word) c ++;
        }
        words[i].occurrences = c;
    }
    
    // remove duplicates of the words
    vector<LyricWord>tempWord;
    for (unsigned int i=0; i<words.size(); i++) {
        bool bAdd = true;
        for(unsigned int j=0; j<tempWord.size(); j++) {
            if(words[i].word == tempWord[j].word) bAdd = false;
        }
        
        if(bAdd) {
            tempWord.push_back(words[i]);
        }
    }
    
    words = tempWord;
    
    // remove word we do not want
    ofRemove(words, ofApp::removeWordIf);
}

//--------------------------------------------------------------


void ofApp::urlResponse(ofHttpResponse & response){
    if(response.status==200 ){
        // if our web request works the set up the text returned
        string   content = response.data;
        cout << "my awesome response request name parsed" << ofToString( response.request.name)  << endl;
        setupWords(content);
        
        loading=false;
    }else{
        
        cout << response.status << " " << response.error << " for request " << response.request.name << endl;
        if(response.status!=-1) loading=false;
    }
}


// sort on abc's
//--------------------------------------------------------------
bool ofApp::sortOnABC(const LyricWord &a, const LyricWord &b) {
    return a.word < b.word;
}

// sort on word length
//--------------------------------------------------------------
bool ofApp::sortOnLength(const LyricWord &a, const LyricWord &b) {
    return (int)a.word.size() > (int)b.word.size();
}

// sort on occurrences
//--------------------------------------------------------------
bool ofApp::sortOnOccurrences(const LyricWord &a, const LyricWord &b) {
    return a.occurrences > b.occurrences;
}


// remove function
//--------------------------------------------------------------
bool ofApp::removeWordIf(LyricWord &wrd) {
    
    bool bRemove = false;
    static string ignoreWords[11] = {"the", "to", "of", "a", "and", "i", "it", "if", "is", "in", "be"};
    
    // if this word empty
    if(wrd.word.empty()) bRemove = true;
    
    // are we a word that we do now want
    for (int j=0; j<11; j++) {
        if(wrd.word == ignoreWords[j]) {
            bRemove = true;
            break;
        }
    }
    
    return bRemove;
}
