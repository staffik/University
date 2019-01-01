-- Lista 10 -  , 

module Main(main) where
import System.IO
import System.Random

seed = 42

randomInts :: Int -> (Int, Int) -> Int -> [Int]
randomInts n (lo, hi) zarodek = take n (randomRs (lo, hi) (mkStdGen zarodek))

gen_board n = randomInts n (1, n) seed

main = 
  do
    n <- prompt " Input number of rows: "
    runGame (gen_board (read n :: Int)) --[5, 4, 3, 2, 1]

prompt msg =
  do  putStrLn msg
      number <- getLine
      return number

replaceElem pos value arr = helper 1 pos value arr 
  where
    helper _ _ _ [] = []
    helper idx pos value [x] = [value]
    helper idx pos value (h:t) =
      if idx == pos then (value:t)
      else helper (idx+1) pos value t

runGame board =
  do  
      putStrLn (show board)
      row <- prompt " Input row number: "
      stars_no <- prompt " Input stars number: "
      runGame (replaceElem (read row :: Int) (read stars_no :: Int) board)
