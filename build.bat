@echo off
echo Building mygit CLI...
g++ .\main.cpp ^
.\CommandHandlers\AddHandler\AddHandler.cpp ^
.\CommandHandlers\InitHandler\InitHandler.cpp ^
.\CommandHandlers\CommitHandler\CommitHandler.cpp ^
.\CommandHandlers\ConfigHandler\ConfigHandler.cpp ^
.\CommandHandlers\BranchHandler\BranchHandler.cpp ^
.\CommandHandlers\LogHandler\LogHandler.cpp ^
.\CommandHandlers\StatusHandler\StatusHandler.cpp ^
.\CommandHandlers\CheckoutHandler\CheckoutHandler.cpp ^
.\Helper\GetAllCommits\GetAllCommits.cpp ^
.\Helper\Hash\Hash.cpp ^
.\Helper\RepoCheck\RepoCheck.cpp ^
.\Helper\SortIndex\SortIndex.cpp ^
.\Helper\GetCurrentCommitHash\GetCurrentCommitHash.cpp ^
.\Helper\UpdateHead\UpdateHead.cpp ^
.\Helper\GetHeadRef\GetHeadRef.cpp ^
.\Helper\GetUserInfo\GetUserInfo.cpp ^
.\Helper\GetAllBranches\GetAllBranches.cpp ^
.\Helper\ReadCommit\ReadCommit.cpp ^
.\Helper\RestoreTree\RestoreTree.cpp ^
.\Objects\Blob\Blob.cpp .\Objects\Tree\Tree.cpp .\Objects\Commit\Commit.cpp -o mygit.exe -static -lssl -lcrypto -lcrypt32 -lws2_32
echo.
echo Build complete! Run mygit.exe to test.
pause