# Implementation Roadmap

This guide walks you through building the B-Tree Key-Value Store step-by-step.

---

## PHASE 1: In-Memory B-Tree

**Goal**: Implement a working B-tree that stores key-value pairs in memory.

### Step 1.1: Define B-Tree Node Structure

**File**: `include/btree/node.h`

```cpp
class BTreeNode {
private:
    bool is_leaf;           // Is this a leaf node?
    int num_keys;           // Current number of keys
    int *keys;              // Array of keys
    std::string *values;    // Array of values (only for leaf nodes)
    BTreeNode **children;   // Array of child pointers
    int max_degree;         // Maximum number of children (t)

public:
    BTreeNode(int degree, bool leaf);
    ~BTreeNode();

    // Core operations
    void insertNonFull(int key, const std::string& value);
    void splitChild(int index, BTreeNode *child);
    BTreeNode* search(int key, std::string& value);
    // ... more methods
};
```

**What to learn**:
- B-tree properties: minimum degree `t`, nodes have [t-1, 2t-1] keys
- Difference between internal nodes and leaf nodes
- Dynamic memory management in C++

**Implementation tasks**:
1. Create the Node class with constructor/destructor
2. Handle memory allocation for arrays
3. Implement basic getters/setters

---

### Step 1.2: Implement B-Tree Class

**File**: `include/btree/btree.h`

```cpp
class BTree {
private:
    BTreeNode *root;
    int min_degree;  // Minimum degree (t)

public:
    BTree(int degree);
    ~BTree();

    // Public interface
    void insert(int key, const std::string& value);
    bool search(int key, std::string& value);
    void remove(int key);
    void traverse();  // For debugging
};
```

**Implementation tasks**:
1. Create empty B-tree with just a root
2. Implement `insert()`:
   - If root is full, split it
   - Call `insertNonFull()` on appropriate node
3. Implement `search()`:
   - Traverse from root to leaf
   - Handle not found case
4. Implement `traverse()` for printing tree

**What to learn**:
- Tree traversal algorithms
- Recursive vs iterative approaches
- When to split nodes (proactive vs reactive)

**Testing**:
```cpp
BTree tree(3);  // Minimum degree 3
tree.insert(10, "value10");
tree.insert(20, "value20");
tree.insert(5, "value5");

std::string result;
if (tree.search(10, result)) {
    std::cout << "Found: " << result << std::endl;
}
```

---

### Step 1.3: Implement Node Split

**Critical algorithm**: When a node becomes full (2t-1 keys), split it.

**Pseudocode**:
```
splitChild(parent, index):
    full_child = parent.children[index]
    new_node = create new node

    // Move half the keys to new node
    mid_key = full_child.keys[t-1]

    // Move keys [t..2t-1] to new node
    copy full_child.keys[t..2t-1] to new_node.keys[0..t-1]

    // If not leaf, move children too
    if not leaf:
        copy full_child.children[t..2t] to new_node.children[0..t]

    // Push mid_key up to parent
    insert mid_key into parent.keys at position 'index'
    insert new_node into parent.children at position 'index+1'

    // Update counts
    full_child.num_keys = t-1
    new_node.num_keys = t-1
```

**What to learn**:
- Array manipulation in C++
- Memory management (copying vs moving)
- Edge cases: splitting root vs internal node

---

### Step 1.4: Implement Search

**File**: `src/btree/node.cpp`

```cpp
BTreeNode* BTreeNode::search(int key, std::string& value) {
    // Find the first key >= search key
    int i = 0;
    while (i < num_keys && key > keys[i]) {
        i++;
    }

    // Found exact match
    if (i < num_keys && keys[i] == key) {
        if (is_leaf) {
            value = values[i];
            return this;
        }
        // For internal node, continue to child
    }

    // Key not in this node
    if (is_leaf) {
        return nullptr;  // Not found
    }

    // Recurse to appropriate child
    return children[i]->search(key, value);
}
```

**What to learn**:
- Linear search in sorted array
- Binary search optimization (optional)
- Recursive tree traversal

---

### Step 1.5: Create CLI Interface

**File**: `src/main.cpp`

```cpp
int main() {
    BTree tree(3);  // Degree 3 B-tree

    std::string command;
    while (true) {
        std::cout << "kvstore> ";
        std::cin >> command;

        if (command == "PUT") {
            int key;
            std::string value;
            std::cin >> key >> value;
            tree.insert(key, value);
            std::cout << "OK\n";

        } else if (command == "GET") {
            int key;
            std::cin >> key;
            std::string value;
            if (tree.search(key, value)) {
                std::cout << value << "\n";
            } else {
                std::cout << "NOT_FOUND\n";
            }

        } else if (command == "EXIT") {
            break;
        }
    }

    return 0;
}
```

