/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

#ifndef ofeliaWindow_h
#define ofeliaWindow_h

#pragma once

#include "ofeliaBase.h"
#include "ofeliaOrien.h"

extern "C" {
    
    /* window object class */
    static t_class *ofeliaWindow_class;
    
    typedef struct _ofeliaWindow
    {
        t_object x_obj;
        void *x_ofeliaWindow;
        static const char *objName;
        static t_symbol *firstLoopSym;
        static t_symbol *firstLoopMess;
        static t_symbol *initSym;
        static t_symbol *initMess;
        static t_symbol *updateSym;
        static t_symbol *updateMess;
        static t_symbol *drawSym;
        static t_symbol *drawMess;
        static t_symbol *exitSym;
        static t_symbol *exitMess;
        static bool bRenderGate;
        
    } t_ofeliaWindow;
    
    void ofeliaWindow_setup();
}

class ofeliaWindow {
    
public:
    
    /* class methods */
    bool _new(t_symbol *s, int argc, t_atom *argv);
    void _firstLoop();
    void _create();
    void _destroy();
    void _reset();
    void _dimen(const t_symbol *s, const int argc, const t_atom *argv);
    void _pos(const t_symbol *s, const int argc, const t_atom *argv);
    void _scale(const t_floatarg f);
    void _bgColor(const t_symbol *s, const int argc, const t_atom *argv);
    void _frameRate(const t_floatarg f);
    void _VSync(const t_symbol *s, const int argc, const t_atom *argv);
    void _depthTest(const t_symbol *s, const int argc, const t_atom *argv);
    void _orien(const t_symbol *s, const int argc, const t_atom *argv);
    void _orienLock(const t_symbol *s, const int argc, const t_atom *argv);
    void _accel(const t_symbol *s, const int argc, const t_atom *argv);
    void _accelSmoothing(const t_floatarg f);
    void _cursor(const t_symbol *s, const int argc, const t_atom *argv);
    void _fullscreen(const t_symbol *s, const int argc, const t_atom *argv);
    void _hotkeys(const t_symbol *s, const int argc, const t_atom *argv);
    void _title(const t_symbol *s, const int argc, const t_atom *argv);
    void _set(const t_symbol *s, const int argc, const t_atom *argv);
    void _print();
    void _free();
    
    /* class variables */
    t_ofeliaWindow *x;
    
    /* static variables */
    static ofAppBaseWindow *window;
    static GLFWwindow *GLFWwin;
    static bool bFirstUpdate;
    static bool bFirstDraw;
    static bool bWindowExists;
    static int retinaScale;
    static float scaleFactor;
    static bool bDepthTestEnabled;
    static bool bFullscreenMode;
    static ofVec2f fullscreenOffset;
    
private:
    
    /* enumerations */
    enum RotationLockMode {
        
        WINDOW_ROTATION_LOCK_FREE,
        WINDOW_ROTATION_LOCK_SOFT,
        WINDOW_ROTATION_LOCK_HARD
    };
    enum Orientation {
        
        WINDOW_ORIENTATION_DEFAULT,
        WINDOW_ORIENTATION_RIGHT,
        WINDOW_ORIENTATION_INVERT,
        WINDOW_ORIENTATION_LEFT
    };
    enum ScaleDirection {
        
        WINDOW_SCALE_DIR_HORIZONTAL,
        WINDOW_SCALE_DIR_VERTICAL
    };
    enum TouchState {
        
        WINDOW_TOUCH_DOWN,
        WINDOW_TOUCH_MOVED,
        WINDOW_TOUCH_UP
    };
    enum MouseState {
        
