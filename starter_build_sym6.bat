echo off

set app_path=C:\Deepak\workspace\Symbian
set sym6_sdk=c:\Symbian\6.1\Series60\Epoc32\Release\armi\urel
set deploy_path=%app_path%\ripplevault\deployment\sym6

@echo ######### Building S60 6.1 #############
set EPOCROOT=\Symbian\6.1\Series60\

@echo ######### Cleaning up first... #########

del %sym6_sdk%\starter.exe

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\starter\group

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym6_sdk%\starter.exe %deploy_path%\

@echo ######### DONE ############

@pause
