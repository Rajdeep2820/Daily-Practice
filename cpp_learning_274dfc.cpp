// Header Comment:
// This tutorial demonstrates how to procedurally generate and visualize a Julia set fractal
// in a graphical window using C++ and the SFML library.
// We will focus on the core concept of iterating a complex function to determine pixel color.
// The SFML library is used for basic graphics output. You'll need to install it separately.
// Installation instructions can be found on the official SFML website.

#include <SFML/Graphics.hpp> // Include the SFML graphics library for window and drawing functions.
#include <complex>           // Include the complex number library for easy handling of complex numbers.
#include <vector>            // Include vector for storing pixel data.

// Define the dimensions of our fractal window.
const int WIDTH = 800;
const int HEIGHT = 600;

// This function calculates the number of iterations it takes for a point
// to escape a certain boundary when repeatedly applying the Julia set function.
// This iteration count determines the color of the pixel.
int julia_iterations(std::complex<double> c, std::complex<double> z0, int max_iterations) {
    // c: The constant complex number defining the specific Julia set.
    // z0: The initial complex number representing the pixel's position in the complex plane.
    // max_iterations: The maximum number of iterations to perform before assuming the point is within the set.

    std::complex<double> z = z0; // Initialize z with the starting point.

    for (int i = 0; i < max_iterations; ++i) {
        // The core Julia set iteration: z = z*z + c
        // This is the complex function that generates the fractal pattern.
        z = z * z + c;

        // Check if the magnitude of z has exceeded a certain threshold (e.g., 2.0).
        // If it exceeds the threshold, the point is considered to "escape" and is not part of the set.
        if (std::norm(z) > 4.0) { // std::norm(z) calculates the squared magnitude of z.
            return i; // Return the number of iterations it took to escape.
        }
    }
    return max_iterations; // If the point didn't escape within max_iterations, it's considered inside the set.
}

int main() {
    // 1. Setting up the SFML Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Julia Set Fractal"); // Create a window with specified dimensions and title.

    // 2. Defining the Julia Set Parameters
    // The constant 'c' defines the specific Julia set we want to visualize.
    // Different values of 'c' produce vastly different fractal patterns.
    std::complex<double> julia_constant(-0.7, 0.27015); // Example constant for a common Julia set.

    // 3. Creating an Image to Store Pixel Data
    sf::Image fractal_image;
    fractal_image.create(WIDTH, HEIGHT); // Create an empty image with the window's dimensions.

    // 4. Generating the Fractal Pixels
    int max_iterations = 100; // Number of iterations for each pixel. Higher values give more detail but take longer.

    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            // Map the pixel coordinates (x, y) to the complex plane.
            // We're mapping the screen coordinates to a specific region of the complex plane.
            // Here, we map the pixel grid [0, WIDTH]x[0, HEIGHT] to the complex region [-2.0, 2.0]x[-1.5, 1.5].
            // You can adjust these ranges to zoom into different parts of the fractal.
            double real_part = -2.0 + (double)x / WIDTH * 4.0;
            double imag_part = -1.5 + (double)y / HEIGHT * 3.0;
            std::complex<double> z0(real_part, imag_part); // The initial complex number for this pixel.

            // Calculate the number of iterations for this point.
            int iterations = julia_iterations(julia_constant, z0, max_iterations);

            // 5. Coloring the Pixels
            // The color is determined by the number of iterations.
            // We use a simple coloring scheme:
            // - If the point is inside the set (max_iterations reached), color it black.
            // - Otherwise, color it based on the number of iterations to create a gradient.
            sf::Color pixel_color;
            if (iterations == max_iterations) {
                pixel_color = sf::Color::Black; // Point is likely in the set.
            } else {
                // Map iterations to a color gradient. This is a basic example;
                // more sophisticated coloring can create stunning visuals.
                // We're using 'iterations' to control the R, G, B components.
                unsigned char color_value = static_cast<unsigned char>((iterations * 255) / max_iterations);
                pixel_color = sf::Color(color_value, color_value / 2, color_value / 4); // A simple hue shift.
            }
            fractal_image.setPixel(x, y, pixel_color); // Set the color of the pixel in the image.
        }
    }

    // 6. Displaying the Fractal
    sf::Texture fractal_texture;
    fractal_texture.loadFromImage(fractal_image); // Load the image data into a texture.
    sf::Sprite fractal_sprite;
    fractal_sprite.setTexture(fractal_texture); // Create a sprite to draw the texture.

    // Main application loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window if the close button is clicked.
            }
        }

        window.clear(); // Clear the window with a background color (default is black).
        window.draw(fractal_sprite); // Draw the fractal sprite onto the window.
        window.display(); // Update the window to show what has been drawn.
    }

    return 0; // Indicate successful execution.
}

// Example Usage:
// To compile and run this code:
// 1. Make sure you have SFML installed.
// 2. Save the code as a .cpp file (e.g., julia_fractal.cpp).
// 3. Compile using a C++ compiler like g++:
//    g++ julia_fractal.cpp -o julia_fractal -lsfml-graphics -lsfml-window -lsfml-system
// 4. Run the executable:
//    ./julia_fractal
//
// You should see a graphical window displaying a Julia set fractal.
// Try changing the 'julia_constant' variable to see different fractal patterns!
// Experiment with 'max_iterations' to control detail and computation time.
// Adjust the mapping of pixel coordinates to the complex plane (real_part, imag_part) to zoom and pan.