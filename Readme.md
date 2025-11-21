# SnapGit — A Custom Version Control System (C++)

SnapGit is a fully functional miniature version control system written from scratch in C++.  
It recreates the core mechanics of Git — staging, committing, branching, merging, conflict detection, and repository state management — but in a simplified, transparent form ideal for learning.

If you've ever wanted to understand *how Git actually works under the hood*, SnapGit shows you every moving part.



## 🔥 Why SnapGit Exists

Git is powerful — but most beginners fear it.

They struggle with:
- What is a commit?
- What does “snapshot” really mean?
- How does branching work?
- What happens during a merge?
- How do conflicts appear?

SnapGit removes the mystery.  
Nothing is hidden. Everything is stored in plain text inside a `.mygit` folder.  
You can literally *open the files* and see the internals.

SnapGit is **not** a replacement for Git.  
It’s a **learning tool**, a **reference**, and a **clean, readable implementation** of a VCS.

---

## 🚀 Features

### Repository Operations  
- `init` — Create a new repository  
- `status` — View working directory vs staged vs committed changes  

### Staging  
- `add` — Stage files  
- `rm` — Remove files (working directory or index)  

### Commits & Object Storage  
- `commit` — Create snapshots  
- `log` — Explore commit history  

### Branching & Navigation  
- `branch` — Create/list branches  
- `checkout` — Switch branches or checkout commits  

### Merge System  
- `merge` — Merge branches  
- Automatic conflict detection  
- Conflict markers written inside files  

### Safety Tools  
- `reset` — Move HEAD to previous commit  
- `restore` — Restore files from index or HEAD  

### Plumbing  
- File hashing  
- Object storage  
- Tree & commit structure  
- HEAD pointers  
- Branch pointers  
- Index file  

This is a complete, fully working mini Git.



## 📥 Installation Options

You can use SnapGit in **two different ways**:



## ✅ **Option 1 — Download Installer (Recommended)**

Visit the official website below and download the latest release:

👉 **Website:** https://snapgit.lovable.app  
👉 **Download Installer:** *(available on website)*

Install once → SnapGit is added to PATH → Ready to use anywhere.



## ✅ **Option 2 — Clone the Repository**

```bash
git clone https://github.com/Ahsan1Murtaza/SnapGit.git
cd SnapGit