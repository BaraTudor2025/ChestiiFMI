{-
f [3,1,1,3,3,5] 
> [1,3]
> f [3,3,1,3,1]
> [3]
> f [42] 
>[] 
-}
--frec [1,1,1,3,3,3,3,5,1,3]

fdup :: [a] -> [a]
fdup v = [ a | (a, b) <- zip v (tail v), a == b]


frec :: [Int] -> [Int]
frec [] = []
frec (a:[]) = []
frec (a:b:t) = 
    if a == b then
        a : frec (b:t)
    else
        frec (b:t)

