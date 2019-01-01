-- Lista 9 (zadania 1, 2) -  , 

data Tree a = Leaf a | Node (Tree a) (Tree a)
  deriving (Eq, Ord, Show, Read)


-- Zadanie 1

isBalanced :: Tree a -> Bool
isBalanced tree = fst (isBalancedHelper tree)
  where
    isBalancedHelper :: Tree a -> (Bool, Int)
    isBalancedHelper (Leaf _) = (True, 1)
    isBalancedHelper (Node left_subtree right_subtree) =
      let
        (left_balanced, left_leafs_num) = isBalancedHelper left_subtree
        (right_balanced, right_leafs_num) = isBalancedHelper right_subtree
      in (left_balanced && right_balanced && abs(left_leafs_num - right_leafs_num) <= 1, left_leafs_num + right_leafs_num)
            

tree1a = Leaf 1
tree1b = Node (Leaf 1) (Leaf 2)
tree1c = Node (Node (Leaf 1) (Leaf 2)) (Leaf 3)
tree1d = Node (Leaf 1) (Node (Node (Leaf 2) (Leaf 3)) (Leaf 4))

test1a = isBalanced tree1a
test1b = isBalanced tree1b
test1c = isBalanced tree1c
test1d = isBalanced tree1d


-- Zadanie 2

balance :: [a] -> Tree a
balance [value] = Leaf value
balance list = Node (balance (take m list)) (balance (drop m list))
  where
  m = (length list) `div` 2

test2a = balance [0]
test2b = balance [1,2]
test2c = balance [1,2,3]
test2d = balance [1,2,3,4]
test2e = balance [1,2,3,4,5,6]

