import tkinter as tk
import math, random
import numpy as np

points = None
pere = 300
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
    do_pere(points)
    T = np.array([[sx, 0, 0],
                  [0, sy, 0],
                  [0, 0, 1]])
    for i in range(len(points)):
        points[i] = multiply_vec_on_matrix(points[i], T)
    do_obr_pere(points)

def rotate(phi_deg):
    global points
    do_pere(points)
    a = math.radians(phi_deg)
    cos_a, sin_a = math.cos(a), math.sin(a)
    T = np.array([[cos_a,  sin_a, 0],
                  [-sin_a, cos_a, 0],
                  [0,      0,     1]])
    for i in range(len(points)):
        points[i] = multiply_vec_on_matrix(points[i], T)
    do_obr_pere(points)
def rotate_o_dot(phi_deg, cx, cy):
    do_pere(points)
    for i in range(len(points)):
        X_shift(i, -cx); Y_shift(i, -cy)

    a = math.radians(phi_deg)
    cos_a, sin_a = math.cos(a), math.sin(a)
    T = np.array([[cos_a, -sin_a, 0],
                  [sin_a,  cos_a, 0],
                  [0,      0,     1]])

    for i in range(len(points)):
        points[i] = multiply_vec_on_matrix(points[i], T)

    for i in range(len(points)):
        X_shift(i,  cx); Y_shift(i,  cy)
    do_obr_pere(points)
def star5(outer_r=20, inner_r=9):
    pts = []
    for k in range(10):
        ang = math.pi/2 + k * math.pi/5
        r = outer_r if k % 2 == 0 else inner_r
        pts.append([r * math.cos(ang), r * math.sin(ang), 1])
    return np.array(pts, float)

def pentagon(r=12):
    return np.array([[r * math.cos(math.pi/2 + k * 2 * math.pi / 5),
                      r * math.sin(math.pi/2 + k * 2 * math.pi / 5), 1]
                     for k in range(5)], float)

# режим 1
class Snowflake:
    def __init__(self, canvas, cx, cy, scale=1.0,
                 fall_speed=2.0, spin=0.0, color="white"):
        self.canvas, self.fall, self.drift, self.spin = canvas, fall_speed, random.uniform(-.5,.5), spin
        self.star_pts, self.pent_pts = star5(), pentagon()

        for arr in (self.star_pts, self.pent_pts):
            global points
            points = arr
            scaling(scale, scale)
            for i in range(len(points)):
                X_shift(i, cx); Y_shift(i, cy)

        self.item_star = canvas.create_polygon(*self.star_pts[:,:2].flatten(),
                                               outline=color, fill="")
        self.item_pent = canvas.create_polygon(*self.pent_pts[:,:2].flatten(),
                                               outline=color, fill="")

    def update(self, h):
        dx, dy, spin = self.drift, self.fall, self.spin
        for arr in (self.star_pts, self.pent_pts):
            global points
            points = arr
            for i in range(len(points)):
                X_shift(i, dx); Y_shift(i, dy)
            if spin: rotate(spin)

        if max(self.star_pts[:,1].max(), self.pent_pts[:,1].max()) > h+30:
            for arr in (self.star_pts, self.pent_pts):
                points = arr
                for i in range(len(points)): Y_shift(i, -(h+60))

        self.canvas.coords(self.item_star, *self.star_pts[:,:2].flatten())
        self.canvas.coords(self.item_pent, *self.pent_pts[:,:2].flatten())

def run_snowflakes():
    global pere
    pere=0
    W, H = 1000, 800
    root = tk.Tk(); root.title("Падающие снежинки")
    canvas = tk.Canvas(root, width=W, height=H, bg="black"); canvas.pack()

    flakes = [Snowflake(canvas,
                        random.randint(0, W),
                        random.randint(-H, 0),
                        random.uniform(.6,1.4)*10,
                        random.uniform(1.,3.),
                        random.choice([0,1,-1]))
              for _ in range(15)]

    def tick():
        for flake in flakes: flake.update(H)
        root.after(20, tick)
    tick(); root.mainloop()

def do_pere(arr):
    for c in arr:
        c[0]-=pere
        c[1]-=pere
def do_obr_pere(arr):
    for c in arr:
        c[0]+=pere
        c[1]+=pere
def reflect_ox(arr):
    R = np.array([[1,0,0],[0,-1,0],[0,0,1]])
    do_pere(arr)
    d = np.array([multiply_vec_on_matrix(v,R) for v in arr])
    do_obr_pere(d)
    return d
def reflect_oy(arr):
    R = np.array([[-1,0,0],[0,1,0],[0,0,1]])
    do_pere(arr)
    d = np.array([multiply_vec_on_matrix(v,R) for v in arr])
    do_obr_pere(d)
    return d
def reflect_y_eq_x(arr):
    R = np.array([[0,-1,0],[-1,0,0],[0,0,1]])
    do_pere(arr)
    d = np.array([multiply_vec_on_matrix(v,R) for v in arr])
    do_obr_pere(d)
    return d

def build_flag():
   # rect = np.array([[-50,-30,1],[50,-30,1],[50,30,1],[-50,30,1]],float)
    star = star5(40,17)
    #poly_in = []
    return [star]

