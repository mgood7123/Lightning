#ifndef PIXELTONDC_H
#define PIXELTONDC_H

#include <array>
#include <QDebug>


class PixelToNDC {
public:
    bool LOG_PRINT_CONVERSIONS = false;
    float width;
    float height;
    PixelToNDC() = default;
    PixelToNDC(float width, float height);
    void resize(float width, float height);

    template<typename TYPE> float inverse(TYPE num) {
        return num < 0 ? -num : -(num);
    }

    template<typename TYPE> float convert(TYPE num, TYPE num_max) {
        // 3 year old magic
        return (num - (num_max / 2)) / (num_max / 2);
    }

    template<typename TYPE>
    class Coordinates {
    public:
        TYPE x;
        TYPE y;
        TYPE z;
        Coordinates() {}
        Coordinates(TYPE x, TYPE y, TYPE z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        Coordinates(TYPE TYPE_INITIALIZER) {
            x = TYPE_INITIALIZER;
            y = TYPE_INITIALIZER;
            z = TYPE_INITIALIZER;
        }

        std::array<TYPE, 3> to_array() {
            return {x, y, z};
        };
    };

    static const int CONVERSION_ORIGIN_TOP_LEFT = 0;
    static const int CONVERSION_ORIGIN_TOP_RIGHT = 1;
    static const int CONVERSION_ORIGIN_BOTTOM_LEFT = 2;
    static const int CONVERSION_ORIGIN_BOTTOM_RIGHT = 3;

    int CONVERSION_ORIGIN = CONVERSION_ORIGIN_TOP_LEFT;

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, TYPEFROM z, bool clip) {
        return toNDC(TYPETO_INITIALIZER, x, y, z, width, height, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, TYPEFROM z, TYPEFROM w, TYPEFROM h, bool clip) {
        Coordinates<TYPETO> xyz(TYPETO_INITIALIZER);
        xyz.z = z;
        if (x > w) {
            if (LOG_PRINT_CONVERSIONS)
                qDebug() << "x is out of bounds (expected" << w << ", got" << x << ")";
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) qDebug() << "clipping to" << w;
                x = w;
            }
        } else if (x < 0) {
            if (LOG_PRINT_CONVERSIONS)
                qDebug() << "x is out of bounds (expected 0, got" << x << ")";
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) qDebug() << "clipping to 0";
                x = 0;
            }
        }
        if (y > h) {
            if (LOG_PRINT_CONVERSIONS)
                qDebug() << "x is out of bounds (expected" << h << ", got" << y << ")";
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) qDebug() << "clipping to" << h;
                y = h;
            }
        } else if (y < 0) {
            if (LOG_PRINT_CONVERSIONS)
                qDebug() << "y is out of bounds (expected 0, got" << y << ")";
            if (clip) {
                if (LOG_PRINT_CONVERSIONS) qDebug() << "clipping to 0";
                y = 0;
            }
        }
        xyz.x = convert<TYPETO>(static_cast<TYPETO>(x), static_cast<TYPETO>(w)); // x
        xyz.y = convert<TYPETO>(static_cast<TYPETO>(y), static_cast<TYPETO>(h)); // y

        switch (CONVERSION_ORIGIN) {
            case CONVERSION_ORIGIN_TOP_LEFT:
                if (LOG_PRINT_CONVERSIONS) qDebug() << "inverting 'y'";
                xyz.y = inverse<TYPETO>(xyz.y);
                break;
            case CONVERSION_ORIGIN_TOP_RIGHT:
            if (LOG_PRINT_CONVERSIONS) qDebug() << "inverting 'x'";
                xyz.x = inverse<TYPETO>(xyz.x);
                if (LOG_PRINT_CONVERSIONS) qDebug() << "inverting 'y'";
                xyz.y = inverse<TYPETO>(xyz.y);
                break;
            case CONVERSION_ORIGIN_BOTTOM_LEFT: {
                if (LOG_PRINT_CONVERSIONS) qDebug() << "no conversion";
                break;
            }
            case CONVERSION_ORIGIN_BOTTOM_RIGHT:
                if (LOG_PRINT_CONVERSIONS) qDebug() << "inverting 'x'";
                xyz.x = inverse<TYPETO>(xyz.x);
                break;
            default:
                if (LOG_PRINT_CONVERSIONS) qDebug() << "unknown conversion";
                break;
        }
        if (LOG_PRINT_CONVERSIONS) {
            qDebug() << "CONVERSION INFO:"
                             << "\nx:" << x << ", y:" << y << ", z:" << z
                             << "\nwidth:" << w << ", height:" << h
                             << "\nNormalized Device Coordinates:" << xyz;
        }
        return xyz;
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(TYPETO_INITIALIZER, x, y, 0, width, height, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPETO TYPETO_INITIALIZER, TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(TYPETO_INITIALIZER, x, y, 0, w, h, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, 0, width, height, true);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, 0, w, h, true);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM z) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, z, width, height, true);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM z, TYPEFROM w, TYPEFROM h) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, z, w, h, true);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, 0, width, height, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM w, TYPEFROM h, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, 0, w, h, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM z, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, z, width, height, clip);
    }

    template<typename TYPEFROM, typename TYPETO>
    Coordinates<TYPETO> toNDC(TYPEFROM x, TYPEFROM y, TYPEFROM z, TYPEFROM w, TYPEFROM h, bool clip) {
        return toNDC<TYPEFROM, TYPETO>(0, x, y, z, w, h, clip);
    }
};

template<typename TYPE>
QDebug operator<<(QDebug debug, const PixelToNDC::Coordinates<TYPE> &c)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << '{' << c.x << ", " << c.y << ", " << c.z << '}';
    return debug;
}

#endif // PIXELTONDC_H
