import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Arrays;
import java.util.function.BiPredicate;
import java.util.function.Predicate;
import java.util.stream.IntStream;

import static java.lang.Math.abs;

public class Main {
    public static void main(String[] args) {
        star1("example_input.txt"); // 2
        star1("input.txt"); // 314
        star2("example_input.txt"); // 4
        star2("input.txt"); // 373
    }

    public static void star1(String filepath) {
        processFile(filepath, Main::isSafe1);
    }

    public static void star2(String filepath) {
        processFile(filepath, Main::isSafe2);
    }

    static private void processFile(String filepath, Predicate<int[]> isSafe) {
        try (BufferedReader reader = new BufferedReader(new FileReader(filepath))) {
            String line;
            int safeCount = 0;
            while ((line = reader.readLine()) != null) {
                if (line.isEmpty()) break;
                String[] tokens = line.split(" ");
                int[] array = Arrays.stream(tokens).mapToInt(Integer::parseInt).toArray();
                if (isSafe.test(array)) {
                    safeCount++;
                }
            }
            System.out.println(safeCount);
        } catch (Exception e) {
            System.err.println("Exception: " + e.getMessage());
        }
    }

    private static boolean isIntervalGood(int pref, int next) {
        int interval = abs(pref - next);
        return (interval >= 1 && interval <= 3);
    }

    private static boolean isSafe1(int[] array) {
        boolean isIncreasing = true;
        boolean isDecreasing = true;
        boolean isIntervalGood = true;
        for (int i = 0; i + 1 < array.length; i++) {
            isIncreasing = isIncreasing && (array[i] < array[i + 1]);
            isDecreasing = isDecreasing && (array[i] > array[i + 1]);
            isIntervalGood = isIntervalGood && isIntervalGood(array[i], array[i + 1]);
        }
        return isIntervalGood && (isIncreasing || isDecreasing);
    }

    private static boolean isSafe2(int[] array) {
        BiPredicate<Integer, Integer> isIncreasingPredicate = (a, b) -> a < b;
        BiPredicate<Integer, Integer> isDecreasingPredicate = (a, b) -> a > b;
        return isSafe2WithPredicate(array, isIncreasingPredicate) || isSafe2WithPredicate(array, isDecreasingPredicate);
    }

    private static boolean isSafe2WithPredicate(int[] array, BiPredicate<Integer, Integer> predicate) {
        final int n = array.length;
        boolean isMonotonouslyGoodArray = true;
        for (int i = 0; i + 1 < n; ++i) {
            if (!predicate.test(array[i], array[i + 1]) || !isIntervalGood(array[i], array[i + 1])) {
                int[] arrayWithoutPrev = arrayWithoutElement(array, i);
                int[] arrayWithoutNext = arrayWithoutElement(array, i + 1);
                if (isSafe1(arrayWithoutPrev) || isSafe1(arrayWithoutNext)) {
                    return true;
                } else {
                    isMonotonouslyGoodArray = false;
                    break;
                }
            }
        }
        return isMonotonouslyGoodArray;
    }

    private static int[] arrayWithoutElement(int[] array, int elementIndex) {
        assert elementIndex >= 0 && elementIndex < array.length;
        return IntStream.range(0, array.length).filter(j -> j != elementIndex).map(j -> array[j]).toArray();
    }
}