class TransformerGUI:
    def __init__(self, root):
        self.root, self.history = root, []
        self.canvas = tk.Canvas(root, width=600, height=600, bg="white")
        self.canvas.grid(row=1, column=0, columnspan=24)

        self.canvas.create_line(300, 0, 300, 600)  # Oy
        self.canvas.create_line(0, 300, 600, 300)  # Ox

        self.point_sets = build_flag()
        for arr in self.point_sets:  # перенос к (300,300)
            global points;
            points = arr
            for i in range(len(points)):
                X_shift(i, 300);
                Y_shift(i, 300)

        self.items = [self.canvas.create_polygon(*p[:, :2].flatten(),
                                                 outline="blue", fill="")
                      for p in self.point_sets]

        # ----------------- ПОЛЯ ВВОДА -----------------
        def entry(col, label, default):
            tk.Label(root, text=label, font=("Arial", 10)
                     ).grid(row=0, column=col, sticky="e")
            e = tk.Entry(root, width=5);
            e.insert(0, default)
            e.grid(row=0, column=col + 1, padx=(0, 10))
            return e

        self.delta = entry(0, "Δ:", "20")  # col 0–1
        self.angle = entry(2, "φ°:", "30")  # col 2–3
        self.xp = entry(4, "Px:", "0")  # col 4–5
        self.yp = entry(6, "Py:", "0")  # col 6–7
        self.sx = entry(8, "Sx:", "1.0")  # col 8–9
        self.sy = entry(10, "Sy:", "1.0")  # col 10–11

        # ----------------- КНОПКИ -----------------
        make = lambda txt, w, cmd, col: tk.Button(
            root, text=txt, width=w, command=cmd
        ).grid(row=0, column=col, padx=2)

        # стрелки  (col 12‑15)
        make("←", 4, lambda: self.shift(-self.d(), 0), 12)
        make("→", 4, lambda: self.shift(+self.d(), 0), 13)
        make("↑", 4, lambda: self.shift(0, -self.d()), 14)
        make("↓", 4, lambda: self.shift(0, +self.d()), 15)

        # масштаб произвольный и отражения  (col 16‑19)
        make("⤧ Sx,Sy", 8, self.scale_by_entry, 16)
        make("Отраж. Ox", 10, self.flip_ox, 17)
        make("Отраж. Oy", 10, self.flip_oy, 18)
        make("Отраж. y=x", 10, self.flip_xy, 19)

        # быстрый ×2 и вращения  (col 20‑23)
        make("×2 Ox", 6, lambda: self.rescale(2, 1), 20)
        make("×2 Oy", 6, lambda: self.rescale(1, 2), 21)
        make("↻ φ°", 6, self.rotate_by_entry, 22)
        make("↻ P φ°", 7, self.rotate_about_entry, 23)

        # сервисные  (col 24‑25)
        make("Сброс", 6, self.reset, 24)
        make("Отмена", 6, self.undo, 25)

    def d(self):
        try: return float(self.delta.get())
        except ValueError: return 20

    def save(self):
        self.history.append([arr.copy() for arr in self.point_sets])

    def redraw(self):
        for arr,it in zip(self.point_sets,self.items):
            self.canvas.coords(it,*arr[:,:2].flatten())

    def shift(self,dx,dy):
        self.save()
        for arr in self.point_sets:
            global points; points = arr
            for i in range(len(points)): X_shift(i,dx); Y_shift(i,dy)
        self.redraw()

    def scale_by_entry(self):
        """Независимый масштаб по осям из полей Sx, Sy."""
        try:
            sx = float(self.sx.get())
            sy = float(self.sy.get())
        except ValueError:
            return  # неправильный ввод — игнорируем

        self.save()
        for arr in self.point_sets:
            global points
            points = arr
            scaling(sx, sy)  # уже готовая матричная функция
        self.redraw()

    def rotate_about_entry(self):
        try:
            phi = float(self.angle.get())
            cx = float(self.xp.get())
            cy = float(self.yp.get())
            cy = -cy
        except ValueError:
            return

        self.save()
        for arr in self.point_sets:
            global points
            points = arr
            rotate_o_dot(phi, cx, cy)
        self.redraw()

    def rotate_by_entry(self):
        try:
            phi = float(self.angle.get())
        except ValueError:
            return
        self.save()
        for arr in self.point_sets:
            global points
            points = arr
            rotate(phi)
        self.redraw()

    def flip_ox(self):
        self.save(); self.point_sets=[reflect_ox(p) for p in self.point_sets]; self.redraw()
    def flip_oy(self):
        self.save(); self.point_sets=[reflect_oy(p) for p in self.point_sets]; self.redraw()
    def flip_xy(self):
        self.save(); self.point_sets=[reflect_y_eq_x(p) for p in self.point_sets]; self.redraw()

    def rescale(self,sx,sy):
        self.save()
        for arr in self.point_sets:
            global points; points=arr; scaling(sx,sy)
        self.redraw()

    def rotate_all(self,phi):
        self.save()
        for arr in self.point_sets:
            global points; points=arr; rotate(phi)
        self.redraw()

    def reset(self):
        self.save(); self.point_sets=build_flag()
        for arr in self.point_sets:
            global points; points=arr
            for i in range(len(points)): X_shift(i,300); Y_shift(i,300)
        self.redraw()

    def undo(self):
        if self.history:
            self.point_sets = self.history.pop()
            self.redraw()

def run_transformer():
    root = tk.Tk(); root.title("Геометрические преобразования")
    TransformerGUI(root); root.mainloop()


def select_mode():
    choice={'m':None}
    def setm(m): choice['m']=m; win.destroy()

    win=tk.Tk(); win.title("Выбор режима")
    tk.Label(win,text="Выберите режим:",font=("Arial",13)).pack(padx=20,pady=10)
    tk.Button(win,text="Падающие снежинки",width=22,command=lambda:setm(1)).pack(pady=4)
    tk.Button(win,text="Перемещение фигуры",width=22,command=lambda:setm(2)).pack(pady=4)
    win.mainloop(); return choice['m']

if __name__=="__main__":
    m=select_mode()
    if   m==1: run_snowflakes()
    elif m==2: run_transformer()
