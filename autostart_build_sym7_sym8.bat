echo off

set app_path=C:\Deepak\workspace\Symbian
set sym_sdk=c:\Symbian\7.0s\Series60_v20\Epoc32\Release\armi\urel
set deploy_path=%app_path%\ripplevault\deployment\sym7
set deploy_path1=%app_path%\ripplevault\deployment\sym8

@echo ######### Building S60 7.0s #############
set EPOCROOT=\Symbian\7.0s\Series60_v20\

@echo ######### Cleaning up first... #########

del %sym_sdk%\vl_autostart.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\Autostart\group_s60

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\vl_autostart.mdl %deploy_path%\
copy %sym_sdk%\vl_autostart.mdl %deploy_path1%\

@echo ######### DONE ############

@pause
