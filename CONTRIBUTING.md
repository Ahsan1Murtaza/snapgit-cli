# Contributing to SnapGit

Thanks for your interest in contributing! This document helps new contributors set up a development environment, explains the code layout, and gives a suggested approach for implementing the planned Learning Mode.

**Quick checklist**
- Fork the repository and create a feature branch.
- Make small, focused changes and open a pull request with a clear description.
- Run local builds and provide reproduction steps for any bugs you fix.

---

**Local development setup (Windows)**
- Install a C++ toolchain: Visual Studio (with C++), or Mingw/MSYS for `g++`.
- Open PowerShell and run:

```powershell
cd <path-to-cloned-repo>
.\build.bat
```

- If you prefer manual compilation use the `g++` line in `runInstructions.txt` or in the `Readme.md`.

**Helpful tools**
- A good editor: Visual Studio Code or Visual Studio
- Optional: `clang-format` for consistent formatting. We prefer reasonable, clear C++ style; keep functions short and avoid global state where possible.

---

**Project structure**
- `main.cpp` — CLI entry and dispatcher
- `CommandHandlers/` — separate folder per command; each handler contains `.cpp` and `.h`
- `Helper/` — utility functions (hashing, reading/writing index, HEAD updates)
- `Objects/` — `Blob`, `Tree`, `Commit` implementations

When changing behavior for a command, prefer editing the corresponding handler in `CommandHandlers/<Name>Handler/`.

---

**Coding guidelines**
- Keep functions short and focused.
- Use descriptive names for functions and variables.
- Avoid changing unrelated files in a single PR.
- No external dependencies beyond common system libraries (`-lssl -lcrypto` may be required for hashing). Document additional dependencies in your PR.

**Commit message style**
- Short subject (<= 72 chars), blank line, longer description if needed. Example:

```
Add LearningHelper skeleton and hook in main dispatcher

This adds a new Helper/LearningHelper.* and calls Explain() after command dispatch.
```

---

**Testing changes**
- There is no automated test harness included yet. Test manually by building with `build.bat` and exercising commands. Provide step-by-step repro steps in PR descriptions so the maintainers can verify.

**Suggested PR checklist**
- Build succeeds locally with `build.bat`.
- New behavior covered by a short manual test and reproduction steps.
- Code compiles without warnings where feasible.
- Update `Readme.md` or add a short note if the user-visible commands or behaviors change.

---

Implementing Learning Mode — suggested approach

Goal: when enabled, after each command the CLI prints a short explanation and points to the exact files in `.mygit` that illustrate what changed.

Suggested design:
- Add `Helper/LearningHelper.h` and `Helper/LearningHelper.cpp`.
- Define a small API: `class LearningHelper { public: static void Explain(const std::string &cmd, const Context &ctx); }` where `Context` is a minimal struct with fields like `commitHash`, `affectedPaths`, `branchName`.
- Add a small toggle in the CLI: a global flag `bool learningMode = false;` that can be set by a CLI option (e.g., `--learn` or `learn on`).
- After the command handler finishes (successfully), call `LearningHelper::Explain(cmd, ctx)` from `main.cpp` or the dispatcher so the logic is centralized.

Example `Explain()` behavior:
- For `commit`: print the new commit hash and the path to the commit object: `.mygit/objects/<hash>`; show which tree it points to and where the branch pointer was updated (`.mygit/refs/heads/<branch>`).
- For `add`: show the updated `.mygit/index` and which files were staged.
- For `branch`: show the newly created file under `.mygit/refs/heads/<branch-name>`.

Design notes:
- Keep `LearningHelper` read-only: it should not change repository state, only inspect `.mygit` and print instructions.
- Keep explanations concise and actionable — a few short lines with filenames and what they mean is better than long essays.

Example printout for `commit` when learning mode is enabled:

```
Committed as: 3a1b2c4...20
See .mygit/objects/3a/1b2c4...20   -> the commit object (message, author, tree)
See .mygit/objects/2d/1v2fa...20 -> the snapshot of files at commit
See .mygit/refs/heads/main   -> branch now points to this commit
```

---

Thank you for helping make SnapGit easier to learn and extend!
