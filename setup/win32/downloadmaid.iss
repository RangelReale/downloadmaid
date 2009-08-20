[Setup]
AppName=DownloadMaid
AppVerName=DownloadMaid 0.1
DefaultDirName={pf}\DownloadMaid
DefaultGroupName=DownloadMaid
SourceDir=..\..
OutputBaseFilename=downloadmaid_0.1_win32
OutputDir=setup\win32
ChangesAssociations=yes
VersionInfoVersion=0.1.0.2
UninstallDisplayIcon={app}\downloadmaid.exe
UninstallDisplayName=DownloadMaid
;AlwaysRestart=yes

[Registry]
Root: HKCU; Subkey: "Software\SIB IT"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\SIB IT\DownloadMaid"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\SIB IT"; Flags: uninsdeletekeyifempty
Root: HKLM; Subkey: "Software\SIB IT\DownloadMaid"; Flags: uninsdeletekey
Root: HKLM; Subkey: "Software\SIB IT\DownloadMaid"; ValueType: string; ValueName: "InstallPath"; ValueData: "{app}"

[Files]
Source: "bin\Win32Release\DownloadMaid.exe"; DestDir: "{app}"
Source: "setup\win32\redist\vcredist_x86.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall
Source: "M:\prog\src\QT\2009.03\qt\lib\QtCore4.dll"; DestDir: "{app}"
Source: "M:\prog\src\QT\2009.03\qt\lib\QtGui4.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\DownloadMaid"; Filename: "{app}\DownloadMaid.exe"; WorkingDir: "{app}"
Name: "{group}\Uninstall DownloadMaid"; Filename: "{uninstallexe}"

[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/q"; Description: "Visual Studio Redistributable"; StatusMsg: "Installing Visual Studio Redistributable..."

