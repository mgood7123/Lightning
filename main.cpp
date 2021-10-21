#include <QGuiApplication>
#include <Lightning/Lightning.h>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    LightningWindowOpenGL w;
    w.show();

//    auto tv = new Lightning_TextView("TextView!", Qt::green);
//    w.setContentView(tv);

    auto * layer = new Lightning_Application_FPS_Layout();

    auto * table = new Lightning_LinearLayout();
    table->setOrientation(table->Horizontal);
    layer->addChild(table);

    auto * colors = new Lightning_LinearLayout();
    auto * buttons = new Lightning_LinearLayout();
    table->addChild(buttons);
    table->addChild(colors);

    colors->addChild(new Lightning_ColorView(1, 0, 0));
    buttons->addChild(new Lightning_ButtonView("red", QColor::fromRgbF(1, 0, 0)));

    colors->addChild(new Lightning_ColorView(0, 1, 0));
    buttons->addChild(new Lightning_ButtonView("green", QColor::fromRgbF(0, 1, 0)));

    colors->addChild(new Lightning_ColorView(0, 0, 1));
    buttons->addChild(new Lightning_ButtonView("blue", QColor::fromRgbF(0, 0, 1)));

    colors->addChild(new Lightning_ColorView(1, 0, 1));
    buttons->addChild(new Lightning_ButtonView("pink", QColor::fromRgbF(1, 0, 1)));

    w.setContentView(layer);

//    w.printDebugLog = true;
    return a.exec();
}
