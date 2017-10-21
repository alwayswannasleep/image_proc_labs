#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

int main() {
    std::string imageName = "image3.jpg";
    auto inPath = std::string("../resources/") + imageName;
    auto outPath = std::string("../output/") + imageName;
    int channels = 3;
    int threshold = 4;

    int width;
    int height;
    auto bytes = stbi_load(inPath.c_str(), &width, &height, NULL, STBI_rgb);

    if (bytes == NULL) {
        std::cout << stbi_failure_reason() << std::endl;
        return -1;
    }

    std::cout << "Width: " << width << " height: " << height << std::endl;

    auto outBytes = new unsigned char[width * height];

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto pixelPosition = (x + (y * width));
            auto pixelStartIndex = pixelPosition * channels;

            auto grayScaleValue = (bytes[pixelStartIndex] + bytes[pixelStartIndex + 1] + bytes[pixelStartIndex + 2]) / channels;
            outBytes[pixelPosition] = static_cast<unsigned char>(grayScaleValue);
        }
    }

    auto result = stbi_write_jpg(outPath.c_str(), width, height, 1, outBytes, 85);

    if (result == 0) {
        return -1;
    }

    std::cout << "Finished successfully\n";

    return 0;
}