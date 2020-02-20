# Python program for Morris Preorder traversal

# A binary tree Node
class Node:
    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None


# Preorder traversal without
# recursion and without stack
def MorrisTraversalInner(root):
    curr = root

    while curr:
        # If left child is null, print the
        # current node data. And, update
        # the current pointer to right child.
        if curr.left is None:
            yield curr.data
            curr = curr.right

        else:
            # Find the inorder predecessor
            prev = curr.left

            while prev.right is not None and prev.right is not curr:
                prev = prev.right

            # If the right child of inorder
            # predecessor already points to
            # the current node, update the
            # current with it's right child
            if prev.right is curr:
                prev.right = None
                curr = curr.right

            # else If right child doesn't point
            # to the current node, then print this
            # node's data and update the right child
            # pointer with the current node and update
            # the current with it's left child
            else:
                yield curr.data
                prev.right = curr
                curr = curr.left


# # Reference: recursive traversal
# def RecursiveTraversal(root):
#     if root:
#         yield root.data
#         yield from RecursiveTraversal(root.left)
#         yield from RecursiveTraversal(root.right)
