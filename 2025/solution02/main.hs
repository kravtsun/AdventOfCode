-- This software was partially written using Suggestions from GitHub Copilot.

-- Avoiding `import Data.List.Split (splitOn)` by implementing a simple splitOn function
splitOn :: Eq a => [a] -> [a] -> [[a]]
splitOn _ [] = []
splitOn [] str = map (:[]) str
splitOn delimStr str = let
    delimLen = length delimStr

    -- splitOnHelper splits the string rest (second argument) recursively: 
    -- if meets the delimiter string as prefix, new segment is cut;
    -- otherwise, it adds the first character to the current segment (first argument)
    splitOnHelper currentStringMember [] = [currentStringMember]
    splitOnHelper currentStringMember strRest@(firstChar:otherChars) = let
        strPrefix = take delimLen strRest
        in if strPrefix == delimStr
            then currentStringMember : splitOnHelper [] (drop delimLen strRest)
            else splitOnHelper (currentStringMember ++ [firstChar]) otherChars
    in splitOnHelper [] str


isValidSecondStar :: Int -> Bool
isValidSecondStar n | n < 10 = True -- cannot have non-trivial periodicity in single-digit numbers
                    | otherwise = let
    nStr = show n
    nStrSize = length nStr
    -- leaving out nStrSize itself to avoid trivial periodicity
    nStrSizeDivisors = filter (\size -> mod nStrSize size == 0) [1..nStrSize-1]
    -- isStringPeriodic returns True if str is periodic with period size equal to periodSize
    isStringPeriodic :: String -> Int -> Bool
    isStringPeriodic str periodSize = let
        periodString = take periodSize str
        strRest = drop periodSize str
        -- isStringPeriodicHelper checks whether the rest of the string is made up of periodString repetitions
        isStringPeriodicHelper :: String -> Bool
        isStringPeriodicHelper [] = True
        isStringPeriodicHelper strPart = let 
            strPrefix = take periodSize strPart
            strSuffix = drop periodSize strPart
            in strPrefix == periodString && isStringPeriodicHelper strSuffix
        in isStringPeriodicHelper strRest
    -- if any divisor gives periodicity, then n is invalid
    in not (any (isStringPeriodic nStr) nStrSizeDivisors)


isValid :: Int -> Bool -> Bool
isValid n isSecondStar | isSecondStar = isValidSecondStar n
                       | n < 10 = True
                       | otherwise = let
    nStr = show n
    nStrSize = length nStr
    in odd nStrSize || let
        halfSize = div nStrSize 2
        (nStrLeftHalf, nStrRightHalf) = splitAt halfSize nStr
        in nStrLeftHalf /= nStrRightHalf

solve :: String -> Bool -> IO ()
solve filepath isSecondStar = do
    line <- readFile filepath
    let tokens = splitOn "," line
    let abStringRanges = map (splitOn "-") tokens
    let abIntRanges = map (\[a,b] -> (read a, read b)) abStringRanges
    let listRanges = map(\(a,b) -> [a..b]) abIntRanges
    let invalidIds = map (filter (\n -> not (isValid n isSecondStar))) listRanges
    let res = sum (concat invalidIds)

    print res

star1 :: String -> IO ()
star1 filepath = solve filepath False

star2 :: String -> IO ()
star2 filepath = solve filepath True

main :: IO ()
main = do
    star1 "example_input.txt"
    star1 "input.txt"
    star2 "example_input.txt"
    star2 "input.txt"
