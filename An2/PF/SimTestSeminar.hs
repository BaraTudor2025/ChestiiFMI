import Data.Char
f :: Char -> Bool
-- varianta 1
f c = if isAlpha c then (>0) $ length $ filter ((toLower c)==) ['a'..'m'] else error "eroare"
-- varianta 2
--f c = if isAlpha c then elem (toLower c) ['a'..'m'] else error "eroare"

g :: String -> Bool
g s = 
    let s' = [f c | c <- s, isAlpha c]
        s'' = [if b then 1 else -1 | b <- s']
    in sum s'' > 0

g_rec :: String -> Bool
g_rec s = (>0) $ r 0 s
    where r cnt [] = cnt
          r cnt (c:cs) | isAlpha c = if f c then r (cnt + 1) cs else r (cnt - 1) cs
                       | otherwise = r cnt cs


