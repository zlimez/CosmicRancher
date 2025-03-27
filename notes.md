## Map Generation
### Voronoi Diagrams
Spawn n seeds (can use to symbollize regions) randomly across the map, for all cells paint the cell in the same color as the "nearest" seed, (different distance algo avail e.g. pythagoras, chebyshev, manhattan)

### Perlin/Simplex noise


### References
[RogueBasin](https://roguebasin.com/index.php/Main_Page)
Research minecraft how to generate new map and stitch up the terrain to look consistent

### Shader ideas
- Shadows (2D shadows)
- Lighting shader (directional, point light)
- VFX Bloom, Greyscale

### TODO 
- Animation (sprite based)
- Infinite map (not only wrap around but also minecraft like generates new area on demand)
- Layer several maps on top of one another for aesthetics
- Render grouping by index and by texture to reduces texture bind switch and vao dont need generate for all cells

### Random
- `install_name_tool -add_rpath @executable_path/../lib bin/main` before running debugger