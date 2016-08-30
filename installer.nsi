;Furqan Installer
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  
;--------------------------------
;General

  ;Name and file
  Name "Furqan"
  OutFile "Furqan v1.0.0.exe"

  ;Default installation folder
  InstallDir "D:\Furqan"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\ShamSoft\Furqan" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\ShamSoft\Furqan" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "license.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "ARABIC" ;first language is the default language
  !insertmacro MUI_LANGUAGE "ENGLISH"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

Section Sec

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  
  CreateDirectory $INSTDIR\intl
  SetOutPath $INSTDIR\intl
  File "intl\fbintl.conf"
  File "intl\fbintl.dll"

  CreateDirectory $INSTDIR\platforms
  SetOutPath $INSTDIR\platforms
  File "platforms\qwindows.dll"

  CreateDirectory $INSTDIR\sqldrivers
  SetOutPath $INSTDIR\sqldrivers
  File "sqldrivers\qsqlibase.dll"
 
  CreateDirectory $INSTDIR\printsupport
  SetOutPath $INSTDIR\printsupport
  File "printsupport\windowsprintersupport.dll"
 
  SetOutPath $INSTDIR
  
  ; Put files there

  File "Furqan.exe"
  File "ar.qm"
  File "en.qm"
  File "license.txt"
  File "data.fdb"
  File "fbclient.dll"
  File "firebird.msg"
  File "ib_util.dll"
  File "icudt30.dll"
  File "icudt52.dll"
  File "icuin30.dll"
  File "icuin52.dll"
  File "icuuc30.dll"
  File "icuuc52.dll"
  File "libgcc_s_dw2-1.dll"
  File "libGLESv2.dll"
  File "libstdc++-6.dll"
  File "libwinpthread-1.dll"
  File "Microsoft.VC80.CRT.manifest"
  File "msvcp80.dll"
  File "msvcr80.dll"
  File "Qt5Core.dll" 
  File "Qt5Gui.dll" 
  File "Qt5PrintSupport.dll" 
  File "Qt5Sql.dll" 
  File "Qt5Widgets.dll" 

  ;Store installation folder
  WriteRegStr HKCU "Software\ShamSoft\Furqan" "" $INSTDIR
  
  ;Add an entry to Add/Remove Programs.
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan" "DisplayName" "Furqan v1.0.0"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan" "NoRepair" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan" "VersionMajor" 1 
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan" "VersionMinor" 0
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"

  CreateDirectory "$SMPROGRAMS\Furqan"
  CreateShortCut "$SMPROGRAMS\Furqan\uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\Furqan\Furqan v1.0.0.lnk" "$INSTDIR\Furqan.exe" "" "$INSTDIR\Furqan.exe" 0
  CreateShortCut "$DESKTOP\Furqan v1.0.0.lnk" "$INSTDIR\Furqan.exe" "" "$INSTDIR\Furqan.exe" 0

SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ; Deleting the program's files and directories

  RMDir /r "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\ShamSoft\Furqan"

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Furqan\*.*"
  Delete "$DESKTOP\Furqan v1.0.0.lnk"

  ; Remove the program's entry from add/remove programs.
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Furqan"
  
SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd