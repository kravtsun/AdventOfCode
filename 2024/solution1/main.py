example_input = """
3   4
4   3
2   5
1   3
3   9
3   3"""

def main1(input_str: str):
    lines = input_str.split('\n')
    pairs = [tuple(map(int, l.split('   '))) for l in lines if len(l.strip()) > 0]
    l, r = list(zip(*pairs))
    l, r = sorted(l), sorted(r)
    distances = [abs(lel - rel) for (lel, rel) in zip(l, r)]
    return sum(distances)

def main2(input_str: str):
    lines = input_str.split('\n')
    pairs = [tuple(map(int, l.split('   '))) for l in lines if len(l.strip()) > 0]
    l, r = list(zip(*pairs))

    from collections import Counter
    rcount = Counter(r)
    return sum(lel * rcount[lel] for lel in l if lel in rcount)

if __name__ == '__main__':
    print(main2(example_input))
    with open('input.txt') as fin:
        input_str = fin.read()
        print(main2(input_str))
