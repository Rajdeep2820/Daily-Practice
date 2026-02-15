// --- LEARNING OBJECTIVE ---
// This tutorial teaches the fundamental concept of a Quadtree, a spatial partitioning data structure,
// used to efficiently narrow down potential collision candidates in a 2D game world.
// Instead of checking every object against every other object (O(n^2) complexity),
// a Quadtree divides the world into quadrants, allowing us to quickly query
// only objects within a specific area. This significantly improves performance for
// tasks like collision detection, rendering frustum culling, and proximity queries.
// We will focus on building the basic Quadtree structure and its `insert` and `query` methods.

#include <iostream> // For console output
#include <vector>   // For storing collections of objects
#include <array>    // For fixed-size array of child Quadtree nodes
#include <memory>   // For std::unique_ptr to manage memory of child nodes

// 1. Basic Structures: Define simple types for bounding boxes and game objects.

// Represents an axis-aligned bounding box (AABB).
// Used for game object bounds and Quadtree node boundaries.
struct Rect {
    float x, y, width, height;

    // Checks if this rectangle contains a point (px, py).
    // Not directly used in this tutorial's core logic but useful for general Rect utility.
    bool contains(float px, float py) const {
        return px >= x && px <= x + width &&
               py >= y && py <= y + height;
    }

    // Checks if this rectangle intersects with another rectangle.
    // This is crucial for both object insertion and querying in the Quadtree.
    bool intersects(const Rect& other) const {
        return !(x + width < other.x ||
                 y + height < other.y ||
                 x > other.x + other.width ||
                 y > other.y + other.height);
    }
};

// Represents a simple game object with an ID and a bounding box.
// In a real game, this would be a more complex class with rendering, physics, etc.
struct GameObject {
    int id;       // A unique identifier for the object
    Rect bounds;  // The bounding box of the object

    // Constructor to easily create GameObjects.
    GameObject(int id, Rect bounds) : id(id), bounds(bounds) {}
};

// 2. The Quadtree Class: The core of our spatial partitioning system.

class Quadtree {
private:
    Rect boundary;                       // The area this Quadtree node covers.
    int capacity;                        // Max objects this node can hold before subdividing.
    std::vector<GameObject*> objects;    // Objects stored directly in this node.

    bool divided = false;                // True if this node has subdivided into children.
    // An array of unique_pointers to child Quadtree nodes.
    // std::unique_ptr ensures automatic memory management (children are deleted when parent is).
    // children[0]: North-East, children[1]: North-West, children[2]: South-East, children[3]: South-West
    std::array<std::unique_ptr<Quadtree>, 4> children; 

public:
    // Constructor: Initializes a Quadtree node with its boundary and object capacity.
    Quadtree(Rect boundary, int capacity) : boundary(boundary), capacity(capacity) {}

    // subdivide(): Splits this Quadtree node into four equal-sized children.
    void subdivide() {
        float subWidth = boundary.width / 2;
        float subHeight = boundary.height / 2;
        float x = boundary.x;
        float y = boundary.y;

        // Create and store the four new child Quadtree nodes using make_unique.
        children[0] = std::make_unique<Quadtree>(Rect{x + subWidth, y, subWidth, subHeight}, capacity);      // North-East
        children[1] = std::make_unique<Quadtree>(Rect{x, y, subWidth, subHeight}, capacity);                  // North-West
        children[2] = std::make_unique<Quadtree>(Rect{x + subWidth, y + subHeight, subWidth, subHeight}, capacity); // South-East
        children[3] = std::make_unique<Quadtree>(Rect{x, y + subHeight, subWidth, subHeight}, capacity);      // South-West

        divided = true; // Mark this node as having children.
    }

    // insert(): Adds a GameObject to the Quadtree.
    // Returns true if the object was successfully inserted into this branch, false otherwise.
    bool insert(GameObject* obj) {
        // 1. If the object's bounding box doesn't intersect this node's boundary, it cannot be stored here.
        if (!boundary.intersects(obj->bounds)) {
            return false;
        }

        // 2. If this node has space (below capacity) AND hasn't subdivided yet, add the object directly.
        if (objects.size() < capacity && !divided) {
            objects.push_back(obj);
            return true;
        }

        // 3. If at capacity or already divided:
        //    If not divided yet, subdivide this node first.
        if (!divided) {
            subdivide();
        }

        // 4. Try to insert the object into one of the child nodes.
        //    We iterate through children and try to insert. If any child successfully takes the object, we're done.
        for (int i = 0; i < 4; ++i) {
            if (children[i]->insert(obj)) {
                return true; // Object was fully contained and inserted into a child.
            }
        }
        
        // 5. If the object's bounds intersect this node's boundary but do not fit *entirely* within any single child
        //    (e.g., the object spans across multiple quadrants), it stays in this (parent) node's object list.
        //    This is crucial for objects that cannot be fully contained by a single child node after subdivision.
        objects.push_back(obj); 
        return true;
    }

