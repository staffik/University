-- Lista 8 -  , 

import Data.List (sort)

-- Zadanie 1

lrepeat :: (Int -> Int) -> [a] -> [a]

lrepeat func list = repeatTmp list 0
  where
    repeatTmp [] _ = []
    repeatTmp (h:t) idx = (repeatElem h (func idx) (repeatTmp t (idx+1)))
      where
        repeatElem elem 0 acc = acc
        repeatElem elem k acc = elem:(repeatElem elem (k-1) acc)
      
test1a = lrepeat (\x -> x+1) ['a','b','c','d']
test1b = lrepeat (\x -> x+1) []
test1c = take 8 (lrepeat (\x -> 2*x+1) [1..])


-- Zadanie 2

sublist :: [Int] -> [a] -> [a]

sublist indices list = sublistTmp (sort indices) list 0
  where
    sublistTmp _ [] _ = []
    sublistTmp [] list _ = list
    sublistTmp (idx:idxs) (h:t) currIdx =
      if idx==currIdx then
        sublistTmp idxs t (currIdx+1)
      else
        h:(sublistTmp (idx:idxs) t (currIdx+1))

test2a = take 10 (sublist [1,4,7,2] [10..])
test2b = take 10 (sublist [1,1] [2,3,4,5])
test2c = take 10 (sublist [] [1..])
test2d = take 10 (sublist [1] [])
test2e = take 10 (sublist [] [])


-- Zadanie 3

eps = 1e-15

-- (a)

root3 :: Double -> Double
root3 0 = 0
root3 a = if a>1 then root3Tmp (a/3) else root3Tmp a
  where
  root3Tmp x = if abs(x^3-a) < eps*abs(a) then x else root3Tmp (x+(a/x^2-x)/3)

test3aa = root3 8
test3ab = root3 (-8)
test3ac = root3 0
test3ad = root3 1
test3ae = root3 (-2)


-- (b)

root3l :: Double -> Double
root3l 0 = 0
root3l a = last approximations
  where
    approximations = if a>1 then iterate root3Tmp (a/3) else iterate root3Tmp a
      where
        root3Tmp x = if abs(x^3-a) < eps*abs(a) then x else root3Tmp (x+(a/x^2-x)/3)

test3ba = root3 8
test3bb = root3 (-8)
test3bc = root3 0
test3bd = root3 1
test3be = root3 (-2)

