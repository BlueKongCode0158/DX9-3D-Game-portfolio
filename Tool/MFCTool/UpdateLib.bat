
xcopy	/y	.\Engine\public\*.h				.\Reference\inc\
xcopy	/y	.\Engine\bin\Engine.lib			.\Reference\lib\
xcopy	/y	.\Engine\bin\Engine.dll			.\Client\bin\
xcopy	/y	.\Engine\bin\Engine.dll			.\MFCTool\bin\
xcopy	/y	.\MFCTool\bin\ShaderFiles\*.*	.\Client\bin\ShaderFiles\*.*
xcopy	/y	.\Engine\bin\ShaderFiles\*.*	.\MFCTool\bin\ShaderFiles\*.*
xcopy	/y	.\Engine\bin\ShaderFiles\*.*	.\Client\bin\ShaderFiles\*.*
xcopy	/y	.\TinyXML\bin\TinyXML.lib		.\Reference\lib\
xcopy	/y	.\TinyXML\public\*.h			.\Reference\inc\
xcopy	/y	.\imgui\bin\Imgui.lib			.\Reference\lib\Imgui
xcopy	/y	.\imgui\public\*.h				.\Reference\inc\Imgui
