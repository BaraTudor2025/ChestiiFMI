import Data.Char;
import Data.List;
import Data.Bool;

bc4 :: Char -> (Char, Char)
bc4 c 
    | isLower c = (c, '#')
    | isUpper c = (c, '*')
    | isDigit c = (c, '^')
    | otherwise = (c, '_')

bc1 :: [Char] -> [(Char, Char)]
bc1 [] = []
bc1 (h:t) = bc4 h : bc1 t


bc2 :: [Char] -> [(Char, Char)]
bc2 ls = [bc4 c | c <- ls]


bc3 :: [Char] -> [(Char, Char)]
bc3 ls = map bc4 ls

bc5 :: [Char] -> Bool
bc5 ls = bc1 ls == bc3 ls


bc7 :: Char -> Bool
bc7 c = elem c "aeiou"

bc8 :: Char -> Char
bc8 c = if c == 'p' || bc7 c then error "Cod Invalid" else c

bc6 :: [Char] -> [Char]
bc6 [] = []
-- verific daca au ramas vocale sau 'p'-uri nefolosite
bc6 (h:[]) = [bc8 h]
bc6 (h:a:[]) = bc8 h : [bc8 a]
-- verific daca este pasareasca si pastrez doar vocala
bc6 (h:a:b:t) = 
    if bc7 h then
        if (h == b) && (a == 'p') then
            h : bc6 t
        else
            error "Cod Invalid"
    else
        h : bc6 (a:b:t)
-- bc6 (a:b:c:t) = 
    -- if b == 'p' then
    --     if bc7 a and bc7 c and a == c then
    --         a : bc6 t
    --     else
    --         []
    -- else








