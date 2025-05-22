import numpy as np
import matplotlib.pyplot as plt

def f(t, x):
    # тестовая функция
    return x - t**2 + 1

def exact_solution(t):
    # точноее решение
    return (t + 1)**2 - 0.5 * np.exp(t)

# метод Эйлера
def euler_step(t, x, h):
    return x + h * f(t, x)



def rk2_step(t, x, h):
    k1 = f(t, x)
    k2 = f(t + h, x + h * k1)
    return x + h * (k1 + k2) / 2

def rk4_step(t, x, h):
    k1 = f(t, x)
    k2 = f(t + h / 2, x + h * k1 / 2)
    k3 = f(t + h / 2, x + h * k2 / 2)
    k4 = f(t + h, x + h * k3)
    return x + h * (k1 + 2*k2 + 2*k3 + k4) / 6

# Словарь методов: номер -> (функция шага, порядок метода s)
methods = {
    1: (euler_step, 1),
    2: (rk2_step, 2),
    3: (rk4_step, 4),
}

def solve_ivp_adaptive(t0, x0, T, h, eps, method):
    step_func, order = methods[method]
    ts, xs = [t0], [x0]
    t, x = t0, x0

    while t < T:
        if t + h > T:
            h = T - t
        x_full = step_func(t, x, h)
        x_half = step_func(t, x, h / 2)
        x_half_full = step_func(t + h / 2, x_half, h / 2)
        delta = abs(x_half_full - x_full) / (2**order - 1)

        if delta < eps:
            t += h
            x = x_half_full
            ts.append(t)
            xs.append(x)

        else:
            h/=2
    return np.array(ts), np.array(xs)

def main():
    t0 = float(input("Введите t0: "))
    x0 = float(input("Введите x0: "))
    T = float(input("Введите T: "))
    eps = float(input("Введите точность ε: "))
    h = float(input("Введите начальный шаг h: "))

    print("Выберите метод:\n"
          "1 — метод Эйлера\n"
          "2 — метод Рунге–Кутты 2-го порядка\n"
          "3 — метод Рунге–Кутты 4-го порядка")
    method = int(input("Номер метода (1/2/3): "))

    ts, xs = solve_ivp_adaptive(t0, x0, T, h, eps, method)

    ts_exact = np.linspace(t0, T, 500)
    xs_exact = exact_solution(ts_exact)

    plt.figure(figsize=(8, 5))
    plt.plot(ts, xs, 'o-', label="Численное решение")
    plt.plot(ts_exact, xs_exact,  '-', label="Точное решение")
    plt.xlabel("t")
    plt.ylabel("x(t)")
    plt.title("Численное и точное решение задачи Коши")
    plt.grid(True)
    plt.legend()
    plt.show()

if __name__ == "__main__":
    main()
