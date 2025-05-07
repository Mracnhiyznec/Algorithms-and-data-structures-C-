import tkinter as tk
import math, random
import numpy as np

points = None

def multiply_vec_on_matrix(vec, matrix):
    res = [0, 0, 0]
    for i in range(3):
        for j in range(3):
            res[i] += vec[j] * matrix[j][i]
    return res

def X_shift(ind, dx):
    T = np.array([[1, 0, 0],
                  [0, 1, 0],
                  [dx, 0, 1]])
    points[ind] = multiply_vec_on_matrix(points[ind], T)

def Y_shift(ind, dy):
    T = np.array([[1, 0, 0],
                  [0, 1, 0],
                  [0, dy, 1]])
    points[ind] = multiply_vec_on_matrix(points[ind], T)

def scaling(sx, sy):
    global points
    T = np.array([[sx, 0, 0],
                  [0, sy, 0],
                  [0, 0, 1]])
    for i in range(len(points)):
        points[i] = multiply_vec_on_matrix(points[i], T)

def rotate(phi_deg):
    global points
    a = math.radians(phi_deg)
    cos_a, sin_a = math.cos(a), math.sin(a)
    T = np.array([[cos_a,  sin_a, 0],
                  [-sin_a, cos_a, 0],
                  [0,      0,     1]])
    for i in range(len(points)):
        points[i] = multiply_vec_on_matrix(points[i], T)


def star5(outer_r=20, inner_r=9):
    pts = []
    for k in range(10):
        ang = math.pi/2 + k * math.pi/5
        r = outer_r if k % 2 == 0 else inner_r
        pts.append([r * math.cos(ang), r * math.sin(ang), 1])
    return np.array(pts)

def pentagon(r=12):
    return np.array([[r * math.cos(math.pi/2 + k * 2 * math.pi / 5),
                      r * math.sin(math.pi/2 + k * 2 * math.pi / 5), 1]
                     for k in range(5)])
class Snowflake:
    def __init__(self, canvas: tk.Canvas, cx, cy, scale=1.0, fall_speed=2.0, spin=0.0):
        self.canvas = canvas
        self.fall = fall_speed
        self.drift = random.uniform(-0.5, 0.5)
        self.spin = spin

        self.star_pts = star5()
        self.pent_pts = pentagon()

        for arr in (self.star_pts, self.pent_pts):
            global points
            points = arr
            scaling(scale, scale)
            for i in range(len(points)):
                X_shift(i, cx)
                Y_shift(i, cy)

        self.item_star = canvas.create_polygon(*self.star_pts[:, :2].flatten(),
                                               outline="white", fill="")
        self.item_pent = canvas.create_polygon(*self.pent_pts[:, :2].flatten(),
                                               outline="white", fill="")

    def update(self, h):
        dx, dy = self.drift, self.fall
        spin = self.spin

        for arr in (self.star_pts, self.pent_pts):
            global points
            points = arr
            for i in range(len(points)):
                X_shift(i, dx)
                Y_shift(i, dy)
            if spin:
                rotate(spin)

        ymax = max(self.star_pts[:, 1].max(), self.pent_pts[:, 1].max())
        if ymax > h + 30:
            dy_up = -(h + 60)
            for arr in (self.star_pts, self.pent_pts):
                points = arr
                for i in range(len(points)):
                    Y_shift(i, dy_up)

        self.canvas.coords(self.item_star, *self.star_pts[:, :2].flatten())
        self.canvas.coords(self.item_pent, *self.pent_pts[:, :2].flatten())

def main():
    W, H = 1000, 1000
    root = tk.Tk()
    root.title("Падающие снежинки")
    canvas = tk.Canvas(root, width=W, height=H, bg="black")
    canvas.pack()

    flakes = []
    for _ in range(10):
        cx = random.randint(0, W)
        cy = random.randint(-H, 0)
        scale = random.uniform(0.6, 1.4)
        speed = random.uniform(1.0, 3.0)
        spin = random.choice([0, 1, -1])
        flakes.append(Snowflake(canvas, cx, cy, scale*10, speed, spin))

    def tick():
        for flake in flakes:
            flake.update(H)
        root.after(20, tick)

    tick()
    root.mainloop()

if __name__ == "__main__":
    main()
