echo off

set app_path=C:\Deepak\workspace\Symbian\ripplevault
set sym6_sdk=c:\Symbian\6.1\Series60\Epoc32\Release\armi\urel
set deploy_path=%app_path%\deployment\sym6_wap

@echo ######### Building S60 6.1 #############
set EPOCROOT=\Symbian\6.1\Series60\

@echo ######### Cleaning up first... #########

del %sym6_sdk%\ripplevault.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Ripple Vault... #########

cd %app_path%\group_sym6_wap

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym6_sdk%\ripplevault.app %deploy_path%\
copy %sym6_sdk%\ripplevault.r01 %deploy_path%\
copy %sym6_sdk%\ripplevault.r31 %deploy_path%\

@echo ######### DONE ############

@pause
