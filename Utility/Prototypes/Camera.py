import numpy as np


class Camera:
    def __init__(self, 
                 aspectRatio, 
                 fov, 
                 near = 0.1,
                 far = 1000.0,
                 position = np.array([0, 0, 0]), 
                 target = np.array([0, 0, 0]), 
                 up = np.array([0, 1, 0])):
        self.aspect_ratio = aspectRatio
        self.fov = fov
        self.near = near
        self.far = far
        self.position = position
        self.target = target
        self.up = up
        self.view_matrix = np.eye(4)
        self.projection_matrix = np.eye(4)

    def update(self):
        self.update_view_matrix()
        self.update_projection_matrix()


    def update_view_matrix(self):
        forward = self.target - self.position
        forward = forward / np.linalg.norm(forward)
        right = np.cross(forward, self.up)
        right = right / np.linalg.norm(right)
        up = np.cross(right, forward)
        up = up / np.linalg.norm(up)

        self.view_matrix[0, 0] = right[0]
        self.view_matrix[0, 1] = right[1]
        self.view_matrix[0, 2] = right[2]
        self.view_matrix[0, 3] = -np.dot(right, self.position)

        self.view_matrix[1, 0] = up[0]
        self.view_matrix[1, 1] = up[1]
        self.view_matrix[1, 2] = up[2]
        self.view_matrix[1, 3] = -np.dot(up, self.position)

        self.view_matrix[2, 0] = -forward[0]
        self.view_matrix[2, 1] = -forward[1]
        self.view_matrix[2, 2] = -forward[2]
        self.view_matrix[2, 3] = np.dot(forward, self.position)

        self.view_matrix[3, 0] = 0
        self.view_matrix[3, 1] = 0
        self.view_matrix[3, 2] = 0
        self.view_matrix[3, 3] = 1


    def update_projection_matrix(self):
        self.projection_matrix = np.eye(4)
        self.projection_matrix[0, 0] = 1 / (self.aspect_ratio * np.tan(np.radians(self.fov) / 2))
        self.projection_matrix[1, 1] = 1 / np.tan(np.radians(self.fov) / 2)
        self.projection_matrix[2, 2] = -((self.far + self.near) / (self.far - self.near))
        self.projection_matrix[2, 3] = -((2 * self.far * self.near) / (self.far - self.near))
        self.projection_matrix[3, 2] = -1
        self.projection_matrix[3, 3] = 0

        


    def get_view_matrix(self):
        return self.view_matrix
        
    
    def get_projection_matrix(self):
        return self.projection_matrix
    

    def set_position(self, position):
        self.position = position
        self.update()

    def set_target(self, target):
        self.target = target
        self.update()

class Plane:
    def __init__(self):
        self.vertices = np.array([[ 1.0,  1.0, 0.0, 1.0],
                                  [-1.0,  1.0, 0.0, 1.0],
                                  [-1.0, -1.0, 0.0, 1.0],
                                  [ 1.0, -1.0, 0.0, 1.0]])
        self.indices = np.array([0, 1, 2, 0, 2, 3])


    
if __name__ == "__main__":
    camera = Camera(2467/901, 60, 0.1, 100, np.array([0, 0, 5]), np.array([0, 0, 0]), np.array([0, 1, 0]))


    camera.update()
    print(camera.get_view_matrix())
    print(camera.get_projection_matrix())

    #Print plane vertices model view projection matrix multiplication
    plane = Plane()
    model_matrix = np.eye(4)


    print("Vertices after model matrix multiplication: \n", np.dot(plane.vertices, model_matrix))

    print("Vertices after model view matrix multiplication: \n", np.dot(plane.vertices, np.dot(camera.get_view_matrix(), model_matrix)))

    print("Vertices after model view projection matrix multiplication: \n", np.dot(plane.vertices, np.dot(camera.get_projection_matrix(), np.dot(camera.get_view_matrix(), model_matrix))))

