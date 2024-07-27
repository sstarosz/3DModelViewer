import numpy as np
from mayavi import mlab

class Vertex:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

class HalfEdge:
    def __init__(self):
        self.origin = None
        self.twin = None
        self.next = None
        self.face = None

def generate_grid(width, height):
    vertices = []
    half_edges = []

    # Generate vertices
    for i in range(width):
        for j in range(height):
            vertices.append(Vertex(i, j, 0))

    # Generate half-edges
    for i in range(width):
        for j in range(height):
            index = j * width + i
            if i < width - 1:
                he1 = HalfEdge()
                he2 = HalfEdge()

                he1.origin = vertices[index]
                he1.twin = he2
                he2.origin = vertices[index + 1]
                he2.twin = he1

                half_edges.append(he1)
                half_edges.append(he2)

            if j < height - 1:
                he3 = HalfEdge()
                he4 = HalfEdge()

                he3.origin = vertices[index]
                he3.twin = he4
                he4.origin = vertices[index + width]
                he4.twin = he3

                half_edges.append(he3)
                half_edges.append(he4)

    return vertices, half_edges

def visualize_half_edge_structure(vertices, half_edges, current_edge=None):
    # Create vertices array
    vertices_array = np.array([[v.x, v.y, v.z] for v in vertices])

    # Create edges array
    edges_array = []
    for he in half_edges:
        edges_array.append([he.origin.x, he.origin.y, he.origin.z])
        edges_array.append([he.twin.origin.x, he.twin.origin.y, he.twin.origin.z])
        edges_array.append([np.nan, np.nan, np.nan])  # Separate each edge with NaNs for proper visualization

    edges_array = np.array(edges_array)

    # Plot
    mlab.points3d(vertices_array[:, 0], vertices_array[:, 1], vertices_array[:, 2], scale_factor=0.1)
    for i in range(0, len(edges_array), 3):
        x = edges_array[i:i+3, 0]
        y = edges_array[i:i+3, 1]
        z = edges_array[i:i+3, 2]
        if current_edge is not None and i // 3 == current_edge:
            mlab.plot3d(x, y, z, tube_radius=None, color=(1, 0, 0), line_width=3)  # Highlight current edge
        else:
            mlab.plot3d(x, y, z, tube_radius=None)

    mlab.show()

# Example usage: Generate a 5x5 grid and visualize with highlighted edge at index 10
vertices, half_edges = generate_grid(5, 5)
visualize_half_edge_structure(vertices, half_edges, current_edge=10)
