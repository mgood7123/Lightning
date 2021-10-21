QT += opengl widgets quick

CONFIG += c++11
CONFIG += warn_on
QMAKE_CXXFLAGS += "-Wreturn-type"
QMAKE_CFLAGS += "-Wreturn-type"

# sanitizer slows down execution
#CONFIG += sanitizer
#CONFIG += sanitize_address
# clang++: error: invalid argument '-fsanitize=address' not allowed with '-fsanitize=memory'
#CONFIG += sanitize_memory
# clang++: error: invalid argument '-fsanitize=address' not allowed with '-fsanitize=thread'
#CONFIG += sanitize_thread
#CONFIG += sanitize_undefined

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Lightning/Helpers/CoordinateHelper.cpp \
    Lightning/Helpers/PaintHolderOpenGLHelper.cpp \
    Lightning/Helpers/PropertyAnimatorHelper.cpp \
    Lightning/Helpers/WindowDataHelper.cpp \
    Lightning/LightningCommandList.cpp \
    Lightning/LightningEngine.cpp \
    Lightning/LightningLayouts/Lightning_Application_FPS_Layout.cpp \
    Lightning/LightningLayouts/Lightning_LayerLayout.cpp \
    Lightning/LightningLayouts/Lightning_Layout.cpp \
    Lightning/LightningLayouts/Lightning_LinearLayout.cpp \
    Lightning/LightningViews/Lightning_ButtonView.cpp \
    Lightning/LightningViews/Lightning_ColorView.cpp \
    Lightning/LightningViews/Lightning_TextView.cpp \
    Lightning/LightningViews/Lightning_View.cpp \
    Lightning/LightningWindowOpenGL.cpp \
    Lightning/LightningWindowData.cpp \
    Lightning/Tools/PixelToNDC.cpp \
    Lightning/Tools/TimeEngine.cpp \
    Lightning/Tools/chronotimer.cpp \
    Lightning/Tools/lambdathread.cpp \
    Lightning/Tools/paintholder.cpp \
    main.cpp

HEADERS += \
    Lightning/Helpers/CoordinateHelper.h \
    Lightning/Helpers/PaintHolderOpenGLHelper.h \
    Lightning/Helpers/PropertyAnimatorHelper.h \
    Lightning/Helpers/WindowDataHelper.h \
    Lightning/Lightning.h \
    Lightning/LightningCommandList.h \
    Lightning/LightningEngine.h \
    Lightning/LightningLayouts/Lightning_Application_FPS_Layout.h \
    Lightning/LightningLayouts/Lightning_LayerLayout.h \
    Lightning/LightningLayouts/Lightning_Layout.h \
    Lightning/LightningLayouts/Lightning_LinearLayout.h \
    Lightning/LightningViews/Lightning_ButtonView.h \
    Lightning/LightningViews/Lightning_ColorView.h \
    Lightning/LightningViews/Lightning_TextView.h \
    Lightning/LightningViews/Lightning_View.h \
    Lightning/LightningWindowOpenGL.h \
    Lightning/LightningWindowData.h \
    Lightning/Tools/PixelToNDC.h \
    Lightning/Tools/TimeEngine.h \
    Lightning/Tools/TypeSafeKeyValuePropertyAnimators.h \
    Lightning/Tools/TypeSafePropertyAnimators.h \
    Lightning/Tools/chronotimer.h \
    Lightning/Tools/lambdathread.h \
    Lightning/Tools/paintholder.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
