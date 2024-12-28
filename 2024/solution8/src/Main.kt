import java.io.File
import java.util.*

typealias Point = Pair<Int, Int>

fun isGoodCoordinate(x: Int, n: Int): Boolean {
    return x in 0..<n
}

fun isGoodPoint(p: Point, n: Int, m: Int): Boolean {
    return isGoodCoordinate(p.first, n) && isGoodCoordinate(p.second, m)
}

operator fun Point.minus(rhs: Point): Point {
    return Point(first - rhs.first, second - rhs.second)
}

operator fun Point.plus(rhs: Point): Point {
    return Point(first + rhs.first, second + rhs.second)
}

fun readSignalToPositions(lines: List<String>): Map<Char, List<Point>> {
    val n = lines.size
    val m = lines[0].length

    val signalToPositions = TreeMap<Char, MutableList<Point>>()

    for (i in 0..<n) {
        for (j in 0..<m) {
            val c = lines[i][j]
            if (c != '.') {
                if (!signalToPositions.containsKey(c)) {
                    signalToPositions[c] = mutableListOf()
                }

                signalToPositions[c]!!.add(Pair(i, j))
            }
        }
    }
    return signalToPositions
}

fun star1(filepath: String): Int {
    val lines = File(filepath).readLines()
    val n = lines.size
    val m = lines[0].length

    val signalToPositions = readSignalToPositions(lines)

    val pairComparator = compareBy<Point>({it.first}, {it.second})
    val results = TreeSet(pairComparator)

    for ((_, positions) in signalToPositions) {
        for (i in positions.indices) {
            for (j in positions.indices) {
                if (i == j) continue
                val delta = positions[j] - positions[i]
                val pointAfterJ = positions[j] + delta
                val pointBeforeI = positions[i] - delta
                if (isGoodPoint(pointAfterJ, n, m)) {
                    results.add(pointAfterJ)
                }
                if (isGoodPoint(pointBeforeI, n, m)) {
                    results.add(pointBeforeI)
                }
            }
        }
    }
    return results.size
}

fun star2(filepath: String): Int {
    val lines = File(filepath).readLines()
    val n = lines.size
    val m = lines[0].length

    val signalToPositions = readSignalToPositions(lines)

    val pairComparator = compareBy<Point>({it.first}, {it.second})
    val results = TreeSet(pairComparator)

    for ((_, positions) in signalToPositions) {
        val signalResults = TreeSet(pairComparator)
        for (i in positions.indices) {
            for (j in positions.indices) {
                if (i == j) continue
                val delta = positions[j] - positions[i]

                var p = positions[i]
                while (isGoodPoint(p, n, m)) {
                    signalResults.add(p)
                    p -= delta
                }

                p = positions[j]
                while (isGoodPoint(p, n, m)) {
                    signalResults.add(p)
                    p += delta
                }
            }
        }
        results.addAll(signalResults)
    }
    return results.size
}

fun main() {
    println(star1("example_input.txt")) // 14
    println(star1("input.txt")) // 308
    println(star2("example_input.txt")) // 34
    println(star2("input.txt")) // 1147
}
