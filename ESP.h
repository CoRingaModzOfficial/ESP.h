#ifndef ESPCMODS_H
#define ESPCMODS_H

#include <jni.h>
#include "pixel.h"

class ESPCMODS {
private:
    JNIEnv *_env;
    jobject _cvsView;
    jobject _cvs;
	jmethodID drawline;
	jmethodID drawcircle;
	class Paint *m_LinePaint;
	class Paint *m_Paint;
	
public:
    ESPCMODS() {
        _env = nullptr;
        _cvsView = nullptr;
        _cvs = nullptr;
    }

    ESPCMODS(JNIEnv *env, jobject cvsView, jobject cvs) {
        this->_env = env;
        this->_cvsView = cvsView;
        this->_cvs = cvs;
		jclass canvasView = _env->FindClass("android/graphics/Canvas");
		drawline = _env->GetMethodID(canvasView, "drawLine","(FFFFLandroid/graphics/Paint;)V");
        drawcircle = _env->GetMethodID(canvasView, "drawCircle", "(FFFLandroid/graphics/Paint;)V");
		m_LinePaint = new Paint(this->_env);
		m_LinePaint->setStyle(Style::STROKE);
		m_LinePaint->setAntiAlias(true);
		
		m_Paint = new Paint(this->_env);
		m_Paint->setStyle(Style::FILL_AND_STROKE);
		m_Paint->setAntiAlias(true);
		
    }
	bool isValid() const {
        return (_env != nullptr && _cvsView != nullptr && _cvs != nullptr);
    }

    int getWidth() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID width = _env->GetMethodID(canvas, "getWidth", "()I");
            return _env->CallIntMethod(_cvs, width);
        }
        return 0;
    }

    int getHeight() const {
        if (isValid()) {
            jclass canvas = _env->GetObjectClass(_cvs);
            jmethodID width = _env->GetMethodID(canvas, "getHeight", "()I");
            return _env->CallIntMethod(_cvs, width);
        }
        return 0;
    }

    void drawLine(float startX, float startY, float stopX, float stopY, float thicc, int color) {
		Paint *paint = this->m_LinePaint;
		paint->setColor(color);
		paint->setStrokeWidth(thicc);
		_env->CallVoidMethod(_cvsView, drawline, startX, startY, stopX, stopY, paint->paintObj);
    }
	
	
	void drawCircle(float x, float y, float radius, float thicc, bool fill, int color) {
		Paint *paint = this->m_Paint;
		if (!fill)
        paint->setStyle(Style::STROKE);
		else
        paint->setStyle(Style::FILL);
		paint->setColor(color);
		paint->setStrokeWidth(thicc);
		_env->CallVoidMethod(_cvsView, drawcircle, x, y, radius, paint->paintObj);
	}
};
#endif

