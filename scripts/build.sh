#! /bin/bash

echo "=================================开始构建================================="
if [ -d "../output/" ];then
  rm -r ../output
fi
mkdir ../output

if [ -d "../tmp/" ];then
  rm -r ../tmp
fi
mkdir ../tmp
cd ../tmp
cmake ../
make
mv ./MetamorphicTestFramework ../output
cd ..
rm -r ./tmp
echo "=================================构建完成================================="
