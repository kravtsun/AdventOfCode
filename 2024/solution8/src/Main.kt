import java.io.File
import java.util.*

typealias PII = Pair<Int, Int>;

fun isGoodCoordinate(x: Int, n: Int): Boolean {
    return x in 0..<n;
}

fun isGoodPoint(p: PII, n: Int, m: Int): Boolean {
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m);
}

operator fun PII.minus(rhs: PII): PII {
    return PII(first - rhs.first, second - rhs.second);
}

operator fun PII.plus(rhs: PII): PII {
    return PII(first + rhs.first, second + rhs.second);
}

fun readSignalToPositions(lines: List<String>): Map<Char, List<PII>> {
    val n = lines.size;
    val m = lines[0].length;

    val signalToPositions = TreeMap<Char, MutableList<PII>>();

    for (i in 0..<n) {
        for (j in 0..<m) {
            val c = lines[i][j];
            if (c != '.') {
                if (!signalToPositions.containsKey(c)) {
                    signalToPositions[c] = mutableListOf();
                }

                signalToPositions[c]!!.add(Pair(i, j));
            }
        }
    }
    return signalToPositions;
}

// 14, 308
fun star1(lines: List<String>): Int {
    val n = lines.size;
    val m = lines[0].length;

    val signalToPositions = readSignalToPositions(lines);

    val pairComparator = compareBy<PII>({it.first}, {it.second});
    val results = TreeSet(pairComparator);

    for ((_, positions) in signalToPositions) {
        for (i in positions.indices) {
            for (j in positions.indices) {
                if (i == j) continue;
                val delta = positions[j] - positions[i];
                val pointAfterJ = positions[j] + delta;
                val pointBeforeI = positions[i] - delta;
                if (isGoodPoint(pointAfterJ, n, m)) {
                    results.add(pointAfterJ);
                }
                if (isGoodPoint(pointBeforeI, n, m)) {
                    results.add(pointBeforeI);
                }
            }
        }
    }
    return results.size;
}

// 34, 1147
fun star2(lines: List<String>): Int {
    val n = lines.size;
    val m = lines[0].length;

    val signalToPositions = readSignalToPositions(lines);

    val pairComparator = compareBy<PII>({it.first}, {it.second});
    val results = TreeSet(pairComparator);

    for ((_, positions) in signalToPositions) {
        val signalResults = TreeSet(pairComparator);
        for (i in positions.indices) {
            for (j in positions.indices) {
                if (i == j) continue;
                val delta = positions[j] - positions[i];

                var p = positions[i];
                while (isGoodPoint(p, n, m)) {
                    signalResults.add(p);
                    p -= delta;
                }

                p = positions[j];
                while (isGoodPoint(p, n, m)) {
                    signalResults.add(p);
                    p += delta;
                }
            }
        }
        results.addAll(signalResults);
    }
    return results.size;
}

fun main() {
//    val lines = File("example_input.txt").readLines()
    val lines = File("input.txt").readLines()
//    println(star1(lines));
    println(star2(lines));
}
