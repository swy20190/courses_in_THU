import unittest
from m_tree import *

node01 = Node(1)
node02 = Node(2)
node03 = Node(3)
node01.left = node02
node01.right = node03

node11 = Node(1)
node12 = Node(2)
node13 = Node(3)
node14 = Node(4)
node11.left = node12
node11.right = node13
node13.right = node14

node21 = Node(1)
node22 = Node(2)
node23 = Node(3)
node24 = Node(4)
node25 = Node(5)
node26 = Node(6)
node27 = Node(7)
node21.left = node22
node21.right = node25
node22.left = node23
node22.right = node24
node25.left = node26
node25.right = node27

node31 = Node(1)
node32 = Node(2)
node33 = Node(3)
node34 = Node(4)
node31.left = node32
node32.left = node33
node33.left = node34

node41 = Node(1)
node42 = Node(2)
node43 = Node(3)
node44 = Node(4)
node41.right = node42
node42.right = node43
node43.right = node44


class MyTestCase(unittest.TestCase):
    def test_example_0(self):
        base = 1
        for n in MorrisTraversalInner(node01):
            self.assertEqual(n, base)
            base = base+1

    def test_example_1(self):
        base = 1
        for n in MorrisTraversalInner(node11):
            self.assertEqual(n, base)
            base = base+1

    def test_example_2(self):
        base = 1
        for n in MorrisTraversalInner(node21):
            self.assertEqual(n, base)
            base = base+1

    def test_example_3(self):
        base = 1
        for n in MorrisTraversalInner(node31):
            self.assertEqual(n, base)
            base = base+1

    def test_example_4(self):
        base = 1
        for n in MorrisTraversalInner(node41):
            self.assertEqual(n, base)
            base = base+1



if __name__ == '__main__':
    unittest.main()
