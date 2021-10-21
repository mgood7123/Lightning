#ifndef PAINTHOLDEROPENGLHELPER_H
#define PAINTHOLDEROPENGLHELPER_H

#include "../Tools/paintholder.h"

class PaintHolderOpenGLHelper
{
public:
    PaintHolder paintHolder;

    QOpenGLContext * getOpenGLContext();

    QOpenGLFunctions * getOpenGLFunctions();

    QOpenGLExtraFunctions * getOpenGLExtraFunctions();
};

#endif // PAINTHOLDEROPENGLHELPER_H
