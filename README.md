# Boids Simulation in Unreal Engine 5

This project is my deep dive into the classic Boids flocking algorithm, which I've been fascinated by for years. It serves as a personal challenge to explore programming concepts, including spatial partitioning and GPU-based compute shaders, while further developing my skills in the Unreal Engine environment.

My goal wasn't just to get something that looks cool but to build a robust and performant system that can be tuned and expanded upon.

---

## So, What are Boids? 🤔

Boids are basically simulated "bird-like objects." The cool part is that their complex, lifelike flocking behavior comes from just three simple rules that each boid follows on its own. There's no central brain telling them what to do.

1.  **Separation** – Steer to avoid crowding local flockmates.
![Separation With Wrapping](Content\Assets\Github\Gifs\SeparationWithWrapping.gif)
2.  **Alignment** – Steer towards the average heading of local flockmates.
![Alignment With Wrapping](Content\Assets\Github\Gifs\AlignmentWithWrapping.gif)
3.  **Cohesion** – Steer to move toward the average position of local flockmates.
![Cohesion With Wrapping](Content\Assets\Github\Gifs\CohesionWithWrapping.gif)

When you mix these rules together, you get the mesmerizing, emergent patterns you see in real flocks of birds or schools of fish!

### Early Development & Boundary Handling

In the initial stages of development, the simulation had no collision boundaries. To keep the boids contained, I implemented a "wrapping" behavior where a boid exiting one side of the bounds would instantly teleport to the opposite side.

Here's a look at the pure flocking behavior with this wrapping mechanic:
![All Forces With Wrapping](Content\Assets\Github\Gifs\AllForcesWithWrapping.gif)

This was later replaced by a "soft wall" collision system, which uses pre-computed force vectors to steer boids away from the boundaries, resulting in a more natural look.

Here is the current behavior with all forces and boundary collision active:
![All Forces With Collision Bounding Box](Content\Assets\Github\Gifs\AllForcesWithCollision.gif)

---

## Project Architecture and Class Structure 🏗️

The simulation is built on a decoupled, data-oriented architecture that separates the core logic from its visual representation, which makes the system clean and maintainable.

### Core Simulation Layer 🖥️

This layer contains lightweight C++ classes and structs that handle the raw data, with no dependencies on Unreal's Actor framework.
* `FBoid`: A simple data struct representing a single boid with `Position`, `Velocity` and `Acceleration` properties. It contains no logic other than a simple `Update` function to integrate its position.
* `FBounds` / `FWorldCollisionBounds`: A custom AABB implementation and its child class, which manages a voxelized grid. This grid is pre-computed with "soft wall" force vectors to steer boids away from boundaries. It will be replaced later by a generic version of the voxelized grid for multiple purposes.
* `UBoidManagerSubsystem`: The heart of the simulation. This `UTickableWorldSubsystem` owns and updates all `FBoid` data structures each frame. It is responsible for:
    * Calculating all boid forces (separation, alignment, cohesion, collision).
    * Managing the neighbor search algorithm.
    * Broadcasting delegates with simulation data for visualizers to consume.

### Visual Layer 👁️

This layer consists of `AActor` classes responsible for representing the simulation state in the game world. They are visualizers that listen to the `BoidManagerSubsystem`.
* `AVisualBoid`: The visual representation of a single boid, containing a `UStaticMeshComponent`. It does not update itself; its position and rotation are set by the `AVisualBoidManager` each frame.
* `AVisualBoidManager`: An actor that subscribes to the `BoidManagerSubsystem`'s delegates. It manages a pool of `AVisualBoid` actors, updating their transforms to match the data from the core simulation.
* `AVisualWorldBounds`: An actor that visualizes the simulation boundaries, updating its scale and position in response to delegates from the subsystem.

### Data and Tooling 🛠️

This layer will drive the simulation's parameters and allow for in-editor tweaking.
* `UBoidsData`: A `UPrimaryDataAsset`-based class that holds all the tuning parameters for a boid "species" (e.g., force multipliers, max speed, perception radius, color).
* **Editor Utility Widget:** An in-progress tool that will allow for real-time manipulation of `UBoidsData` assets and simulation parameters, providing immediate visual feedback in the editor. It is in very early development and was one of the main points of this project—to explore Unreal Engine's tooling system capabilities. Editor Utility Widgets are a very neat way of tooling since they provide designers with real-time control over the simulation without needing to write any code.

**Here are some of the planned features:**
* **Boid Panel:**
    * Tweak Separation, Cohesion, and Alignment forces per species.
    * Adjust speed, perception radius, and vision cone.
    * Change species color.
    * Add or remove boids from the simulation at runtime.
* **Environment Panel:**
    * Modify the simulation bounds (center and extent).
    * Trigger a recalculation of the environment collision grid.
    * Visualize the collision grid and its force vectors for debugging.

Here is an early design of how it will look later on (*don't take the color scheme seriously, it's a prototype*):

![Tool Prototype](Content\Assets\Github\Images\BoidsToolPrototype.png)

---

## Spatial Partitioning: Voxel Grid vs. Octree ✨

To optimize the neighbor search from a brute-force `O(N²)` check, I evaluated both Octrees and Voxel Grids (Uniform Grids). While Octrees are excellent for sparse environments, I ultimately chose a **Voxel Grid**, even though my initial learning of QuadTrees was a catalyst for this project. Here are my thoughts and reasons specific to this project's goals:
* **Distribution:** The boids are expected to be relatively uniformly distributed throughout the simulation area. Voxel Grids perform exceptionally well in dense or uniform scenarios.
* **Lookup Speed:** Voxel Grids offer a constant-time `O(1)` lookup for finding the cell a boid belongs to. An Octree's lookup is `O(log n)`, which is still fast but inherently slower.
* **Dynamic Objects:** Since boids move every frame, an Octree would need to be partially or fully rebuilt frequently, which was the main argument for me. This dynamic reconstruction carries a significant performance cost. A Voxel Grid, being a simple array, is much cheaper to update with the boids' new positions each frame.

---

## Future Development Roadmap 🗺️

This project is being developed iteratively, with a clear plan for future enhancements.

1.  **Implement Tool UI:** Build the visual interface for the Editor Utility Widget.
2.  **Template Voxel Grid:** Create a generic, reusable Voxel Grid class to optimize boid-to-boid collision and neighbor finding. I will also reuse this class in my later project on 3D pathfinding.
3.  **Environment Collision Grid:** Implement the voxelized collision system for static geometry based on the generic class.
4.  **Data Integration:** Hook up the Editor Tool to load and save `UBoidsData` assets, allowing for persistent changes.
5.  **Tool Interactivity:** Implement the tool's core logic to communicate with the `BoidManagerSubsystem` at runtime.
6.  **Template Object Pool:** Create a generic object pool for efficiently managing `AVisualBoid` actors and Threads