**Testing**:
```
kvstore> PUT 10 hello
OK
kvstore> PUT 20 world
OK
kvstore> GET 10
hello
kvstore> GET 30
NOT_FOUND
```

---

## PHASE 2: Disk Persistence

**Goal**: Store B-tree nodes on disk instead of memory.

### Step 2.1: Design Page-Based Storage

**Key concept**: Disk I/O happens in fixed-size blocks called "pages" (typically 4KB or 8KB).

**File**: `include/storage/page.h`

```cpp
constexpr size_t PAGE_SIZE = 4096;  // 4KB pages

struct Page {
    char data[PAGE_SIZE];

    // Metadata
    int page_id;
    bool is_dirty;  // Has been modified?
    int pin_count;  // Number of users
};
```

**What to learn**:
- Why databases use fixed-size pages
- Page alignment and padding
- Difference between logical page ID and physical offset

---

### Step 2.2: Implement Disk Manager

**File**: `include/storage/disk_manager.h`

```cpp
class DiskManager {
private:
    std::fstream db_file;
    std::string file_name;

public:
    DiskManager(const std::string& db_file_name);
    ~DiskManager();

    void readPage(int page_id, char *page_data);
    void writePage(int page_id, const char *page_data);
    int allocatePage();  // Get a new page ID
    void deallocatePage(int page_id);
};
```

**Implementation**:
```cpp
void DiskManager::readPage(int page_id, char *page_data) {
    // Calculate offset
    off_t offset = page_id * PAGE_SIZE;

    // Seek to position
    db_file.seekg(offset);

    // Read PAGE_SIZE bytes
    db_file.read(page_data, PAGE_SIZE);

    if (db_file.bad()) {
        throw std::runtime_error("I/O error while reading");
    }
}

void DiskManager::writePage(int page_id, const char *page_data) {
    off_t offset = page_id * PAGE_SIZE;
    db_file.seekp(offset);
    db_file.write(page_data, PAGE_SIZE);
    db_file.flush();  // Ensure it's written to disk
}
```

**What to learn**:
- File I/O in C++ (fstream)
- Seeking to specific offsets
- Error handling for I/O operations

---

### Step 2.3: Implement Serialization

**File**: `include/utils/serializer.h`

You need to convert B-tree nodes to/from byte arrays.

```cpp
class Serializer {
public:
    // Serialize node to page
    static void serializeNode(BTreeNode *node, char *page_data);

    // Deserialize page to node
    static BTreeNode* deserializeNode(const char *page_data);
};
```

**Page layout example**:
```
Offset | Size | Field
-------|------|-------
0      | 1    | is_leaf (boolean)
1      | 4    | num_keys (int)
5      | 4    | max_degree (int)
9      | 4*n  | keys array
...    | var  | values (if leaf) or child page IDs
```

**What to learn**:
- Memory layout and alignment
- Serialization techniques
- Fixed vs variable-length data

---

### Step 2.4: Implement Buffer Pool Manager

**File**: `include/storage/page_manager.h`

The buffer pool caches frequently accessed pages in memory.

```cpp
class BufferPoolManager {
private:
    Page *pages;              // Array of pages
    std::unordered_map<int, int> page_table;  // page_id -> frame_id
    std::list<int> free_list;  // Available frames
    int pool_size;
    DiskManager *disk_manager;

public:
    BufferPoolManager(int pool_size, DiskManager *disk_mgr);
    ~BufferPoolManager();

    Page* fetchPage(int page_id);
    bool unpinPage(int page_id, bool is_dirty);
    bool flushPage(int page_id);
    Page* newPage(int &page_id);
    bool deletePage(int page_id);
};
```

**What to learn**:
- LRU/Clock page replacement algorithms
- Pin/unpin semantics
- When to write dirty pages back to disk

---

### Step 2.5: Modify B-Tree to Use Disk

Now modify your B-tree to work with the buffer pool instead of `new`/`delete`.

**Before (in-memory)**:
```cpp
BTreeNode *child = new BTreeNode(degree, true);
```

**After (disk-based)**:
```cpp
int page_id;
Page *page = buffer_pool->newPage(page_id);
BTreeNode *child = Serializer::deserializeNode(page->data);
child->page_id = page_id;
```

**Key changes**:
1. Store `page_id` instead of pointers
2. Fetch pages through buffer pool
3. Serialize before unpinning
4. Handle page pins/unpins correctly

---

## PHASE 3: Network Layer

**Goal**: Allow remote clients to connect via TCP and execute commands.

### Step 3.1: Design Protocol

Simple text protocol:
```
Client -> Server: "PUT 10 hello\n"
Server -> Client: "+OK\n"

Client -> Server: "GET 10\n"
Server -> Client: "+hello\n"

Client -> Server: "GET 99\n"
Server -> Client: "-NOT_FOUND\n"
```

