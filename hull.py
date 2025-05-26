import sys
import matplotlib.pyplot as plt
import matplotlib.animation as animation

INF = 4 * 10**18

class R:
    __slots__ = ('x','y')
    def __init__(self, x, y):
        self.x, self.y = x, y
    def __lt__(self, o):
        return (self.x, self.y) < (o.x, o.y)
    def __sub__(self, o):
        return R(self.x - o.x, self.y - o.y)
    def cross(self, o):
        return self.x * o.y - o.x * self.y

def nxt(i, n):
    return (i + 1) % n

def prev(i, n):
    return (i - 1 + n) % n

states = []

def find_down_bridge(left, right):
    ind1 = max(range(len(left)), key=lambda i: left[i].x)
    ind2 = min(range(len(right)), key=lambda i: right[i].x)
    moved = True
    while moved:
        moved = False
        while (right[ind2] - left[ind1]).cross(right[prev(ind2, len(right))] - left[ind1]) < 0:
            ind2 = prev(ind2, len(right)); moved = True
        while (left[ind1] - right[ind2]).cross(left[nxt(ind1, len(left))] - right[ind2]) > 0:
            ind1 = nxt(ind1, len(left)); moved = True
    return ind1, ind2

def find_up_bridge(left, right):
    ind1 = max(range(len(left)), key=lambda i: left[i].x)
    ind2 = min(range(len(right)), key=lambda i: right[i].x)
    moved = True
    while moved:
        moved = False
        while (right[ind2] - left[ind1]).cross(right[nxt(ind2, len(right))] - left[ind1]) > 0:
            ind2 = nxt(ind2, len(right)); moved = True
        while (left[ind1] - right[ind2]).cross(left[prev(ind1, len(left))] - right[ind2]) < 0:
            ind1 = prev(ind1, len(left)); moved = True
    return ind1, ind2

def find_sol(p):
    n = len(p)
    if n <= 1:
        return p[:]
    all_col = all((p[i] - p[0]).cross(p[1] - p[0]) == 0 for i in range(2, n))
    if all_col:
        p.sort()
        return [p[-1], p[0]]
    p.sort()
    mid = n // 2
    left = find_sol(p[:mid])
    right = find_sol(p[mid:])
    i1, j1 = find_down_bridge(left, right)
    i2, j2 = find_up_bridge(left, right)
    merged = []
    i = i1
    while True:
        merged.append(left[i])
        if i == i2: break
        i = nxt(i, len(left))
    j = j2
    while True:
        merged.append(right[j])
        if j == j1: break
        j = nxt(j, len(right))
    states.append(merged[:])
    return merged

def animate(pts):
    fig, ax = plt.subplots()
    ax.set_xlim(min(p.x for p in pts)-1, max(p.x for p in pts)+1)
    ax.set_ylim(min(p.y for p in pts)-1, max(p.y for p in pts)+1)
    def update(frame):
        ax.clear()
        ax.scatter([pt.x for pt in pts], [pt.y for pt in pts])
        for k in range(frame+1):
            hull = states[k]
            xs = [pt.x for pt in hull] + [hull[0].x]
            ys = [pt.y for pt in hull] + [hull[0].y]
            ax.plot(xs, ys)
        return []
    ani = animation.FuncAnimation(fig, update, frames=len(states),
                                  interval=1000, repeat=False)
    plt.show()

def main():
    n = int(input().strip())
    pts = []
    for _ in range(n):
        x, y = map(int, input().split())
        pts.append(R(x, y))
    _ = find_sol(pts)
    print(len(states[-1]))
    for pt in states[-1]:
        print(pt.x, pt.y)
    animate(pts)

if __name__ == "__main__":
    main()
