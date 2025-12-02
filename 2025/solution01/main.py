#!/usr/bin/env python3
# This software was partially written using Suggestions from GitHub Copilot.

def star1(filepath: str, start_x = 50, MOD = 100):
    x = start_x
    res = 0
    with open(filepath) as fin:
        for l in fin:
            c = l[0]
            num = int(l[1:].strip())
            assert num > 0 and x >= 0
            if c == 'L':
                x -= num
            else:
                x += num
            x %= MOD
            res += 1 if x == 0 else 0
    return res


def star2(filepath: str, start_x = 50, MOD = 100):
    x = start_x
    res = 0
    with open(filepath) as fin:
        for l in fin:
            c = l[0]
            num = int(l[1:].strip())
            assert num > 0 and x >= 0
            if c == 'L':
                res += (num + ((MOD - x) % MOD)) // MOD
                x -= num
            else:
                res += (num + x) // MOD
                x += num
            x %= MOD
    return res


if __name__ == '__main__':
    print(star1("example_input.txt"))
    print(star1("input.txt"))
    print(star2("example_input.txt"))
    print(star2("input.txt"))
