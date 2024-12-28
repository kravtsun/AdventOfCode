import math


def read_button(line: str):
    tokens = line.strip().split(" ")
    assert len(tokens) == 4
    assert tokens[0] == "Button"
    assert tokens[2].startswith("X+")
    assert tokens[3].startswith("Y+")
    return int(tokens[2][2:-1]), int(tokens[3][2:])


def read_prize(line: str, is_star2: bool):
    tokens = line.strip().split(" ")
    assert len(tokens) == 3
    assert tokens[1].startswith("X=")
    assert tokens[2].startswith("Y=")
    bias = 10000000000000 if is_star2 else 0
    coords = (int(tokens[1][2:-1]) + bias, int(tokens[2][2:]) + bias)
    return coords


def gcd(a, b):
    if b == 0:
        return a, 1, 0
    else:
        g, x, y = gcd(b, a % b)
        ia = y
        ib = x - (a // b) * y
        return g, ia, ib


def solve_diophant(a, b, c):
    g, ia, ib = gcd(a, b)
    if c % g != 0:
        return None
    multiplier = c // g
    ia *= multiplier
    ib *= multiplier

    lcd = a // g * b

    # deltas: (ia + dia * k) * a + (ib - dib * k) * b = prize
    dia = lcd // a
    dib = lcd // b

    return ia, ib, dia, dib


def solve(a, b, prize):
    solution = solve_diophant(a[0], b[0], prize[0])
    if solution is None:
        return None

    ia_first, ib_first, dia_first, dib_first = solution

    k_nominator = prize[1] - ia_first * a[1] - ib_first * b[1]
    k_denominator = dia_first * a[1] - dib_first * b[1]

    kg = math.gcd(abs(k_nominator), abs(k_denominator))
    k_nominator //= kg
    k_denominator //= kg

    if k_nominator % k_denominator != 0:
        return None

    k0 = k_nominator // k_denominator
    ia0 = ia_first + k0 * dia_first
    ib0 = ib_first - k0 * dib_first

    return 3 * ia0 + ib0


def star(filepath: str, is_star2: bool):
    with open(filepath) as fin:
        lines = fin.readlines()
        lines = [line.strip() for line in lines if len(line.strip()) > 0]
        assert len(lines) % 3 == 0
        result = 0
        for i in range(0, len(lines), 3):
            a = read_button(lines[i])
            b = read_button(lines[i + 1])
            prize = read_prize(lines[i + 2], is_star2)
            current_result = solve(a, b, prize)
            if current_result is not None:
                result += current_result
        print(result)


def star1(filepath: str):
    star(filepath, False)


def star2(filepath: str):
    star(filepath, True)


if __name__ == '__main__':
    star1("example_input.txt") # 480
    star1("input.txt") # 26299
    # no sample reference result this time
    star2("input.txt") # 107824497933339
