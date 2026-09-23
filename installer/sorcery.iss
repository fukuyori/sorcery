#ifndef PayloadDir
  #error PayloadDir must identify a staged Release payload.
#endif
#ifndef AppVersion
  #error AppVersion must be supplied by build-installer.ps1.
#endif

[Setup]
AppId={{0535c4ba-1011-4e9e-9670-abb4dc1705ae}
AppName=Sorcery Japanese Edition
AppVersion={#AppVersion}
AppPublisher=fukuyori
AppPublisherURL=https://github.com/fukuyori/sorcery
DefaultDirName={autopf}\Sorcery Japanese Edition
DefaultGroupName=Sorcery Japanese Edition
UninstallDisplayIcon={app}\sorcery.exe
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputBaseFilename=Sorcery-JA-{#AppVersion}-Setup
Compression=lzma2
SolidCompression=yes
#ifdef SignBuild
SignTool=sorcerysign
SignedUninstaller=yes
#endif

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"

[Files]
Source: "{#PayloadDir}\sorcery.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PayloadDir}\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PayloadDir}\cfg\config.ini"; DestDir: "{app}\cfg"; Flags: ignoreversion
Source: "{#PayloadDir}\cfg\config.legacy-en.ini"; DestDir: "{app}\cfg"; Flags: ignoreversion
Source: "{#PayloadDir}\dat\*"; DestDir: "{app}\dat"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#PayloadDir}\doc\*"; DestDir: "{app}\doc"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#PayloadDir}\gfx\*"; DestDir: "{app}\gfx"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#PayloadDir}\sav\game.json"; DestDir: "{app}\sav"; Flags: ignoreversion
Source: "{#PayloadDir}\sfx\*"; DestDir: "{app}\sfx"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#PayloadDir}\vfx\*"; DestDir: "{app}\vfx"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#PayloadDir}\installed.flag"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\Sorcery Japanese Edition"; Filename: "{app}\sorcery.exe"; WorkingDir: "{app}"
