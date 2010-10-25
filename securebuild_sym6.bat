echo off

set app_path=C:\Deepak\workspace\Symbian\ripplevault\Secure
set sym6_sdk=c:\Symbian\6.1\Series60\Epoc32\Release\armi\urel
set deploy_path=C:\Deepak\workspace\Symbian\ripplevault\deployment\sym6
set deploy_path1=C:\Deepak\workspace\Symbian\ripplevault\deployment\sym6_wap

@echo ######### Building S60 6.1 #############
set EPOCROOT=\Symbian\6.1\Series60\

@echo ######### Cleaning up first... #########

del %sym6_sdk%\secure.*

@echo ######### Cleaning DONE   ##############

@echo ######### Building Secure... #########

cd %app_path%\group_sym6

call bldmake bldfiles

call abld clean armi urel

call abld build armi urel

@echo ######### Building DONE, copying to deployment folder   ##############

copy %sym6_sdk%\secure.app %deploy_path%\
copy %sym6_sdk%\secure.r01 %deploy_path%\
copy %sym6_sdk%\secure.app %deploy_path1%\
copy %sym6_sdk%\secure.r01 %deploy_path1%\

@echo ######### DONE ############

@pause
