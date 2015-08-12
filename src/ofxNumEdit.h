#pragma once

#include "ofxBaseGui.h"
#include "ofParameter.h"

/*
 * TODO
 * floating point (multiple)
 * ENTER
 * check min/max
 * crash?
 * no key spam
 * */

template<typename Type>
class ofxNumEdit : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxNumEdit();
	~ofxNumEdit();
	ofxNumEdit(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxNumEdit* setup(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxNumEdit* setup(const std::string& numEditName, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
	void setMin(Type min);
	Type getMin();
	void setMax(Type max);
	Type getMax();

	virtual bool mouseMoved(ofMouseEventArgs & args);
	virtual bool mousePressed(ofMouseEventArgs & args);
	virtual bool mouseDragged(ofMouseEventArgs & args);
	virtual bool mouseReleased(ofMouseEventArgs & args);
    virtual bool mouseScrolled(ofMouseEventArgs & args);

    void registerKeyEvents();
    void unregisterKeyEvents();

    virtual void keyPressed(ofKeyEventArgs & args);
    virtual void keyReleased(ofKeyEventArgs & args);

	void setUpdateOnEnterOnly(bool bUpdateOnEnterOnly);

	template<class ListenerClass, typename ListenerMethod>
	void addListener(ListenerClass * listener, ListenerMethod method){
		value.addListener(listener,method);
	}

	template<class ListenerClass, typename ListenerMethod>
	void removeListener(ListenerClass * listener, ListenerMethod method){
		value.removeListener(listener,method);
	}

	double operator=(Type v);
	operator const Type & ();

	ofAbstractParameter & getParameter();

protected:
	virtual void render();
	ofParameter<Type> value;
	bool bUpdateOnEnterOnly;
	bool bGuiActive, bMousePressed;
	bool mouseInside;
	bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	virtual void generateText();
	void valueChanged(Type & value);
	ofPath bg;
	ofVboMesh textMesh;

	bool bRegisteredForKeyEvents;

	std::string valueStr;
	float valueStrWidth;
	void setValue(std::string valStr);
	bool bChangedInternally;

	int selectIdx1, selectIdx2; //set by mouse interaction
	bool hasSelectionArea(){ return selectIdx1 != selectIdx2; }

	float selectStartX, selectWidth; //calculated from select indices
	int selectStartIdx, selectEndIdx;
	void calculateSelectionArea();

	int pressCounter;
};

typedef ofxNumEdit<float> ofxFloatNumEdit;
typedef ofxNumEdit<int> ofxIntNumEdit;
