@echo off
echo Building mygit CLI...

g++ .\main.cpp .\CommandHandlers\AddHandler\AddHandler.cpp .\CommandHandlers\InitHandler\InitHandler.cpp .\CommandHandlers\CommitHandler\CommitHandler.cpp .\Helper\Hash\Hash.cpp .\Objects\Blob\Blob.cpp .\Objects\Tree\Tree.cpp .\Objects\Commit\Commit.cpp .\Helper\RepoCheck\RepoCheck.cpp .\Helper\SortIndex\SortIndex.cpp .\Helper\GetCurrentCommitHash\GetCurrentCommitHash.cpp .\Helper\UpdateHead\UpdateHead.cpp .\Helper\GetHeadRef\GetHeadRef.cpp .\CommandHandlers\ConfigHandler\ConfigHandler.cpp .\Helper\GetUserInfo\GetUserInfo.cpp .\CommandHandlers\BranchHandler\BranchHandler.cpp .\Helper\GetAllBranches\GetAllBranches.cpp -o mygit.exe -lssl -lcrypto

echo.
echo Build complete! Run mygit.exe to test.
pause