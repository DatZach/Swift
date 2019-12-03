/*
 *  Compiler.hpp
 *  Compiler Abstraction
 */

#ifndef __COMPILER_HPP
#define __COMPILER_HPP

#include <QString>
#include <QProcess>
#include <QDir>

#define Compiler CompilerSingleton::GetInstance()

class CompilerSingleton
{
private:
    QDir compilerPath;
    QString workingDirectory;

public:
    CompilerSingleton();

    bool Discover();
    QProcess* ExecuteThreaded(const QStringList& arguments, const QString& overrideWorkingDirectory = "");
    QByteArray Execute(const QStringList& arguments, const QString& overrideWorkingDirectory = "");
    QByteArray Execute(const QString& command, const QString& overrideWorkingDirectory = "");

    static CompilerSingleton& GetInstance();
};

#endif
