echo off

set app_path=C:\Deepak\workspace\Symbian
set sym_sdk=c:\Symbian\uiq_21\Epoc32\Release\armi\urel
set deploy_path=%app_path%\ripplevault\deployment\uiq

@echo ######### Building UIQ #############
set EPOCROOT=\Symbian\UIQ_21\

@echo ######### Cleaning up first... #########

del %sym_sdk%\starter.exe

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\starter\group

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\starter.exe %deploy_path%\

@echo ######### DONE ############

@pause
