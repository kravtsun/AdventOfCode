-- Avoiding `import Data.List.Split (splitOn)` by implementing a simple splitOn function
splitOn :: Eq a => [a] -> [a] -> [[a]]
splitOn _ [] = []
splitOn [] str = map (:[]) str
splitOn delimStr str = let
    delimLen = length delimStr

    -- splitOnHelper :: Eq a => [a] -> [a] -> [[a]]
    splitOnHelper currentStringMember [] = [currentStringMember]
    splitOnHelper currentStringMember strRest@(firstChar:otherChars) = let
        strPrefix = take delimLen strRest
        in if strPrefix == delimStr 
            then currentStringMember : splitOnHelper [] (drop delimLen strRest)
            else splitOnHelper (currentStringMember ++ [firstChar]) otherChars
    in splitOnHelper [] str
