#pragma once

#include "ofxBaseGui.h"

template<typename Type>
class ofxInputField : public ofxBaseGui{
	friend class ofPanel;
	
public:	
	ofxInputField();
	~ofxInputField();
	ofxInputField(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxInputField* setup(ofParameter<Type> _val, float width = defaultWidth, float height = defaultHeight);
	ofxInputField* setup(const std::string& numEditName, Type _val, Type _min, Type _max, float width = defaultWidth, float height = defaultHeight);
	
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
	bool bGuiActive, bMousePressed;
	bool mouseInside;
	bool setValue(float mx, float my, bool bCheck);
	virtual void generateDraw();
	virtual void generateText();
	void valueChanged(Type & value);
	ofPath bg;
	ofVboMesh textMesh;

	bool bRegisteredForKeyEvents;

	std::string inputValue;
	float inputValueWidth;
	void setValue(std::string valStr);
	bool bChangedInternally;

	int mousePressedPos; //set by mouse interaction
	bool hasSelectedText();

	virtual void drawSelectedArea();
	virtual void drawCursor();
	virtual void drawFocusedBB();
	virtual void drawMesh();

	float selectStartX, selectionWidth; //calculated from select indices
	int selectStartPos, selectEndPos;
	void calculateSelectionArea(int selectIdx1, int selectIdx2);

	int pressCounter;

	void leaveFocus();
};

typedef ofxInputField<float> ofxFloatNumEdit;
typedef ofxInputField<int> ofxIntNumEdit;