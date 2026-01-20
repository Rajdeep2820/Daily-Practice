// Learning Objective: This tutorial demonstrates how to procedurally generate a
// unique, repeating visual texture using C++ and simple algorithms. We will focus
// on creating a basic grayscale noise pattern that tiles seamlessly.

#include <iostream>
#include <vector>
#include <cmath> // For std::sin and std::cos
#include <random> // For random number generation

// Define the dimensions of our texture
const int TEXTURE_WIDTH = 128;
const int TEXTURE_HEIGHT = 128;

// Structure to represent a pixel's color (grayscale in this case)
struct Pixel {
    unsigned char intensity; // 0 (black) to 255 (white)
};

// Function to generate a single pixel's value based on its coordinates.
// This function is the core of our procedural generation.
// The 'offset' parameter is crucial for creating variations.
Pixel generatePixel(int x, int y, float offset) {
    Pixel p;

    // We'll use a combination of sine and cosine waves to create a smooth,
    // repeating pattern. The input to sin/cos will be a combination of the
    // pixel coordinates (x, y) and the offset.
    // Multiplying by a frequency value (e.g., 0.05f) controls how many
    // waves appear across the texture.
    float value = std::sin(x * 0.05f + offset) * std::cos(y * 0.05f - offset);

    // The 'value' will range from -1.0 to 1.0. We need to map this to
    // our 0-255 intensity range.
    // First, shift it to 0.0 to 2.0 by adding 1.0.
    // Then, scale it to 0.0 to 255.0 by multiplying by 127.5 (half of 255).
    p.intensity = static_cast<unsigned char>((value + 1.0f) * 127.5f);

    return p;
}

// Function to generate the entire texture.
// It iterates through each pixel and calls generatePixel.
std::vector<Pixel> generateTexture() {
    std::vector<Pixel> texture;
    texture.reserve(TEXTURE_WIDTH * TEXTURE_HEIGHT); // Pre-allocate memory for efficiency

    // We use a random offset to make each generated texture unique.
    // For truly repeatable generation, you would seed the random number generator
    // with a fixed value.
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine for good randomness
    std::uniform_real_distribution<float> dist(0.0f, 100.0f); // Range for the offset
    float offset = dist(gen);

    for (int y = 0; y < TEXTURE_HEIGHT; ++y) {
        for (int x = 0; x < TEXTURE_WIDTH; ++x) {
            // Call generatePixel for each coordinate.
            // The offset is passed to ensure a consistent pattern across the texture,
            // but a different offset means a different pattern.
            texture.push_back(generatePixel(x, y, offset));
        }
    }

    return texture;
}

// --- Example Usage ---
int main() {
    std::cout << "Generating a " << TEXTURE_WIDTH << "x" << TEXTURE_HEIGHT << " texture...\n";

    std::vector<Pixel> myTexture = generateTexture();

    std::cout << "Texture generated. First 10 pixels (intensity):\n";
    for (int i = 0; i < 10 && i < myTexture.size(); ++i) {
        std::cout << static_cast<int>(myTexture[i].intensity) << " ";
    }
    std::cout << "\n";

    // In a real application, you would now save this 'myTexture' data to an image file
    // (e.g., BMP, PNG) or use it directly in a graphics API.
    // For demonstration, we just print a few pixel values.

    return 0;
}