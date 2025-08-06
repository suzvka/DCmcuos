### 项目依赖：

* \[qmsetup](https://github.com/stdware/qmsetup)
* \[syscmdline](https://github.com/SineStriker/syscmdline)
* \[etl](https://github.com/ETLCPP/etl)



### 快速启动(Windows适用)：



* 在项目根目录(本文件所在目录)中 Shift + 右键 打开 PowerShell；
* 执行：git clone https://github.com/microsoft/vcpkg.git；
* 执行：cd vcpkg；
* 执行：bootstrap-vcpkg.bat；
* 执行：mkdir C:\\TEMPDATA；
* 执行：$env:TEMP ="C:\\TEMPDATA"；
* 执行：$env:TMP ="C:\\TEMPDATA"；
* 执行：vcpkg install --x-manifest-root=../scripts/vcpkg-manifest --x-install-root=./installed；
* 将 IDE 导航至根目录打开；
