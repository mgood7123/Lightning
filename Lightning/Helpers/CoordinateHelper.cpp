#include "CoordinateHelper.h"


CoordinateHelper::CoordinateInfo::CoordinateInfo(): CoordinateInfo(0, 0, 0, 0)
{}

CoordinateHelper::CoordinateInfo::CoordinateInfo(float x1, float y1, float x2, float y2): CoordinateInfo(QRectF{x1, y1, x2, y2})
{}

CoordinateHelper::CoordinateInfo::CoordinateInfo(const QRectF &r): CoordinateInfo(QList<float>{
                                                                                  static_cast<float>(r.x()),
                                                                                  static_cast<float>(r.y()),
                                                                                  static_cast<float>(r.width()),
                                                                                  static_cast<float>(r.height())
                                                                                  })
{}

CoordinateHelper::CoordinateInfo::CoordinateInfo(const QList<float> &r):
    topLeftX(r.at(0)),
    topLeftY(r.at(1)),
    bottomRightX(r.at(2)),
    bottomRightY(r.at(3)),
    width(bottomRightX - topLeftX),
    height(bottomRightY - topLeftY)
{}

QRectF CoordinateHelper::CoordinateInfo::toQRectF() {
    return QRectF(topLeftX, topLeftY, width, height);
}

QDebug operator<<(QDebug debug, const CoordinateHelper::CoordinateInfo &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "CoordinateInfo(" << c.topLeftX << "," << c.topLeftY << " " << c.bottomRightX << "x" << c.bottomRightY << ')';
    return debug;
}
