
import Data.Char (isDigit)

readInt :: String -> Int
readInt x = (read x) :: Int

readNumbers :: String -> (Int, [Int])
readNumbers line = let
    tokens = words line
    firstToken = head tokens
    expectedResult = readInt (takeWhile isDigit firstToken)
    numbers = map readInt (tail tokens)
    in (expectedResult, numbers)

isGood :: (Int, [Int]) -> Bool -> Bool
isGood (expectedResult, numbers) isSecondStar = let
    isGoodHelper :: [Int] -> Int -> Int -> Bool
    isGoodHelper numbers currentSum expectedSum | currentSum > expectedSum = False
                                                | null numbers = currentSum == expectedSum
                                                | otherwise = let 
            first = head numbers
            others = tail numbers
            isGoodWithMul = isGoodHelper others (currentSum * first) expectedSum
            isGoodWithAdd = isGoodHelper others (currentSum + first) expectedSum
            isGoodWithConcat = isGoodHelper others (readInt ((show currentSum) ++ (show first))) expectedSum
        in isGoodWithMul || isGoodWithAdd || (isSecondStar && isGoodWithConcat)
    in isGoodHelper (tail numbers) (head numbers) expectedResult

solve :: String -> Bool -> IO ()
solve filepath isSecondStar = do
    content <- readFile filepath
    let ls = lines content
    let numbersLines = map readNumbers ls
    let goodResults = map (\t@(expectedResult, numbers) -> if (isGood t isSecondStar) then expectedResult else 0) numbersLines
    putStrLn (show (sum goodResults))

star1 :: String -> IO ()
star1 filepath = solve filepath False

star2 :: String -> IO ()
star2 filepath = solve filepath True

main :: IO ()
-- main = star1 "example_input.txt"
-- main = star1 "input.txt"
-- main = star2 "example_input.txt"
main = star2 "input.txt"
