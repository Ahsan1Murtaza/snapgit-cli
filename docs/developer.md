````markdown
# Developer Guide — SnapGit

This document explains the internal design of SnapGit for contributors who want to read, extend, or debug the codebase.

**Goal:** make it easy for a developer to find the code to change, understand data flow, and add features (especially the planned Learning Mode).

---

**1. High-level architecture**
- `main.cpp` is the CLI entrypoint. It parses arguments and dispatches to command handlers.
- `CommandHandlers/` implements each user command as a pair of files (e.g., `AddHandler.cpp` / `AddHandler.h`). Handlers implement command-specific logic and call helpers.
- `Helper/` contains utilities for hashing, reading/writing the index, ref management, and other plumbing.
- `Objects/` contains object types used by the object store: `Blob`, `Tree`, and `Commit`.
- Repository state is represented on-disk under `.mygit` (objects, refs, index, HEAD). SnapGit intentionally stores things in human-readable form so learners can inspect them.

Diagram (simple):
- CLI (`main.cpp`) → Command Handler (`CommandHandlers/<X>Handler`) → Helper utilities (`Helper/*`) → write/read `.mygit` (`Objects/*`) → return status/message to CLI

---

**2. On-disk model (.mygit)**
- `.mygit/objects/<hash>` — stored objects (blobs, trees, commits). Open them to see contents.
- `.mygit/index` — staging area representation. This file is updated by `add` and used by `commit`.
- `.mygit/refs/heads/<branch>` — branch pointers (files with commit hashes).
- `.mygit/HEAD` — pointer to current branch or commit id.

Inspect these files after commands to see exactly what the command did — this is the main learning value.

---

**3. Command flow & where to look**
- Entry point: `main.cpp` — locate the argument parsing and command dispatch code. This is a good place to add global hooks (e.g., a Learning Mode toggle) because every command is routed through it.
- Command handlers: `CommandHandlers/<Command>Handler/` — open the handler for the command you care about. Each handler typically:
  - validates inputs
  - calls helpers to read/write index/objects
  - writes updated refs or objects
  - prints user-facing messages
- Helpers: `Helper/*` — reusable operations like hashing (`Helper/Hash/Hash.cpp`), reading HEAD (`Helper/GetHeadRef/GetHeadRef.cpp`), updating HEAD (`Helper/UpdateHead/UpdateHead.cpp`), reading commits (`Helper/ReadCommit/ReadCommit.cpp`), etc.
- Objects: `Objects/*` — low-level object serialization/deserialization.

Common change pattern:
1. Add/modify logic in `CommandHandlers/<X>Handler/<X>Handler.cpp`.
2. If needed, add or reuse helper functions in `Helper/`.
3. Update object code in `Objects/` only if adding new object types or changing serialization.

---

**4. How to add a new command**
1. Create `CommandHandlers/MyCommandHandler/MyCommandHandler.h` and `.cpp` by copying an existing handler as a template.
2. Register the command in `main.cpp` (where commands are dispatched). Follow existing parsing style.
3. Add helper functions to `Helper/` if you need to share logic.
4. Update `Readme.md` to document the new command and add small manual test steps.
5. Build with `build.bat` and run manual tests.

Tip: keep new commands focused and small. Provide manual testing steps in your PR description.

---

**5. Adding the Learning Mode (detailed guide)**
Goal: after a command succeeds (when learning mode is enabled), print a short explanation and point to the exact `.mygit` files that demonstrate what changed.

Suggested implementation (small, incremental):

Step A — Add `Helper/LearningHelper` files
- Add `Helper/LearningHelper/LearningHelper.h` and `Helper/LearningHelper/LearningHelper.cpp`.
- Minimal public API:
  - `struct LearningContext { std::string command; std::string commitHash; std::vector<std::string> affectedFiles; std::string branch; /* other fields as needed */ }`
  - `class LearningHelper { public: static void Explain(const LearningContext &ctx); }`

Step B — Add CLI toggle
- Add a global `bool learningMode = false;` and a command-line flag `--learn` or a subcommand `learn on|off` in `main.cpp` parsing. When `--learn` is present, set `learningMode = true`.

Step C — Populate context & call helper
- After a command handler finishes successfully, build a `LearningContext` instance with details available from that handler or helpers (commit hash, branch name, affected file names).
- Call `if (learningMode) LearningHelper::Explain(ctx);` from the central dispatcher in `main.cpp` so the logic is centralized rather than repeated in every handler.

Step D — Implement concise explanations
- For each command type, `Explain()` should print 2–4 short lines pointing to the files in `.mygit` you want the learner to inspect. Example for `commit`:

```
Committed: 3a1b2c4
See: .mygit/objects/3a1b2c4   -> commit object (message, author, tree)
See: .mygit/objects/<tree-hash> -> snapshot for that commit
See: .mygit/refs/heads/main   -> branch pointer updated
```

Design rules:
- `LearningHelper` must not mutate repository state — only read `.mygit` and print guidance.
- Keep explanations short and actionable.
- Use relative file paths so instructions work on any repo clone.

Where to hook the helper:
- Preferred: `main.cpp` right after the handler returns success status. This centralizes logic and keeps handlers focused on state changes.

---

**6. Build, debug, and test**
- Build (PowerShell):

```powershell
cd C:\Users\DELL\OneDrive\Desktop\SnapGit
.\build.bat
```

- For debugging in an IDE: import the folder into Visual Studio or VS Code with C/C++ extension.
- Run small manual tests after changes:
  - `mygit.exe init`
  - create a file, `mygit.exe add <file>`
  - `mygit.exe commit -m "msg"`
  - inspect `.mygit/objects` and `.mygit/refs` to verify changes

---

**7. Coding conventions & PR expectations**
- Keep diffs focused and small.
- Build must succeed locally.
- Include manual test steps in the PR description.
- Document any public API changes in `Readme.md` or `CONTRIBUTING.md`.

---

**8. Debugging tips / common pitfalls**
- If commits don't appear under `.mygit/objects`, check hashing code in `Helper/Hash/Hash.cpp` and the object writer.
- If branches don't move, inspect `Helper/UpdateHead/UpdateHead.cpp` and `Helper/GetHeadRef/GetHeadRef.cpp`.
- If index handling is inconsistent, inspect `Helper/ReadIndex/ReadIndex.cpp` and `Helper/SortIndex/SortIndex.cpp`.

---

**9. Starter issues (good first tasks)**
- Add a `LearningHelper` skeleton and a CLI flag to enable it.
- Improve user-facing messages for `merge` conflicts with clearer paths to conflict markers in files.
- Add an example walkthrough (a `docs/tutorial.md`) that shows step-by-step commands and the exact `.mygit` files to open.
- Add unit-like manual tests: small scripts that run a sequence of commands and assert `.mygit` files existence/contents (shell scripts or PowerShell scripts).

---

If you'd like, I can implement the `LearningHelper` skeleton and add the CLI toggle in `main.cpp` as a small PR so contributors can iterate on the explanations. Want me to implement that now?

````
