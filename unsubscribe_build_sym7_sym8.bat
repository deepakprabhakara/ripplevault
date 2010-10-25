echo off

set app_path=C:\Deepak\workspace\Symbian\Asurion
set sym_sdk=c:\Symbian\7.0s\Series60_v20\Epoc32\Release\armi\urel
set deploy_path=%app_path%\deployment\sym7

@echo ######### Building S60 7.0s #############
set EPOCROOT=\Symbian\7.0s\Series60_v20\

@echo ######### Cleaning up first... #########

del %sym_sdk%\unsubscribe.exe

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\unsubscribe\group

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\unsubscribe.exe %deploy_path%\

@echo ######### DONE ############

@pause
