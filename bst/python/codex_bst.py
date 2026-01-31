class BSTNode:
    def __init__(self, key):
        self.key = key
        self.left = None
        self.right = None


class BSTree:
    def __init__(self):
        self.root = None
        self.size = 0

    def is_empty(self):
        return self.size == 0

    def contains(self, key):
        node = self.root
        while node is not None:
            if key < node.key:
                node = node.left
            elif key > node.key:
                node = node.right
            else:
                return True
        return False

    def insert(self, key):
        if self.root is None:
            self.root = BSTNode(key)
            self.size = 1
            return True

        parent = None
        node = self.root
        while node is not None:
            parent = node
            if key < node.key:
                node = node.left
            elif key > node.key:
                node = node.right
            else:
                return False

        new_node = BSTNode(key)
        if key < parent.key:
            parent.left = new_node
        else:
            parent.right = new_node
        self.size += 1
        return True

    def _detach_min(self, node):
        if node.left is None:
            return node, node.right
        min_node, node.left = self._detach_min(node.left)
        return min_node, node

    def _remove_node(self, node, key):
        if node is None:
            return None, False
        if key < node.key:
            node.left, removed = self._remove_node(node.left, key)
            return node, removed
        if key > node.key:
            node.right, removed = self._remove_node(node.right, key)
            return node, removed

        if node.left is None:
            return node.right, True
        if node.right is None:
            return node.left, True

        min_node, node.right = self._detach_min(node.right)
        node.key = min_node.key
        return node, True

    def remove(self, key):
        self.root, removed = self._remove_node(self.root, key)
        if removed:
            self.size -= 1
        return removed

    def _in_order(self, node, visit, ctx):
        if node is None:
            return
        self._in_order(node.left, visit, ctx)
        visit(node, ctx)
        self._in_order(node.right, visit, ctx)

    def in_order(self, visit, ctx=None):
        if visit is None:
            return
        self._in_order(self.root, visit, ctx)

    def _pre_order(self, node, visit, ctx):
        if node is None:
            return
        visit(node, ctx)
        self._pre_order(node.left, visit, ctx)
        self._pre_order(node.right, visit, ctx)

    def pre_order(self, visit, ctx=None):
        if visit is None:
            return
        self._pre_order(self.root, visit, ctx)

    def _post_order(self, node, visit, ctx):
        if node is None:
            return
        self._post_order(node.left, visit, ctx)
        self._post_order(node.right, visit, ctx)
        visit(node, ctx)

    def post_order(self, visit, ctx=None):
        if visit is None:
            return
        self._post_order(self.root, visit, ctx)

    def min_key(self):
        if self.root is None:
            return None
        node = self.root
        while node.left is not None:
            node = node.left
        return node.key

    def max_key(self):
        if self.root is None:
            return None
        node = self.root
        while node.right is not None:
            node = node.right
        return node.key
