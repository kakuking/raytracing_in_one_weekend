# Following https://raytracing.github.io/books/RayTracingInOneWeekend.html

Completed Book

Output generated using the multithreaded renderer:
![final_image](https://github.com/kakuking/raytracing_in_one_weekend/assets/64602961/abb4747b-b48b-44c5-99e6-2e5e08f869d9)
Final render from the book, with 100 samples per pixel and a recursion depth of 20

Suggested improvements that could be made:

- [x] Add hittable support for objects that are not parametric, such as polygonal meshes
- [x] Add multithreading to speed up process

Uses the ![Möller–Trumbore Algorithm](https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm#C++_implementation) to find ray-triangle intersection
Currently uses brute force to go through all triangles

Further improvements that could be made
- [ ] Better algorithm to check intersection with mesh, beyond brute force

Used the Surface Area Heuristic to create Bounding Volume Heirarchy