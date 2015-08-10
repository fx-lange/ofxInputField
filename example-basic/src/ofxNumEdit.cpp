#include "ofGraphics.h"
#include "ofxNumEdit.h"
using namespace std;

template<typename Type>
ofxNumEdit<Type>::ofxNumEdit(){
	bUpdateOnEnterOnly = false;
	bGuiActive = false;
	mouseInside = false;
}

template<typename Type>
ofxNumEdit<Type>::~ofxNumEdit(){
	value.removeListener(this,&ofxNumEdit::valueChanged);
}

template<typename Type>
ofxNumEdit<Type>::ofxNumEdit(ofParameter<Type> _val, float width, float height){
	setup(_val,width,height);
}

template<typename Type>
ofxNumEdit<Type>* ofxNumEdit<Type>::setup(ofParameter<Type> _val, float width, float height){
	bUpdateOnEnterOnly = false;
	value.makeReferenceTo(_val);
	b.x = 0;
	b.y = 0;
	b.width = width;
	b.height = height;
	bGuiActive = false;
	setNeedsRedraw();

	value.addListener(this,&ofxNumEdit::valueChanged);
	registerMouseEvents();
	registerKeyEvents();
	return this;
}

template<typename Type>
ofxNumEdit<Type>* ofxNumEdit<Type>::setup(const std::string& numEditName, Type _val, Type _min, Type _max, float width, float height){
	value.set(numEditName,_val,_min,_max);
	return setup(value,width,height);
}

template<typename Type>
void ofxNumEdit<Type>::setMin(Type min){
    value.setMin(min);
}

template<typename Type>
Type ofxNumEdit<Type>::getMin(){
    return value.getMin();
}

template<typename Type>
void ofxNumEdit<Type>::setMax(Type max){
    value.setMax(max);
}

template<typename Type>
Type ofxNumEdit<Type>::getMax(){
    return value.getMax();
}

template<typename Type>
bool ofxNumEdit<Type>::mouseMoved(ofMouseEventArgs & args){
	mouseInside = isGuiDrawing() && b.inside(ofPoint(args.x,args.y));
	return mouseInside;
}

template<typename Type>
bool ofxNumEdit<Type>::mousePressed(ofMouseEventArgs & args){
//	if(bUpdateOnEnterOnly){
//		value.disableEvents();
//	}
	if(b.inside(args.x,args.y)){
		if(!bGuiActive){
			bGuiActive = true;
		}
	}else{
		if(bGuiActive){
			bGuiActive = false;
			//TODO confirm/abort?
		}
	}
	return false;
}

template<typename Type>
bool ofxNumEdit<Type>::mouseDragged(ofMouseEventArgs & args){
	return false;
}

template<typename Type>
bool ofxNumEdit<Type>::mouseReleased(ofMouseEventArgs & args){
//	if(bUpdateOnEnterOnly){
//		value.enableEvents();
//	}
	return false;
}

template<typename Type>
void ofxNumEdit<Type>::registerKeyEvents(){
	if(bRegisteredForKeyEvents == true){
		return; // already registered.
	}
	bRegisteredForKeyEvents = true;
	ofRegisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
}

template<typename Type>
void ofxNumEdit<Type>::unregisterKeyEvents(){
	if(bRegisteredForKeyEvents == false){
		return; // not registered.
	}
	ofUnregisterKeyEvents(this, OF_EVENT_ORDER_BEFORE_APP);
	bRegisteredForKeyEvents = false;
}

template<typename Type>
void ofxNumEdit<Type>::keyPressed(ofKeyEventArgs & args){
	if(bGuiActive){
		ofLogNotice("keyPressed") << args.key;
	}
}

template<typename Type>
void ofxNumEdit<Type>::keyReleased(ofKeyEventArgs & args){
	if(bGuiActive){
		ofLogNotice("keyReleased") << args.key;
	}
}

template<typename Type>
typename std::enable_if<std::is_integral<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return std::max(range,1.0);
}

template<typename Type>
typename std::enable_if<std::is_floating_point<Type>::value, Type>::type
getRange(Type min, Type max, float width){
	double range = max - min;
	range /= width*4;
	return range;
}

template<typename Type>
bool ofxNumEdit<Type>::mouseScrolled(ofMouseEventArgs & args){
	if(mouseInside){
		if(args.y>0 || args.y<0){
			double range = getRange(value.getMin(),value.getMax(),b.width);
			Type newValue = value + ofMap(args.y,-1,1,-range, range);
			newValue = ofClamp(newValue,value.getMin(),value.getMax());
			value = newValue;
		}
		return true;
	}else{
		return false;
	}
}

template<typename Type>
double ofxNumEdit<Type>::operator=(Type v){
	value = v;
	return v;
}

template<typename Type>
ofxNumEdit<Type>::operator const Type & (){
	return value;
}

template<typename Type>
void ofxNumEdit<Type>::generateDraw(){
	bg.clear();

	if(bGuiActive){
		bg.setFillColor(ofColor::white - thisBackgroundColor);
	}else{
		bg.setFillColor(thisBackgroundColor);
	}
	bg.setFilled(true);
	bg.rectangle(b);

	generateText();
}


template<typename Type>
void ofxNumEdit<Type>::generateText(){
	string valStr = ofToString(value);
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<>
void ofxNumEdit<unsigned char>::generateText(){
	string valStr = ofToString((int)value);
	textMesh = getTextMesh(getName(), b.x + textPadding, b.y + b.height / 2 + 4);
	textMesh.append(getTextMesh(valStr, b.x + b.width - textPadding - getTextBoundingBox(valStr,0,0).width, b.y + b.height / 2 + 4));
}

template<typename Type>
void ofxNumEdit<Type>::render(){
	ofColor c = ofGetStyle().color;

	bg.draw();
	if(bGuiActive){
		ofPushStyle();
		ofSetColor(255);
		ofNoFill();
		ofDrawRectangle(b);
		ofPopStyle();
	}

	ofBlendMode blendMode = ofGetStyle().blendingMode;
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableAlphaBlending();
	}
	ofSetColor(thisTextColor);

	bindFontTexture();
	textMesh.draw();
	unbindFontTexture();

	ofSetColor(c);
	if(blendMode!=OF_BLENDMODE_ALPHA){
		ofEnableBlendMode(blendMode);
	}
}


template<typename Type>
bool ofxNumEdit<Type>::setValue(float mx, float my, bool bCheck){
//	if( !isGuiDrawing() ){
//		bGuiActive = false;
//		return false;
//	}
//	if( bCheck ){
//		if( b.inside(mx, my) ){
//			bGuiActive = true;
//		}else{
//			bGuiActive = false;
//		}
//	}
//	if( bGuiActive ){
//		value = ofMap(mx, b.x, b.x + b.width, value.getMin(), value.getMax(), true);
//		return true;
//	}
	return false;
}

template<typename Type>
ofAbstractParameter & ofxNumEdit<Type>::getParameter(){
	return value;
}


template<typename Type>
void ofxNumEdit<Type>::setUpdateOnEnterOnly(bool _bUpdateOnEnterOnly){
	bUpdateOnEnterOnly = _bUpdateOnEnterOnly;
}

template<typename Type>
void ofxNumEdit<Type>::valueChanged(Type & value){
    setNeedsRedraw();
}

template class ofxNumEdit<int>;
template class ofxNumEdit<unsigned int>;
template class ofxNumEdit<float>;
template class ofxNumEdit<double>;
template class ofxNumEdit<signed char>;
template class ofxNumEdit<unsigned char>;
template class ofxNumEdit<unsigned short>;
template class ofxNumEdit<short>;