        WINDOW_MOUSE_PRESSED,
        WINDOW_MOUSE_MOVED,
        WINDOW_MOUSE_RELEASED
    };
    /* class methods */
    bool getWindowPosFromAlignModes(const char *horMode, const char *verMode, int &posX, int &posY);
    bool getWindowElemFromArgs(const int argc, const t_atom *argv, int &width, int &height,
                               int &posX, int &posY, float &scale, ofColor &bgColor);
    void initVariables();
    void setWindowProperties();
    void createWindow();
    void destroyWindow();
    void bindGettersValueSymbols();
    void unbindGettersValueSymbols();
    bool setDeviceDimensions(const int dWidth, const int dHeight);
    bool setWindowScale(const float wScale);
    void setWindowPosition(const int posX, const int posY);
    void setFrameRate(const int fRate);
    void setDeviceRotationLockMode(const int newLockMode,
                                   int &currentOrientation);
    void setDeviceOrientation(int newOrientation);
    void setCursorMode(const bool cMode);
    void setFullscreenMode(const bool fMode);
    void swapWindowDimensions();
    void addWindowListeners();
    void removeWindowListeners();
    
    inline void calibrateMousePosition(ofMouseEventArgs &e)
    {
        if (bFullscreenMode)
            e -= fullscreenOffset;
        e /= scaleFactor;
        
        if (orientation != WINDOW_ORIENTATION_DEFAULT && lockMode != WINDOW_ROTATION_LOCK_FREE) {
            
            e -= myOrien.centerPoint;
            e.rotate(-myOrien.degree);
            e -= myOrien.restorePoint;
        }
    }
    /* event listeners */
    void windowUpdate(ofEventArgs &e);
    void windowDraw(ofEventArgs &e);
    void windowMousePressed(ofMouseEventArgs &e);
    void windowMouseMoved(ofMouseEventArgs &e);
    void windowMouseDragged(ofMouseEventArgs &e);
    void windowMouseReleased(ofMouseEventArgs &e);
    void windowMouseScrolled(ofMouseEventArgs &e);
    void windowKeyPressed(ofKeyEventArgs &e);
    void windowKeyReleased(ofKeyEventArgs &e);
    void windowResized(ofResizeEventArgs &e);
    void windowDestroyed(ofEventArgs &e);
    
    /* static methods */
    static void resizeWindow();
    static void pollEventsMethod(void *nul);
    static void windowCreatedMethod(void *nul);
    static void sendTouchToPd(const int touchState, const int touchID, const float posX, const float posY);
    static void sendMouseToPd(const int mouseState, const int button, const float posX, const float posY);
    static void sendScrollToPd(const float scrollX, const float scrollY);
    static void sendKeyToPd(const bool pressed, const int key);
    static void sendScaleToPd(const float wScale);
    static void sendFullscreenToPd(const bool fMode);
    static void sendFocusToPd(GLFWwindow* window, const int focused);
    static void sendPosToPd(GLFWwindow* window, const int posX, const int posY);
    static void sendWindowToPd(const bool wMode);
    static void sendBackToPd();
    
    /* class variables */
    int argWindowWidth;
    int argWindowHeight;
    int argWindowPosX;
    int argWindowPosY;
    float argWindowScale;
    ofColor argBackgroundColor;
    int initWindowWidth;
    int initWindowHeight;
    int initWindowPosX;
    int initWindowPosY;
    int screenWidth;
    int screenHeight;
    float initWindowScale;
    float windowScaleStored;
    bool bScaleDirection;
    int frameRate;
    int lockMode;
    ofeliaOrien myOrien;
    int orientation = WINDOW_ORIENTATION_DEFAULT;
    int initOrientation;
    bool bCursorMode;
    bool bVSyncEnabled;
    ofColor backgroundColor;
    t_string title;
    bool bFullscreenMaskPathSet;
    int touchID;
    int touchState;
    float mouseX;
    float mouseY;
    bool bWindowOwner;
    bool bFullscreenScheduled;
    
    /* static variables */
    static bool bFirstLoop;
    static bool bWindowCreatedSent;
    static int windowPosX;
    static int windowPosY;
    static int windowWidth;
    static int windowHeight;
    static float windowScale;
    static int widthScaled;
    static int heightScaled;
    static int windowResizeCount;
    static bool bWindowResizeLocked;
    static bool bScaleDirectionFixed;
    static t_clock *pollEventsClock;
    static t_clock *windowCreatedClock;
    static GLFWcursor *blankCursor;
    static unique_ptr<ofPath> fullscreenMaskPath;
    static unique_ptr<ofVboMesh> fullscreenMaskMesh;
};

#endif /* ofeliaWindow_h */
