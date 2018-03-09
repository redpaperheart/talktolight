//
//  MenuView
//
//  Created by Greg Schomburg on 2/21/18.
//

#pragma once

#include "ofMain.h"
#include "MenuText.h"
struct talkList {
    std::vector<std::shared_ptr<MenuText>> mTexts;
    string mOSCId;
    float mItemYOffset;
} ;
class MenuView
{
public:
//    void setup();
    void update();
    void draw();
    void setup();
    void show();
    void setupLight();
    void setupTalk();
    
    std::shared_ptr<MenuText> addItem( string text, string oscId, float height); //pass in the item info and mode id
    
    std::shared_ptr<talkList> addList(string oscId);
    std::shared_ptr<MenuText> addListItem( std::shared_ptr<talkList> list, string text, string oscId, float height);
    void activateMode(string oscId); //pass in the mode
    
    int viewType=0;
    
    ofVec2f mPosition;
    float mScale = 1;
    bool debug=false;
    float mMargin = 90;
    float fontSize = 50;
    float mLineHeight = fontSize * 2;
    float mItemYOffset = fontSize; //increments as items get added
    
    ofFloatColor highlightColor;
    ofFloatColor whiteColor;
    ofFloatColor dimColor;
    
    ofImage headerImage;
    
    std::shared_ptr<ofTrueTypeFont> font;
protected:
    std::vector<std::shared_ptr<MenuText>> mTexts;
    std::vector<std::shared_ptr<talkList>> mTalkLists;
};
