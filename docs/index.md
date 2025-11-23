````markdown
# SnapGit — A Transparent Mini Git (C++)

SnapGit is a compact, readable implementation of a version-control system written in C++. It recreates core Git concepts (objects, commits, branches, merges, index, HEAD) while storing everything in a plain-text `.mygit` folder so learners can inspect and understand the internals.

**Purpose:** teach and demystify how Git-like systems work by exposing repository internals and providing a small, approachable codebase for contributors.

**Not a replacement for Git:** SnapGit is intended as an educational reference and learning tool, not as a full drop-in substitute for `git`.

**Project Website (optional):** https://snapgit.lovable.app

---

**Scope & Audience**
- **Scope:** core VCS primitives — `init`, `add`, `commit`, `branch`, `checkout`, `merge`, `status`, `log`, `reset`, `restore`. Uses a simple object store and plain-text index to show how snapshots and references are managed.
- **Audience:** beginners and developers who want to understand what commits, snapshots, branches, merges, and conflicts actually do behind the scenes. Perfect for learners who run Git commands today without understanding their effects.

---

**How SnapGit Helps Learners**
- Everything is saved in a `.mygit` directory in readable formats. Inspecting those files shows how snapshots, trees, commits, and refs are represented.
- The codebase is intentionally structured so each command lives in `CommandHandlers/` and helpers live in `Helper/`, making it easy to read and extend.

**Learning Mode (planned / contribution idea)**
- A learning feature can be implemented to optionally explain what happened after every command by printing pointers to files under `.mygit` (for example: "See `.mygit/refs/heads/main` for the branch pointer", "See `.mygit/objects/<hash>` for the commit object").
- This feature is not fully implemented yet — see `CONTRIBUTING.md` for suggested implementation and how to contribute it.

---

**Quick Start (build & run)**
- **Windows (recommended):** run the shipped build script in PowerShell:

```powershell
cd <path-to-repo>
.\build.bat
```

- **Manual g++ build:** this repository can be compiled with `g++`. Example (works with MSYS/mingw):

```powershell
g++ .\main.cpp .\CommandHandlers\AddHandler\AddHandler.cpp .\CommandHandlers\InitHandler\InitHandler.cpp ^
  .\CommandHandlers\CommitHandler\CommitHandler.cpp .\CommandHandlers\ConfigHandler\ConfigHandler.cpp ^
  .\CommandHandlers\BranchHandler\BranchHandler.cpp .\CommandHandlers\CheckoutHandler\CheckoutHandler.cpp ^
  .\CommandHandlers\LogHandler\LogHandler.cpp .\CommandHandlers\ResetHandler\ResetHandler.cpp ^
  .\CommandHandlers\StatusHandler\StatusHandler.cpp .\CommandHandlers\RestoreHandler\RestoreHandler.cpp ^
  .\CommandHandlers\RemoveHandler\RemoveHandler.cpp .\CommandHandlers\MergeHandler\MergeHandler.cpp ^
  .\Helper\Hash\Hash.cpp .\Helper\RepoCheck\RepoCheck.cpp .\Helper\SortIndex\SortIndex.cpp ^
  .\Helper\GetCurrentCommitHash\GetCurrentCommitHash.cpp .\Helper\UpdateHead\UpdateHead.cpp ^
  .\Helper\GetHeadRef\GetHeadRef.cpp .\Helper\GetUserInfo\GetUserInfo.cpp ^
  .\Helper\GetAllBranches\GetAllBranches.cpp .\Helper\ReadCommit\ReadCommit.cpp ^
  .\Helper\RestoreTree\RestoreTree.cpp .\Helper\GetAllCommits\GetAllCommits.cpp ^
  .\Helper\ReadIndex\ReadIndex.cpp .\Objects\Blob\Blob.cpp .\Objects\Tree\Tree.cpp .\Objects\Commit\Commit.cpp -o mygit.exe -lssl -lcrypto
```

After building, run the CLI:

```powershell
.\mygit.exe init
.\mygit.exe add file.txt
.\mygit.exe commit -m "first commit"
.\mygit.exe status
```

---

**Commands (high level)**
- **`init`**: create `.mygit` folder and initial metadata.
- **`add`**: stage files into index.
- **`commit`**: create commit object and update refs.
- **`branch`**: create/list branches under `.mygit/refs/heads`.
- **`checkout`**: switch branch or restore tree from commit.
- **`merge`**: attempt to combine branches, write conflict markers when needed.
- **`status`, `log`, `reset`, `restore`, `rm`**: utility commands mirroring common git behaviors.

---

**Project Layout**
- **`main.cpp`**: CLI entrypoint, argument parsing, dispatching to handlers.
- **`CommandHandlers/`**: each command lives in its own folder (`AddHandler/`, `CommitHandler/`, etc.). Look here to add or modify commands.
- **`Helper/`**: helper utilities for hashing, reading/writing index, updating HEAD, etc.
- **`Objects/`**: implementations for `Blob`, `Tree`, `Commit` objects used by the object store.
- **`build.bat`**: Windows build helper (PowerShell friendly).
- **`runInstructions.txt`**: quick compile/run hints.

---

**Inspecting the Repository Internals**
- **`.mygit/objects/<hash>`**: object files (blobs, trees, commits) — open as plain text to inspect contents.
- **`.mygit/index`**: staging area representation.
- **`.mygit/refs/heads/<branch>`**: branch pointers (commit hashes).
- **`.mygit/HEAD`**: reference to the current branch or commit.

Exploring these files after commands shows exactly what changed — that is the learning value of SnapGit.

---

**Learning Mode & Current Status**
- **Current behavior:** SnapGit prints human-friendly messages similar to `git` for successful commands and errors. The codebase does not yet include a fully implemented interactive learning mode.
- **Planned / suggested learning mode:** a command-line flag (for example `--learn` or subcommand `learn on`) that, when enabled, prints after each command a concise explanation and points to specific files inside `.mygit` that demonstrate what changed (e.g., "A new commit object was written to `.mygit/objects/<hash>` — open it to see the tree and message").
- **How contributors can help:** see `CONTRIBUTING.md` for step-by-step guidance and a suggested API hook location where commands can call a `LearningHelper::Explain(command, context)` routine.

---

**Where to Start if You Want to Contribute**
- Read `CONTRIBUTING.md` for development setup and a contribution checklist.
- Explore `CommandHandlers/` and add tests or small enhancements (e.g., better messages, more test coverage).
- Implement the Learning Mode by adding a small library under `Helper/` and invoking it from command dispatch in `main.cpp`.

---

If you'd like, I can now:
- add a `CONTRIBUTING.md` with detailed steps and a suggested implementation for the Learning Mode (I will create it now),
- or implement a minimal Learning Mode prototype in the codebase.

Thank you for building something that helps people learn how version control works!
````
