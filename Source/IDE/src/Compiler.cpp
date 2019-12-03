/*
 *  Compiler.cpp
 *  Compiler Abstraction
 */

//#include <QApplication>
#include <Compiler.hpp>

#define SWIFT_COMPILER_DEV_PATH         "X:\\Swift\\Source\\Compiler\\bin\\windows\\Release\\"
#define SWIFT_COMPILER_FILENAME         "swift.exe"

CompilerSingleton::CompilerSingleton()
    : compilerPath("."),
      workingDirectory(".")
{

}

bool CompilerSingleton::Discover()
{
    compilerPath.setPath(".");
    if (compilerPath.exists(SWIFT_COMPILER_FILENAME))
        return true;

    compilerPath.setPath(SWIFT_COMPILER_DEV_PATH);
    return compilerPath.exists(SWIFT_COMPILER_FILENAME);
}

QProcess* CompilerSingleton::ExecuteThreaded(const QStringList& arguments,
                                              const QString& overrideWorkingDirectory)
{
    QProcess* compilerProcess = new QProcess();
    compilerProcess->setWorkingDirectory(overrideWorkingDirectory == ""
                                            ? workingDirectory
                                            : overrideWorkingDirectory);
    compilerProcess->start(compilerPath.absoluteFilePath(SWIFT_COMPILER_FILENAME), arguments);

    return compilerProcess;
}

QByteArray CompilerSingleton::Execute(const QStringList& arguments, const QString& overrideWorkingDirectory)
{
    QProcess compilerProcess;
    compilerProcess.setWorkingDirectory(overrideWorkingDirectory == ""
                                            ? workingDirectory
                                            : overrideWorkingDirectory);
    compilerProcess.start(compilerPath.absoluteFilePath(SWIFT_COMPILER_FILENAME), arguments);
    compilerProcess.waitForFinished();

    return compilerProcess.readAll();
}

QByteArray CompilerSingleton::Execute(const QString& command, const QString& overrideWorkingDirectory)
{
    QStringList arguments;
    arguments << command;

    return Execute(arguments, overrideWorkingDirectory);
}

CompilerSingleton& CompilerSingleton::GetInstance()
{
    static CompilerSingleton instance;
    return instance;
}
