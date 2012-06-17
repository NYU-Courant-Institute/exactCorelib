When building mev , on page 184, the two addhe should be exchanged because we should 
keep the vertex and add an edge first or the original vertex with an halfedge but no
edge will be lost.

When using mev, before using mef, we should focus on one face

Before your Solid is exactly a 3-D solid, all mev should be used on the same face.

For mef, he1 in the newloop,while he2 remains in the old one.
When drawing a cube after you have built a square and four edges you have to make 
sure that the edge on the upper face of the lamina should be in the new face.

Be careful when "closing" the cube because in the last step you may only have
single circle, which means that you cannot meet a duplicat point in the loop.

We can build different solids through #ifdef