**What to learn**:
- Protocol design principles
- Text vs binary protocols
- Message framing (using newlines)

---

### Step 3.2: Implement TCP Server

**File**: `include/network/server.h`

```cpp
class Server {
private:
    int server_fd;
    int port;
    BTree *tree;
    bool running;

public:
    Server(int port, BTree *tree);
    ~Server();

    void start();
    void stop();
    void handleClient(int client_fd);
};
```

**Implementation**:
```cpp
void Server::start() {
    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind to port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    // Accept loop
    while (running) {
        int client_fd = accept(server_fd, nullptr, nullptr);

        // Handle in new thread
        std::thread(&Server::handleClient, this, client_fd).detach();
    }
}
```

**What to learn**:
- Socket programming (socket, bind, listen, accept)
- Network byte order
- Multi-threaded servers

---

### Step 3.3: Handle Concurrent Access

**Problem**: Multiple threads accessing B-tree simultaneously.

**Solution**: Add read-write locks.

```cpp
class BTree {
private:
    std::shared_mutex tree_mutex;

public:
    void insert(int key, const std::string& value) {
        std::unique_lock lock(tree_mutex);  // Write lock
        // ... insertion logic
    }

    bool search(int key, std::string& value) {
        std::shared_lock lock(tree_mutex);  // Read lock
        // ... search logic
    }
};
```

**What to learn**:
- Reader-writer locks
- Deadlock prevention
- Lock granularity (coarse vs fine-grained)

---

## PHASE 4: Write-Ahead Logging

**Goal**: Ensure durability and enable crash recovery.

### Step 4.1: Design Log Format

**Log record structure**:
```cpp
struct LogRecord {
    uint64_t lsn;          // Log sequence number
    uint64_t txn_id;       // Transaction ID
    LogRecordType type;    // INSERT, UPDATE, DELETE, COMMIT
    int key;
    std::string old_value;
    std::string new_value;
};
```

### Step 4.2: Implement WAL

**File**: `include/storage/wal.h`

```cpp
class WriteAheadLog {
private:
    std::fstream log_file;
    uint64_t next_lsn;

public:
    uint64_t appendLog(const LogRecord& record);
    void flush();
    std::vector<LogRecord> readAll();
};
```

**Rule**: Write log BEFORE modifying data.

```cpp
void BTree::insert(int key, const std::string& value) {
    // 1. Write to log
    LogRecord record;
    record.type = LogRecordType::INSERT;
    record.key = key;
    record.new_value = value;
    wal->appendLog(record);
    wal->flush();  // Ensure it's on disk

    // 2. Modify data
    insertInternal(key, value);
}
```

**What to learn**:
- WAL protocol
- Force-log-at-commit rule
- Recovery algorithms (ARIES)

---

## PHASE 5: Advanced Features

### Range Queries
```cpp
std::vector<std::pair<int, std::string>> BTree::range(int start, int end);
```

### Transactions
```cpp
class Transaction {
    uint64_t txn_id;
    std::vector<LogRecord> undo_log;

    void begin();
    void commit();
    void abort();
};
```

---

## PHASE 6: Distribution (Future)

This is where you'd add:
- Consistent hashing for data partitioning
- Replication protocol (leader-follower or multi-master)
- Gossip protocol for node discovery
- Quorum-based reads/writes
- Anti-entropy mechanisms

---

## Testing Strategy

### Unit Tests
Test each component in isolation:
```cpp
TEST(BTreeTest, InsertAndSearch) {
    BTree tree(3);
    tree.insert(10, "ten");
    std::string result;
    ASSERT_TRUE(tree.search(10, result));
    ASSERT_EQ(result, "ten");
}
```

### Integration Tests
Test full system:
```bash
# Start server
./kvstore --port 8080

# In another terminal
echo "PUT 10 hello" | nc localhost 8080
echo "GET 10" | nc localhost 8080
```

### Stress Tests
```cpp
// Insert 1 million keys
for (int i = 0; i < 1000000; i++) {
    tree.insert(i, "value" + std::to_string(i));
}
```

---

## Debugging Tips

1. **Print the tree**: Implement `traverse()` to visualize structure
2. **Use assertions**: Check invariants frequently
3. **Log everything**: Add debug logs for page fetches, splits, etc.
4. **Valgrind**: Check for memory leaks
5. **GDB**: Step through node splits carefully

---

## Performance Benchmarks

Track these metrics:
- Insertions per second
- Search latency (avg, p95, p99)
- Disk I/O (reads/writes per operation)
- Buffer pool hit rate
- Tree height vs number of keys

---

## Next Steps

1. Start with Phase 1.1 - implement the node structure
2. Test each step before moving to the next
3. Don't skip the CLI - it's useful for debugging
4. Read about B-trees from the resources in README.md

Good luck! This is a challenging but rewarding project.
