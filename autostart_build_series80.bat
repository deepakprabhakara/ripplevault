echo off

set app_path=C:\Deepak\workspace\Symbian
set sym_sdk=c:\Symbian\7.0s\S80_DP2_0_SDK\Epoc32\Release\armi\urel
set deploy_path=%app_path%\ripplevault\deployment\series80

@echo ######### Building UIQ #############
set EPOCROOT=\Symbian\7.0s\S80_DP2_0_SDK\

@echo ######### Cleaning up first... #########

del %sym_sdk%\vl_autostart.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\Autostart\group_uiq

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\vl_autostart.mdl %deploy_path%\

@echo ######### DONE ############

@pause
