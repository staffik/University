-- Lista 9 (zadanie 3) -  , 

data BTree a = Leaf | Node (BTree a) a (BTree a)
  deriving (Eq,Ord,Show,Read)


-- Zadanie 3

btree3a = Leaf
btree3b = Node Leaf 0 Leaf
btree3c = Node Leaf 0 (Node Leaf 1 Leaf)
btree3d = Node (Node Leaf 1 Leaf) 0 (Node Leaf 1 Leaf)


-- (a)

inPathLen :: BTree a -> Int
inPathLen btree = snd (inPathLenHelper btree)
  where
    inPathLenHelper :: BTree a -> (Int, Int)
    inPathLenHelper Leaf = (0, 0)
    inPathLenHelper (Node left_subtree _ right_subtree) =
      let
        (left_nonleafs_no, left_in_path_len) = inPathLenHelper left_subtree
        (right_nonleafs_no, right_in_path_len) = inPathLenHelper right_subtree
      in (left_nonleafs_no + 1 + right_nonleafs_no, left_in_path_len + right_in_path_len + left_nonleafs_no + right_nonleafs_no)

test3aa = inPathLen btree3a
test3ab = inPathLen btree3b
test3ac = inPathLen btree3c
test3ad = inPathLen btree3d


-- (b)

outPathLen :: BTree a -> Int
outPathLen btree = snd (outPathLenHelper btree)
  where
    outPathLenHelper :: BTree a -> (Int, Int)
    outPathLenHelper Leaf = (1, 0)
    outPathLenHelper (Node left_subtree _ right_subtree) =
      let
        (left_leafs_no, left_out_path_len) = outPathLenHelper left_subtree
        (right_leafs_no, right_out_path_len) = outPathLenHelper right_subtree
      in (left_leafs_no + right_leafs_no, left_out_path_len + right_out_path_len + left_leafs_no + right_leafs_no)

test3ba = outPathLen btree3a
test3bb = outPathLen btree3b
test3bc = outPathLen btree3c
test3bd = outPathLen btree3d

