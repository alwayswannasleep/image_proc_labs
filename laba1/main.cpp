#include <iostream>
#include "stb_image.h"
#include "stb_image_write.h"

unsigned char modulateColorValue(const int &value) {
    if (value < 0) {
        return 0;
    } else if (value > 255) {
        return 255;
    }

    return static_cast<unsigned char>(value);
}

void makeConvolution(const unsigned char *in, unsigned char *outBytes, const int &width, const int &height,
                     const int &channels, const int *kernel, const int &kernelWidth, const int &kernelHeight) {
    auto maskWidthStartPos = -(kernelWidth - 1) / 2;
    auto maskWidthEndPos = (kernelWidth - 1) / 2;
    auto maskHeightStartPos = -(kernelHeight - 1) / 2;
    auto maskHeightEndPos = (kernelHeight - 1) / 2;

    auto maskSum = 0;

    for (int i = 0; i < 9; i++) {
        maskSum += kernel[i];
    }

    if (maskSum <= 0) {
        maskSum = 1;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixelPosition = j + (i * width);
            int startPixelIndex = channels * pixelPosition;

            unsigned char r = in[startPixelIndex + 0];
            unsigned char g = in[startPixelIndex + 1];
            unsigned char b = in[startPixelIndex + 2];

            if ((i + maskHeightStartPos) >= 0
                && (j + maskWidthStartPos) >= 0
                && i < (height - maskHeightEndPos)
                && j < (width - maskWidthEndPos)) {

                int rSum = 0;
                int gSum = 0;
                int bSum = 0;

                for (int k = 0; k < kernelHeight; k++) {
                    for (int m = 0; m < kernelWidth; m++) {
                        auto maskIndex = m + (k * kernelWidth);
                        auto row = (i + maskHeightStartPos + k);
                        auto column = (j + maskWidthStartPos + m);
                        auto position = column + (row * width);

                        rSum += in[channels * position + 0] * kernel[maskIndex];
                        gSum += in[channels * position + 1] * kernel[maskIndex];
                        bSum += in[channels * position + 2] * kernel[maskIndex];
                    }
                }

                r = modulateColorValue(rSum / maskSum);
                g = modulateColorValue(gSum / maskSum);
                b = modulateColorValue(bSum / maskSum);
            }

            outBytes[startPixelIndex + 0] = r;
            outBytes[startPixelIndex + 1] = g;
            outBytes[startPixelIndex + 2] = b;
        }
    }
}

int main() {
    std::string imageName = "image4.jpg";
    auto inPath = std::string("../resources/") + imageName;
    auto outPath = std::string("../output/") + imageName;
    int channels = 3;

    int width;
    int height;
    auto bytes = stbi_load(inPath.c_str(), &width, &height, NULL, STBI_rgb);

    if (bytes == NULL) {
        std::cout << stbi_failure_reason() << std::endl;
        return -1;
    }

    std::cout << "Width: " << width << " height: " << height << std::endl;

    auto maskWidth = 3;
    auto maskHeight = 3;
    auto imageMask = new int[maskWidth * maskHeight]{
            0, -1, 0,
            -1, 4, -1,
            0, -1, 0
    };
    auto outBytes = new unsigned char[width * height * channels];
    makeConvolution(bytes, outBytes, width, height, channels, imageMask, maskWidth, maskHeight);

    auto result = stbi_write_jpg(outPath.c_str(), width, height, channels, outBytes, 85);

    if (result == 0) {
        return -1;
    }

    std::cout << "Finished successfully\n";

    return 0;
}