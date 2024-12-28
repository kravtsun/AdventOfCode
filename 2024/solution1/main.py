def get_lr(all_input: str):
    lines = all_input.split('\n')
    pairs = [tuple(map(int, l.split('   '))) for l in lines if len(l.strip()) > 0]
    l, r = list(zip(*pairs))
    return l, r


def main1(all_input: str):
    l, r = get_lr(all_input)
    l, r = sorted(l), sorted(r)
    distances = [abs(lel - rel) for (lel, rel) in zip(l, r)]
    return sum(distances)


def main2(all_input: str):
    l, r = get_lr(all_input)
    from collections import Counter
    rcount = Counter(r)
    return sum(lel * rcount[lel] for lel in l if lel in rcount)


if __name__ == '__main__':
    example_input = """
3   4
4   3
2   5
1   3
3   9
3   3"""
    print(main2(example_input))  # 31
    with open('input.txt') as fin:
        input_str = fin.read()
        print(main2(input_str))  # 19437052
