#pragma once

#include "AndroidUtils.h"
#include "AndroidView.h

#pragma once

#include "ofBaseApp.h"
#include "ofEvents.h"
#include "ofConstants.h"
#include "ofxAndroidUtils.h"

class ofxAdapter {
public:
  virtual void pause(){};
  virtual void stop(){};
  virtual void resume(){};
  virtual void reloadTextures(){}
  virtual void unloadTextures(){}
  virtual void reloadGL(){reloadTextures();}
  virtual void unloadGL(){unloadTextures();}
  virtual void swipe(ofxAndroidSwipeDir swipeDir, int id){

  }
  virtual void swipe(ofxAndroidSwipeEventArgs & args){
    swipe(args.dir,args.id);
  }

  virtual bool backPressed(){
    return false;
  }

  virtual void okPressed(){};
  virtual void cancelPressed(){};
  virtual void networkConnected(bool connected){};
  virtual void networkConnectedEvent(bool & connected){
    networkConnected(connected);
  }
};