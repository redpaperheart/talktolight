//
//  MenuView
//
//  Created by Greg Schomburg on 2/21/18.
//

#include "MenuView.h"

void MenuView::setup(){
//    mScale = .5;
    highlightColor = ofFloatColor(238.0/255.0, 132.0/255.0, 255.0/255.0, 1);
    whiteColor = ofFloatColor(1, 1, 1, 1);
    dimColor = ofFloatColor(.7, .7, .7, 1);
}

void MenuView::setupLight(){
    viewType = 0; //left screen
    setup();
    
    //add header
    auto t = addItem("CHANGE THE LIGHT", "", mLineHeight*2);
    t->inactiveColor = dimColor;
    t->activeColor = whiteColor;
    t = addItem("Hey Google...", "", mLineHeight*2);
    t->inactiveColor = dimColor;
    t->activeColor = whiteColor;
}
void MenuView::setupTalk(){
    viewType = 1; //right screen
    setup();
    
    //add header
    auto t = addItem("TALK TO THE LIGHT", "", mLineHeight*2);
    t = addItem("Hey Google...", "", mLineHeight*2);
}
void MenuView::show(){
    for (auto i : mTexts) {
        i->animateToInactiveColor();
    }
}

std::shared_ptr<MenuText> MenuView::addItem( string text, string oscId, float height){
    if(viewType == 0){
        auto newText = std::make_shared<MenuText>();
        newText->font = font;
        newText->mText = text;
        newText->mOSCId = oscId;
        newText->setup();
        newText->inactiveColor = dimColor;
        newText->activeColor = highlightColor;
        newText->mPosition = ofVec2f(0, mItemYOffset);
        mItemYOffset += height;
        
        mTexts.push_back(newText);
        return newText;
    }
    if(viewType == 1){
        auto newText = std::make_shared<MenuText>();
        newText->font = font;
        newText->mText = text;
        newText->mOSCId = oscId;
        newText->mPosition = ofVec2f(0, mItemYOffset);
        newText->setup();
        newText->inactiveColor = highlightColor;
        newText->activeColor = highlightColor;
        mItemYOffset += height;
        mTexts.push_back(newText);
        return newText;
    }
}
std::shared_ptr<talkList> MenuView::addList(string oscId){
    auto list = std::make_shared<talkList>();
    list->mOSCId = oscId;
    mTalkLists.push_back(list);
    return list;
}
std::shared_ptr<MenuText> MenuView::addListItem( std::shared_ptr<talkList> list, string text, string oscId, float height){

    auto newText = std::make_shared<MenuText>();
    newText->font = font;
    newText->mText = text;
    newText->mOSCId = oscId;
    newText->mPosition = ofVec2f(0, mItemYOffset + list->mItemYOffset);
    newText->setup();
    newText->inactiveColor = highlightColor;
    newText->activeColor = highlightColor;
    
    list->mItemYOffset += height;
    list->mTexts.push_back(newText);

    cout << "addItemToList:"<< text<< " : " << list->mTexts.size() << endl;
    return newText;
}
void MenuView::activateMode(string oscId){
    for (auto i = mTexts.begin(); i != mTexts.end();) {
        if((*i)->mOSCId==oscId){
            (*i)->animateToActiveColor();
        }else{
            (*i)->animateToInactiveColor();
        }
        ++i;
    }
    for(auto i : mTalkLists){
        if(i->mOSCId == oscId){
            for (auto t : i->mTexts) {
                t->animateToInactiveColor();
            }
        }else{
            for (auto t : i->mTexts) {
                t->animateOut();
            }
        }
    }
}

void MenuView::update(){
   
}

void MenuView::draw(){
    //draw some text
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(mPosition * mScale);
    
    ofScale(mScale, mScale);
    if(debug){
        ofNoFill();
        ofDrawRectangle(0, 0, 1080, 1920);
    }
    
    ofTranslate(mMargin, mMargin);
    
    ofEnableAlphaBlending();
    for (auto i : mTexts) {
        i->draw();
    }
    for(auto i : mTalkLists){
//        cout << "mTalkLists loop:" << endl;
        for (auto t : i->mTexts) {
//            cout << "text found:" << endl;
            t->draw();
        }
    }
    ofPopStyle();
    ofPopMatrix();
}