    // query(): Finds all objects in the Quadtree that intersect with a given 'range' (Rect).
    // Stores the found objects in the 'found' vector.
    void query(const Rect& range, std::vector<GameObject*>& found) {
        // 1. If the query range does not intersect this node's boundary, no objects here can match.
        if (!boundary.intersects(range)) {
            return;
        }

        // 2. Add objects directly stored in *this* node that intersect the query range.
        for (GameObject* obj : objects) {
            if (range.intersects(obj->bounds)) {
                found.push_back(obj);
            }
        }

        // 3. If this node has children, recursively query them as well.
        if (divided) {
            for (int i = 0; i < 4; ++i) {
                children[i]->query(range, found);
            }
        }
    }
};

// 3. Example Usage: Demonstrating how to use the Quadtree.

int main() {
    // Define the overall game world boundary (e.g., a 800x600 pixel game screen).
    Rect worldBoundary = {0, 0, 800, 600};
    int capacity = 4; // Each Quadtree node can hold up to 4 objects before it tries to subdivide.

    // Create the main Quadtree for the entire game world.
    Quadtree quadtree(worldBoundary, capacity);
    std::cout << "Quadtree initialized for world (" << worldBoundary.width << "x" << worldBoundary.height << ") with node capacity " << capacity << ".\n\n";

    // Create some example game objects. Note: Using stack-allocated objects here.
    // If dynamically allocated, you'd manage their memory.
    GameObject obj1(1, {10, 10, 20, 20});
    GameObject obj2(2, {700, 50, 30, 30});
    GameObject obj3(3, {50, 500, 40, 40});
    GameObject obj4(4, {300, 250, 50, 50});
    GameObject obj5(5, {320, 270, 10, 10}); // Close to obj4
    GameObject obj6(6, {150, 150, 60, 60}); // Larger object, might span quadrants
    GameObject obj7(7, {380, 290, 20, 20}); // Near center, potentially spans
    GameObject obj8(8, {750, 550, 10, 10}); // Far corner

    std::cout << "Inserting objects into the Quadtree...\n";
    quadtree.insert(&obj1);
    quadtree.insert(&obj2);
    quadtree.insert(&obj3);
    quadtree.insert(&obj4);
    quadtree.insert(&obj5);
    quadtree.insert(&obj6);
    quadtree.insert(&obj7);
    quadtree.insert(&obj8);
    std::cout << "All objects inserted.\n\n";

    // Define a 'query range' – this could be a player's attack radius, a camera's view, etc.
    Rect queryRange = {280, 200, 100, 100}; // A 100x100 area around (280, 200)
    std::cout << "Querying for potential colliders within range: (" 
              << queryRange.x << "," << queryRange.y << "," << queryRange.width << "," << queryRange.height << ")\n";

    std::vector<GameObject*> potentialColliders;
    quadtree.query(queryRange, potentialColliders); // Populate the vector with found objects.

    // Print the results of the query.
    if (potentialColliders.empty()) {
        std::cout << "  No objects found in the query range.\n";
    } else {
        std::cout << "  Found " << potentialColliders.size() << " potential colliders:\n";
        for (GameObject* obj : potentialColliders) {
            std::cout << "  - Object ID: " << obj->id 
                      << " (Bounds: [" << obj->bounds.x << "," << obj->bounds.y 
                      << "] size: " << obj->bounds.width << "x" << obj->bounds.height << ")\n";
        }
    }
    std::cout << "\n";

    // Another query example: Simulating a player's view at the start of the world.
    Rect playerView = {0, 0, 100, 100}; 
    std::vector<GameObject*> objectsInPlayerView;
    quadtree.query(playerView, objectsInPlayerView);
    std::cout << "Querying objects within player's view (bounds: " 
              << playerView.x << "," << playerView.y << "," << playerView.width << "," << playerView.height << ")\n";
    if (!objectsInPlayerView.empty()) {
        std::cout << "  Objects in player's view (IDs): ";
        for (GameObject* obj : objectsInPlayerView) {
            std::cout << obj->id << " ";
        }
        std::cout << "\n";
    } else {
        std::cout << "  No objects currently in player's view.\n";
    }

    // The std::unique_ptr for children automatically handles memory cleanup when the Quadtree
    // (and its child unique_ptrs) go out of scope, so no manual `delete` for Quadtree nodes is needed.
    // Since our GameObjects are stack-allocated in main, no special cleanup is required for them.

    return 0;
}