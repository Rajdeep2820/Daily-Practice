// Learning Objective: This tutorial will teach you how to generate
// beautiful fractal patterns using recursion and basic C++ graphics.
// We will focus on the Sierpinski Triangle as a prime example.
// By the end, you'll understand how simple recursive rules
// can create complex and visually stunning fractal structures.

#include <iostream> // For console output (e.g., error messages)
#include <vector>   // For storing points that define our triangle
#include <cmath>    // For mathematical operations if needed (not strictly for Sierpinski, but good to have)

// --- Graphics Library Placeholder ---
// For simplicity and broad compatibility, we'll simulate graphics.
// In a real-world scenario, you'd use a library like SFML, SDL, or OpenGL.
// This placeholder allows us to demonstrate the logic without external dependencies.

// Represents a 2D point
struct Point {
    double x, y;
};

// Function to draw a line between two points
// In a real graphics library, this would render a visible line.
void drawLine(const Point& p1, const Point& p2) {
    // For this educational example, we'll just print the points that would be connected.
    // This helps visualize the recursion.
    std::cout << "Drawing line from (" << p1.x << ", " << p1.y << ") to (" << p2.x << ", " << p2.y << ")\n";
}

// --- Recursive Fractal Generation ---

// Function to calculate the midpoint between two points.
// This is a key operation for generating fractals like Sierpinski.
Point midpoint(const Point& p1, const Point& p2) {
    return {(p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0};
}

// The core recursive function to draw the Sierpinski Triangle.
// Parameters:
//   p1, p2, p3: The three vertices of the current triangle.
//   depth: The current level of recursion. Controls the complexity of the fractal.
void drawSierpinski(Point p1, Point p2, Point p3, int depth) {
    // Base Case: If the depth is 0, we've reached the desired complexity.
    // We draw the outermost triangle (or stop subdividing).
    if (depth == 0) {
        // Draw the current triangle's edges.
        // This is where the visual lines would appear in a real graphics context.
        drawLine(p1, p2);
        drawLine(p2, p3);
        drawLine(p3, p1);
        return; // Stop recursion for this branch
    }

    // Recursive Step: If depth > 0, we subdivide the current triangle.
    // We find the midpoints of each side.
    Point m12 = midpoint(p1, p2); // Midpoint of side p1-p2
    Point m23 = midpoint(p2, p3); // Midpoint of side p2-p3
    Point m31 = midpoint(p3, p1); // Midpoint of side p3-p1

    // Now, we recursively call drawSierpinski for the three smaller triangles
    // formed by the original vertices and the midpoints.
    // Each recursive call reduces the depth by 1.

    // Top triangle (using p1 and the two new midpoints)
    drawSierpinski(p1, m12, m31, depth - 1);

    // Left triangle (using p2 and the two new midpoints)
    drawSierpinski(p2, m23, m12, depth - 1);

    // Right triangle (using p3 and the two new midpoints)
    drawSierpinski(p3, m31, m23, depth - 1);

    // Important Note: We DO NOT draw the middle triangle (formed by m12, m23, m31).
    // This is what creates the "holes" and the characteristic Sierpinski pattern.
}

// --- Example Usage ---

int main() {
    std::cout << "--- Generating Sierpinski Triangle ---" << std::endl;

    // Define the initial triangle vertices.
    // These can be any three non-collinear points.
    // For simplicity, let's use a large equilateral triangle.
    Point start_p1 = {100.0, 100.0}; // Top vertex
    Point start_p2 = {50.0, 400.0};  // Bottom-left vertex
    Point start_p3 = {350.0, 400.0}; // Bottom-right vertex

    // Define the desired depth of recursion.
    // Higher depth means more intricate patterns.
    // Be careful: depth grows exponentially, so keep it reasonable for testing.
    int recursion_depth = 4; // Try values like 0, 1, 2, 3, 4

    std::cout << "Initial triangle points: P1(" << start_p1.x << ", " << start_p1.y
              << "), P2(" << start_p2.x << ", " << start_p2.y << "), P3(" << start_p3.x << ", " << start_p3.y << ")\n";
    std::cout << "Recursion depth: " << recursion_depth << std::endl;
    std::cout << "\n--- Drawing Process ---" << std::endl;

    // Call the recursive function to start drawing.
    drawSierpinski(start_p1, start_p2, start_p3, recursion_depth);

    std::cout << "\n--- Fractal Generation Complete ---" << std::endl;

    return 0; // Indicates successful execution
}