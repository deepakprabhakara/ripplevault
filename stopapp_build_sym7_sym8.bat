echo off

set app_path=C:\Deepak\workspace\Symbian\Vodacom
set sym_sdk=c:\Symbian\7.0s\Series60_v20\Epoc32\Release\armi\urel
set deploy_path=%app_path%\deployment\sym7
set deploy_path1=%app_path%\deployment\sym8

@echo ######### Building S60 7.0s #############
set EPOCROOT=\Symbian\7.0s\Series60_v20\

@echo ######### Cleaning up first... #########

del %sym_sdk%\vault.exe

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\stopapp\group

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\vault.exe %deploy_path%\vault.ex
copy %sym_sdk%\vault.exe %deploy_path1%\vault.ex

@echo ######### DONE ############

@pause
