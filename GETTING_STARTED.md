# Getting Started

This guide helps you start implementing the B-Tree Key-Value Store.

## Your First Steps

### 1. Verify Project Structure

You should have these directories:
```
cpp-distributed-systems/
├── CMakeLists.txt          (build configuration)
├── README.md               (project overview)
├── ROADMAP.md              (detailed implementation guide)
├── include/                (header files)
│   ├── btree/
│   ├── storage/
│   ├── network/
│   └── utils/
├── src/                    (implementation files)
│   ├── btree/
│   ├── storage/
│   ├── network/
│   └── utils/
└── tests/                  (test files)
```

### 2. Start with Phase 1: In-Memory B-Tree

Read `ROADMAP.md` - Phase 1 for detailed guidance.

**Your first task**: Implement the B-Tree Node

Create these files:
- `include/btree/node.h` - Node class declaration
- `src/btree/node.cpp` - Node class implementation

**Key questions to answer in your design**:
- What data does a node need to store?
- How will you differentiate leaf nodes from internal nodes?
- What's your B-tree degree (t)? Start with t=3 for easier debugging

### 3. Build and Test as You Go

After creating each component:

```bash
cd cpp-distributed-systems
mkdir build
cd build
cmake ..
make
```

Fix compilation errors before moving to the next step.

### 4. Recommended Order of Implementation

**Week 1: Basic B-Tree**
1. BTreeNode class (header + implementation)
2. BTree class (header + implementation)
3. Insert operation
4. Search operation
5. Simple CLI in main.cpp

**Week 2: Make It Robust**
1. Node splitting logic
2. Traverse/print for debugging
3. Handle edge cases
4. Test with various inputs

**Week 3: Disk Persistence**
1. Page abstraction
2. DiskManager for I/O
3. Serialization logic
4. Modify B-tree to use pages

**Week 4+: Networking**
1. TCP server basics
2. Protocol handling
3. Multi-threading
4. Concurrent access control

## How to Use Me (Claude)

As you implement:

1. **Before coding**: "I'm about to implement BTreeNode::search(). Does my approach make sense?"
2. **During coding**: "I'm getting a segfault in splitChild(). Here's my code..."
3. **After coding**: "Can you review my insert() implementation?"
4. **Debugging**: "The tree structure looks wrong after inserting these keys..."

## Tips

- **Start small**: Get a single insert and search working first
- **Print everything**: Add debug output to see what's happening
- **Draw it out**: Sketch the tree structure on paper
- **Test incrementally**: Don't write 500 lines before testing
- **Read ROADMAP.md carefully**: It has pseudocode and explanations

## Common Mistakes to Avoid

1. Not handling array bounds properly
2. Forgetting to update `num_keys` after modifications
3. Memory leaks (forgetting to delete[] arrays)
4. Off-by-one errors in split logic
5. Not testing with degree t=3 (easier to debug than larger values)

## Resources

Before implementing, read about B-trees:
- Visualization: https://www.cs.usfca.edu/~galles/visualization/BTree.html
- Play with different values of t (degree)
- Watch how splits happen
- Understand search path

## Next Step

Go read `ROADMAP.md` Section "Phase 1: Step 1.1" and create your node.h file!

When you're ready, show me your design and I'll give you feedback before you implement it.
