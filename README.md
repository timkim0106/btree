# B-Tree Key-Value Store

A disk-based key-value storage system built from scratch in C++ to learn fundamental systems concepts.

## Learning Objectives

This project teaches:
- **Operating Systems**: Multi-threading, file I/O, memory management, process coordination
- **Networking**: TCP/IP sockets, client-server architecture, protocol design
- **Databases**: B-tree indexing, disk persistence, WAL, transaction management
- **Distributed Systems**: (Future phases) Replication, consistency, fault tolerance

## Project Phases

### Phase 1: In-Memory B-Tree (START HERE)
- Implement B-tree data structure
- Basic operations: INSERT, SEARCH, DELETE
- Simple command-line interface

### Phase 2: Disk Persistence
- Page-based storage abstraction
- Serialize B-tree nodes to disk
- Buffer pool management
- Simple recovery mechanism

### Phase 3: Network Layer
- TCP server for remote clients
- Custom protocol (text-based)
- Multi-threaded connection handling
- Concurrent access control

### Phase 4: Write-Ahead Logging
- WAL implementation for durability
- Crash recovery
- Checkpoint mechanism

### Phase 5: Advanced Features
- Range queries
- Transactions (ACID properties)
- Query optimization

### Phase 6: Distribution (Optional)
- Multi-node architecture
- Replication
- Consistency protocols
- Failure detection

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./kvstore
```

## Architecture

```
┌─────────────────────────────────────┐
│         Client Interface            │
│    (CLI / Network Protocol)         │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│      B-Tree Query Engine            │
│   (INSERT/SEARCH/DELETE/RANGE)      │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│       Buffer Pool Manager           │
│    (In-memory page cache)           │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│       Disk Manager                  │
│    (Page I/O, file management)      │
└─────────────────────────────────────┘
```

## Key Concepts You'll Learn

1. **B-Tree**: Self-balancing tree optimized for disk access
2. **Page-based storage**: Fixed-size blocks for efficient I/O
3. **Buffer pool**: LRU cache for frequently accessed pages
4. **WAL**: Ensuring durability and enabling recovery
5. **Concurrency**: Latches and locks for multi-threaded access
6. **Networking**: Socket programming and protocol design

## Resources

- [B-Tree Visualization](https://www.cs.usfca.edu/~galles/visualization/BTree.html)
- [CMU Database Course](https://15445.courses.cs.cmu.edu/)
- [Database Internals Book](https://www.databass.dev/)

See `ROADMAP.md` for detailed implementation guide.
