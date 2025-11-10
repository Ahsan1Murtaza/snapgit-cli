### **🧩 SnapGit - A Mini Version Control System**

SnapGit is a simplified reimplementation of Git, written in C++.

### Supporting:  

  `mygit init`  
  `mygit add`  
  `mygit commit`  
  `mygit config`  
  `mygit log`  
  `mygit reset`  
  `mygit branch`  
  `mygit checkout`  
   Merge and Status (coming soon)


### Setup

1. **Clone the Repository**  
   `git clone https://github.com/Ahsan1Murtaza/SnapGit.git`  
   `cd SnapGit`

2. **Build (Windows)**  
   `build.bat`  
   That creates a file `mygit.exe`

3. **Add to PATH**  
   Add the folder containing `mygit.exe` to system environment variables.


### Now run anywhere:  

`mygit init`  
`mygit add <file>`  
`mygit commit -m "<message>"`  
`mygit config` or `mygit config user.name <name>` or `mygit config user.email <email>`
`mygit reset --hard <commitHash>`
`mygit branch` or `mygit branch <branchName>`
`mygit log`
`mygit checkout <branchName>` or `mygit checkout <commitHash>`
`mygit --help`
