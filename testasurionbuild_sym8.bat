echo off

set app_path=C:\Deepak\workspace\Symbian\Asurion
set sym_sdk=c:\Symbian\7.0s\Series60_v20\Epoc32\Release\armi\urel
set deploy_path=%app_path%\deployment\sym8

@echo ######### Building S60 8.0s #############
set EPOCROOT=\Symbian\7.0s\Series60_v20\

@echo ######### Cleaning up first... #########

del %sym_sdk%\ripplevault.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\group_sym8

call bldmake bldfiles

@echo call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym_sdk%\ripplevault.app %deploy_path%\
copy %sym_sdk%\ripplevault.r01 %deploy_path%\
copy %sym_sdk%\ripplevault.r31 %deploy_path%\

@echo ######### DONE ############

@pause
