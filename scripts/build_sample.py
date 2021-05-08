import os
import shutil
import argparse


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Metamorphic Test Scaffolding")
    parser.add_argument("--name", help="Program Name")
    parser.add_argument("--T", help="Input Type")
    parser.add_argument("--U", help="Output Type")
    parser.add_argument("--n", type=int, help="MR Number")

    arg = parser.parse_args()
    kwargs = vars(arg)
    T = kwargs["T"]
    U = kwargs["U"]
    name = kwargs["name"]
    mrNum = kwargs["n"]
    path = "../src/"+name+"/"
    name = name[0].upper() + name[1:]

    if os.path.exists(path):
        shutil.rmtree(path)
    os.mkdir(path)

    gen = "\n\n#ifndef METAMORPHICTESTFRAMEWORK_{0}GEN_HPP\n" \
          "#define METAMORPHICTESTFRAMEWORK_{0}GEN_HPP\n\n" \
          "#include <memory>\n" \
          "#include \"GenSourceCase.h\"\n\n" \
          "using std::shared_ptr;\n\n" \
          "class {1}Gen : public metamorphicTestFramework::GenSourceCase<{2}> {{\n" \
          "public:\n" \
          "\t{2} genSourceCase() override {{\n" \
          "\t\t// 此处编写source case生成逻辑\n" \
          "\t\treturn;\n" \
          "\t}};\n\n" \
          "\t~{1}Gen() noexcept override = default;\n" \
          "}};\n\n\n" \
          "#endif //METAMORPHICTESTFRAMEWORK_{0}GEN_HPP\n\n".format(name.upper(), name, T, U)
    with open(path+name+"Gen.hpp", 'w') as f:
        f.write(gen)

    for i in range(mrNum):
        mr = "\n\n#ifndef METAMORPHICTESTFRAMEWORK_{0}MR{4}_HPP\n" \
             "#define METAMORPHICTESTFRAMEWORK_{0}MR{4}_HPP\n\n" \
             "#include <memory>\n" \
             "#include \"MR.h\"\n\n" \
             "using std::shared_ptr;\n\n" \
             "class {1}MR{4} : public metamorphicTestFramework::MR<{2}, {3}> {{\n" \
             "public:\n" \
             "\t{2} genFollowCase({2}& sourceCase, {3}& sourceCaseResult) override {{\n" \
             "\t\t// 此处编写follow case生成逻辑\n" \
             "\t\treturn;\n" \
             "\t}};\n\n" \
             "\tbool mrRelation({3} &sourceCaseResult, {3} &followCaseResult, {2}& sourceCase, {2}& followCase) override {{\n" \
             "\t\t// 此处编写mr验证逻辑\n" \
             "\t\treturn;\n" \
             "\t}};\n\n" \
             "\t~{1}MR{4}() noexcept override = default;\n\n" \
             "}};\n\n\n" \
             "#endif //METAMORPHICTESTFRAMEWORK_{0}MR{4}_HPP\n\n".format(name.upper(), name, T, U, i+1)
        with open(path+name+"MR"+str(i+1)+".hpp", 'w') as f:
            f.write(mr)

    program = "\n\n#ifndef METAMORPHICTESTFRAMEWORK_{0}PROGRAM_HPP\n" \
              "#define METAMORPHICTESTFRAMEWORK_{0}PROGRAM_HPP\n\n" \
              "#include <memory>\n" \
              "#include \"Program.h\"\n\n" \
              "using std::shared_ptr;\n\n" \
              "class {1}Program : public metamorphicTestFramework::Program<{2}, {3}> {{\n" \
              "public:\n" \
              "\t{3} genResult({2} &testCase) override {{\n" \
              "\t\t// 此处编写被测程序逻辑\n" \
              "\t\treturn;\n" \
              "\t}};\n\n" \
              "\t~{1}Program() noexcept override = default;\n\n" \
              "}};\n\n\n" \
              "#endif //METAMORPHICTESTFRAMEWORK_{0}PROGRAM_HPP\n\n".format(name.upper(), name, T, U)
    with open(path+name+"Program.hpp", 'w') as f:
        f.write(program)
