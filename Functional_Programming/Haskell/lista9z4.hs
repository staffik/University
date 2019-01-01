-- Lista 9 (zadanie 4) -  , 

data MtreeL a = MTL a [MtreeL a]
  deriving (Eq,Ord,Show,Read)


-- Zadanie 4

mtreel4a = MTL 0 []
mtreel4b = MTL 0 [MTL 1 []]
mtreel4c = MTL 0 [MTL 1 [], MTL 2 []]
mtreel4d = MTL 0 [MTL 1 [MTL 2 [], MTL 3 []], MTL 4 []]


-- (a)

preorderMTL :: MtreeL a -> [a]
preorderMTL mtreel = reverse (visitSubtree mtreel [])
  where
    visitSubtree :: MtreeL a -> [a] -> [a]
    visitSubtree (MTL value sons) acc = visitSons sons (value:acc)

    visitSons :: [MtreeL a] -> [a] -> [a]
    visitSons [] acc = acc
    visitSons (h:t) acc = visitSons t (visitSubtree h acc)

test4aa = preorderMTL mtreel4a
test4ab = preorderMTL mtreel4b
test4ac = preorderMTL mtreel4c
test4ad = preorderMTL mtreel4d


-- (b)

breadthMTL :: MtreeL a -> [a]
breadthMTL mtreel = reverse (visitNodes [mtreel] [] [])
  where
    visitNodes :: [MtreeL a] -> [MtreeL a] -> [a] -> [a]
    visitNodes [] [] acc = acc
    visitNodes [] sons acc = visitNodes sons [] acc
    visitNodes ((MTL value children):t) sons acc = visitNodes t (children++sons) (value:acc)

test4ba = breadthMTL mtreel4a
test4bb = breadthMTL mtreel4b
test4bc = breadthMTL mtreel4c
test4bd = breadthMTL mtreel4d

