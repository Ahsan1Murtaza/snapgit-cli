; MyGit Installer Script
[Setup]
AppName=MyGit
AppVersion=1.0
DefaultDirName={pf}\MyGit
DefaultGroupName=MyGit
OutputDir=.
OutputBaseFilename=MyGitInstaller
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin

[Files]
; Main executable
Source: "mygit.exe"; DestDir: "{app}"; Flags: ignoreversion

; Required DLLs
Source: "libcrypto-3-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libssl-3-x64.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\MyGit"; Filename: "{app}\mygit.exe"

[Run]
; DO NOT run the CLI automatically after install – pointless
; Removed on purpose
[Registry]
; Add MyGit to PATH only if not already present
Root: HKLM; \
 Subkey: "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"; \
 ValueType: expandsz; \
 ValueName: "Path"; \
 ValueData: "{olddata};{app}"; \
 Flags: preservestringtype uninsdeletevalue

[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssPostInstall then
  begin
    // Tell Windows to refresh environment variables
    SendMessageTimeout(
      HWND_BROADCAST, WM_SETTINGCHANGE, 0,
      Integer(PChar('Environment')),
      SMTO_ABORTIFHUNG, 5000, nil);
  end;
end;