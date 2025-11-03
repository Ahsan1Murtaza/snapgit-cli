@echo off
echo Building mygit CLI...

g++ -std=c++17 ^
 .\main.cpp ^
 .\CommandHandlers\AddHandler\AddHandler.cpp ^
 .\CommandHandlers\InitHandler\InitHandler.cpp ^
 .\CommandHandlers\CommitHandler\CommitHandler.cpp ^
 .\CommandHandlers\ConfigHandler\ConfigHandler.cpp ^
 .\CommandHandlers\BranchHandler\BranchHandler.cpp ^
 .\CommandHandlers\StatusHandler\StatusHandler.cpp ^
 .\CommandHandlers\LogHandler\LogHandler.cpp ^
 .\Helper\Hash\Hash.cpp ^
 .\Helper\RepoCheck\RepoCheck.cpp ^
 .\Helper\SortIndex\SortIndex.cpp ^
 .\Helper\GetCurrentCommitHash\GetCurrentCommitHash.cpp ^
 .\Helper\UpdateHead\UpdateHead.cpp ^
 .\Helper\GetHeadRef\GetHeadRef.cpp ^
 .\Helper\GetUserInfo\GetUserInfo.cpp ^
 .\Helper\GetAllBranches\GetAllBranches.cpp ^
 .\Objects\Blob\Blob.cpp ^
 .\Objects\Tree\Tree.cpp ^
 .\Objects\Commit\Commit.cpp ^
 -o mygit.exe -lssl -lcrypto

echo.
echo Build complete! Run mygit.exe to test.
pause
