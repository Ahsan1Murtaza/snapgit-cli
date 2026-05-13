# SnapGit — A Transparent Mini Git (C++)

SnapGit is a compact, readable implementation of a version-control system written in **C++**.

It recreates core Git concepts—**objects, commits, branches, merges, index, and HEAD**—while storing everything in a plain-text `.snapgit` folder so learners can inspect and understand how version control works internally.

Unlike real Git, which stores compressed object files that require commands like `git cat-file` to inspect, SnapGit stores all repository objects as plain text. This allows learners to directly open and trace commits, trees, and blobs without decompression, making Git internals easier to understand.

**Purpose:** Teach and demystify Git internals through a small, approachable codebase.

**Not a replacement for Git:** SnapGit is an educational project

🌐 **Project Website:** https://snapgit.lovable.app

---

# Why SnapGit?

Most developers use Git daily without fully understanding what happens behind commands like:

```bash
git add
git commit
git checkout
git merge
```

SnapGit helps you **see those internals directly**.

After every command, you can inspect:

- `.snapgit/index`
- `.snapgit/objects/<hash>`
- `.snapgit/refs/heads/<branch>`
- `.snapgit/HEAD`

Everything is stored as **readable plain text**, making Git concepts easier to understand.

---

# Features

## Core Git-like Commands

- `init` — Initialize a SnapGit repository  
- `add` — Stage files into the index  
- `commit` — Create commits and snapshots  
- `branch` — Create and list branches  
- `checkout` — Switch branches or restore commits  
- `merge` — Merge branches and handle conflicts  
- `status` — Show repository state  
- `log` — View commit history  
- `reset` — Move branch pointers  
- `restore` — Restore files  
- `rm` — Remove tracked files  

---

## Learning-Friendly Design

### Transparent Object Store

All repository data is stored in:

```bash
.snapgit/
```

with readable contents for exploration.

---

### Plain-Text Internals

Inspect files directly:

```bash
cat .snapgit/HEAD
cat .snapgit/index
cat .snapgit/refs/heads/main
```

---

### Modular Codebase

Commands and internals are organized cleanly for contributors.

---

# Project Structure

```text
SnapGit/
├── CMakeLists.txt
├── LICENSE
├── Readme.md
├── src/
│   ├── cli/
│   │   ├── main.cpp
│   │   ├── commands/
│   │   │   ├── AddHandler.cpp
│   │   │   ├── CommitHandler.cpp
│   │   │   ├── BranchHandler.cpp
│   │   │   ├── CheckoutHandler.cpp
│   │   │   ├── MergeHandler.cpp
│   │   │   ├── StatusHandler.cpp
│   │   │   ├── LogHandler.cpp
│   │   │   ├── ResetHandler.cpp
│   │   │   └── RestoreHandler.cpp
│   │   └── utils/
│   │       ├── Hash.cpp
│   │       ├── ReadIndex.cpp
│   │       ├── ReadTree.cpp
│   │       ├── UpdateHead.cpp
│   │       └── ...
│   └── core/
│       └── objects/
│           ├── Blob.cpp
│           ├── Tree.cpp
│           └── Commit.cpp
└── build/
```

---

# Build Instructions

## Prerequisites

### Ubuntu / Debian

Install OpenSSL headers:

```bash
sudo apt update
sudo apt install -y libssl-dev
```

---

## Build with CMake

```bash
git clone <repo-url>
cd SnapGit

cmake -S . -B build
cmake --build build
```

---

# Running SnapGit

### Linux/macOS

```bash
./build/snapgit init
```


# Example Usage

```bash
./build/snapgit init

echo "hello" > file.txt

./build/snapgit add file.txt

./build/snapgit commit -m "first commit"

./build/snapgit status

./build/snapgit log
```

---

# Inspecting Internals

## HEAD Pointer

```bash
cat .snapgit/HEAD
```

Shows current branch reference.

---

## Branch References

```bash
cat .snapgit/refs/heads/main
```

Shows current commit hash.

---

## Staging Index

```bash
cat .snapgit/index
```

Shows staged files.

---

## Object Files

```bash
ls .snapgit/objects/
cat .snapgit/objects/<hash>
```

Displays blobs, trees, and commit objects.

---

# Commenting Standard (Doxygen + SPDX)

## File Header

Every `.cpp` and `.h` file begins with:

```cpp
// SPDX-License-Identifier: MIT
```

Followed by a short file-purpose comment.

---

## Function Documentation

Use Doxygen comments:

```cpp
/**
 * @brief Creates a new commit object.
 * @param message Commit message.
 * @return Commit hash.
 */
```

### Required Tags

- `@brief`
- `@param`
- `@return` (if non-void)

### Optional Tags

- `@note`

Comments should explain **intent and behavior**, not obvious syntax.

---

# Learning Mode (Planned)

A future optional mode may explain what changed after every command.

Example:

```bash
./snapgit commit -m "test" --learn
```

Output:

```text
Commit object created:
.snapgit/objects/abc123

Branch pointer updated:
.snapgit/refs/heads/main

HEAD still points to:
refs/heads/main
```

This would help users connect commands to repository internals.

---

# Contributing

Contributions are welcome.

Good beginner contributions:

- Improve command behavior
- Add tests
- Improve error messages
- Implement Learning Mode
- Improve documentation

Start by exploring:

```text
src/cli/commands/
src/cli/utils/
src/core/objects/
```

---

# Educational Goal

SnapGit is built to help developers answer:

> **What actually happens when I run Git commands?**

By reading code and inspecting `.snapgit`, learners can understand:

- snapshots  
- commit graphs  
- branch pointers  
- merges  
- conflict markers  
- repository state transitions  

---

# License

MIT License

---

**Thank you for building tools that help people understand version control.**