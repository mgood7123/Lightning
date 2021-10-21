#include "PaintHolderOpenGLHelper.h"

QOpenGLContext *PaintHolderOpenGLHelper::getOpenGLContext() {
    return paintHolder.glContext;
}

QOpenGLFunctions *PaintHolderOpenGLHelper::getOpenGLFunctions() {
    return paintHolder.glES2Functions;
}

QOpenGLExtraFunctions *PaintHolderOpenGLHelper::getOpenGLExtraFunctions() {
    return paintHolder.glES3Functions;
}
