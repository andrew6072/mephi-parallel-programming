#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsInColorTable;
    uint32_t importantColorCount;
};
#pragma pack(pop)

int main(int argc, char* argv[]) {
    std::string path_to_file = argv[1];
    std::cout << argv[0] << "\n";
    std::ifstream file(path_to_file, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    BMPHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));

    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        std::cerr << "Invalid BMP file format." << std::endl;
        return 1;
    }

    if (header.bitsPerPixel != 24) {
        std::cerr << "Only 24-bit BMP files are supported." << std::endl;
        return 1;
    }

    // Move file pointer to the beginning of pixel data
    file.seekg(header.dataOffset, std::ios::beg);

    // Variables to store pixel colors counts
    int redCount = 0, greenCount = 0, blueCount = 0;

    // Read pixel data and count colors
    for (int y = 0; y < header.height; ++y) {
        for (int x = 0; x < header.width; ++x) {
            uint8_t pixel[3];
            file.read(reinterpret_cast<char*>(pixel), 3);

            // Check if the pixel is red, green, or blue
            if (pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 255) {
                blueCount++;
            } else if (pixel[0] == 0 && pixel[1] == 128 && pixel[2] == 0) {
                greenCount++;
            } else if (pixel[0] == 255 && pixel[1] == 0 && pixel[2] == 0) {
                redCount++;
            }
        }
        // Handle padding (if any)
        file.seekg(header.width % 4, std::ios::cur);
    }

    // Output the counts
    std::cout << "Red Pixels: " << redCount << std::endl;
    std::cout << "Green Pixels: " << greenCount << std::endl;
    std::cout << "Blue Pixels: " << blueCount << std::endl;

    file.close();

    return 0;
}
