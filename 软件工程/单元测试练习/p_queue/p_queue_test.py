import unittest
from p_queue import *

q1 = []
l2 = list(range(1000))
l2.reverse
l3 = list(range(3, 10))
l4 = []
l5 = list(range(3, 10))

l6 = list(range(1, 100000))
l7 = list(range(1, 1000000))
l8 = list(range(1, 10000000))
l9 = list(range(1, 20000000))
l10 = list(range(1, 40000000))


class MyTestCase(unittest.TestCase):
    def test_example0(self):
        self.assertEqual(heappop(q1), False)

    def test_example1(self):
        self.assertEqual(heapreplace(q1, 233), False)

    def test_example2(self):
        heapify(l2)
        for n in range(1000):
            self.assertEqual(heappop(l2), n)

    def test_example3(self):
        heapify(l3)
        self.assertEqual(heapreplace(l3, 1), 3)
        self.assertEqual(heappop(l3), 1)

    def test_example4(self):
        tmp = list(range(100))
        tmp.reverse()
        for n in tmp:
            heappush(l4, n)
        for n in range(100):
            self.assertEqual(heappop(l4), n)

    def test_example5(self):
        self.assertEqual(heappushpop(l5, 4.4), 3)
        self.assertEqual(heappop(l5), 4)
        self.assertEqual(heappop(l5), 4.4)

    def test_example6(self):
        self.assertEqual(heappush(l6, 100000), None)

    def test_example7(self):
        self.assertEqual(heappush(l7, 1000000), None)

    def test_example8(self):
        self.assertEqual(heappush(l8, 10000000), None)

    def test_example9(self):
        self.assertEqual(heappush(l9, 20000000), None)

    def test_example10(self):
        self.assertEqual(heappush(l10, 400000000), None)


if __name__ == '__main__':
    unittest.main()
