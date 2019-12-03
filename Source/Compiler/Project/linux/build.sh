dmcs *.cs */*.cs -r References/*.dll -out:bin/linux/swift.exe
mv bin/linux/swift.exe bin/linux/swift
cp References/*.dll bin/linux
python ../../Tools/CompilerPostBuild.py $(pwd)
