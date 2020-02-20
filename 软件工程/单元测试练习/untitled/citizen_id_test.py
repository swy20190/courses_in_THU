import unittest
from citizen_id import is_legit_citizen_id


class MyTestCase(unittest.TestCase):
    def test_example_0(self):
        self.assertEqual(is_legit_citizen_id(""), False)

    def test_example_1(self):
        self.assertEqual(is_legit_citizen_id(114514), False)

    def test_example_2(self):
        self.assertEqual(is_legit_citizen_id("21030419991016121X"), True)

    def test_example_3(self):
        self.assertEqual(is_legit_citizen_id(210304199910331216), False)

    def test_example_4(self):
        self.assertEqual(is_legit_citizen_id(100304199910161214), False)

    def test_example_5(self):
        self.assertEqual(is_legit_citizen_id("210304199910161218"), False)

    def test_example_6(self):
        self.assertEqual(is_legit_citizen_id("210304200002291218"), False)


if __name__ == '__main__':
    unittest.main()
