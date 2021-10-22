#include <QGuiApplication>
#include <Lightning/Lightning.h>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    LightningWindowOpenGL w;
    w.show();

    auto * debug = new Lightning_DebugLayout();

    auto * table = new Lightning_LinearLayout();
    table->setOrientation(table->Horizontal);
    debug->addChild(table);

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

    w.setContentView(debug);

//    w.printDebugLog = true;

    return a.exec();
}
