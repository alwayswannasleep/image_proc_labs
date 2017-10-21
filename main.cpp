#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"

unsigned char modulateColorValue(const unsigned char &value) {
    if (value < 0) {
        return 0;
    } else if (value > 255) {
        return 255;
    }

    return value;
}

int main() {
    std::string imageName = "image1.jpg";
    auto inPath = std::string("resources/") + imageName;
    auto outPath = std::string("output/") + imageName;
    int channels = 3;

    int width;
    int height;
    auto bytes = stbi_load(inPath.c_str(), &width, &height, NULL, STBI_rgb);

    if (bytes == NULL) {
        std::cout << stbi_failure_reason() << std::endl;
        return -1;
    }

    std::cout << "Width: " << width << " height: " << height << std::endl;

    auto outBytes = new unsigned char[width * height * channels];

    auto maskWidth = 3;
    auto maskHeight = 3;
    auto maskWidthStartPos = -(maskWidth - 1) / 2;
    auto maskWidthEndPos = (maskWidth - 1) / 2;
    auto maskHeightStartPos = -(maskHeight - 1) / 2;
    auto maskHeightEndPos = (maskHeight - 1) / 2;

    auto imageMask = new int[maskWidth * maskHeight]{
            1, 0, -1,
            2, 0, -2,
            1, 0, -1
    };
    auto maskSum = 0;

    for (int i = 0; i < 9; i++) {
        maskSum += imageMask[i];
    }

    if (maskSum <= 0) {
        maskSum = 1;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelPosition = j + (i * width);
            int startPixelIndex = channels * pixelPosition;

            unsigned char r = bytes[startPixelIndex + 0];
            unsigned char g = bytes[startPixelIndex + 1];
            unsigned char b = bytes[startPixelIndex + 2];

            if ((i + maskHeightStartPos) >= 0
                && (j + maskWidthStartPos) >= 0
                && i < (height - maskHeightEndPos)
                && j < (width - maskWidthEndPos)) {

                int rSum = 0;
                int gSum = 0;
                int bSum = 0;

                for (int k = 0; k < maskHeight; k++) {
                    for (int m = 0; m < maskWidth; m++) {
                        auto maskIndex = m + (k * maskWidth);
                        auto row = (i + maskHeightStartPos + k);
                        auto column = (j + maskWidthStartPos + m);
                        auto position = column + (row * width);

                        rSum += bytes[channels * position + 0] * imageMask[maskIndex];
                        gSum += bytes[channels * position + 1] * imageMask[maskIndex];
                        bSum += bytes[channels * position + 2] * imageMask[maskIndex];
                    }
                }

                r = modulateColorValue(static_cast<unsigned char>(rSum / maskSum));
                g = modulateColorValue(static_cast<unsigned char>(gSum / maskSum));
                b = modulateColorValue(static_cast<unsigned char>(bSum / maskSum));
            }

            outBytes[startPixelIndex + 0] = r;
            outBytes[startPixelIndex + 1] = g;
            outBytes[startPixelIndex + 2] = b;
        }
    }

    auto result = stbi_write_jpg(outPath.c_str(), width, height, channels, outBytes, 85);

    if (result == 0) {
        return -1;
    }

    std::cout << "Finished successfully\n";

    return 0;
}