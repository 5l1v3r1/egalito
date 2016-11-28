#ifndef EGALITO_CHUNK_INSTRUCTION_H
#define EGALITO_CHUNK_INSTRUCTION_H

#include <string>
#include <capstone/capstone.h>  // for cs_insn
#include "types.h"

class Instruction;
class Link;

/** Abstract base class for special instruction data.
*/
class InstructionSemantic {
public:
    virtual ~InstructionSemantic() {}

    virtual size_t getSize() const = 0;
    virtual void setSize(size_t value) = 0;

    virtual Link *getLink() const = 0;
    virtual void setLink(Link *newLink) = 0;

    virtual void writeTo(char *target) = 0;
    virtual void writeTo(std::string &target) = 0;
    virtual std::string getData() = 0;
};

#if 0
class Storage {
public:
    virtual ~Storage() {}

    virtual size_t getSize() const = 0;

    virtual void writeTo(char *target) = 0;
    virtual void writeTo(std::string &target) = 0;
    virtual std::string getData() const = 0;
};

class RawByteStorage : public Storage {
private:
    std::string rawData;
public:
    RawByteStorage(const std::string &rawData) : rawData(rawData) {}

    virtual size_t getSize() const { return rawData.size(); }

    virtual void writeTo(char *target);
    virtual void writeTo(std::string &target);
    virtual std::string getData() const;
};

class DisassembledStorage : public Storage {
private:
    cs_insn insn;
public:
    DisassembledStorage(const cs_insn &insn) : insn(insn) {}

    virtual size_t getSize() const { return insn.size; }

    virtual void writeTo(char *target);
    virtual void writeTo(std::string &target);
    virtual std::string getData() const;
};
#else
class Storage {
public:
    virtual ~Storage() {}

    virtual size_t getSize() const = 0;

    virtual void writeTo(char *target) = 0;
    virtual void writeTo(std::string &target) = 0;
    virtual std::string getData() = 0;
};

class RawByteStorage : public Storage {
private:
    std::string rawData;
public:
    RawByteStorage(const std::string &rawData) : rawData(rawData) {}

    size_t getSize() const { return rawData.size(); }

    void writeTo(char *target);
    void writeTo(std::string &target);
    std::string getData();
};

class DisassembledStorage : public Storage {
private:
    cs_insn insn;
public:
    DisassembledStorage(const cs_insn &insn) : insn(insn) {}

    size_t getSize() const { return insn.size; }

    void writeTo(char *target);
    void writeTo(std::string &target);
    std::string getData();
};
#endif

template <typename Storage>
class SemanticImpl : public InstructionSemantic {
private:
    Storage storage;
public:
    SemanticImpl(const Storage &storage) : storage(storage) {}

    virtual size_t getSize() const { return storage.getSize(); }
    virtual void setSize(size_t value)
        { throw "Can't set size for this instruction type!"; }

    virtual Link *getLink() const { return nullptr; }
    virtual void setLink(Link *newLink)
        { throw "Can't set link for this instruction type!"; }

    virtual void writeTo(char *target) { storage.writeTo(target); }
    virtual void writeTo(std::string &target) { storage.writeTo(target); }
    virtual std::string getData() { return storage.getData(); }
};

template <typename BaseType>
class LinkDecorator : public BaseType {
private:
    Link *link;
public:
    LinkDecorator() : link(nullptr) {}

    virtual Link *getLink() const { return link; }
    virtual void setLink(Link *link) { this->link = link; }
};

// --- concrete classes follow ---

typedef SemanticImpl<RawByteStorage> RawInstruction;
typedef SemanticImpl<DisassembledStorage> DisassembledInstruction;

class ControlFlowInstruction : public LinkDecorator<InstructionSemantic> {
private:
    Instruction *source;
    std::string opcode;
    int displacementSize;
public:
    ControlFlowInstruction(Instruction *source)
        : source(source), displacementSize(0) {}

    virtual size_t getSize() const { return opcode.size() + displacementSize; }
    virtual void setSize(size_t value);

    virtual void writeTo(char *target);
    virtual void writeTo(std::string &target);
    virtual std::string getData();
private:
    Instruction *getSource() const { return source; }
    diff_t calculateDisplacement();
};

#endif