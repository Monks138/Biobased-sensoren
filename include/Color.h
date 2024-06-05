//
// Created by Merij on 6/5/2024.
//

#ifndef BIOBASED_SENSOREN_COLOR_H
#define BIOBASED_SENSOREN_COLOR_H

struct Color {
    int red;
    int green;
    int blue;
};

namespace Colors {
    const Color Red = {255, 0, 0};
    const Color Green = {0, 255, 0};
    const Color Blue = {0, 0, 255};
    const Color White = {255, 255, 255};
    const Color Yellow = {255, 255, 0};
    const Color Cyan = {0, 255, 255};
    const Color Magenta = {255, 0, 255};
    const Color Orange = {255, 165, 0};
    const Color Purple = {128, 0, 128};
    const Color Off = {0, 0, 0};
}

#endif //BIOBASED_SENSOREN_COLOR_H
