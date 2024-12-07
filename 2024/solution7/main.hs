
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

isGood :: (Int, [Int]) -> Bool
isGood (expectedResult, numbers) = let
    isGoodHelper :: [Int] -> Int -> Int -> Bool
    isGoodHelper numbers currentSum expectedSum = 
        if (null numbers) then (currentSum == expectedSum) else let 
            first = head numbers
            others = tail numbers
            isGoodWithMul = isGoodHelper others (currentSum * first) expectedSum
            isGoodWithAdd = isGoodHelper others (currentSum + first) expectedSum
        in isGoodWithMul || isGoodWithAdd
    in isGoodHelper (tail numbers) (head numbers) expectedResult


star1 :: String -> IO ()
star1 filepath = do
    content <- readFile filepath
    let ls = lines content
    let numbersLines = map readNumbers ls
    let goodResults = map (\t@(expectedResult, numbers) -> if (isGood t) then expectedResult else 0) numbersLines
    putStrLn (show (sum goodResults))


main :: IO ()
main = star1 "input.txt"
