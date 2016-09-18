//
// Created by Leif Shackelford on 9/2/15.
//

// COUPLING THIS VIEW TO THE OPEN FRAMEWORKS ANDROID JAVA CLASSES

static void setAppDataDir(String data_dir);
static void init();
static void onRestart();
static void onPause();
static void onResume();
static void onStop();
static void onDestroy();
static void onSurfaceCreated();
static void onSurfaceDestroyed();
static void setup(int w, int h);
static void resize(int w, int h);
static void render();
static void exit();

static void onTouchDown(int id,float x,float y,float pressure,float majoraxis,float minoraxis,float angle);
static void onTouchDoubleTap(int id,float x,float y,float pressure);
static void onTouchUp(int id,float x,float y,float pressure,float majoraxis,float minoraxis,float angle);
static void onTouchMoved(int id,float x,float y,float pressure,float majoraxis,float minoraxis,float angle);
static void onTouchCancelled(int id,float x,float y);

static void onSwipe(int id, int swipeDir);

public static native boolean onScaleBegin(ScaleGestureDetector detector);
static void onScaleEnd(ScaleGestureDetector detector);
public static native boolean onScale(ScaleGestureDetector detector);

static void onKeyDown(int keyCode);
static void onKeyUp(int keyCode);
public static native boolean onBackPressed();

public static native boolean onMenuItemSelected(String menu_id);
public static native boolean onMenuItemChecked(String menu_id, boolean checked);

static void okPressed();
static void cancelPressed();

static void networkConnected(boolean conected);

class AndroidView {

};


#endif //NODEKITTENX_ANDROIDVIEW_H
