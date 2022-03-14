
xcopy	/y	.\Engine\public\*.h				.\Reference\inc\
xcopy	/y	.\Engine\bin\Engine.lib			.\Reference\lib\
xcopy	/y	.\Engine\bin\Engine.dll			.\Client\bin\
xcopy	/y	.\Engine\bin\Engine.dll			.\MFCTool\bin\
xcopy	/y	.\MFCTool\bin\ShaderFiles\*.*	.\Client\bin\ShaderFiles\*.*
xcopy	/y	.\Engine\bin\ShaderFiles\*.*	.\MFCTool\bin\ShaderFiles\*.*
xcopy	/y	.\Engine\bin\ShaderFiles\*.*	.\Client\bin\ShaderFiles\*.*
