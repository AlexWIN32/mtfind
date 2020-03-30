/*******************************************************************************
    Author: Alexey Frolov (alexwin32@mail.ru)

    This software is distributed freely under the terms of the MIT License.
    See "LICENSE" or "http://copyfree.org/content/standard/licenses/mit/license.txt".
*******************************************************************************/

#pragma once
#include <cstdio>
#include <string>
#include <stdint.h>
#include <Exception.h>
#ifdef __linux__
#include <Utils/ToString.h>
#endif

namespace Utils
{

class DefaultSerialisingStrategy
{
public:
    template<class TVar>
    static void Write(FILE *File, const std::wstring &Path, const TVar &Var)
    {
        if(fwrite(&Var, sizeof(Var), 1, File) != 1)
            throw IOException(L"cant write to file " + Path);
    }
    template<class TVar>
    static void WriteArray(FILE *File, const TVar *Array, size_t ArraySize, const std::wstring &Path)
    {
        if(fwrite(Array, sizeof(TVar), ArraySize, File) != ArraySize)
            throw IOException(L"cant write to file " + Path);
    }
    template<class TVar>
    static size_t ReadArray(FILE *File, TVar *OutArray, size_t ArraySize, const std::wstring &Path, bool &Eof)
    {
        size_t elemsReaded = fread(OutArray, sizeof(TVar), ArraySize, File);
        if(elemsReaded != ArraySize && !feof(File))
            throw IOException(L"cant read from file " + Path);

        Eof = feof(File) != 0;

        return elemsReaded;
    }
    template<class TVar>
    static TVar Read(FILE *File, const std::wstring &Path, bool &Eof)
    {
        TVar var;
        if(fread(&var, sizeof(TVar), 1, File) != 1 && !feof(File))
            throw IOException(L"cant read from file " + Path);

        Eof = feof(File) != 0;

        return var;
    }
    template<class TContainer, class TVar>
    static TContainer ReadUntil(FILE *File,
                                const std::wstring &Path,
                                bool &Eof,
                                const TVar &TerminateElement)
    {
        TContainer container;

        while(true){

            TVar var = Read<TVar>(File, Path, Eof);

            if(Eof)
                break;

            if(var == TerminateElement)
                break;
            else
                container.push_back(var);
        }

        return container;
    }
};

template<class TStrategy>
class BasicFileGuard final
{
private:
    FILE *file = nullptr;
    bool eof = false;
    std::wstring path;
public:
    BasicFileGuard(const BasicFileGuard &) = delete;
    BasicFileGuard &operator= (const BasicFileGuard &) = delete;
    BasicFileGuard(FILE *File, const std::wstring &Path) : file(File), path(Path){}
    BasicFileGuard(const std::wstring &Path, const std::wstring &Mode)
    {
#ifdef __linux__
        std::string asciiPath = Utils::ToString(Path);
        std::string asciiMode = Utils::ToString(Mode);
        
	file = fopen(asciiPath.c_str(), asciiMode.c_str());
	if(file == nullptr)
#else
        errno_t res = _wfopen_s(&file, Path.c_str(), Mode.c_str());
        if(res != 0)
#endif
           throw IOException(L"Cant open file " + Path);

        path = Path;
    }
    FILE *get() const {return file;}
    ~BasicFileGuard() { if(file) fclose(file);}
    operator const FILE *() const {return file;}
    operator FILE *() {return file;}
    template<class TVar>
    TVar Read()
    {
        return TStrategy::template Read<TVar>(file, path, eof);
    }
    template<class TVar>
    size_t ReadArray(TVar *OutArray, size_t ArraySize)
    {
        return TStrategy::template ReadArray<TVar>(file, OutArray, ArraySize, path, eof);
    }
    template<class TContainer, class TVar>
    TContainer ReadUntil(const TVar &TerminateElement)
    {
        return TStrategy::template ReadUntil<TContainer>(file, path, eof, TerminateElement);
    }
    template<class TVar>
    void Write(const TVar &Var)
    {
        TStrategy::template Write<TVar>(file, path, Var);
    }
    template<class TVar>
    void WriteArray(const TVar *Array, size_t ArraySize)
    {
        TStrategy::template WriteArray<TVar>(file, Array, ArraySize, path);
    }
    bool Eof() const {return eof;}
    size_t Size() const 
    {
        fseek(file, 0, SEEK_END);
        size_t size = ftell(file);
        rewind(file);

        return size;
    }
    void SetPos(size_t NewPos)
    {
        if(fseek(file, NewPos, SEEK_SET) != 0)
            throw IOException(L"Cant set pos in file " + path);
    }
    const std::wstring &GetPath() const
    {
        return path;
    }
};

typedef BasicFileGuard<DefaultSerialisingStrategy> FileGuard;
}
