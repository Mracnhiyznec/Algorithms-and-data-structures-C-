import numpy as np
import matplotlib.pyplot as plt

# 1. Параметры сетки
hx = hy = 0.1                      # шаг сетки
x_min, x_max = -3, 3               # прямоугольник, в который вписан эллипс
y_min, y_max = -4, 4

x = np.arange(x_min, x_max + hx, hx)
y = np.arange(y_min, y_max + hy, hy)
X, Y = np.meshgrid(x, y, indexing='ij')

# 2. Определяем, где эллипс, граница и «внутренность»
ellipse = (X**2)/9 + (Y**2)/16             # выражение эллипса
tol = 0.02                                 # «толщина» полосы для границы
boundary = np.abs(ellipse - 1) <= tol      # узлы ≈ на границе
inside   = ellipse < 1 - tol               # узлы внутри
# Все прочие узлы — вне области; они не участвуют в итерациях,
# но им сразу присвоим граничное значение, чтобы формулы были одинаковыми
u = np.zeros_like(X)
f = np.abs(X) + np.abs(Y)                  # |x|+|y|
u[boundary] = f[boundary]
u[~inside & ~boundary] = f[~inside & ~boundary]

# 3. Метод Якоби
max_it = 5_000
eps    = 1e-4
for k in range(max_it):
    u_new = u.copy()
    # обновляем только внутренние узлы (векторизовано)
    u_new[1:-1, 1:-1][inside[1:-1, 1:-1]] = (
        u[:-2, 1:-1][inside[1:-1, 1:-1]] +
        u[2:, 1:-1][inside[1:-1, 1:-1]] +
        u[1:-1, :-2][inside[1:-1, 1:-1]] +
        u[1:-1, 2:][inside[1:-1, 1:-1]]
    ) * 0.25

    if np.max(np.abs(u_new - u)) < eps:    # сходимость
        print(f'Converged in {k+1} iterations.')
        break
    u = u_new

# 4. График
plt.figure(figsize=(8, 6))
plt.title('Приближённое решение u(x, y) для варианта 1')
plt.xlabel('x')
plt.ylabel('y')
plt.imshow(np.flipud(u.T), extent=[x_min, x_max, y_min, y_max],
           origin='lower', aspect='auto')
plt.colorbar(label='u')
plt.show()
